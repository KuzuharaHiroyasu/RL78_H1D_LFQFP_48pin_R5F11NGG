/**
 ****************************************************************************************
 *
 * @file		uart.c
 *
 * @brief UART Driver - UART0 of RL78 RENESAS 16b MCU.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2013-2016 Renesas Electronics Corporation
 *
 * $Rev: 2153 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup UART
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "r_cg_macrodriver.h"

#include "sys.h"
//#include "r_cg_serial.h"
#include "r_cg_sau.h"
#include "r_cg_userdefine.h"

#include "arch.h"
#include "types.h"
#include "uart.h"
#include "timer.h"

//#include "r_cg_serial.h"
#include "r_cg_sau.h"
/*
 * DEFINES
 ****************************************************************************************
 */
#if (SERIAL_U_DIV_2WIRE)
/* transmission request code */
#define UART_REQ_BYTE       (0xC0)
#define UART_REQ_BYTE_SIZE  (1)

/* transmission acknowledge code */
#define UART_ACK_BYTE       (0x88)

/* transmission retry count */
#define MAX_TX_TO_COUNT     (4)

/* 2-wire with branch connection state */
enum
{
    T_IDLE              = 0,
    T_REQUESTING        = 1,
    T_RCV_BF_REQUESTED  = 2,
    T_REQUESTED         = 3,
    T_ACTIVE            = 4
};
#endif

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
#if (SERIAL_U_DIV_2WIRE)
static const uint8_t g_uart_req_byte_buf = UART_REQ_BYTE;

static uint8_t  *g_sau_rx_buf;
static uint16_t g_sau_rx_size;

static uint8_t  g_sau_tx_stat;
static uint8_t  *g_sau_tx_buf;
static uint16_t g_sau_tx_size;

static uint8_t  g_sau_to_cnt;
#endif

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
extern void RSCIP_Uart_Tx_Done(void);       /* defined by rscip_uart.c */
extern void RSCIP_Uart_Rx_Done(void);       /* defined by rscip_uart.c */
extern void RSCIP_Uart_Rx_Error(void);      /* defined by rscip_uart.c */
extern bool RSCIP_Uart_Rx_Complete(void);   /* defined by rscip_uart.c */

static void uart_tx_timeout(void);

/**
 ****************************************************************************************
 * @brief Initialize UART communication.(including initializing DMA and interrupt) 
 *        If argument value is wrong, this function does nothing.
 *****************************************************************************************
 */
BOOL serial_init(void)
{
#if (SERIAL_U_DIV_2WIRE)
    g_sau_tx_stat = T_IDLE;
#endif

    return( TRUE );
}

void serial_exit( void )
{
}

/**
 ****************************************************************************************
 * @brief Start UART receiving data.
 * When finished receiving data, calls callback function.
 *
 * @param[in,out] bufptr  destination pointer, start of the space where data is transfered to.
 * @param[in] size        Transfer size, receive data size byte by UART.
 *****************************************************************************************
 */
BOOL serial_read(uint8_t *bufptr, const uint16_t size)
{
    MD_STATUS ret;

#if (SERIAL_U_DIV_2WIRE)
    /* store the argument parameter */
    g_sau_rx_buf  = bufptr;
    g_sau_rx_size = size;
#endif

    /* start UART receiving */
    ret = R_UART0_Receive(bufptr, size);

    return (ret == MD_OK) ? TRUE : FALSE;
}

/**
 ****************************************************************************************
 * @brief Start UART transferring data.
 *        When finished receiving data, calls callback function.
 *
 * @param[in] bufptr  destination pointer, start of the space where data is transfered to.
 * @param[in] size    Transfer size, transferred data size byte by UART.
 *****************************************************************************************
 */
BOOL serial_write(uint8_t *bufptr, uint16_t size)
//BOOL serial_write(const uint8_t *bufptr, const uint16_t size)
{
    MD_STATUS ret;

#if (SERIAL_U_DIV_2WIRE)
    g_sau_tx_stat = T_REQUESTING;

    g_sau_tx_buf  = bufptr;
    g_sau_tx_size = size;
    g_sau_to_cnt  = 0;

    /* transmit the request byte */
    ret = R_UART0_Send((uint8_t *)&g_uart_req_byte_buf, UART_REQ_BYTE_SIZE);
#else
    /* start UART transferring */
    ret = R_UART0_Send(bufptr, size);
#endif

    return (ret == MD_OK) ? TRUE : FALSE;
}

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for when finished UART receiving.
 *****************************************************************************************
 */
void uart_sr_isr(void)
{
#if (SERIAL_U_DIV_2WIRE)
    if((RSCIP_Uart_Rx_Complete() == false) || (g_sau_rx_buf[0] != UART_ACK_BYTE))
    {
        /* Inform that transmission is finished */
        RSCIP_Uart_Rx_Done();
    }
    else
    {
        R_UART0_Receive(g_sau_rx_buf, g_sau_rx_size);
    }
    if(g_sau_tx_stat == T_REQUESTED)
    {
        g_sau_tx_stat = T_ACTIVE;

        /* stop to check timeout */
        RBLE_Clear_Timer_Id(_RBLE_TIMER_ID_SERIAL);

        R_UART0_Send(g_sau_tx_buf, g_sau_tx_size);
    }
    else if (g_sau_tx_stat == T_REQUESTING)
    {
        g_sau_tx_stat = T_RCV_BF_REQUESTED;
    }
    else
    {
    }
#else
    /* Inform that transmission is finished */
    RSCIP_Uart_Rx_Done();
#endif
}

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for when finished UART transmission.
 *****************************************************************************************
 */
void uart_st_isr(void)
{
#if (SERIAL_U_DIV_2WIRE)

    if(g_sau_tx_stat == T_REQUESTING)
    {
        g_sau_tx_stat = T_REQUESTED;

        /* start to check timeout */
        RBLE_Set_Timer_Id(_RBLE_TIMER_ID_SERIAL, (RBLE_FUNC)uart_tx_timeout, 0x0002);
    }
    else if (g_sau_tx_stat == T_RCV_BF_REQUESTED)
    {
        g_sau_tx_stat = T_ACTIVE;

        R_UART0_Send(g_sau_tx_buf, g_sau_tx_size);
    }
    else if(g_sau_tx_stat == T_ACTIVE)
    {
        g_sau_tx_stat = T_IDLE;

        /* Inform the end of transmission */
        RSCIP_Uart_Tx_Done();
    }
#else
    /* Inform that transmission if finished */
    RSCIP_Uart_Tx_Done();
#endif
}

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for when occurred UART receiving error.
 *****************************************************************************************
 */
void uart_sre_isr(void)
{
    /* inform that uart rx_error was occured */
    RSCIP_Uart_Rx_Error();
}

#if (SERIAL_U_DIV_2WIRE)
/**
 ****************************************************************************************
 * @brief re-request to tx after timeout timer was expired
 *****************************************************************************************
 */
static void uart_tx_timeout(void)
{
    /* stop to check timeout */
    RBLE_Clear_Timer_Id(_RBLE_TIMER_ID_SERIAL);

    if(g_sau_tx_stat == T_REQUESTED)
    {
        g_sau_to_cnt++;
        if(g_sau_to_cnt < MAX_TX_TO_COUNT)
        {
            g_sau_tx_stat = T_REQUESTING;

            R_UART0_Send((uint8_t *)&g_uart_req_byte_buf, UART_REQ_BYTE_SIZE);
        }
        /* timeout discontinued */
        else
        {
            g_sau_tx_stat = T_IDLE;

            /* Inform the end of transmission */
            RSCIP_Uart_Tx_Done();
        }
    }
}
#endif // (SERIAL_U_DIV_2WIRE)

/// @} UART


/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2015, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_sau_user.c
* Version      : Code Generator for RL78/G1H V1.00.00.04 [08 Mar 2016]
* Device(s)    : R5F11FLJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for SAU module.
* Creation Date: 2017/12/19
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "sys.h"
#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"

#include <stdio.h>
#include <string.h>


/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
//#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_uart0_interrupt_send(vect=INTST0)
#pragma interrupt r_uart0_interrupt_receive(vect=INTSR0)
#pragma interrupt drv_uart1_snd_int(vect=INTST1)
#pragma interrupt drv_uart1_rcv_int(vect=INTSR1)
/* Start user code for pragma. Do not edit comment generated here */
extern void uart_sr_isr(void);          // defined by uart.c
extern void uart_st_isr(void);          // defined by uart.c
extern void uart_sre_isr(void);         // defined by uart.c
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
extern volatile uint8_t * gp_uart0_tx_address;         /* uart0 send buffer address */
extern volatile uint16_t  g_uart0_tx_count;            /* uart0 send data number */
extern volatile uint8_t * gp_uart0_rx_address;         /* uart0 receive buffer address */
extern volatile uint16_t  g_uart0_rx_count;            /* uart0 receive data number */
extern volatile uint16_t  g_uart0_rx_length;           /* uart0 receive data length */
extern volatile uint8_t * gp_uart1_tx_address;         /* uart1 send buffer address */
extern volatile uint16_t  g_uart1_tx_count;            /* uart1 send data number */
extern volatile uint8_t * gp_uart1_rx_address;         /* uart1 receive buffer address */
extern volatile uint16_t  g_uart1_rx_count;            /* uart1 receive data number */
extern volatile uint16_t  g_uart1_rx_length;           /* uart1 receive data length */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
extern void getmode_in(void);

#define UART_DBG_LEN	40
uint8_t	s_uart_dbg[UART_DBG_LEN];
uint8_t	s_uart_dbg_len;

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_receive
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart0_interrupt_receive(void)
{
    volatile uint8_t rx_data;
    volatile uint8_t err_type;
    
    err_type = (uint8_t)(SSR01 & 0x0007U);
    SIR01 = (uint16_t)err_type;

    if (err_type != 0U)
    {
        r_uart0_callback_error(err_type);
    }
    
    rx_data = RXD0;


//maeda
s_uart_dbg[s_uart_dbg_len] = rx_data;
s_uart_dbg_len++;
s_uart_dbg_len %= UART_DBG_LEN;



    if (g_uart0_rx_length > g_uart0_rx_count)
    {
        *gp_uart0_rx_address = rx_data;
        gp_uart0_rx_address++;
        g_uart0_rx_count++;

        if (g_uart0_rx_length == g_uart0_rx_count)
        {
            r_uart0_callback_receiveend();
        }
    }
    else
    {
        r_uart0_callback_softwareoverrun(rx_data);
    }
}

/***********************************************************************************************************************
* Function Name: r_uart0_interrupt_send
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void __near r_uart0_interrupt_send(void)
{
    if (g_uart0_tx_count > 0U)
    {
        TXD0 = *gp_uart0_tx_address;
        gp_uart0_tx_address++;
        g_uart0_tx_count--;
    }
    else
    {
        r_uart0_callback_sendend();
    }
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_receiveend
* Description  : This function is a callback function when UART0 finishes reception.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_receiveend(void)
{
    /* Start user code. Do not edit comment generated here */
        uart_sr_isr();
	/* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_softwareoverrun
* Description  : This function is a callback function when UART0 receives an overflow data.
* Arguments    : rx_data -
*                    receive data
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_softwareoverrun(uint16_t rx_data)
{
    /* Start user code. Do not edit comment generated here */
     uart_sre_isr();
   /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_sendend
* Description  : This function is a callback function when UART0 finishes transmission.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_sendend(void)
{
    /* Start user code. Do not edit comment generated here */
    uart_st_isr();
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: r_uart0_callback_error
* Description  : This function is a callback function when UART0 reception error occurs.
* Arguments    : err_type -
*                    error type value
* Return Value : None
***********************************************************************************************************************/
static void r_uart0_callback_error(uint8_t err_type)
{
    /* Start user code. Do not edit comment generated here */
     uart_sre_isr();
   /* End user code. Do not edit comment generated here */
}

/********************/
/*     内部変数     */
/********************/
UB drv_uart1_send_buf[DRV_UART1_RCV_RING_LENGTH];				/* 送信バッファ(リング用) */
UB drv_uart1_rcv_buf[DRV_UART1_SND_RING_LENGTH];				/* 受信バッファ(リング用) */
RING_BUF drv_uart1_send_ring;									/* 送信リングバッファ用コントローラ */
RING_BUF drv_uart1_rcv_ring;									/* 受信リングバッファ用コントローラ */

/********************/
/* プロトタイプ宣言 */
/********************/

/************************************************************************/
/* 関数     : drv_uart1_init											*/
/* 関数名   : UART1初期化処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART1の初期化処理													*/
/************************************************************************/
/* 注意事項 :															*/
/* RX321ハードウェアマニュアルP.1386参照								*/
/* デバッグポート有効時はSCI10、無効時はSCI1で動作、sys.hで切り替え可能 */
/************************************************************************/
void drv_uart1_data_init( void )
{
	/* 送信リングバッファ初期化 */
	memset( &drv_uart1_send_buf[0], 0x00, sizeof(drv_uart1_send_buf) );
	ring_buf_init( &drv_uart1_send_ring, &drv_uart1_send_buf[0], DRV_UART1_SND_RING_LENGTH );
	
	/* 受信リングバッファ初期化 */
	memset( &drv_uart1_rcv_buf[0], 0x00, sizeof(drv_uart1_rcv_buf) );
	ring_buf_init( &drv_uart1_rcv_ring, &drv_uart1_rcv_buf[0], DRV_UART1_RCV_RING_LENGTH );	
}


/************************************************************************/
/* 関数     : drv_uart1_clear_err										*/
/* 関数名   : ドライバエラークリア処理									*/
/* 引数     : 															*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* ドライバエラークリア処理												*/
/* CPUマニュアルP.656に基づき処理を行う									*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void drv_uart1_clear_err( void )
{
#if 0
	UB tmp;
	/**************/
	/* エラー処理 */
	/**************/
	// (1)シリアルデータレジスタ(SDR)をリードする。(受信バッファビットをクリアしオーバーランエラー発生を防ぐ)
	// (2)シリアルステータスレジスタ(SSR)をリードする。(エラー種別の判断を行う)
	// (3)シリアルフラグクリアレジスタ(SLR)に"1"をライトする。(リードしたSSRをそのままライトする)
	tmp = DRV_UART_232C_RX_BUF_REG;
	DRV_UART_232C_ERR_CLEAR_REG = DRV_UART_232C_RX_ERR_FLG;
#endif
}


/************************************************************************/
/* 関数     : drv_uart1_snd_int											*/
/* 関数名   : UART1送信割り込み処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART1送信割り込み処理												*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void __near drv_uart1_snd_int(void)
{
	UB snd_data;
	INT	ercd;
	
	ercd = read_ring_buf( &drv_uart1_send_ring, &snd_data );
	if(ercd == E_OK){
		TXD1 = snd_data;
	}
}

/************************************************************************/
/* 関数     : drv_uart1_rcv_int											*/
/* 関数名   : UART1受信割り込み処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART1受信割り込み処理												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void __near drv_uart1_rcv_int(void)
{
    volatile UB rx_data;
    volatile UB err_type;
    
    err_type = (UB)(SSR03 & 0x0007U);
    SIR03 = (UH)err_type;

#if 1
	// エラーでも継続
    if (err_type != 0U){
		drv_uart1_clear_err();
	}
	rx_data = RXD1;
	if( E_QOVR == write_ring_buf( &drv_uart1_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* リングバッファオーバーを異常管理へ通知 */
	}
#else
	// エラーの場合停止
    if (err_type != 0U){
		drv_uart1_clear_err();
	}else{
		rx_data = RXD1;
		if( E_QOVR == write_ring_buf( &drv_uart1_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* リングバッファオーバーを異常管理へ通知 */
		}
	}
#endif
}


/************************************************************************/
/* 関数     : drv_uart1_send_sart										*/
/* 関数名   : UART1送信開始処理											*/
/* 引数    : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART1送信開始処理													*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void drv_uart1_send_start( void )
{
	UB snd_data;
	
	if(E_OK == read_ring_buf( &drv_uart1_send_ring, &snd_data )){
        STMK1 = 1U;    /* disable INTST1 interrupt */
		TXD1 = snd_data;
        STMK1 = 0U;    /* enable INTST1 interrupt */
	}
}

/************************************************************************/
/* 関数     : drv_uart1_get_snd_ring									*/
/* 関数名   : 送信リングバッファポインタ取得							*/
/* 引数     : なし														*/
/* 戻り値   : 送信リングバッファのポインタ								*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* リングバッファのポインタを返す										*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
RING_BUF* drv_uart1_get_snd_ring( void )
{
	return &drv_uart1_send_ring;

}

/************************************************************************/
/* 関数     : drv_uart1_get_rcv_ring									*/
/* 関数名   : 受信リングバッファポインタ取得							*/
/* 引数     : なし														*/
/* 戻り値   : 受信リングバッファのポインタ								*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* リングバッファのポインタを返す										*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
RING_BUF* drv_uart1_get_rcv_ring( void )
{
	return &drv_uart1_rcv_ring;
}


/************************************************************/
/* END OF TEXT												*/
/************************************************************/
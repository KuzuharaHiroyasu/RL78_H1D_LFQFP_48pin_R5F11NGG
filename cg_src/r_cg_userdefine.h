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
* Copyright (C) 2014, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_userdefine.h
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file includes user definition.
* Creation Date: 2017/06/01
***********************************************************************************************************************/
#ifndef _USER_DEF_H
#define _USER_DEF_H

/***********************************************************************************************************************
User definitions
***********************************************************************************************************************/

/* Start user code for function. Do not edit comment generated here */
#include "r_cg_macrodriver.h"

#define DUMMY_DATA          (0xFF)  /* dummy data */
#define CMD_PART_SIZE       (1)     /* size of command part */
#define DATA_PART_SIZE      (2)     /* size of data part */
#define ADR_PART_SIZE       (3)     /* size of address part */
#define RCV_BUF_SIZE        (16)    /* size of receiving buffer */
#define TRS_BUF_SIZE        (16)    /* size of transmitting buffer */

#define RCV_CMD_PART_INDEX  (0)     /* index of receiving command part */
#define RCV_ADR_PART_INDEX  (1)     /* index of receiving address part */
#define RCV_DATA_PART_INDEX (4)     /* index of receiving data part */

#define SFR_AFEPWS          (0x0F0440L) /* AFEPWS */
#define SFR_AFECKS          (0x0F0442L) /* AFECKS */
#define SFR_VSBIAS          (0x0F0443L) /* VSBIAS */
#define SFR_DSADMR          (0x0F0458L) /* DSADMR */
#define SFR_DSADCTL         (0x0F0459L) /* DSADCTL */
#define SFR_PGA0CTL0        (0x0F045AL) /* PGA0CTL0 */
#define SFR_PGA0CTL1        (0x0F045BL) /* PGA0CTL1 */
#define SFR_PGA0CTL2        (0x0F045CL) /* PGA0CTL2 */
#define SFR_PGA0CTL3        (0x0F045DL) /* PGA0CTL3 */
#define SFR_PGA1CTL0        (0x0F045EL) /* PGA1CTL0 */
#define SFR_PGA1CTL1        (0x0F045FL) /* PGA1CTL1 */
#define SFR_PGA1CTL2        (0x0F0460L) /* PGA1CTL2 */
#define SFR_PGA1CTL3        (0x0F0461L) /* PGA1CTL3 */
#define SFR_PGA2CTL0        (0x0F0462L) /* PGA2CTL0 */
#define SFR_PGA2CTL1        (0x0F0463L) /* PGA2CTL1 */
#define SFR_PGA2CTL2        (0x0F0464L) /* PGA2CTL2 */
#define SFR_PGA2CTL3        (0x0F0465L) /* PGA2CTL3 */
#define SFR_PGA3CTL0        (0x0F0466L) /* PGA3CTL0 */
#define SFR_PGA3CTL1        (0x0F0467L) /* PGA3CTL1 */
#define SFR_PGA3CTL2        (0x0F0468L) /* PGA3CTL2 */
#define SFR_PGA3CTL3        (0x0F0469L) /* PGA3CTL3 */
#define SFR_PGABOD          (0x0F046EL) /* PGABOD */
#define SFR_AMPMC           (0x0F0470L) /* AMPMC */
#define SFR_AMPTRM          (0x0F0471L) /* AMPTRM */
#define SFR_AMPTRS          (0x0F0472L) /* AMPTRS */
#define SFR_AMPC            (0x0F0473L) /* AMPC */
#define SFR_PGA1GC          (0x0F0476L) /* PGA1GC */
#define SFR_PGA1S           (0x0F0477L) /* PGA1S */
#define SFR_AMP0S           (0x0F0478L) /* AMP0S */
#define SFR_AMP1S           (0x0F0479L) /* AMP1S */
#define SFR_AMP2S           (0x0F047AL) /* AMP2S */
#define SFR_DAC0DR          (0x0F0480L) /* DAC0DR */
#define SFR_DAC1DR          (0x0F0482L) /* DAC1DR */
#define SFR_DACM0           (0x0F0484L) /* DACM0 */
#define SFR_DACM1           (0x0F0485L) /* DACM1 */

typedef enum
{
    CMD_DSADC_START = 0x00,         /* command of starting delta sigma A/D conversion */
    CMD_DSADC_STOP = 0x01,          /* command of stopping delta sigma A/D conversion */
    CMD_DSADC_RESULT = 0x02,        /* command of result delta sigma A/D conversion */
    CMD_ACK = 0x06,                 /* command of ACK */
    CMD_SFR_READ = 0x10,            /* command of reading SFR */
    CMD_SFR_WRITE = 0x11,           /* command of writing SFR */
    CMD_NACK = 0x15                 /* command of NACK */
} cmd_for_L1A_t;

typedef enum
{
    RCV_PHASE_CMD = 0,              /* phase of receiveing command */
    RCV_PHASE_DATA,                 /* phase of receiveing data */
    RCV_PHASE_COMP                  /* phase of receiveing complete */
} rcv_phase_t;

typedef enum
{
    ADR_INVALID = 0,                /* invalid access */
    ADR_BYTE_ACCESS,                /* byte address */
    ADR_WORD_ACCESS                 /* word access */
} adr_access_t;

typedef enum
{
    TRS_IDLE = 0,                   /* status of transmitting idle */
    TRS_BUSY                        /* status of transmitting busy */
} trs_status_t;

typedef enum
{
    DSADC_EMPTY = 0,                /* status of Delta Sigma A/D conversion data empty */
    DSADC_FULL                      /* status of Delta Sigma A/D conversion data full */
} dsadc_status_t;

typedef enum
{
    DSAD_START_DISABLE  = 0,        /* flag of Delta Sigma A/D conversion data disable */
    DSAD_START_ENABLE               /* flag of Delta Sigma A/D conversion data enable */
} dsadc_enable_t;

struct w_reg8
{
    uint8_t  reg8;
};

struct w_reg16
{
    uint8_t  reg16[2];
};

/* End user code. Do not edit comment generated here */
#endif

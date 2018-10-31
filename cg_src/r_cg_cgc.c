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
* File Name    : r_cg_cgc.c
* Version      : Code Generator for RL78/G1H V1.00.00.04 [08 Mar 2016]
* Device(s)    : R5F11FLJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for CGC module.
* Creation Date: 2017/12/19
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_CGC_Create
* Description  : This function initializes the clock generator.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CGC_Create(void)
{
    volatile uint32_t w_count;

    CMC = _00_CGC_HISYS_PORT | _10_CGC_SUB_OSC | _00_CGC_SYSOSC_UNDER10M;
//    CMC = _00_CGC_HISYS_PORT | _00_CGC_SUB_PORT | _00_CGC_SYSOSC_UNDER10M;
    /* Set fMX */
    MSTOP = 1U;     /* X1 oscillator/external clock stopped */
    /* Set fSUB */
#if 0
    OSMC = _10_CGC_TMRJ_IT_CLK_FIL;
    XTSTOP = 1U;    /* XT1 oscillator/external clock stopped */
#else
    OSMC = _00_CGC_CLK_ENABLE | _00_CGC_TMRJ_IT_CLK_SUBSYSTEM_CLK;
    XTSTOP = 0U;    /* XT1 oscillator/external clock operating */
#endif
    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= CGC_SUBWAITTIME; w_count++)
    {
        NOP();
    }
    /* Set fCLK */
    CSS = 0U;       /* main system clock (fMAIN) */
    /* Set fMAIN */
    MCM0 = 0U;      /* selects the high-speed on-chip oscillator clock (fIH) as the main system clock (fMAIN) */
    /* Set fIH */
    HOCODIV = 1U;	/* RD8001Žb’èF12MHzÝ’è */
    HIOSTOP = 0U;   /* high-speed on-chip oscillator operating */
}



/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

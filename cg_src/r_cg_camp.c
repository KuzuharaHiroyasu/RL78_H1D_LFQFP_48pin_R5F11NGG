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
* File Name    : r_cg_camp.c
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for CAMP module.
* Creation Date: 2017/06/01
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_camp.h"
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
* Function Name: R_CAMP_Create
* Description  : This function initializes the selectable power-on-reset circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CAMP_Create(void)
{
    AFEEN = 1U;     /* supplies AFE input clock */
    AFEPON = 1U;    /* power on AFE */
    /* Wait until AFE stabilize */
    while(0U == AFESTAT);
    AMPEN = 1U;     /* supplies AMP input clock */
    /* AMP Unit ★↓ */
    AMPPON = 1U;
    AMPTRM = 0U;	/*ソフトウェアトリガーモード */
    AMPTRS = 0U;	
    AMPMC = 0xB0; /* AMPSP:1 / AMPSP1:1 / AMPSP0:1 */
//    AMPC = 0x0F; /* AMPE3:1 / AMPE2:0 / AMPE1:1 / AMPE0:1 *//* PGA1(アンプユニット0),AMP0(アンプユニット1),AMP2(アンプユニット3)スタート */
	AMPC   = 0x0B;	/* PGA1(アンプユニット0),AMP0(アンプユニット1),AMP2(アンプユニット3)スタート */
//    PGA1GC = 0x10; /* PGA1GC5:0 / PGA1GC4:1 / PGA1GC3:0 / PGA1GC2:0 / PGA1GC1:0 / PGA1GC0:0 */
    PGA1GC = 0x30; /* PGA1GC5:1 / PGA1GC4:1 / PGA1GC3:0 / PGA1GC2:0 / PGA1GC1:0 / PGA1GC0:0 */
//    PGA1S = 0x01; /* PGA1CS1:0 / PGA1CS0:1 */
    PGA1S = 0x02; /* PGA1CS1:0 / PGA1CS0:1 *///RD8001暫定：ポート設定
    AMP0S = 0x50; /* AMP0NFB:0 / AMP0NSO:1 /      0    / AMP0PDA:1 / 0 / AMP0PS2:0 / AMP0PS1:0 / AMP0PS0:0 */
//    AMP1S = 0x50; /* AMP1NFB:0 / AMP1NSO:1 / AMP1PVR:1 / AMP1PDA:1 / 0 / AMP1PS2:0 / AMP1PS1:0 / AMP1PS0:0 */
    AMP2S = 0x50; /* AMP2NFB:0 / AMP2NSO:1 / AMP2PVR:0 / AMP2PDA:1 / 1 / AMP1PS2:0 /     0     / AMP1PS0:0 */
    /* AMP Unit ★↑ */
	
	R_CAMP0_Start();
}
/***********************************************************************************************************************
* Function Name: R_CAMP0_Start
* Description  : This function starts the CAMP0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CAMP0_Start(void)
{
//	AMPC   = 0x0B;	/* PGA1(アンプユニット0),AMP0(アンプユニット1),AMP2(アンプユニット3)スタート */
//	AMPMON = 0x03;	/* PGA1(アンプユニット0),AMP0(アンプユニット1)スタート */

}
/***********************************************************************************************************************
* Function Name: R_CAMP0_Stop
* Description  : This function stops the CAMP0.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_CAMP0_Stop(void)
{

}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

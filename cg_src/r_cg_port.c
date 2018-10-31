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
* File Name    : r_cg_port.c
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for Port module.
* Creation Date: 2017/06/01
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
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
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
//    P1 = _00_Pn5_OUTPUT_0;
//    POM1 = _00_POMn5_NCH_OFF;
//    PM1 = _80_PMn7_MODE_UNUSED | _40_PMn6_MODE_UNUSED | _20_PMn5_MODE_UNUSED | _10_PMn4_MODE_UNUSED | 
//          _08_PMn3_MODE_UNUSED | _04_PMn2_MODE_UNUSED | _02_PMn1_MODE_UNUSED | _01_PMn0_MODE_UNUSED;
//    PM3 = _80_PMn7_MODE_INPUT | _40_PMn6_MODE_INPUT | _20_PMn5_MODE_UNUSED |
//    PM3 = _80_PMn7_MODE_UNUSED | _40_PMn6_MODE_UNUSED | _20_PMn5_MODE_UNUSED |
    PM3 = _00_PMn7_MODE_OUTPUT | _40_PMn6_MODE_UNUSED | _20_PMn5_MODE_UNUSED |
          _04_PMn2_MODE_UNUSED | _02_PMn1_MODE_UNUSED | _01_PMn0_MODE_UNUSED;
	// 赤外線(P77)、赤色フォトダイオード(P76)ON/OFF切替
    PM7 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _20_PMn5_MODE_UNUSED |
          _04_PMn2_MODE_UNUSED | _02_PMn1_MODE_UNUSED | _01_PMn0_MODE_UNUSED;
    P7  = 0xBF;


}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

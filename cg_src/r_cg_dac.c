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
* Copyright (C) . All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_dac.c
* Version      :  
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for DAC module.
* Creation Date: 2018/04/18
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_dac.h"
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
* Function Name: R_DAC_Create
* Description  : This function initializes the DA converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC_Create(void)
{
    AFEEN = 1U;     /* enables input clock supply */
    AFEPON = 1U;    /* power on AFE */

    while (0U == AFESTAT)
    {
        ;/* Wait until AFE stabilize  */
    }

    DACEN = 1U;     /* enables input clock supply */
    DACM0 |= _00_DA1_FLUSH_RIGHT_FORMAT | _00_DA1_NORMAL_MODE;
    DACM1 = _00_DA1_REFERENCE_AVDD;
    DAC1DR = _0000_DA1_COUVERSION_VALUE;
    
}
/***********************************************************************************************************************
* Function Name: R_DAC1_Start
* Description  : This function enables the DA converter channel1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC1_Start(void)
{
    volatile uint16_t w_count;

    DAC1PON = 1U;   /* enables D/A conversion operation */

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count <= DA1_WAITTIME; w_count++)
    {
        NOP();
    }
}
/***********************************************************************************************************************
* Function Name: R_DAC1_Stop
* Description  : This function stops the DA converter channel1.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC1_Stop(void)
{
    DAC1PON = 0U;   /* stops D/A conversion operation */
}
/***********************************************************************************************************************
* Function Name: R_DAC1_Set_ConversionValue
* Description  : This function sets the DA converter channel1 value.
* Arguments    : reg_value -
*                    value of conversion
* Return Value : None
***********************************************************************************************************************/
void R_DAC1_Set_ConversionValue(uint16_t regvalue)
{
//    DAC1DR = regvalue;
	DAC1DR = ( regvalue & 0x0FFF );
}
/***********************************************************************************************************************
* Function Name: R_DAC_Set_PowerOff
* Description  : This function stops supply of input clock and reset all SFR.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_DAC_Set_PowerOff(void)
{
    DACEN = 0U;     /* stops input clock supply */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

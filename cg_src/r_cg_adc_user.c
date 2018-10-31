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
* File Name    : r_cg_adc_user.c
* Version      : Code Generator for RL78/G1H V1.00.00.04 [08 Mar 2016]
* Device(s)    : R5F11FLJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for ADC module.
* Creation Date: 2017/12/20
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
#pragma interrupt r_adc_interrupt(vect=INTAD)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
unsigned short adc_sub( unsigned char ad_type);

/***********************************************************************************************************************
* Function Name: r_adc_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static unsigned short ad_finish = 0;
static void __near r_adc_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
    
	ad_finish = 1;
}

void adc_do( uint16_t* ad1, uint16_t* ad2, uint16_t* ad3 )
{
	*ad1 = adc_sub( AD_TYPE_ANI8 );
	*ad2 = adc_sub( AD_TYPE_ANI9 );
	*ad3 = adc_sub( AD_TYPE_ANI10 );
}

unsigned short adc_sub( unsigned char ad_type)
{
	unsigned short ad_val;
	
	R_ADC_Create();
	ADS = ad_type;
	R_ADC_Set_OperationOn();
	R_ADC_Start();
	
	while(1){
		if( 1 == ad_finish ){
			R_ADC_Get_Result( &ad_val );
			ad_finish = 0;
			break;
		}
	}
	
	return ad_val;
}


/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

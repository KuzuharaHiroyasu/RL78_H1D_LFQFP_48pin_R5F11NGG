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
* File Name    : r_cg_intp_user.c
* Version      :  
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for INTP module.
* Creation Date: 2018/04/11
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "sys.h"

#include "r_cg_macrodriver.h"
#include "r_cg_intp.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
//#pragma interrupt r_intc1_interrupt(vect=INTP1)
//#pragma interrupt r_intc3_interrupt(vect=INTP3)
//#pragma interrupt r_intc5_interrupt(vect=INTP5)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: r_intc1_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/* static */ void __near r_intc1_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
   	NOP();
}
/***********************************************************************************************************************
* Function Name: r_intc3_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/* static */ void __near r_intc3_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
   	NOP();
}
/***********************************************************************************************************************
* Function Name: r_intc5_interrupt
* Description  : None
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/* static */ void __near r_intc5_interrupt(void)
{
    /* Start user code. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
   	NOP();
}




UB drv_intp_read_pow_sw(void)
{
	UB port1;
	UB port2;
	UB ret;
	
	// ポート読み出し ※2回一致で抜ける(一致し続けない時は致命的異常なので考慮しない)
	do{
		port1 = DRV_I_PORT_POW_SW;
		port2= DRV_I_PORT_POW_SW;
	}while( port1 != port2 );

	// 論理対応
	if( LOW == port1 ){
		ret = ON;
	}else{
		ret = OFF;
	}
	
	return ret;
}

UB drv_intp_read_g1d_int(void)
{
	UB port1;
	UB port2;
	UB ret;
	
	// ポート読み出し ※2回一致で抜ける(一致し続けない時は致命的異常なので考慮しない)
	do{
		port1 = DRV_I_PORT_G1D_INT;
		port2 = DRV_I_PORT_G1D_INT;
	}while( port1 != port2 );

	// 論理対応
	if( HIGH == port1 ){
		ret = ON;
	}else{
		ret = OFF;
	}
	
	return ret;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

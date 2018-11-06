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
* File Name    : r_cg_port_user.c
* Version      :  
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for Port module.
* Creation Date: 2018/04/10
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "sys.h"

#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"



// ユーザー定義
void drv_o_port_sekigai( UB data )
{
	if( ON == data ){
		DRV_O_PORT_SEKIGAI = LOW;
	}else{
		DRV_O_PORT_SEKIGAI = HIGH;
	}
}

void drv_o_port_sekishoku( UB data )
{
	if( ON == data ){
		DRV_O_PORT_SEKISHOKU = LOW;
	}else{
		DRV_O_PORT_SEKISHOKU = HIGH;
	}
}


void drv_o_port_mike( UB data )
{
	if( ON == data ){
		DRV_O_PORT_MIKE = LOW;
	}else{
		DRV_O_PORT_MIKE = HIGH;
	}
}
 
void drv_o_port_g1d_int( UB data )
{
	if( ON == data ){
		DRV_O_PORT_G1D_INT = HIGH;
	}else{
		DRV_O_PORT_G1D_INT = LOW;
	}
}

UB drv_i_port_kensa( void )
{
	UB ret = OFF;
	
	if( HIGH == DRV_I_PORT_KENSA ){
		ret = ON;
	}
	
	return ret;
}


UB drv_i_port_bat_chg( void )
{
	UB ret = OFF;
	
	if( LOW == DRV_I_PORT_BAT_CHG ){
		ret = ON;
	}
	
	return ret;
}

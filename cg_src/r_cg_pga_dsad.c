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
* File Name    : r_cg_pga_dsad.c
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for PGIA module.
* Creation Date: 2017/06/01
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_pga_dsad.h"
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
* Function Name: R_PGA_DSAD_Create
* Description  : This function initializes the PGA and DS A/D converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Create(void)
{
    AFEEN = 1U;     /* supplies AFE input clock */
    AFEPON = 1U;    /* power on AFE */
    /* Wait until AFE stabilize  */ 
    while(0U == AFESTAT);
    PGAEN = 1U;     /* supplies PGA and DS A/D  input clock */
    DSADMR = _00_DSAD_CONVMODE_NORMAL;			/* ソフトトリガ、ノーマル */
    AFECKS = _0D_AFE_CLOCK_DIVIDED6;			/* fCLK/6(6分周) */
    VSBIAS = _0E_VSBIAS_OUTPUT_VOLTAGE_10;
    PGAPON = 1U;    /* power on PGA and DS A/D */
    /* Wait until PGA stabilize  */ 
    while(0U == PGASTAT);
    DSADMK = 1U;    /* disable INTDSAD interrupt */
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    /* Set INTDSAD low priority */
    DSADPR1 = 1U;
    DSADPR0 = 1U;
//    DSADMR |= _00_DSAD_CONVTRI_SOFTWARE;	//処理重複削除
	/* シングルスキャン,入力マルチプレクサ0のみ許可*/
    DSADCTL = _80_DSAD_CONVERSION_START | _20_DSAD_SCAN_MODE_STOP | _08_DSAD_INPUT3_DISABLE | 
              _04_DSAD_INPUT2_DISABLE | _02_DSAD_INPUT1_DISABLE | _00_DSAD_INPUT0_ENABLE;
	/* 連続スキャン,入力マルチプレクサ0のみ許可*/
//    DSADCTL = _80_DSAD_CONVERSION_START | _00_DSAD_SCAN_MODE_SEQUENTIAL | _08_DSAD_INPUT3_DISABLE | 
//              _04_DSAD_INPUT2_DISABLE | _02_DSAD_INPUT1_DISABLE | _00_DSAD_INPUT0_ENABLE;		/* シングルスキャン,入力マルチプレクサ0のみ許可*/
//★    PGA0CTL0 = _40_PGA_CHANNELn_SAMPLE_RATIO_256 | _08_PGA_CHANNELn_GSET1_3 | _03_PGA_CHANNELn_GSET2_8;
	//入力マルチプレクサ0の設定 ※詳細不明
    PGA0CTL0 = _40_PGA_CHANNELn_SAMPLE_RATIO_256 | _10_PGA_CHANNELn_GSET1_8 | _03_PGA_CHANNELn_GSET2_8;		//GTOTAL 64倍
    PGA0CTL1 = _00_PGA_CHANNELn_DIFFERENTIAL | _10_PGA_CHANNELn_OFFSET_16;			/* 入力マルチプレクサ0の計数 */
//    PGA0CTL1 = _00_PGA_CHANNELn_DIFFERENTIAL | _1F_PGA_CHANNELn_OFFSET_31;			/* 入力マルチプレクサ0の計数 */
    PGA0CTL2 = _01_PGA_CHANNEL0_SAMPLE_COUNT;
    PGA0CTL3 = _00_PGA_CHANNELn_MUMBER_MODE_8032 | _00_PGA_CHANNELn_AVERAGING_NOT;
//    PGA0CTL3 = 0x08;
	//入力マルチプレクサ1の設定
    PGA1CTL0 = _40_PGA_CHANNELn_SAMPLE_RATIO_256 | _00_PGA_CHANNELn_GSET1_1 | _00_PGA_CHANNELn_GSET2_1;//★
    PGA1CTL1 = _80_PGA_CHANNELn_SINGLEEND | _10_PGA_CHANNELn_OFFSET_16;
    PGA1CTL2 = 0x01U;//★
    PGA1CTL3 = _00_PGA_CHANNELn_MUMBER_MODE_8032 | _00_PGA_CHANNELn_AVERAGING_NOT;//★
	//入力マルチプレクサ2の設定
    PGA2CTL0 = _40_PGA_CHANNELn_SAMPLE_RATIO_256 | _00_PGA_CHANNELn_GSET1_1 | _00_PGA_CHANNELn_GSET2_1;//★
    PGA2CTL1 = _80_PGA_CHANNELn_SINGLEEND | _10_PGA_CHANNELn_OFFSET_16;
    PGA2CTL2 = 0x01U;//★
    PGA2CTL3 = _00_PGA_CHANNELn_MUMBER_MODE_8032 | _00_PGA_CHANNELn_AVERAGING_NOT;//★
	//入力マルチプレクサ3の設定
    PGA3CTL0 = _40_PGA_CHANNELn_SAMPLE_RATIO_256 | _00_PGA_CHANNELn_GSET1_1 | _00_PGA_CHANNELn_GSET2_1;//★
    PGA3CTL1 = _00_PGA_CHANNELn_DIFFERENTIAL | _40_PGA_CHANNELn_OFFSET | _10_PGA_CHANNELn_OFFSET_16;//★
    PGA3CTL2 = 0x01U;//★
    PGA3CTL3 = _00_PGA_CHANNELn_MUMBER_MODE_8032 | _00_PGA_CHANNELn_AVERAGING_NOT | _00_PGA_CHANNELn_AVERAGING_UNIT_8;//★
    
    PGABOD = _00_DSAD_DISCONNECTION_DETECTION_UNUSED;		/* 断線検知は必要？ */
    
    // 24ビットΔΣA/Dコンバータ
//    PGAEN = 1;		// 重複
//    DSADCRC = 0x20;
    
//    VSBIAS = 0x0E;		/* 1.0 */
//    VSBIAS = 0x09;		/* 0.5 */
    
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Start
* Description  : This function starts the PGA and DS A/D converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Start(void)
{
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    DSADMK = 0U;    /* enable INTDSAD interrupt */
    DSADST = 1U;    /* start conversion */
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Stop
* Description  : This function stops the PGA and DS A/D converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Stop(void)
{
    DSADMK = 1U;    /* disable INTDSAD interrupt */
    DSADIF = 0U;    /* clear INTDSAD interrupt flag */
    DSADST = 0U;    /* stop conversion */
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Get_Result
* Description  : This function returns the DS A/D conversion result.
* Arguments    : bufferH -
*                    high 16 bits
*                bufferL -
*                    low 16 bits
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Get_Result(uint16_t * const bufferH,uint16_t * const bufferL)
{
	*bufferH = DSADCR1;
	*bufferL = DSADCR0;
}
/***********************************************************************************************************************
* Function Name: R_PGA_DSAD_Get_AverageResult
* Description  : This function returns the DS A/D conversion average result.
* Arguments    : bufferH -
*                    high 16 bits
*                bufferL -
*                    low 16 bits
* Return Value : None
***********************************************************************************************************************/
void R_PGA_DSAD_Get_AverageResult(uint16_t * const bufferH,uint16_t * const bufferL)
{
	*bufferH = DSADMV1;
	*bufferL = DSADMV0;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

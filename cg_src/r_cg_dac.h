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
* File Name    : r_cg_dac.h
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for DAC module.
* Creation Date: 2017/06/01
***********************************************************************************************************************/
#ifndef DAC_H
#define DAC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    D/A converter mode register 0 (DACM0) 
*/
/* Selection of trigger mode (DACTMD) */
#define _00_DAC_TRIG_SOFTWARE          (0x00U) /* software trigger mode */
#define _80_DAC_TRIG_HARDWARE          (0x80U) /* hardware trigger mode */
/* Selection of D/A converter resolution (DACRES) */
#define _00_DAC_BIT_12                 (0x00U) /* 12 bits */
#define _01_DAC_BIT_8                  (0x01U) /* 8 bits */

/*
    D/A converter mode register 1 (DACM1) 
*/
/* Selection of D/A converter reference voltage source (DACVRF) */
#define _00_DAC_AVDD                   (0x00U) /* 5 V voltage supplied: select AVDD */
#define _01_DAC_VREFDA                 (0x01U) /* 1.45 V voltage supplied: select VREFDA */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define DA_WAITINGTIME                (0xA0U) /* set the waiting time */
#define _5A_DAC_SAMPLE_COUNT           (0x5AU) /* sample count per AUTOSCAN cycle for channel DACDL */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_DAC_Create(void);
void R_DAC_Start(void);
void R_DAC_Stop(void);
void R_DAC_Change_OutputVoltage_8bit(uint8_t outputVoltage);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

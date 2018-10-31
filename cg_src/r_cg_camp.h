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
* File Name    : r_cg_camp.h
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for CAMP module.
* Creation Date: 2017/06/01
***********************************************************************************************************************/
#ifndef CAMP_H
#define CAMP_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Configurable amplifier 0 mode register (AMP0MR) 
*/
/* Selection of operating mode of operational amplifier 0 (AMP0HSM) */
#define _00_AMP_CH0_NOMAL                             (0x00U) /* normal mode */
#define _01_AMP_CH0_HIGHSEEP                          (0x01U) /* high-speed mode */

/*
    Configurable amplifier 1 mode register (AMP1MR) 
*/
/* Selection of operating mode of operational amplifier 1 (AMP1HSM) */
#define _00_AMP_CH1_NOMAL                             (0x00U) /* normal mode */
#define _01_AMP_CH1_HIGHSEEP                          (0x01U) /* high-speed mode */

/*
    Configurable amplifier 2 mode register (AMP2MR) 
*/
/* Selection of operating mode of operational amplifier 2 (AMP2HSM) */
#define _00_AMP_CH2_NOMAL                             (0x00U) /* normal mode */
#define _01_AMP_CH2_HIGHSEEP                          (0x01U) /* high-speed mode */

/*
    Configurable amplifier 0 output selection register (AMP0S0) 
*/
/* selection of offset trimming monitor (AMPMONI1,AMPMONI0) */
#define _00_AMP_OFFSET_TRIMMING_MONITOR_NONE          (0x00U) /* no connection */
#define _40_AMP_OFFSET_TRIMMING_MONITOR_AMP0          (0x40U) /* input the positive input signal and output signal of 
                                                                 operational amplifier 0 to the PGA */
#define _80_AMP_OFFSET_TRIMMING_MONITOR_AMP1          (0x80U) /* input the positive input signal and output signal of 
                                                                 operational amplifier 1 to the PGA */
#define _C0_AMP_OFFSET_TRIMMING_MONITOR_AMP2          (0xC0U) /* input the positive input signal and output signal of 
                                                                 operational amplifier 2 to the PGA */
/* Connection of operational amplifier 0 output pin to general-purpose analog port ANX1 (AMP0OX1) */
#define _00_AMP_CH0_OUT_ANX1_DISABLE                  (0x00U) /* none */
#define _02_AMP_CH0_OUT_ANX1_ENABLE                   (0x02U) /* connect */
/* Connection of operational amplifier 0 output pin to general-purpose analog port ANX0 (AMP0OX0) */
#define _00_AMP_CH0_OUT_ANX0_DISABLE                  (0x00U) /* none */
#define _01_AMP_CH0_OUT_ANX0_ENABLE                   (0x01U) /* connect */

/*
    Configurable amplifier 1 output selection register (AMP1S0) 
*/
/* Connection of operational amplifier 1 output pin to general-purpose analog port ANX3 (AMP1OX3) */
#define _00_AMP_CH1_OUT_ANX3_DISABLE                  (0x00U) /* none */
#define _08_AMP_CH1_OUT_ANX3_ENABLE                   (0x08U) /* connect */
/* Connection of operational amplifier 1 output pin to general-purpose analog port ANX2 (AMP1OX2) */
#define _00_AMP_CH1_OUT_ANX2_DISABLE                  (0x00U) /* none */
#define _04_AMP_CH1_OUT_ANX2_ENABLE                   (0x04U) /* connect */
/* Connection of operational amplifier 1 output pin to general-purpose analog port ANX1 (AMP1OX1) */
#define _00_AMP_CH1_OUT_ANX1_DISABLE                  (0x00U) /* none */
#define _02_AMP_CH1_OUT_ANX1_ENABLE                   (0x02U) /* connect */
/* Connection of operational amplifier 1 output pin to general-purpose analog port ANX0 (AMP1OX0) */
#define _00_AMP_CH1_OUT_ANX0_DISABLE                  (0x00U) /* none */
#define _01_AMP_CH1_OUT_ANX0_ENABLE                   (0x01U) /* connect */

/*
    Configurable amplifier 2 output selection register (AMP2S0) 
*/
/* Connection of operational amplifier 2 output pin to general-purpose analog port ANX5 (AMP2OX5) */
#define _00_AMP_CH2_OUT_ANX5_DISABLE                  (0x00U) /* none */
#define _20_AMP_CH2_OUT_ANX5_ENABLE                   (0x20U) /* connect */
/* Connection of operational amplifier 2 output pin to general-purpose analog port ANX4 (AMP2OX4) */
#define _00_AMP_CH2_OUT_ANX4_DISABLE                  (0x00U) /* none */
#define _10_AMP_CH2_OUT_ANX4_ENABLE                   (0x10U) /* connect */
/* Connection of operational amplifier 2 output pin to general-purpose analog port ANX3 (AMP2OX3) */
#define _00_AMP_CH2_OUT_ANX3_DISABLE                  (0x00U) /* none */
#define _08_AMP_CH2_OUT_ANX3_ENABLE                   (0x08U) /* connect */
/* Connection of operational amplifier 2 output pin to general-purpose analog port ANX2 (AMP2OX2) */
#define _00_AMP_CH2_OUT_ANX2_DISABLE                  (0x00U) /* none */
#define _04_AMP_CH2_OUT_ANX2_ENABLE                   (0x04U) /* connect */
/* Connection of operational amplifier 2 output pin to general-purpose analog port ANX1 (AMP2OX1) */
#define _00_AMP_CH2_OUT_ANX1_DISABLE                  (0x00U) /* none */
#define _02_AMP_CH2_OUT_ANX1_ENABLE                   (0x02U) /* connect */
/* Connection of operational amplifier 2 output pin to general-purpose analog port ANX0 (AMP2OX0) */
#define _00_AMP_CH2_OUT_ANX0_DISABLE                  (0x00U) /* none */
#define _01_AMP_CH2_OUT_ANX0_ENABLE                   (0x01U) /* connect */

/*
    Configurable amplifier 0 negative input selection register (AMP0S1) 
*/
/* Connection of operational amplifier 0 output pin to negative input pin and feedback circuit (AMP0NFB) */
#define _00_AMP_CH0_VFC_DISABLE                       (0x00U) /* none */
#define _80_AMP_CH0_VFC_ENABLE                        (0x80U) /* connect */
/* Connection of operational amplifier 0 negative input pin to general-purpose analog port ANX1 (AMP0NX1) */
#define _00_AMP_CH0_NEG_ANX1_DISABLE                  (0x00U) /* none */
#define _02_AMP_CH0_NEG_ANX1_ENABLE                   (0x02U) /* connect */
/* Connection of operational amplifier 0 negative input pin to general-purpose analog port ANX0 (AMP0NX0) */
#define _00_AMP_CH0_NEG_ANX0_DISABLE                  (0x00U) /* none */
#define _01_AMP_CH0_NEG_ANX0_ENABLE                   (0x01U) /* connect */

/*
    Configurable amplifier 1 negative input selection register (AMP1S1) 
*/
/* Connection of operational amplifier 1 output pin to negative input pin and feedback circuit (AMP1NFB) */
#define _00_AMP_CH1_VFC_DISABLE                       (0x00U) /* none */
#define _80_AMP_CH1_VFC_ENABLE                        (0x80U) /* connect */
/* Connection of operational amplifier 1 negative input pin to general-purpose analog port ANX3 (AMP1NX3) */
#define _00_AMP_CH1_NEG_ANX3_DISABLE                  (0x00U) /* none */
#define _08_AMP_CH1_NEG_ANX3_ENABLE                   (0x08U) /* connect */
/* Connection of operational amplifier 1 negative input pin to general-purpose analog port ANX2 (AMP1NX2) */
#define _00_AMP_CH1_NEG_ANX2_DISABLE                  (0x00U) /* none */
#define _04_AMP_CH1_NEG_ANX2_ENABLE                   (0x04U) /* connect */
/* Connection of operational amplifier 1 negative input pin to general-purpose analog port ANX1 (AMP1NX1) */
#define _00_AMP_CH1_NEG_ANX1_DISABLE                  (0x00U) /* none */
#define _02_AMP_CH1_NEG_ANX1_ENABLE                   (0x02U) /* connect */
/* Connection of operational amplifier 1 negative input pin to general-purpose analog port ANX0 (AMP1NX0) */
#define _00_AMP_CH1_NEG_ANX0_DISABLE                  (0x00U) /* none */
#define _01_AMP_CH1_NEG_ANX0_ENABLE                   (0x01U) /* connect */

/*
    Configurable amplifier 2 negative input selection register (AMP2S1) 
*/
/* Connection of operational amplifier 2 output pin to negative input pin and feedback circuit (AMP2NFB) */
#define _00_AMP_CH2_VFC_DISABLE                       (0x00U) /* none */
#define _80_AMP_CH2_VFC_ENABLE                        (0x80U) /* connect */
/* Connection of operational amplifier 2 negative input pin to general-purpose analog port ANX5 (AMP2NX5) */
#define _00_AMP_CH2_NEG_ANX5_DISABLE                  (0x00U) /* none */
#define _20_AMP_CH2_NEG_ANX5_ENABLE                   (0x20U) /* connect */
/* Connection of operational amplifier 2 negative input pin to general-purpose analog port ANX4 (AMP2NX4) */
#define _00_AMP_CH2_NEG_ANX4_DISABLE                  (0x00U) /* none */
#define _10_AMP_CH2_NEG_ANX4_ENABLE                   (0x10U) /* connect */
/* Connection of operational amplifier 2 negative input pin to general-purpose analog port ANX3 (AMP2NX3) */
#define _00_AMP_CH2_NEG_ANX3_DISABLE                  (0x00U) /* none */
#define _08_AMP_CH2_NEG_ANX3_ENABLE                   (0x08U) /* connect */
/* Connection of operational amplifier 2 negative input pin to general-purpose analog port ANX2 (AMP2NX2) */
#define _00_AMP_CH2_NEG_ANX2_DISABLE                  (0x00U) /* none */
#define _04_AMP_CH2_NEG_ANX2_ENABLE                   (0x04U) /* connect */
/* Connection of operational amplifier 2 negative input pin to general-purpose analog port ANX1 (AMP2NX1) */
#define _00_AMP_CH2_NEG_ANX1_DISABLE                  (0x00U) /* none */
#define _02_AMP_CH2_NEG_ANX1_ENABLE                   (0x02U) /* connect */
/* Connection of operational amplifier 2 negative input pin to general-purpose analog port ANX0 (AMP2NX0) */
#define _00_AMP_CH2_NEG_ANX0_DISABLE                  (0x00U) /* none */
#define _01_AMP_CH2_NEG_ANX0_ENABLE                   (0x01U) /* connect */

/*
    Configurable amplifier 0 positive input selection register (AMP0S2) 
*/
/* Inputting the 12-bit D/A converter output signal to the operational amplifier 0 positive input pin (AMP0PDA) */
#define _00_AMP_CH0_POS_DAC_DISABLE                   (0x00U) /* none */
#define _80_AMP_CH0_POS_DAC_ENABLE                    (0x80U) /* connect */
/* Connection of operational amplifier 0 positive input pin to general-purpose analog port ANX1 (AMP0PX1) */
#define _00_AMP_CH0_POS_ANX1_DISABLE                  (0x00U) /* None */
#define _02_AMP_CH0_POS_ANX1_ENABLE                   (0x02U) /* Connect */
/* Connection of operational amplifier 0 positive input pin to general-purpose analog port ANX0 (AMP0PX0) */
#define _00_AMP_CH0_POS_ANX0_DISABLE                  (0x00U) /* none */
#define _01_AMP_CH0_POS_ANX0_ENABLE                   (0x01U) /* connect */

/*
    Configurable amplifier 1 positive input selection register (AMP1S2) 
*/
/* Inputting the 12-bit D/A converter output signal to the operational amplifier 1 positive input pin (AMP1PDA) */
#define _00_AMP_CH1_POS_DAC_DISABLE                   (0x00U) /* None */
#define _80_AMP_CH1_POS_DAC_ENABLE                    (0x80U) /* Connect */
/* Connection of operational amplifier 1 positive input pin to general-purpose analog port ANX3 (AMP1PX3) */
#define _00_AMP_CH1_POS_ANX3_DISABLE                  (0x00U) /* none */
#define _08_AMP_CH1_POS_ANX3_ENABLE                   (0x08U) /* connect */
/* Connection of operational amplifier 1 positive input pin to general-purpose analog port ANX2 (AMP1PX2) */
#define _00_AMP_CH1_POS_ANX2_DISABLE                  (0x00U) /* None */
#define _04_AMP_CH1_POS_ANX2_ENABLE                   (0x04U) /* Connect */
/* Connection of operational amplifier 1 positive input pin to general-purpose analog port ANX1 (AMP1PX1) */
#define _00_AMP_CH1_POS_ANX1_DISABLE                  (0x00U) /* None */
#define _02_AMP_CH1_POS_ANX1_ENABLE                   (0x02U) /* Connect */
/* Connection of operational amplifier 1 positive input pin to general-purpose analog port ANX0 (AMP1PX0) */
#define _00_AMP_CH1_POS_ANX0_DISABLE                  (0x00U) /* none */
#define _01_AMP_CH1_POS_ANX0_ENABLE                   (0x01U) /* connect */

/*
    Configurable amplifier 2 positive input selection register (AMP2S2) 
*/
/* Inputting the 12-bit D/A converter output signal to the operational amplifier 2 positive input pin (AMP2PDA) */
#define _00_AMP_CH2_POS_DAC_DISABLE                   (0x00U) /* none */
#define _80_AMP_CH2_POS_DAC_ENABLE                    (0x80U) /* connect */
/* Connection of operational amplifier 2 positive input pin to general-purpose analog port ANX5 (AMP2PX5) */
#define _00_AMP_CH2_POS_ANX5_DISABLE                  (0x00U) /* none */
#define _20_AMP_CH2_POS_ANX5_ENABLE                   (0x20U) /* connect */
/* Connection of operational amplifier 2 positive input pin to general-purpose analog port ANX4 (AMP2PX4) */
#define _00_AMP_CH2_POS_ANX4_DISABLE                  (0x00U) /* none */
#define _10_AMP_CH2_POS_ANX4_ENABLE                   (0x10U) /* connect */
/* Connection of operational amplifier 2 positive input pin to general-purpose analog port ANX3 (AMP2PX3) */
#define _00_AMP_CH2_POS_ANX3_DISABLE                  (0x00U) /* none */
#define _08_AMP_CH2_POS_ANX3_ENABLE                   (0x08U) /* connect */
/* Connection of operational amplifier 2 positive input pin to general-purpose analog port ANX2 (AMP2PX2) */
#define _00_AMP_CH2_POS_ANX2_DISABLE                  (0x00U) /* none */
#define _04_AMP_CH2_POS_ANX2_ENABLE                   (0x04U) /* connect */
/* Connection of operational amplifier 2 positive input pin to general-purpose analog port ANX1 (AMP2PX1) */
#define _00_AMP_CH2_POS_ANX1_DISABLE                  (0x00U) /* none */
#define _02_AMP_CH2_POS_ANX1_ENABLE                   (0x02U) /* connect */
/* Connection of operational amplifier 2 positive input pin to general-purpose analog port ANX0 (AMP2PX0) */
#define _00_AMP_CH2_POS_ANX0_DISABLE                  (0x00U) /* none */
#define _01_AMP_CH2_POS_ANX0_ENABLE                   (0x01U) /* connect */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define CAMP_WAITINGTIME1                             (0x320U) /* set the waiting time */
#define CAMP_WAITINGTIME2                             (0x215U) /* set the waiting time */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_CAMP_Create(void);
void R_CAMP0_Start(void);
void R_CAMP0_Stop(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

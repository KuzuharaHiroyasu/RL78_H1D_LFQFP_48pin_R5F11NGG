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
* File Name    : r_cg_amp.h
* Version      :  
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for AMP module.
* Creation Date: 2018/04/18
***********************************************************************************************************************/
#ifndef AMP_H
#define AMP_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Amplifier mode control register (AMPMC) 
*/
/* Selection of operating mode for amplifier unit 2 and amplifier unit 3 (AMPSP) */
#define _00_AMP_CH2_3_LOWPOWER                (0x00U) /* low-power mode */
#define _80_AMP_CH2_3_HIGHSPEED               (0x80U) /* high-speed mode */
/* Selection of operating mode for amplifier unit 1 (AMPSP1) */
#define _00_AMP_CH1_LOWPOWER                  (0x00U) /* low-power mode */
#define _20_AMP_CH1_HIGHSPEED                 (0x20U) /* high-speed mode */
/* Selection of operating mode for amplifier unit 0 (AMPSP0) */
#define _00_AMP_CH0_LOWPOWER                  (0x00U) /* low-power mode */
#define _10_AMP_CH0_HIGHSPEED                 (0x10U) /* high-speed mode */

/*
    Amplifier trigger mode control register (AMPTRM) 
*/
/* Control of activation/stop trigger for the amplifier unit 3 (AMPTRM31,AMPTRM30) */
#define _00_AMP2_TRIGGER_SOFTWARE             (0x00U) /* software trigger mode */
#define _40_AMP2_TRIGGER_ELC                  (0x40U) /* ELC trigger mode */
#define _C0_AMP2_TRIGGER_ELC_AD               (0xC0U) /* ELC and A/D trigger mode */
/* Control of activation/stop trigger for the amplifier unit 2 (AMPTRM21,AMPTRM20) */
#define _00_AMP1_TRIGGER_SOFTWARE             (0x00U) /* software trigger mode */
#define _10_AMP1_TRIGGER_ELC                  (0x10U) /* ELC trigger mode */
#define _30_AMP1_TRIGGER_ELC_AD               (0x30U) /* ELC and A/D trigger mode */
/* Control of activation/stop trigger for the amplifier unit 1 (AMPTRM11,AMPTRM10) */
#define _00_AMP0_TRIGGER_SOFTWARE             (0x00U) /* software trigger mode */
#define _04_AMP0_TRIGGER_ELC                  (0x04U) /* ELC trigger mode */
#define _0C_AMP0_TRIGGER_ELC_AD               (0x0CU) /* ELC and A/D trigger mode */
/* Control of activation/stop trigger for the amplifier unit 0 (AMPTRM01,AMPTRM00) */
#define _00_PGA1_TRIGGER_SOFTWARE             (0x00U) /* software trigger mode */
#define _01_PGA1_TRIGGER_ELC                  (0x01U) /* ELC trigger mode */
#define _03_PGA1_TRIGGER_ELC_AD               (0x03U) /* ELC and A/D trigger mode */

/*
    Amplifier ELC trigger select register (AMPTRS) 
*/
/* ELC trigger selection (AMPTRS1,AMPTRS0) */
#define _00_AMP_ELC_TRIGGER_SOURCE_0          (0x00U) /* ELC trigger 0-3 */
#define _01_AMP_ELC_TRIGGER_SOURCE_1          (0x01U) /* ELC trigger 0-1 */
#define _03_AMP_ELC_TRIGGER_SOURCE_2          (0x03U) /* ELC trigger 0 */

/*
    Amplifier control signal monitor register (AMPMON) 
*/
/* Status of amplifier unit 3 (AMPMON3) */
#define _08_AMPMON3_OPERATING                 (0x08U) /* an amplifier unit 3 is operating */
/* Status of amplifier unit 2 (AMPMON2) */
#define _04_AMPMON2_OPERATING                 (0x04U) /* an amplifier unit 2 is operating */
/* Status of amplifier unit 1 (AMPMON1) */
#define _02_AMPMON1_OPERATING                 (0x02U) /* an amplifier unit 1 is operating */
/* Status of amplifier unit 0 (AMPMON0) */
#define _01_AMPMON0_OPERATING                 (0x01U) /* an amplifier unit 0 is operating */

/*
    Amplifier unit 0 gain setting register (PGA1GC) 
*/
/* Specification of the gain for the pre-amplifiers of PGA1 (GSET11) (PGA1GC5,PGA1GC4) */
#define _00_PGA1_GSET11_1                     (0x00U) /* GSET11 = 12 */
#define _10_PGA1_GSET11_2                     (0x10U) /* GSET11 = 16 */
#define _20_PGA1_GSET11_3                     (0x20U) /* GSET11 = 20 */
#define _30_PGA1_GSET11_4                     (0x30U) /* GSET11 = 24 */
/* Specification of the gain for the final amplifiers of PGA1 (GSET12) (PGA1GC3,PGA1GC2,PGA1GC1,PGA1GC0) */
#define _07_PGA1_GSET12_1                     (0x07U) /* GSET12 = 1.246 (+24.6%) */
#define _06_PGA1_GSET12_2                     (0x06U) /* GSET12 = 1.207 (+20.7%) */
#define _05_PGA1_GSET12_3                     (0x05U) /* GSET12 = 1.169 (+16.9%) */
#define _04_PGA1_GSET12_4                     (0x04U) /* GSET12 = 1.133 (+13.3%) */
#define _03_PGA1_GSET12_5                     (0x03U) /* GSET12 = 1.098 (+9.8%) */
#define _02_PGA1_GSET12_6                     (0x02U) /* GSET12 = 1.065 (+6.5%) */
#define _01_PGA1_GSET12_7                     (0x01U) /* GSET12 = 1.032 (+3.2%) */
#define _00_PGA1_GSET12_8                     (0x00U) /* GSET12 = 1.000 (0.0%) */
#define _0F_PGA1_GSET12_9                     (0x0FU) /* GSET12 = 0.969 (-3.1%) */
#define _0E_PGA1_GSET12_10                    (0x0EU) /* GSET12 = 0.939 (-6.1%) */
#define _0D_PGA1_GSET12_11                    (0x0DU) /* GSET12 = 0.910 (-9.0%) */
#define _0C_PGA1_GSET12_12                    (0x0CU) /* GSET12 = 0.882 (-11.8%) */
#define _0B_PGA1_GSET12_13                    (0x0BU) /* GSET12 = 0.855 (-14.5%) */
#define _0A_PGA1_GSET12_14                    (0x0AU) /* GSET12 = 0.829 (-17.1%) */
#define _09_PGA1_GSET12_15                    (0x09U) /* GSET12 = 0.803 (-19.7%) */
#define _08_PGA1_GSET12_16                    (0x08U) /* GSET12 = 0.778 (-22.2%) */

/*
    Amplifier unit 0 input select register (PGA1S) 
*/
/* Selection of the source of the connection to the input terminal of PGA1 (PGA1CS1,PGA1CS0) */
#define _01_PGA1_PGA10N_PGA10P                (0x01U) /* connect the PGA10P, PGA10N pin of PGA1 */
#define _02_PGA1_PGA11N_PGA11P                (0x02U) /* connect the PGA11P, PGA11N pin of PGA1 */

/*
    Amplifier unit 1 input select register (AMP0S) 
*/
/* Selection of the source of the connection to AMP0 (AMP0NFB,AMP0NS0) */
#define _40_AMP0_AMP0N_INPUT4                 (0x40U) /* connect the AMP0N pin (multiplexed AFE input 4) of AMP0 */
#define _80_AMP0_AMP0_FED                     (0x80U) /* the output from AMP0 is fed back */
/* Selection of the source of the connection to  AMP0 (AMP0PDA,AMP0PS2,AMP0PS1,AMP0PS0) */
#define _01_AMP0_AMP0P_INPUT4                 (0x01U) /* connect the AMP0P pin (multiplexed AFE input 4) of AMP0 */
#define _02_AMP0_AMP0P_INPUT5                 (0x02U) /* connect the AMP0P pin (multiplexed AFE input 5) of AMP0 */
#define _04_AMP0_AMP0P_INPUT7                 (0x04U) /* connect the AMP0P pin (multiplexed AFE input 7) of AMP0 */
#define _10_AMP0_DAC0                         (0x10U) /* connect 8-bit D/A converter (DAC0) of AMP0 */

/*
    Amplifier unit 2 input select register (AMP1S) 
*/
/* Selection of the source of the connection to the negative input terminal of AMP1 (AMP1NFB,AMP1NS0) */
#define _40_AMP1_AMP1N_INPUT5                 (0x40U) /* connect the AMP1N pin (multiplexed AFE input 5) of AMP1 */
#define _80_AMP1_AMP1_FED                     (0x80U) /* the output from AMP1 is fed back */
/* Selection of the source of the connection to AMP1 (AMP1PVR,AMP1PDA,AMP1PS2,AMP1PS1,AMP1PS0) */
#define _01_AMP1_AMP1P_INPUT5                 (0x01U) /* connect the AMP1P pin (multiplexed AFE input 5) of AMP1 */
#define _02_AMP1_AMP1P_INPUT6                 (0x02U) /* connect the AMP1P pin (multiplexed AFE input 6) of AMP1 */
#define _04_AMP1_AMP1P_INPUT0                 (0x04U) /* connect the AMP1P pin (multiplexed AFE input 0) of AMP1 */
#define _10_AMP1_DAC1                         (0x10U) /* connect 12-bit D/A converter (DAC1) of AMP1 */
#define _20_AMP1_VBIAS                        (0x20U) /* connect the internal bias voltage (VBIAS) of AMP1 */

/*
    Amplifier unit 3 input select register (AMP2S) 
*/
/* Selection of the source of the connection to the negative input terminal of AMP2 (AMP2NFB,AMP2NS0) */
#define _40_AMP2_AMP2N_INPUT6                 (0x40U) /* connect the AMP2N pin (multiplexed AFE input 6) of AMP2 */
#define _80_AMP2_AMP2_FED                     (0x80U) /* the output from AMP2 is fed back */
/* Selection of the source of the connection to AMP2 (AMP2PVR,AMP2PDA,AMP2PS2,AMP2PS0) */
#define _01_AMP2_AMP2P_INPUT6                 (0x01U) /* connect AMP2P pin (multiplexed AFE input 6) of AMP2 */
#define _04_AMP2_AMP2P_INPUT1                 (0x04U) /* connect AMP2P pin (multiplexed AFE input 1) of AMP2 */
#define _10_AMP2_DAC1                         (0x10U) /* connect 12-bit D/A converter (DAC1) of AMP2 */
#define _20_AMP2_VBIAS                        (0x20U) /* connect the internal bias voltage (VBIAS) of AMP2 */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_AMP_Create(void);
void R_PGA1_Start(void);
void R_PGA1_Stop(void);
void R_AMP0_Start(void);
void R_AMP0_Stop(void);
void R_AMP2_Start(void);
void R_AMP2_Stop(void);
void R_AMP_Set_PowerOn(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

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
* File Name    : r_cg_pga_dsad.h
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for PGIA module.
* Creation Date: 2017/06/01
***********************************************************************************************************************/
#ifndef PGIA_H
#define PGIA_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Configurable amplifier 0 output selection register (AMP0S0) 
*/
/* Selection of offset trimming monitor (AMPMONI1,AMPMONI0) */
#define _00_DSAD_OFFSET_TRIMMING_MONITOR_NONE                 (0x00U) /* no connection */
#define _40_DSAD_OFFSET_TRIMMING_MONITOR_AMP0                 (0x40U) /* input positive input signal and output
                                                                         signal of AMP 0 to the PGA */
#define _80_DSAD_OFFSET_TRIMMING_MONITOR_AMP1                 (0x80U) /* input positive input signal and output
                                                                         signal of AMP 1 to the PGA */
#define _C0_DSAD_OFFSET_TRIMMING_MONITOR_AMP2                 (0xC0U) /* input positive input signal and output
                                                                         signal of AMP 2 to the PGA */

/*
    D/A converter mode register 0 (DACM0) 
*/
/* Selection of trigger mode (DACTMD) */
#define _00_DSAD_DAC_TRIG_SOFTWARE                            (0x00U) /* software trigger mode */
#define _80_DSAD_DAC_TRIG_HARDWARE                            (0x08U) /* hardware trigger mode */

/* Selection of D/A converter resolution (DACRES) */
#define _00_DSAD_DAC_BIT_12                                   (0x00U) /* 12 bits */
#define _01_DSAD_DAC_BIT_8                                    (0x01U) /* 8 bits */

/*
    D/A converter mode register 1 (DACM1) 
*/
/* Selection of D/A converter reference voltage source (DACVRF) */
#define _00_DSAD_DAC_AVDD                                     (0x00U) /* 5 V voltage supplied: select AVDD */
#define _01_DSAD_DAC_VREFDA                                   (0x01U) /* 1.45 V voltage supplied: select VREFDA */

/*
    Sensor reference voltage setting register (VSBIAS) 
*/
/* SBIAS output voltage (VSBIAS4,VSBIAS3,VSBIAS2,VSBIAS1,VSBIAS0) */
#define _09_VSBIAS_OUTPUT_VOLTAGE_05                          (0x09U) /* output voltage 0.5V */
#define _0A_VSBIAS_OUTPUT_VOLTAGE_06                          (0x0AU) /* output voltage 0.6V */
#define _0B_VSBIAS_OUTPUT_VOLTAGE_07                          (0x0BU) /* output voltage 0.7V */
#define _0C_VSBIAS_OUTPUT_VOLTAGE_08                          (0x0CU) /* output voltage 0.8V */
#define _0D_VSBIAS_OUTPUT_VOLTAGE_09                          (0x0DU) /* output voltage 0.9V */
#define _0E_VSBIAS_OUTPUT_VOLTAGE_10                          (0x0EU) /* output voltage 1.0V */
#define _0F_VSBIAS_OUTPUT_VOLTAGE_11                          (0x0FU) /* output voltage 1.1V */
#define _10_VSBIAS_OUTPUT_VOLTAGE_12                          (0x10U) /* output voltage 1.2V */
#define _11_VSBIAS_OUTPUT_VOLTAGE_13                          (0x11U) /* output voltage 1.3V */
#define _12_VSBIAS_OUTPUT_VOLTAGE_14                          (0x12U) /* output voltage 1.4V */
#define _13_VSBIAS_OUTPUT_VOLTAGE_15                          (0x13U) /* output voltage 1.5V */
#define _14_VSBIAS_OUTPUT_VOLTAGE_16                          (0x14U) /* output voltage 1.6V */
#define _15_VSBIAS_OUTPUT_VOLTAGE_17                          (0x15U) /* output voltage 1.7V */
#define _16_VSBIAS_OUTPUT_VOLTAGE_18                          (0x16U) /* output voltage 1.8V */
#define _17_VSBIAS_OUTPUT_VOLTAGE_19                          (0x17U) /* output voltage 1.9V */
#define _18_VSBIAS_OUTPUT_VOLTAGE_20                          (0x18U) /* output voltage 2.0V */
#define _19_VSBIAS_OUTPUT_VOLTAGE_21                          (0x19U) /* output voltage 2.1V */
#define _1A_VSBIAS_OUTPUT_VOLTAGE_22                          (0x1AU) /* output voltage 2.2V */

/*
    Analog front-end clock selection register (AFECKS) 
*/
/* Analog front-end clock selection register (AFECKS3,AFECKS2,AFECKS1,AFECKS0) */
#define _00_AFE_CLOCK_STOP                                    (0x00U) /* stop the clock output (default) */
#define _08_AFE_CLOCK_UNDIVIDED                               (0x08U) /* undivided */
#define _09_AFE_CLOCK_DIVIDED2                                (0x09U) /* divided by 2 */
#define _0A_AFE_CLOCK_DIVIDED3                                (0x0AU) /* divided by 3 */
#define _0B_AFE_CLOCK_DIVIDED4                                (0x0BU) /* divided by 4 */
#define _0C_AFE_CLOCK_DIVIDED5                                (0x0CU) /* divided by 5 */
#define _0D_AFE_CLOCK_DIVIDED6                                (0x0DU) /* divided by 6 */
#define _0E_AFE_CLOCK_DIVIDED8                                (0x0EU) /* divided by 8 */

/*
    DS A/D converter mode register (DSADMR) 
*/
/* Selection of A/D conversion trigger signal (DSADTMD) */
#define _00_DSAD_CONVTRI_SOFTWARE                             (0x00U) /* software trigger */
#define _80_DSAD_CONVTRI_HARDWARE                             (0x80U) /* hardware trigger */
/* Selection of A/D conversion mode (DSADLPM) */
#define _00_DSAD_CONVMODE_NORMAL                              (0x00U) /* normal operation */
#define _40_DSAD_CONVMODE_LOW_POWER                           (0x40U) /* low power mode */

/*
    DS A/D converter control register (DSADCTL) 
*/
/* Control of A/D conversion (AUTOSCAN) (DSADST) */
#define _00_DSAD_CONVERSION_STOP                              (0x00U) /* stop conversion */
#define _80_DSAD_CONVERSION_START                             (0x80U) /* start conversion */
/* Selection of autoscan mode (DSADSCM) */
#define _00_DSAD_SCAN_MODE_SEQUENTIAL                         (0x00U) /* sequential conversion mode */
#define _20_DSAD_SCAN_MODE_STOP                               (0x20U) /* stop each time Scan mode is entered */
/* Signal from input multiplexer 3 (DSADBMP3) */
#define _00_DSAD_INPUT3_ENABLE                                (0x00U) /* enable signal from input multiplexer 3 */
#define _08_DSAD_INPUT3_DISABLE                               (0x08U) /* disable signal from input multiplexer 3 */
/* Signal from input multiplexer 2 (DSADBMP2) */
#define _00_DSAD_INPUT2_ENABLE                                (0x00U) /* enable signal from input multiplexer 2 */
#define _04_DSAD_INPUT2_DISABLE                               (0x04U) /* disable signal from input multiplexer 2 */
/* Signal from input multiplexer 1 (DSADBMP1) */
#define _00_DSAD_INPUT1_ENABLE                                (0x00U) /* enable signal from input multiplexer 1 */
#define _02_DSAD_INPUT1_DISABLE                               (0x02U) /* disable signal from input multiplexer 1 */
/* Signal from input multiplexer 0 (DSADBMP0) */
#define _00_DSAD_INPUT0_ENABLE                                (0x00U) /* enable signal from input multiplexer 0 */
#define _01_DSAD_INPUT0_DISABLE                               (0x01U) /* eisable signal from input multiplexer 0 */

/*
    Disconnection detection setting register (PGABOD) 
*/
/* SControl of disconnection detection (PGABOD0) */
#define _00_DSAD_DISCONNECTION_DETECTION_UNUSED               (0x00U) /* disconnection detection not used */
#define _01_DSAD_DISCONNECTION_DETECTION_USED                 (0x01U) /* enable disconnection detection */

/*
    Input multiplexer n setting register 0 (PGAnCTL0) 
*/
/* OSR (oversampling ratio) (PGAnOSR2,PGAnOSR1,PGAnOSR0) */
#define _00_PGA_CHANNELn_SAMPLE_RATIO_64                      (0x00U) /* sample reatio 64 */
#define _20_PGA_CHANNELn_SAMPLE_RATIO_128                     (0x20U) /* sample reatio 128 */
#define _40_PGA_CHANNELn_SAMPLE_RATIO_256                     (0x40U) /* sample reatio 256 */
#define _60_PGA_CHANNELn_SAMPLE_RATIO_512                     (0x60U) /* sample reatio 512 */
#define _80_PGA_CHANNELn_SAMPLE_RATIO_1024                    (0x80U) /* sample reatio 1024 */
#define _A0_PGA_CHANNELn_SAMPLE_RATIO_2048                    (0xA0U) /* sample reatio 2048 */
/* Gain of Gset1 (PGAnGC4,PGAnGC3,PGAnGC2) */
#define _00_PGA_CHANNELn_GSET1_1                              (0x00U) /* gain 1 */
#define _04_PGA_CHANNELn_GSET1_2                              (0x04U) /* gain 2 */
#define _08_PGA_CHANNELn_GSET1_3                              (0x08U) /* gain 3 */
#define _0C_PGA_CHANNELn_GSET1_4                              (0x0CU) /* gain 4 */
#define _10_PGA_CHANNELn_GSET1_8                              (0x10U) /* gain 8 */
/* Gain of Gset2 (PGAnGC1,PGAnGC0) */
#define _00_PGA_CHANNELn_GSET2_1                              (0x00U) /* gain 1 */
#define _01_PGA_CHANNELn_GSET2_2                              (0x01U) /* gain 2 */
#define _02_PGA_CHANNELn_GSET2_4                              (0x02U) /* gain 4 */
#define _03_PGA_CHANNELn_GSET2_8                              (0x03U) /* gain 8 */

/*
    Input multiplexer n setting register 1 (PGAnCTL1) 
*/
/* Control of input multiplexer n (PGAnSEL) */
#define _00_PGA_CHANNELn_DIFFERENTIAL                         (0x00U) /* differential input */
#define _80_PGA_CHANNELn_SINGLEEND                            (0x80U) /* single-ended input */
/* Control of input multiplexer n (PGAnTSEL) */
#define _00_PGA_CHANNELn_PIN                                  (0x00U) /* channeln input mode */
#define _40_PGA_CHANNELn_OFFSET                               (0x40U) /* offset of AMP or self-diagnosis for PGA */
/* offset voltage for input multiplexer n (PGAnOFS4,PGAnOFS3,PGAnOFS2,PGAnOFS1,PGAnOFS0) */
#define _01_PGA_CHANNELn_OFFSET_1                             (0x01U) /* -164.06/GSET1 */
#define _02_PGA_CHANNELn_OFFSET_2                             (0x02U) /* -153.13/GSET1 */
#define _03_PGA_CHANNELn_OFFSET_3                             (0x03U) /* -142.12/GSET1 */
#define _04_PGA_CHANNELn_OFFSET_4                             (0x04U) /* -131.25/GSET1 */
#define _05_PGA_CHANNELn_OFFSET_5                             (0x05U) /* -120.31/GSET1 */
#define _06_PGA_CHANNELn_OFFSET_6                             (0x06U) /* -109.38/GSET1 */
#define _07_PGA_CHANNELn_OFFSET_7                             (0x07U) /* -98.44/GSET1 */
#define _08_PGA_CHANNELn_OFFSET_8                             (0x08U) /* -87.50/GSET1 */
#define _09_PGA_CHANNELn_OFFSET_9                             (0x09U) /* -76.57/GSET1 */
#define _0A_PGA_CHANNELn_OFFSET_10                            (0x0AU) /* -65.63/GSET1 */
#define _0B_PGA_CHANNELn_OFFSET_11                            (0x0BU) /* -54.69/GSET1 */
#define _0C_PGA_CHANNELn_OFFSET_12                            (0x0CU) /* -43.75/GSET1 */
#define _0D_PGA_CHANNELn_OFFSET_13                            (0x0DU) /* -32.81/GSET1 */
#define _0E_PGA_CHANNELn_OFFSET_14                            (0x0EU) /* -21.88/GSET1 */
#define _0F_PGA_CHANNELn_OFFSET_15                            (0x0FU) /* -10.94/GSET1 */
#define _10_PGA_CHANNELn_OFFSET_16                            (0x10U) /* -0/GSET1 */
#define _11_PGA_CHANNELn_OFFSET_17                            (0x11U) /* 10.94/GSET1 */
#define _12_PGA_CHANNELn_OFFSET_18                            (0x12U) /* 21.88/GSET1 */
#define _13_PGA_CHANNELn_OFFSET_19                            (0x13U) /* 32.81/GSET1 */
#define _14_PGA_CHANNELn_OFFSET_20                            (0x14U) /* 43.75/GSET1 */
#define _15_PGA_CHANNELn_OFFSET_21                            (0x15U) /* 54.69/GSET1 */
#define _16_PGA_CHANNELn_OFFSET_22                            (0x16U) /* 65.63/GSET1 */
#define _17_PGA_CHANNELn_OFFSET_23                            (0x17U) /* 76.57/GSET1 */
#define _18_PGA_CHANNELn_OFFSET_24                            (0x18U) /* 87.50/GSET1 */
#define _19_PGA_CHANNELn_OFFSET_25                            (0x19U) /* 98.44/GSET1 */
#define _1A_PGA_CHANNELn_OFFSET_26                            (0x1AU) /* 109.38/GSET1 */
#define _1B_PGA_CHANNELn_OFFSET_27                            (0x1BU) /* 120.31/GSET1 */
#define _1C_PGA_CHANNELn_OFFSET_28                            (0x1CU) /* 131.25/GSET1 */
#define _1D_PGA_CHANNELn_OFFSET_29                            (0x1DU) /* 142.12/GSET1 */
#define _1E_PGA_CHANNELn_OFFSET_30                            (0x1EU) /* 153.13/GSET1 */
#define _1F_PGA_CHANNELn_OFFSET_31                            (0x1FU) /* 164.06/GSET1 */

/*
    Input multiplexer n setting register 3 (PGAnCTL3) 
*/
/* Selection of the mode for specifying the number of A/D conversions (PGAnCTM) */
#define _00_PGA_CHANNELn_MUMBER_MODE_8032                     (0x00U) /* specify 1 to 8,032 times by using 8 bits */
#define _80_PGA_CHANNELn_MUMBER_MODE_255                      (0x80U) /* specify 1 to 255 times lineally by using 8 bits */
/* Selection of averaging processing (PGAnAVE3,PGAnAVE2) */
#define _00_PGA_CHANNELn_AVERAGING_NOT                        (0x00U) /* do not average the A/D conversion results */
#define _08_PGA_CHANNELn_AVERAGING_CONVERSION_OCCURS          (0x08U) /* average each time an A/D conversion occurs. */
#define _0C_PGA_CHANNELn_AVERAGING_MEAM_VALUE                 (0x0CU) /* averageeach time the mean value is updated */
/* Selection of the number of data units to be ageraged (PGAnAVE1,PGAnAVE0) */
#define _00_PGA_CHANNELn_AVERAGING_UNIT_8                     (0x00U) /* average uint 8 */
#define _01_PGA_CHANNELn_AVERAGING_UNIT_16                    (0x01U) /* average uint 16 */
#define _02_PGA_CHANNELn_AVERAGING_UNIT_32                    (0x02U) /* average uint 32 */
#define _03_PGA_CHANNELn_AVERAGING_UNIT_64                    (0x03U) /* average uint 64 */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _01_PGA_CHANNEL0_SAMPLE_COUNT                         (0x01U) /* sample count per AUTOSCAN cycle for channel 0 */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_PGA_DSAD_Create(void);
void R_PGA_DSAD_Start(void);
void R_PGA_DSAD_Stop(void);
void R_PGA_DSAD_Get_Result(uint16_t * const bufferH,uint16_t * const bufferL);
void R_PGA_DSAD_Get_AverageResult(uint16_t * const bufferH,uint16_t * const bufferL);
void R_PGA_DSAD_CAMP_OffsetTrimming(void);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

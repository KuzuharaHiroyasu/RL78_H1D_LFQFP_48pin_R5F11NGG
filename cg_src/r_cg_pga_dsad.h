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
* File Name    : r_cg_pga_dsad.h
* Version      :  
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for PGIA module.
* Creation Date: 2018/04/18
***********************************************************************************************************************/
#ifndef PGIA_H
#define PGIA_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Analog front-end clock selection register (AFECKS) 
*/
/* Selection of AFE operating clock (AFECKS3,AFECKS2,AFECKS1,AFECKS0) */
#define _08_AFE_CLOCK_UNDIVIDED                             (0x08U) /* undivided */
#define _09_AFE_CLOCK_DIVIDED2                              (0x09U) /* divided by 2 */
#define _0A_AFE_CLOCK_DIVIDED3                              (0x0AU) /* divided by 3 */
#define _0B_AFE_CLOCK_DIVIDED4                              (0x0BU) /* divided by 4 */
#define _0C_AFE_CLOCK_DIVIDED5                              (0x0CU) /* divided by 5 */
#define _0D_AFE_CLOCK_DIVIDED6                              (0x0DU) /* divided by 6 */
#define _0E_AFE_CLOCK_DIVIDED8                              (0x0EU) /* divided by 8 */

/*
    Sensor reference voltage setting register (VSBIAS) 
*/
/* SBIAS output voltage (VSBIAS4,VSBIAS3,VSBIAS2,VSBIAS1,VSBIAS0) */
#define _09_VSBIAS_OUTPUT_VOLTAGE_05                        (0x09U) /* output voltage 0.5V */
#define _0A_VSBIAS_OUTPUT_VOLTAGE_06                        (0x0AU) /* output voltage 0.6V */
#define _0B_VSBIAS_OUTPUT_VOLTAGE_07                        (0x0BU) /* output voltage 0.7V */
#define _0C_VSBIAS_OUTPUT_VOLTAGE_08                        (0x0CU) /* output voltage 0.8V */
#define _0D_VSBIAS_OUTPUT_VOLTAGE_09                        (0x0DU) /* output voltage 0.9V */
#define _0E_VSBIAS_OUTPUT_VOLTAGE_10                        (0x0EU) /* output voltage 1.0V */
#define _0F_VSBIAS_OUTPUT_VOLTAGE_11                        (0x0FU) /* output voltage 1.1V */
#define _10_VSBIAS_OUTPUT_VOLTAGE_12                        (0x10U) /* output voltage 1.2V */
#define _11_VSBIAS_OUTPUT_VOLTAGE_13                        (0x11U) /* output voltage 1.3V */
#define _12_VSBIAS_OUTPUT_VOLTAGE_14                        (0x12U) /* output voltage 1.4V */
#define _13_VSBIAS_OUTPUT_VOLTAGE_15                        (0x13U) /* output voltage 1.5V */
#define _14_VSBIAS_OUTPUT_VOLTAGE_16                        (0x14U) /* output voltage 1.6V */
#define _15_VSBIAS_OUTPUT_VOLTAGE_17                        (0x15U) /* output voltage 1.7V */
#define _16_VSBIAS_OUTPUT_VOLTAGE_18                        (0x16U) /* output voltage 1.8V */
#define _17_VSBIAS_OUTPUT_VOLTAGE_19                        (0x17U) /* output voltage 1.9V */
#define _18_VSBIAS_OUTPUT_VOLTAGE_20                        (0x18U) /* output voltage 2.0V */
#define _19_VSBIAS_OUTPUT_VOLTAGE_21                        (0x19U) /* output voltage 2.1V */
#define _1A_VSBIAS_OUTPUT_VOLTAGE_22                        (0x1AU) /* output voltage 2.2V */

/*
    DS A/D converter mode register (DSADMR) 
*/
/* Selection of A/D conversion trigger signal (DSADTMD) */
#define _00_PGA_DSAD_TRIGGER_SOFTWARE                       (0x00U) /* software trigger */
#define _80_PGA_DSAD_TRIGGER_HARDWARE                       (0x80U) /* hardware trigger */
/* Selection of A/D conversion mode (DSADLPM) */
#define _00_PGA_DSAD_MODE_NORMAL                            (0x00U) /* normal mode */
#define _40_PGA_DSAD_MODE_LOW_POWER                         (0x40U) /* low power mode */

/*
    Input multiplexer n setting register 0 (PGAnCTL0) 
*/
/* Oversampling ratio (PGAnOSR2,PGAnOSR1,PGAnOSR0) */
#define _00_PGA_MULTIPLEXERn_SAMPLE_RATIO_64                (0x00U) /* sample reatio 64 */
#define _20_PGA_MULTIPLEXERn_SAMPLE_RATIO_128               (0x20U) /* sample reatio 128 */
#define _40_PGA_MULTIPLEXERn_SAMPLE_RATIO_256               (0x40U) /* sample reatio 256 */
#define _60_PGA_MULTIPLEXERn_SAMPLE_RATIO_512               (0x60U) /* sample reatio 512 */
#define _80_PGA_MULTIPLEXERn_SAMPLE_RATIO_1024              (0x80U) /* sample reatio 1024 */
#define _A0_PGA_MULTIPLEXERn_SAMPLE_RATIO_2048              (0xA0U) /* sample reatio 2048 */
/* Gain of gset01 (PGAnGC4,PGAnGC3,PGAnGC2) */
#define _00_PGA_MULTIPLEXERn_GSET01_1                       (0x00U) /* gain 1 */
#define _04_PGA_MULTIPLEXERn_GSET01_2                       (0x04U) /* gain 2 */
#define _08_PGA_MULTIPLEXERn_GSET01_3                       (0x08U) /* gain 3 */
#define _0C_PGA_MULTIPLEXERn_GSET01_4                       (0x0CU) /* gain 4 */
#define _10_PGA_MULTIPLEXERn_GSET01_8                       (0x10U) /* gain 8 */
/* Gain of gset02 (PGAnGC1,PGAnGC0) */
#define _00_PGA_MULTIPLEXERn_GSET02_1                       (0x00U) /* gain 1 */
#define _01_PGA_MULTIPLEXERn_GSET02_2                       (0x01U) /* gain 2 */
#define _02_PGA_MULTIPLEXERn_GSET02_4                       (0x02U) /* gain 4 */
#define _03_PGA_MULTIPLEXERn_GSET02_8                       (0x03U) /* gain 8 */

/*
    Input multiplexer n setting register 1 (PGAnCTL1) 
*/
/* Offset voltage for input multiplexer n (PGAnOFS4,PGAnOFS3,PGAnOFS2,PGAnOFS1,PGAnOFS0) */
#define _01_PGA_MULTIPLEXERn_OFFSET_1                       (0x01U) /* -164.06/GSET01 */
#define _02_PGA_MULTIPLEXERn_OFFSET_2                       (0x02U) /* -153.13/GSET01 */
#define _03_PGA_MULTIPLEXERn_OFFSET_3                       (0x03U) /* -142.12/GSET01 */
#define _04_PGA_MULTIPLEXERn_OFFSET_4                       (0x04U) /* -131.25/GSET01 */
#define _05_PGA_MULTIPLEXERn_OFFSET_5                       (0x05U) /* -120.31/GSET01 */
#define _06_PGA_MULTIPLEXERn_OFFSET_6                       (0x06U) /* -109.38/GSET01 */
#define _07_PGA_MULTIPLEXERn_OFFSET_7                       (0x07U) /* -98.44/GSET01 */
#define _08_PGA_MULTIPLEXERn_OFFSET_8                       (0x08U) /* -87.50/GSET01 */
#define _09_PGA_MULTIPLEXERn_OFFSET_9                       (0x09U) /* -76.57/GSET01 */
#define _0A_PGA_MULTIPLEXERn_OFFSET_10                      (0x0AU) /* -65.63/GSET01 */
#define _0B_PGA_MULTIPLEXERn_OFFSET_11                      (0x0BU) /* -54.69/GSET01 */
#define _0C_PGA_MULTIPLEXERn_OFFSET_12                      (0x0CU) /* -43.75/GSET01 */
#define _0D_PGA_MULTIPLEXERn_OFFSET_13                      (0x0DU) /* -32.81/GSET01 */
#define _0E_PGA_MULTIPLEXERn_OFFSET_14                      (0x0EU) /* -21.88/GSET01 */
#define _0F_PGA_MULTIPLEXERn_OFFSET_15                      (0x0FU) /* -10.94/GSET01 */
#define _10_PGA_MULTIPLEXERn_OFFSET_16                      (0x10U) /* 0/GSET01 */
#define _11_PGA_MULTIPLEXERn_OFFSET_17                      (0x11U) /* 10.94/GSET01 */
#define _12_PGA_MULTIPLEXERn_OFFSET_18                      (0x12U) /* 21.88/GSET01 */
#define _13_PGA_MULTIPLEXERn_OFFSET_19                      (0x13U) /* 32.81/GSET01 */
#define _14_PGA_MULTIPLEXERn_OFFSET_20                      (0x14U) /* 43.75/GSET01 */
#define _15_PGA_MULTIPLEXERn_OFFSET_21                      (0x15U) /* 54.69/GSET01 */
#define _16_PGA_MULTIPLEXERn_OFFSET_22                      (0x16U) /* 65.63/GSET01 */
#define _17_PGA_MULTIPLEXERn_OFFSET_23                      (0x17U) /* 76.57/GSET01 */
#define _18_PGA_MULTIPLEXERn_OFFSET_24                      (0x18U) /* 87.50/GSET01 */
#define _19_PGA_MULTIPLEXERn_OFFSET_25                      (0x19U) /* 98.44/GSET01 */
#define _1A_PGA_MULTIPLEXERn_OFFSET_26                      (0x1AU) /* 109.38/GSET01 */
#define _1B_PGA_MULTIPLEXERn_OFFSET_27                      (0x1BU) /* 120.31/GSET01 */
#define _1C_PGA_MULTIPLEXERn_OFFSET_28                      (0x1CU) /* 131.25/GSET01 */
#define _1D_PGA_MULTIPLEXERn_OFFSET_29                      (0x1DU) /* 142.12/GSET01 */
#define _1E_PGA_MULTIPLEXERn_OFFSET_30                      (0x1EU) /* 153.13/GSET01 */
#define _1F_PGA_MULTIPLEXERn_OFFSET_31                      (0x1FU) /* 164.06/GSET01 */
/* Control of input multiplexer 0 (PGA0SEL) */
#define _00_PGA_MULTIPLEXER0_DIFFERENTIAL                   (0x00U) /* differential input */
#define _80_PGA_MULTIPLEXER0_SINGLEEND                      (0x80U) /* single-ended input */
/* Control of input multiplexer 3 (PGA3SEL1,PGA3SEL0) */
#define _00_PGA_MULTIPLEXER3_INTERNAL_SINGLEEND             (0x00U) /* single-ended input */
#define _40_PGA_MULTIPLEXER3_EXTERNAL_DIFFERENTIAL          (0x40U) /* differential input */
#define _C0_PGA_MULTIPLEXER3_EXTERNAL_SINGLEEND             (0xC0U) /* single-ended input */

/*
    Disconnection detection setting register (PGABOD) 
*/
/* Control of disconnection detection (PGABOD0) */
#define _00_DSAD_DISCONNECTION_DETECTION_UNUSED             (0x00U) /* normal operation */
#define _01_DSAD_DISCONNECTION_DETECTION_USED               (0x01U) /* state of disconnection detection */

/*
    DS A/D converter control register (DSADCTL) 
*/
/* Control of A/D conversion (DSADST) */
#define _00_DSAD_CONVERSION_STOP                            (0x00U) /* stop A/D conversion */
#define _80_DSAD_CONVERSION_START                           (0x80U) /* start A/D conversion */
/* Selection of stabilization wait time for starting the A/D conversion or switching the channel (DSADSWM) */
#define _00_DSAD_STABILIZATION_TIME_3T                      (0x00U) /* 128 usec + 3T */
#define _40_DSAD_STABILIZATION_TIME_4T_5T                   (0x40U) /* 4T or 5T */
/* Selection of the autoscan mode (DSADSCM) */
#define _00_DSAD_SCAN_MODE_CONTINUOUS                       (0x00U) /* continuous scan mode */
#define _20_DSAD_SCAN_MODE_SINGLE                           (0x20U) /* single scan mode */
/* Signal from input multiplexer 0 (DSADBMP0) */
#define _00_DSAD_INPUT0_ENABLE                              (0x00U) /* enable signal from input multiplexer 0 */
#define _01_DSAD_INPUT0_DISABLE                             (0x01U) /* disable signal from input multiplexer 0 */
/* Signal from input multiplexer 1 (DSADBMP1) */
#define _00_DSAD_INPUT1_ENABLE                              (0x00U) /* enable signal from input multiplexer 1 */
#define _02_DSAD_INPUT1_DISABLE                             (0x02U) /* disable signal from input multiplexer 1 */
/* Signal from input multiplexer 2 (DSADBMP2) */
#define _00_DSAD_INPUT2_ENABLE                              (0x00U) /* enable signal from input multiplexer 2 */
#define _04_DSAD_INPUT2_DISABLE                             (0x04U) /* disable signal from input multiplexer 2 */
/* Signal from input multiplexer 3 (DSADBMP3) */
#define _00_DSAD_INPUT3_ENABLE                              (0x00U) /* enable signal from input multiplexer 3 */
#define _08_DSAD_INPUT3_DISABLE                             (0x08U) /* disable signal from input multiplexer 3 */

/*
    Input multiplexer n setting register 3 (PGAnCTL3) 
*/
/* Selection of the mode for specifying the number of A/D conversions (PGAnCTM) */
#define _00_PGA_MULTIPLEXERn_MUMBER_MODE_8032               (0x00U) /* specify 1 to 8,032 times by using 8 bits */
#define _80_PGA_MULTIPLEXERn_MUMBER_MODE_255                (0x80U) /* specify 1 to 255 times by using 8 bits */
/* Selection of averaging processing (PGAnAVE3,PGAnAVE2) */
#define _00_PGA_MULTIPLEXERn_AVG_NOT                        (0x00U) /* do not average the A/D conversion results */
#define _08_PGA_MULTIPLEXERn_AVG_CONVERSION_OCCURS          (0x08U) /* average each time an A/D conversion occurs */
#define _0C_PGA_MULTIPLEXERn_AVG_MEAM_VALUE                 (0x0CU) /* average each time the mean value is updated */
/* Selection of the number of data units to be averaged (PGAnAVE1,PGAnAVE0) */
#define _00_PGA_MULTIPLEXERn_AVG_UNIT_8                     (0x00U) /* average uint 8 */
#define _01_PGA_MULTIPLEXERn_AVG_UNIT_16                    (0x01U) /* average uint 16 */
#define _02_PGA_MULTIPLEXERn_AVG_UNIT_32                    (0x02U) /* average uint 32 */
#define _03_PGA_MULTIPLEXERn_AVG_UNIT_64                    (0x03U) /* average uint 64 */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _01_PGA_MULTIPLEXER0_SAMPLE_COUNT                   (0x01U) /* sample count per autoscan cycle */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_PGA_DSAD_Create(void);
void R_PGA_DSAD_Start(void);
void R_PGA_DSAD_Stop(void);
void R_PGA_DSAD_Get_Result(uint16_t * const bufferH, uint16_t * const bufferL);
void R_PGA_DSAD_Get_AverageResult(uint16_t * const bufferH, uint16_t * const bufferL);
void __near r_pga_dsad_conversion_interrupt(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

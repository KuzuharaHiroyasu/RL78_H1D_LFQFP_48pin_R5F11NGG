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
* File Name    : r_cg_adc.h
* Version      : Code Generator for RL78/G1H V1.00.00.04 [08 Mar 2016]
* Device(s)    : R5F11FLJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for ADC module.
* Creation Date: 2017/12/20
***********************************************************************************************************************/
#ifndef ADC_H
#define ADC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    A/D converter mode register 0 (ADM0) 
*/
/* A/D conversion operation control (ADCS) */
#define _00_AD_CONVERSION_DISABLE               (0x00U) /* stops conversion operation */
#define _80_AD_CONVERSION_ENABLE                (0x80U) /* enables conversion operation */
/* Specification of the A/D conversion channel selection mode (ADMD) */
#define _00_AD_OPERMODE_SELECT                  (0x00U) /* select mode */
/* A/D conversion clock selection (FR2,FR1,FR0) */
#define _00_AD_CONVERSION_CLOCK_64              (0x00U) /* fCLK/64 */
#define _08_AD_CONVERSION_CLOCK_32              (0x08U) /* fCLK/32 */
#define _10_AD_CONVERSION_CLOCK_16              (0x10U) /* fCLK/16 */
#define _18_AD_CONVERSION_CLOCK_8               (0x18U) /* fCLK/8 */
#define _20_AD_CONVERSION_CLOCK_6               (0x20U) /* fCLK/6 */
#define _28_AD_CONVERSION_CLOCK_5               (0x28U) /* fCLK/5 */
#define _30_AD_CONVERSION_CLOCK_4               (0x30U) /* fCLK/4 */
#define _38_AD_CONVERSION_CLOCK_2               (0x38U) /* fCLK/2 */
/* A/D conversion time mode selection (LV1,LV0) */
#define _00_AD_TIME_MODE_NORMAL_1               (0x00U) /* normal 1 mode */
#define _02_AD_TIME_MODE_NORMAL_2               (0x02U) /* normal 2 mode */
#define _04_AD_TIME_MODE_LOWVOLTAGE_1           (0x04U) /* low-voltage 1 mode */
#define _06_AD_TIME_MODE_LOWVOLTAGE_2           (0x06U) /* low-voltage 2 mode */
/* A/D voltage comparator operation control (ADCE) */
#define _00_AD_COMPARATOR_DISABLE               (0x00U) /* stops A/D voltage comparator operation */
#define _01_AD_COMPARATOR_ENABLE                (0x01U) /* enables A/D voltage comparator operation */

/*
    A/D converter mode register 1 (ADM1) 
*/
/* A/D conversion trigger mode selection (ADTMD1,ADTMD0) */
#define _00_AD_TRIGGER_SOFTWARE                 (0x00U) /* software trigger mode */
#define _40_AD_TRIGGER_SOFTWARE                 (0x40U) /* software trigger mode */
#define _80_AD_TRIGGER_HARDWARE_NOWAIT          (0x80U) /* hardware trigger mode (no wait) */
#define _C0_AD_TRIGGER_HARDWARE_WAIT            (0xC0U) /* hardware trigger mode (wait) */
/* AD convertion mode selection (ADSCM) */
#define _00_AD_CONVMODE_CONSELECT               (0x00U) /* sequential convertion mode */
#define _20_AD_CONVMODE_ONESELECT               (0x20U) /* one-shot convertion mode */
/* Trigger signal selection (ADTRS1, ADTRS0) (ADTRS1,ADTRS0) */
#define _00_AD_TRIGGER_INTTM01                  (0x00U) /* INTTM01 */
#define _01_AD_TRIGGER_ELC                      (0x01U) /* ELC */
#define _02_AD_TRIGGER_INTRTC                   (0x02U) /* INTRTC */
#define _03_AD_TRIGGER_INTIT                    (0x03U) /* INTIT */

/*
    A/D converter mode register 2 (ADM2) 
*/
/* AD VREF(+) selection (ADREFP0) */
#define _00_AD_POSITIVE_VDD                     (0x00U) /* use VDD as VREF(+) */
#define _40_AD_POSITIVE_AVREFP                  (0x40U) /* use AVREFP as VREF(+) */
/* AD VREF(-) selection (ADREFM) */
#define _00_AD_NEGATIVE_VSS                     (0x00U) /* use VSS as VREF(-) */
#define _20_AD_NEGATIVE_AVREFM                  (0x20U) /* use AVREFM as VREF(-) */
/* AD conversion result upper/lower bound value selection (ADRCK) */
#define _00_AD_AREA_MODE_1                      (0x00U) /* generates INTAD when ADLL <= ADCRH <= ADUL */
#define _08_AD_AREA_MODE_2_3                    (0x08U) /* generates INTAD when ADUL < ADCRH or ADLL > ADCRH */
/* AD wakeup function selection (AWC) */
#define _00_AD_SNOOZE_OFF                       (0x00U) /* stop SNOOZE function */
#define _04_AD_SNOOZE_ON                        (0x04U) /* use SNOOZE function */
/* AD resolution selection (ADTYP) */
#define _00_AD_RESOLUTION_10BIT                 (0x00U) /* 10 bits */
#define _01_AD_RESOLUTION_8BIT                  (0x01U) /* 8 bits */

/*
    Analog input channel specification register (ADS) 
*/
/* Specification of analog input channel (ADS4,ADS3,ADS2,ADS1,ADS0) */
#define _00_AD_INPUT_CHANNEL_0                  (0x00U) /* ANI0 */
#define _01_AD_INPUT_CHANNEL_1                  (0x01U) /* ANI1 */
#define _02_AD_INPUT_CHANNEL_2                  (0x02U) /* ANI2 */
#define _0D_AD_INPUT_CHANNEL_13                 (0x0DU) /* ANI13 */
#define _0E_AD_INPUT_CHANNEL_14                 (0x0EU) /* ANI14 */
#define _13_AD_INPUT_CHANNEL_19                 (0x13U) /* ANI19 */

/*
    AD test function register (ADTES) 
*/
/* AD test mode signal (ADTES1,ADTES0) */
#define _00_AD_NORMAL_INPUT                     (0x00U) /* normal mode */
#define _02_AD_TEST_AVREFM                      (0x02U) /* use AVREFM as test signal */
#define _03_AD_TEST_AVREFP                      (0x03U) /* use AVREFP as test signal */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define _FF_AD_ADUL_VALUE                       (0xFFU) 
#define _00_AD_ADLL_VALUE                       (0x00U) 


/* ���� */
// AD��� ADS�ɐݒ肷�ׂ��l���Z�b�g
#define AD_TYPE_ANI8			(0x08)
#define AD_TYPE_ANI9			(0x09)
#define AD_TYPE_ANI10			(0x0A)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef enum
{
    ADCHANNEL0,
    ADCHANNEL1,
    ADCHANNEL2,
    ADCHANNEL13 = 13U,
    ADCHANNEL14,
    ADCHANNEL19 = 19U,
} ad_channel_t;

typedef enum
{
    ADNORMALINPUT,
    ADAVREFM = 2U,
    ADAVREFP
} test_channel_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_ADC_Create(void);
void R_ADC_Start(void);
void R_ADC_Stop(void);
void R_ADC_Set_OperationOn(void);
void R_ADC_Set_OperationOff(void);
void R_ADC_Get_Result(uint16_t * const buffer);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
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
* File Name    : r_cg_intp.h
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for INTP module.
* Creation Date: 2017/06/01
***********************************************************************************************************************/
#ifndef INTP_H
#define INTP_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Specify the valid edge for INTP0 to INTP6 (EGP0) 
*/
/* Valid edge selection (EGP7) */
#define _00_INTP7_EDGE_FALLING_DISABLE          (0x00U) /* edge detection disabled or falling edge */
#define _80_INTP7_EDGE_RISING_BOTH              (0x80U) /* rising or both edge */
/* Valid edge selection (EGP6) */
#define _00_INTP6_EDGE_FALLING_DISABLE          (0x00U) /* edge detection disabled or falling edge */
#define _40_INTP6_EDGE_RISING_BOTH              (0x40U) /* rising or both edge */
/* Valid edge selection (EGP5) */
#define _00_INTP5_EDGE_FALLING_DISABLE          (0x00U) /* edge detection disabled or falling edge */
#define _20_INTP5_EDGE_RISING_BOTH              (0x20U) /* rising or both edge */
/* Valid edge selection (EGP4) */
#define _00_INTP4_EDGE_FALLING_DISABLE          (0x00U) /* edge detection disabled or falling edge */
#define _10_INTP4_EDGE_RISING_BOTH              (0x10U) /* rising or both edge */
/* Valid edge selection (EGP3) */
#define _00_INTP3_EDGE_FALLING_DISABLE          (0x00U) /* edge detection disabled or falling edge */
#define _08_INTP3_EDGE_RISING_BOTH              (0x08U) /* rising or both edge */
/* Valid edge selection (EGP2) */
#define _00_INTP2_EDGE_FALLING_DISABLE          (0x00U) /* edge detection disabled or falling edge */
#define _04_INTP2_EDGE_RISING_BOTH              (0x04U) /* rising or both edge */
/* Valid edge selection (EGP1) */
#define _00_INTP1_EDGE_FALLING_DISABLE          (0x00U) /* edge detection disabled or falling edge */
#define _02_INTP1_EDGE_RISING_BOTH              (0x02U) /* rising or both edge */
/* Valid edge selection (EGP0) */
#define _00_INTP0_EDGE_FALLING_DISABLE          (0x00U) /* edge detection disabled or falling edge */
#define _01_INTP0_EDGE_RISING_BOTH              (0x01U) /* rising or both edge */

/*
    Specify the valid edge for INTP0 to INTP6 (EGN0) 
*/
/* Valid edge selection (EGN7) */
#define _00_INTP7_EDGE_RISING_DISABLE           (0x00U) /* edge detection disabled or rising edge */
#define _80_INTP7_EDGE_FALLING_BOTH             (0x80U) /* falling or both edge */
/* Valid edge selection (EGN6) */
#define _00_INTP6_EDGE_RISING_DISABLE           (0x00U) /* edge detection disabled or rising edge */
#define _40_INTP6_EDGE_FALLING_BOTH             (0x40U) /* falling or both edge */
/* Valid edge selection (EGN5) */
#define _00_INTP5_EDGE_RISING_DISABLE           (0x00U) /* edge detection disabled or rising edge */
#define _20_INTP5_EDGE_FALLING_BOTH             (0x20U) /* falling or both edge */
/* Valid edge selection (EGN4) */
#define _00_INTP4_EDGE_RISING_DISABLE           (0x00U) /* edge detection disabled or rising edge */
#define _10_INTP4_EDGE_FALLING_BOTH             (0x10U) /* falling or both edge */
/* Valid edge selection (EGN3) */
#define _00_INTP3_EDGE_RISING_DISABLE           (0x00U) /* edge detection disabled or rising edge */
#define _08_INTP3_EDGE_FALLING_BOTH             (0x08U) /* falling or both edge */
/* Valid edge selection (EGN2) */
#define _00_INTP2_EDGE_RISING_DISABLE           (0x00U) /* edge detection disabled or rising edge */
#define _04_INTP2_EDGE_FALLING_BOTH             (0x04U) /* falling or both edge */
/* Valid edge selection (EGN1) */
#define _00_INTP1_EDGE_RISING_DISABLE           (0x00U) /* edge detection disabled or rising edge */
#define _02_INTP1_EDGE_FALLING_BOTH             (0x02U) /* falling or both edge */
/* Valid edge selection (EGN0) */
#define _00_INTP0_EDGE_RISING_DISABLE           (0x00U) /* edge detection disabled or rising edge */
#define _01_INTP0_EDGE_FALLING_BOTH             (0x01U) /* falling or both edge */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_INTC_Create(void);
void R_INTC1_Start(void);
void R_INTC1_Stop(void);
UB drv_read_pow_sw(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif

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
* File Name    : r_cg_intp.c
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
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_INTC_Create
* Description  : This function initializes INTP module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC_Create(void)
{
    PMK0 = 1U;      /* disable INTP0 interrupt */
    PIF0 = 0U;      /* clear INTP0 interrupt flag */
    PMK1 = 1U;      /* disable INTP1 interrupt */
    PIF1 = 0U;      /* clear INTP1 interrupt flag */
    PMK2 = 1U;      /* disable INTP2 interrupt */
    PIF2 = 0U;      /* clear INTP2 interrupt flag */
    PMK3 = 1U;      /* disable INTP3 interrupt */
    PIF3 = 0U;      /* clear INTP3 interrupt flag */
    PMK4 = 1U;      /* disable INTP4 interrupt */
    PIF4 = 0U;      /* clear INTP4 interrupt flag */
    PMK5 = 1U;      /* disable INTP5 interrupt */
    PIF5 = 0U;      /* clear INTP5 interrupt flag */
    PMK6 = 1U;      /* disable INTP6 interrupt */
    PIF6 = 0U;      /* clear INTP6 interrupt flag */
    /* Set INTP1 low priority */
    PPR11 = 1U;
    PPR01 = 1U;
    /* Set INTP3 low priority */
    PPR13 = 1U;
    PPR03 = 1U;
    /* Set INTP5 low priority */
    PPR15 = 1U;
    PPR05 = 1U;
    EGN0 = _20_INTP5_EDGE_FALLING_BOTH | _00_INTP3_EDGE_RISING_DISABLE | _02_INTP1_EDGE_FALLING_BOTH;
    EGP0 = _00_INTP5_EDGE_FALLING_DISABLE | _08_INTP3_EDGE_RISING_BOTH | _00_INTP1_EDGE_FALLING_DISABLE;
    /* Set INTP1 pin */
    /* Set INTP3 pin */
    PM3 |= 0x01U;
    /* Set INTP5 pin */
    
    R_INTC1_Start();
    R_INTC3_Start();
    R_INTC5_Start();
    

}

/***********************************************************************************************************************
* Function Name: R_INTC1_Start
* Description  : This function clears INTP1 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC1_Start(void)
{
    PIF1 = 0U;      /* clear INTP1 interrupt flag */
    PMK1 = 0U;      /* enable INTP1 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_INTC1_Stop
* Description  : This function disables INTP1 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC1_Stop(void)
{
    PMK1 = 1U;      /* disable INTP1 interrupt */
    PIF1 = 0U;      /* clear INTP1 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_INTC3_Start
* Description  : This function clears INTP3 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC3_Start(void)
{
    PIF3 = 0U;      /* clear INTP3 interrupt flag */
    PMK3 = 0U;      /* enable INTP3 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_INTC3_Stop
* Description  : This function disables INTP3 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC3_Stop(void)
{
    PMK3 = 1U;      /* disable INTP3 interrupt */
    PIF3 = 0U;      /* clear INTP3 interrupt flag */
}
/***********************************************************************************************************************
* Function Name: R_INTC5_Start
* Description  : This function clears INTP5 interrupt flag and enables interrupt.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC5_Start(void)
{
    PIF5 = 0U;      /* clear INTP5 interrupt flag */
    PMK5 = 0U;      /* enable INTP5 interrupt */
}
/***********************************************************************************************************************
* Function Name: R_INTC5_Stop
* Description  : This function disables INTP5 interrupt and clears interrupt flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_INTC5_Stop(void)
{
    PMK5 = 1U;      /* disable INTP5 interrupt */
    PIF5 = 0U;      /* clear INTP5 interrupt flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

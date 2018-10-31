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
* File Name    : r_cg_port_user.c
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements device driver for Port module.
* Creation Date: 2017/06/01
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
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


/* Start user code for adding. Do not edit comment generated here */
/***********************************************************************************************************************
* Function Name: R_SAU0_Create
* Description  : This function initializes the SAU0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
/***********************************************************************************************************************
* Function Name : R_PORT_OutputBit
* Description   : This function sets the port output
* Arguments     : (e_port_pinname_t) pin_name
*                 - Pin name
*                 (e_port_bitdata_t) bit_data
*                 - Output data
* Return Value  : None
***********************************************************************************************************************/
void R_PORT_OutputBit( e_port_pinname_t pin_name, e_port_bitdata_t bit_data )
{
    /* -----------------------------------------------------------------------------------------------------------------
     * Processing to control the port output
     * -------------------------------------------------------------------------------------------------------------- */
    switch ( pin_name )
    {
        /* -------------------------------------------------------------------------------------------------------------
         * In the case of P15 ( Handshake to RL78/L1A ) pin
         * ---------------------------------------------------------------------------------------------------------- */
        case E_PORT_HS_TO_L1A:
            /* ---------------------------------------------------------------------------------------------------------
             * Output = 0
             * ------------------------------------------------------------------------------------------------------ */
            if ( E_PORT_BIT_LOW == bit_data )
            {
                P1_bit.no5 = 0U;
            }
            /* ---------------------------------------------------------------------------------------------------------
             * Output = 1
             * ------------------------------------------------------------------------------------------------------ */
            else
            {
                P1_bit.no5 = 1U;
            }
            break;
        /* -------------------------------------------------------------------------------------------------------------
         * Others
         * ---------------------------------------------------------------------------------------------------------- */
        default:
            /* Do nothing */
            break;
    }
} /* End of function ( R_PORT_OutputBit ) */
/* End user code. Do not edit comment generated here */

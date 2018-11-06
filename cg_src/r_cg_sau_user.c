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
* File Name    : r_cg_sau_user.c
* Version      : Code Generator for RL78/G1H V1.00.00.04 [08 Mar 2016]
* Device(s)    : R5F11FLJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for SAU module.
* Creation Date: 2017/12/19
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "sys.h"
#include "r_cg_macrodriver.h"
#include "r_cg_sau.h"
#include "r_cg_port.h"

#include <stdio.h>
#include <string.h>


/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
//#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
//#pragma interrupt drv_uart0_snd_int(vect=INTST0)
//#pragma interrupt drv_uart0_rcv_int(vect=INTSR0)
//#pragma interrupt drv_uart1_snd_int(vect=INTST1)
//#pragma interrupt drv_uart1_rcv_int(vect=INTSR1)
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */



/********************/
/*     �����ϐ�     */
/********************/
UB drv_uart0_send_buf[DRV_UART0_RCV_RING_LENGTH];				/* ���M�o�b�t�@(�����O�p) */
UB drv_uart0_rcv_buf[DRV_UART0_SND_RING_LENGTH];				/* ��M�o�b�t�@(�����O�p) */
RING_BUF drv_uart0_send_ring;									/* ���M�����O�o�b�t�@�p�R���g���[�� */
RING_BUF drv_uart0_rcv_ring;									/* ��M�����O�o�b�t�@�p�R���g���[�� */
UB dv_uart0_send_flg;											/* ���M���t���O */

UB drv_uart1_send_buf[DRV_UART1_RCV_RING_LENGTH];				/* ���M�o�b�t�@(�����O�p) */
UB drv_uart1_rcv_buf[DRV_UART1_SND_RING_LENGTH];				/* ��M�o�b�t�@(�����O�p) */
RING_BUF drv_uart1_send_ring;									/* ���M�����O�o�b�t�@�p�R���g���[�� */
RING_BUF drv_uart1_rcv_ring;									/* ��M�����O�o�b�t�@�p�R���g���[�� */

/********************/
/* �v���g�^�C�v�錾 */
/********************/

/************************************************************************/
/* �֐�     : drv_uart0_init											*/
/* �֐���   : UART0����������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* UART0�̏���������													*/
/************************************************************************/
/* ���ӎ��� :															*/
/* RX321�n�[�h�E�F�A�}�j���A��P.1386�Q��								*/
/* �f�o�b�O�|�[�g�L������SCI10�A��������SCI1�œ���Asys.h�Ő؂�ւ��\ */
/************************************************************************/
void drv_uart0_data_init( void )
{
	/* ���M�����O�o�b�t�@������ */
 	memset( &drv_uart0_send_buf[0], 0x00, sizeof(drv_uart0_send_buf) );
	ring_buf_init( &drv_uart0_send_ring, &drv_uart0_send_buf[0], DRV_UART0_SND_RING_LENGTH );
	
	/* ��M�����O�o�b�t�@������ */
	memset( &drv_uart0_rcv_buf[0], 0x00, sizeof(drv_uart0_rcv_buf) );
	ring_buf_init( &drv_uart0_rcv_ring, &drv_uart0_rcv_buf[0], DRV_UART0_RCV_RING_LENGTH );	
	
	dv_uart0_send_flg = OFF;
}


/************************************************************************/
/* �֐�     : drv_uart0_clear_err										*/
/* �֐���   : �h���C�o�G���[�N���A����									*/
/* ����     : 															*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �h���C�o�G���[�N���A����												*/
/* CPU�}�j���A��P.656�Ɋ�Â��������s��									*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC void drv_uart0_clear_err( void )
{
#if 0
	UB tmp;
	/**************/
	/* �G���[���� */
	/**************/
	// (1)�V���A���f�[�^���W�X�^(SDR)�����[�h����B(��M�o�b�t�@�r�b�g���N���A���I�[�o�[�����G���[������h��)
	// (2)�V���A���X�e�[�^�X���W�X�^(SSR)�����[�h����B(�G���[��ʂ̔��f���s��)
	// (3)�V���A���t���O�N���A���W�X�^(SLR)��"1"�����C�g����B(���[�h����SSR�����̂܂܃��C�g����)
	tmp = DRV_UART_232C_RX_BUF_REG;
	DRV_UART_232C_ERR_CLEAR_REG = DRV_UART_232C_RX_ERR_FLG;
#endif
}


/************************************************************************/
/* �֐�     : drv_uart0_snd_int											*/
/* �֐���   : UART0���M���荞�ݏ���										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* UART0���M���荞�ݏ���												*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void __near drv_uart0_snd_int(void)
{
#if 1
	UB snd_data;
	INT	ercd;
	
	ercd = read_ring_buf( &drv_uart0_send_ring, &snd_data );
	if(ercd == E_OK){
		TXD0 = snd_data;
	}else{
		drv_o_port_g1d_int( OFF );
		dv_uart0_send_flg = OFF;
	}
#endif
}

/************************************************************************/
/* �֐�     : drv_uart0_rcv_int											*/
/* �֐���   : UART0��M���荞�ݏ���										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* UART0��M���荞�ݏ���												*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
void __near drv_uart0_rcv_int(void)
{
    volatile UB rx_data;
    volatile UB err_type;
    
    err_type = (UB)(SSR03 & 0x0007U);
    SIR03 = (UH)err_type;

#if 1
	// �G���[�ł��p��
    if (err_type != 0U){
		drv_uart0_clear_err();
	}
	rx_data = RXD0;
	if( E_QOVR == write_ring_buf( &drv_uart0_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* �����O�o�b�t�@�I�[�o�[���ُ�Ǘ��֒ʒm */
	}
	
	//��M���o�b���O��
#if 0
	write_ring_buf(&drv_uart1_send_ring, (UB)rx_data );
#endif
	
#else
	// �G���[�̏ꍇ��~
    if (err_type != 0U){
		drv_uart1_clear_err();
	}else{
		rx_data = RXD1;
		if( E_QOVR == write_ring_buf( &drv_uart1_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* �����O�o�b�t�@�I�[�o�[���ُ�Ǘ��֒ʒm */
		}
	}
#endif
}


/************************************************************************/
/* �֐�     : drv_uart0_send_sart										*/
/* �֐���   : UART0���M�J�n����											*/
/* ����    : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* UART0���M�J�n����													*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void drv_uart0_send_start( void )
{
	UB snd_data;

//	drv_o_port_g1d_int( ON );
	
	if(E_OK == read_ring_buf( &drv_uart0_send_ring, &snd_data )){
		dv_uart0_send_flg = ON;
        STMK0 = 1U;    /* disable INTST0 interrupt */
		TXD0 = snd_data;
        STMK0 = 0U;    /* enable INTST0 interrupt */
	}
}

/************************************************************************/
/* �֐�     : drv_uart0_get_snd_ring									*/
/* �֐���   : ���M�����O�o�b�t�@�|�C���^�擾							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ���M�����O�o�b�t�@�̃|�C���^								*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �����O�o�b�t�@�̃|�C���^��Ԃ�										*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
RING_BUF* drv_uart0_get_snd_ring( void )
{
	return &drv_uart0_send_ring;

}

/************************************************************************/
/* �֐�     : drv_uart0_get_rcv_ring									*/
/* �֐���   : ��M�����O�o�b�t�@�|�C���^�擾							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ��M�����O�o�b�t�@�̃|�C���^								*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �����O�o�b�t�@�̃|�C���^��Ԃ�										*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
RING_BUF* drv_uart0_get_rcv_ring( void )
{
	return &drv_uart0_rcv_ring;
}


/************************************************************************/
/* �֐�     : drv_uart1_init											*/
/* �֐���   : UART1����������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* UART1�̏���������													*/
/************************************************************************/
/* ���ӎ��� :															*/
/* RX321�n�[�h�E�F�A�}�j���A��P.1386�Q��								*/
/* �f�o�b�O�|�[�g�L������SCI10�A��������SCI1�œ���Asys.h�Ő؂�ւ��\ */
/************************************************************************/
void drv_uart1_data_init( void )
{
	/* ���M�����O�o�b�t�@������ */
	memset( &drv_uart1_send_buf[0], 0x00, sizeof(drv_uart1_send_buf) );
	ring_buf_init( &drv_uart1_send_ring, &drv_uart1_send_buf[0], DRV_UART1_SND_RING_LENGTH );
	
	/* ��M�����O�o�b�t�@������ */
	memset( &drv_uart1_rcv_buf[0], 0x00, sizeof(drv_uart1_rcv_buf) );
	ring_buf_init( &drv_uart1_rcv_ring, &drv_uart1_rcv_buf[0], DRV_UART1_RCV_RING_LENGTH );	
}


/************************************************************************/
/* �֐�     : drv_uart1_clear_err										*/
/* �֐���   : �h���C�o�G���[�N���A����									*/
/* ����     : 															*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �h���C�o�G���[�N���A����												*/
/* CPU�}�j���A��P.656�Ɋ�Â��������s��									*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC void drv_uart1_clear_err( void )
{
#if 0
	UB tmp;
	/**************/
	/* �G���[���� */
	/**************/
	// (1)�V���A���f�[�^���W�X�^(SDR)�����[�h����B(��M�o�b�t�@�r�b�g���N���A���I�[�o�[�����G���[������h��)
	// (2)�V���A���X�e�[�^�X���W�X�^(SSR)�����[�h����B(�G���[��ʂ̔��f���s��)
	// (3)�V���A���t���O�N���A���W�X�^(SLR)��"1"�����C�g����B(���[�h����SSR�����̂܂܃��C�g����)
	tmp = DRV_UART_232C_RX_BUF_REG;
	DRV_UART_232C_ERR_CLEAR_REG = DRV_UART_232C_RX_ERR_FLG;
#endif
}


/************************************************************************/
/* �֐�     : drv_uart1_snd_int											*/
/* �֐���   : UART1���M���荞�ݏ���										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* UART1���M���荞�ݏ���												*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void __near drv_uart1_snd_int(void)
{
#if 1
	UB snd_data;
	INT	ercd;
	
	ercd = read_ring_buf( &drv_uart1_send_ring, &snd_data );
	if(ercd == E_OK){
		TXD1 = snd_data;
	}
#endif
}

/************************************************************************/
/* �֐�     : drv_uart1_rcv_int											*/
/* �֐���   : UART1��M���荞�ݏ���										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* UART1��M���荞�ݏ���												*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
void __near drv_uart1_rcv_int(void)
{
    volatile UB rx_data;
    volatile UB err_type;
    
    err_type = (UB)(SSR03 & 0x0007U);
    SIR03 = (UH)err_type;

#if 1
	// �G���[�ł��p��
    if (err_type != 0U){
		drv_uart1_clear_err();
	}
	rx_data = RXD1;
	if( E_QOVR == write_ring_buf( &drv_uart1_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* �����O�o�b�t�@�I�[�o�[���ُ�Ǘ��֒ʒm */
	}
#else
	// �G���[�̏ꍇ��~
    if (err_type != 0U){
		drv_uart1_clear_err();
	}else{
		rx_data = RXD1;
		if( E_QOVR == write_ring_buf( &drv_uart1_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* �����O�o�b�t�@�I�[�o�[���ُ�Ǘ��֒ʒm */
		}
	}
#endif
}


/************************************************************************/
/* �֐�     : drv_uart1_send_sart										*/
/* �֐���   : UART1���M�J�n����											*/
/* ����    : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* UART1���M�J�n����													*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void drv_uart1_send_start( void )
{
#if 1
UB snd_data;
	
	if(E_OK == read_ring_buf( &drv_uart1_send_ring, &snd_data )){
        STMK1 = 1U;    /* disable INTST1 interrupt */
		TXD1 = snd_data;
        STMK1 = 0U;    /* enable INTST1 interrupt */
	}
#endif
}

/************************************************************************/
/* �֐�     : drv_uart1_get_snd_ring									*/
/* �֐���   : ���M�����O�o�b�t�@�|�C���^�擾							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ���M�����O�o�b�t�@�̃|�C���^								*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �����O�o�b�t�@�̃|�C���^��Ԃ�										*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
RING_BUF* drv_uart1_get_snd_ring( void )
{
	return &drv_uart1_send_ring;

}

/************************************************************************/
/* �֐�     : drv_uart1_get_rcv_ring									*/
/* �֐���   : ��M�����O�o�b�t�@�|�C���^�擾							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ��M�����O�o�b�t�@�̃|�C���^								*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �����O�o�b�t�@�̃|�C���^��Ԃ�										*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
RING_BUF* drv_uart1_get_rcv_ring( void )
{
	return &drv_uart1_rcv_ring;
}


UB drv_uart0_get_send( void )
{
	return dv_uart0_send_flg;
}


// debug
void test_uart_0_send( void )
{
	UB snd_data = 0xAA;
	
	write_ring_buf( &drv_uart0_send_ring, snd_data );
	write_ring_buf( &drv_uart0_send_ring, snd_data );
	write_ring_buf( &drv_uart0_send_ring, snd_data );
	
	drv_uart0_send_start();
}
/************************************************************/
/* END OF TEXT												*/
/************************************************************/

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
/*     内部変数     */
/********************/
UB drv_uart0_send_buf[DRV_UART0_RCV_RING_LENGTH];				/* 送信バッファ(リング用) */
UB drv_uart0_rcv_buf[DRV_UART0_SND_RING_LENGTH];				/* 受信バッファ(リング用) */
RING_BUF drv_uart0_send_ring;									/* 送信リングバッファ用コントローラ */
RING_BUF drv_uart0_rcv_ring;									/* 受信リングバッファ用コントローラ */
UB dv_uart0_send_flg;											/* 送信中フラグ */

UB drv_uart1_send_buf[DRV_UART1_RCV_RING_LENGTH];				/* 送信バッファ(リング用) */
UB drv_uart1_rcv_buf[DRV_UART1_SND_RING_LENGTH];				/* 受信バッファ(リング用) */
RING_BUF drv_uart1_send_ring;									/* 送信リングバッファ用コントローラ */
RING_BUF drv_uart1_rcv_ring;									/* 受信リングバッファ用コントローラ */

/********************/
/* プロトタイプ宣言 */
/********************/

/************************************************************************/
/* 関数     : drv_uart0_init											*/
/* 関数名   : UART0初期化処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART0の初期化処理													*/
/************************************************************************/
/* 注意事項 :															*/
/* RX321ハードウェアマニュアルP.1386参照								*/
/* デバッグポート有効時はSCI10、無効時はSCI1で動作、sys.hで切り替え可能 */
/************************************************************************/
void drv_uart0_data_init( void )
{
	/* 送信リングバッファ初期化 */
 	memset( &drv_uart0_send_buf[0], 0x00, sizeof(drv_uart0_send_buf) );
	ring_buf_init( &drv_uart0_send_ring, &drv_uart0_send_buf[0], DRV_UART0_SND_RING_LENGTH );
	
	/* 受信リングバッファ初期化 */
	memset( &drv_uart0_rcv_buf[0], 0x00, sizeof(drv_uart0_rcv_buf) );
	ring_buf_init( &drv_uart0_rcv_ring, &drv_uart0_rcv_buf[0], DRV_UART0_RCV_RING_LENGTH );	
	
	dv_uart0_send_flg = OFF;
}


/************************************************************************/
/* 関数     : drv_uart0_clear_err										*/
/* 関数名   : ドライバエラークリア処理									*/
/* 引数     : 															*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* ドライバエラークリア処理												*/
/* CPUマニュアルP.656に基づき処理を行う									*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void drv_uart0_clear_err( void )
{
#if 0
	UB tmp;
	/**************/
	/* エラー処理 */
	/**************/
	// (1)シリアルデータレジスタ(SDR)をリードする。(受信バッファビットをクリアしオーバーランエラー発生を防ぐ)
	// (2)シリアルステータスレジスタ(SSR)をリードする。(エラー種別の判断を行う)
	// (3)シリアルフラグクリアレジスタ(SLR)に"1"をライトする。(リードしたSSRをそのままライトする)
	tmp = DRV_UART_232C_RX_BUF_REG;
	DRV_UART_232C_ERR_CLEAR_REG = DRV_UART_232C_RX_ERR_FLG;
#endif
}


/************************************************************************/
/* 関数     : drv_uart0_snd_int											*/
/* 関数名   : UART0送信割り込み処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART0送信割り込み処理												*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
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
/* 関数     : drv_uart0_rcv_int											*/
/* 関数名   : UART0受信割り込み処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART0受信割り込み処理												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void __near drv_uart0_rcv_int(void)
{
    volatile UB rx_data;
    volatile UB err_type;
    
    err_type = (UB)(SSR03 & 0x0007U);
    SIR03 = (UH)err_type;

#if 1
	// エラーでも継続
    if (err_type != 0U){
		drv_uart0_clear_err();
	}
	rx_data = RXD0;
	if( E_QOVR == write_ring_buf( &drv_uart0_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* リングバッファオーバーを異常管理へ通知 */
	}
	
	//受信をＰＣログへ
#if 0
	write_ring_buf(&drv_uart1_send_ring, (UB)rx_data );
#endif
	
#else
	// エラーの場合停止
    if (err_type != 0U){
		drv_uart1_clear_err();
	}else{
		rx_data = RXD1;
		if( E_QOVR == write_ring_buf( &drv_uart1_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* リングバッファオーバーを異常管理へ通知 */
		}
	}
#endif
}


/************************************************************************/
/* 関数     : drv_uart0_send_sart										*/
/* 関数名   : UART0送信開始処理											*/
/* 引数    : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART0送信開始処理													*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
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
/* 関数     : drv_uart0_get_snd_ring									*/
/* 関数名   : 送信リングバッファポインタ取得							*/
/* 引数     : なし														*/
/* 戻り値   : 送信リングバッファのポインタ								*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* リングバッファのポインタを返す										*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
RING_BUF* drv_uart0_get_snd_ring( void )
{
	return &drv_uart0_send_ring;

}

/************************************************************************/
/* 関数     : drv_uart0_get_rcv_ring									*/
/* 関数名   : 受信リングバッファポインタ取得							*/
/* 引数     : なし														*/
/* 戻り値   : 受信リングバッファのポインタ								*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* リングバッファのポインタを返す										*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
RING_BUF* drv_uart0_get_rcv_ring( void )
{
	return &drv_uart0_rcv_ring;
}


/************************************************************************/
/* 関数     : drv_uart1_init											*/
/* 関数名   : UART1初期化処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART1の初期化処理													*/
/************************************************************************/
/* 注意事項 :															*/
/* RX321ハードウェアマニュアルP.1386参照								*/
/* デバッグポート有効時はSCI10、無効時はSCI1で動作、sys.hで切り替え可能 */
/************************************************************************/
void drv_uart1_data_init( void )
{
	/* 送信リングバッファ初期化 */
	memset( &drv_uart1_send_buf[0], 0x00, sizeof(drv_uart1_send_buf) );
	ring_buf_init( &drv_uart1_send_ring, &drv_uart1_send_buf[0], DRV_UART1_SND_RING_LENGTH );
	
	/* 受信リングバッファ初期化 */
	memset( &drv_uart1_rcv_buf[0], 0x00, sizeof(drv_uart1_rcv_buf) );
	ring_buf_init( &drv_uart1_rcv_ring, &drv_uart1_rcv_buf[0], DRV_UART1_RCV_RING_LENGTH );	
}


/************************************************************************/
/* 関数     : drv_uart1_clear_err										*/
/* 関数名   : ドライバエラークリア処理									*/
/* 引数     : 															*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* ドライバエラークリア処理												*/
/* CPUマニュアルP.656に基づき処理を行う									*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void drv_uart1_clear_err( void )
{
#if 0
	UB tmp;
	/**************/
	/* エラー処理 */
	/**************/
	// (1)シリアルデータレジスタ(SDR)をリードする。(受信バッファビットをクリアしオーバーランエラー発生を防ぐ)
	// (2)シリアルステータスレジスタ(SSR)をリードする。(エラー種別の判断を行う)
	// (3)シリアルフラグクリアレジスタ(SLR)に"1"をライトする。(リードしたSSRをそのままライトする)
	tmp = DRV_UART_232C_RX_BUF_REG;
	DRV_UART_232C_ERR_CLEAR_REG = DRV_UART_232C_RX_ERR_FLG;
#endif
}


/************************************************************************/
/* 関数     : drv_uart1_snd_int											*/
/* 関数名   : UART1送信割り込み処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART1送信割り込み処理												*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
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
/* 関数     : drv_uart1_rcv_int											*/
/* 関数名   : UART1受信割り込み処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART1受信割り込み処理												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void __near drv_uart1_rcv_int(void)
{
    volatile UB rx_data;
    volatile UB err_type;
    
    err_type = (UB)(SSR03 & 0x0007U);
    SIR03 = (UH)err_type;

#if 1
	// エラーでも継続
    if (err_type != 0U){
		drv_uart1_clear_err();
	}
	rx_data = RXD1;
	if( E_QOVR == write_ring_buf( &drv_uart1_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* リングバッファオーバーを異常管理へ通知 */
	}
#else
	// エラーの場合停止
    if (err_type != 0U){
		drv_uart1_clear_err();
	}else{
		rx_data = RXD1;
		if( E_QOVR == write_ring_buf( &drv_uart1_rcv_ring, rx_data )){
//			err_info(ERR_ID_RING_OVER_LOG);		/* リングバッファオーバーを異常管理へ通知 */
		}
	}
#endif
}


/************************************************************************/
/* 関数     : drv_uart1_send_sart										*/
/* 関数名   : UART1送信開始処理											*/
/* 引数    : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* UART1送信開始処理													*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
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
/* 関数     : drv_uart1_get_snd_ring									*/
/* 関数名   : 送信リングバッファポインタ取得							*/
/* 引数     : なし														*/
/* 戻り値   : 送信リングバッファのポインタ								*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* リングバッファのポインタを返す										*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
RING_BUF* drv_uart1_get_snd_ring( void )
{
	return &drv_uart1_send_ring;

}

/************************************************************************/
/* 関数     : drv_uart1_get_rcv_ring									*/
/* 関数名   : 受信リングバッファポインタ取得							*/
/* 引数     : なし														*/
/* 戻り値   : 受信リングバッファのポインタ								*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* リングバッファのポインタを返す										*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
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

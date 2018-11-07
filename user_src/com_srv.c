/************************************************************************/
/* システム名   : RD8001 快眠チェッカー									*/
/* ファイル名   : com_srv.c                                             */
/* 機能         : ログ通信（RS-232C）				                    */
/* 変更履歴     : 2018.01.25 Axia Soft Design 西島 稔	初版作成		*/
/* 注意事項     : なし                                                  */
/************************************************************************/
/********************/
/*     include      */
/********************/
#include "header.h"

/********************/
/* プロトタイプ宣言 */
/********************/
void com_srv_init( void );
void com_srv_log_title( void );
void com_srv_cyc( void );
STATIC void com_srv_command( UB data );
void com_srv_send( UB* tx_data, UB len );

/********************/
/*     内部定数     */
/********************/


/********************/
/*     内部変数     */
/********************/
STATIC COM_SRV_ANA	s_com_srv_ana;


/********************/
/*     外部参照     */
/********************/
extern const B		version_product_tbl[];				/* ソフトウェアバージョン */
extern const UB	* const version_boot_tbl;				/* ブートバージョンアドレス */

extern void drv_uart1_data_init( void );

/************************************************************************/
/* 関数     : com_srv_init                                              */
/* 関数名   : 保守アプリ初期化処理                                      */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 保守アプリ初期化処理                                          */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void com_srv_init( void )
{
#if FUNC_DEBUG_LOG == ON
	memset( &s_com_srv_ana, 0x00, sizeof(s_com_srv_ana) );

	drv_uart1_data_init();
#endif
}

/************************************************************************/
/* 関数     : com_srv_log_title                                         */
/* 関数名   : タイトル出力			                                    */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : タイトル出力処理の関数化                                      */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void com_srv_log_title( void )
{
#if FUNC_DEBUG_LOG == ON
	UB	boot_ver[4];				/* Boot部バージョン情報 */
	

	int len;
	UB tx_data[DRV_UART1_DATA_LENGH] = {0};
	
	len = sprintf((char*)tx_data, "POW ON APL V.%d.%d.%d.%d  ", version_product_tbl[0], version_product_tbl[1],
																	   version_product_tbl[2], version_product_tbl[3] );
	com_srv_send( tx_data, len );
	wait_ms(5);
	
	
	// アプリが保持しているアドレスからブートバージョンを生成
	memcpy((UB *)boot_ver, (UB *)version_boot_tbl, sizeof(boot_ver));

	len = sprintf((char*)tx_data, "BOOT  V.%d.%d.%d.%d\r\n", boot_ver[0], boot_ver[1],
																	   boot_ver[2], boot_ver[3] );
	
	com_srv_send( tx_data, len );
	wait_ms(5);
	
#endif
}

/************************************************************************/
/* 関数     : com_srv_cyc                                               */
/* 関数名   : 周期表示処理                                              */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 周期表示処理                                                  */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void com_srv_cyc( void )
{
#if FUNC_DEBUG_LOG == ON
	RING_BUF* p_ring_buf = NULL;
	UB rcv_data;
	
	p_ring_buf = drv_uart1_get_rcv_ring();

	if(E_OK == read_ring_buf( p_ring_buf, &rcv_data )){
		// 受信データあり
		com_srv_command( rcv_data );
	}else{
		if( s_com_srv_ana.rcv_non++ >= COM_RCV_NON_CNT_MAX ){
			// 一定期間未受信で受信状態クリア
			s_com_srv_ana.rcv_non = 0;
		}
	}
#endif
}

/************************************************************************/
/* 関数     : com_srv_command                                           */
/* 関数名   : コマンド解析処理                                          */
/* 引数     : pInData	コマンド文									    */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : コマンド解析処理                                              */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
STATIC void com_srv_command( UB data )
{
#if FUNC_DEBUG_LOG == ON
#if 0
	//途中リリース時のコマンド
	if( 0 == s_com_srv_ana.len ){
		if( 'G' == data ){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len++;
		}else{
			s_com_srv_ana.len = 0;
		}
	}else if( 1 == s_com_srv_ana.len ){
		if(( 'E' == data ) && ( 'G' == s_com_srv_ana.data[0] )){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len++;
		}else{
			s_com_srv_ana.len = 0;
		}
	}else if( 2 == s_com_srv_ana.len ){
		if(( 'T' == data ) && ( 'E' == s_com_srv_ana.data[1] )){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len++;
		}else{
			s_com_srv_ana.len = 0;
		}
#if 0
	}else{
		s_com_srv_ana.len = 0;
	}
#else
	}else if( 3 == s_com_srv_ana.len ){
		if(( '\r' == data ) && ( 'T' == s_com_srv_ana.data[2] )){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len++;
		}else{
			s_com_srv_ana.len = 0;
		}
	}else if( 4 == s_com_srv_ana.len ){
		if(( '\n' == data ) && ( '\r' == s_com_srv_ana.data[3] )){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len = 0;
			getmode_in();
		}else{
			s_com_srv_ana.len = 0;
		}
	}
#endif
#endif
#endif
}

/************************************************************************/
/* 関数     : com_srv_send                                              */
/* 関数名   : 送信処理					                                */
/* 引数     : tx_data		送信バッファ								*/
/*          : len			送信長										*/
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2018.09.10  Axia Soft Design 西島 稔   初版作成           */
/************************************************************************/
/* 機能 : 送信処理									                    */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void com_srv_send( UB* tx_data, UB len )
{
#if FUNC_DEBUG_LOG == ON
	RING_BUF* p_ring_buf;
	UB i = 0;
	UB ret;
	
	p_ring_buf = drv_uart1_get_snd_ring();
	
	/* 送信バッファを全て書き込み */
	for(i = 0; i < len; i++){
		ret = write_ring_buf(p_ring_buf, (UB)tx_data[i] );
		if(ret != E_OK){
			break;
		}
	}
	
	/* 送信開始 */
	drv_uart1_send_start();
#endif
}


/************************************************************************/
/* 関数     : com_srv_puts                                              */
/* 関数名   : 標準出力処理(固定文字列のみ)                              */
/* 引数     : 文字列													*/
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2017.03.02  Axia Soft Design 西島 稔   初版作成           */
/************************************************************************/
/* 機能 : 標準出力処理(固定文字列のみ)				                    */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void com_srv_puts( const B* pMsg)
{
#if FUNC_DEBUG_LOG == ON
	RING_BUF* p_ring_buf;
	
	p_ring_buf = drv_uart1_get_snd_ring();
	
	/* 送信バッファを全て書き込み */
	while(*pMsg != 0x00){
		if( E_OK != write_ring_buf(p_ring_buf, (UB)(*pMsg) )){
			return;
		}
		pMsg++;
	}
	
	/* 送信開始 */
	drv_uart1_send_start();
#endif
}

/*****************************************************************************
 *								 end of text
 *****************************************************************************/

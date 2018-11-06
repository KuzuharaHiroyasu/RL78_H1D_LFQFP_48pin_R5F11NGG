/************************************************************************/
/* システム名   : RD1402 パチンコ/パチスロ用呼び出しランプ				*/
/* ファイル名   : cpu_com.c												*/
/* 機能         : CPU間通信ミドル										*/
/* 変更履歴     : 2014.05.08 Axia Soft Design 吉居		初版作成		*/
/* 注意事項     : なし													*/
/************************************************************************/
//RD1508暫定：見直しが必要
/********************/
/*     include      */
/********************/
#include	"header.h"

/********************/
/* プロトタイプ宣言 */
/********************/
STATIC void cpu_com_rcv_proc(void);
STATIC void cpu_com_send_proc(void);
STATIC UB cpu_com_analyze_msg(void);
STATIC UB cpu_com_make_send_data(void);
STATIC void cpu_com_clear_send_order(void);
STATIC void cpu_com_crcset(UB *msg, UH size, UH *crc);
STATIC void cpu_com_sumget(UB *msg, UH size, UH *psum);
STATIC void cpu_com_rcv_proc(void);
STATIC UB cpu_com_analyze_msg(void);
STATIC UB cpu_com_analyze_msg_check_etx(void);
STATIC UB cpu_com_analyze_msg_check_data(void);
STATIC void cpu_com_send_proc(void);
STATIC UB cpu_com_make_send_data(void);
STATIC void cpu_com_clear_send_order(void);
void drv_cpu_com_set_send_status( UB status );
void drv_cpu_com_get_send_status( UB *status );


/********************/
/*     内部変数     */
/********************/
//RD8001暫定：メインとのデータIF未定
STATIC DS_CPU_COM_ORDER s_ds_cpu_com_order;
STATIC DS_CPU_COM_INPUT s_cpu_com_ds_input;					/* データ管理部のコピーエリア アプリへのデータ受け渡し用 */
STATIC DS_CPU_COM_ORDER *s_p_cpu_com_ds_order;				/* データ管理部のポインタ アプリからの指示用 */

STATIC UB s_cpu_com_snd_cmd;								/* 送信コマンド */
STATIC UB s_cpu_com_snd_data[CPU_COM_MSG_SIZE_MAX];			/* 送信メッセージ */
STATIC UH s_cpu_com_snd_size;								/* 送信メッセージ長 */
STATIC UB s_cpu_com_snd_type;								/* 送信コマンドタイプ */
STATIC UB s_cpu_com_snd_retry_cnt;							/* 送信リトライ回数 */
STATIC UW s_cpu_com_snd_timeout;							/* リトライタイムアウト時間 *10ms */
STATIC UB s_cpu_com_snd_rensou_cnt;							/* 送信連送回数 */

STATIC UB s_cpu_com_snd_seq_no;								/* 送信シーケンスNo */
STATIC UB s_cpu_com_res_seq_no;								/* 受信シーケンスNo */
STATIC UB s_cpu_com_snd_rty_no;								/* 送信リトライNo */
STATIC UB s_cpu_com_res_rty_no;								/* 受信リトライNo */

STATIC UB s_cpu_com_snd_status;								/* CPU間通信 送信ステータス */
																/*	CPU_COM_SND_STATUS_IDLE			送信可能状態		*/
																/* 	CPU_COM_SND_STATUS_RCV_WAIT		応答受信待ち状態	*/
																/* 	CPU_COM_SND_STATUS_RENSOU		連送中状態			*/
																/* 	CPU_COM_SND_STATUS_COMPLETE		送信完了状態		*/
																/* 	CPU_COM_SND_STATUS_SEND_NG		リトライNG			*/

STATIC CPU_COM_RCV_MSG	s_cpu_com_rcv_msg;					/* 受信メッセージ */
STATIC UH s_cpu_com_rcv_msg_size;							/* 受信メッセージサイズ */

/********************/
/*   テーブル定義   */
/********************/
/* コマンドテーブル ※マスター専用 */
/* タイプを変更する事で受信専用にも対応 */
STATIC const T_CPU_COM_CMD_INFO s_tbl_cmd_info[CPU_COM_CMD_MAX] = {
	/*コマンド*/ /*タイプ*/					/*リトライ(初送含む)*//*リトライ間隔 *10ms*/ /*連送回数*/
	{	0x00,	CPU_COM_CMD_TYPE_ONESHOT_SEND,		0,				0,					0	},	/* コマンド無し				*/
	{	0xE0,	CPU_COM_CMD_TYPE_RETRY,				0,				0,					0	},	/* ステータス要求			*/
	{	0xA0,	CPU_COM_CMD_TYPE_ONESHOT_SEND,		0,				0,					0	},	/* センサーデータ更新		*/
	{	0xB0,	CPU_COM_CMD_TYPE_RETRY,				3,				3,					0	},	/* 状態変更(G1D)			*/
	{	0xF0,	CPU_COM_CMD_TYPE_ONESHOT_RCV,		0,				0,					0	},	/* PCログ送信(内部コマンド)	*/
																								// プログラム更新
	{	0xC0,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* プログラム転送開始 */
	{	0xC1,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* プログラム転送データ要求 */
	{	0xC2,	CPU_COM_CMD_TYPE_ONESHOT_RCV,		5,				3,					0	},	/* プログラム転送(受信)	*/
	{	0xC3,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* プログラム転送サム値要求 */
	{	0xCF,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* プログラム転送結果要求 */
	{	0xD0,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* ファイル転送開始 */
	{	0xD1,	CPU_COM_CMD_TYPE_RENSOU,			0,				0,					3	},	/* ファイル転送(連送) */
	{	0xD3,	CPU_COM_CMD_TYPE_RETRY,				5,				10,					0	},	/* ブロック転送結果要求 */
	{	0xDF,	CPU_COM_CMD_TYPE_RETRY,				5,				10,					0	},	/* ファイル転送結果要求 *///SH側の結果が遅くなる事があった為
};


/************************************************************************/
/* 関数     : cpu_com_init												*/
/* 関数名   : 初期化処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.05.08  Axia Soft Design 吉居		初版作成			*/
/************************************************************************/
/* 機能 : 初期化処理													*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void cpu_com_init(void)
{
	memset(&s_cpu_com_ds_input, 0x00, sizeof(s_cpu_com_ds_input));
	s_p_cpu_com_ds_order = &s_ds_cpu_com_order;

	s_cpu_com_snd_cmd = 0x00;
	memset(s_cpu_com_snd_data, 0x00, sizeof(s_cpu_com_snd_data));
	s_cpu_com_snd_size = 0;
	s_cpu_com_snd_type = 0;
	s_cpu_com_snd_retry_cnt = 0;
	s_cpu_com_snd_timeout = 0;
	s_cpu_com_snd_rensou_cnt = 0;
	s_cpu_com_snd_seq_no = 0;
	s_cpu_com_res_seq_no = 0;
	
	s_cpu_com_snd_status = CPU_COM_SND_STATUS_IDLE;
	
	memset(s_cpu_com_rcv_msg.buf, 0x00, sizeof(s_cpu_com_rcv_msg.buf));
	s_cpu_com_rcv_msg_size = 0;
	
#if FUNC_DEBUG_CPU_COM == OFF
	drv_uart0_data_init();
#else
	drv_uart1_data_init();
#endif
}
/************************************************************************/
/* 関数     : cpu_com_crcset											*/
/* 関数名   : CRCセット													*/
/* 引数     : UB *msg : 対象データアドレス								*/
/*			: UH size : 対象データ長									*/
/*          : UH crc : CRC計算結果										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.11.30  Axia Soft Design 浦久保   初版作成			*/
/*          : 2014.05.13  Axia Soft Design 吉居		CPU間通信用に移植	*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 : 演算の対象は、CMD、SEQ、DATAまでとする。					*/
/************************************************************************/
STATIC void cpu_com_crcset(UB *msg, UH size, UH *crc)
{
	/* CRC計算 */
	*crc = crc16(msg, (INT)size);

}

/************************************************************************/
/* 関数     : cpu_com_sumget											*/
/* 関数名   : SUM取得													*/
/* 引数     : UB *msg : 対象データアドレス								*/
/*			: UH size : 対象データ長									*/
/*			: UH *psum : SUM値格納										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.11.30  Axia Soft Design 浦久保   初版作成			*/
/*          : 2014.05.13  Axia Soft Design 吉居		CPU間通信用に移植	*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 : 演算の対象は、CMD、SEQ、DATAまでとする。					*/
/************************************************************************/
STATIC void cpu_com_sumget(UB *msg, UH size, UH *psum)
{
	UW sum = 0;
	UH i;
	
	/* SUM計算 */
	for(i = 0;i < size;i++){
		sum += msg[i];
	}
	sum &= 0x0000FFFF;	/* 下位2バイトにマスク */
	
	*psum = (UH)sum;
}

/************************************************************************/
/* 関数     : cpu_com_proc												*/
/* 関数名   : CPU間通信周期処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.05.08  Axia Soft Design 吉居		初版作成			*/
/************************************************************************/
/* 機能 : 周期処理														*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void cpu_com_proc(void)
{
#if 1
	/* アプリからデータ取得 */
	ds_get_cpu_com_order( &s_p_cpu_com_ds_order );
#endif
	
	/* 受信データ解析 */
	cpu_com_rcv_proc();
	
	/* 周期送信処理 */
	cpu_com_send_proc();
	
#if 1
	/* 状態通知用変数のセット */
	s_cpu_com_ds_input.cpu_com_send_status = s_cpu_com_snd_status;
	
	/* アプリへのデータ設定 */
	ds_set_cpu_com_input( &s_cpu_com_ds_input );
	
	/* データ通知後はクリア */
	s_cpu_com_ds_input.rcv_cmd = 0x00;
	memset( s_cpu_com_ds_input.rcv_data, 0x00, sizeof(s_cpu_com_ds_input.rcv_data));
	
#endif
}

/************************************************************************/
/* 関数     : get_cpu_com_send_req										*/
/* 関数名   : CPU間通信レスポンス待ち送信処理							*/
/* 引数     : *order 送信要求											*/
/* 引数     : *input 受信データ格納用									*/
/* 戻り値   : CPU_COM_SND_RES_OK										*/
/*          : CPU_COM_SND_RES_BUSY_NG									*/
/*          : CPU_COM_SND_RES_RETRY_OUT_NG								*/
/* 変更履歴 : 2014.05.08  Axia Soft Design 吉居		初版作成			*/
/************************************************************************/
/* 機能 : 指定コマンドを送信しレスポンス受信待ちを行う					*/
/************************************************************************/
/* 注意事項 :送信可能状態のみ実行可能									*/
/*          :送信ステータスは周期処理によって更新されるためビジー応答が	*/
/*          :解消されるのを上位関数のループで待つ事は出来ない			*/
/*          :上位のシーケンスに送信可能待ちを設けること					*/
/************************************************************************/
UB get_cpu_com_send_req( DS_CPU_COM_ORDER *order, DS_CPU_COM_INPUT *input )
{
#if 0
	
	if( CPU_COM_SND_STATUS_IDLE != s_cpu_com_snd_status ){
		/* 送信可能状態以外は無効 */
		return CPU_COM_SND_RES_BUSY_NG;	/* CPU間通信ビジー */
	}
	/* オーダーのセット */
	s_p_cpu_com_ds_order = order;
	
	/* 送信完了まで繰り返し */
	while( CPU_COM_SND_STATUS_COMPLETE != s_cpu_com_snd_status ){
		/* 受信データ解析 */
		cpu_com_rcv_proc();
		/* 送信処理 */ /* 周期処理と同等 */
		cpu_com_send_proc();
		
		if( CPU_COM_SND_STATUS_SEND_NG == s_cpu_com_snd_status ){
			/* リトライアウト */
			return CPU_COM_SND_RES_RETRY_OUT_NG;
		}else if( CPU_COM_SND_STATUS_IDLE == s_cpu_com_snd_status ){
			/* アイドル状態に遷移する事は無い 遷移した場合はコマンド設定誤り */
			return CPU_COM_SND_RES_RETRY_OUT_NG;
		}else{
			/* 何もしない */
		}
		
		/* RD1402暫定 リトライ待ち時間によってはWDTリフレッシュ処理必要 */
		wdt_refresh();
	}
	
	/* 状態通知用変数のセット */
	s_cpu_com_ds_input.cpu_com_send_status = s_cpu_com_snd_status;
	
	/* アプリへのデータ設定 */
	*input = s_cpu_com_ds_input;
	
	/* データ通知後はクリア */
	s_cpu_com_ds_input.rcv_cmd = 0x00;
	memset( s_cpu_com_ds_input.rcv_data, 0x00, sizeof(s_cpu_com_ds_input.rcv_data));
	
#endif
	return CPU_COM_SND_RES_OK;
}

/************************************************************************/
/* 関数     : cpu_com_rcv_proc											*/
/* 関数名   : 周期受信処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.05.08  Axia Soft Design 吉居		初版作成			*/
/************************************************************************/
/* 機能 : 周期処理														*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void cpu_com_rcv_proc(void)
{
	if( ON == cpu_com_analyze_msg() ){
		/* 受信データ正常 */
#if 1
		s_cpu_com_ds_input.rcv_cmd = s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_CMD];
		/* 受信シーケンスNoのセット */
		s_cpu_com_res_seq_no = s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_SEQ];				/* SEQ下位ビット */
		s_cpu_com_res_rty_no = s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_SEQ + 1] << 8;	/* SEQ上位ビット */
		/* アプリ通知用のデータに受信データをセット */
		memcpy( s_cpu_com_ds_input.rcv_data, &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_DATA], ( s_cpu_com_rcv_msg_size- CPU_COM_MSG_SIZE_MIN ));
#endif
	}else{
#if 1
		/* 受信なし */
		s_cpu_com_ds_input.rcv_cmd = 0x00;
		s_cpu_com_res_seq_no = 0x0000;
		s_cpu_com_res_rty_no = 0x0000;
		memset( s_cpu_com_ds_input.rcv_data, 0x00, sizeof(s_cpu_com_ds_input.rcv_data));
#endif
	}
}


/************************************************************************/
/* 関数     : cpu_com_analyze_msg										*/
/* 関数名   : 受信メッセージ解析										*/
/* 引数     : なし														*/
/* 戻り値   : ON :1メッセージ受信										*/
/*          : OFF:未受信												*/
/* 変更履歴 : 2014.05.08  Axia Soft Design 吉居		初版作成			*/
/************************************************************************/
/* 機能 : 周期処理														*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC UB cpu_com_analyze_msg(void)
{
	//stxからetxまでチェックしメッセージバッファに格納
	if( OFF == cpu_com_analyze_msg_check_etx() ){
		return OFF;
	}
	
	// デバッグ
//	com_srv_puts( (const B *__near)"R ETX\r\n" );
	
	if( OFF ==  cpu_com_analyze_msg_check_data() ){
		return OFF;
	}
	// デバッグ
//	com_srv_puts( (const B *__near)"R MSG\r\n" );
	
	return ON;
	
}

/************************************************************************/
/* 関数     : cpu_com_analyze_msg_check_etx								*/
/* 関数名   : 受信メッセージ解析(ETX)									*/
/* 引数     : なし														*/
/* 戻り値   : ON :1メッセージ受信										*/
/*          : OFF:未受信												*/
/* 変更履歴 : 2015.10.23 Axia Soft Design 西島		初版作成			*/
/************************************************************************/
/* 機能 : 受信メッセージ解析(ETX)										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC UB cpu_com_analyze_msg_check_etx(void)
{
	UB ret = OFF;
	RING_BUF *p_ring_buf;
	UB rcv_data;
	
#if FUNC_DEBUG_CPU_COM == OFF
	p_ring_buf = drv_uart0_get_rcv_ring();
#else
	p_ring_buf = drv_uart1_get_rcv_ring();
#endif
	
	
	// 1バイト毎に受信しETXまでチェック
	while(1){
		if( E_OK != read_ring_buf(p_ring_buf, &rcv_data ) ){
			break;
		}
		//バッファオーバーラン対策
		if( s_cpu_com_rcv_msg.pos >= CPU_COM_MSG_SIZE_MAX ){
			s_cpu_com_rcv_msg.pos = 0;
			s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_STX_WAIT;
		}
		
		// 制御コード
		if( rcv_data == CPU_COM_CTRL_CODE_DLE ){
			if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state )){
				//データ部のDLE重複と判断し ※格納しない
				s_cpu_com_rcv_msg.last_dle_flg = OFF;
			}else{
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = rcv_data;
				s_cpu_com_rcv_msg.last_dle_flg = ON;
			}
		}else{
			//制御コード以外
			if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_CTRL_CODE_STX == rcv_data )){
				// STX受信と判断
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
//				com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"rcv stx\r\n");
#endif
				s_cpu_com_rcv_msg.pos = 0;
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_DLE;
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_STX;
				s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_ETX_WAIT;
			}else if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_CTRL_CODE_ETX == rcv_data ) && ( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state )){
				// ETXまで取得
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
//				com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"rcv etx\r\n");
#endif
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_ETX;
				s_cpu_com_rcv_msg_size = s_cpu_com_rcv_msg.pos;
				s_cpu_com_rcv_msg.pos = 0;
				s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_STX_WAIT;
				ret = ON;
				break;
			}else{
				if( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state ){
					s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = rcv_data;
				}else{
					s_cpu_com_rcv_msg.pos = 0;
				}
			}
			s_cpu_com_rcv_msg.last_dle_flg = OFF;
		}
	}
	return ret;
}

/************************************************************************/
/* 関数     : cpu_com_analyze_msg_check_data							*/
/* 関数名   : 受信メッセージ解析(CRC,SUMチェック)						*/
/* 引数     : なし														*/
/* 戻り値   : ON :1メッセージ受信										*/
/*          : OFF:未受信												*/
/* 変更履歴 : 2015.10.23 Axia Soft Design 西島		初版作成			*/
/************************************************************************/
/* 機能 : 受信メッセージ解析(CRC,SUMチェック)							*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC UB cpu_com_analyze_msg_check_data(void)
{
	UH sum_rcv = 0;	/* 受信データSUM値 */
	UH crc_rcv = 0; /* 受信データCRC */
	UH tmp = 0;
	UH data_size = 0;
	RING_BUF* p_ring;
	UB seq_num;
	int i;
	
	if(( CPU_COM_MSG_SIZE_MIN > s_cpu_com_rcv_msg_size ) ||
		( CPU_COM_MSG_SIZE_MAX < s_cpu_com_rcv_msg_size )){
		/* メッセージサイズ異常 */
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
		com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"CPU_COM DATA_SIZE_NG\r\n");
#endif
		return OFF;
	}
	/* STX,ETX,SUM、CRCを除いたデータ長 */
	data_size = s_cpu_com_rcv_msg_size - ( CPU_COM_STX_SIZE + CPU_COM_SUM_SIZE + CPU_COM_CRC_SIZE  + CPU_COM_ETX_SIZE );
	
	/* SUM値チェック */
	sum_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size ];			/* SUM値下位ビット */
	sum_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size + 1] << 8;	/* SUM値上位ビット */
	
	cpu_com_sumget( &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_CMD], data_size, &tmp );
	if( sum_rcv != tmp ){
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
		com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"CPU_COM SUM_NG\r\n");
#endif
		return OFF;
	}
	
	/*  CRCチェック */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size + CPU_COM_SUM_SIZE ];			/* CRC下位ビット */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size + CPU_COM_SUM_SIZE + 1 ] << 8;	/* CRC上位ビット */
	cpu_com_crcset( &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_CMD], data_size, &tmp );

	if( crc_rcv != tmp ){
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
		com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"CPU_COM CRC_NG\r\n");
#endif
		return OFF;
	}

	/* シーケンス番号チェック */
	// RD8001暫定：リトライ番号未使用
	seq_num = s_cpu_com_rcv_msg.buf[ CPU_COM_CRC_SIZE ];				/* シーケンス番号下位ビット */

	if( s_cpu_com_res_seq_no == seq_num ){
		//応答を再送
		// 送信リングバッファ書き込み
		p_ring = drv_uart0_get_snd_ring();
		for( i = 0;i < s_cpu_com_snd_size ; i++ ){
			if( E_OK != write_ring_buf( p_ring, s_cpu_com_snd_data[i] )){	/* リングバッファ書き込み */
				break;
			}
		}
		return OFF;
	}
	s_cpu_com_res_seq_no = seq_num;		//シーケンス番号更新
	
	/* チェックOK */
	return ON;
}

/************************************************************************/
/* 関数     : cpu_com_send_proc											*/
/* 関数名   : 周期送信処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.05.08  Axia Soft Design 吉居		初版作成			*/
/*          : 2016.03.10  Axia Soft Design 西島		シーケンスチェック削除(RD1508対応) */
/************************************************************************/
/* 機能 : 周期送信処理													*/
/* 送信処理および送信ステータス(ミドル)の変更を行う						*/
/* 例)																	*/
/* 応答が必要なコマンドは、応答受信またはエラー確定まで送信可能状態とはしない */
/* 連送や応答が不要なコマンドは、送信後送信可能状態とする。				*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
UB dbg_rcv_int = 0;
#endif

STATIC void cpu_com_send_proc(void)
{
#if 1
	//スレーブ
	if( CPU_COM_CMD_NONE != s_p_cpu_com_ds_order->snd_cmd_id ){
		cpu_com_make_send_data();
		s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_NONE;
	}
#else
	//マスター
	UW time;
	UB send_status;
//	UH res_seq_expect;	/* 前回送信時シーケンスNo *///RD1508削除予定：シーケンス
	
	/* ドライバの送信状態を取得 */
	drv_cpu_com_get_send_status( &send_status );
	
	switch( s_cpu_com_snd_status ){
		case CPU_COM_SND_STATUS_RCV_WAIT:
			/* 受信待ち状態 */
//			res_seq_expect = s_cpu_com_snd_seq_no-1;//RD1508削除予定：シーケンス

#if 1		//RD1508暫定	シーケンスチェックは無くす予定
			if( s_cpu_com_snd_cmd == s_cpu_com_ds_input.rcv_cmd ){
#else
			if(( s_cpu_com_snd_cmd == s_cpu_com_ds_input.rcv_cmd ) &&
				( res_seq_expect == s_cpu_com_res_seq_no )){	/* 同一コマンド かつ 同一シーケンスNo */
#endif
				/* 応答受信 */
				s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE; /* 送信完了 */
				
			}else{
				/* 受信なし */
				time_soft_get_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, &time);
				if( 0 == time ){
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
					com_srv_printf(COM_SRV_LOG_COMMAND,(const B*)"CPU_COM_RCV TIME_OUT:%d\r\n",dbg_rcv_int );
//					if( 1 == dbg_rcv_int ){
						dbg_rcv_int = 0;
//						//drv_cpu_com_init(ON);		//効果なし
//					}
#endif
					if( 0 == s_cpu_com_snd_retry_cnt ){
						/* リトライアウト */
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_SEND_NG; /* リトライNG */
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
						com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"CPU間通信リトライアウト(致命的)\r\n");
#endif
					}else{
						if( DRV_CPU_COM_STATUS_CAN_SEND == send_status ){
							/* 送信可能状態 */
#if FUNC_DEBUG_CPU_COM == OFF
							drv_uart0_send_start();	/* 送信開始 */
#else
							drv_uart1_send_start();	/* 送信開始 */
#endif
							/* リトライ回数減算 */
							s_cpu_com_snd_retry_cnt--;
							/* タイムアウト時間再設定 */
							time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
						}
					}
				}
			}
			break;
		case CPU_COM_SND_STATUS_RENSOU:
			/* 連送中状態 */
			time_soft_get_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, &time);
			if( 0 == time ){
				if( DRV_CPU_COM_STATUS_CAN_SEND == send_status ){
					/* 送信可能状態 */
					if( 0 < s_cpu_com_snd_rensou_cnt ){
#if FUNC_DEBUG_CPU_COM == OFF
						drv_uart0_send_start();	/* 送信開始 */
#else
						drv_uart1_send_start();	/* 送信開始 */
#endif
						/* 連送回数減算 */
						s_cpu_com_snd_rensou_cnt--;
						/* タイムアウト時間再設定 */
						time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
					}else{
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE; /* 送信完了 */
					}
				}
			}
			break;
		case CPU_COM_SND_STATUS_SEND_WAIT:
			/* 送信待ち状態 */
			if( DRV_CPU_COM_STATUS_CAN_SEND == send_status ){
				/* 送信可能まで待つ */
				if( TRUE == cpu_com_make_send_data() ){
#if FUNC_DEBUG_CPU_COM == OFF
					drv_uart0_send_start();	/* 送信開始 */
#else
					drv_uart1_send_start();	/* 送信開始 */
#endif
					if( CPU_COM_CMD_TYPE_RETRY == s_cpu_com_snd_type ){
						/* リトライタイプ 受信待ちへ遷移 */
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_RCV_WAIT;
						time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
					}else if( CPU_COM_CMD_TYPE_RENSOU == s_cpu_com_snd_type ){
						/* 連送タイプ 連送待ちへ遷移 */
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_RENSOU;
						time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
					}else{
						/* ワンショット 送信完了へ遷移 */
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE;
					}
				}
				/* アプリからのデータ引き取り完了 */
				cpu_com_clear_send_order();
			}
			break;
		case CPU_COM_SND_STATUS_IDLE:
		case CPU_COM_SND_STATUS_COMPLETE:
		case CPU_COM_SND_STATUS_SEND_NG:
		default:
			/* 送信可能状態 */
			if( CPU_COM_CMD_NONE != s_p_cpu_com_ds_order->snd_cmd_id ){
				/* 送信要求有り */
				if( DRV_CPU_COM_STATUS_CAN_SEND == send_status ){
					/* 送信可能状態 */
					if( TRUE == cpu_com_make_send_data() ){
#if FUNC_DEBUG_CPU_COM == OFF
						drv_uart0_send_start();	/* 送信開始 */
#else
						drv_uart1_send_start();	/* 送信開始 */
#endif
						if( CPU_COM_CMD_TYPE_RETRY == s_cpu_com_snd_type ){
							/* リトライタイプ 受信待ちへ遷移 */
							s_cpu_com_snd_status = CPU_COM_SND_STATUS_RCV_WAIT;
							time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
						}else if( CPU_COM_CMD_TYPE_RENSOU == s_cpu_com_snd_type ){
							/* 連送タイプ 連送待ちへ遷移 */
							s_cpu_com_snd_status = CPU_COM_SND_STATUS_RENSOU;
							time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
						}else{
							/* ワンショット 送信完了へ遷移 */
							s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE;
						}
					}
					/* アプリからのデータ引き取り完了 */
					cpu_com_clear_send_order();
				}else{
					/* ドライバ送信不可 */
					/* 送信待ち状態へ遷移 */
					s_cpu_com_snd_status = CPU_COM_SND_STATUS_SEND_WAIT;
				}
			}else{
				/* 送信要求無しでアイドル状態 */
				s_cpu_com_snd_status = CPU_COM_SND_STATUS_IDLE;
			}
			break;
		
	}
#endif
}


/************************************************************************/
/* 関数     : cpu_com_make_send_data									*/
/* 関数名   : 送信データ生成											*/
/* 引数     : なし														*/
/* 戻り値   : TRUE:正常 FALSE:異常										*/
/* 変更履歴 : 2014.05.08  Axia Soft Design 吉居		初版作成			*/
/************************************************************************/
/* 機能 : アプリからの指示に従い送信データを生成する					*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC UB cpu_com_make_send_data(void)
{
	CPU_COM_CMD_ID cmd_id;
	UH size;
	
	UH crc;
	UH sum;
	UH pos = 0;
	RING_BUF* p_ring;
	INT i = 0;
	UH wr_size;
	
	/* コマンド、データ、データ長のクリア */
	s_cpu_com_snd_cmd = 0x00;
	memset( s_cpu_com_snd_data, 0x00, sizeof(s_cpu_com_snd_data) );
	s_cpu_com_snd_size = 0;
	
	/* リトライ回数、タイムアウト時間、連送回数クリア */
	s_cpu_com_snd_type = CPU_COM_CMD_TYPE_ONESHOT_SEND;
	s_cpu_com_snd_retry_cnt = 0;
	s_cpu_com_snd_timeout = 0;
	s_cpu_com_snd_rensou_cnt = 0;
	
	if(( CPU_COM_CMD_NONE == s_p_cpu_com_ds_order->snd_cmd_id ) || ( CPU_COM_CMD_MAX <= s_p_cpu_com_ds_order->snd_cmd_id ) ||
		( CPU_COM_CMD_TYPE_ONESHOT_RCV == s_tbl_cmd_info[s_p_cpu_com_ds_order->snd_cmd_id].cmd_type )){
		/* コマンドID異常 */
		return FALSE;
	}
	if( CPU_COM_DATA_SIZE_MAX <= s_p_cpu_com_ds_order->data_size ){
		/* データ長異常 */
		return FALSE;
	}
	
	/* コマンドID、データ長取得 */
	cmd_id = s_p_cpu_com_ds_order->snd_cmd_id;
	size = s_p_cpu_com_ds_order->data_size;
	
	
	/* コマンド、データ長保持 */
	s_cpu_com_snd_cmd = s_tbl_cmd_info[cmd_id].cmd;
	s_cpu_com_snd_size = size + CPU_COM_MSG_SIZE_MIN;
	
	/* STX */
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_DLE;
	pos++;
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_STX;
	pos++;
	
	/* コマンドのセット */
	s_cpu_com_snd_data[pos] = s_cpu_com_snd_cmd;
	pos++;
	
	/* SEQのセット */
	s_cpu_com_snd_data[pos] = s_cpu_com_snd_seq_no;
	pos += CPU_COM_SEQ_SIZE;

	/* リトライのセット */
	s_cpu_com_snd_data[pos] = 0;
	pos += CPU_COM_RTY_SIZE;

	
	//RD1508暫定
#if FUNC_DBG_CPU_COM_LOG_EVENT == ON
	if( s_cpu_com_snd_cmd == 0xA0 ){
		//RD1508暫定：CPU間通信デバッグ用処理
#if 0
		if( s_p_cpu_com_ds_order->snd_data[0] == 214 ){
//			return FALSE;
		}
#endif
		com_srv_printf(COM_SRV_LOG_COMMAND,(const B*)"CPUCOM 表示更新:%d\r\n",s_p_cpu_com_ds_order->snd_data[0] + ( s_p_cpu_com_ds_order->snd_data[1] << 8 ));
	}
	if( 0xA1 == s_cpu_com_snd_cmd ){
		com_srv_printf(COM_SRV_LOG_DISP,(const B*)"CPUCOM イベント:%d\r\n",s_p_cpu_com_ds_order->snd_data[0] + ( s_p_cpu_com_ds_order->snd_data[1] << 8 ));
	}
	if( 0xA2 == s_cpu_com_snd_cmd ){
		//ステータス要求
		com_srv_puts(COM_SRV_LOG_COMMAND,(const B*)"CPUCOM ステータス要求\r\n");
	}
#endif
	/* データのセット */
	memcpy( &s_cpu_com_snd_data[pos], s_p_cpu_com_ds_order->snd_data, size );
	pos += size;
	
	/* SUM値のセット */
	cpu_com_sumget( &s_cpu_com_snd_data[CPU_COM_MSG_TOP_POS_CMD], size + CPU_COM_CMD_SIZE + CPU_COM_SEQ_SIZE + CPU_COM_RTY_SIZE , &sum );
	s_cpu_com_snd_data[pos] = (UB)( sum & 0x00FF );
	s_cpu_com_snd_data[pos+1] = (UB)( sum >> 8 );
	pos += CPU_COM_SUM_SIZE;
	
	/* CRCのセット */
	cpu_com_crcset( &s_cpu_com_snd_data[CPU_COM_MSG_TOP_POS_CMD], size + CPU_COM_CMD_SIZE + CPU_COM_SEQ_SIZE + CPU_COM_RTY_SIZE , &crc );
	s_cpu_com_snd_data[pos] = (UB)( crc & 0x00FF );
	s_cpu_com_snd_data[pos+1] = (UB)( crc >> 8 );
	pos += CPU_COM_CRC_SIZE;
	
	/* ETX */
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_DLE;
	pos++;
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_ETX;
	pos++;

	/* DLE 拡張 */
	s_cpu_com_snd_size = cpu_com_dle_extension( &s_cpu_com_snd_data[0], s_cpu_com_snd_size );
	
	/* コマンドタイプ、リトライ・連送回数セット */
	s_cpu_com_snd_type = s_tbl_cmd_info[cmd_id].cmd_type;
	s_cpu_com_snd_retry_cnt = s_tbl_cmd_info[cmd_id].retry_cnt;
	s_cpu_com_snd_timeout = s_tbl_cmd_info[cmd_id].retry_time;
	if( 1 < s_tbl_cmd_info[cmd_id].rensou_cnt ){
		/* 連送回数は初回を含むため1減算 */
		s_cpu_com_snd_rensou_cnt = s_tbl_cmd_info[cmd_id].rensou_cnt-1;
	}
	
	/* シーケンスNo加算 */
	s_cpu_com_snd_seq_no++;
	
	
	
	// 送信リングバッファ書き込み
	p_ring = drv_uart0_get_snd_ring();
	for( i = 0;i < s_cpu_com_snd_size ; i++ ){
		if( E_OK != write_ring_buf( p_ring, s_cpu_com_snd_data[i] )){	/* リングバッファ書き込み */
			break;
		}
	}
#if FUNC_DEBUG_CPU_COM == OFF
	drv_uart0_send_start();
#else
	drv_uart1_send_start();
#endif
	
	return TRUE;
}


/************************************************************************/
/* 関数     : cpu_com_clear_send_order									*/
/* 関数名   : 送信要求クリア											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.05.08  Axia Soft Design 吉居		初版作成			*/
/************************************************************************/
/* 機能 : アプリからの指示をクリアする									*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void cpu_com_clear_send_order(void)
{
	if( NULL != s_p_cpu_com_ds_order ){
		/* コマンドクリア */
		s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_NONE;
		/* データクリア */
		memset( s_p_cpu_com_ds_order->snd_data, 0x00, CPU_COM_DATA_SIZE_MAX );
		/* データ長クリア */
		s_p_cpu_com_ds_order->data_size = 0;
	}
}

/************************************************************************/
/* 関数     : cpu_com_sub_reset											*/
/* 関数名   : サブマイコン側リセット処理								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.06.24 Axia Soft Design 吉居	初版作成				*/
/*          : 2016.02.23 Axia Soft Design 西島	サブマイコンに名称変更	*/
/************************************************************************/
/* 機能 : サブマイコンをリセットする									*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
#if 0
void cpu_com_sub_reset(void)
{
	/* ドライバ関数をコール */
	drv_cpu_com_sub_reset();
}
#endif

/************************************************************************/
/* 関数     : cpu_com_get_status										*/
/* 関数名   : CPUステータス取得											*/
/* 引数     : なし														*/
/* 戻り値   :	CPU_COM_SND_STATUS_IDLE			送信可能状態			*/
/*				CPU_COM_SND_STATUS_RCV_WAIT		応答受信待ち状態		*/
/* 				CPU_COM_SND_STATUS_RENSOU		連送中状態				*/
/* 				CPU_COM_SND_STATUS_COMPLETE		送信完了状態			*/
/* 				CPU_COM_SND_STATUS_SEND_NG		リトライNG				*/
/* 変更履歴 : 2014.07.09 Axia Soft Design 西島		初版作成			*/
/************************************************************************/
/* 機能 : CPUステータス取得												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
UB cpu_com_get_status(void)
{
	/* ドライバ関数をコール */
	return s_cpu_com_snd_status;
}


/************************************************************************/
/* 関数     : cpu_com_dle_extension										*/
/* 関数名   : DLE伸長													*/
/* 引数     : *data	DLE伸長するデータ									*/
/*          : size	DLE伸長するサイズ									*/
/* 戻り値   : DLE伸長後のサイズ											*/
/* 変更履歴 : 2015.10.23 Axia Soft Design 西島		初版作成			*/
/************************************************************************/
/* 機能 : DLE伸長														*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
UH cpu_com_dle_extension( UB* data, UH size )
{
	UB cpu_com_buf_org[CPU_COM_DATA_SIZE_MAX];
	int i = 0;
	UH extension_size = 0;
	
	memcpy( &cpu_com_buf_org[0], data, size );
	
	// STX対応 ※無拡張
	*data++ = cpu_com_buf_org[0];
	*data++ = cpu_com_buf_org[1];
	
	// DLE拡張
	for( i = CPU_COM_STX_SIZE; i < ( size - CPU_COM_ETX_SIZE ); i++ ){
		*data++ = cpu_com_buf_org[i];
		if( CPU_COM_CTRL_CODE_DLE == cpu_com_buf_org[i] ){
			*data++ = CPU_COM_CTRL_CODE_DLE;
			extension_size++;
		}
	}
	// ETX対応 ※無拡張
	*data++ = cpu_com_buf_org[i++];
	*data   = cpu_com_buf_org[i];

	return ( size + extension_size );
}



STATIC UB			s_drv_cpu_com_snd_status;							/* CPU間通信送信ステータス */
/************************************************************************/
/* 関数     : drv_cpu_com_set_send_status								*/
/* 関数名   : CPU間通信送信状態セット									*/
/* 引数     : UB status : 更新する状態									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.04.25 Axia Soft Design 吉居 久和	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* CPU間通信の送信状態を更新する										*/
/************************************************************************/
/* 注意事項 :															*/
/* 未使用関数(2016.02.24)プロトコル変更により上位から設定されなくなった */
/************************************************************************/
void drv_cpu_com_set_send_status( UB status )
{
	
	s_drv_cpu_com_snd_status = status;
}

/************************************************************************/
/* 関数     : drv_cpu_com_get_send_status								*/
/* 関数名   : CPU間通信送信状態取得										*/
/* 引数     : UB *status : 送信状態										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.04.25 Axia Soft Design 吉居 久和	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* CPU間通信の送信状態を取得する										*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void drv_cpu_com_get_send_status( UB *status )
{
	
	*status = s_drv_cpu_com_snd_status;
}


//RD8001暫定：テスト版
void test_cpu_com_send( void )
{
#if 0
	s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_STATUS_CHG_REQ;		/* 送信コマンドID */
	s_p_cpu_com_ds_order->snd_data[0] = 0xAA;								/* 送信データ */
	s_p_cpu_com_ds_order->data_size = 1;										/* 送信データ長 */
	
	// データ作成
	cpu_com_make_send_data();
#endif
}
/************************************************************************/
/* END OF TEXT															*/
/************************************************************************/
/************************************************************************/
/* システム名   : RD8001 快眠チェッカー									*/
/* ファイル名   : cpu_com.c												*/
/* 機能         : CPU間通信ミドル										*/
/* 変更履歴     : 2018.04.10 Axia Soft Design 西島		初版作成		*/
/* 注意事項     : なし													*/
/************************************************************************/
/********************/
/*     include      */
/********************/
#include	"header.h"

/********************/
/* プロトタイプ宣言 */
/********************/
STATIC void cpu_com_crcset(UB *msg, UH size, UH *crc);
STATIC void cpu_com_rcv_proc(void);
STATIC UB cpu_com_analyze_msg(void);
STATIC UB cpu_com_analyze_msg_check_etx(void);
STATIC UB cpu_com_analyze_msg_check_data(void);
STATIC void cpu_com_send_proc(void);
STATIC UB cpu_com_make_send_data(void);
STATIC UH cpu_com_dle_extension( UB* data, UH size );


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
//RD8001暫定：テーブル削除予定
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
/* 関数     : cpu_com_send_end											*/
/* 関数名   : CPU間送信(終了まで)										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.08  Axia Soft Design 西島		初版作成			*/
/************************************************************************/
/* 機能 : CPU間送信(終了まで)											*/
/************************************************************************/
/* 注意事項 :ドライバレベルの送信終了まで本関数で行う					*/
/************************************************************************/
void cpu_com_send_end( void )
{
	/* アプリからデータ取得 */
	ds_get_cpu_com_order( &s_p_cpu_com_ds_order );
	
	//スレーブ
	if( CPU_COM_CMD_NONE != s_p_cpu_com_ds_order->snd_cmd_id ){
		cpu_com_make_send_data();
		s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_NONE;
	}
	
	while(1){
		if( OFF == drv_uart0_get_send() ){
			break;
		}
	}
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
		/* アプリ通知用のデータに受信データをセット */
		memcpy( s_cpu_com_ds_input.rcv_data, &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_DATA], ( s_cpu_com_rcv_msg_size- CPU_COM_MSG_SIZE_MIN ));
#endif
	}else{
#if 1
		/* 受信なし */
		s_cpu_com_ds_input.rcv_cmd = 0x00;
		s_cpu_com_res_seq_no = 0x0000;
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
//	UH sum_rcv = 0;	/* 受信データSUM値 */
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
	data_size = s_cpu_com_rcv_msg_size - ( CPU_COM_STX_SIZE + CPU_COM_CRC_SIZE  + CPU_COM_ETX_SIZE );
	
	/*  CRCチェック */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size ];			/* CRC下位ビット */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size + 1 ] << 8;	/* CRC上位ビット */
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
/* 周期送信処理を行う													*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void cpu_com_send_proc(void)
{
	//スレーブ
	if( CPU_COM_CMD_NONE != s_p_cpu_com_ds_order->snd_cmd_id ){
		cpu_com_make_send_data();
		s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_NONE;
	}
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
//	UH sum;
	UH pos = 0;
	RING_BUF* p_ring;
	INT i = 0;
//	UH wr_size;
	
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
	
	/* CRCのセット */
	cpu_com_crcset( &s_cpu_com_snd_data[CPU_COM_MSG_TOP_POS_CMD], size + CPU_COM_CMD_SIZE + CPU_COM_SEQ_SIZE , &crc );
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
STATIC UH cpu_com_dle_extension( UB* data, UH size )
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


// ==================================================
//RD8001暫定：テストコード↓↓↓↓↓↓↓↓↓↓↓↓↓
// ==================================================


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
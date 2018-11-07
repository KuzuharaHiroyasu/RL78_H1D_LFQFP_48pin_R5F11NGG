/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : main.c														*/
/* 機能         : 全体管理（関数,変数,定数定義）								*/
/* 変更履歴     : 2018.03.02 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/

#include "header.h"

/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/
STATIC UB main_sleep_check(void);
STATIC void user_main_mode_sensor(void);
STATIC W main_ad24_to_w( UH bufferH, UH bufferL );
STATIC void main_init(void);
STATIC void sw_cyc(void);
STATIC void mode_cyc(void);
STATIC void main_cyc(void);
STATIC void disp_cyc(void);
STATIC void disp_ptn_order( void );
STATIC void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
STATIC void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
STATIC void main_acl_init(void);
STATIC void main_acl_stop(void);
STATIC void main_acl_start(void);
STATIC void main_acl_read(void);
STATIC void stop_in( void );
STATIC void stop_restore_ram( void );
STATIC void main_cpu_com_snd_sensor_data( void );
STATIC void main_cpu_com_proc(void);
STATIC void main_cpu_com_rcv_sts( void );
STATIC void main_cpu_com_rcv_sensing_order( void );
STATIC void main_cpu_com_rcv_date_set( void );
STATIC void main_cpu_com_rcv_pc_log( void );
STATIC void main_cpu_com_rcv_mode_chg( void );
STATIC void main_cpu_com_rcv_prg_hd_ready(void);
STATIC void main_cpu_com_rcv_prg_hd_check(void);
STATIC void main_cpu_com_rcv_disp(void);
STATIC void main_cpu_com_rcv_version(void);

/************************************************************/
/* 変数定義													*/
/************************************************************/
STATIC T_UNIT s_unit;
STATIC DS s_ds;

/************************************************************/
/* テーブル定義												*/
/************************************************************/
// バージョン(APL)
const B		version_product_tbl[]= {0, 0, 0, 24};				/* ソフトウェアバージョン */
																/* バージョン表記ルール */
																/* ①メジャーバージョン：[0 ～ 9] */
																/* ②マイナーバージョン：[0 ～ 9]  */
																/* ③リビジョン：[0 ～ 9] */
																/* ④ビルドバージョン：[0 ～ 9] */

// バージョンのアドレス(BOOT)
const UB	* const version_boot_tbl= (const UB*)0x00006EF0;	/* ブートバージョンアドレス */

/* 受信データ処理 関数テーブル */
STATIC const CPU_COM_RCV_CMD_TBL s_cpu_com_rcv_func_tbl[CPU_COM_CMD_MAX] = {
	/* コマンド */		/* 関数  */					
	{	0x00,			NULL							},	/* 【CPU間通信コマンド】コマンド無し				*/
	{	0xE0,			main_cpu_com_rcv_sts			},	/* 【CPU間通信コマンド】ステータス要求				*/
	{	0xA0,			NULL							},	/* 【CPU間通信コマンド】センサーデータ更新			*/
	{	0xA1,			main_cpu_com_rcv_sensing_order	},	/* 【CPU間通信コマンド】センサシング指示			*/
	{	0xB0,			main_cpu_com_rcv_mode_chg		},	/* 【CPU間通信コマンド】状態変更(G1D)				*/
	{	0xF0,			main_cpu_com_rcv_pc_log			},	/* 【CPU間通信コマンド】PCログ送信(内部コマンド)	*/
	{	0xB1,			main_cpu_com_rcv_date_set		},	/* 【CPU間通信コマンド】日時設定					*/
	{	0xD5,			main_cpu_com_rcv_prg_hd_ready	},	/* 【CPU間通信コマンド】プログラム転送準備			*/
	{	0xD2,			NULL							},	/* 【CPU間通信コマンド】プログラム転送開始			*/
	{	0xD4,			NULL							},	/* 【CPU間通信コマンド】プログラム転送消去			*/
	{	0xD0,			NULL							},	/* 【CPU間通信コマンド】プログラム転送データ		*/
	{	0xD1,			NULL							},	/* 【CPU間通信コマンド】プログラム転送結果			*/
	{	0xD3,			main_cpu_com_rcv_prg_hd_check	},	/* 【CPU間通信コマンド】プログラム転送確認			*/
	{	0xB2,			main_cpu_com_rcv_disp			},	/* 【CPU間通信コマンド】表示指示					*/
	{	0xB3,			main_cpu_com_rcv_version		},	/* 【CPU間通信コマンド】バージョン					*/
};


#if 0
rtc_counter_value_t dbg_rtc;
rtc_counter_value_t dbg_rtc_set;
UB dbg_rtc_flg = 0;
#endif

/************************************************************************/
/* 関数     : user_main													*/
/* 関数名   : メインループ												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.03.02 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* メインループ															*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void user_main(void)
{
//	UW time = 0;
    
	vect_set_tbl_apl();	

    main_init();
	
#if 0
	// RTC設定
	{
		rtc_counter_value_t rtc;
		
		if( dbg_set_clock == 7 ){
	//		R_RTC_Set_CounterValue( rtc );
		}
		R_RTC_Get_CounterValue( &rtc );
		s_unit.hour = rtc.hour;
		s_unit.min = rtc.min;
		s_unit.sec = rtc.sec;
	//	R_RTC_Start();
	}
#endif
	
	// 起動ログ
	com_srv_log_title();

	while (1U)
	{
		com_srv_cyc();
		sw_cyc();
		main_cpu_com_proc();
		mode_cyc();				// main_cpu_com_proc()の後ろで実施 ※CPU間受信後の処理
		cpu_com_proc();
		main_cyc();
		disp_cyc();
		
		if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
			user_main_mode_sensor();
		}else if( SYSTEM_MODE_IDLE_REST == s_unit.system_mode ){
			// スリープしない
			// RD8001暫定：残量表示の一部は表示間隔が細かい為にスリープすると正常に動作しない。表示の変更を希望する。
		}else{
			if( ON == main_sleep_check() ){
					stop_in();
			}
		}

		if( OFF == s_unit.non_wdt_refresh ){
			wdt_refresh();		//WDTリフレッシュ
		}
	}
}


/************************************************************************/
/* 関数     : main_sleep_check											*/
/* 関数名   : スリープチェック											*/
/* 引数     : なし														*/
/* 戻り値   : ON		sleep有効										*/
/*          : OFF		sleep無効										*/
/* 変更履歴 : 2018.09.12  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* スリープチェック														*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC UB main_sleep_check(void)
{
	UB ret = OFF;
	
	// スリープ突入条件検討
	if(( OFF == drv_intp_read_g1d_int() ) &&		// G1Dからの割り込み
#if FUNC_DEBUG_LOG == ON 
	   ( OFF == drv_uart0_get_send() )    &&		// ログ通信
#endif
	   ( CPU_COM_CMD_NONE == s_ds.cpu_com.order.snd_cmd_id ) &&		// CPU間ミドル送信
	   ( CPU_COM_CMD_NONE == s_ds.cpu_com.input.rcv_cmd ) &&		// CPU間ミドル受信
		   ( OFF == drv_uart1_get_send() )){						// CPU間ドライバ送信
		ret = ON;
	}
	return ret;
}




/************************************************************************/
/* 関数     : user_main_mode_sensor										*/
/* 関数名   : センシング処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.12  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* センシング処理														*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
#if FUNC_DEBUG_RAW_VAL_OUT == ON
UB dbg_send_cnt;
char dbg_tx_data[50] = {0};
int dbg_len;
#endif
// RD8001暫定：処理タイミングの制御仕様未定
STATIC void user_main_mode_sensor(void)
{
	UW time;
//	UH bufferH_ave;		// 平均用
//	UH bufferL_ave;		// 平均用		
	UH bufferH;
	UH bufferL;
	
	
//	MEAS meas;
	if( ON == s_unit.sensing_end_flg ){
		return;
	}
	
	// センシング遅延 ※まだスリープしないのでソフトタイマーは有効
	time_soft_get_10ms( TIME_TYPE_10MS_SENSING_DELAY, &time );
	if( 0 != time ){
		return;
	}
	
	// 12時間判定
	if( s_unit.sensing_cnt_50ms >= HOUR12_CNT_50MS ){
		s_unit.system_evt = EVENT_COMPLETE;
		s_unit.sensing_end_flg = ON;
	}
	
	// センシング中SW押下処理
	if( ON == drv_intp_read_pow_sw() ){
		INC_MAX( s_unit.sensing_sw_on_cnt, UCHAR_MAX );
	}else{
		if( SENSING_SW_LONG_CNT_50MS <= s_unit.sensing_sw_on_cnt ){
			// 規定時間以上連続押下と判断
			s_unit.system_evt = EVENT_POW_SW_LONG;
			s_unit.sensing_end_flg = ON;
		}
		s_unit.sensing_sw_on_cnt = 0;
	}
	
	// 電池残量低下
	if( DENCH_ZANRYO_STS_MIN == s_unit.dench_sts ){
		s_unit.sensing_end_flg = ON;
	}
	// 充電ON
	if( ON == s_unit.bat_chg ){
		s_unit.sensing_end_flg = ON;
	}
	
	// センサーモード
	// メイン周期(50ms)
	if( ON == s_unit.main_cyc_req ){
		s_unit.sensing_cnt_50ms++;		//12時間カウントアップはこの箇所で行う ※最終リリース時は50ms毎にしかログ等でも起床する為

		R_DAC1_Start();			// ■DAC ON　※共通用途

		// ↓↓マイク用(呼吸、イビキ)開始↓↓
		R_DAC1_Set_ConversionValue( 0x0000 );
		R_AMP0_Start();		// ■AMP0 ON
		R_PGA1_Start();		// ■PGA1 ON

		wait_ms( 2 );
		adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
		
		wait_ms( 2 );
#if 0	// RD8001暫定：AMP0,PGA1をOFFすると正常にイビキ等が取得できない。制御仕様
		R_AMP0_Stop();		// □AMP0 OFF
		R_PGA1_Stop();		// □PGA1 OFF
#endif
		// ↑↑マイク用(呼吸、イビキ)終了↑↑

		R_PGA_DSAD_Start();		// ■PGA0_DSAD  ON
		R_AMP2_Start();			// ■AMP2       ON
		
#if FUNC_VALID_AMP == ON
		R_DAC1_Set_ConversionValue( 0x0500 );
		// 赤色光ON
		drv_o_port_sekigai( OFF );
		drv_o_port_sekishoku( ON );
		wait_ms( 2 );
		

		pga_do();
//		R_PGA_DSAD_Get_AverageResult(&bufferH_ave, &bufferL_ave);		//平均
		R_PGA_DSAD_Get_Result(&bufferH, &bufferL);
		s_unit.meas.info.dat.sekishoku_val = main_ad24_to_w( bufferH, bufferL );
		wait_ms( 2 );

		// 赤外光ON
		if( ON == s_unit.sensing_sekigai_flg ){
			drv_o_port_sekigai( ON );
			drv_o_port_sekishoku( OFF );

			R_DAC1_Set_ConversionValue( 0x0B00 );
			wait_ms( 2 );
			pga_do();
	//		R_PGA_DSAD_Get_AverageResult(&bufferH_ave, &bufferL_ave);		//平均
			R_PGA_DSAD_Get_Result(&bufferH, &bufferL);
			s_unit.meas.info.dat.sekigaival = main_ad24_to_w( bufferH, bufferL );
			
			R_PGA_DSAD_Stop();		// □PGA0_DSAD  OFF
			R_AMP2_Stop();			// □AMP2  OFF
			
			wait_ms( 2 );
			// 赤外光,赤色光OFF
			drv_o_port_sekigai( OFF );
			drv_o_port_sekishoku( OFF );
			
			R_DAC1_Stop();			// □DAC  OFF
		}else{
			// 無効時はデータクリアして止める。待ち不要
			s_unit.meas.info.dat.sekigaival = 0;
			R_PGA_DSAD_Stop();		// □PGA0_DSAD  OFF
			R_AMP2_Stop();			// □AMP2  OFF
			
			// 赤外光,赤色光OFF
			drv_o_port_sekigai( OFF );
			drv_o_port_sekishoku( OFF );
			
			R_DAC1_Stop();		// □DAC  OFF
		}
		
		// 加速度センサ取得(10回(500ms毎)に1回)
#if FUNC_DEBUG_RAW_VAL_OUT == OFF
		if( ++s_unit.acl_sensor_timing >= ACL_TIMING_VAL ){
			s_unit.acl_sensor_timing = 0;
			main_acl_read();
		}
		
#else
		//生値デバッグ出力　※加速度は無効
		if( dbg_send_cnt++ >= 0 ){	
			dbg_send_cnt = 0;
			{
				
				
				
				dbg_len = sprintf((char*)dbg_tx_data, "%ld,%ld,%d,%d,0,0,0\r\n",		 s_unit.meas.info.dat.sekishoku_val
																					,s_unit.meas.info.dat.sekigaival
																					,s_unit.meas.info.dat.kokyu_val
																					,s_unit.meas.info.dat.ibiki_val);
				com_srv_send( &dbg_tx_data[0], dbg_len );
			}
		}
		wait_ms( 5 );
#endif
		
#endif
		
		s_unit.main_cyc_req  = OFF;
		
		// 赤外、赤色OFF
		drv_o_port_sekigai( OFF );
		drv_o_port_sekishoku( OFF );
		
		// 送信完了待ち
		cpu_com_send_end_wait();		// 送信完了待ち
#if 1
		stop_in();		//　ストップモード
#endif
		// データ送信開始 ※ストップモード前だとストップモードへの突入が出来ないからセンシングしながら送信を並行して行う。
		main_cpu_com_snd_sensor_data();	
	}

}

/************************************************************************/
/* 関数     : main_ad24_to_w											*/
/* 関数名   : AD24bit4byte(符号あり)変換								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.03.02 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* AD24bit4byte(符号あり)変換											*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC W main_ad24_to_w( UH bufferH, UH bufferL )
{
	W value = 0;
	UW work_uw = 0;
	W work_w = 0;
	UB m_flg = 0;
	
	// bufferH ※上位8bit,中位8bit
	// bufferL ※下位8bit,変換結果8bit
	work_uw  = bufferH;
	work_uw <<= 16;
	work_uw += bufferL;
	work_uw >>= 8;
	work_uw &= 0x00FFFFFF;		// 24bitデータ化
	// 負数処理
	if( work_uw & 0x00800000 ){
		work_w = work_uw;
		work_w *= -1;			// プラスに変換
		m_flg = 1;
	}else{
		work_w = work_uw;
	}
	work_uw = work_w;
	work_uw &= 0x007FFFFF;
	if( 1 == m_flg ){
		value = (W)work_uw * (W)-1;
	}else{
		value = (W)work_uw;
	}
		
	return value;
}

/************************************************************************/
/* 関数     : main_init													*/
/* 関数名   : 初期化													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.03.02 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 初期化																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void main_init(void)
{
	wdt_refresh();		//WDTリフレッシュ

	//データ初期化
	memset( &s_unit, 0, sizeof(s_unit) );
	memset( &s_ds, 0, sizeof(s_ds) );
	
	//データ初期化(0以外)
	s_unit.disp.last_ptn = 0xFF;
	
	// ミドル初期化
	com_srv_init();
	cpu_com_init();
	time_init();
	
	EI();
	main_acl_init();	// 加速度センサ初期化(割り込み使用)

}

/************************************************************************/
/* 関数     : sw_cyc													*/
/* 関数名   : SW周期処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.03.02 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* SW周期処理															*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void sw_cyc(void)
{
	UB pow_sw;
	UW time = 0;
	
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		// センシング中はソフトウェアタイマーが使用できないセンシング処理内でSWを行う
		return;
	}
	
	pow_sw = drv_intp_read_pow_sw();
	
	s_unit.disp.ptn_inital_order = OFF;
	
	time_soft_get_10ms( TIME_TYPE_10MS_POW_SW, &time );
	
	if( ON == pow_sw ){		// ON処理
		// なにもしない(電源SW押下タイマー継続)
		if( 0 == time ){
			s_unit.disp.ptn_inital_order = ON;		// 10秒以上押し続けている時だけ表示ON
		}
	}else{					// OFF処理
		if( ON == s_unit.pow_sw_last ){
			// ON→OFFエッジ
			if( time <= ( TIME_10MS_CNT_POW_SW_INITIAL_DISP - TIME_10MS_CNT_POW_SW_LONG )){
				// 規定時間以上連続押下と判断
				s_unit.system_evt = EVENT_POW_SW_LONG;
//				s_unit.disp.ptn_sw_long_order = ON;		// 長押し表示指示 ※影舞22：G1Dからの通知に変更
				// 電源SW押下タイマー再スタート 
//				time_soft_set_10ms( TIME_TYPE_10MS_SW_LONG_DISP, TIME_10MS_CNT_POW_SW_LONG_DISP );
				// センシングの処理に移行
//				if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
//					s_unit.sensing_end_flg = ON;
//				}
			}else if( time <= ( TIME_10MS_CNT_POW_SW_INITIAL_DISP - TIME_10MS_CNT_POW_SW_SHORT )){
				s_unit.system_evt = EVENT_POW_SW_SHORT;
			}else{
				// 何もしない
			}
		}
		// 電源SW押下タイマー再スタート 
		time_soft_set_10ms( TIME_TYPE_10MS_POW_SW, TIME_10MS_CNT_POW_SW_INITIAL_DISP );
	}
	s_unit.pow_sw_last = pow_sw;
	
	time_soft_get_10ms( TIME_TYPE_10MS_SW_LONG_DISP, &time );
	if( 0 == time ){
		s_unit.disp.ptn_sw_long_order = OFF;		// 長押し表示指示
	}
}

/************************************************************************/
/* 関数     : mode_cyc													*/
/* 関数名   : モード周期処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.03.02 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 初期化																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void mode_cyc(void)
{
	
	// システムモード変更時処理
	if( s_unit.last_system_mode != s_unit.system_mode ){
		if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
			time_soft_set_10ms( TIME_TYPE_10MS_SENSING_DELAY, TIME_10MS_CNT_SENSING_DELAY );
			s_unit.sensing_end_flg = OFF;
			s_unit.sensing_cnt_50ms = 0;
			
			// センシング時有効機能
			main_acl_start();
			drv_o_port_mike( ON );
		}else{
			
			// センシング時以外無効機能 ※消費電流低減の為
			main_acl_stop();
			drv_o_port_mike( OFF );
		}
		
	}
	
	s_unit.last_system_mode = s_unit.system_mode;
}

/************************************************************************/
/* 関数     : main_cyc													*/
/* 関数名   : アプリ周期処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 初期化																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void main_cyc(void)
{
	UH dench_val;
	UB bat_chg_1;
	UB bat_chg_2;
	
	UB kensa_1;
	UB kensa_2;
//	UB bat_chg_off_trig = OFF;
	UW time = 0;
	
	if(( ON == s_unit.sensing_end_flg ) && 
	   ( SYSTEM_MODE_SENSING == s_unit.system_mode )){
		// センシング終了待ち時は以下の状態を更新しない ※G1Dとの状態不一致を防ぐため
		return;
	}
	
	// 電池残量
	dench_val = DENCH_ZANRYO_1_VAL;
	
	adc_dench( &dench_val );
	
	if( dench_val >= DENCH_ZANRYO_1_VAL ){
		s_unit.dench_sts = DENCH_ZANRYO_STS_MAX;
	}else if( dench_val >= DENCH_ZANRYO_2_VAL ){
		s_unit.dench_sts = DENCH_ZANRYO_STS_HIGH;
	}else if( dench_val >= DENCH_ZANRYO_3_VAL ){
		s_unit.dench_sts = DENCH_ZANRYO_STS_LOW;
	}else{
		s_unit.dench_sts = DENCH_ZANRYO_STS_MIN;
	}
	
	
	// 充電状態(ポート) ※2回一致
	bat_chg_1 = drv_i_port_bat_chg();
	bat_chg_2 = drv_i_port_bat_chg();
	if( bat_chg_1 == bat_chg_2 ){
		// 2回一致で更新
		s_unit.bat_chg = bat_chg_1;
	}
	if(( OFF == s_unit.bat_chg_last ) &&
	   ( ON == s_unit.bat_chg )){	
		time_soft_set_10ms( TIME_TYPE_10MS_CHG_ON_DISP_IVALID, TIME_10MS_CNT_CHG_ON_DISP_IVALID );
	}
	
	// 充電完了チェック
	if( ON == s_unit.bat_chg ){
		time_soft_set_10ms( TIME_TYPE_10MS_BAT_CHG_FIN, TIME_10MS_CNT_BAT_CHG_FIN );
		// OFFエッジの規定時間後なのでONの場合は一旦充電完了状態は未定とする
		s_unit.bat_chg_fin_state = BAT_CHG_FIN_STATE_UNKNOWN;
	}else{
		time_soft_get_10ms( TIME_TYPE_10MS_BAT_CHG_FIN, &time );
		if( 0 == time ){
			if( BAT_CHG_FIN_STATE_UNKNOWN == s_unit.bat_chg_fin_state ){
				// 充電完了状態が未定(OFFエッジ)の場合に充電完了状態を確定させる
				if( dench_val >= DENCH_BAT_CHG_FIN_VAL ){
					s_unit.bat_chg_fin_state = BAT_CHG_FIN_STATE_ON;
				}else{
					s_unit.bat_chg_fin_state = BAT_CHG_FIN_STATE_OFF;
				}
			}
		}
	}
	s_unit.bat_chg_last = s_unit.bat_chg;


	
	// 検査状態 ※2回一致
	kensa_1 = drv_i_port_kensa();
	kensa_2 = drv_i_port_kensa();
	
	if( kensa_1 == kensa_2 ){
		// 2回一致で更新
		s_unit.kensa = kensa_1;
	}
	s_unit.kensa_last = s_unit.kensa;
	
}


/************************************************************************/
/* 関数     : disp_cyc													*/
/* 関数名   : 表示周期処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.24 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 表示周期処理															*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void disp_cyc(void)
{
	UW time = 0;
	
	// センサー中は赤色LED制御は避ける
	if(( SYSTEM_MODE_SENSING == s_unit.system_mode ) &&
	   ( OFF == s_unit.sensing_end_flg )){
		return;
	}
	
	// 表示パターン指示
	disp_ptn_order();
	
	if( s_unit.disp.last_ptn != s_unit.disp.ptn ){
		s_unit.disp.seq = 0;
		// OFF 処理
		// RD8001暫定：LEDを光らせる為のダック設定の最適値とは？制御仕様未定義
		if( DISP_PTN_OFF == s_unit.disp.ptn ){
			R_DAC1_Stop();			// □DAC  OFF
//			R_PGA_DSAD_Stop();		// □PGA0_DSAD  OFF
			R_AMP2_Stop();			// □AMP2  OFF
			R_DAC1_Set_ConversionValue( 0x0500 );
		}else{
			R_DAC1_Start();			// ■DAC        ON
//			R_PGA_DSAD_Start();		// ■PGA0_DSAD  ON
			R_AMP2_Start();			// ■AMP2       ON
			R_DAC1_Set_ConversionValue( 0x0500 );
		}
#if 0
		if( DISP_PTN_OFF == s_unit.disp.last_ptn ){
			R_DAC1_Start();			// ■DAC        ON
//			R_PGA_DSAD_Start();		// ■PGA0_DSAD  ON
			R_AMP2_Start();			// ■AMP2       ON
			R_DAC1_Set_ConversionValue( 0x0500 );
		}
#endif
	}
	
	if( DISP_PTN_SELF_CHECK_FLASH_100MS == s_unit.disp.ptn ){
		drv_o_port_sekishoku( OFF );
		time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
		if( 0 == time ){
			time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
			INC_MAX_INI( s_unit.disp.seq, DISP_PTN_SELF_CHECK_FLASH_100MS_SEQ_MAX, 0 );
		}
		if( 0 == s_unit.disp.seq ){
			drv_o_port_sekishoku( ON );
		}else{
			drv_o_port_sekishoku( OFF );
		}
		
	}else if( DISP_PTN_SELF_CHECK_ON == s_unit.disp.ptn ){
		drv_o_port_sekishoku( ON );
	}else if( DISP_PTN_DENCH_ZANRYO_1 == s_unit.disp.ptn ){
		if( 0 == s_unit.disp.seq ){
			drv_o_port_sekishoku( OFF );
			time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_500MS );
			s_unit.disp.seq = 1;
		}else if( 1 == s_unit.disp.seq ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_3S );
				s_unit.disp.seq = 2;
			}
		}else{
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( OFF );
			}
		}
	}else if( DISP_PTN_DENCH_ZANRYO_2 == s_unit.disp.ptn ){
		if( 0 == s_unit.disp.seq ){
			drv_o_port_sekishoku( OFF );
			time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_500MS );
			s_unit.disp.seq = 1;
		}else if( 1 == s_unit.disp.seq ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 2;
			}
		}else if( 2 == s_unit.disp.seq ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 3;
			}
		}else if( 3 == s_unit.disp.seq ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 4;
			}
		}else{
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( OFF );
			}
		}
	}else if( DISP_PTN_DENCH_ZANRYO_3 == s_unit.disp.ptn ){
		if( 0 == s_unit.disp.seq ){
			drv_o_port_sekishoku( OFF );
			time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_500MS );
			s_unit.disp.seq = 1;
		}else if( s_unit.disp.seq < DISP_PTN_FLASH_DENCH_ZANRYO_3_SEQ_MAX ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_50MS );
				INC_MAX( s_unit.disp.seq, DISP_PTN_FLASH_DENCH_ZANRYO_3_SEQ_MAX );
				if( s_unit.disp.seq & 0x01 ){
					drv_o_port_sekishoku( ON );
				}else{
					drv_o_port_sekishoku( OFF );
				}
			}
		}else{
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( OFF );
			}
		}
	}else if( DISP_PTN_BLE_ON == s_unit.disp.ptn ){
		time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
		if( 0 == time ){
			if( 0 == s_unit.disp.seq ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 1;
			}else{
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 0;
			}
		}
	}else if( DISP_PTN_BLE_OFF == s_unit.disp.ptn ){
		time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
		if( 0 == time ){
			if( 0 == s_unit.disp.seq ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 1;
			}else if( 1 == s_unit.disp.seq ){
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 2;
			}else if( 2 == s_unit.disp.seq ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 3;
			}else if( 3 == s_unit.disp.seq ){
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 4;
			}else if( 4 == s_unit.disp.seq ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 5;
			}else{
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 0;
			}
		}
	}else if( DISP_PTN_INITAL_RESET == s_unit.disp.ptn ){
		drv_o_port_sekishoku( ON );
	}else if( DISP_PTN_PW_SW_LONG == s_unit.disp.ptn ){
		drv_o_port_sekishoku( ON );
	}else if( DISP_PTN_CHG_ON == s_unit.disp.ptn ){
		time_soft_get_10ms( TIME_TYPE_10MS_CHG_ON_DISP_IVALID, &time );
		if( 0 == time ){
			drv_o_port_sekishoku( ON );
		}else{
			drv_o_port_sekishoku( OFF );
		}
	}else{
		drv_o_port_sekishoku( OFF );
//		drv_o_port_sekigai( OFF );
	}
	
	// 前回パターン格納
	s_unit.disp.last_ptn = s_unit.disp.ptn;
}


/************************************************************************/
/* 関数     : disp_ptn_order											*/
/* 関数名   : 表示パターン指示											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.24 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 表示パターン指示														*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void disp_ptn_order( void )
{
	s_unit.disp.ptn = DISP_PTN_OFF;
	
	
	// 優先度順にパターン設定
	if( SYSTEM_MODE_SELF_CHECK == s_unit.system_mode ){
		s_unit.disp.ptn = s_unit.disp.ptn_g1d_order;
	}else if( ON == s_unit.bat_chg ){
		s_unit.disp.ptn = DISP_PTN_CHG_ON;
	}else if( ON == s_unit.disp.ptn_inital_order ){
		s_unit.disp.ptn = DISP_PTN_INITAL_RESET;
	}else if( ON == s_unit.disp.ptn_sw_long_order ){
		s_unit.disp.ptn = DISP_PTN_PW_SW_LONG;
	}else if( SYSTEM_MODE_IDLE_REST == s_unit.system_mode ){
		if( DENCH_ZANRYO_STS_MAX == s_unit.dench_sts ){
			s_unit.disp.ptn = DISP_PTN_DENCH_ZANRYO_1;
		}else if( DENCH_ZANRYO_STS_HIGH == s_unit.dench_sts ){
			s_unit.disp.ptn = DISP_PTN_DENCH_ZANRYO_2;
		}else if( DENCH_ZANRYO_STS_LOW == s_unit.dench_sts ){
			s_unit.disp.ptn = DISP_PTN_DENCH_ZANRYO_3;
		}else{
			// 処理なし
		}
	}else if( SYSTEM_MODE_GET == s_unit.system_mode ){
		s_unit.disp.ptn = DISP_PTN_BLE_ON;
	}else if( SYSTEM_MODE_IDLE_COM == s_unit.system_mode ){
		if( ON == s_unit.g1d.info.bit.ble ){
			s_unit.disp.ptn = DISP_PTN_BLE_ON;
		}else{
			s_unit.disp.ptn = DISP_PTN_BLE_OFF;
		}
	}else{
		//表示不要
	}
}


/************************************************************************/
/* 関数     : set_req_main_cyc											*/
/* 関数名   : メインサイクルタイミング設定								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.24 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* メインサイクルタイミング設定											*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void set_req_main_cyc(void)
{
	s_unit.main_cyc_req = ON;
}



/************************************************************************/
/* 関数     : err_info													*/
/* 関数名   : 異常通知													*/
/* 引数     : 異常ID(10進2桁)											*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : 異常をログ出力(デバッグ機能)									*/
/************************************************************************/
/* 注意事項 :															*/
/* ①デバッグ機能です。エラー出力出来ない可能性があります。				*/
/************************************************************************/
void err_info( ERR_ID id )
{
#if FUNC_DEBUG_LOG == ON
	UB tx[COM_STR_MAX] = {0};

	tx[0] = 'H';
	tx[1] = '1';
	tx[2] = 'E';
	tx[3] = 'R';
	tx[4] = 'R';
	tx[5] = (id / 10 ) + 0x30;
	tx[6] = (id % 10 ) + 0x30;
	tx[7] = '\r';
	tx[8] = '\n';
	
	// PC側にスルー出力
	if( s_unit.last_err_id != id ){
		com_srv_send( tx, 9 );
	}


#if 0
	while(1){
		// 異常による永久ループ
	}
#endif
#endif
	s_unit.err_cnt++;
	s_unit.last_err_id = id;
}

//================================
//ACL関連
//================================

/************************************************************************/
/* 関数     : i2c_write_sub												*/
/* 関数名   : I2C書き込みサブ											*/
/* 引数     : device_adrs	デバイスアドレス							*/
/*          : wr_data		書き込みデータ								*/
/*          : len			データ長									*/
/*          : wait_flg		書き込み後の待ちフラグ[未使用]				*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : I2C書き込みサブ												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg )
{
	UW lock_cnt = 0;

	if( 0 != R_IICA0_Master_Send( device_adrs, wr_data, len, I2C_WAIT)){
		err_info(ERR_ID_I2C);
	}else{
		while(1){
			if( OFF == s_unit.acl_info.i2c_snd_flg ){
				// 送信完了
				break;
			}
			if( lock_cnt++ >= I2C_LOCK_ERR_VAL ){
				// 異常時
				err_info(ERR_ID_I2C);
				s_unit.acl_info.i2c_err_flg = ON;
				break;
			}
		}
		if( OFF == s_unit.acl_info.i2c_err_flg ){
			R_IICA0_StopCondition();		//　異常発生時は予期せぬ書き込みを防止する為にストップコンディション発行しない
		}
	}
	while(0U == SPD0){}

	if( ON == s_unit.acl_info.i2c_err_flg ){
		s_unit.acl_info.i2c_err_flg = OFF;
		R_IICA0_Create();		// I2C初期化
	}
}


/************************************************************************/
/* 関数     : i2c_read_sub												*/
/* 関数名   : I2C読み出しサブ											*/
/* 引数     : device_adrs	デバイスアドレス							*/
/*          : read_adrs		読み出しアドレス							*/
/*          : read_data		読み出しデータ								*/
/*          : len			データ長									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : I2C読み出しサブ												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len )
{
	UB adrs[2];
	UB adrs_size = 0;
	UW lock_cnt = 0;

	// 加速度センサー時
	adrs[0] = (UB)( read_adrs & 0x00ff );			//アドレス
	adrs_size  = 1;

	if( 0 != R_IICA0_Master_Send( device_adrs, &adrs[0], adrs_size, I2C_WAIT )){
		err_info(ERR_ID_I2C);
	}else{
		while(1){
			if( OFF == s_unit.acl_info.i2c_snd_flg ){
				// 受信完了
				break;
			}
			if( lock_cnt++ >= I2C_LOCK_ERR_VAL ){
				// 異常時
				err_info(ERR_ID_I2C);
				s_unit.acl_info.i2c_err_flg = ON;
				break;
			}
		}
// RD8001暫定：どの様(バス開放orストップコンディション)に対応するか問合せ中
#if 0
	//ストップコンディションあり
		R_IICA0_StopCondition();
	}
	/* ストップコンディション後の待ち */
	while(0U == SPD0){}
//	WAIT_5US();
//	WAIT_5US();
//	wait_ms(5);
#else
	}
	//バス開放
	R_IICA0_Stop();
	R_IICA0_Create();
	
#endif

	if( 0 != R_IICA0_Master_Receive(device_adrs, read_data, len, I2C_WAIT)){
		err_info(ERR_ID_I2C);
	}else{
		while(1){
			if( OFF == s_unit.acl_info.i2c_rcv_flg ){
				break;
			}
			if( lock_cnt++ >= I2C_LOCK_ERR_VAL ){
				// 異常時
				err_info(ERR_ID_I2C);
				s_unit.acl_info.i2c_err_flg = ON;
				break;
			}
		}
		if( OFF == s_unit.acl_info.i2c_err_flg ){
			R_IICA0_StopCondition();		//　異常発生時は予期せぬ書き込みを防止する為にストップコンディション発行しない
		}
	}
	while(0U == SPD0){}
	
	if( ON == s_unit.acl_info.i2c_err_flg ){
		s_unit.acl_info.i2c_err_flg = OFF;
		R_IICA0_Create();		// I2C初期化
	}
}

/************************************************************************/
/* 関数     : i2c_set_snd_flg											*/
/* 関数名   : 送信フラグ設定											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 送信フラグ設定												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void i2c_set_snd_flg( UB data )
{
	s_unit.acl_info.i2c_snd_flg = data;
}

/************************************************************************/
/* 関数     : i2c_set_snd_flg											*/
/* 関数名   : 受信フラグ設定											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 受信フラグ設定												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void i2c_set_rcv_flg( UB data )
{
	s_unit.acl_info.i2c_rcv_flg = data;
}

/************************************************************************/
/* 関数     : i2c_set_err_flg											*/
/* 関数名   : エラーフラグ設定											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : エラーフラグ設定												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void i2c_set_err_flg( UB data )
{
	s_unit.acl_info.i2c_err_flg = data;
}

/************************************************************************/
/* 関数     : main_acl_init												*/
/* 関数名   : 加速度センサ初期化										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.13 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 加速度センサ初期化 ※初期状態は停止にしておく					*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_acl_init(void)
{
	UB rd_data[2];
	
	wait_ms( 30 );		// 加速度センサ　※電源ON待ち

	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_WHO_AM_I, &rd_data[0], 1 );
	if( rd_data[0] != ACL_REG_RECOGNITION_CODE ){
		err_info( ERR_ID_ACL );
	}
	
	main_acl_stop();
}

/************************************************************************/
/* 関数     : main_acl_stop												*/
/* 関数名   : 加速度センサ停止											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 加速度センサ停止												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_acl_stop(void)
{
	UB rd_data[2];
	UB wr_data[2];
	
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	// 動作モード設定
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_CTRL_REG1, &rd_data[0], 1 );
	if( rd_data[0] != 0x00 ){
		err_info( ERR_ID_ACL );
	}
}

/************************************************************************/
/* 関数     : main_acl_stop												*/
/* 関数名   : 加速度センサスタート										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 加速度センサスタート											*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_acl_start(void)
{
	UB wr_data[2];
	
	// 動作モード初期化
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	// 動作モード設定
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x20;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );

	// 動作モード開始
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0xA0;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	
}

/************************************************************************/
/* 関数     : main_acl_stop												*/
/* 関数名   : 加速度センサ読出し										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 加速度センサス読出し											*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_acl_read(void)
{
	UB rd_data[10];
	
	// INT_SOURCE1		
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_INT_SRC1, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & BIT04 )){
		// データ未達
		err_info( ERR_ID_ACL );
		return;
	}
	
	// データ取得
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_DATA_XYZ, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
	// INT_REL読み出し　※割り込み要求クリア
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_INT_REL, &rd_data[0], 1 );
}


/************************************************************************/
/* 関数     : stop_in													*/
/* 関数名   : CPUストップモード											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.26 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* CPUストップモード													*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void stop_in( void )
{
	//RD8001暫定：突入復帰処理に関して要検討
#if FUNC_LOG_STOP == ON
	com_srv_puts( (const B *__near)"STOP IN\r\n" );
	wait_ms( 10 );
#endif
	
	// 割り込み止める処理
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		// 10msタイマー停止
		R_IT8Bit0_Channel0_Stop();
	}else{
		// 50msタイマー停止
		R_IT_Stop();
	}
	
	STOP();
	
	// STOPモード解除時間(クロック供給停止+ウェイト)　※CPUマニュアル　Page 1066 of 1325
	// クロック供給停止：18 μs ～65 μs
	// ウエイト：ベクタ割り込み処理を行う場合(7クロック)
	//             ベクタ割り込み処理を行わない場合(1クロック)
	Nop5();Nop1();Nop1();
	
	stop_restore_ram();		//RAM復旧
	
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		// 10msタイマー開始
		R_IT8Bit0_Channel0_Create();
	}else{
		// 50msタイマー開始
		R_IT_Create();
	}
//	R_SAU0_Create();

#if FUNC_LOG_STOP == ON
	com_srv_puts( (const B *__near)"STOP OUT\r\n" );
#endif
}

/************************************************************************/
/* 関数     : stop_restore_ram											*/
/* 関数名   : CPUストップからのRAM復旧処理								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.26 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* CPUストップからの復旧処理											*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void stop_restore_ram( void )
{
	// 処理が必要であれば随時追加
	
//	if( SYSTEM_MODE_INITIAL == s_unit.system_mode ){
//		s_unit.pow_sw_last = OFF;	//電源ボタンの押下をOFFに変更
//	}
	
}


// =============================================================
// CPU通信関連
// =============================================================
/************************************************************************/
/* 関数     : main_cpu_com_snd_sensor_data								*/
/* 関数名   : センサーデータ送信										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : センサーデータ送信											*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_snd_sensor_data( void )
{

	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_SENSOR_DATA;
	memcpy( &s_ds.cpu_com.order.snd_data[0], &s_unit.meas.info.byte[0], CPU_COM_SND_DATA_SIZE_SENSOR_DATA );
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_SENSOR_DATA;
	
	cpu_com_send_start();
}

/************************************************************************/
/* 関数     : main_cpu_com_proc											*/
/* 関数名   : CPU間通信周期処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 周期処理														*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_cpu_com_proc(void)
{
	int i = 0;

	/* 受信データチェック */
	if( 0x00 != s_ds.cpu_com.input.rcv_cmd ){
		//何かデータ受信  旧アライブ
//		s_unit.cpu_com_success_once = ON;
//		time_get_elapsed_time( &s_unit.cpu_com_rcv_int_elapsed_time );
		/* 受信コマンド有り */
		for( i=CPU_COM_CMD_NONE; i<CPU_COM_CMD_MAX; i++){
			if( s_cpu_com_rcv_func_tbl[i].cmd == s_ds.cpu_com.input.rcv_cmd ){
				/* 受信コマンドとコマンドテーブルが一致 */
				if( NULL != s_cpu_com_rcv_func_tbl[i].func ){
					/* 受信処理有り */
					s_cpu_com_rcv_func_tbl[i].func();
				}
			}
		}
		// 受信コマンドクリア
		s_ds.cpu_com.input.rcv_cmd = 0x00;
	}
	
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_sts										*/
/* 関数名   : ステータス要求受信										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : ステータス要求受信											*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_sts( void )
{
	rtc_counter_value_t rtc_val;
	rtc_counter_value_t rtc_val_bin;
	H1D_INFO h1d;
	h1d.info.byte = 0;
	
	// 受信
	s_unit.g1d.info.byte = s_ds.cpu_com.input.rcv_data[1];
	
	// 送信
	if( MD_OK != R_RTC_Get_CounterValue( &rtc_val ) ){
		err_info( ERR_ID_MAIN );
	}
	
	// BCD→バイナリ変換
	bcd2bin(&rtc_val_bin.year, &rtc_val.year);
	bcd2bin(&rtc_val_bin.month, &rtc_val.month);
	bcd2bin(&rtc_val_bin.week, &rtc_val.week);
	bcd2bin(&rtc_val_bin.day, &rtc_val.day);
	bcd2bin(&rtc_val_bin.hour, &rtc_val.hour);
	bcd2bin(&rtc_val_bin.min, &rtc_val.min);
	bcd2bin(&rtc_val_bin.sec, &rtc_val.sec);
	
	// h1D情報作成
	if( ON == s_unit.bat_chg ){
		h1d.info.bit.bat_chg = 1;	//充電検知ポート
	}
	if( ON == s_unit.kensa ){
		h1d.info.bit.kensa = 1;		//検査ポート
	}

	
	if(( BAT_CHG_FIN_STATE_ON == s_unit.bat_chg_fin_state ) && ( EVENT_NON == s_unit.system_evt )){
		s_unit.system_evt = EVENT_CHG_FIN;	//充電完了イベント
		s_unit.bat_chg_fin_state = EVENT_CHG_FIN;
	}
	
	if( SYSTEM_MODE_NON != s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_evt;
		s_ds.cpu_com.order.snd_data[1] = s_unit.system_mode;
		s_ds.cpu_com.order.snd_data[2] = h1d.info.byte;
		s_ds.cpu_com.order.snd_data[3] = rtc_val_bin.year;
		s_ds.cpu_com.order.snd_data[4] = rtc_val_bin.month;
		s_ds.cpu_com.order.snd_data[5] = rtc_val_bin.week;
		s_ds.cpu_com.order.snd_data[6] = rtc_val_bin.day;
		s_ds.cpu_com.order.snd_data[7] = rtc_val_bin.hour;
		s_ds.cpu_com.order.snd_data[8] = rtc_val_bin.min;
		s_ds.cpu_com.order.snd_data[9] = rtc_val_bin.sec;
		s_ds.cpu_com.order.snd_data[10] = s_unit.dench_sts;

		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_evt = EVENT_NON;
	}else{
		// 何もしない
		
	}
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_sensing_order							*/
/* 関数名   : センシング指示											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : センシング指示												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_sensing_order( void )
{
	s_unit.sensing_sekigai_flg = s_ds.cpu_com.input.rcv_data[0];
}


/************************************************************************/
/* 関数     : main_cpu_com_rcv_date_set									*/
/* 関数名   : 日時設定受信												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 日時設定受信													*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_date_set( void )
{
	rtc_counter_value_t rtc_val;
	
	rtc_val.year = s_ds.cpu_com.input.rcv_data[0];
	rtc_val.month = s_ds.cpu_com.input.rcv_data[1];
	rtc_val.week = s_ds.cpu_com.input.rcv_data[2];
	rtc_val.day = s_ds.cpu_com.input.rcv_data[3];
	rtc_val.hour = s_ds.cpu_com.input.rcv_data[4];
	rtc_val.min = s_ds.cpu_com.input.rcv_data[5];
	rtc_val.sec = s_ds.cpu_com.input.rcv_data[6];
	
	// バイナリ→BCD変換
	rtc_val.year  = bin2bcd(rtc_val.year);
	rtc_val.month = bin2bcd(rtc_val.month);
	rtc_val.week  = bin2bcd(rtc_val.week);
	rtc_val.day   = bin2bcd(rtc_val.day);
	rtc_val.hour  = bin2bcd(rtc_val.hour);
	rtc_val.min   = bin2bcd(rtc_val.min);
	rtc_val.sec   = bin2bcd(rtc_val.sec);
	
	
	if( MD_OK != R_RTC_Set_CounterValue( rtc_val ) ){
		err_info( ERR_ID_MAIN );
	}
	
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DATE_SET;
	s_ds.cpu_com.order.snd_data[0] = 0;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DATE;
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_pc_log									*/
/* 関数名   : PCログ受信												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : PCログ受信													*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_pc_log( void )
{
#if FUNC_DEBUG_LOG == ON
	int len;
	UB tx_data[COM_STR_MAX] = {0};

	memcpy( &tx_data[0], &s_ds.cpu_com.input.rcv_data[0], CPU_COM_RCV_DATA_SIZE_PC_LOG );
	
	len = strlen( (const char *)tx_data );		// singed char(B)でも警告が出るのでcharでキャスト
	tx_data[len]		= '\r';
	tx_data[len + 1]	= '\n';
	
	// PC側にスルー出力
	com_srv_send( tx_data, len + 2 );
#endif
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_pc_log									*/
/* 関数名   : モード変更受信											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : モード変更受信												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_mode_chg( void )
{
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_MODE_CHG;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_MODE_CHG;
	
	// 状態変更
	s_unit.system_mode = (SYSTEM_MODE)s_ds.cpu_com.input.rcv_data[0];
	// 正常応答
	s_ds.cpu_com.order.snd_data[0] = 0;		// 正常応答
	// RD8001対応：状態管理はG1Dで行っているので、現在異常ケースは無し
	
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_pc_log									*/
/* 関数名   : プログラム転送開始準備受信								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : プログラム転送開始準備受信									*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_prg_hd_ready(void)
{
	// 識別コード消去
	download_change_boot();

	// RD8001対応：ブートへの切替処理(WDTによる自己リセット)
	s_unit.non_wdt_refresh = ON;
	
	// 応答
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_READY;
	s_ds.cpu_com.order.snd_data[0] = 0;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_PRG_READY;
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_pc_log									*/
/* 関数名   : プログラム転送確認受信									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : プログラム転送確認受信										*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_prg_hd_check(void)
{
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_CHECK;
	s_ds.cpu_com.order.snd_data[0] = version_product_tbl[0];
	s_ds.cpu_com.order.snd_data[1] = version_product_tbl[1];
	s_ds.cpu_com.order.snd_data[2] = version_product_tbl[2];
	s_ds.cpu_com.order.snd_data[3] = version_product_tbl[3];
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_PRG_CHECK;
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_disp										*/
/* 関数名   : 表示指示													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 表示指示														*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_disp(void)
{
	UB result = OK;
	
	switch( s_ds.cpu_com.input.rcv_data[0] ){
		case CPU_COM_DISP_ORDER_SELF_CHECK_NON:
			s_unit.disp.ptn_g1d_order = DISP_PTN_OFF;
			break;
		case CPU_COM_DISP_ORDER_SELF_CHECK_ERR:
			s_unit.disp.ptn_g1d_order = DISP_PTN_SELF_CHECK_FLASH_100MS;
			break;
		case CPU_COM_DISP_ORDER_SELF_CHECK_FIN:
			s_unit.disp.ptn_g1d_order = DISP_PTN_SELF_CHECK_ON;
			break;
		case CPU_COM_DISP_TRG_LONG_SW_RECEPTION:
				s_unit.disp.ptn_sw_long_order = ON;		// 長押し表示指示
				// 電源SW押下タイマー再スタート 
				time_soft_set_10ms( TIME_TYPE_10MS_SW_LONG_DISP, TIME_10MS_CNT_POW_SW_LONG_DISP );
			break;
		default:
			// ありえない
			result = NG;
		break;
	}
	
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DISP_ORDER;
	s_ds.cpu_com.order.snd_data[0] = result;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DISP_OREDER;
	
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_version									*/
/* 関数名   : バージョン												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : バージョン													*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_version(void)
{
	UB	boot_ver[4];				/* Boot部バージョン情報 */
	
	// アプリが保持しているアドレスからブートバージョンを生成
	memcpy((UB *)boot_ver, (UB *)version_boot_tbl, sizeof(boot_ver));

	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_VERSION;
	s_ds.cpu_com.order.snd_data[0] = version_product_tbl[0];	// アプリバージョン
	s_ds.cpu_com.order.snd_data[1] = version_product_tbl[1];
	s_ds.cpu_com.order.snd_data[2] = version_product_tbl[2];
	s_ds.cpu_com.order.snd_data[3] = version_product_tbl[3];
	s_ds.cpu_com.order.snd_data[4] = boot_ver[0];				// ブートバージョン
	s_ds.cpu_com.order.snd_data[5] = boot_ver[1];
	s_ds.cpu_com.order.snd_data[6] = boot_ver[2];
	s_ds.cpu_com.order.snd_data[7] = boot_ver[3];
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_VERSION;
}

/************************************************************************/
/* 関数     : ds_get_cpu_com_order										*/
/* 関数名   : CPU間通信用データ取得										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.03.02 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* CPU間通信用データ取得												*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data )
{
	*p_data = &s_ds.cpu_com.order;
}
/************************************************************************/
/* 関数     : ds_get_cpu_com_rcv_cmd									*/
/* 関数名   : CPU間通信データセット										*/
/* 引数     : CPU間通信データ格納ポインタ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.03.02 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* CPU間通信ミドルデータセット取得										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data )
{
	s_ds.cpu_com.input = *p_data;
}


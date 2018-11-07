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
void user_main(void);
static void user_main_mode_sensor(void);
static void main_init(void);
static void sw_cyc(void);
static void mode_cyc(void);
void set_req_main_cyc(void);
void main_send_uart1_rtc(void);
void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
void getmode_in(void);
void wait_ms( int ms );
void err_info( int id );
void main_acl_start(void);
void main_acl_stop(void);
void main_acl_read(void);
void main_acl_write(void);
STATIC W main_ad24_to_w( UH bufferH, UH bufferL );
STATIC void stop_in( void );
STATIC void stop_restore_ram( void );STATIC void main_cpu_com_snd_sensor_data( void );
STATIC void main_cpu_com_proc(void);
STATIC void main_cpu_com_rcv_sts( void );
STATIC void main_cpu_com_rcv_sensing_order( void );
STATIC void main_cpu_com_rcv_date_set( void );
STATIC void main_cpu_com_rcv_pc_log( void );
STATIC void main_cpu_com_rcv_mode_chg( void );
void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );
STATIC void main_cyc(void);
STATIC void disp_cyc(void);
STATIC void disp_ptn_order( void );
STATIC void main_cpu_com_rcv_prg_hd_ready(void);
STATIC void main_cpu_com_rcv_prg_hd_check(void);
STATIC void main_cpu_com_rcv_disp(void);
STATIC void main_cpu_com_rcv_version(void);

/************************************************************/
/* 変数定義													*/
/************************************************************/

UH bufferH_dbg_ave;
UH bufferL_dbg_ave;
UH bufferH_dbg;
UH bufferL_dbg;
UH tx_num;
UB wait;

int i2c_cmplete;

UB dbg_set_clock = 0;



STATIC T_UNIT s_unit;
STATIC DS s_ds;

/************************************************************/
/* テーブル定義												*/
/************************************************************/
// バージョン(APL)
const B		version_product_tbl[]= {0, 0, 0, 20};				/* ソフトウェアバージョン */
																/* バージョン表記ルール */
																/* ①メジャーバージョン：[0 ～ 9] */
																/* ②マイナーバージョン：[0 ～ 9]  */
																/* ③リビジョン：[0 ～ 9] */
																/* ④ビルドバージョン：[0 ～ 9] */

// バージョンのアドレス(BOOT)  //RD8001暫定
const UB	* const version_boot_tbl= (const UB*)0x00006EF0;	/* ブートバージョンアドレス */

/* 受信データ処理 関数テーブル */
STATIC const CPU_COM_RCV_CMD_TBL s_cpu_com_rcv_func_tbl[CPU_COM_CMD_MAX] = {
	/* コマンド */		/* 関数  */					/* 応答有無 */
	{	0x00,			NULL,							OFF	},	/* 【CPU間通信コマンド】コマンド無し				*/
	{	0xE0,			main_cpu_com_rcv_sts,			OFF	},	/* 【CPU間通信コマンド】ステータス要求				*/
	{	0xA0,			NULL,							OFF	},	/* 【CPU間通信コマンド】センサーデータ更新			*/
	{	0xA1,			main_cpu_com_rcv_sensing_order,	OFF	},	/* 【CPU間通信コマンド】センサーデータ更新			*/
	{	0xB0,			main_cpu_com_rcv_mode_chg,		OFF	},	/* 【CPU間通信コマンド】状態変更(G1D)				*/
	{	0xF0,			main_cpu_com_rcv_pc_log,		OFF	},	/* 【CPU間通信コマンド】PCログ送信(内部コマンド)	*/
	{	0xB1,			main_cpu_com_rcv_date_set,		OFF	},	/* 【CPU間通信コマンド】日時設定					*/
	{	0xD5,			main_cpu_com_rcv_prg_hd_ready,	OFF	},	/* 【CPU間通信コマンド】プログラム転送準備			*/
	{	0xD2,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送開始			*/
	{	0xD4,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送消去			*/
	{	0xD0,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送データ		*/
	{	0xD1,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送結果			*/
	{	0xD3,			main_cpu_com_rcv_prg_hd_check,	OFF	},	/* 【CPU間通信コマンド】プログラム転送確認			*/
	{	0xB2,			main_cpu_com_rcv_disp,			OFF	},	/* 【CPU間通信コマンド】表示指示					*/
	{	0xB3,			main_cpu_com_rcv_version,		OFF	},	/* 【CPU間通信コマンド】バージョン					*/
};

extern void test_uart_0_send( void );
extern void test_cpu_com_send( void );

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
	UW sleep_cnt = 0;
//    int cnt_20ms = 0;
//    int cnt_30sec = 0;
	rtc_counter_value_t rtc;
//	char com_data[10];
	UW time = 0;
    
	vect_set_tbl_apl();	

    main_init();
    /* Start user code. Do not edit comment generated here */
	
	// RTC設定
#if 1
	if( dbg_set_clock == 7 ){
//		R_RTC_Set_CounterValue( rtc );
	}
#endif
	R_RTC_Get_CounterValue( &rtc );
	s_unit.hour = rtc.hour;
	s_unit.min = rtc.min;
	s_unit.sec = rtc.sec;
//	R_RTC_Start();
	
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
		
		if( SYSTEM_MODE_INITIAL == s_unit.system_mode ){
			/* 何もしない */
			// スリープ突入条件検討　※関数化も含めて
			if(( OFF == drv_intp_read_g1d_int() ) &&
			   ( OFF == s_unit.pow_sw_last )  &&
			   ( OFF == drv_uart0_get_send() ) &&
			   ( DISP_PTN_OFF == s_unit.disp.ptn )){
				// RD8001暫定：通信の受信対応時間を待つ10MSではNG(リトライが発生する)
				time_soft_get_10ms( TIME_TYPE_10MS_WAIT_SLEEP, &time );
				if( 0 == time ){
					stop_in();
				}
			}else{
				time_soft_set_10ms( TIME_TYPE_10MS_WAIT_SLEEP, TIME_10MS_CNT_WAIT_SLEEP );
			}
		}else if( SYSTEM_MODE_GET == s_unit.system_mode ){
			// ゲットモード
			
//			get_mode();
			
			
			NOP();
		
		}else if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
			user_main_mode_sensor();
		}else{
			// ありえない
			
		}
		if( OFF == s_unit.non_wdt_refresh ){
			wdt_refresh();		//WDTリフレッシュ
		}
    }
    /* End user code. Do not edit comment generated here */
}

//#define IBIKI_COUNT			//RD8001暫定：いびきカウントアップ
#ifdef	IBIKI_COUNT
UH dbg_ibiki_cnt = 0;
#endif
#if FUNC_DEBUG_RAW_VAL_OUT == ON
UB dbg_send_cnt;
char dbg_tx_data[50] = {0};
int dbg_len;
#endif

static void user_main_mode_sensor(void)
{
	UW time;
	
	
//	MEAS meas;
	
#ifdef	IBIKI_COUNT
	dbg_ibiki_cnt = 0;
#endif
	
	if( s_unit.sensing_cnt_50ms++ >= HOUR12_CNT_50MS ){
		s_unit.sensing_end_flg = ON;
	}
	
	if( ON == s_unit.sensing_end_flg ){
		return;
	}
	
	time_soft_get_10ms( TIME_TYPE_10MS_SENSING_DELAY, &time );
	if( 0 != time ){
		return;
	}
	
	//RD8001暫定：センサーが動作しない
	// センサーモード
	// メイン周期(50ms)
	if( ON == s_unit.main_cyc_req ){
		R_DAC1_Start();			// ■DAC ON　※共通用途

		// ↓↓マイク用(呼吸、イビキ)開始↓↓
		R_DAC1_Set_ConversionValue( 0x0000 );
		R_AMP0_Start();		// ■AMP0 ON
		R_PGA1_Start();		// ■PGA1 ON

		wait_ms( 2 );
		adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
		
		// RD8001暫定：いびきデータをデバッグ用カウント
#ifdef	IBIKI_COUNT
		s_unit.meas.info.dat.ibiki_val = dbg_ibiki_cnt++;
#endif
		wait_ms( 2 );
#if 0	// RD8001暫定：AMP0,PGA1をOFFすると正常にイビキ等が取得できない
		R_AMP0_Stop();		// □AMP0 OFF
		R_PGA1_Stop();		// □PGA1 OFF
#endif
		// ↑↑マイク用(呼吸、イビキ)終了↑↑

		R_PGA_DSAD_Start();		// ■PGA0_DSAD  ON
		R_AMP2_Start();			// ■AMP2       ON
		
//RD8001暫定：初期化を行うと動作しない
#if FUNC_VALID_AMP == ON
		R_DAC1_Set_ConversionValue( 0x0500 );
		// 赤色光ON
		drv_o_port_sekigai( OFF );
		drv_o_port_sekishoku( ON );
		wait_ms( 2 );
		

		pga_do();
//		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);		//平均
		R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
		s_unit.meas.info.dat.sekishoku_val = main_ad24_to_w( bufferH_dbg, bufferL_dbg );
		wait_ms( 2 );

		// 赤外光ON
		if( ON == s_unit.sensing_sekigai_flg ){
			drv_o_port_sekigai( ON );
			drv_o_port_sekishoku( OFF );

			R_DAC1_Set_ConversionValue( 0x0B00 );
			wait_ms( 2 );
			pga_do();
	//		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);		//平均
			R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
			s_unit.meas.info.dat.sekigaival = main_ad24_to_w( bufferH_dbg, bufferL_dbg );
			
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
		
		// 加速度センサ取得
		// RD8001暫定：30msかかるが50ms毎に取得
#if FUNC_DEBUG_RAW_VAL_OUT == OFF
		if( ++s_unit.acl_sensor_timing >= ACL_TIMING_VAL ){
			s_unit.acl_sensor_timing = 0;
			main_acl_read();
		}
		
#else
		//生値デバッグ出力　※加速度は無効
		if( dbg_send_cnt++ >= 0 ){	
			dbg_send_cnt = 0;
//			printf( "kokyu%d", s_unit.meas.info.dat.kokyu_val );		//コンソールデバッグ用　※現状コンソールの使い方が不明
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
		// RD8001暫定：stopモード無効
#if 1
		stop_in();
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
static void main_init(void)
{
	wdt_refresh();		//WDTリフレッシュ

	//データ初期化
	memset( &s_unit, 0, sizeof(s_unit) );
	/* Start user code. Do not edit comment generated here */
	
	//データ初期化(0以外)
	s_unit.disp.last_ptn = 0xFF;
	
	
	
	// ミドル初期化
	com_srv_init();
	cpu_com_init();
	time_init();
	
	EI();
}

/************************************************************************/
/* 関数     : sw_cyc													*/
/* 関数名   : SW周期処理												*/
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
static void sw_cyc(void)
{
	UB pow_sw;
	UW time = 0;
	
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
				s_unit.disp.ptn_sw_long_order = ON;		// 長押し表示指示
				// 電源SW押下タイマー再スタート 
				time_soft_set_10ms( TIME_TYPE_10MS_SW_LONG_DISP, TIME_10MS_CNT_POW_SW_LONG_DISP );
				if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
					s_unit.sensing_end_flg = ON;
				}
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
static void mode_cyc(void)
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
	UB bat_chg_off_trig = OFF;
	UW time = 0;
	
	
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
		// RD8001暫定：LEDを光らせる為のダック設定の最適値とは？
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


// システムモードなどの情報で表示パターンを決定
STATIC void disp_ptn_order( void )
{
	s_unit.disp.ptn = DISP_PTN_OFF;
	
	
	// 優先度順にパタン設定
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


void set_req_main_cyc(void)
{
	s_unit.main_cyc_req = ON;
}

//================================
//UART関連
//================================

#define UART1_STR_MAX		150
// debug
void main_send_uart1_rtc(void)
{
	int len;
	UB tx_data[UART1_STR_MAX] = {0};
	rtc_counter_value_t rtc;
	
	R_RTC_Get_CounterValue( &rtc );
	s_unit.hour = rtc.hour;
	s_unit.min = rtc.min;
	s_unit.sec = rtc.sec;
	
	len = sprintf((char*)tx_data, "CLOCK %x,%x,%x\r\n", s_unit.hour, s_unit.min, s_unit.sec );

	com_srv_send( tx_data, len );

//	wait_ms(5);			//RD8001暫定：すぐに書き込むと送信が乱れる

}

//================================
//EEP関連
//================================
#define			EEP_WAIT		255		//RD8001暫定値

void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg )
{
	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Send( device_adrs, wr_data, len, EEP_WAIT)){
		err_info(2);
	}else{
		while(1){
			if( 1 == i2c_cmplete ){
				break;
			}
		}
		R_IICA0_StopCondition();
	}
	while(0U == SPD0)

	/* ストップコンディション後の待ち */
	if( wait_flg == ON ){
		wait_ms(5);
	}
}

void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len )
{
	UB adrs[2];
	UB adrs_size = 0;
	UW lcok_cnt = 0;

#if 1		// ストップビットなし
	
	if( EEP_DEVICE_ADR == device_adrs ){
		// EEP時
		adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//アドレス上位
		adrs[1] = (UB)( read_adrs & 0x00ff );			//アドレス下位
		adrs_size  = 2;
	}else if( ACL_DEVICE_ADR == device_adrs ){
		// 加速度センサー時
		adrs[0] = (UB)( read_adrs & 0x00ff );			//アドレス
		adrs_size  = 1;
	}

	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Send( device_adrs, &adrs[0], adrs_size, EEP_WAIT )){
		err_info(3);
	}else{
#if 1
		while(1){
			if(( 1 == i2c_cmplete ) || ( lcok_cnt++ >= 10000 )){		//RD8001暫定
//			if( 1 == i2c_cmplete ){		//RD8001暫定
				break;
			}
		}
//		R_IICA0_StopCondition();
#endif
	}
	/* ストップコンディション後の待ち */
//	WAIT_5US();		//RD8001暫定
//	WAIT_5US();		//RD8001暫定
//	wait_ms(5);
	R_IICA0_Stop();
	R_IICA0_Create();
#else
	// ストップビットあり
	//RD8001暫定：ストップコンディションを送らないと読み出し失敗する
	adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//アドレス上位
	adrs[1] = (UB)( read_adrs & 0x00ff );		//アドレス下位
		
	i2c_write_sub(device_adrs, &adrs[0], 2 , OFF );
#endif
	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Receive(device_adrs, read_data, len, EEP_WAIT)){
		err_info(4);
	}else{
		while(1){
			if( 1 == i2c_cmplete ){
				break;
			}
		}
		R_IICA0_StopCondition();
	}
	while(0U == SPD0)
	/* ストップコンディション後の待ち */
	wait_ms(5);
	WAIT_5US();		//RD8001暫定
}

void getmode_in(void)
{
	s_unit.system_mode = SYSTEM_MODE_GET;
}

void wait_ms( int ms )
{
	int i,j;
	
	for(i = 0; i < ms; i++ ){
		for(j = 0; j < 100; j++ ){
			WAIT_10US()
		}
	}
}



//void err_info( ERR_ID id )
void err_info( int id )
{
	s_unit.err_cnt++;
#if 0
	while(1){
		// 異常による永久ループ
	}
#endif
}

void main_acl_stop(void)
{
	UB rd_data[2];
	UB wr_data[2];
	
	i2c_read_sub( ACL_DEVICE_ADR, 0x0F, &rd_data[0], 1 );
	if( rd_data[0] != 0x35 ){
		err_info( 5 );
	}
	
	wr_data[0] = 0x1B;
	wr_data[1] = 0x00;
	// 動作モード設定
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	i2c_read_sub( ACL_DEVICE_ADR, 0x1B, &rd_data[0], 1 );
	if( rd_data[0] != 0x00 ){
		err_info( 7 );
	}
}

void main_acl_start(void)
{
	UB rd_data[2];
	UB wr_data[2];
	
	i2c_read_sub( ACL_DEVICE_ADR, 0x0F, &rd_data[0], 1 );
	if( rd_data[0] != 0x35 ){
		err_info( 5 );
	}
	
	wr_data[0] = 0x1B;
	wr_data[1] = 0x00;
	// 動作モード設定
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	wr_data[0] = 0x1B;
	wr_data[1] = 0x20;
	// 動作モード設定
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );

	wr_data[0] = 0x1F;
	wr_data[1] = 0xBF;
	// 動作モード設定
//	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	
	
	wr_data[0] = 0x1B;
	wr_data[1] = 0xA0;
	// 動作モード設定
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	
}


void main_acl_read(void)
{
	UB rd_data[10];
	
	// INT_SOURCE1		
	i2c_read_sub( ACL_DEVICE_ADR, 0x16, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & 0x10 )){
		// データ未達
		return;
	}
	
	// データ取得
	i2c_read_sub( ACL_DEVICE_ADR, 0x06, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
	// INT_REL読み出し　※割り込み要求クリア
	i2c_read_sub( ACL_DEVICE_ADR, 0x1A, &rd_data[0], 1 );
}

void main_acl_write(void)
{
//	main_acl_read_sub( ACL_DEVICE_ADR, 0x0C, &rd_data[0], 1 );
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
#if 0
	com_srv_puts( (const B *__near)"STOP IN\r\n" );
	wait_ms( 10 );
#endif
	
	// 割り込み止める処理
	if( SYSTEM_MODE_INITIAL == s_unit.system_mode ){
		// 10msタイマー停止
		TMKAMK = 1U;    	  /* disable INTIT interrupt */
		TMKAIF = 0U;  	    /* clear INTIT interrupt flag */
	}
	
	STOP();
	
	// STOPモード解除時間(クロック供給停止+ウェイト)　※CPUマニュアル　Page 1066 of 1325
	// クロック供給停止：18 μs ～65 μs
	// ウエイト：ベクタ割り込み処理を行う場合(7クロック)
	//             ベクタ割り込み処理を行わない場合(1クロック)
	Nop5();Nop1();Nop1();
	
	stop_restore_ram();		//RAM復旧
	
	if( SYSTEM_MODE_INITIAL == s_unit.system_mode ){
		// 10msタイマー開始
		TMKAMK = 0U;    	/* enable INTIT interrupt */
		TMKAIF = 0U;  	    /* clear INTIT interrupt flag */
	}

#if 0
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
	
	if( SYSTEM_MODE_INITIAL == s_unit.system_mode ){
		s_unit.pow_sw_last = OFF;	//電源ボタンの押下をOFFに変更
	}
	
}


// CPU間通信サービス
// =============================================================
// CPU通信関連
// =============================================================
STATIC void main_cpu_com_snd_sensor_data( void )
{
	drv_o_port_g1d_int( ON );

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
/* 変更履歴 : 2014.06.05  Axia Soft Design 宮本		初版作成			*/
/*          : 2014.06.27  Axia Soft Design 吉居							*/
/*          :						状態に応じて送信トリガを切り換える	*/
/* 			: 2016.05.19  Axia Soft Design 西島　リトライアウト時の処理追加(CPU間通信異常対応) */
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
//					s_cpu_com_rcv_func_tbl[i].func(&s_ds.cpu_com.input.rcv_data[0]);
					s_cpu_com_rcv_func_tbl[i].func();
				}
			}
		}
		// 受信コマンドクリア ※暫定
		s_ds.cpu_com.input.rcv_cmd = 0x00;
	}
	
}

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
		err_info( 13 );
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

	
	// RD8001暫定：イベントはバッファリングできるべき
	if(( BAT_CHG_FIN_STATE_ON == s_unit.bat_chg_fin_state ) && ( EVENT_NON == s_unit.system_evt )){
		s_unit.system_evt = EVENT_CHG_FIN;	//充電完了イベント
		s_unit.bat_chg_fin_state = EVENT_CHG_FIN;			//RD8001暫定：１回通知
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

STATIC void main_cpu_com_rcv_sensing_order( void )
{
	s_unit.sensing_sekigai_flg = s_ds.cpu_com.input.rcv_data[0];
}



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
		err_info( 12 );
	}
	
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DATE_SET;
	s_ds.cpu_com.order.snd_data[0] = 0;
	s_ds.cpu_com.order.data_size = 1;
}


STATIC void main_cpu_com_rcv_pc_log( void )
{
#if 0
	//何の処理？
	UB	state;			/* レコード有効/無効				*/
	UH	ibiki_val;
	UB	myaku_val;
	UB	spo2_val;
	UB	kubi;
	UB	dummy;
	int len;
	UB tx_data[UART1_STR_MAX] = {0};
	
	state = s_ds.cpu_com.input.rcv_data[0];			/* レコード有効/無効				*/
	ibiki_val = (( s_ds.cpu_com.input.rcv_data[2] << 8 )  | s_ds.cpu_com.input.rcv_data[1] );
	myaku_val = s_ds.cpu_com.input.rcv_data[3];
	spo2_val = s_ds.cpu_com.input.rcv_data[4];
	kubi = s_ds.cpu_com.input.rcv_data[5];
	dummy = s_ds.cpu_com.input.rcv_data[6];
	
	len = sprintf((char*)tx_data, "%d,%u,%d,%d,%d,%d\r\n"    , state
															  , ibiki_val
															  , myaku_val
															  , spo2_val
															  , kubi
															  , dummy);

	com_srv_send( tx_data, len );
#else
	int len;
	UB tx_data[UART1_STR_MAX] = {0};

	memcpy( &tx_data[0], &s_ds.cpu_com.input.rcv_data[0], CPU_COM_RCV_DATA_SIZE_PC_LOG );
	
	len = strlen( tx_data );
	tx_data[len]		= '\r';
	tx_data[len + 1]	= '\n';
	
	// PC側にスルー出力
	com_srv_send( tx_data, len + 2 );
#endif
}

STATIC void main_cpu_com_rcv_mode_chg( void )
{
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_MODE_CHG;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_MODE_CHG;
	
	// RD8001暫定：異常ケースは？
	if( 1 ){
		// 状態変更
		s_unit.system_mode = s_ds.cpu_com.input.rcv_data[0];
		// 正常応答
		s_ds.cpu_com.order.snd_data[0] = 0;		// 正常応答
	}else{
		s_ds.cpu_com.order.snd_data[0] = 1;		// 異常応答
	}
	
	
}

/* 【CPU間通信コマンド】プログラム転送開始準備		*/
STATIC void main_cpu_com_rcv_prg_hd_ready(void)
{
	// 識別コード消去
	download_change_boot();

	// RD8001暫定：ブートへの切替処理(WDTによる事故リセット)
	s_unit.non_wdt_refresh = ON;
	
	// 応答
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_READY;
	s_ds.cpu_com.order.snd_data[0] = 0;
	s_ds.cpu_com.order.data_size = 1;
}

/* 【CPU間通信コマンド】プログラム転送確認		*/
STATIC void main_cpu_com_rcv_prg_hd_check(void)
{
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_CHECK;
	s_ds.cpu_com.order.snd_data[0] = version_product_tbl[0];
	s_ds.cpu_com.order.snd_data[1] = version_product_tbl[1];
	s_ds.cpu_com.order.snd_data[2] = version_product_tbl[2];
	s_ds.cpu_com.order.snd_data[3] = version_product_tbl[3];
	s_ds.cpu_com.order.data_size = 4;
}

/* 表示指示 */
STATIC void main_cpu_com_rcv_disp(void)
{
	switch( s_ds.cpu_com.input.rcv_data[0] ){
		case CPU_COM_DISP_ORDER_NON:
			s_unit.disp.ptn_g1d_order = DISP_PTN_OFF;
			break;
		case CPU_COM_DISP_ORDER_SELF_CHECK_ERR:
			s_unit.disp.ptn_g1d_order = DISP_PTN_SELF_CHECK_FLASH_100MS;
			break;
		case CPU_COM_DISP_ORDER_SELF_CHECK_FIN:
			s_unit.disp.ptn_g1d_order = DISP_PTN_SELF_CHECK_ON;
			break;
		default:
			// ありえない
		break;
	}
}

/* バージョン */
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
/* 変更履歴 : 2014.05.15 Axia Soft Design 吉居 久和	初版作成			*/
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
/* 変更履歴 : 2014.06.05 Axia Soft Design 宮本 和幹	初版作成			*/
/*          : 2014.06.12 Axia Soft Design 吉居							*/
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




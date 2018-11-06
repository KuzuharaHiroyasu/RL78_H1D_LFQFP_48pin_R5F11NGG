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
void main_send_uart1(void);
void main_send_uart1_rtc(void);
void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
void getmode_in(void);
void wait_ms( int ms );
void err_info( int id );
void main_acl_init(void);
void main_acl_read(void);
void main_acl_write(void);
STATIC W main_ad24_to_w( UH bufferH, UH bufferL );
static void stop_in( void );
static void halt_in( void );
STATIC void main_cpu_com_snd_sensor_data( void );
STATIC void main_cpu_com_proc(void);
STATIC void main_cpu_com_rcv_sts( void );
STATIC void main_cpu_com_rcv_date_set( void );
STATIC void main_cpu_com_rcv_pc_log( void );
STATIC void main_cpu_com_rcv_mode_chg( void );
void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );
STATIC void main_cyc(void);
STATIC void main_cpu_com_rcv_prg_hd_ready(void);
STATIC void main_cpu_com_rcv_prg_hd_check(void);

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
const B		version_product_tbl[]= {0, 0, 0, 5};				/* ソフトウェアバージョン */
																/* バージョン表記ルール */
																/* ①メジャーバージョン：[0 ～ 9] */
																/* ②マイナーバージョン：[0 ～ 9]  */
																/* ③リビジョン：[0 ～ 9] */
																/* ④ビルドバージョン：[0 ～ 9] */

/* 受信データ処理 関数テーブル */
STATIC const CPU_COM_RCV_CMD_TBL s_cpu_com_rcv_func_tbl[CPU_COM_CMD_MAX] = {
	/* コマンド */		/* 関数  */					/* 応答有無 */
	{	0x00,			NULL,							OFF	},	/* 【CPU間通信コマンド】コマンド無し				*/
	{	0xE0,			main_cpu_com_rcv_sts,			OFF	},	/* 【CPU間通信コマンド】ステータス要求				*/
	{	0xA0,			NULL,							OFF	},	/* 【CPU間通信コマンド】センサーデータ更新			*/
	{	0xB0,			main_cpu_com_rcv_mode_chg,		OFF	},	/* 【CPU間通信コマンド】状態変更(G1D)				*/
	{	0xF0,			main_cpu_com_rcv_pc_log,		OFF	},	/* 【CPU間通信コマンド】PCログ送信(内部コマンド)	*/
	{	0xB1,			main_cpu_com_rcv_date_set,		OFF	},	/* 【CPU間通信コマンド】日時設定					*/
	{	0xD5,			main_cpu_com_rcv_prg_hd_ready,	OFF	},	/* 【CPU間通信コマンド】プログラム転送準備			*/
	{	0xD2,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送開始			*/
	{	0xD4,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送消去			*/
	{	0xD0,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送データ		*/
	{	0xD1,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送結果			*/
	{	0xD3,			main_cpu_com_rcv_prg_hd_check,							OFF	},	/* 【CPU間通信コマンド】プログラム転送確認			*/
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

	main_acl_init();
	
	while (1U)
	{
		com_srv_cyc();
		sw_cyc();
		mode_cyc();
		cpu_com_proc();
		main_cpu_com_proc();
		main_cyc();
		
		if( SYSTEM_MODE_NON == s_unit.system_mode ){
			drv_o_port_mike( OFF );		//RD8001暫定
			/* 何もしない */
			// RD8001暫定：スリープ突入時間
			if( sleep_cnt++ > 5000 ){
				sleep_cnt = 0;
#if 0			//RTCデバッグ
				R_RTC_Get_CounterValue( &dbg_rtc );
				
				if( 1 == dbg_rtc_flg ){
					dbg_rtc_flg = 0;
					dbg_rtc_set.year = 9;
					dbg_rtc_set.sec = 0;
					dbg_rtc_set.min = 0;
					dbg_rtc_set.hour = 5;
					dbg_rtc_set.day = 0;
					dbg_rtc_set.week = 0;
					dbg_rtc_set.month = 0;
//					R_RTC_Stop();
					R_RTC_Set_CounterValue( dbg_rtc_set );
//					R_RTC_Start();
				}
#endif
//				drv_uart1_send_start();
//				test_cpu_com_send();		//ミドルレベルCPU間通信
//				test_uart_0_send();			//ドライバレベルCPU間通信

				//stop_in();
			}
		}else if( SYSTEM_MODE_GET_MODE == s_unit.system_mode ){
			drv_o_port_mike( OFF );		//RD8001暫定
			// ゲットモード
			
//			get_mode();
			
			
			NOP();
		
		}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
			user_main_mode_sensor();
		}else{
			drv_o_port_mike( OFF );		//RD8001暫定
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
		R_DAC1_Start();			// ■DAC        ON
		R_PGA_DSAD_Start();		// ■PGA0_DSAD  ON
		R_AMP2_Start();			// ■AMP2       ON
		
//RD8001暫定：初期化を行うと動作しない
#if FUNC_VALID_AMP == ON
		R_DAC1_Set_ConversionValue( 0x0B00 );
		wait_ms( 2 );
		
		// 赤色光ON
		drv_o_port_sekigai( OFF );
		drv_o_port_sekishoku( ON );

		pga_do();
//		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);		//平均
		R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
		s_unit.meas.info.dat.sekishoku_val = main_ad24_to_w( bufferH_dbg, bufferL_dbg );
		wait_ms( 2 );

		// 赤外光ON
		drv_o_port_sekigai( ON );
		drv_o_port_sekishoku( OFF );

		R_DAC1_Set_ConversionValue( 0x0500 );
		wait_ms( 2 );
		pga_do();
//		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);		//平均
		R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
		s_unit.meas.info.dat.sekigaival = main_ad24_to_w( bufferH_dbg, bufferL_dbg );
		
		R_PGA_DSAD_Stop();		// □PGA0_DSAD  OFF
		R_AMP2_Stop();		// □AMP2  OFF
		
		wait_ms( 2 );
		
		// マイク用(呼吸、イビキ)開始
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
		R_DAC1_Stop();		// □DAC  OFF

#if 0	// RD8001暫定：AMP0,PGA1をOFFすると正常にイビキ等が取得できない
		R_AMP0_Stop();		// □AMP0 OFF
		R_PGA1_Stop();		// □PGA1 OFF
#endif
		
		// 加速度センサ取得
		// RD8001暫定：30msかかるが50ms毎に取得
#if FUNC_DEBUG_RAW_VAL_OUT == OFF
		main_acl_read();
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

		main_cpu_com_snd_sensor_data();
		
		s_unit.main_cyc_req  = OFF;
		
		// 赤外、赤色OFF
		drv_o_port_sekigai( OFF );
		drv_o_port_sekishoku( OFF );

		// RD8001暫定：stopモード無効
#if 1
		stop_in();
#endif
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
	//データ初期化
	memset( &s_unit, 0, sizeof(s_unit) );
	/* Start user code. Do not edit comment generated here */
	
	
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
	
	if( ON == pow_sw ){
		// なにもしない(電源SW押下タイマー継続)
	}else{
		if( ON == s_unit.pow_sw_last ){
			// ON→OFFエッジ
			time_soft_get_10ms( TIME_TYPE_10MS_POW_SW_ACT_START, &time );
			if( 0 == time ){
				// 規定時間以上連続押下と判断
				s_unit.sensing_start_trig = ON;
			}
		}
		// 電源SW押下タイマー再スタート 
		time_soft_set_10ms( TIME_TYPE_10MS_POW_SW_ACT_START, TIME_10MS_CNT_POW_SW_ACT_START );
	}
	s_unit.pow_sw_last = pow_sw;
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
	//RD8001暫定：状態変更箇所が一箇所
	if( ON == s_unit.sensing_start_trig ){
		s_unit.sensing_start_trig = OFF;
		if( SYSTEM_MODE_NON == s_unit.system_mode ){
			s_unit.main_cyc_req = OFF;
			s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_SENSING;
			
			
			
		}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
			s_unit.sensing_end_flg = ON;
			s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_IDLE;
//			s_unit.system_mode = SYSTEM_MODE_NORMAL;
//		}else if( SYSTEM_MODE_NORMAL == s_unit.system_mode ){
//			s_unit.system_mode = SYSTEM_MODE_NON;
		}else{
			/* 何もしない */
		}
	}
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
	
	
	dench_val = DENCH_ZANRYO_1_VAL;
	
	adc_dench( &dench_val );
	
	if( dench_val >= DENCH_ZANRYO_1_VAL ){
		s_unit.dench_sts = 1;
	}else if( dench_val >= DENCH_ZANRYO_2_VAL ){
		s_unit.dench_sts = 2;
	}else if( dench_val >= DENCH_ZANRYO_3_VAL ){
		s_unit.dench_sts = 3;
	}else{
		s_unit.dench_sts = 4;
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
void main_send_uart1(void)
{
#if 0
	uint8_t tx_data[UART1_STR_MAX] = {0};
	int len;
	
#if 0
	s_unit.sekishoku_val = 1;
	s_unit.sekigaival = 2;
	s_unit.kokyu_val = 3;
	s_unit.ibiki_val = 4;
#endif

#if 0		//EEP無効
#if 1
	len = sprintf((char*)tx_data, "%ld,%ld,%d,%d,%d,%d,%d\r\n", s_unit.eep.record.data.sekishoku_val
															  , s_unit.eep.record.data.sekigaival
															  , s_unit.eep.record.data.kokyu_val
															  , s_unit.eep.record.data.ibiki_val
															  , s_unit.eep.record.data.acl_x
															  , s_unit.eep.record.data.acl_y
															  , s_unit.eep.record.data.acl_z );




#else
	len = sprintf((char*)tx_data, "%d,%f,%f,%f,%f,%f,%d,%d,%d\r\n", s_unit.eep.record.data.state_flg,					/* フラグ(呼吸状態,睡眠ステージ,いびき有無 */
															 s_unit.eep.record.data.ibiki_val,					/* いびき */
															 s_unit.eep.record.data.myaku_val_sekishoku,		/* 脈拍_赤外 */
															 s_unit.eep.record.data.myaku_val_sekigai,			/* 脈拍_赤色 */
															 s_unit.eep.record.data.spo2_val_normal,			/* SPO2_通常 */
															 s_unit.eep.record.data.spo2_val_konica,			/* SPO2 */
															 s_unit.eep.record.data.acl_x,
															 s_unit.eep.record.data.acl_y,
															 s_unit.eep.record.data.acl_z );
	
//	main_acl_read();
//	len = sprintf(tx_data, "%d,%d,%d\r\n", s_unit.acl_x, s_unit.acl_y, s_unit.acl_z );		//加速度
#endif
#endif
	
	
	com_srv_send( &tx_data[0], len );


#if 0
	len = sprintf((char*)tx_data, "%d,%d,%d\r\n", s_unit.hour,
												  s_unit.min,
												  s_unit.sec );
	com_srv_send( &tx_data[0], len );
#endif




//	com_srv_send( s, len );
#endif
}

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
	s_unit.system_mode = SYSTEM_MODE_GET_MODE;
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

void main_acl_init(void)
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



static void stop_in( void )
{
	//RD8001暫定：突入復帰処理に関して要検討
#if 0
	com_srv_puts( (const B *__near)"STOP IN\r\n" );
	wait_ms( 10 );
#endif
	
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	
	// 割り込み止める処理
//	TMKAMK = 1U;    	  /* disable INTIT interrupt */
//	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	STOP();
	
	// STOPモード解除時間(クロック供給停止+ウェイト)　※CPUマニュアル　Page 1066 of 1325
	// クロック供給停止：18 μs ～65 μs
	// ウエイト：ベクタ割り込み処理を行う場合(7クロック)
	//             ベクタ割り込み処理を行わない場合(1クロック)
	wait_ms( 1 );
//	TMKAMK = 0U;    	  /* disable INTIT interrupt */
//	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
#if 0
	com_srv_puts( (const B *__near)"STOP OUT\r\n" );
#endif
}

static void halt_in( void )
{
	com_srv_puts( (const B *__near)"HALT IN\r\n" );
	wait_ms( 10 );
	//RD8001暫定：突入復帰処理に関して要検討
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	
	// 割り込み止める処理
	TMKAMK = 1U;    	  /* disable INTIT interrupt */
	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	HALT();
	// ウェイト　※CPUマニュアル　Page 1060 of 1325
	// ベクタ割り込み処理を行う場合
	// メイン・システム・クロック時 ：15～16クロック
	// ベクタ割り込み処理を行わない場合
	// メイン・システム・クロック時 ：9～10クロック
	wait_ms( 1 );
	TMKAMK = 0U;    	  /* disable INTIT interrupt */
	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	com_srv_puts( (const B *__near)"HALT OUT\r\n" );
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
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
	
	// 以降スリープ処理の為に送信終了までの処理を呼ぶ
	cpu_com_send_end();
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
	if( MD_OK != R_RTC_Get_CounterValue( &rtc_val ) ){
		err_info( 13 );
	}
	

	if( SYSTEM_MODE_NON == s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode_chg_req;
		s_ds.cpu_com.order.snd_data[1] = s_unit.system_mode;
		s_ds.cpu_com.order.snd_data[2] = s_unit.info_data;
		s_ds.cpu_com.order.snd_data[3] = rtc_val.year;
		s_ds.cpu_com.order.snd_data[4] = rtc_val.month;
		s_ds.cpu_com.order.snd_data[5] = rtc_val.week;
		s_ds.cpu_com.order.snd_data[6] = rtc_val.day;
		s_ds.cpu_com.order.snd_data[7] = rtc_val.hour;
		s_ds.cpu_com.order.snd_data[8] = rtc_val.min;
		s_ds.cpu_com.order.snd_data[9] = rtc_val.sec;

		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_NON;
	}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode_chg_req;
		s_ds.cpu_com.order.snd_data[1] = s_unit.system_mode;
		s_ds.cpu_com.order.snd_data[2] = s_unit.info_data;
		s_ds.cpu_com.order.snd_data[3] = rtc_val.year;
		s_ds.cpu_com.order.snd_data[4] = rtc_val.month;
		s_ds.cpu_com.order.snd_data[5] = rtc_val.week;
		s_ds.cpu_com.order.snd_data[6] = rtc_val.day;
		s_ds.cpu_com.order.snd_data[7] = rtc_val.hour;
		s_ds.cpu_com.order.snd_data[8] = rtc_val.min;
		s_ds.cpu_com.order.snd_data[9] = rtc_val.sec;
		
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_NON;
	}else{
		// 何もしない
		
	}
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

	if( MD_OK != R_RTC_Set_CounterValue( rtc_val ) ){
		err_info( 12 );
	}
	
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DATE_SET;
	s_ds.cpu_com.order.snd_data[0] = 0;
	s_ds.cpu_com.order.data_size = 1;
}


STATIC void main_cpu_com_rcv_pc_log( void )
{
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
}

STATIC void main_cpu_com_rcv_mode_chg( void )
{
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_MODE_CHG;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_MODE_CHG;
	
	// センシング開始時のタイマー
	if(( SYSTEM_MODE_SENSOR != s_unit.system_mode ) &&
	   ( SYSTEM_MODE_SENSOR == s_ds.cpu_com.input.rcv_data[0] )){
		time_soft_set_10ms( TIME_TYPE_10MS_SENSING_DELAY, TIME_10MS_CNT_SENSING_DELAY );
		s_unit.sensing_end_flg = OFF;
		s_unit.sensing_cnt_50ms = 0;
		drv_o_port_mike( ON );		//RD8001暫定
	}
	
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




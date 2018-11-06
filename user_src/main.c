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
void main_send_uart1(void);
MD_STATUS R_IICA0_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait);
MD_STATUS R_IICA0_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait);
void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
int main_eep_read(void);
int main_eep_write(void);
void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
void wait_ms( int ms );
void err_info( int id );
void get_mode( void );
void main_eep_erase_all( void );
void main_acl_init(void);
void main_acl_read(void);
void test_code_eep(void);
void main_send_uart1_rtc(void);
static void main_init(void);
static void sw_cyc(void);
static void mode_cyc(void);
static void stop_in( void );
static void halt_in( void );
STATIC void main_cpu_com_snd_sensor_data( void );
STATIC void main_cpu_com_proc(void);
STATIC void main_cpu_com_rcv_sts( void );
STATIC void main_cpu_com_rcv_pc_log( void );
STATIC void main_cpu_com_rcv_mode_chg( void );
void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );
static void user_main_mode_sensor(void);

/************************************************************/
/* 変数定義													*/
/************************************************************/
//RD8001暫定：削除予定
uint8_t eep_rx_data[I2C_RCV_SND_SIZE];        /* iica0 receive buffer */
uint8_t eep_tx_data[I2C_RCV_SND_SIZE];        /* iica0 send buffer */

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
	{	0xFF,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送開始		*/
	{	0xFF,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送データ要求 */
	{	0xFF,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送(受信)	*/
	{	0xFF,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送サム値要求 */
	{	0xFF,			NULL,							OFF	},	/* 【CPU間通信コマンド】プログラム転送結果要求	*/
	{	0xFF,			NULL,							OFF	},	/* 【CPU間通信コマンド】ファイル転送開始		*/
	{	0xFF,			NULL,							OFF	},	/* 【CPU間通信コマンド】ファイル転送			*/
	{	0xFF,			NULL,							OFF	},	/* 【CPU間通信コマンド】ブロック転送結果要求	*/
	{	0xFF,			NULL,							OFF	},	/* 【CPU間通信コマンド】ファイル転送結果要求	*/
};

extern void test_uart_0_send( void );
extern void test_cpu_com_send( void );




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
    int cnt_20ms = 0;
    int cnt_30sec = 0;
	rtc_counter_value_t rtc;
	char com_data[10];
    
    main_init();
    /* Start user code. Do not edit comment generated here */
	
	// RTC設定
#if 1
	if( dbg_set_clock == 7 ){
		R_RTC_Set_CounterValue( rtc );
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
	
	s_unit.eep_rd_record_cnt = 0;
	s_unit.eep_wr_record_cnt = 0;
#if 0
	// 有効レコードチェック探索  ※しない
	while(1){
		if( INVALID == main_eep_read() ){
			// 終了
			break;
		}
		if( 1 == s_unit.eep.record.data.valid ){
			// 有効レコード
			s_unit.eep_wr_record_cnt++;
		}else{
			// 終了
			break;
		}
	}
#endif
    
    while (1U)
    {
		com_srv_cyc();
		sw_cyc();
		mode_cyc();
		cpu_com_proc();
		main_cpu_com_proc();
		
		if( SYSTEM_MODE_NON == s_unit.system_mode ){
			/* 何もしない */
			// RD8001暫定：スリープ突入時間
			if( sleep_cnt++ > 5000 ){
				sleep_cnt = 0;
//				drv_uart1_send_start();
//				test_cpu_com_send();		//ミドルレベルCPU間通信
//				test_uart_0_send();			//ドライバレベルCPU間通信

				//stop_in();
			}
		}else if( SYSTEM_MODE_GET_MODE == s_unit.system_mode ){
			// ゲットモード
			
			get_mode();
			
			
			NOP();
		
		}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
			user_main_mode_sensor();
		}else{
			// ありえない
			
		}
    }
    /* End user code. Do not edit comment generated here */
}

static void user_main_mode_sensor(void)
{
	UW time;
	UW work_uw = 0;
	MEAS meas;
	
	
	if( ON == s_unit.sensing_end_flg ){
		// 赤外光
		P7 &= ~0x80;
		// 赤色光
		P7 &= ~0x40;
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

//RD8001暫定：初期化を行うと動作しない
#if FUNC_VALID_AMP == ON
		// 赤外光
		P7 &= ~0x80;
		P7 |= 0x40;
		VSBIAS = 0x09;		/* 0.5 */
		R_DAC_Change_OutputVoltage_12bit( 0x07BD );
		wait_ms( 1 );
		pga_do();
		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);
		R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
		work_uw = bufferH_dbg;
		work_uw <<= 16;
		work_uw += bufferL_dbg;
		
		s_unit.meas.info.dat.sekigaival = (( work_uw >> 16 ) & 0x0000FFFF );
//		s_unit.meas.info.dat.sekigaival = work_uw;
//		s_unit.meas.info.dat.sekigaival >>= 8;
		
		R_DAC_Change_OutputVoltage_12bit( 0x0000 );		//RD8001暫定：DAC変更後一定時間待たないと電圧が落ち切らない様に思う
		
		// 赤色光
		P7 &= ~0x40;
		P7 |= 0x80;

//			VSBIAS = 0x0E;		/* 1.0 *///oneA様指示設定　※反応が悪い
		VSBIAS = 0x09;		/* 0.5 */
//			R_DAC_Change_OutputVoltage_12bit( 0x014A );//oneA様指示設定　※反応が悪い 
		R_DAC_Change_OutputVoltage_12bit( 0x02BD ); 
		wait_ms( 1 );
		pga_do();
		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);
		R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
		// RD8001暫定：エンディアンがマニュアルを読んでも確証を持てない
		work_uw = bufferH_dbg;
		work_uw <<= 16;
		work_uw += bufferL_dbg;
		
		s_unit.meas.info.dat.sekishoku_val = (( work_uw >> 16 ) & 0x0000FFFF );
//		s_unit.meas.info.dat.sekishoku_val = work_uw;
//		s_unit.meas.info.dat.sekishoku_val >>= 8;

		// 加速度センサ取得
		main_acl_read();
		
		// マイク用(呼吸、イビキ)開始
		R_DAC_Change_OutputVoltage_12bit( 0x0000 );
		adc_do( &s_unit.dench_val, &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
#endif
#if 0
		//センサーデータ送信
		//200msに一度
		if( s_unit.sensing_cnt++ >= 4 ){
			main_cpu_com_snd_sensor_data();
			s_unit.sensing_cnt = 0;
		}
#else
		main_cpu_com_snd_sensor_data();
#endif
		s_unit.main_cyc_req  = OFF;
		R_DAC_Change_OutputVoltage_12bit( 0x02BD );		//DACを赤色用に変更
	}

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
	
	pow_sw = drv_read_pow_sw();
	
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
	uint8_t tx_data[UART1_STR_MAX] = {0};
//	uint8_t * const s = &tx_data[0];
	int len;
	
#if 0
	s_unit.sekishoku_val = 1;
	s_unit.sekigaival = 2;
	s_unit.kokyu_val = 3;
	s_unit.ibiki_val = 4;
#endif

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
	
	
	com_srv_send( &tx_data[0], len );


#if 0
	len = sprintf((char*)tx_data, "%d,%d,%d\r\n", s_unit.hour,
												  s_unit.min,
												  s_unit.sec );
	com_srv_send( &tx_data[0], len );
#endif




//	com_srv_send( s, len );
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

int main_eep_write(void)
{
	WR_EEP_RECORD		wr;
	UH wr_adrs;
	UB device_adrs;
	int ret = INVALID;
	UH record_offset = 0;

#if 0
	UB adrs[2];

	// ページ処理の断片
	UB page_bit;
	int eep_adrs;
	int eep_page;
	UB  device_adrs;
#endif
	
	// レコード最大
	if( s_unit.eep_wr_record_cnt < EEP_RECORD_1P_MAX ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( s_unit.eep_wr_record_cnt < EEP_RECORD_2P_MAX ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
		record_offset = EEP_RECORD_1P_MAX;
	}else{
		return ret;
	}

#if 0
	// ページ処理の断片
	eep_adrs = eep_wr_record_cnt * EEP_RECORD_SIZE;
	eep_page = eep_adrs / EEP_DATA_SIZE_PAGE;
	eep_adrs = eep_adrs % EEP_DATA_SIZE_PAGE;

	if( 0 == page ){
		device_adrs = EEP_DEVICE_ADR;
	}else{
		device_adrs = EEP_DEVICE_ADR | 0x02;
	}
#endif
	wr_adrs = ( s_unit.eep_wr_record_cnt - record_offset ) * EEP_RECORD_SIZE;
//	wr.record.data.wr_adrs				 = s_unit.eep_wr_record_cnt * EEP_RECORD_SIZE;
	wr.record.byte[0] = (UB)(( wr_adrs >> 8 ) & 0xff );		//アドレス上位
	wr.record.byte[1] = (UB)( wr_adrs & 0x00ff );		//アドレス下位
	wr.record.data.valid				 = ON;			/* レコード有効/無効				*/
	
	wr.record.data.kokyu_val			 = s_unit.eep.record.data.kokyu_val;		
	wr.record.data.ibiki_val			 = s_unit.eep.record.data.ibiki_val;		
	wr.record.data.sekishoku_val		 = s_unit.eep.record.data.sekishoku_val;	// 差動入力の為に符号あり
	wr.record.data.sekigaival			 = s_unit.eep.record.data.sekigaival;		// 差動入力の為に符号あり
	wr.record.data.acl_x				 = s_unit.eep.record.data.acl_x;
	wr.record.data.acl_y				 = s_unit.eep.record.data.acl_y;
	wr.record.data.acl_z				 = s_unit.eep.record.data.acl_z;
	
	
	//書き込み
	i2c_write_sub( device_adrs, &wr.record.byte[0], sizeof(wr), ON );	

//	eep_tx_data[0] = (UB)( wr_adrs & 0x00ff );		//アドレス下位
//	eep_tx_data[1] = (UB)(( wr_adrs >> 8 ) & 0xff );		//アドレス上位
//	eep_tx_data[14]  = 1;
//	i2c_write_sub( EEP_DEVICE_ADR, &eep_tx_data[0], sizeof(wr) );
	
	s_unit.eep_wr_record_cnt++;
	
	ret = VALID;
	
	return ret;
}

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

int main_eep_read(void)
{
	int ret = INVALID;
	UH read_adrs;
	EEP_RECORD		rd;
	UB device_adrs;
	UH record_offset = 0;
	
	// レコード最大
	if( s_unit.eep_rd_record_cnt < EEP_RECORD_1P_MAX ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( s_unit.eep_rd_record_cnt < EEP_RECORD_2P_MAX ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
		record_offset = EEP_RECORD_1P_MAX;
	}else{
		return ret;
	}
	
	read_adrs = ( s_unit.eep_rd_record_cnt - record_offset ) * EEP_RECORD_SIZE;
	i2c_read_sub( device_adrs, read_adrs, &s_unit.eep.record.byte[0], sizeof(rd) );
//	i2c_read_sub( EEP_DEVICE_ADR, read_adrs, &eep_rx_data[0], 32 );
	
	s_unit.eep_rd_record_cnt++;
	
	ret = VALID;
	
	return ret;
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

void get_mode( void )
{
	com_srv_puts( (const B *__near)"START\r\n" );

//	wait_ms(10);

	// ゲットモードループ
	s_unit.eep_rd_record_cnt = 0;
	while(1){
		if( INVALID == main_eep_read() ){
			// 終了
			break;
		}
		wait_ms(10);
		if( 1 == s_unit.eep.record.data.valid ){
			// 有効レコード
#if 0		// 暫定
			s_unit.kokyu_val	 = s_unit.eep.record.data.kokyu_val;		
			s_unit.ibiki_val	 = s_unit.eep.record.data.ibiki_val;		
			s_unit.sekishoku_val = s_unit.eep.record.data.sekishoku_val;	// 差動入力の為に符号あり
			s_unit.sekigaival	 = s_unit.eep.record.data.sekigaival;		// 差動入力の為に符号あり
#endif
			
//			wait_ms(5);
			main_send_uart1();
//			wait_ms(5);
		}else{
			// 終了
			break;
		}
	}
	main_eep_erase_all();

	s_unit.eep_rd_record_cnt = 0;
	s_unit.eep_wr_record_cnt  =0;
	s_unit.system_mode = SYSTEM_MODE_NON;
	
	com_srv_puts( (const B *__near)"END\r\n" );
}


void main_eep_erase_all(void)
{
	WR_EEP_RECORD wr;
	UB device_adrs =0;
	UH record_offset = 0;
	UH wr_adrs = 0;
	
	s_unit.eep_wr_record_cnt = 0;
	
	while(1){
		// レコード最大
		if( s_unit.eep_wr_record_cnt < EEP_RECORD_1P_MAX ){
			device_adrs = EEP_DEVICE_ADR;
		}else if( s_unit.eep_wr_record_cnt < EEP_RECORD_2P_MAX ){
			device_adrs = EEP_DEVICE_ADR | 0x02;
			record_offset = EEP_RECORD_1P_MAX;
		}else{
			return;
		}
		
		// 使用分のみで終了
#if 0
		if( s_unit.eep_rd_record_cnt < s_unit.eep_wr_record_cnt ){
			return;
		}
#endif
		
		wr_adrs = ( s_unit.eep_wr_record_cnt - record_offset ) * EEP_RECORD_SIZE;
		wr.record.byte[0] = (UB)(( wr_adrs >> 8 ) & 0xff );		//アドレス上位
		wr.record.byte[1] = (UB)( wr_adrs & 0x00ff );		//アドレス下位
		wr.record.data.valid				 = OFF;			/* レコード有効/無効				*/
		
#if 0		// 暫定
		wr.record.data.kokyu_val			 = 0;
		wr.record.data.ibiki_val			 = 0;
		wr.record.data.sekishoku_val		 = 0;
		wr.record.data.sekigaival			 = 0;
#endif
			
		//書き込み
		i2c_write_sub( device_adrs, &wr.record.byte[0], sizeof(wr), ON );	
		wait_ms(5);
	
		s_unit.eep_wr_record_cnt++;
	}
	
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
	
	
	i2c_read_sub( ACL_DEVICE_ADR, 0x1A, &rd_data[0], 1 );
	
	i2c_read_sub( ACL_DEVICE_ADR, 0x16, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & 0x10 )){
		return;
	}

	i2c_read_sub( ACL_DEVICE_ADR, 0x06, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
}

void main_acl_write(void)
{
//	main_acl_read_sub( ACL_DEVICE_ADR, 0x0C, &rd_data[0], 1 );
}

void test_code_eep(void)
{
#if 0
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
	eep_tx_data[2] = 11;
	eep_tx_data[3] = 22;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 );
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
	eep_tx_data[2] = 33;
	eep_tx_data[3] = 44;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
	eep_tx_data[2] = 55;
	eep_tx_data[3] = 66;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
	eep_tx_data[2] = 77;
	eep_tx_data[3] = 88;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 );
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 0, &eep_rx_data[0], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 5, &eep_rx_data[2], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
///	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 8, &eep_rx_data[4], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 13, &eep_rx_data[6], 2 );

	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	

#endif

#if 0
	//デバッグコード
	s_unit.eep_wr_record_cnt = 0;
	
	s_unit.kokyu_val  = 1111;		
	s_unit.ibiki_val  = 2222;		
	s_unit.sekishoku_val = 333333;	// 差動入力の為に符号あり
	s_unit.sekigaival = -444444;		// 差動入力の為に符号あり
    main_eep_write();
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}
	s_unit.eep_wr_record_cnt = 4096;
	
	s_unit.kokyu_val  = 3333;		
	s_unit.ibiki_val  = 4444;		
	s_unit.sekishoku_val = 555555;	// 差動入力の為に符号あり
	s_unit.sekigaival = -666666;		// 差動入力の為に符号あり
    main_eep_write();
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}



	s_unit.eep_wr_record_cnt = 4095;
	
	s_unit.kokyu_val  = 2222;		
	s_unit.ibiki_val  = 3333;		
	s_unit.sekishoku_val = 444444;		// 差動入力の為に符号あり
	s_unit.sekigaival = -555555;		// 差動入力の為に符号あり
    main_eep_write();
    
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}
	s_unit.eep_rd_record_cnt = 0;
    main_eep_read();
	s_unit.eep_rd_record_cnt = 4096;
    main_eep_read();
	s_unit.eep_rd_record_cnt = 4095;
    main_eep_read();
#endif
	
#if 0
	s_unit.eep_rd_record_cnt = 0;
	s_unit.eep_wr_record_cnt = 0;
	s_unit.kokyu_val  = 1111;		
	s_unit.ibiki_val  = 2222;		
	s_unit.sekishoku_val = 333333;	// 差動入力の為に符号あり
	s_unit.sekigaival = -444444;		// 差動入力の為に符号あり
	while(1){
		if( INVALID == main_eep_write() ){
			// 終了
			break;
		}
		wait_ms(5);
//		for(i = 0; i < 500; i++ ){
//			WAIT_10US()
//		}
	}	
	
#endif	
	
	
}


static void stop_in( void )
{
	//RD8001暫定：突入復帰処理に関して要検討
	com_srv_puts( (const B *__near)"STOP IN\r\n" );
	wait_ms( 10 );
	
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	
	// 割り込み止める処理
	TMKAMK = 1U;    	  /* disable INTIT interrupt */
	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	STOP();
	
	// STOPモード解除時間(クロック供給停止+ウェイト)　※CPUマニュアル　Page 1066 of 1325
	// クロック供給停止：18 μs ～65 μs
	// ウエイト：ベクタ割り込み処理を行う場合(7クロック)
	//             ベクタ割り込み処理を行わない場合(1クロック)
	wait_ms( 1 );
	TMKAMK = 0U;    	  /* disable INTIT interrupt */
	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	com_srv_puts( (const B *__near)"STOP OUT\r\n" );
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
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_SENSOR_DATA;
	memcpy( &s_ds.cpu_com.order.snd_data[0], &s_unit.meas.info.byte[0], CPU_COM_SND_DATA_SIZE_SENSOR_DATA );
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_SENSOR_DATA;
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
					s_cpu_com_rcv_func_tbl[i].func(&s_ds.cpu_com.input.rcv_data[0]);
				}
			}
		}
		// 受信コマンドクリア ※暫定
		s_ds.cpu_com.input.rcv_cmd = 0x00;
	}
	
}

STATIC void main_cpu_com_rcv_sts( void )
{
	if( SYSTEM_MODE_NON == s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode_chg_req;
		s_ds.cpu_com.order.snd_data[1] = 0;
		s_ds.cpu_com.order.snd_data[2] = 0;
		s_ds.cpu_com.order.snd_data[3] = 0;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_NON;
	}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode_chg_req;
		s_ds.cpu_com.order.snd_data[1] = 0;
		s_ds.cpu_com.order.snd_data[2] = 0;
		s_ds.cpu_com.order.snd_data[3] = 0;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_NON;
	}else{
		// 何もしない
		
		
		
	}


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
		time_soft_set_10ms( TIME_TYPE_10MS_POW_SW_ACT_START, TIME_10MS_CNT_POW_SW_ACT_START );
		s_unit.sensing_end_flg = OFF;
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




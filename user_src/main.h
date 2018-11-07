/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : main.c														*/
/* 機能         : 全体管理(マクロ定義、型定義、関数の外部参照宣言)				*/
/* 変更履歴     : 2018.03.02 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef		_MAIN_H_			/* 二重定義防止 */
#define		_MAIN_H_


// システムモード
typedef enum{
	SYSTEM_MODE_INITIAL,					// イニシャル
	SYSTEM_MODE_IDLE_REST,					// アイドル_残量表示 ※RD8001暫定：IDLEを統合するか要検討
	SYSTEM_MODE_IDLE_COM,					// アイドル_通信待機 ※RD8001暫定：IDLEを統合するか要検討
	SYSTEM_MODE_SENSING,					// センシング
	SYSTEM_MODE_GET,						// データ取得
	SYSTEM_MODE_PRG_H1D,					// H1Dプログラム更新
	SYSTEM_MODE_PRG_G1D,					// G1Dプログラム更新
	SYSTEM_MODE_SELF_CHECK,					// 自己診断
	//↑↑↑機能仕様上の上限↑↑↑
	SYSTEM_MODE_MOVE,						// 移行
	SYSTEM_MODE_NON,						// なし
	SYSTEM_MODE_MAX
}SYSTEM_MODE;

// イベント ※H1D/G1D共通
typedef enum{
	EVENT_NON = 0,				// なし
	EVENT_POW_SW_SHORT,			// 電源SW押下(短)
	EVENT_POW_SW_LONG,			// 電源SW押下(長)
	EVENT_CHG_PORT_ON,			// 充電検知ポートON
	EVENT_CHG_PORT_OFF,			// 充電検知ポートOFF
	EVENT_DENCH_LOW,			// 電池残量低下
	EVENT_CHG_FIN,				// 充電完了
	EVENT_GET_DATA,				// データ取得
	EVENT_H1D_PRG,				// プログラム書き換え(H1D)
	EVENT_G1D_PRG,				// プログラム書き換え(G1D)
	EVENT_SELF_CHECK_COM,		// 自己診断(通信)
	EVENT_COMPLETE,				// 完了
	EVENT_STOP,					// 中断
	EVENT_TIME_OUT,				// タイムアウト
	EVENT_KENSA_ON,				// 検査ポートON
	EVENT_MAX,					// 最大
}EVENT_NUM;


// 電池残量閾値
#define DENCH_ZANRYO_1_VAL						(UH)( 1023.0 * (1.95 / 3.0 ))		// 1.95V以上
#define DENCH_ZANRYO_2_VAL						(UH)( 1023.0 * (1.72 / 3.0 ))		// 1.72V以上
#define DENCH_ZANRYO_3_VAL						(UH)( 1023.0 * (1.5 / 3.0 ))		// 1.5V以上

#define DENCH_BAT_CHG_FIN_VAL					(UH)( 1023.0 * (2.0 / 3.0 ))		// 2.0V以上


// 電池残量状態 ※H1D/G1D共通
#define DENCH_ZANRYO_STS_MAX					1	// 充電満タン
#define DENCH_ZANRYO_STS_HIGH					2	// 数日持つ
#define DENCH_ZANRYO_STS_LOW					3	// 残り１日持たない
#define DENCH_ZANRYO_STS_MIN					4	// 電池残量なし






#define			HOUR12_CNT_50MS		(UW)( 12L * 60L * 60L * (1000L / 50L))	//12時間のカウント値[50ms]
//#define			HOUR12_CNT_50MS			( 1L * 10L * 60L * (1000L / 50L))	//12時間のカウント値[50ms]		短縮版10分






// EEP
#define EEP_DEVICE_ADR			0xA0				// デバイスアドレス

// 加速度センサ
#define ACL_DEVICE_ADR			0x1C				// 加速度センサデバイスアドレス
#define ACL_TIMING_VAL			10					// 加速度センサ処理タイミング


typedef struct{
	union{
		UB	byte[CPU_COM_SND_DATA_SIZE_SENSOR_DATA];
		struct{
			W	sekigaival;		// 差動入力の為に符号あり
			W	sekishoku_val;	// 差動入力の為に符号あり
			UH	kokyu_val;		
			UH	ibiki_val;		
			B acl_x;
			B acl_y;
			B acl_z;
			B dummy;			// ダミー[パディング調整]
		}dat;
	}info;
}MEAS;

// 表示パターン
#define			DISP_PTN_OFF						0		// 消灯
#define			DISP_PTN_SELF_CHECK_ON				1		// 自己診断点灯 ※G1Dからの指示
#define			DISP_PTN_SELF_CHECK_FLASH_100MS		2		// 自己診断点滅(100ms) ※G1Dからの指示
#define			DISP_PTN_CHG_ON						3		// 充電中
#define			DISP_PTN_INITAL_RESET				4		// 初期化リセット
#define			DISP_PTN_PW_SW_LONG					5		// 電源ボタン長押し
#define			DISP_PTN_DENCH_ZANRYO_1				6		// 電池残量1
#define			DISP_PTN_DENCH_ZANRYO_2				7		// 電池残量2
#define			DISP_PTN_DENCH_ZANRYO_3				8		// 電池残量3

#define			DISP_PTN_BLE_ON						9		// 通信待機/データ取得 ※BLE接続済み
#define			DISP_PTN_BLE_OFF					10		// 通信待機/データ取得 ※BLE接続未接続



#define			DISP_PTN_SELF_CHECK_FLASH_100MS_SEQ_MAX		1				// シーケンス最大

#define			DISP_PTN_FLASH_DENCH_ZANRYO_3_SEQ_MAX		61		// シーケンス最大

#define			DISP_PTN_BLE_OFF_SEQ_MAX		5		// シーケンス最大
#define			DISP_PTN_BLE_ON_SEQ_MAX			1		// シーケンス最大

typedef struct{
	UB seq;					// シーケンス
	UB ptn;					// パターン
	UB last_ptn;			// 前回パターン
	
	UB ptn_g1d_order;					// パターン(G1Dからの指示)
	UB ptn_inital_order;				// パターン(初期化リセット指示)
	UB ptn_sw_long_order;				// パターン(電源長押し指示)
}DISP;

// 充電完了状態
#define BAT_CHG_FIN_STATE_OFF			0		// なし ※起動直後はOFF確定
#define BAT_CHG_FIN_STATE_ON			1		// あり
#define BAT_CHG_FIN_STATE_UNKNOWN		2		// 不明 ※一度充電検知ポートがONになると不明状態になる

typedef struct{
	union {
		UB	byte;
		/*呼出ランプ状態*/
		struct{
			UB	ble				:1;		/* 1  BLE    */
			UB	dummy1			:1;		/* 2  未定義 */
			UB	dummy2			:1;		/* 3  未定義 */
			UB	dummy3			:1;		/* 4  未定義 */
			UB	dummy4			:1;		/* 5  未定義 */
			UB	dummy5			:1;		/* 6  未定義 */
			UB	dummy6			:1;		/* 7  未定義 */
			UB	dummy7			:1;		/* 8  未定義 */
		}bit;
	}info;
}G1D_INFO;


typedef struct{
	union {
		UB	byte;
		/*呼出ランプ状態*/
		struct{
			UB	bat_chg			:1;		/* 1  充電検知ポート */
			UB	kensa			:1;		/* 2  検査ポート */
			UB	dummy1			:1;		/* 3  未定義 */
			UB	dummy2			:1;		/* 4  未定義 */
			UB	dummy3			:1;		/* 5  未定義 */
			UB	dummy4			:1;		/* 6  未定義 */
			UB	dummy5			:1;		/* 7  未定義 */
			UB	dummy6			:1;		/* 8  未定義 */
		}bit;
	}info;
}H1D_INFO;



typedef struct{
	UB main_cyc_req;		/* メイン周期要求(20ms) */

	SYSTEM_MODE system_mode;			/* システムモード */
	SYSTEM_MODE last_system_mode;		/* システムモード */
	EVENT_NUM system_evt;	/* システムモード変更要求 */
	
	MEAS meas;				/* 計測値(50ms) */
	UH dench_sts;			/* 電池残量状態 */
	UB bat_chg;				/* 充電検知ポート */
	UB bat_chg_fin_state;	/* 充電完了イベント */
	UB kensa;				/* 検査状態 */
	
	UB hour;
	UB min;
	UB sec;
	
	// イベント
//	UB event_sw_long;			// SW押下(長)
//	UB event_sw_short;			// SW押下(短)
	
	G1D_INFO g1d;				// G1D情報

	UB sensing_end_flg;			// センシング終了
	UB sensing_sekigai_flg;		// 赤外フラグ(ON:有効、OFF:無効)
	
	
	

	UW sensing_cnt_50ms;		// センシング終了[50ms]
	UB acl_sensor_timing;			// 加速度センサタイミング
	
	
	UB non_wdt_refresh;			// WDTリフレッシュなし
	
	DISP disp;					// 表示	
	
	// ワーク
	UB pow_sw_last;				// 電源ボタン状態(前回)
	UB bat_chg_last;			// 充電状態(前回)
	UB kensa_last;				/* 検査状態(前回) */
	
	UW err_cnt;			//異常回数(デバッグ用途)
}T_UNIT;


//RD8001暫定：DSをどうするか検討
/*##################################################################*/
/*							CPU間通信部								*/
/*##################################################################*/
typedef struct _DS_CPU_COM_INPUT{
	UB rcv_cmd;											/* 受信コマンド */
	UB rcv_data[CPU_COM_DATA_SIZE_MAX];					/* 受信データ */
	UB cpu_com_send_status;								/* CPU間通信送信ステータス */
														/* 	CPU_COM_SND_STATUS_IDLE			アイドル状態 ※送信可能状態	*/
														/* 	CPU_COM_SND_STATUS_SEND			送信中						*/
														/* 	CPU_COM_SND_STATUS_COMPLETE		送信完了					*/
														/* 	CPU_COM_SND_STATUS_SEND_NG		リトライNG					*/
}DS_CPU_COM_INPUT;

typedef struct _DS_CPU_COM_ORDER{
	CPU_COM_CMD_ID snd_cmd_id;							/* 送信コマンドID */
	UB snd_data[CPU_COM_DATA_SIZE_MAX];					/* 送信データ */
	UH data_size;										/* 送信データ長 */
}DS_CPU_COM_ORDER;

typedef struct _DS_CPU_COM{
	DS_CPU_COM_INPUT input;
	DS_CPU_COM_ORDER order;
}DS_CPU_COM;

typedef struct _CPU_COM_RCV_CMD_TBL{
	UB cmd;							/* 受信コマンド */
//	void (*func)(UB *p_data);		/* 受信処理 */
	void (*func)(void);		/* 受信処理 */
	UB res;							/* 応答有(ON)無(OFF) */
}CPU_COM_RCV_CMD_TBL;


/* DS構造体 */
typedef struct{
	DS_CPU_COM 		cpu_com;		/* CPU間通信部 */
}DS;


/************************************************************/
/* 外部参照宣言 ※無いと正常に動かない						*/
/************************************************************/
extern void R_IICA0_Create(void);
//extern MD_STATUS com_srv_send(uint8_t * const tx_buf, uint16_t tx_num);
//extern MD_STATUS R_UART1_Receive(uint8_t * const rx_buf, uint16_t rx_num);

extern void R_PGA_DSAD_Get_AverageResult(uint16_t * const bufferH,uint16_t * const bufferL);
extern void R_PGA_DSAD_Get_Result(uint16_t * const bufferH,uint16_t * const bufferL);
extern void R_DAC_Change_OutputVoltage_12bit(uint16_t outputVoltage);
extern unsigned short pga_do( void );
extern void R_IICA0_StopCondition(void);
extern void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
extern void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );




#endif
/************************************************************/
/* END OF TEXT												*/
/************************************************************/

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
#define			SYSTEM_MODE_NON						0
#define			SYSTEM_MODE_SENSOR					1
#define			SYSTEM_MODE_GET_MODE				2



// 電池残量閾値
#define DENCH_ZANRYO_1_VAL			(UH)( 1023.0 * (1.95 / 3.0 ))		// 1.95V以上
#define DENCH_ZANRYO_2_VAL			(UH)( 1023.0 * (1.72 / 3.0 ))		// 1.72V以上
#define DENCH_ZANRYO_3_VAL			(UH)( 1023.0 * (1.5 / 3.0 ))		// 1.5V以上


#define			HOUR12_CNT_50MS		(UW)( 12L * 60L * 60L * (1000L / 50L))	//12時間のカウント値[50ms]
//#define			HOUR12_CNT_50MS			( 1L * 10L * 60L * (1000L / 50L))	//12時間のカウント値[50ms]		短縮版10分






// EEP
#define EEP_DEVICE_ADR			0xA0				// デバイスアドレス

// 加速度センサ
#define ACL_DEVICE_ADR			0x1C				// 加速度センサデバイスアドレス

// 30秒[20ms]
#define SEC_30_CNT				( 30 * 1000 / 20 )
//#define SEC_30_CNT				( 30 * 1000 / 200 )	//10倍デバッグ

// 12時間[30秒]
#define HOUR_12_CNT				( 12 * 60 * 2 )
//#define HOUR_12_CNT				( 10 )

#define I2C_RCV_SND_SIZE	50		//RD8001暫定：サイズ要調整


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

typedef struct{
	UB main_cyc_req;		/* メイン周期要求(20ms) */

	UB system_mode;			/* システムモード */
	UB system_mode_chg_req;	/* システムモード変更要求 */
	UB info_data;			/* H1D情報 */
	
	MEAS meas;				/* 計測値(50ms) */
	UH dench_sts;			/* 電池残量状態 */
	
	UB hour;
	UB min;
	UB sec;
	
	UB sensing_start_trig;		// センシング開始トリガ
	UB sensing_end_flg;			// センシング終了

	UW sensing_cnt_50ms;		// センシング終了[50ms]
	
	UB pow_sw_last;				// 電源ボタン状態(前回)
	
	
	UB non_wdt_refresh;			// WDTリフレッシュなし
	
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
/* 外部参照宣言												*/
/************************************************************/
// 外部参照宣言　※無いと正常に動かない
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

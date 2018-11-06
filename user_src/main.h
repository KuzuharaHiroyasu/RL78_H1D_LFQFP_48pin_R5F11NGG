/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : main.c														*/
/* 機能         : 全体管理(マクロ定義、型定義、関数の外部参照宣言)				*/
/* 変更履歴     : 2018.03.02 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef		_MAIN_H_			/* 二重定義防止 */
#define		_MAIN_H_


/* ファイル名(SD画像書換え,ダウンロード) */
#define			SYSTEM_MODE_NON						0
#define			SYSTEM_MODE_SENSOR					1
#define			SYSTEM_MODE_GET_MODE				2

// EEP
#define EEP_DEVICE_ADR			0xA0				// デバイスアドレス
#define EEP_RECORD_SIZE			16					// 1レコードサイズ ※256の約数である必要がある
#define EEP_ADRS_SIZE			2					// 1レコードサイズ

// 加速度センサ
#define ACL_DEVICE_ADR			0x1C				// 加速度センサデバイスアドレス

#define EEP_DATA_SIZE_ALL		131072				// EEP全サイズ
#define EEP_DATA_SIZE_PAGE		( 131072 / 2 )		// EEPページサイズ


#define EEP_RECORD_1P_MAX			( EEP_DATA_SIZE_PAGE / EEP_RECORD_SIZE )			// 1レコードサイズ(1ページ目)
#define EEP_RECORD_2P_MAX			( EEP_RECORD_1P_MAX * 2 )							// 1レコードサイズ(2ページ目)

// 30秒[20ms]
#define SEC_30_CNT				( 30 * 1000 / 20 )
//#define SEC_30_CNT				( 30 * 1000 / 200 )	//10倍デバッグ

// 12時間[30秒]
#define HOUR_12_CNT				( 12 * 60 * 2 )
//#define HOUR_12_CNT				( 10 )

#define I2C_RCV_SND_SIZE	50		//RD8001暫定：サイズ要調整




// EEP書き込み用レコード(アドレス付き)
typedef struct{
	union{
		UB	byte[EEP_RECORD_SIZE + EEP_ADRS_SIZE];
		struct{
			UH		wr_adrs;				/* EEPアドレス */
			UH	kokyu_val;		
			UH	ibiki_val;		
			W	sekishoku_val;	// 差動入力の為に符号あり
			W	sekigaival;		// 差動入力の為に符号あり
			UB	valid;			/* レコード有効/無効				*/
			B acl_x;
			B acl_y;
			B acl_z;
		}data;
	}record;
}WR_EEP_RECORD;

// EEPレコード
typedef struct{
	union{
		UB	byte[EEP_RECORD_SIZE];
		struct{
			UH	kokyu_val;		
			UH	ibiki_val;		
			W	sekishoku_val;	// 差動入力の為に符号あり
			W	sekigaival;		// 差動入力の為に符号あり
			UB	valid;			/* レコード有効/無効				*/
			B acl_x;
			B acl_y;
			B acl_z;
#if 0
			double	ibiki_val;				/* いびき */
			double	myaku_val_sekishoku;	/* 脈拍_赤外 */
			double	myaku_val_sekigai;		/* 脈拍_赤色 */
			double	spo2_val_normal;		/* SPO2_通常 */
			double	spo2_val_konica;		/* SPO2 */
			UB state_flg;					/* フラグ(呼吸状態,睡眠ステージ,いびき有無 */
			UB	valid;						/* レコード有効/無効 */
#endif
		}data;
	}record;
}EEP_RECORD;
typedef struct{
	union{
		UB	byte[CPU_COM_SND_DATA_SIZE_SENSOR_DATA];
		struct{
			H	sekigaival;		// 差動入力の為に符号あり
			H	sekishoku_val;	// 差動入力の為に符号あり
			UH	kokyu_val;		
			UH	ibiki_val;		
			B acl_x;
			B acl_y;
			B acl_z;
			B dummy;			// 空き[パディング調整]
		}dat;
	}info;
}MEAS;

typedef struct{
	UB main_cyc_req;		/* メイン周期要求(20ms) */
	UB system_mode;			/* メイン周期要求(20ms) */
	
	UB system_mode_chg_req;	/* システムモード変更要求 */
	
	
	
	UH eep_wr_record_cnt;	/* 書き込みレコード */
	UH eep_rd_record_cnt;	/* 読み出しレコード */	
	// 計測値(20ms)
	MEAS meas;				/* 計測値(50ms) */
	UH dench_val;
#if 0
	UH kokyu_val;		
	UH ibiki_val;		
	W sekishoku_val;	// 差動入力の為に符号あり
	W sekigaival;		// 差動入力の為に符号あり
	B acl_x;			// 加速度センサ(8bit)
	B acl_y;
	B acl_z;
#endif
	EEP_RECORD	eep;		// データレコード(EEP読み書き、UART送信)
	
	UB hour;
	UB min;
	UB sec;
	
	
	
	UB sensing_start_trig;		// センシング開始トリガ
	UB sensing_end_flg;			// センシング終了

	UB sensing_cnt;			// センシング終了
	
	UB pow_sw_last;				// 電源ボタン状態(前回)
	
	
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
	void (*func)(UB *p_data);		/* 受信処理 */
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
extern void adc_do( uint16_t* ad1, uint16_t* ad2, uint16_t* ad3 );
extern unsigned short pga_do( void );
extern void R_IICA0_StopCondition(void);
extern void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
extern void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );




#endif
/************************************************************/
/* END OF TEXT												*/
/************************************************************/

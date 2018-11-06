/************************************************************************/
/* システム名   : RD8001 快眠チェッカー									*/
/* ファイル名   : cpu_com.h												*/
/* 機能         : CPU間通信ミドル										*/
/* 変更履歴     : 2018.04.10 Axia Soft Design 西島		初版作成		*/
/* 注意事項     : なし                                                  */
/************************************************************************/
#ifndef _CPU_COM_H_
#define _CPU_COM_H_


/******************/
/* 定数マクロ定義 */
/******************/
#define CPU_COM_SND_DATA_SIZE_STATUS_REQ			13				/* ステータス要求			*/
#define CPU_COM_SND_DATA_SIZE_MODE_CHG				1				/* 状態変更(G1D)			*/
#define CPU_COM_SND_DATA_SIZE_PC_LOG				7				/* PCログ送信(内部コマンド)	*/
#define CPU_COM_SND_DATA_SIZE_SENSOR_DATA			11				/* センサーデータ	*/

// モード変更要求
#define			SYSTEM_MODE_HD_CHG_NON				0		// なし
#define			SYSTEM_MODE_HD_CHG_SENSING			1		// あり(センシング)
#define			SYSTEM_MODE_HD_CHG_IDLE				2		// あり(アイドル)


/* データ長範囲 */								// RD8001暫定
#define CPU_COM_DATA_SIZE_MAX					(262+1)		/* データの最大長 */
															/* 最大データ 表示データ更新 詳細スランプグラフ 種別No(2)+データ長(2)+データ(308) */
// 各データサイズ
#define CPU_COM_CMD_SIZE						(1)			/* コマンドデータサイズ 1byte */
#define CPU_COM_SEQ_SIZE						(1)			/* SEQNoデータサイズ 1byte */
#define CPU_COM_CRC_SIZE						(2)			/* CRCデータサイズ 2byte */
#define CPU_COM_STX_SIZE						(2)			/* STXデータサイズ 2byte */
#define CPU_COM_ETX_SIZE						(2)			/* ETXデータサイズ 2byte */

/* メッセージの最小 */
#define CPU_COM_MSG_SIZE_MIN					( CPU_COM_CMD_SIZE + \
												  CPU_COM_SEQ_SIZE + \
												  CPU_COM_CRC_SIZE + \
												  CPU_COM_STX_SIZE + \
												  CPU_COM_ETX_SIZE )


/* メッセージの最大 コマンド(1)+SEQ(2)+データ(262)+SUM(2)+CRC(2) */
#define CPU_COM_MSG_SIZE_MAX					(CPU_COM_DATA_SIZE_MAX + CPU_COM_MSG_SIZE_MIN)

/* 送信ステータス */
#define CPU_COM_SND_STATUS_IDLE					0			/* 送信可能状態 */
#define CPU_COM_SND_STATUS_RCV_WAIT				1			/* 応答受信待ち状態 */
#define CPU_COM_SND_STATUS_RENSOU				2			/* 連送中状態 */
#define CPU_COM_SND_STATUS_COMPLETE				3			/* 送信完了状態 */
#define CPU_COM_SND_STATUS_SEND_NG				4			/* リトライNG */
#define CPU_COM_SND_STATUS_SEND_WAIT			5			/* 送信待ち */

/* CPU間通信受信待ち送信リクエスト結果 */
#define CPU_COM_SND_RES_OK						0			/* 応答正常受信 */
#define CPU_COM_SND_RES_BUSY_NG					1			/* CPU間通信BUSY状態(送信中or受信待ち)NG */
#define CPU_COM_SND_RES_RETRY_OUT_NG			2			/* リトライアウト */
#define CPU_COM_SND_RES_COM_NG					3			/* 送信コマンドNG */

/* CPU間通信 コマンド種別 */
/* 要求・応答のセット */
typedef enum _CPU_COM_CMD_ID{
	CPU_COM_CMD_NONE=0,							/* 【CPU間通信コマンド】コマンド無し				*/
	CPU_COM_CMD_STATUS,							/* 【CPU間通信コマンド】ステータス要求				*/
	CPU_COM_CMD_SENSOR_DATA,					/* 【CPU間通信コマンド】センサーデータ更新			*/
	CPU_COM_CMD_MODE_CHG,						/* 【CPU間通信コマンド】状態変更(G1D)				*/
	CPU_COM_CMD_PC_LOG,							/* 【CPU間通信コマンド】PCログ送信(内部コマンド)	*/
	CPU_COM_CMD_DATE_SET,						/* 【CPU間通信コマンド】日時設定	*/
	
	CPU_COM_CMD_PRG_DOWNLORD_READY,				/* 【CPU間通信コマンド】プログラム転送準備		*/
	CPU_COM_CMD_PRG_DOWNLORD_START,				/* 【CPU間通信コマンド】プログラム転送開始		*/
	CPU_COM_CMD_PRG_DOWNLORD_ERASE,				/* 【CPU間通信コマンド】プログラム転送消去		*/
	CPU_COM_CMD_PRG_DOWNLORD_DATA,				/* 【CPU間通信コマンド】プログラム転送データ	*/
	CPU_COM_CMD_PRG_DOWNLORD_RESLUT,			/* 【CPU間通信コマンド】プログラム転送結果		*/
	CPU_COM_CMD_PRG_DOWNLORD_CHECK,				/* 【CPU間通信コマンド】プログラム転送確認		*/
//	CPU_COM_CMD_FILE_REQ,						/* 【CPU間通信コマンド】ファイル転送開始		*/
//	CPU_COM_CMD_FILE,							/* 【CPU間通信コマンド】ファイル転送			*/
//	CPU_COM_CMD_FILE_BLOCK_RESULT_REQ,			/* 【CPU間通信コマンド】ブロック転送結果要求	*/
//	CPU_COM_CMD_FILE_RESLUT_REQ,				/* 【CPU間通信コマンド】ファイル転送結果要求	*/
	CPU_COM_CMD_MAX								/* 【CPU間通信コマンド】最大値					*/
}CPU_COM_CMD_ID;


/* コマンドタイプ */
/* ※マスター専用 */
#define CPU_COM_CMD_TYPE_ONESHOT_SEND			0					/* 単発送信コマンド */
#define CPU_COM_CMD_TYPE_RETRY					1					/* リトライ */
#define CPU_COM_CMD_TYPE_RENSOU					2					/* 連送 */
#define CPU_COM_CMD_TYPE_ONESHOT_RCV			3					/* 単発受信コマンド */


/* CPU間通信ダウンロード */
/* RD1402暫定 ブロック番号は0始まりでSH側でオフセットをかける */
/* ブロック番号最大値は暫定値のため要調整 */
#define CPU_COM_BLOCK_NUM_MIN			(0x00000000)		/* CPU間通信ブロック番号 最小値 */
#define CPU_COM_BLOCK_NUM_MAX			(0x00FFFFFF)		/* CPU間通信ブロック番号 最大値 */

// RD8001暫定：ドライバへ移動
/* 送信ステータス */
#define DRV_CPU_COM_STATUS_CAN_SEND		(0)			/* 送信可能状態 */
#define DRV_CPU_COM_STATUS_SENDING		(1)			/* 送信中 */
//#define DRV_CPU_COM_STATUS_SEND_END		(2)			/* 送信終了 */ /* 送信終了状態から5ms後に送信可能となる */



/* 読み出し(文字列、ファイル)結果 */
typedef enum{
	READ_RESULT_NO_ERROR = 0,		/* 正常終了 */
	READ_RESULT_ERROR_READ,			/* 読み出し失敗 */
	READ_RESULT_FILE_NON,			/* ファイルなし */
	READ_RESULT_MAX
} ENUM_READ_RESULT;


/* =====通信仕様関連===== */



// 通信データに於ける制御コード
#define CPU_COM_CTRL_CODE_STX			0x02					/* STX */
#define CPU_COM_CTRL_CODE_ETX			0x03					/* ETX */
#define CPU_COM_CTRL_CODE_DLE			0x10					/* DLE */

/* 読み出し(文字列、ファイル)結果 */
typedef enum{
	CPU_COM_RCV_MSG_STATE_STX_WAIT = 0,			/* STX待ち */
	CPU_COM_RCV_MSG_STATE_ETX_WAIT,				/* ETX待ち */
}CPU_COM_RCV_MSG_STATE;

// メッセージ配列の添え字
#define CPU_COM_MSG_TOP_POS_CMD					2				/* コマンド */
#define CPU_COM_MSG_TOP_POS_SEQ					3				/* シーケンス */
#define CPU_COM_MSG_TOP_POS_DATA				4				/* データ部 */



//サブCPU通信異常判定回数
#define CPU_COM_SUB_CNT_OPE_START			( 20 )					/* 開始用約1秒[10ms] */
#define CPU_COM_SUB_CNT_NORMAL				( 1 * 60 * 100 )		/* 通常約1分[10ms] */
#define CPU_COM_SUB_CNT_PRG					( 6 * 60 * 100 )		/* プログラム書換約6分[10ms] */

#define CPU_COM_SUB_OPE_END_TIME_PRG		7000	/* 操作完了確認周期[70秒] ※プログラム */
#define CPU_COM_SUB_OPE_END_TIME			500		/* 操作完了確認周期[5秒] ※その他 */


// 操作終了状態
typedef enum{
	CPU_COM_SUB_OPE_END_RESULT_OK = 0,			/* 0：正常終了*/
	CPU_COM_SUB_OPE_END_RESULT_NG,				/* 1：異常終了 */
	CPU_COM_SUB_OPE_END_RESULT_CONT,			/* 2：処理中 */
}CPU_COM_SUB_OPE_END_RESULT;

/* サブマイコン操作 */
typedef enum{
	CPU_COM_SUB_OPE_IMAGE = 0,			/* 画像転送(メモリーカード) */
	CPU_COM_SUB_OPE_PRG,				/* アンドロイド(サブマイコン)プログラム更新 */
	CPU_COM_SUB_OPE_BACKUP,				/* バックアップ(アンドロイド→メモリーカード) */
	CPU_COM_SUB_OPE_RECOVERY,			/* リカバリ(メモリーカード→アンドロイド) */
	CPU_COM_SUB_OPE_NON					/* なし */
}CPU_COM_SUB_OPE;

/* サブマイコン操作状態 */
typedef enum{
	CPU_COM_SUB_STATE_NON = 0,			/* なし */
	CPU_COM_SUB_STATE_START,			/* 操作開始(アンドロイド→メモリーカード) */
	CPU_COM_SUB_STATE_END,				/* 操作終了(アンドロイド→メモリーカード) */
	CPU_COM_SUB_STATE_RX_PRG,			/* RX(メインマイコン)プログラム更新開始 */
}CPU_COM_SUB_STATE;

//サブCPU操作
typedef struct{
	CPU_COM_SUB_OPE ope;		// 操作
	CPU_COM_SUB_STATE state;	// 状態
	UW last_time;				// 時間
	UW no_res_cnt;				// 未応答回数
}CPU_COM_SUB;

/******************/
/*   構造体定義   */
/******************/
typedef struct _T_CPU_COM_CMD_INFO {
	UB cmd;					/* コマンド */
	UB cmd_type;			/* コマンドタイプ */
	UB retry_cnt;			/* リトライ回数 */
	UW retry_time;			/* リトライ間隔 */
	UB rensou_cnt;			/* 連送回数 */
} T_CPU_COM_CMD_INFO;


typedef struct{
	UH file_num;					/* ファイル番号	2byte */
	UW block_num;					/* ブロック番号	4byte */
	UB* p_data;						/* 書き換えデータの先頭ポインタ	256byteまで */
	UH len;							/* データ長 *///RD1402暫定
}T_CPU_COM_CMD_FILE_DATA;


typedef struct{
	UW block_num;					/* ブロック番号	4byte */
}T_CPU_COM_CMD_FILE_RESULT_BLOCK;

typedef struct{
	UW sum;							/* サム値	4byte */
}T_CPU_COM_CMD_FILE_RESULT;

// ファイル読み出し
typedef struct{
	UH file_num;					/* ファイル番号	2byte */
	UW block_num;					/* ブロック番号	4byte */
}T_CPU_COM_CMD_READ_FILE;




typedef struct{
	UB buf[CPU_COM_MSG_SIZE_MAX];		/* バッファ */
	UH pos;								/* 受信位置 */
	UB last_dle_flg;					/* 前回受信データがDLE */
	UB state;							/* 前回受信データがDLE */
}CPU_COM_RCV_MSG;



/******************/
/*  外部参照宣言  */
/******************/
extern void cpu_com_init(void);
extern void cpu_com_proc(void);
extern void cpu_com_send_end( void );
extern UB cpu_com_get_status(void);


#endif
/************************************************************************/
/* END OF TEXT															*/
/************************************************************************/
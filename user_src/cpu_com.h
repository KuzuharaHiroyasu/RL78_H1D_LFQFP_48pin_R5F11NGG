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
// 送信データサイズ
#define CPU_COM_SND_DATA_SIZE_STATUS_REQ			13				/* ステータス要求			*/
#define CPU_COM_SND_DATA_SIZE_MODE_CHG				1				/* 状態変更(G1D)			*/
#define CPU_COM_SND_DATA_SIZE_PC_LOG				7				/* PCログ送信(内部コマンド)	*/
#define CPU_COM_SND_DATA_SIZE_SENSOR_DATA			15				/* センサーデータ	*/

// 受信データサイズ
#define CPU_COM_RCV_DATA_SIZE_PC_LOG				10				/* PCログ */



// モード変更要求
// RD8001暫定：削除予定
#if 0
#define			SYSTEM_MODE_HD_CHG_NON				0		// なし
#define			SYSTEM_MODE_HD_CHG_SENSING			1		// あり(センシング)
#define			SYSTEM_MODE_HD_CHG_IDLE				2		// あり(アイドル)
#endif

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
	CPU_COM_CMD_DISP_ORDER,						/* 【CPU間通信コマンド】表示指示				*/
	CPU_COM_CMD_MAX								/* 【CPU間通信コマンド】最大値					*/
}CPU_COM_CMD_ID;


/* コマンドタイプ */
/* ※マスター専用 */
#define CPU_COM_CMD_TYPE_ONESHOT_SEND			0					/* 単発送信コマンド */
#define CPU_COM_CMD_TYPE_RETRY					1					/* リトライ */
#define CPU_COM_CMD_TYPE_RENSOU					2					/* 連送 */
#define CPU_COM_CMD_TYPE_ONESHOT_RCV			3					/* 単発受信コマンド */



/* =====通信仕様関連===== */
// 通信データに於ける制御コード
#define CPU_COM_CTRL_CODE_STX			0x02					/* STX */
#define CPU_COM_CTRL_CODE_ETX			0x03					/* ETX */
#define CPU_COM_CTRL_CODE_DLE			0x10					/* DLE */

// 電文解析用
typedef enum{
	CPU_COM_RCV_MSG_STATE_STX_WAIT = 0,			/* STX待ち */
	CPU_COM_RCV_MSG_STATE_ETX_WAIT,				/* ETX待ち */
}CPU_COM_RCV_MSG_STATE;


// メッセージ配列の添え字
#define CPU_COM_MSG_TOP_POS_CMD					2				/* コマンド */
#define CPU_COM_MSG_TOP_POS_SEQ					3				/* シーケンス */
#define CPU_COM_MSG_TOP_POS_DATA				4				/* データ部 */

// =========================
// データ種別
// =========================
// 表示指示
#define CPU_COM_DISP_ORDER_NON				0	// 表示なし
#define CPU_COM_DISP_ORDER_SELF_CHECK_ERR	1	// 異常
#define CPU_COM_DISP_ORDER_SELF_CHECK_FIN	2	// 完了

/******************/
/*   構造体定義   */
/******************/
typedef struct _T_CPU_COM_CMD_INFO {
	UB cmd;					/* コマンド */
//	UB cmd_type;			/* コマンドタイプ[マスター] */
//	UB retry_cnt;			/* リトライ回数[マスター] */
//	UW retry_time;			/* リトライ間隔[マスター] */
//	UB rensou_cnt;			/* 連送回数[マスター] */
} T_CPU_COM_CMD_INFO;


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
extern void test_cpu_com_send( void );

#endif
/************************************************************************/
/* END OF TEXT															*/
/************************************************************************/
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
#define CPU_COM_SND_DATA_SIZE_SENSOR_DATA			15				/* センサーデータ	*/
#define CPU_COM_SND_DATA_SIZE_VERSION				8				/* バージョン	*/
#define CPU_COM_SND_DATA_SIZE_DATE					1				/* 日時設定	*/
#define CPU_COM_SND_DATA_SIZE_PRG_READY				1				/* プログラム準備 */
#define CPU_COM_SND_DATA_SIZE_PRG_CHECK				4				/* プログラム確認 */
#define CPU_COM_SND_DATA_SIZE_DISP_OREDER			1				/* 表示指示 */


// 受信データサイズ
#define CPU_COM_RCV_DATA_SIZE_PC_LOG				10				/* PCログ */


/* データ長 */
#define CPU_COM_DATA_SIZE_MAX					( 20 )			/* データの最大長 */
																/* 最大データ プログラムデータ アドレス(4) + 書き込みデータ(16) */

// 各ヘッダサイズ
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

/* メッセージの最大[送受共通] ※ヘッダ＋最大データ,DLE伸長なし */
#define CPU_COM_MSG_SIZE_MAX					( CPU_COM_DATA_SIZE_MAX + CPU_COM_MSG_SIZE_MIN )

/* バッファの最大[送受共通] ※DLE伸長考慮で2倍,リングバッファ用に+1 */
#define CPU_COM_BUF_SIZE_MAX					(( CPU_COM_MSG_SIZE_MAX * 2 ) + 1 )


// ドライバのリングバッファ定義
#define	DRV_UART0_RCV_RING_LENGTH				( CPU_COM_BUF_SIZE_MAX )		/* リングバッファ長(1メッセージ分＋１) ※＋１の理由はリングバッファの注意事項参照 */
#define	DRV_UART0_SND_RING_LENGTH				( CPU_COM_BUF_SIZE_MAX )		/* リングバッファ長(1メッセージ分＋１) ※＋１の理由はリングバッファの注意事項参照 */


/* CPU間通信 コマンド種別 */
/* 要求・応答のセット */
typedef enum _CPU_COM_CMD_ID{
	CPU_COM_CMD_NONE=0,							/* 【CPU間通信コマンド】コマンド無し				*/
	CPU_COM_CMD_STATUS,							/* 【CPU間通信コマンド】ステータス要求				*/
	CPU_COM_CMD_SENSOR_DATA,					/* 【CPU間通信コマンド】センサーデータ更新			*/
	CPU_COM_CMD_SENSING_ORDER,					/* 【CPU間通信コマンド】センシング指示				*/
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
	CPU_COM_CMD_VERSION,						/* 【CPU間通信コマンド】バージョン				*/
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
#define CPU_COM_DISP_ORDER_SELF_CHECK_NON	0	// 自己診断中表示(表示なし)
#define CPU_COM_DISP_ORDER_SELF_CHECK_ERR	1	// 自己診断中表示(異常)
#define CPU_COM_DISP_ORDER_SELF_CHECK_FIN	2	// 自己診断中表示(完了)

#define CPU_COM_DISP_TRG_LONG_SW_RECEPTION	3	// SW長押し(受付)

/******************/
/*   構造体定義   */
/******************/
typedef struct _T_CPU_COM_CMD_INFO {
	UB cmd;					/* コマンド[マスター/スレーブ] */
//	UB cmd_type;			/* コマンドタイプ[マスター] */
//	UB retry_cnt;			/* リトライ回数[マスター] */
//	UW retry_time;			/* リトライ間隔[マスター] */
//	UB rensou_cnt;			/* 連送回数[マスター] */
} T_CPU_COM_CMD_INFO;
// 送信用コマンド情報

typedef struct{
	UB buf[CPU_COM_MSG_SIZE_MAX];		/* バッファ[DLEなし] */
	UH pos;								/* 受信位置 */
	UB last_dle_flg;					/* 前回受信データがDLE */
	UB state;							/* 前回受信データがDLE */
}CPU_COM_ANA_RCV_MSG;
// 解析用受信メッセージ


/******************/
/*  外部参照宣言  */
/******************/
extern void cpu_com_init(void);
extern void cpu_com_proc(void);
extern void cpu_com_send_start( void );
extern void cpu_com_send_end_wait( void );

#endif
/************************************************************************/
/* END OF TEXT															*/
/************************************************************************/
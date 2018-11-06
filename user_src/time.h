/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : time.h														*/
/* 機能         : 時間管理(マクロ定義、型定義、関数の外部参照宣言)				*/
/* 変更履歴     : 2018.01.25 Axia Soft Design 西島 稔	初版作成		*/
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef		_TIME_H_			/* 二重定義防止 */
#define		_TIME_H_

/************************************************************/
/* マクロ													*/
/************************************************************/
/* タイマ種別[1ms] */
/* 注意：処理負荷に与える影響が大きい為に安易に増やさない事 */
typedef enum{
	TIME_TYPE_1MS_DUMMY	= 0,							/* ダミー[30ms] */
	TIME_TYPE_1MS_NUM									/* ソフトウェアタイマの数(1ms) */
}TIME_TYPE_1MS;

/* タイマカウント値[1ms] */
#define	TIME_TYPE_1MS_DUMMY						(30)		/* ダミー */



/* タイマ種別[10ms] */
typedef enum{
	TIME_TYPE_10MS_POW_SW_ACT_START = 0		,			/* 電源SW動作開始(1秒) */
	TIME_TYPE_10MS_CPU_COM_RETRY			,			/* CPU間通信リトライ待ちタイマ */
	TIME_TYPE_10MS_SENSING_DELAY			,			/* センシング開始待ち */
	TIME_TYPE_10MS_NUM									/* ソフトウェアタイマの数(10ms) */
}TIME_TYPE_10MS;

/* タイマカウント値[10ms] */
#define	TIME_10MS_CNT_POW_SW_ACT_START		100				/* 電源SW動作開始(1秒) */
#define	TIME_10MS_CNT_STATE_STOP			1000			/* 休止状態遷移時間(10秒) */

#define	TIME_10MS_CNT_SENSING_DELAY				100				/* センシング開始待(1秒) */


/************************************************************/
/* 型定義													*/
/************************************************************/

/************************************************************/
/* 外部参照宣言												*/
/************************************************************/
extern void time_init( void );
extern void time_update_elapsed( void );
extern void time_soft_set_1ms( TIME_TYPE_1MS type ,UW cnt );
extern void time_soft_set_10ms( TIME_TYPE_10MS type ,UW cnt );
extern void time_soft_get_1ms( TIME_TYPE_1MS type, UW* p_cnt );
extern void time_soft_get_10ms( TIME_TYPE_10MS type, UW* p_cnt );
extern void time_get_elapsed_time(UW *petm);
extern void time_set_elapsed_time(UW etm);
extern void time_soft_dec_1ms( void );
extern void time_soft_dec_10ms( void );


#endif

/************************************************************/
/* END OF TEXT												*/
/************************************************************/


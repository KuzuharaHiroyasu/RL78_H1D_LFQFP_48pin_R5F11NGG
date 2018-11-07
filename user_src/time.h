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
	TIME_TYPE_10MS_POW_SW = 0				,			/* 電源SW動作 */
	TIME_TYPE_10MS_CPU_COM_RETRY			,			/* CPU間通信リトライ待ちタイマ */
	TIME_TYPE_10MS_SENSING_DELAY			,			/* センシング開始待ち */
	TIME_TYPE_10MS_DISP_FLASH				,			/* 表示点滅 */
	TIME_TYPE_10MS_WAIT_SLEEP				,			/* sleep待ち */
	TIME_TYPE_10MS_BAT_CHG_FIN				,			/* 充電完了チェック */
	TIME_TYPE_10MS_SW_LONG_DISP				,			/* 電源SW長押し表示 */
	TIME_TYPE_10MS_CHG_ON_DISP_IVALID		,			/* 充電表示無効時間 */
	TIME_TYPE_10MS_NUM									/* ソフトウェアタイマの数(10ms) */
}TIME_TYPE_10MS;

/* タイマカウント値[10ms] */
#define	TIME_10MS_CNT_POW_SW_INITIAL_DISP	1000			/* 電源SW_初期化リセット表示(10秒) */
#define	TIME_10MS_CNT_POW_SW_LONG			300				/* 電源SW_長(3秒) */
#define	TIME_10MS_CNT_POW_SW_SHORT			100				/* 電源SW_短(1秒) */



#define	TIME_10MS_CNT_POW_SW_LONG_DISP		300				/* 電源SW長押し表示(3秒) */

#define	TIME_10MS_CNT_CHG_ON_DISP_IVALID	100				/* 充電表示無効時間(1秒) */


#define	TIME_10MS_CNT_STATE_STOP			1000		/* 休止状態遷移時間(10秒) */

#define	TIME_10MS_CNT_SENSING_DELAY			100			/* センシング開始待(1秒) */

#define	TIME_10MS_CNT_WAIT_SLEEP			( 2 + 1 )	/* sleep待ち[10ms以上] */

#define	TIME_10MS_CNT_BAT_CHG_FIN			( 50 + 1 )	/* 充電完了チェック[500ms以上] */

// 表示点滅
#define	TIME_10MS_CNT_DISP_FLASH_50MS		5				/* 表示点滅(50ms) */
#define	TIME_10MS_CNT_DISP_FLASH_100MS		10				/* 表示点滅(100ms) */
#define	TIME_10MS_CNT_DISP_FLASH_500MS		50				/* 表示点滅(500ms) */
#define	TIME_10MS_CNT_DISP_FLASH_1S			100				/* 表示点滅(1s) */
#define	TIME_10MS_CNT_DISP_FLASH_3S			300				/* 表示点滅(3s) */

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


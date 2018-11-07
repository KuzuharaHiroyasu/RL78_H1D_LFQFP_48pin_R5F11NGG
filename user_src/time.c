/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : time.c														*/
/* 機能         : 時間管理部（関数,RAM,ROM定義）								*/
/* 変更履歴     : 2018.01.25 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : �@センシング中は正しい値が取れない。※設定時時間より長くなる。*/
/*              : �A1msとetc処理は現在未使用									*/
/********************************************************************************/

/********************/
/*     include      */
/********************/
#include "header.h"

/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/
void time_init( void );
void time_update_elapsed( void );
void time_soft_set_1ms( TIME_TYPE_1MS type ,UW cnt );
void time_soft_set_10ms( TIME_TYPE_10MS type ,UW cnt );
void time_soft_get_1ms( TIME_TYPE_1MS type, UW* p_cnt );
void time_soft_get_10ms( TIME_TYPE_10MS type, UW* p_cnt );
void time_get_elapsed_time(UW *petm);
void time_set_elapsed_time(UW etm);
void time_soft_dec_1ms( void );
void time_soft_dec_10ms( void );


/************************************************************/
/* ＲＡＭ定義												*/
/************************************************************/
STATIC UW		s_time_soft_cnt_1ms[TIME_TYPE_1MS_NUM];			/* ソフトウェアタイマの現在値(1ms) */
STATIC UW		s_time_soft_cnt_10ms[TIME_TYPE_10MS_NUM];		/* ソフトウェアタイマの現在値(10ms) */
STATIC UW		s_elapsed_time;									/* 経過時間(10ms) ※約1年132日継続して演算可能 */

/************************************************************/
/* ＲＯＭ定義												*/
/************************************************************/


/************************************************************************/
/* 関数     : time_init													*/
/* 関数名   : ソフトウェアタイマ初期化処理								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.16 Axia Soft Design 六車 裕介	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* ソフトウェアタイマ初期化処理											*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void time_init( void )
{
	INT i;
	
	/* システム時間設定 */
	s_elapsed_time = 0;
	
	/* ソフトウェアタイマのカウントクリア */
	for ( i = 0; i < TIME_TYPE_1MS_NUM; i++ ) {
		s_time_soft_cnt_1ms[i] = 0;
	}

	for ( i = 0; i < TIME_TYPE_10MS_NUM; i++ ) {
		s_time_soft_cnt_10ms[i] = 0;
	}
	
	
#if DEBUG_RAM_CAPA_CHECK == ON
	if( &s_elapsed_time >= (UW*)(0x10000)){
		while(1){
			wdt_refresh();
		}
		
	}

#endif

}

/************************************************************************/
/* 関数     : time_update_elapsed										*/
/* 関数名   : 経過時間更新(10ms)										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.07.06 Axia Soft Design 西島 稔	初版作成			*/
/*          : 2013.04.04 Axia Soft Design 西島 稔	割り込み内専用処理な*/
/*          : ので禁止/許可削除											*/
/************************************************************************/
/* 機能 :																*/
/* 経過時間の更新														*/
/* 約1年132日継続して演算可能なのでクランプ処理は不要					*/
/************************************************************************/
/* 注意事項 :															*/
/* �@割り込み内での専用処理												*/
/************************************************************************/
void time_update_elapsed( void )
{
	/* 経過時間更新 */
	s_elapsed_time++;
}


/************************************************************************/
/* 関数     : time_soft_set_1ms											*/
/* 関数名   : ソフトウェアタイマセット処理								*/
/* 引数     : type	タイマ種別											*/
/*          : cnt	タイマ設定値										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.16 Axia Soft Design 六車 裕介	初版作成			*/
/*          : 2012.09.05 Axia Soft Design 吉居 久和	0セット時処理追加	*/
/*          : 2013.04.04 Axia Soft Design 西島 稔 割り込みから呼ばれる可*/
/*          : 能性がある為に割り込み許可/禁止処理変更					*/
/************************************************************************/
/* 機能 :																*/
/* 1ms周期ソフトウェアタイマセット処理									*/
/************************************************************************/
/* 注意事項 :															*/
/* �@メインと定周期割り込みは非同期である為に、最大1カウントのずれが	*/
/* 発生する。設定値は+1加算した値で設定する								*/
/* ※ただし、0が設定された場合はリセットとみなしそのまま0を設定する		*/
/************************************************************************/
void time_soft_set_1ms( TIME_TYPE_1MS type ,UW cnt )
{
	UB	iflg;

	DI_RET( iflg );								/* 割り込み禁止 */
	if( 0 != cnt ){
		cnt ++;
	}
	s_time_soft_cnt_1ms[type] = cnt;
	EI_RET( iflg );								/* 割り込み許可 */
}


/************************************************************************/
/* 関数     : time_soft_set_10ms										*/
/* 関数名   : ソフトウェアタイマセット処理								*/
/* 引数     : type	タイマ種別											*/
/*          : cnt	タイマ設定値										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.16 Axia Soft Design 六車 裕介	初版作成			*/
/* 変更履歴 : 2012.09.05 Axia Soft Design 吉居 久和	0セット時処理追加	*/
/*          : 2013.04.16 Axia Soft Design 西島 稔	割り込み状態復帰へ変*/
/* 			: 更														*/
/************************************************************************/
/* 機能 :																*/
/* 10ms周期ソフトウェアタイマセット処理									*/
/************************************************************************/
/* 注意事項 :															*/
/* �@メインと定周期割り込みは非同期である為に、最大1カウントのずれが	*/
/* 発生する。設定値は+1加算した値で設定する								*/
/* ※ただし、0が設定された場合はリセットとみなしそのまま0を設定する		*/
/* �A割り込み内では使用しないこと										*/
/************************************************************************/
void time_soft_set_10ms( TIME_TYPE_10MS type ,UW cnt )
{
	UB	iflg;

	DI_RET( iflg );						/* 割り込み禁止 */
	if( 0 != cnt ){
		cnt ++;
	}
	s_time_soft_cnt_10ms[type] = cnt;
	
	EI_RET( iflg );						/* 割り込み許可 */
}


/************************************************************************/
/* 関数     : time_soft_get_1ms											*/
/* 関数名   : ソフトウェアタイマカウント値取得処理						*/
/* 引数     : type	タイマ種別											*/
/*          : p_cnt	タイマカウント値									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.16 Axia Soft Design 六車 裕介	初版作成			*/
/*          : 2013.04.04 Axia Soft Design 西島 稔 割り込みから呼ばれる可*/
/*          : 能性がある為に割り込み許可/禁止処理変更					*/
/************************************************************************/
/* 機能 :																*/
/* 1ms周期ソフトウェアタイマカウント値取得								*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void time_soft_get_1ms( TIME_TYPE_1MS type, UW* p_cnt )
{
	UB	iflg;

	DI_RET( iflg );								/* 割り込み禁止 */
	*p_cnt = s_time_soft_cnt_1ms[type];			/* 指定タイマのカウント値を返す */
	EI_RET( iflg );								/* 割り込み許可 */
}


/************************************************************************/
/* 関数     : time_soft_get_10ms										*/
/* 関数名   : ソフトウェアタイマカウント値取得処理						*/
/* 引数     : type	タイマ種別											*/
/*          : p_cnt	タイマカウント値									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.16 Axia Soft Design 六車 裕介	初版作成			*/
/*          : 2013.04.16 Axia Soft Design 西島 稔	割り込み状態復帰へ変*/
/* 			: 更														*/
/************************************************************************/
/* 機能 :																*/
/* 10ms周期ソフトウェアタイマカウント値取得								*/
/************************************************************************/
/* 注意事項 :															*/
/* �@割り込み内では使用しないこと										*/
/************************************************************************/
void time_soft_get_10ms( TIME_TYPE_10MS type, UW* p_cnt )
{
	UB	iflg;

	DI_RET( iflg );								/* 割り込み禁止 */
	*p_cnt = s_time_soft_cnt_10ms[type];		/* 指定タイマのカウント値を返す */
	EI_RET( iflg );								/* 割り込み許可 */
}


/************************************************************************/
/* 関数     : time_get_elapsed_time										*/
/* 関数名   : ソフトウェア経過時間取得処理								*/
/* 引数     : petm	現在連続稼動時間									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.01.30 Axia Soft Design H.Wada	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 現在連続稼動時間の取得を行う											*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void time_get_elapsed_time(UW *petm)
{
	*petm = s_elapsed_time;
}

/************************************************************************/
/* 関数     : time_set_elapsed_time										*/
/* 関数名   : ソフトウェア経過時間設定									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.01.30 Axia Soft Design H.Wada	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 現在連続稼動時間の設定を行う											*/
/************************************************************************/
/* 注意事項 :															*/
/* 通常使用しない（デバッグ用コマンド）									*/
/************************************************************************/
void time_set_elapsed_time(UW etm)
{
	s_elapsed_time = etm;
}


/************************************************************************/
/* 関数     : time_soft_dec_1ms											*/
/* 関数名   : ソフトウェアタイマ減算処理								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.16 Axia Soft Design 六車 裕介	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 全種類のソフトウェアタイマ(1ms)のカウント減算を行う					*/
/************************************************************************/
/* 注意事項 :															*/
/* �@割り込み内での専用処理												*/
/************************************************************************/
void time_soft_dec_1ms( void )
{
	INT i;
	
	for( i = 0; i < TIME_TYPE_1MS_NUM; i++ ){
		if( 0 != s_time_soft_cnt_1ms[i] ){
			s_time_soft_cnt_1ms[i]--;			/* タイマカウント値減算 */
		}
	}
	
}


/************************************************************************/
/* 関数     : time_soft_dec_10ms										*/
/* 関数名   : ソフトウェアタイマ減算処理								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.16 Axia Soft Design 六車 裕介	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 全種類のソフトウェアタイマ(10ms)のカウント減算を行う					*/
/************************************************************************/
/* 注意事項 :															*/
/* �@割り込み内での専用処理												*/
/************************************************************************/
void time_soft_dec_10ms( void )
{
	INT i;
	
	for( i = 0; i < TIME_TYPE_10MS_NUM; i++ ){
		if( 0 != s_time_soft_cnt_10ms[i] ){
			s_time_soft_cnt_10ms[i]--;			/* タイマカウント値減算 */
		}
	}
	
}


/************************************************************/
/* END OF TEXT												*/
/************************************************************/

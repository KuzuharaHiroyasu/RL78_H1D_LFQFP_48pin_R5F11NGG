/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー												*/
/* ファイル名   : download.h													*/
/* 機能         : プログラム書換処理群の公開定義およびインタフェース			*/
/* 変更履歴     : 2012.02.25 Axia Soft Design K.Wada	初版作成				*/
/*              : 2012.12.26 Axia Soft Design K.Wada    RD1201から流用開始      */
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef		_DOWNLOAD_H_			/* 二重定義防止 */
#define		_DOWNLOAD_H_

#include "./header.h"
#include "common_memory_def.h"


/******************/
/* 定数マクロ定義 */
/******************/

/******************/
/*     型定義     */
/******************/

/******************/
/*    公開定数    */
/******************/


/******************/
/*  外部参照宣言  */
/******************/
extern void download_init(void);
extern void download_change_boot(void);
extern INT download_start(void);
extern INT download_set_data(UB *p_data, UW blocknum, UW size, UW *p_sum);
extern void download_finish_ready(void);
extern INT download_finish(void);
extern BOOL download_check_finish(UW com_sum);
extern BOOL download_check_checksum(void);
extern BOOL download_check_product(void);
extern INT download_get_data(UB *p_get_data,UW blocknum ,UW size);

/******************/
/* 関数マクロ定義 */
/******************/

#endif /*_DOWNLOAD_H_ */

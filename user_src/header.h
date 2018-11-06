/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : header.h														*/
/* 機能         : ヘッダインクルード用											*/
/* 変更履歴		: 2018.01.25 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/

#ifndef _HEADER_H_
#define _HEADER_H_

/* 標準 */
#include <stdio.h>
#include <limits.h>
//#include <stdlib.h>
//#include <ctype.h>
#include <string.h>
//#include <stdarg.h>

/* システム共通定義 */
#include "sys.h"

/* ドライバ */
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_pga_dsad.h"
#include "r_cg_amp.h"
#include "r_cg_dac.h"
#include "r_cg_sau.h"
#include "r_cg_intp.h"
#include "r_cg_it.h"
#include "r_cg_iica.h"
#include "r_cg_rtc.h"
#include "r_cg_adc.h"
#include "r_cg_wdt.h"
#include "r_cg_tau.h"

/* ミドル */
#include "com_srv.h"
#include "cpu_com.h"
#include "time.h"

#include "drv_from.h"
#include "vect_func.h"

/* アプリケーション */
#include "main.h"
#include "common_memory_def.h"

#endif /*_HEADER_H */

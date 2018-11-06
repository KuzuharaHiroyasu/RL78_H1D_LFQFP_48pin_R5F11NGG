/************************************************************************/
/* システム名   : RD8001快眠チェッカー									*/
/* ファイル名   : calc_pulseoximeter.h									*/
/* 機能         : パルスオキシメーター演算処理							*/
/* 変更履歴     : 2018.01.11 Axia Soft Design 和田 初版作成				*/
/* 注意事項     : なし													*/
/************************************************************************/
#ifndef		_CALC_PULSE_OXIMETER_H_		/* 二重定義防止 */
#define		_CALC_PULSE_OXIMETER_H_

#include "../sys.h"

/************************************************************/
/*					マクロ定数定義							*/
/************************************************************/


/************************************************************/
/* 						型定義								*/
/************************************************************/


/************************************************************/
/*					外部参照宣言							*/
/************************************************************/
extern void  calculator_pulse_oximeter(const short* pdata1, const short* pdata2);
extern double  get_result_sp_normal();
extern double  get_result_sp_konica();
extern double  get_result_shinpaku_sekigai();
extern double  get_result_shinpaku_sekisyoku();

#endif	/*_CALC_PULSE_OXIMETER_H_*/
/************************************************************/
/* END OF TEXT												*/
/************************************************************/

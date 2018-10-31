/************************************************************************/
/* システム名   : RD8001快眠チェッカー									*/
/* ファイル名   : calc_pulseoximeter.c									*/
/* 機能         : パルスオキシメーター演算処理							*/
/* 変更履歴     : 2018.01.11 Axia Soft Design 和田 初版作成				*/
/* 注意事項     : なし													*/
/************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "calc_pulseoximeter.h"
#include "../calc_data.h"

/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/
static	void	proc(double *data1, int len, int d);
static	void	debug_out	( char *f , double d[] , int size , int no );

//extern	void	debug_out	( char *f , double d[] , int size , int no );
extern	void	mado		( double in[] , double ot[] , int size , int mode );
extern	void	fft			( double in[] , int N, double ar[] , double ai[] , double p[] );
extern	int		peak_modify	( double in_data[] , int in_res[] , double ot_data[] , double ot_hz[] , int size , double delta );	/* ☆ */
extern	void 	peak_vallay	( double in[] , int    ot[] , int size, int width , double th , int peak );
extern	void	cal_sp1		( double mx1 , double mx2 , int *sp );
extern	void	cal_sp2		( double mx1 , double mx2 , double *sp );
extern	void	ifft(double	ar[],double	ai[],int N, double	ot[]);

/************************************************************/
/* 定数定義													*/
/************************************************************/


/************************************************************/
/* 変数定義													*/
/************************************************************/
//double	meas_data[BUF_SIZE];
/*------------------------------------------------------------------------------*/
//double	h11[BUF_SIZE];
/*------------------------------------------------------------------------------*/
//double	mado1[BUF_SIZE];
/*------------------------------------------------------------------------------*/
//double	ar1[BUF_SIZE];
//double	ai1[BUF_SIZE];
//double	p1 [BUF_SIZE];
/*------------------------------------------------------------------------------*/
//double	p2[BUF_SIZE];
//double	ar2[BUF_SIZE];
//double	ai2[BUF_SIZE];
//double	p3[BUF_SIZE];
//int		peak1[BUF_SIZE];
//double	p4[BUF_SIZE];
//double	f1[BUF_SIZE];
double*	p4;
double*	f1;

double	mx1;
double	mx2;
/*------------------------------------------------------------------------------*/
double	snpk1;
double	snpk2;
int		sp1;
double	sp2;
/*------------------------------------------------------------------------------*/

/********************************************************************/
/* 関数     : calculator_pulse_oximeter								*/
/* 関数名   : パルスオキシメーター演算								*/
/* 引数     : なし													*/
/* 戻り値   : なし													*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 																	*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
void calculator_pulse_oximeter(int *data1, int len1, int *data2, int len2)
{
	double tmp;
	int i;
	
	//データサイズ制限
	if(len1 > DATA_SIZE){
		len1 = DATA_SIZE;
	}
	if(len2 > DATA_SIZE){
		len2 = DATA_SIZE;
	}
	
	for (i=0; i<len1; i++)
	{
		tmp = (double)data1[i];
#if 0
		// 0～10Vの10bit分解能AD値
		meas_data[i] = (double)(tmp * 10 / 1024);
#else
		// -3～3Vの24bit分解能AD値
		meas_data[i] = (double)(tmp * 3 / 8388607);
#endif
	}
	proc(meas_data, len1, 1);
	snpk1 = f1[0];
	mx1 = p4[0];
	if((0 > snpk1) || (snpk1 > 200)){
		snpk1 = 0;
	}
	
	for (i = 0; i<len2; i++)
	{
		tmp = (double)data2[i];
#if 0
		meas_data[i] = (double)(tmp * 10 / 1024);
#else	// ■暫定 AD値調整なしにする
		meas_data[i] = (double)(tmp * 3 / 8388607);
#endif
	}
	proc(meas_data, len1, 2);
	snpk2 = f1[0];
	mx2 = p4[0];
	if((0 > snpk2) || (snpk2 > 200)){
		snpk2 = 0;
	}
	
	/*- SPO2 --------------------------------------------------------------*/
	cal_sp1( mx1 , mx2 , &sp1 );
	if((0 > sp1) || (sp1 > 200)){
		sp1 = 0;
	}
	cal_sp2( mx1 , mx2 , &sp2 );
	if((0 > sp2) || (sp2 > 200)){
		sp2 = 0;
	}
//	tmp = (double)(sp1);
//	debug_out( "sp(通常)"   , &tmp , 1 , 0 );			/* 通常					*/
//	debug_out( "sp(コニカ)" , &sp2 , 1 , 0 );			/* コニカ				*/
}

/********************************************************************/
/* 関数     : proc													*/
/* 関数名   : 														*/
/* 引数     : なし													*/
/* 戻り値   : なし													*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 																	*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
static void	proc(double *data1, int len, int d)
{
	int ii;
	double *d1;
	double dc;
	
	double* h11;
	double* mado1;

	double* ar1;
	double* ai1;
	double* p1;

	double* p2;
	double* ar2;
	double* ai2;
	double* p3;
	int* peak1;
	
	const int start = 8;
	const int end   = 92 + start;
	
	int size;
	double max;
	int peaksize1;
	
	/*- data set ---------------------------------------------------------------*/
	d1 = data1;

	/*- debug_out --------------------------------------------------------------*/
	debug_out( "data" , d1 , len , d );

	/*- DC推定 -----------------------------------------------------------------*/
	dc = 0.0f;
	for(ii=0;ii<len;++ii){
		dc += d1[ii];
	}
	dc /= len;
	
	/*- hosei1 -----------------------------------------------------------------*/
	h11 = &temp_dbl_buf0[0];
	memset(temp_dbl_buf0,0,len*sizeof(double));
	for(ii=0;ii<len;++ii){
		h11[ii] = d1[ii] - dc;
	}
	/*- debug_out --------------------------------------------------------------*/
	debug_out( "hosei1" , h11 , len, d );
	
	/*- mado -------------------------------------------------------------------*/
	mado1 = &temp_dbl_buf1[0];
	memset(temp_dbl_buf1,0,len*sizeof(double));
	mado( h11 , mado1 , len, 1 /* han */);	
	
	/*- debug_out --------------------------------------------------------------*/
	debug_out( "mado" , mado1 , len, d );

	/*- fft --------------------------------------------------------------------*/
	ar1 = &temp_dbl_buf2[0];
	memset(temp_dbl_buf2,0,len*sizeof(double));
	ai1 = &temp_dbl_buf3[0];
	memset(temp_dbl_buf3,0,len*sizeof(double));
	p1 = &temp_dbl_buf0[0];
	memset(temp_dbl_buf0,0,len*sizeof(double));
	fft( mado1 , len, ar1 , ai1 , p1 );

	/*- debug_out --------------------------------------------------------------*/
	debug_out( "ar1" , ar1 , len, d );
	debug_out( "ai1" , ai1 , len, d );
//	debug_out( "p1"  , p1  , len, d );
	
	p2 = &temp_dbl_buf0[0];
	memset(temp_dbl_buf0,0,len*sizeof(double));
	// startまではall 0
	for(ii=0;ii<start;++ii){
		p2[ii] = 0;
	}
	for(ii=start;ii<end;++ii){
		p2[ii] = sqrt(ar1[ii]*ar1[ii] + ai1[ii]*ai1[ii]);
	}
	for(ii=end;ii<len;++ii){
		p2[ii] = 0;
	}
	debug_out( "p2"  , p2  , len, d );
	
	/*- 2乗根 --------------------------------------------------------------*/
	for(ii=0;ii<len;++ii){
		p2[ii] = sqrt(p2[ii]);
	}
	debug_out( "p2(sqrt)"  , p2  , len, d );
	
	/*- 逆FFT --------------------------------------------------------------*/
	ar2 = &temp_dbl_buf2[0];
	memset(temp_dbl_buf2,0,len*sizeof(double));
	ai2 = &temp_dbl_buf3[0];
	memset(temp_dbl_buf3,0,len*sizeof(double));
	size = len/2;
	for(ii=0;ii<size;++ii){
		ar2[ii] = p2[ii];
		ai2[ii] = 0;
	}
	for(ii=0;ii<size;++ii){
		ar2[size+ii] = p2[size-ii];
		ai2[size+ii] = 0;
	}
	debug_out( "ar2"  , ar2  , len, d );
	
	p3 = &temp_dbl_buf0[0];
	memset(temp_dbl_buf0,0,len*sizeof(double));
	ifft(ar2, ai2, len, p3);
	debug_out( "p3"  , p3  , len, d );
	
	/*- 最大値との比を計算 --------------------------------------------------------------*/
	max = 0;
	for(ii=0;ii<len;++ii){
		if(max < p3[ii]){
			max = p3[ii];
		}
	}
	for(ii=0;ii<len;++ii){
		p3[ii] /= max;
	}
	debug_out( "p3(ratio)"  , p3  , len, d );
	
	/*- ピーク検出 --------------------------------------------------------------*/
	peak1 = &temp_int_buf0[0];
	memset(temp_int_buf0,0,len*sizeof(int));
	p4 = &temp_dbl_buf1[0];
	memset(temp_dbl_buf1,0,len*sizeof(double));
	f1 = &temp_dbl_buf2[0];
	memset(temp_dbl_buf2,0,len*sizeof(double));
	peak_vallay( p3 , peak1 , len, 3 , 0.05 , 1 );
	peaksize1 = peak_modify( p3 , peak1 , p4 , f1 , len , 1);
	debug_out("f", f1, peaksize1, d);
	
	/*- HR --------------------------------------------------------------*/
	for(ii=0;ii<peaksize1;++ii){
		f1[ii] = 60 / (f1[ii] / 6);
	}
	debug_out("hr", f1, peaksize1, d);
}

/********************************************************************/
/* 関数     : debug_out												*/
/* 関数名   : デバッグ出力											*/
/* 引数     : 														*/
/* 戻り値   : なし													*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 	元々はファイル出力していたが、今はダミー関数					*/
/********************************************************************/
/* 注意事項 :														*/
/* 	本関数はdebug_out.c参照											*/
/********************************************************************/
static	void	debug_out( char *f , double d[] , int size , int no )
{

}

/********************************************************************/
/* 関数     : get_result_sp_normal									*/
/* 関数名   : 通常のSpO2の結果取得									*/
/* 引数     : なし													*/
/* 戻り値   : double 取得された値									*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 	通常のSpO2の結果の値を取得する									*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
double get_result_sp_normal()
{
	double tmp = (double)(sp1);
	return tmp;
}

/********************************************************************/
/* 関数     : get_result_sp_konica									*/
/* 関数名   : コニカのSpO2の結果取得								*/
/* 引数     : なし													*/
/* 戻り値   : double 取得された値									*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 	コニカのSpO2の結果の値を取得する								*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
double get_result_sp_konica()
{
	return sp2;
}

/********************************************************************/
/* 関数     : get_result_shinpaku_sekigai							*/
/* 関数名   : 赤外の心拍数を取得									*/
/* 引数     : なし													*/
/* 戻り値   : double 取得された値									*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 	赤外の心拍数の値を取得する										*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
double get_result_shinpaku_sekigai()
{
	return snpk1;
}

/********************************************************************/
/* 関数     : get_result_shinpaku_sekisyoku							*/
/* 関数名   : 赤色の心拍数を取得									*/
/* 引数     : なし													*/
/* 戻り値   : double 取得された値									*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 	赤色の心拍数の値を取得する										*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
double get_result_shinpaku_sekisyoku()
{
	return snpk2;
}
/*==============================================================================*/
/* EOF */

/************************************************************************/
/* システム名   : RD8001快眠チェッカー									*/
/* ファイル名   : calc_apnea.c											*/
/* 機能         : 無呼吸判定演算処理									*/
/* 変更履歴     : 2018.01.11 Axia Soft Design 和田 初版作成				*/
/* 注意事項     : なし													*/
/************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calc_apnea.h"
#include "../calc_data.h"


/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/
void getwav_pp(const double* pData, int DSize, double Param1);
void getwav_apnea(const double* pData, int DSize, int Param1, double Param2, double Param3);
void getwav_snore(const double* pData, int DSize, double Param);
void Spline(double*,double*,int,double*,double*,int,double*);
void S_Spline( double *, double *, int, double *, double *, int,
              int, double, int, double, double * );
int S_TriDiag( double *, double *, double *, double *, int, int );
void lowpass(double input[], double output[], int size, double samplerate, double freq, double q);
void bandpass(double input[], double output[], int size, double samplerate, double freq, double bw);

static	void	debug_out	( char *f , double d[] , int size , int no );

extern void	seikika( double in[] , double ot[] , int size , double begin , double end );
//extern void	debug_out( char *f , double d[] , int size , int no );
extern int	peak_modify	( double in_data[] , int in_res[] , double ot_data[] , double ot_hz[] , int size , double delta, double th);	/* ☆ */
extern void peak_vallay	( double in[] , int    ot[] , int size, int width , int peak );

/************************************************************/
/* 定数定義													*/
/************************************************************/


/************************************************************/
/* 変数定義													*/
/************************************************************/
double	result_snore[BUF_SIZE];			// 結果いびき
UW		result_snore_size;
double	result_peak[BUF_SIZE];			// 結果ピーク間隔
int		result_peak_size;
double	result_apnea[BUF_SIZE_APNEA];	// 無呼吸・低呼吸
int		result_apnea_size;
int		result_snore_count;				// いびき回数

/************************************************************************/
/* 関数     : calculator_apnea											*/
/* 関数名   : 無呼吸判定演算											*/
/* 引数     : int *data : 波形データ									*/
/*          : int len   : 波形データサイズ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void calculator_apnea(int *data, int len)
{
	UW datasize;						// 波形データのバイト数
	double* ptest1;
	double* pbandpassout;
	double* plowpassout;
	double* x;
	double* xint;
	double* yint;
	double* ydash;
	UW newdatasize;
	int ii;
	
	//データサイズ制限
	datasize = len;
	if(datasize > DATA_SIZE){
		datasize = DATA_SIZE;
	}
	
	printf("size = %d\n", datasize);
	ptest1 = &temp_dbl_buf0[0];							//calloc
	memset(temp_dbl_buf0,0,datasize*sizeof(double));
	for(ii=0;ii<datasize;++ii){
		ptest1[ii] = (double)data[ii];
	}
	debug_out("原音カウント値1", ptest1, datasize, 1);

	for (ii = 0; ii<datasize; ++ii) {
		//ptest1[ii] = (ptest1[ii] - 512) * 64;	// 10bitカウント値を0中心にし、16bit幅に拡張
		ptest1[ii] = ptest1[ii] * 16;	// 16bit幅に拡張
	}
	debug_out("原音(wav変換)1", ptest1, datasize, 1);

	for (ii = 0; ii<datasize; ++ii) {
		ptest1[ii] = (double)ptest1[ii] * 0.0000305;
	}

	// ここまでの結果(ptest1[0]～)が (12) と一致することを確認済
	debug_out("原音1", ptest1, datasize, 1);
	
	// (13) BPF2
	pbandpassout = &temp_dbl_buf1[0];						//calloc
	memset(temp_dbl_buf1,0,datasize*sizeof(double));
	bandpass(ptest1, pbandpassout, datasize, 8000.0f, 2000.0f, 1.0f);
	debug_out("BPF2", pbandpassout, datasize, 1);
	
	// 絶対値にする
	for(ii=0;ii<datasize;++ii){
		pbandpassout[ii] = fabs(pbandpassout[ii]);
	}
	
	// (14) LPF2
	plowpassout = &temp_dbl_buf0[0];						//calloc
	memset(temp_dbl_buf0,0,datasize*sizeof(double));
	lowpass(pbandpassout, plowpassout, datasize, 8000.0f, 100.0f, 1.0f);
	debug_out("LPF2", plowpassout, datasize, 1);
	
	// (15) スプライン補間
	x = &temp_dbl_buf1[0];									//calloc
	memset(temp_dbl_buf1,0,datasize*sizeof(double));
	xint = &temp_dbl_buf2[0];								//calloc
	memset(temp_dbl_buf2,0,datasize*sizeof(double));
	yint = &meas_data[0];								//calloc
	memset(meas_data,0,datasize*sizeof(double));
	ydash = &temp_dbl_buf3[0];								//calloc
	memset(temp_dbl_buf3,0,datasize*sizeof(double));
	for(ii=0;ii<datasize;++ii){
		x[ii] = 0.000125f*ii;
	}
	
	// 0.000125(1/8000) -> 0.01 区間に変換 0.01 / 0.000125 = 80
	// ↑サンプリング周波数が8000Hzの場合
//	newdatasize = datasize / 80;
	// 0.00333333(1/300) -> 0.01 区間に変換 0.01 / 0.0033333 = 3
	// ↑サンプリング周波数が300Hzの場合
	newdatasize = datasize / 3;
	Spline(x, plowpassout, datasize, xint, yint, newdatasize, ydash);
	
	// この結果が (16) と一致すればここまでは正しい
	debug_out("Spl", yint, newdatasize, 1);
	memcpy(result_snore, yint, newdatasize * sizeof(double));
	result_snore_size = newdatasize;

	// (20) = yint
	
	// (21) - (34)
	getwav_pp(yint, newdatasize, 0.003f);
	
	// (35) - (47)
	getwav_apnea(yint, newdatasize, 450, 0.0015f, 0.002f);
	
	// (48) - (56)
	getwav_snore(yint, newdatasize, 0.0125f);
	
	// (57)
	
}

/************************************************************************/
/* 関数     : getwav_pp													*/
/* 関数名   : ピーク間隔演算処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void getwav_pp(const double* pData, int DSize, double Param1)
{
	int* ppeak1;
	double* ppdata1;
	double* pf1;
	int peakcnt1;
	
	int* ppeak2;
	double* ppdata2;
	double* pf2;
	int peakcnt2;
	
	double* ppdata3;
	int peakpos;
	double max;
	//double pos;
	
	int peakcnt;
	double* ppos;
	double* pinterval;
	int ppcnt;
	double* pp2;
	double* pp5;
	
	int ii;
	int jj;
	
	// (22) = Param1
	
	// (23)
	// (24) = peak1
	ppeak1 = &temp_int_buf0[0];								//calloc
	memset(temp_int_buf0,0,DSize*sizeof(int));
	ppdata1 = &temp_dbl_buf0[0];							//calloc
	memset(temp_dbl_buf0,0,DSize*sizeof(double));
	pf1 = &temp_dbl_buf1[0];								//calloc
	memset(temp_dbl_buf1,0,DSize*sizeof(double));
	peak_vallay(pData, ppeak1, DSize, 3, 1);
	peakcnt1 = peak_modify(pData, ppeak1, ppdata1, pf1, DSize, 1.0, 0.0);
	debug_out("peak1", ppdata1, peakcnt1, 1);
	debug_out("f1", pf1, peakcnt1, 1);
	
	// (25) = peakcnt2
	// (26) = f2
	// (27) = peak2
	ppeak2 = &temp_int_buf0[0];								//calloc
	memset(temp_int_buf0,0,peakcnt1*sizeof(int));
	ppdata2 = &temp_dbl_buf2[0];							//calloc
	memset(temp_dbl_buf2,0,peakcnt1*sizeof(double));
	pf2 = &temp_dbl_buf3[0];								//calloc
	memset(temp_dbl_buf3,0,peakcnt1*sizeof(double));
	peak_vallay(ppdata1, ppeak2, peakcnt1, 3, 1);
	peakcnt2 = peak_modify(ppdata1, ppeak2, ppdata2, pf2, peakcnt1, 1.0, Param1);
	debug_out("peak2", ppdata2, peakcnt2, 1);
	debug_out("f2", pf2, peakcnt2, 1);
	
	ppdata3 = &temp_dbl_buf0[0];							//calloc
	memset(temp_dbl_buf0,0,peakcnt2*sizeof(double));
	peakpos = 0;
	for(ii=0;ii<peakcnt2;++ii){
		peakpos = (int)(pf2[ii] + 0.5);
		ppdata3[ii] = pf1[peakpos] * 0.01f;
	}
	debug_out("peak3", ppdata3, peakcnt2, 1);
	
	// (28) - (30)
	max = pData[0];
	//pos = 0;
	for(ii=1;ii<DSize;++ii){
		if(max < pData[ii]){
			max = pData[ii];
			//pos = ii;
		}
	}
	
	peakcnt = peakcnt2;
	ppos = ppdata3;
	pinterval = &temp_dbl_buf1[0];							//calloc
	memset(temp_dbl_buf1,0,(peakcnt+1)*sizeof(double));
	pinterval[0] = ppos[0];
	for(ii=0;ii<peakcnt;++ii){
		pinterval[ii+1] = ppos[ii+1] - ppos[ii];
	}
	debug_out("ppinterval", pinterval, peakcnt+1, 1);
	
	// (31) - (33)
	ppcnt = 0;
	pp2 = &temp_dbl_buf2[0];								//calloc
	memset(temp_dbl_buf2,0,peakcnt*sizeof(double));
	pp5 = &temp_dbl_buf3[0];								//calloc
	memset(temp_dbl_buf3,0,peakcnt*sizeof(double));
	for(ii=1;ii<peakcnt;++ii){
		if(pinterval[ii] >= 10){
			pp2[ppcnt] = ppos[ii-1];
			pp5[ppcnt] = ppos[ii];
			ppcnt += 1;
		}
	}
	debug_out("pp2", pp2, ppcnt, 1);
	debug_out("pp5", pp5, ppcnt, 1);
	// (34)
	if(ppcnt >= 1){
		int size = (int)(pp5[ppcnt-1] + 0.5);
		int rpos = 0;
		double* presult = &temp_dbl_buf0[0];				//calloc
		memset(temp_dbl_buf0,0,size*sizeof(double));
		for(ii=0;ii<ppcnt;++ii){
			int loop2;
			int loop5;
			loop2 = (int)(pp2[ii] + 0.5) -1;
			for(jj=rpos;jj<loop2;++jj){
				presult[rpos] = 0;
				rpos+=1;
			}
			loop5 = (int)(pp5[ii] + 0.5) - 1;
			for(jj=rpos;jj<loop5;++jj){
				presult[rpos] = max;
				rpos+=1;
			}
		}
		debug_out("peakresult", presult, size, 1);
		memcpy(result_peak, presult, size * sizeof(double));
		result_peak_size = size;
	}else{
		result_peak[0] = 0x00;
		result_peak_size = 0;
	}
}

/************************************************************************/
/* 関数     : getwav_apnea												*/
/* 関数名   : 無呼吸演算処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void getwav_apnea(const double* pData, int DSize, int Param1, double Param2, double Param3)
{
	double* pave;
	double* peval2;
	int datasize;
	double* prms;
	double* ppoint;
	double* papnea;
	int ii;
	int jj;
	
	// (35) = Param1
	// (36) = Param2
	// (40) = Param3
	
	// (37)
	pave = &temp_dbl_buf0[0];								//calloc
	memset(temp_dbl_buf0,0,DSize*sizeof(double));
	for(ii=0;ii<DSize;++ii){
		int min=0;
		int loop=0;
		double ave = 0.0f;
		if(ii <= DSize-1){
			int tmp = DSize-1 - ii;
			if(tmp > Param1){
				min = Param1;
			}else{
				min = tmp;
			}
			loop = min * 2 + 1;
			min = ii - min;
			if(min < 0){
				min = 0;
			}
		}else{
			min = 0;
			loop = ii * 2 + 1;
		}
		for(jj=0;jj<loop;++jj){
			ave += pData[min+jj];
		}
		ave /= loop;
		pave[ii] = ave;
	}
	debug_out("ave", pave, DSize, 1);
	
	// (38) - (39)
	peval2 = &temp_dbl_buf1[0];								//calloc
	memset(temp_dbl_buf1,0,DSize*sizeof(double));
	for(ii=0;ii<DSize;++ii){
		if(pave[ii] >= Param2){
			peval2[ii] = 1;
		}else{
			peval2[ii] = 0;
		}
	}
	debug_out("eval2", peval2, DSize, 1);
	
	// (41) ... 使用していないため省略
	// (42)
	// (43) = prms
	datasize = DSize / 100;
	prms = &temp_dbl_buf0[0];								//calloc
	memset(temp_dbl_buf0,0,datasize*sizeof(double));
	for(ii=0;ii<datasize;++ii){
		double tmp = 0.0f;
		prms[ii] = 0.0f;
		for(jj=0;jj<100;++jj){
			tmp += (pData[ii*100 + jj]*pData[ii*100 + jj]);
		}
		tmp /= 100;
		prms[ii] = sqrt(tmp);
	}
	debug_out("RMS", prms, datasize, 1);
	
	// (44) = ppoint
	// (45) = prms
	ppoint = &temp_dbl_buf1[0];								//calloc
	memset(temp_dbl_buf1,0,datasize*sizeof(double));
	for(ii=0;ii<datasize;++ii){
		if(prms[ii] >= Param3){
			ppoint[ii] = 1;
		}else{
			ppoint[ii] = 0;
		}
	}
	debug_out("point", ppoint, datasize, 1);
	
	// (46)
	// ■暫定　データ数少なく計算失敗するので一旦削除
	papnea = &temp_dbl_buf2[0];								//calloc
	memset(temp_dbl_buf2,0,datasize*sizeof(double));
	if(datasize > 9){
		int loop = datasize - 9;
		for(ii=0;ii<9;++ii){
			papnea[ii] = 0.0f;
		}
		for(ii=0;ii<loop;++ii){
			double ave = 0.0f;
			for(jj=0;jj<9;++jj){
				ave += ppoint[ii + jj];
			}
			ave /= 9;
			if(ave == 0.0f){
				papnea[ii+9] = 1.0f;
			}else{
				papnea[ii+9] = 0.0f;
			}
		}
	}else{
		for(ii=0;ii<datasize;++ii){
			papnea[ii] = 0.0f;
		}
	}
	debug_out("apnea", papnea, datasize, 1);
	memcpy(result_apnea, papnea, datasize * sizeof(double));
	result_apnea_size = datasize;
	
}

/************************************************************************/
/* 関数     : getwav_snore												*/
/* 関数名   : いびき演算処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void getwav_snore(const double* pData, int DSize, double Param)
{
	double* pSnore;
	double* pbase_x_y2;
	double* pbase_x_y3;
	
	int cnt;
	BOOL active;
	double* p_x_y2;
	int pos;
	double* p_x_y3;
	
	double* ptime;
	
	int intervalsize;
	double* pinterval;
	
	int intervalsize2;
	double* pinterval2;
	
	double snorecnt;
	
	int ii;
	// (48) = Param
	
	// 結果[いびき]を入れる箱
	pSnore = &temp_dbl_buf0[0];								//calloc
	memset(temp_dbl_buf0,0,DSize*sizeof(double));
	pbase_x_y2 = &temp_dbl_buf1[0];							//calloc
	memset(temp_dbl_buf1,0,DSize*sizeof(double));
	pbase_x_y3 = &temp_dbl_buf2[0];							//calloc
	memset(temp_dbl_buf2,0,DSize*sizeof(double));
	
	// (49)
	for(ii=0;ii<DSize;++ii){
		if(pData[ii] < Param){
			pSnore[ii] = pData[ii];
			pbase_x_y2[ii] = 1;
			pbase_x_y3[ii] = -1;
		}else{
			pSnore[ii] = 0;
			pbase_x_y2[ii] = 0;
			pbase_x_y3[ii] = 0;
		}
	}
	
	// (50)
	for(ii=0;ii<DSize;++ii){
		if(pData[ii] > Param){
			pSnore[ii] *= (1-0.94);
		}else{
			pSnore[ii] *= (0-0.94);
		}
	}
	
	// (51)
	cnt=0;
	active = FALSE;
	for(ii=0;ii<DSize;++ii){
		if((pbase_x_y2[ii] >= 1) && (active == FALSE)){
			active = TRUE;
			cnt += 1;
		}else if(pbase_x_y2[ii] < 1){
			active = FALSE;
		}else{
			// なにもしない
		}
	}
	p_x_y2 = &temp_dbl_buf0[0];								//calloc
	memset(temp_dbl_buf0,0,cnt*sizeof(double));
	pos = 0;
	active = FALSE;
	for(ii=0;ii<DSize;++ii){
		if((pbase_x_y2[ii] >= 1) && (active == FALSE)){
			active = TRUE;
			p_x_y2[pos] = ii * 0.01;
			pos += 1;
		}else if(pbase_x_y2[ii] < 1){
			active = FALSE;
		}else{
			// なにもしない
		}
	}
	debug_out("x_y2", p_x_y2, cnt, 1);
	
	// (52)
	cnt=0;
	active = FALSE;
	for(ii=0;ii<DSize;++ii){
		if((pbase_x_y3[ii] >= 0) && (active == FALSE)){
			active = TRUE;
			cnt += 1;
		}else if(pbase_x_y3[ii] < 0){
			active = FALSE;
		}else{
			// なにもしない
		}
	}
	p_x_y3 = &temp_dbl_buf1[0];								//calloc
	memset(temp_dbl_buf1,0,((cnt+1)*sizeof(double)));
	pos = 0;
	p_x_y3[pos] = 0;
	pos += 1;
	active = FALSE;
	for(ii=0;ii<DSize;++ii){
		if((pbase_x_y3[ii] >= 0) && (active == FALSE)){
			active = TRUE;
			p_x_y3[pos] = ii * 0.01;
			pos += 1;
		}else if(pbase_x_y3[ii] < 0){
			active = FALSE;
		}else{
			// なにもしない
		}
	}
	debug_out("x_y3", p_x_y3, cnt+1, 1);
	
	// (53)
	ptime = &temp_dbl_buf2[0];								//calloc
	memset(temp_dbl_buf2,0,cnt*sizeof(double));
	for(ii=0;ii<cnt;++ii){
		ptime[ii] = p_x_y2[ii+1] - p_x_y3[ii+1];
	}
	debug_out("time", ptime, cnt, 1);
	
	// (54) pinterval[1] - pinterval[intervalsize] までが 呼吸間隔
	intervalsize = cnt;
	pinterval = &temp_dbl_buf3[0];							//calloc
	memset(temp_dbl_buf3,0,intervalsize*sizeof(double));
	for(ii=0;ii<intervalsize;++ii){
		pinterval[ii] = p_x_y2[ii+1] - p_x_y2[ii];
	}
	debug_out("interval", pinterval, intervalsize, 1);
	
	// (55)
	intervalsize2 = 0;
	for(ii=0;ii<intervalsize;++ii){
		// 3 <= x <= 5 以外の値を0にする
		if((3.0f <= pinterval[ii]) && (pinterval[ii] <= 5.0f)){
			// そのまま
			intervalsize2 += 1;
		}else{
			pinterval[ii] = 0.0f;
		}
	}
	
	pinterval2 = &temp_dbl_buf0[0];							//calloc
	memset(temp_dbl_buf0,0,intervalsize2*sizeof(double));
	pos = 0;
	for(ii=0;ii<intervalsize;++ii){
		if(pinterval[ii] != 0.0f){
			pinterval2[pos] = pinterval[ii];
			pos+=1;
		}
	}
	debug_out("interval2", pinterval2, intervalsize2, 1);
	
	// (56)
	snorecnt = intervalsize2 * 2;
	debug_out("snorecnt", &snorecnt, 1, 1);
	result_snore_count = snorecnt;

}

void Spline
(
	double	x[],		// IN:IN_X  [NP]		X 節点
	double	y[],		// IN:IN_Y  [NP]		Y 節点
	int		NP,			// IN:IN_SIZE			節点数
	double	xint[],		// OT:OT_X	[NINT+1]	X 補完値
	double	yint[],		// OT:OT_Y	[NINT+1]	Y 補完値
    int		NINT,		// IN:OT_SIZE			補完数（等分数）
	double	ydash[]		// OT:COF_Y [NP]		Y 係数（演算領域）
)
{
	double dx;
	int i;
	double rad=4.0*atan(1.0)/180.0;

	//  ======= sin 関数を補間する場合 ======
	//  境界条件の挿入
	int  bcs = 0;					//  始点は自然条件
	int  bce = 0;					//  終点は自然条件
	double yds = 0.0, yde = -1.0;   // 端末条件が自由条件につき、この値はダミー

	//  =================================
	//  全区間を NINT 等分して補間点を設定
	dx=(x[NP-1]-x[0])/NINT;
	xint[0]=x[0];
	for( i=1; i<=NINT;  i++)
	{
		xint[i]=x[0]+dx*i;  //  補間点のx座標
	}

	//  3次スプライン補間により、補間点y座標を求める
	S_Spline( x , y , NP , xint , yint , NINT+1 , bcs , yds , bce , yde , ydash );
}
/*******  3次スプライン補間関数      ************************************/
/*   入力  bcs,bce;  始点・終点での境界条件(0 = 自然条件,  1 = 固定条件)*/
/*         yds,yde;  始点・終点での固定条件挿入時の１次微係数           */
/*         x[],y[];  節点の座標 ( 要素数： n )                          */
/*         xint[];   補間点のx座標 ( 要素数： no )                      */
/*   出力  yint[];   補間点のy座標,  ydash[];  節点の１次微係数         */
/************************************* by K.Minemura ********************/
void S_Spline(double x[],double y[],int n,double xint[],double yint[],
      int no,int bcs, double yds, int bce, double yde,double ydash[])
{
    int i,j,status;
    double ai,bi,ci,di,dx,*a,*b,*c,*h;
    a=&temp_dbl_buf0[0];									//calloc
	memset(temp_dbl_buf0,0,n*sizeof(double));
    b=&temp_dbl_buf1[0];									//calloc
	memset(temp_dbl_buf1,0,n*sizeof(double));
    c=&temp_dbl_buf2[0];									//calloc
	memset(temp_dbl_buf2,0,n*sizeof(double));
    h=&temp_dbl_buf3[0];  									//calloc
	memset(temp_dbl_buf3,0,n*sizeof(double));

    for(i=0; i<n-1; i++)  h[i]=x[i+1]-x[i];
    //  始点における境界条件の挿入
    a[0] = 0.0;
    if(bcs == 0)  // 自然条件の場合
    {  
        b[0] = 2.0*h[0]; 
        c[0] = h[0]; 
        ydash[0] = 3.0*(y[1]-y[0]);
    }
    if(bcs==1)    // 固定条件の場合
    { 
        b[0] = 1.0;    
        c[0] = 0.0;  
        ydash[0] = yds; 
    }
    //  中間の節点での係数    
    for(i=1; i<n-1; i++)
    {
        a[i] = h[i];   
        b[i] = 2.0*(h[i-1]+h[i]);   
        c[i] = h[i-1];
        ydash[i]=3.0*((y[i]-y[i-1])*h[i]/h[i-1]+(y[i+1]-y[i])*h[i-1]/h[i]);
    }
    //   終点における境界条件の挿入
    c[n-1] = 0.0; 
    if(bce==0)   //  自然条件の挿入
    { 
        a[n-1] = h[n-2];   
        b[n-1] = 2.0*h[n-2];
        ydash[n-1] = 3.0*(y[n-1]-y[n-2]);
    }
    if(bce==1)   //  固定条件の挿入
    {
        a[n-1] = 0.0; 
        b[n-1] = 1.0; 
        ydash[n-1] = yde;
    }   
    //   ３重対角係数行列をもつ連立方程式の解析
    status = S_TriDiag( b, c, a, ydash, n, 0 );
    if( status == 0 ){
        //  for(i=0;i<n;i++) printf(" %d %f \n",i,ydash[i]); 
        //   補間値の計算
        for(j=0, i=0; i<n-1; i++){
            ai=(2.0*(y[i]-y[i+1])/h[i]+ydash[i]+ydash[i+1])/(h[i]*h[i]);
            bi=(3.0*(y[i+1]-y[i])/h[i]-2.0*ydash[i]-ydash[i+1])/h[i];
            ci=ydash[i];
            di=y[i]; 
	    while( xint[j] < x[i+1] )
            {
                dx=xint[j]-x[i];                   // 補間されるx座標
                yint[j]=dx*(dx*(dx*ai+bi)+ci)+di;  // 補間されたy座標
                if( j < no ){ j++;}
	    }
	    yint[no-1] = y[n-1]; 
	    //     printf(" %d %d %f \n",j,i,yint[j]);  
       }
    }
}
/*********  3重対角行列を直接法(LU分解法)により解析する関数 ********************/
/*  リターンコード：  0=正常,  9=異常                                          */
/*  入力  ：diag[]=対角要素，upp[]=上側要素,low[]=下側要素 （low[0]はダミー）  */
/*          righ[]=右辺べクトル， n=配列 diag,upp,low,righ の要素数            */
/*          dec=0 (LU分解する), dec != 0(LU分解せず、既存のLUを用いて解析する) */
/*  出力  ：righ[]=解                                                          */
/************************************* by K.Minemura ***************************/
int S_TriDiag( double diag[], double upp[], double low[], double righ[], int n, 
     int dec)
{
    int i;
    double Tiny=1.0e-8;

	for(i=0; i<n; i++)
	{
       if( diag[i] < Tiny ){ return 9;  }
    }

    if( dec == 0 )
	{   //   LU-分解  
        upp[0] /= diag[0];
        for(i=1; i<n-1; i++)
		{
            diag[i] -= upp[i-1]*low[i];
            upp[i] /= diag[i];
        }
        diag[n-1] -= upp[n-2]*low[n-1];
    }
    //    前進代入
    righ[0] /= diag[0];  
    for(i=1; i<n; i++)  righ[i] = (righ[i]-low[i]*righ[i-1])/diag[i];
    //    後退代入
    for(i=n-2; i>=0; i--)  righ[i] -= righ[i+1]*upp[i]; 
    return 0;
}

void lowpass(double input[], double output[], int size, double samplerate, double freq, double q)
{
	// フィルタ係数を計算する
	double omega = 2.0f * 3.14159265f *  freq / samplerate;
	double alpha = sin(omega) / (2.0f * q);
 
	double a0 =  1.0f + alpha;
	double a1 = -2.0f * cos(omega);
	double a2 =  1.0f - alpha;
	double b0 = (1.0f - cos(omega)) / 2.0f;
	double b1 =  1.0f - cos(omega);
	double b2 = (1.0f - cos(omega)) / 2.0f;
 
	// フィルタ計算用のバッファ変数。
	double in1  = 0.0f;
	double in2  = 0.0f;
	double out1 = 0.0f;
	double out2 = 0.0f;
	
	int i;
 
	// フィルタを適用
	for(i = 0; i < size; i++)
	{
		// 入力信号にフィルタを適用し、出力信号として書き出す。
		output[i] = b0/a0 * input[i] + b1/a0 * in1  + b2/a0 * in2
		                             - a1/a0 * out1 - a2/a0 * out2;
 
		in2  = in1;       // 2つ前の入力信号を更新
		in1  = input[i];  // 1つ前の入力信号を更新
 
		out2 = out1;      // 2つ前の出力信号を更新
		out1 = output[i]; // 1つ前の出力信号を更新
	}
}

/*==============================================================================*/
/*	bandpass																	*/
/*==============================================================================*/
/**
	float input[]  …入力信号の格納されたバッファ。
	flaot output[] …フィルタ処理した値を書き出す出力信号のバッファ。
	int   size     …入力信号・出力信号のバッファのサイズ。
	float samplerate … サンプリング周波数
	float freq … カットオフ周波数
	float bw   … 帯域幅
**/
/*==============================================================================*/
void bandpass(double input[], double output[], int size, double samplerate, double freq, double bw)
{
	// フィルタ係数を計算する
	double omega = 2.0f * 3.14159265f *  freq/samplerate;
	double alpha = sin(omega) * sinh(log(2.0f) / 2.0 * bw * omega / sin(omega));
 
	double a0 =  1.0f + alpha;
	double a1 = -2.0f * cos(omega);
	double a2 =  1.0f - alpha;
	double b0 =  alpha;
	double b1 =  0.0f;
	double b2 = -alpha;

	// フィルタ計算用のバッファ変数。
	double in1  = 0.0f;
	double in2  = 0.0f;
	double out1 = 0.0f;
	double out2 = 0.0f;
	
	int i;
 
	// フィルタを適用
	for(i = 0; i < size; i++)
	{
		// 入力信号にフィルタを適用し、出力信号として書き出す。
		output[i] = b0/a0 * input[i] + b1/a0 * in1  + b2/a0 * in2
		                             - a1/a0 * out1 - a2/a0 * out2;
 
		in2  = in1;       // 2つ前の入力信号を更新
		in1  = input[i];  // 1つ前の入力信号を更新
 
		out2 = out1;      // 2つ前の出力信号を更新
		out1 = output[i]; // 1つ前の出力信号を更新
	}
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
/* 関数     : get_result_snore										*/
/* 関数名   : いびき演算結果取得									*/
/* 引数     : double *data : いびきデータ格納先						*/
/* 戻り値   : int いびきデータ数									*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 	いびき演算結果を取得する										*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
int get_result_snore(double* data)
{
	int i;
	
	if (data == NULL) {
		return FALSE;
	}
	// 仮 ファイルデータ大きすぎるので丸める
	if (result_snore_size > DATA_SIZE) {
		result_snore_size = DATA_SIZE;
	}
	for (i = 0; i < result_snore_size; ++i) {
		data[i] = result_snore[i];
	}
	return result_snore_size;
}

/********************************************************************/
/* 関数     : get_result_peak										*/
/* 関数名   : ピーク間隔演算結果取得								*/
/* 引数     : double *data : ピーク間隔データ格納先					*/
/* 戻り値   : int ピーク間隔データ数								*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 	ピーク間隔演算結果を取得する									*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
int get_result_peak(double* data)
{
	int i;
	
	if (data == NULL) {
		return FALSE;
	}
	// 仮 ファイルデータ大きすぎるので丸める
	if (result_peak_size > DATA_SIZE) {
		result_peak_size = DATA_SIZE;
	}
	for (i = 0; i < result_peak_size; ++i) {
		data[i] = result_peak[i];
	}
	return result_peak_size;
}

/********************************************************************/
/* 関数     : get_result_apnea										*/
/* 関数名   : 無呼吸演算結果取得									*/
/* 引数     : double *data : 無呼吸判定データ格納先					*/
/* 戻り値   : int 無呼吸判定データ数								*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 	無呼吸判定演算結果を取得する									*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
int get_result_apnea(double* data)
{
	int i;
	
	if (data == NULL) {
		return FALSE;
	}
	// 仮 ファイルデータ大きすぎるので丸める
	if (result_apnea_size > BUF_SIZE_APNEA) {
		result_apnea_size = BUF_SIZE_APNEA;
	}
	for (i = 0; i < result_apnea_size; ++i) {
		data[i] = result_apnea[i];
	}
	return result_apnea_size;
}

/********************************************************************/
/* 関数     : get_result_snore_count								*/
/* 関数名   : いびき回数取得										*/
/* 引数     : なし													*/
/* 戻り値   : int いびき回数										*/
/* 変更履歴 : 2018.01.11 Axia Soft Design 和田	初版作成			*/
/********************************************************************/
/* 機能 :															*/
/* 	いびき回数を取得する											*/
/********************************************************************/
/* 注意事項 :														*/
/* なし																*/
/********************************************************************/
int get_result_snore_count()
{
	return result_snore_count;
}
/*==============================================================================*/
/* EOF */

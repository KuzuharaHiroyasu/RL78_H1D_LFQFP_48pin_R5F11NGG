/*
	極大値、極小値抽出
*/
/*==============================================================================*/
/* include																		*/
/*==============================================================================*/
#include	<stdio.h>
#include	<stdlib.h>
/*==============================================================================*/
/*
	Xは解析する信号を表す入力値の配列です。 
	データは、単一配列または連続データブロックです。
	連続したデータブロックは、大きいデータ配列またはリアルタイム処理に役に立ちます。
	リアルタイム処理では、幅の約半分のデータポイントが＋ピークまたは−ピークの位置を超えるまで±ピーク は検出されないことに注目してください。
*/
/*==============================================================================*/
/*	peak_vallay	極大値、極小値抽出												*/
/*==============================================================================*/
#define	WMAX	(100)	/* 幅最大=51											*/
/*------------------------------------------------------------------------------*/
void peak_vallay
(
	double	in[],		/* IN：入力信号の格納されたバッファ						*/
	int		ot[],		/* OT：判定結果（0=NO、1=YES)							*/
	int		size,		/* IN：入力信号・出力信号のバッファのサイズ				*/
	int		width,		/* IN：検査幅、奇数、最小は３指定						*/
	int		peak		/* IN：1=極大値抽出、else=極小処理抽出					*/
)
{
//	int		w = (width/2);
	int		i;
	int		ii;
	int		loop;
	/*--------------------------------------------------------------------------*/
	for( i = 0 ; i < size ; i++ )
	{
		ot[i] = 0;
	}
	// width = 3 -> w = 1 前提のコード
	loop = size - 1;
	for( ii=1;ii<loop;++ii){
		if( peak == 1){
			if((in[ii-1] <= in[ii]) && (in[ii] >= in[ii+1])){
				ot[ii] = 1;
			}
		}else{
			// 使用しないため、未実装
		}
	}
}
/*==============================================================================*/
/* EOF */

/**

	http://mayo.hatenablog.com/entry/2013/01/30/155054
	任意の範囲に正規化

	b = (a - vmin).astype(float) / (vmax - vmin).astype(float)
	xi(new) = xi - xmin / xmax - xmin

	import itertools
	import numpy as np

	def normalize_range(src, begin=0., end=255.):

	    amin, amax = np.amin(src), np.amax(src)

	    for y, x in itertools.product(xrange(len(src)), xrange(len(src[0]))):
	        if amin != amax:
	            src[y][x] = (src[y][x] - amin) * (end - begin) / (amax - amin) + bein
	        else:
	            src[y][x] = (end + begin) / 2.
	    return src

	http://marumo.hatenablog.com/entry/2014/01/20/002611
		FFTはすごく早い。
		結果はDFTと同じ。

	http://qiita.com/QUANON/items/188baf469fd493a2f1e6

**/
/*==============================================================================*/
/* include																		*/
/*==============================================================================*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<math.h>
#include	<float.h>
/*==============================================================================*/
#define		M_PI	((double)3.1415926535)
/*==============================================================================*/
/*	seikika																		*/
/*==============================================================================*/
void	seikika( double in[] , double ot[] , int size , double begin , double end )
{

	double amax = DBL_MAX * (-1);
	double amin = DBL_MAX;
	int i;

	for( i = 0 ; i<size ; i++ )
	{
		if( in[i] > amax )
		{
			amax = in[i];
		}
		if( in[i] < amin )
		{
			amin = in[i];
		}
	}

	for( i = 0 ; i<size ; i++ )
	{
		if( amax != amin )
		{
			ot[i] = (in[i] - amin) * (end - begin) / (amax - amin) + begin;
		}
		else
		{
			ot[i] = (end + begin) / 2.0;
		}
	}
}
/*==============================================================================*/
/* EOF */

/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : vect_tbl.c                                                    */
/* 機能         : ブートプログラム用ベクタテーブル                              */
/* 変更履歴     : 2013.01.23 Axia Soft Design K.Wada    初版作成                */
/* 注意事項     : なし                                                          */
/********************************************************************************/

#include "header.h"

/********************/
/*     内部定数     */
/********************/

//#pragma section @@CNSTL VCT_TBL AT 9F00H
#pragma section const   VCT_TBL	//far属性

/* ここにプログラムのベクタ関数を登録する */

const ST_VECT_FUNC	__far  vect_tbl[VECT_NUM_COUNT] = {
	 0x7000
,	dummy	/*	1	0x0004	INTWDTI	*/
,	dummy	/*	2	0x0006	INTLVI	*/
,	dummy	/*	3	0x0008	INTP0	*/
,	r_intc1_interrupt	/*	4	0x000A	INTP1	*/
,	dummy	/*	5	0x000C	INTP2	*/
,	r_intc3_interrupt	/*	6	0x000E	INTP3	*/
,	dummy	/*	7	0x0010	INTP4	*/
,	r_intc5_interrupt	/*	8	0x0012	INTP5	*/
,	dummy	/*	9	0x0014	INTCSI20	*/
,	dummy	/*	10	0x0014	INTIIC20	*/
,	dummy	/*	11	0x0014	INTST2	*/
,	dummy	/*	12	0x0016	INTSR2	*/
,	dummy	/*	13	0x0018	INTSRE2	*/
,	dummy	/*	14	0x001E	INTCSI00	*/
,	dummy	/*	15	0x001E	INTIIC00	*/
,	drv_uart0_snd_int	/*	16	0x001E	INTST0	*/
,	r_tau0_channel0_interrupt	/*	17	0x0020	INTTM00	*/
,	drv_uart0_rcv_int	/*	18	0x0022	INTSR0	*/
,	dummy	/*	19	0x0024	INTSRE0	*/
,	dummy	/*	20	0x0024	INTTM01H	*/
,	dummy	/*	21	0x0026	INTCSI10	*/
,	dummy	/*	22	0x0026	INTIIC10	*/
,	drv_uart1_snd_int	/*	23	0x0026	INTST1	*/
,	drv_uart1_rcv_int	/*	24	0x0028	INTSR1	*/
,	dummy	/*	25	0x002A	INTSRE1	*/
,	dummy	/*	26	0x002A	INTTM03H	*/
,	r_iica0_interrupt	/*	27	0x002C	INTIICA0	*/
,	dummy	/*	28	0x002E	INTRTIT	*/
,	dummy	/*	29	0x0032	INTTM01	*/
,	dummy	/*	30	0x0034	INTTM02	*/
,	dummy	/*	31	0x0036	INTTM03	*/
,	dummy	/*	32	0x0038	INTAD	*/
,	dummy	/*	33	0x003A	INTRTC	*/
,	r_it_interrupt	/*	34	0x003C	INTIT	*/
,	dummy	/*	35	0x0042	INTTM04	*/
,	dummy	/*	36	0x0044	INTTM05	*/
,	dummy	/*	37	0x0046	INTP6	*/
,	dummy	/*	38	0x004C	INTTM06	*/
,	dummy	/*	39	0x004E	INTTM07	*/
,	dummy	/*	40	0x0050	INTIT00	*/
,	dummy	/*	41	0x0052	INTIT01	*/
,	r_pga_dsad_conversion_interrupt	/*	42	0x005C	INTDSAD	*/
,	dummy	/*	43	0x0060	INTDSADS	*/
,	dummy	/*	44	0x0062	INTFL	*/
};


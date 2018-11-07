/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : sys_func.c													*/
/* 機能         : システム共通（関数,RAM,ROM定義）								*/
/* 変更履歴		: 2018.01.25 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/

#include "header.h"

/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/

/************************************************************/
/* ROM定義													*/
/************************************************************/

/************************************************************************/
/* 関数     : calc_sum													*/
/* 関数名   : サム値計算処理											*/
/* 引数     : *p_in		サム値計算対象の先頭データ						*/
/*          : len		サム値計算長									*/
/* 戻り値   : sum		計算後サム値									*/
/* 変更履歴 : 2012.01.30 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* チェックサム計算を行う												*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
UB calc_sum( UB *p_in, INT len )
{
	INT i;
	UB sum = CALC_SUM_INIT;
	
	for( i = 0; i < len; i++ ){
		sum += (*p_in++);
	}
	
	return sum;
}


/************************************************************************/
/* 関数     : ring_buf_init												*/
/* 関数名   : リングバッファ初期化(1バイト)								*/
/* 引数     : *p_ring	リングバッファ構造体							*/
/*          : *p_data	バッファのポインタ								*/
/*          : size		サイズ											*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.27 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* リングバッファ初期化を行う											*/
/************************************************************************/
/* 注意事項 :															*/
/* sizeはバイト数ではなく、要素の個数を指定すること　					*/
/* 実際に使用できる個数は 要素数-1個									*/
/* これは全部使ってしまうと、ロジックの上で、「rd_pos == wr_pos」		*/
/* がバッファが満杯の状態を表すのか、それともバッファが空の状態			*/
/* を表すのか、区別ができなくなるからである。							*/
/************************************************************************/
void ring_buf_init( RING_BUF* p_ring, UB* p_data, UH size)
{
	/* バッファポインタ設定 */
	p_ring->buf = p_data;
	
	/* 読み書き位置初期化 */
	p_ring->wr_pos = 0;
	p_ring->rd_pos = 0;
	
	/* サイズ設定 */
	p_ring->size = size;
}


/************************************************************************/
/* 関数     : read_ring_buf												*/
/* 関数名   : リングバッファ読み出し処理(1バイト用)						*/
/* 引数     : p_ring	リングバッファ									*/
/*          : p_data	読み出しデータ									*/
/* 戻り値   : E_OK	正常												*/
/*			: E_OBJ	オブジェクト状態エラー(データなし)					*/
/* 変更履歴 : 2012.02.13 Axia Soft Design 西島 稔	初版作成			*/
/* 			: 2012.04.09 Axia Soft Design 西島 稔	ノイズ対策(読み出し */
/*			: 後0クリア、ポインタがサイズ外の場合にポインタ初期化。		*/
/************************************************************************/
/* 機能 :																*/
/* 受信リングバッファからデータを読み出す。								*/
/************************************************************************/
/* 注意事項 :															*/
/* 実際に使用できる個数は 要素数-1個									*/
/* これは全部使ってしまうと、ロジックの上で、「rd_pos == wr_pos」		*/
/* がバッファが満杯の状態を表すのか、それともバッファが空の状態			*/
/* を表すのか、区別ができなくなるからである。							*/
/************************************************************************/
INT read_ring_buf( RING_BUF* p_ring, UB* p_data )
{
	INT	ercd = E_OK;
	UB	iflg;
	
	/* データ競合対策開始 */
	DI_RET( iflg );

	/* 異常対策 */
	if(( p_ring->rd_pos >= p_ring->size ) ||
	   ( p_ring->wr_pos >= p_ring->size )){
		p_ring->wr_pos = 0;
		p_ring->rd_pos = 0;
		ercd = E_OBJ;
	}else if( p_ring->rd_pos != p_ring->wr_pos ){	/* ライトポインタとの比較 */
		/* データ有り */
		*p_data = p_ring->buf[p_ring->rd_pos];		/* データの取得 */
		p_ring->buf[p_ring->rd_pos] = 0;			/* データを0クリア */
		/* リードポインタの更新 */
		p_ring->rd_pos = (p_ring->rd_pos + 1) % p_ring->size;
	}else{
		/* データなし */
		ercd = E_OBJ;
	}
	
	/* データ競合対策終了 */
	EI_RET( iflg );
	
	return ercd;
}

/************************************************************************/
/* 関数     : write_ring_buf											*/
/* 関数名   : 送信リングバッファ書き込み処理(1バイト用)					*/
/* 引数     : p_ring	リングバッファ									*/
/*          : data		書き込みデータ									*/
/* 戻り値   : E_QOVR	バッファオーバーフロー							*/
/*			: E_OK		正常											*/
/* 変更履歴 : 2012.02.13 Axia Soft Design 西島 稔	初版作成			*/
/* 			: 2012.04.09 Axia Soft Design 西島 稔	ノイズ対策(読み出し */
/*			: 後0クリア、ポインタがサイズ外の場合にポインタ初期化。		*/
/************************************************************************/
/* 機能 :																*/
/* 送信リングバッファに書き込みを行う。									*/
/************************************************************************/
/* 注意事項 :															*/
/* 実際に使用できる個数は 要素数-1個									*/
/* これは全部使ってしまうと、ロジックの上で、「rd_pos == wr_pos」		*/
/* がバッファが満杯の状態を表すのか、それともバッファが空の状態			*/
/* を表すのか、区別ができなくなるからである。							*/
/************************************************************************/
INT write_ring_buf( RING_BUF* p_ring ,UB data )
{
	UH	next;	/* 次の書き込み位置 */
	INT	ercd = E_OK;
	UB	iflg;
	
	/* データ競合対策開始 */
	DI_RET( iflg );

	/* 異常対策 */
	if(( p_ring->rd_pos >= p_ring->size ) ||
	   ( p_ring->wr_pos >= p_ring->size )){
		p_ring->wr_pos = 0;
		p_ring->rd_pos = 0;
	}
	
	/* 書き込み位置の計算 */
	next = ( p_ring->wr_pos + 1 ) % p_ring->size;
	
	/* リードポインタとの比較 */
	if( next == p_ring->rd_pos ){
		/* リングバッファが足りていない(送信取りこぼしが発生) */
		ercd = E_QOVR;
	}else{
		/* データの書き込み */
		p_ring->buf[p_ring->wr_pos] = data;
		/* ライトポインタの更新 */
		p_ring->wr_pos = next;
	}

	/* データ競合対策終了 */
	EI_RET( iflg );

	return ercd;
}

/************************************************************************/
/* 関数     : dummy														*/
/* 関数名   : ダミー関数												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.31 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :何もしない空関数												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void dummy( void )
{
	/* 何もしない */
}
/************************************************************************/
/* 関数     : calc_sum_32												*/
/* 関数名   : サム値計算処理(32bit型)									*/
/* 引数     : *p_in		サム値計算対象の先頭データ						*/
/*          : len		サム値計算長									*/
/* 戻り値   : sum		計算後サム値									*/
/* 変更履歴 : 2012.03.13 Axia Soft Design K.Wada	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 32bitサイズのチェックサム計算を行う									*/
/************************************************************************/
/* 注意事項 :															*/
/* RL78コンパイラには、データを64KB境界に配置できないという制限があり、 */
/* ポインタによるアクセスについても64KB境界をまたいでアクセスすることは */
/* できないため、unsingned long変数に入れて__far*キャストする。 		*/
/************************************************************************/
UW calc_sum_32( UB __far *p_in, UW len )
{
	UW i;
	UW sum = CALC_SUM_INIT_UW;
	UW addr = (UW)p_in;
	
	for( i = 0; i < len; i++ ){
		sum += (UW)(*(UB __far *)addr++);
 #ifdef PRODUCT_ONLY
		if((i & 0x00000FFF) == 0){
			wdt_refresh();			/* WDTリフレッシュ */
		}
 #endif
	}
	
	return sum;
}

/************************************************************************/
/* 関数     : calc_sum_32_any_times										*/
/* 関数名   : サム値計算処理(32bit型)									*/
/* 引数     : *p_sum	計算対象サム値									*/
/*          : *p_in		サム値計算対象の先頭データ						*/
/*          : len		サム値計算長									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.03.13 Axia Soft Design K.Wada	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 32bitサイズのチェックサム計算を行う(複数回に分けて計算する版)		*/
/************************************************************************/
/* 注意事項 :															*/
/* チェックサム本体は呼び出し側で準備すること							*/
/************************************************************************/
void calc_sum_32_any_times(UW *p_sum, UB __far *p_in, UW len )
{
	UW i;
	
	for( i = 0; i < len; i++ ){
		*p_sum += (UW)(*p_in++);
	}
}



// RD8001暫定：CPUでCRC演算が出来るか未調査
/************************************************************************/
/* 関数		: crc16														*/
/* 関数名	: CRC-16計算関数											*/
/* 引数		: *p_in		CRC値計算対象の先頭データ						*/
/*			: len		CRC値計算長										*/
/* 戻り値	: ret		計算結果										*/
/* 変更履歴	: 2009.06.19 Axia Soft Design H.Wada	初版作成			*/
/************************************************************************/
/* 機能 :	CRC-16の計算												*/
/************************************************************************/
/* 注意事項 : CRC-16のCCTIPを使用している								*/
/************************************************************************/
// CRC右送り用テーブル
const UH CRC16Table[ 256 ] = {
/* 0 */   0x0000,0x1189,0x2312,0x329b,0x4624,0x57ad,0x6536,0x74bf,
/* 8 */   0x8c48,0x9dc1,0xaf5a,0xbed3,0xca6c,0xdbe5,0xe97e,0xf8f7,
/* 16 */  0x1081,0x0108,0x3393,0x221a,0x56a5,0x472c,0x75b7,0x643e,
/* 24 */  0x9cc9,0x8d40,0xbfdb,0xae52,0xdaed,0xcb64,0xf9ff,0xe876,
/* 32 */  0x2102,0x308b,0x0210,0x1399,0x6726,0x76af,0x4434,0x55bd,
/* 40 */  0xad4a,0xbcc3,0x8e58,0x9fd1,0xeb6e,0xfae7,0xc87c,0xd9f5,
/* 48 */  0x3183,0x200a,0x1291,0x0318,0x77a7,0x662e,0x54b5,0x453c,
/* 56 */  0xbdcb,0xac42,0x9ed9,0x8f50,0xfbef,0xea66,0xd8fd,0xc974,
/* 64 */  0x4204,0x538d,0x6116,0x709f,0x0420,0x15a9,0x2732,0x36bb,
/* 72 */  0xce4c,0xdfc5,0xed5e,0xfcd7,0x8868,0x99e1,0xab7a,0xbaf3,
/* 80 */  0x5285,0x430c,0x7197,0x601e,0x14a1,0x0528,0x37b3,0x263a,
/* 88 */  0xdecd,0xcf44,0xfddf,0xec56,0x98e9,0x8960,0xbbfb,0xaa72,
/* 96 */  0x6306,0x728f,0x4014,0x519d,0x2522,0x34ab,0x0630,0x17b9,
/* 104 */ 0xef4e,0xfec7,0xcc5c,0xddd5,0xa96a,0xb8e3,0x8a78,0x9bf1,
/* 112 */ 0x7387,0x620e,0x5095,0x411c,0x35a3,0x242a,0x16b1,0x0738,
/* 120 */ 0xffcf,0xee46,0xdcdd,0xcd54,0xb9eb,0xa862,0x9af9,0x8b70,
/* 128 */ 0x8408,0x9581,0xa71a,0xb693,0xc22c,0xd3a5,0xe13e,0xf0b7,
/* 136 */ 0x0840,0x19c9,0x2b52,0x3adb,0x4e64,0x5fed,0x6d76,0x7cff,
/* 144 */ 0x9489,0x8500,0xb79b,0xa612,0xd2ad,0xc324,0xf1bf,0xe036,
/* 152 */ 0x18c1,0x0948,0x3bd3,0x2a5a,0x5ee5,0x4f6c,0x7df7,0x6c7e,
/* 160 */ 0xa50a,0xb483,0x8618,0x9791,0xe32e,0xf2a7,0xc03c,0xd1b5,
/* 168 */ 0x2942,0x38cb,0x0a50,0x1bd9,0x6f66,0x7eef,0x4c74,0x5dfd,
/* 176 */ 0xb58b,0xa402,0x9699,0x8710,0xf3af,0xe226,0xd0bd,0xc134,
/* 184 */ 0x39c3,0x284a,0x1ad1,0x0b58,0x7fe7,0x6e6e,0x5cf5,0x4d7c,
/* 192 */ 0xc60c,0xd785,0xe51e,0xf497,0x8028,0x91a1,0xa33a,0xb2b3,
/* 200 */ 0x4a44,0x5bcd,0x6956,0x78df,0x0c60,0x1de9,0x2f72,0x3efb,
/* 208 */ 0xd68d,0xc704,0xf59f,0xe416,0x90a9,0x8120,0xb3bb,0xa232,
/* 216 */ 0x5ac5,0x4b4c,0x79d7,0x685e,0x1ce1,0x0d68,0x3ff3,0x2e7a,
/* 224 */ 0xe70e,0xf687,0xc41c,0xd595,0xa12a,0xb0a3,0x8238,0x93b1,
/* 232 */ 0x6b46,0x7acf,0x4854,0x59dd,0x2d62,0x3ceb,0x0e70,0x1ff9,
/* 240 */ 0xf78f,0xe606,0xd49d,0xc514,0xb1ab,0xa022,0x92b9,0x8330,
/* 248 */ 0x7bc7,0x6a4e,0x58d5,0x495c,0x3de3,0x2c6a,0x1ef1,0x0f78
};
UH crc16( UB* p_in, int len )
{
#if 0
	while ( size != 0 ){
		//crc16 = CRC16Table2[ (unsigned char)crc16 ^ *buff ] ^ (crc16 >> CHAR_BIT);
		crc16 = (crc16 >> 8)^ CRC16Table2[ (crc16 ^ *buff++) & 0xff ] ;	//右送り用
		//buff++;
		size--;
	}
	return crc16;
#else
	UH crc16 = 0xFFFF;
	while ( len != 0 ){
		crc16 = CRC16Table[ (unsigned char)crc16 ^ *p_in ] ^ (crc16 >> CHAR_BIT);
		p_in++;
		len--;
	}
	return crc16;
#endif
}


/************************************************************************/
/* 関数     : bin2bcd													*/
/* 関数名   : バイナリ(16進)BCD変換										*/
/* 引数     : bin	変換元												*/
/* 戻り値   : BCD	変換結果		                                    */
/* 変更履歴 : 2018.07.03 Axia Soft Design 西島 稔　初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* バイナリ(16進)をBCDに変換する										*/
/************************************************************************/
/* 注意事項 :															*/
/* 99以上は変換出来ない為99として変換します								*/
/************************************************************************/
UB bin2bcd( UB bin )
{
	UB bcd;
	
	if( 99 < bin ){
		bin = 99;
	}
	
	bcd = (bin / 10) * 16;
	
	bcd += bin % 10;
	
	return bcd;
}


/************************************************************************/
/* 関数     : bcd2bin													*/
/* 関数名   : BCD(16進)バイナリ変換										*/
/* 引数     : bin      変換先											*/
/*          : src_bcd  変換元											*/
/* 戻り値   : なし				                                        */
/* 変更履歴 : 2018.07.03 Axia Soft Design 西島 稔　初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* 	1byteBCD(16進)をバイナリに変換する									*/
/************************************************************************/
/* 注意事項 :															*/
/* 	引数異常時は0を返す													*/
/************************************************************************/
INT bcd2bin( UB *bin, const UB *src_bcd )
{
	UB temp = 0;
	
	/* 上位4bitを変換 */
	temp = (UB)((*src_bcd >> 4) & 0x0F);
	if( temp < 10 ) {
		/* 10未満 */
		*bin = (UB)(temp * 10);
		
		/* 下位4bitを変換 */
		temp = (UB)(*src_bcd & 0x0F);
		if( temp < 10 ) {
			/* 10未満 */
			*bin += temp;
		} else {
			*bin = 0;
			return E_GENE;	/* 10以上の場合はエラー */
		}
	} else {
		*bin = 0;
		return E_GENE;	/* 10以上の場合はエラー */
	}
	
	return E_OK;
}

/************************************************************************/
/* 関数     : wait_ms													*/
/* 関数名   : ms待ち													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.24 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* ms待ち(正確では無い)													*/
/************************************************************************/
/* 注意事項 : 時間は正確では無い										*/
/* �@1ms≒1.10ms,2ms≒2.20msである事を確認(2018.09.11)					*/
/************************************************************************/
void wait_ms( int ms )
{
	int i,j;
	
	for(i = 0; i < ms; i++ ){
		for(j = 0; j < 100; j++ ){
			WAIT_10US()
		}
	}
}

/************************************************************/
/* END OF TEXT												*/
/************************************************************/

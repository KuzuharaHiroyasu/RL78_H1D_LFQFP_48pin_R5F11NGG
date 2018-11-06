/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー												*/
/* ファイル名   : download.c                                                    */
/* 機能         : プログラム書換処理ミドル                                      */
/* 変更履歴     : 2012.02.25 Axia Soft Design K.Wada    初版作成(RD1201)        */
/*              : 2012.12.26 Axia Soft Design K.Wada    RD1201から流用開始      */
/* 注意事項     : なし                                                          */
/********************************************************************************/

#include "header.h"

extern const UB __far ProductIdCode[PRODUCT_ID_SIZE];

/********************/
/*     内部定数     */
/********************/
/* FROMブロックサイズ */
#define	PRODUCT_PRG_BLOCK_SIZE	 	(87)
#define	BOOTLOADER_BLOCK_SIZE	 	(DRV_FROM_BLOCK_PROGRAM_NUM - PRODUCT_PRG_BLOCK_SIZE)

/* FROMブロック番号 */
#define	PRODUCT_PRG_BLOCK_NUM_TOP	(0x28)
#define	PRODUCT_PRG_BLOCK_NUM_END	(PRODUCT_PRG_BLOCK_NUM_TOP + PRODUCT_PRG_BLOCK_SIZE -1)
#define	BOOTLOADER_BLOCK_NUM_TOP	(PRODUCT_PRG_BLOCK_NUM_END +1)
#define	BOOTLOADER_BLOCK_NUM_END	(BOOTLOADER_BLOCK_NUM_TOP + BOOTLOADER_BLOCK_SIZE -1)

const UW ComBlockNumPerFromBlock = (BLOCK_SIZE / COMM_BLOCK_SIZE);	/* FROMの1ブロックが通信上の何ブロックに当たるか */

const UW ProductIdAddr = (UW)0x01FBF0;		/* 識別コード 先頭アドレス */




/* FROMアドレス定義 */
const UW ProductIdBlockNum = (UW)0x1FB;		/* 識別コードを含む通信ブロック番号 */

const UW ProductPrgAddr = (UW)0x00A000;	/* 製品用ファーム先頭アドレス */
const UW ProductPrgSize = (UW)0x015BF0;	/* 製品用ファームサイズ(87KB-識別コードサイズ) */
const UW BootLoaderAddr = (UW)0x000000;	/* ブートローダー先頭アドレス */
const UW BootLoaderSize = (UW)0x00A000;	/* ブートローダーサイズ(40KB) */

/********************/
/*     内部変数     */
/********************/

// ID用に固定のセクションを設定する必要あり！！！
//#pragma SECTION bss bss_id
//STATIC UB id_area[PRODUCT_ID_SIZE];

STATIC UB write_buf[COMM_BLOCK_SIZE];	/* 書込み用一時領域 */
STATIC UB verify_buf[VERIFY_SIZE_MAX];	/* ベリファイ用一時領域 */
STATIC UB id_temp_buf[PRODUCT_ID_SIZE];	/* 識別ID保存領域 */


/*********************/
/* プロトタイプ宣言 */
/********************/
STATIC void download_delete_id(void);
STATIC void download_make_id(UB *p_id);
STATIC BOOL download_verify(UB *addr_src , UB *addr_dest, UW size);
STATIC UW download_blocknum2addr(UW blocknum);
STATIC UW download_com_blocknum2from_blocknum(UW blocknum);

/************************************************************************/
/* 関数     : download_init                                             */
/* 関数名   : プログラム書換処理初期化処理                              */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.10.03 Axia Soft Design 西島 稔	初期化データ追加    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : プログラム書換部のデータ初期化                                */
/************************************************************************/
/* 注意事項 :                                                           */
/************************************************************************/
void download_init(void)
{
	memset( &write_buf[0], 0x00, sizeof(write_buf) );
	memset( &verify_buf[0], 0x00, sizeof(verify_buf) );
	memset( &id_temp_buf[0],0x00, sizeof(id_temp_buf) );
}

/************************************************************************/
/* 関数     : download_change_boot                                      */
/* 関数名   : ブートプログラム切り替え処理                              */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : ブートプログラムのプログラム書換え状態に切り替える            */
/************************************************************************/
/* 注意事項 : WDTを利用して自己リセットする                             */
/************************************************************************/
/* 使用するプログラム : 製品プログラム                                  */
/************************************************************************/
void download_change_boot(void)
{
	download_delete_id();

	/* WDTでリセットする */
/*	DI();	//割り込み禁止		// RD1201暫定 ここでリセットしない
/*	while(1){};	*/				// RD1201暫定 ここでリセットしない
}

/************************************************************************/
/* 関数     : download_delete_id                                        */
/* 関数名   : チェックサムを含むIDコードを削除する処理                  */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : IDコードを削除する(0を書き込む)                               */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
STATIC void download_delete_id(void)
{
	
	UB clr_src[PRODUCT_ID_SIZE] ={0};
	DRV_FROM_ERROR drv_err;
	
	drv_from_init();
	drv_from_mode_enable();
	
	//識別コード削除
	drv_err = drv_from_write( &clr_src[0],(UB __far *)ProductIdAddr, PRODUCT_ID_SIZE);
	
	drv_from_mode_disable();
}

/************************************************************************/
/* 関数     : download_start                                            */
/* 関数名   : プログラム書換開始時処理(準備)                            */
/* 引数     : なし                                                      */
/* 戻り値   : 処理結果                                                  */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 :FROMの製品プログラム領域をイレースする                         */
/************************************************************************/
/* 注意事項 : プログラム転送前に必ず1回行うこと                         */
/************************************************************************/
/* 使用するプログラム : ブート                                          */
/************************************************************************/
INT download_start(void)
{
	INT i;
	DRV_FROM_ERROR drv_err;
	INT ret = E_OK;
	
	drv_from_mode_enable();
	
	// プログラム全領域イレース
	for(i=PRODUCT_PRG_BLOCK_NUM_TOP; i<PRODUCT_PRG_BLOCK_NUM_END+1; i++){
		drv_err = drv_from_erase_block((DRV_FROM_BLOCK_NO)i);
		if(drv_err == DRV_FROM_ERR_NONE){
			ret = E_OK;
		}else{
			ret = E_GENE;
			break;
		}
	}
	
	drv_from_mode_disable();

	memset( &id_temp_buf[0],0x00, sizeof(id_temp_buf) );
	
	return ret;
}

/************************************************************************/
/* 関数     : download_set_data                                         */
/* 関数名   : プログラム書き込み処理                                    */
/* 引数     : p_data 書き込むデータのアドレス                           */
/*          : blocknum 通信上のブロック番号                             */
/*          : size 書き込むデータのサイズ (最大 COMM_BLOCK_SIZE)        */
/* 戻り値   : 処理結果                                                  */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/*          : 2013.04.09 Axia Soft Design K.Wada    チェックサム計算追加*/
/************************************************************************/
/* 機能 : プログラムデータ(HEX形式 = ROMイメージそのまま)を書き込む     */
/************************************************************************/
/* 注意事項 : 開始時処理を事前に行う必要がある(初回のみ)                */
/************************************************************************/
/* 使用するプログラム : ブート                                          */
/************************************************************************/
INT download_set_data(UB *p_data, UW blocknum, UW size, UW *p_sum)
{
	DRV_FROM_ERROR drv_err;
	INT ret = E_OK;
	UW write_addr = download_blocknum2addr(blocknum);
	UW from_blocknum = download_com_blocknum2from_blocknum(blocknum);
	
	/* パラメータチェック */
	if( (from_blocknum < PRODUCT_PRG_BLOCK_NUM_TOP)||(PRODUCT_PRG_BLOCK_NUM_END < from_blocknum) ){
		return E_GENE;
	}
	
	/* 書き換える可能性があるので、コピーを使う */
	if(size <= COMM_BLOCK_SIZE){
		memcpy(write_buf,p_data,(size_t)size);
	}else{
		return E_GENE;
	}
	
	/* 識別コード領域はdownload_finishで書くため、ここでは書かせない(0xFFを書く) */
	if( blocknum == ProductIdBlockNum ){
		memset((UB*)(write_buf + (COMM_BLOCK_SIZE-PRODUCT_ID_SIZE)) ,0xFF, PRODUCT_ID_SIZE);
	}
	
	drv_from_mode_enable();
	
	/* 書き込み */
	drv_err = drv_from_write(write_buf, (UB *)write_addr, size);
	
	drv_from_mode_disable();
	
	if(drv_err == DRV_FROM_ERR_NONE){
		/* ベリファイ */
		if(download_verify(write_buf, (UB *)write_addr, size) ){
			ret = E_OK;
			
			/* チェックサム計算 */
			/* write_bufはベリファイをクリアしているので、直接FROMを読むのと同意とする */
			if( blocknum == ProductIdBlockNum ){
				/* 識別IDの箇所はFROMに書かないので、受信データを上書きして計算する */
				memcpy((UB*)(write_buf + (COMM_BLOCK_SIZE-PRODUCT_ID_SIZE)) ,(UB*)(p_data + (COMM_BLOCK_SIZE-PRODUCT_ID_SIZE)), PRODUCT_ID_SIZE);
			}
			calc_sum_32_any_times(p_sum, write_buf, size );
			
		}else{
			ret = E_GENE;
		}
	}else{
		ret = E_GENE;
	}
	
	return ret;
}

/************************************************************************/
/* 関数     : download_finish_ready                                     */
/* 関数名   : プログラム書換終了準備処理(チェックサム計算)              */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2012.08.26 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 書換終了時にチェックサムを計算する                            */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
/* 使用するプログラム : ブート                                          */
/************************************************************************/
void download_finish_ready(void)
{
	INT ret = E_OK;
	
	download_make_id(id_temp_buf);
	
}

/************************************************************************/
/* 関数     : download_finish                                           */
/* 関数名   : プログラム書換終了時処理(チェックサム書き込み)            */
/* 引数     : なし                                                      */
/* 戻り値   : 処理結果                                                  */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 書換終了時にチェックサム・識別IDを書き込み、製品プログラムが  */
/*        存在することを示す                                            */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
/* 使用するプログラム : ブート                                          */
/************************************************************************/
INT download_finish(void)
{
	INT ret = E_OK;
	DRV_FROM_ERROR drv_err;
	
	drv_from_mode_enable();
	
	/* チェックサム・識別ID書き込み */
	drv_err = drv_from_write(&id_temp_buf[0], (UB *)ProductIdAddr, PRODUCT_ID_SIZE);

	if(drv_err == DRV_FROM_ERR_NONE){
		/* ベリファイ */
		if(download_verify(&id_temp_buf[0], (UB *)ProductIdAddr, PRODUCT_ID_SIZE) ){
			ret = E_OK;
		}else{
			ret = E_GENE;
		}
	}else{
		ret = E_GENE;
	}	
	
	drv_from_mode_disable();
	
	return ret;
}


/************************************************************************/
/* 関数     : download_make_id                                          */
/* 関数名   : チェックサム・識別ID作成処理                              */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : チェックサムを計算し・識別IDを設定する                        */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
STATIC void download_make_id(UB *p_id)
{
	UW sum = 0;
	UW * p_lwork = NULL;
	UW i;

	//IDコピー
	for(i=0; i<PRODUCT_ID_SIZE; i++){
		p_id[i] = ProductIdCode[i];
	}

	//チェックサム計算
	sum = calc_sum_32((UB *)ProductPrgAddr, ProductPrgSize );
	
	//識別コードのsize,sumを4バイト書き込み(エンディアン考慮)
	p_lwork = (UW *)(&p_id[0]);
	*(p_lwork + PRODUCT_ID_OFFSET_SIZE_PTR) = ProductPrgSize;
	*(p_lwork + PRODUCT_ID_OFFSET_SUM_PTR) = sum;

}

/************************************************************************/
/* 関数     : download_check_finish                                     */
/* 関数名   : プログラム書換チェック(チェックサム照合)                  */
/* 引数     : 転送されたチェックサム                                    */
/* 戻り値   : BOOL TRUE:チェックOK,FALSE:チェックNG                     */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : プログラムの書換え結果をチェックする                          */
/*        ・転送されたプログラム自体が正しいこと                        */
/*        ・書き込みが正しく書き込まれたこと                            */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
/* 使用するプログラム : ブート                                          */
/************************************************************************/
BOOL download_check_finish(UW com_sum)
{
	UW cal_sum = 0;
	UW memory_sum = 0;

	memory_sum = *(UW *)(ProductIdAddr + PRODUCT_ID_OFFSET_SUM );
	
	/* プログラムがFROMに正しく書き込まれたことを確認 */
	cal_sum = calc_sum_32((UB *)ProductPrgAddr, ProductPrgSize );
	if(cal_sum != memory_sum){
		return FALSE;
	}
	
	/* 転送上でプログラムに異常が発生していないことを確認 */
	if(cal_sum != com_sum){
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
/* 関数     : download_check_checksum                                   */
/* 関数名   : プログラム書換チェック(チェックサム照合)                  */
/* 引数     : なし                                                      */
/* 戻り値   : BOOL TRUE:チェックOK,FALSE:チェックNG                     */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : FROMのチェックサムをチェックする                              */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
/* 使用するプログラム : 製品プログラム                                  */
/************************************************************************/
BOOL download_check_checksum(void)
{
	UW cal_sum = 0;
	UW memory_sum = 0;

	memory_sum = *(UW *)(ProductIdAddr + PRODUCT_ID_OFFSET_SUM );
	
	/* プログラムがFROMに正しく書き込まれたことを確認 */
	cal_sum = calc_sum_32((UB *)ProductPrgAddr, ProductPrgSize );
	if(cal_sum != memory_sum){
		return FALSE;
	}
	
	return TRUE;
}

/************************************************************************/
/* 関数     : download_check_product                                    */
/* 関数名   : 製品プログラムチェック(ID照合)                            */
/* 引数     : なし                                                      */
/* 戻り値   : BOOL TRUE:存在する,FALSE:存在しない                       */
/* 変更履歴 : 2012.03.10 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 製品プログラムが存在することを確認する                        */
/************************************************************************/
/* 注意事項 : 起動時間の短縮のため、チェックサムチェックは行わず、      */
/*            識別IDだけで判定を行う                                    */
/************************************************************************/
/* 使用するプログラム : ブート                                          */
/************************************************************************/
BOOL download_check_product(void)
{
	INT i;
	
	//識別コード全領域取得
	UB * p_id_data = (UB *)ProductIdAddr;
	UB id_buf[PRODUCT_ID_SIZE];
	
	for(i=0; i<PRODUCT_ID_SIZE; i++){
		id_buf[i] = *p_id_data++;
	}
	
	//識別IDチェック
	for(i=PRODUCT_ID_OFFSET_ID; i<PRODUCT_ID_SIZE; i++){
		if(id_buf[i] != ProductIdCode[i]){
			return FALSE;	/* 製品プログラムが存在しない */
		}
	}
	return TRUE;	/* 製品プログラムが存在する */
}

/************************************************************************/
/* 関数     : download_get_data                                         */
/* 関数名   : プログラム読み出し                                        */
/* 引数     : p_get_data 取得したデータの格納アドレス                   */
/*          : blocknum 読み出したい通信上のブロック番号                 */
/*          : size 読み込むデータのサイズ                               */
/* 戻り値   : 処理結果                                                  */
/* 変更履歴 : 2012.03.10 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 自身のプログラムを読み出す                                    */
/************************************************************************/
/* 注意事項 :                                                           */
/************************************************************************/
/* 使用するプログラム : 製品プログラム                                  */
/************************************************************************/
INT download_get_data(UB *p_get_data,UW blocknum ,UW size)
{
	DRV_FROM_ERROR drv_err;
	INT ret = E_OK;
	UW read_addr = download_blocknum2addr(blocknum);
	
	/* 読み込み */
	drv_err = drv_from_read((UB *)read_addr, p_get_data, size);
	if(drv_err == DRV_FROM_ERR_NONE){
		ret = E_OK;
	}else{
		ret = E_GENE;
	}
	
	return ret;
}

/************************************************************************/
/* 関数     : download_verify                                           */
/* 関数名   : ベリファイ処理                                            */
/* 引数     : なし                                                      */
/* 戻り値   : TRUE:一致 / FALSE:不一致                                  */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 書き込もうとしたデータと書き込んだ結果が一致すること          */
/************************************************************************/
/* 注意事項 : 最大サイズ制限あり                                        */
/************************************************************************/
STATIC BOOL download_verify(UB *addr_src , UB *addr_dest, UW size)
{
	DRV_FROM_ERROR drv_err;
	UW i;
	
	/* サイズチェック */
	if(size > VERIFY_SIZE_MAX){
		return FALSE;
	}
	
	/* 読み込み */
	drv_err = drv_from_read(addr_dest, &verify_buf[0], size);
	if(drv_err != DRV_FROM_ERR_NONE){
		return FALSE;
	}
	
	/* ベリファイ */
	for(i=0; i<size; i++){
		if(*addr_src++ != verify_buf[i]){
			return FALSE;
		}
	}
	
	return TRUE;
}

/************************************************************************/
/* 関数     : download_blocknum2addr                                    */
/* 関数名   : ブロック番号→FROMアドレス変換処理                        */
/* 引数     : ブロック番号                                              */
/* 戻り値   : 変換後アドレス                                            */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 通信上のブロック番号をFROMのアドレスに変換する                */
/************************************************************************/
/* 注意事項 : ここでのブロック番号はFROMブロック図のブロック番号とは違う*/
/************************************************************************/
STATIC UW download_blocknum2addr(UW blocknum)
{
	return (UW)(blocknum * COMM_BLOCK_SIZE);
}

/************************************************************************/
/* 関数     : download_com_blocknum2from_blocknum                       */
/* 関数名   : 通信ブロック番号→FROMブロック番号変換処理                */
/* 引数     : 通信ブロック番号                                          */
/* 戻り値   : FROMブロック番号                                          */
/* 変更履歴 : 2012.01.21 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 通信上のブロック番号をFROMのブロック番号に変換する            */
/************************************************************************/
/* 注意事項 :                                                           */
/************************************************************************/
STATIC UW download_com_blocknum2from_blocknum(UW blocknum)
{
	return (UW)((blocknum / ComBlockNumPerFromBlock));
}

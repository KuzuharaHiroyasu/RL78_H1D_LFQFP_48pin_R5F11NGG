/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー												*/
/* ファイル名   : drv_from.c													*/
/* 機能         : FROMの読み書き												*/
/* 変更履歴     : 2012.02.18 Axia Soft Design K.Wada	初版作成(RD1201)		*/
/*              : 2012.12.26 Axia Soft Design K.Wada    RD1201から流用開始      */
/* 注意事項     : なし															*/
/********************************************************************************/

#include "./header.h"

/******************/
/* 定数マクロ定義 */
/******************/
#define DUMMY_DATA      0xFF        /* ダミー書き込みデータ         */

/*------------------------------------------------------------------------------*/
/* external reference parameter                                                 */
/*------------------------------------------------------------------------------*/
/* データ・バッファ本体 */
fsl_u08 fsl_data_buffer[ WRITE_SIZE_BYTE ];

#define	FSL_DESCRIPTOR1_FULL_SPEED_MODE		0x00
#define	FSL_DESCRIPTOR1_WIDE_VOLTAGE_MODE	0x01

#define	FSL_DESCRIPTOR2_MAIN_CLOCK		12	//動作周波数[MHz]

#define	FSL_DESCRIPTOR3_STATES_CHECK_USER_MODE		0x00
#define	FSL_DESCRIPTOR3_STATES_CHECK_INTERNAL_MODE		0x01


/* 動作クロック 内部12MHz設定 */
const __far fsl_descriptor_t fsl_descriptor_pstr = { 	FSL_DESCRIPTOR1_FULL_SPEED_MODE,
														FSL_DESCRIPTOR2_MAIN_CLOCK, 
														FSL_DESCRIPTOR3_STATES_CHECK_INTERNAL_MODE };


/********************/
/*     内部変数     */
/********************/
//STATIC DRV_FROM_STATUS	s_drv_from_status;

/*********************/
/* プロトタイプ宣言 */
/********************/
STATIC BOOL drv_from_is_true_area(UW addr,UW *p_get_block_no);
STATIC BOOL drv_from_is_true_block_no(INT no);
STATIC UW   drv_from_addr2blocknum(UW pFrom);

/************************************************************************/
/* 関数     : drv_from_init                                             */
/* 関数名   : FROMドライバ初期化処理                                    */
/* 引数     : なし                                                      */
/* 戻り値   :  															*/
/* 変更履歴 : 2012.02.18 Axia Soft Design K.Wada	初版作成(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 :                                                               */
/************************************************************************/
/* 注意事項 :                                                           */
/************************************************************************/
void drv_from_init(void)
{
DRV_FROM_ERROR	ercd = DRV_FROM_ERR_NONE;

    fsl_u08 dubRetCode;

    /* フラッシュ・セルフ・プログラミング・ライブラリ初期化実行 */
    dubRetCode = FSL_Init( &fsl_descriptor_pstr );
    
    /* 初期化が正常に終了した場合 */
    if( dubRetCode == FSL_OK )
    {
        // ログを出力する
    }
}

/************************************************************************/
/* 関数     : drv_from_mode_enable                                      */
/* 関数名   : CPU書き換えモード開始設定処理                             */
/* 引数     : なし                                                      */
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.12.21 Axia Soft Design K.Wada	初版作成(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 書き換えモード開始設定                                        */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void drv_from_mode_enable(void)
{
	DI();
	
    /* フラッシュ・セルフ・プログラミング・ライブラリ開始処理 */
    FSL_Open();
    
    /* RAM実行が必要なフラッシュ関数(標準書き換え関数)を使用できるように準備 */
    FSL_PrepareFunctions();
}

/************************************************************************/
/* 関数     : drv_from_mode_disable                                     */
/* 関数名   : CPU書き換えモード終了設定処理                             */
/* 引数     : なし                                                      */
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.12.21 Axia Soft Design K.Wada	初版作成(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 書き換えモード終了設定                                        */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void drv_from_mode_disable(void)
{
    /* フラッシュ・セルフ・プログラミング・ライブラリ終了処理 */
    FSL_Close();

	EI();
}

/************************************************************************/
/* 関数     : drv_from_read                                             */
/* 関数名   : FROMサイズ指定読み込み処理                                */
/* 引数     : *addr_src ソースアドレス				                    */
/*			: *addr_dest	あて先アドレス								*/
/*			: size	データサイズ										*/
/* 戻り値   : DRV_FROM_ERR_NONE 	正常								*/
/*			: DRV_FROM_ERR_PARAM 	パラメータ異常						*/
/* 変更履歴 : 2012.02.18 Axia Soft Design K.Wada	初版作成(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : FROMサイズ指定読み込み                                        */
/************************************************************************/
/* 注意事項 : 読み込みデータの保存領域は呼び出し側で確保すること        */
/************************************************************************/
DRV_FROM_ERROR drv_from_read(UB  __far  *addr_src , UB  __far  *addr_dest, UW size)
{
	DRV_FROM_ERROR	ercd = DRV_FROM_ERR_NONE;
	UW block_num = DRV_FROM_BLOCK_NUM;
	
	/* パラメータチェック */
	if(size == 0){
		return DRV_FROM_ERR_PARAM;
	}
	
	if( (!drv_from_is_true_area((UW)     addr_src         , &block_num)) 
	  ||(!drv_from_is_true_area((UW)((UW)addr_src+size -1), &block_num)) ){
		return DRV_FROM_ERR_PARAM;
	}
	
	_COM_memcpy_ff( addr_dest, addr_src, (size_t)size);
	
	return ercd;
}

/************************************************************************/
/* 関数     : drv_from_write                                            */
/* 関数名   : FROMサイズ指定書き込み処理                                */
/* 引数     : *addr_src ソースアドレス				                    */
/*			: *addr_dest	あて先アドレス								*/
/*			: size	データサイズ										*/
/* 戻り値   : DRV_FROM_ERR_NONE 	正常							    */
/*			: DRV_FROM_ERR_PARAM 	パラメータ異常						*/
/*			: DRV_FROM_ERR_PROGRAM	プログラムエラー					*/
/* 変更履歴 : 2012.02.18 Axia Soft Design K.Wada	初版作成(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : FROMサイズ指定書き込み                                        */
/************************************************************************/
/* 注意事項 : 256バイトまで書き込み可									*/
/************************************************************************/
DRV_FROM_ERROR drv_from_write(UB __far *addr_src ,  const UB __far *addr_dest, UW size)
{
	DRV_FROM_ERROR	ercd = DRV_FROM_ERR_NONE;
	
    fsl_u08     dubRetCode = FSL_OK;/* リターン・コード                   */
	
	UW write_addr = (UW)addr_dest;
	UW offset;
	UW write_unit_size;
	UW block_num_top = DRV_FROM_BLOCK_NUM;
	UW block_num_end = DRV_FROM_BLOCK_NUM;
	
	/* ライブラリ書き込み用データ */
	__near fsl_write_t dtyWriteBuff;
	
	/* ワードアラインへのオフセット */
	offset = write_addr % WORD_SIZE;
	
	/* パラメータチェック */
	/* オフセット含めて書き込みする */
	/* つまりアドレスがワードアライン合っている場合のみMax256バイト書ける */
	if( (offset + size > WRITE_SIZE_BYTE) || (size == 0)){
		return DRV_FROM_ERR_PARAM;
	}
	if( (!drv_from_is_true_area((UW)     addr_dest         , &block_num_top)) 
	  ||(!drv_from_is_true_area((UW)((UW)addr_dest+size -1), &block_num_end)) ){
		return DRV_FROM_ERR_PARAM;
	}
	
	/* データ・バッファ初期化 */
//    memset( fsl_data_buffer,DUMMY_DATA,WRITE_SIZE_BYTE);			// 0xFF埋め
	_COM_memcpy_ff( fsl_data_buffer, addr_dest-offset, WRITE_SIZE_BYTE);	// 元データ埋め
    /* 書き込みデータをデータ・バッファにコピー */
	_COM_memcpy_ff( fsl_data_buffer + offset, addr_src, (size_t)size);
	
	/* 書き込みサイズ調整(ワード拡張) */
	write_unit_size = (UW)((offset + size) / WORD_SIZE);	// 切り捨て
	if((offset + size) % WORD_SIZE != 0){
		write_unit_size += 1;	// 切り上げ
	}
	
	/* パラメータを設定 */
	dtyWriteBuff.fsl_data_buffer_p_u08       = (fsl_u08 __near *)fsl_data_buffer;
	dtyWriteBuff.fsl_destination_address_u32 = write_addr - offset;
	dtyWriteBuff.fsl_word_count_u08          = (fsl_u08)( write_unit_size );

	/* 書き込み実行 */
	wdt_refresh();
	dubRetCode = FSL_Write( &dtyWriteBuff );
	wdt_refresh();
	
	/* ベリファイ */
	if( dubRetCode == FSL_OK ){
		// (注意事項)
		// ベリファイは1ブロックのみ行うので，複数のブロックをベリファイする場合は，
		// FSL_IVerifyを複数回呼び出す
        dubRetCode = FSL_IVerify( (fsl_u16)block_num_top );
		if(dubRetCode == FSL_OK){
			if(block_num_top != block_num_end){
				dubRetCode = FSL_IVerify( (fsl_u16)block_num_end );
			}
		}
    }
	/* エラー判定 */
	if(dubRetCode != FSL_OK){
		ercd = DRV_FROM_ERR_PROGRAM;
	}
	
	return ercd;

}

/************************************************************************/
/* 関数     : drv_from_erase_block                                      */
/* 関数名   : FROM指定ブロック消去処理                                  */
/* 引数     : block ブロック番号				                        */
/* 戻り値   : DRV_FROM_ERR_NONE		正常								*/
/*			: DRV_FROM_ERR_PARAM 	パラメータ異常						*/
/*			: DRV_FROM_ERR_ERASE	イレーズエラー						*/
/* 変更履歴 : 2012.02.18 Axia Soft Design K.Wada	初版作成(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : FROM指定ブロック消去                                          */
/************************************************************************/
/* 注意事項 :                                                           */
/************************************************************************/
DRV_FROM_ERROR drv_from_erase_block(DRV_FROM_BLOCK_NO block)
{
	DRV_FROM_ERROR	ercd = DRV_FROM_ERR_NONE;
	fsl_u08 dubRetCode;         /* リターン・コード                   */
	fsl_u08 block_no = (fsl_u08)block;
		
	/* パラメータチェック */
	if(! drv_from_is_true_block_no(block)){
		return DRV_FROM_ERR_PARAM;
	}
	
	/* 対象ブロックの状態を確認 */
	dubRetCode = FSL_BlankCheck( block_no );
	
	if( dubRetCode == FSL_OK ){
		/* ブランク状態 */
		ercd = DRV_FROM_ERR_NONE;
	}else if( dubRetCode == FSL_ERR_BLANKCHECK ){
		/* ブランクではない状態 */
		wdt_refresh();
	    dubRetCode = FSL_Erase( block_no );
		wdt_refresh();
		if( dubRetCode != FSL_OK ){
			ercd = DRV_FROM_ERR_ERASE;
		}
	}else{
		/* 内部エラー */
		ercd = DRV_FROM_ERR_ERASE;
	}

	return ercd;
}

/************************************************************************/
/* 関数     : drv_from_is_true_area                                     */
/* 関数名   : アドレスチェック処理                                      */
/* 引数     : addr	調査したいアドレス                                  */
/*          : p_get_block_no	取得するブロック番号					*/
/* 戻り値   : TRUE	ブロック範囲内 										*/
/*			: FALSE	ブロック範囲外										*/
/* 変更履歴 : 2012.02.18 Axia Soft Design K.Wada	初版作成(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 指定したアドレスが書込・読込できるFROM領域かチェックする      */
/*      : ブロック0番はベクタテーブルなどがあるため、触らないようにする */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
STATIC BOOL drv_from_is_true_area(UW addr,UW *p_get_block_no)
{
	DRV_FROM_BLOCK_NO i = DRV_FROM_BLOCK_NUM;
	
	for(i=DRV_FROM_BLOCK_PROGRAM_TOP; i<DRV_FROM_BLOCK_PROGRAM_END+1; i++){
		if( (i * BLOCK_SIZE <= addr)
		  &&(addr <= ((i+1) * BLOCK_SIZE -1)) ){
		  	*p_get_block_no = i;
			return TRUE;
		}
	}
	return FALSE;
}

/************************************************************************/
/* 関数     : drv_from_is_true_block_no                                 */
/* 関数名   : ブロック番号チェック処理                                  */
/* 引数     : no	調査したいブロック番号                              */
/* 戻り値   : TRUE	ブロック範囲内 										*/
/*			: FALSE	ブロック範囲外										*/
/* 変更履歴 : 2012.02.18 Axia Soft Design K.Wada	初版作成(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : 指定したブロックが書込・読込できるFROM領域かチェックする      */
/*      : ブロック0番はベクタテーブルなどがあるため、触らないようにする */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
STATIC BOOL drv_from_is_true_block_no(INT no)
{
	if( (DRV_FROM_BLOCK_PROGRAM_TOP <= no) && (no <= DRV_FROM_BLOCK_PROGRAM_END) ){
		return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/* 関数     : drv_from_addr2blocknum                                    */
/* 関数名   : FROMアドレス→ブロック番号変換処理                        */
/* 引数     : FROMアドレス                                              */
/* 戻り値   : 変換後ブロック番号                                        */
/* 変更履歴 : 2012.02.25 Axia Soft Design K.Wada	初版作成(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    初版作成(RD1215)    */
/************************************************************************/
/* 機能 : FROMのアドレスをFROMのブロック番号に変換する                  */
/************************************************************************/
/* 注意事項 : ここでのブロック番号はFROMブロック図のブロック番号        */
/************************************************************************/
STATIC UW drv_from_addr2blocknum(UW pFrom)
{
	UW block_num = DRV_FROM_BLOCK_NUM;
	
	drv_from_is_true_area((UW)pFrom, &block_num);
	
	return block_num;
}

/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : drv_from.h													*/
/* 機能         : FROMの読み書き												*/
/* 変更履歴     : 2012.12.21 Axia Soft Design K.Wada	初版作成				*/
/*              : 2012.12.26 Axia Soft Design K.Wada    RD1201から流用開始      */
/*              : 2018.04.01 Axia Soft Design S.Shima   RD1215から流用開始      */
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef		_DRV_FROM_H_			/* 二重定義防止 */
#define		_DRV_FROM_H_


/* フラッシュ・セルフ・プログラミング・ライブラリ */
#include "fsl.h"                /* ライブラリ・ヘッダーファイル             */
#include "fsl_types.h"          /* ライブラリ・ヘッダーファイル             */

/******************/
/* 定数マクロ定義 */
/******************/
/* 閾値 */
#define		DRV_FROM_ERASE_TIME_MAX				1000	/* 10s(10ms * DRV_FROM_ERASE_TIME_MAX)	 */
#define		DRV_FROM_CMD_RO_CNT					20000	/* リトライアウト回数					 */
#define		DRV_FROM_ERASE_CNT					3		/* ハードウェアマニュアルより指定		 */

/* 書き込み単位 */
#define 	WRITE_SIZE_WORD						(64)
#define 	WORD_SIZE							(4)		/* 標準ワードサイズ */
#define 	WRITE_SIZE_BYTE						(WRITE_SIZE_WORD*WORD_SIZE)

#define		BLOCK_SIZE							(0x400l) /* 標準ブロック・サイズ         */

/******************/
/*     型定義     */
/******************/
typedef enum  {
	// コードフラッシュ・メモリ:128KB
	DRV_FROM_BLOCK_NUM               = 0x80,	/* 全体 */
	DRV_FROM_BLOCK_BOOTCLUSTER0_SIZE = 0x04,	/* ブートクラスタ0:4KB */
	DRV_FROM_BLOCK_BOOTCLUSTER1_SIZE = 0x04,	/* ブートクラスタ1:4KB */
	DRV_FROM_BLOCK_BOOTCLUSTER_SIZE  = DRV_FROM_BLOCK_BOOTCLUSTER0_SIZE + DRV_FROM_BLOCK_BOOTCLUSTER1_SIZE,

	DRV_FROM_BLOCK_TOP               = 0x00,
	DRV_FROM_BLOCK_END               = DRV_FROM_BLOCK_TOP + DRV_FROM_BLOCK_NUM -1,
	
	DRV_FROM_BLOCK_BOOTCLUSTER0_TOP  = 0x00,
	DRV_FROM_BLOCK_BOOTCLUSTER0_END  = DRV_FROM_BLOCK_BOOTCLUSTER0_TOP + DRV_FROM_BLOCK_BOOTCLUSTER0_SIZE -1,
	DRV_FROM_BLOCK_BOOTCLUSTER1_TOP  = DRV_FROM_BLOCK_BOOTCLUSTER0_END + 1,
	DRV_FROM_BLOCK_BOOTCLUSTER1_END  = DRV_FROM_BLOCK_BOOTCLUSTER1_TOP + DRV_FROM_BLOCK_BOOTCLUSTER1_SIZE -1,

	DRV_FROM_BLOCK_PROGRAM_TOP       = 0x01,
	DRV_FROM_BLOCK_PROGRAM_END       = DRV_FROM_BLOCK_NUM - 1,
	
	DRV_FROM_BLOCK_PROGRAM_NUM       = DRV_FROM_BLOCK_NUM - DRV_FROM_BLOCK_BOOTCLUSTER_SIZE
}DRV_FROM_BLOCK_NO;

/* FROMエラーコード */
typedef enum  {
	DRV_FROM_ERR_NONE = 0,		/* エラーなし */
	DRV_FROM_ERR_ERASE,			/* イレースエラー */
	DRV_FROM_ERR_PROGRAM,		/* プログラムエラー */
	DRV_FROM_ERR_PARAM,			/* 指定パラメータ異常 */
	DRV_FROM_ERR_STATUS_ERROR	/* 内部ステータスエラー */
}DRV_FROM_ERROR;

/******************/
/*  外部参照宣言  */
/******************/
extern void				drv_from_init(void);
extern void				drv_from_mode_enable(void);
extern void				drv_from_mode_disable(void);
extern DRV_FROM_ERROR	drv_from_read(UB  __far *, UB  __far *, UW );
extern DRV_FROM_ERROR	drv_from_write(UB __far  *addr_src ,const  UB __far  *addr_dest, UW size);
extern DRV_FROM_ERROR	drv_from_erase_block(DRV_FROM_BLOCK_NO );


/******************/
/* 関数マクロ定義 */
/******************/

#endif /*_DRV_FROM_H_ */

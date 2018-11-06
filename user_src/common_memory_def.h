/********************************************************************************/
/* システム名   : RD1215 共有メモリ領域定義										*/
/* ファイル名   : common_memory_def.h											*/
/* 機能         : ブート、製品プログラム共有のメモリ領域定義					*/
/* 変更履歴     : 2012.12.21 Axia Soft Design K.Wada	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef		_COMMON_MEMORY_DEF_H_			/* 二重定義防止 */
#define		_COMMON_MEMORY_DEF_H_

//#include "lamp_iobox_common_def.h"

/******************/
/* 定数マクロ定義 */
/******************/
#define PRODUCT_ID_SIZE				(16)
#define PRODUCT_ID_OFFSET_SIZE		(0)
#define PRODUCT_ID_OFFSET_SUM		(4)
#define PRODUCT_ID_OFFSET_ID		(8)
#define PRODUCT_ID_OFFSET_SIZE_PTR	(PRODUCT_ID_OFFSET_SIZE / sizeof(UW))	/* ポインタ用 */
#define PRODUCT_ID_OFFSET_SUM_PTR	(PRODUCT_ID_OFFSET_SUM / sizeof(UW))	/* ポインタ用 */
#define PRODUCT_ID_OFFSET_ID_PTR	(PRODUCT_ID_OFFSET_ID / sizeof(UW))		/* ポインタ用 */

#define COMM_BLOCK_SIZE				(256)	/* 通信上の1ブロックのデータサイズ(単位：バイト) */
#define VERIFY_SIZE_MAX				COMM_BLOCK_SIZE

#define BLOCK_NUM_MIN				(IOBOX_BLOCK_NUM_MIN)			/* ブロック番号対応アドレス : 0x000400～ 256バイト分 */
#define BLOCK_NUM_MAX				(IOBOX_BLOCK_NUM_MAX)			/* ブロック番号対応アドレス : 0x01FB00～ 256バイト分 */
#define BLOCK_NUM_CNT				(IOBOX_BLOCK_NUM_CNT)

#define BLOCK_NUM_MIN_UPDATE		(IOBOX_BLOCK_NUM_MIN_UPDATE)	/* 書換え対象: 0x00A000～0x01FBFF  */
#define BLOCK_NUM_MAX_UPDATE		(IOBOX_BLOCK_NUM_MAX_UPDATE)	/*             87KB分 */
#define BLOCK_NUM_CNT_UPDATE		(IOBOX_BLOCK_NUM_CNT_UPDATE)

//RD1201とブートとアプリの順番が変わったので、本マクロの意味合いが逆になっているので注意すること
//#define BLOCK_NUM_NOT_WRITE			(0x00A0)

#define FIX_VECT_TBL_SIZE			(128)
/* 製品プログラムベクタテーブル先頭 (!注意!)vect_tbl.cの VCT_TBL の配置と一致させること */
//#define PRODUCT_VECT_TBL_HEAD_ADDR	(0x01FEF0) /* 後ろに付けるが、ID10h,ベクタテーブル100h分上を指す */
#define PRODUCT_VECT_TBL_HEAD_ADDR	(0x01FAF0) /* 後ろに付けるが、ID10h,ベクタテーブル100h分上を指す */

#endif /*_COMMON_MEMORY_DEF_H_ */

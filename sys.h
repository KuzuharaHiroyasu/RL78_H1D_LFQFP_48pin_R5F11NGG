/********************************************************************************/
/* システム名   : RD8001														*/
/* ファイル名   : sys.h															*/
/* 機能         : システム共通(マクロ定義、型定義、関数の外部参照宣言)			*/
/* 変更履歴     : 2017.12.20 Axia Soft Design 西島		初版差					*/
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef		_SYS_H_			/* 二重定義防止 */
#define		_SYS_H_

/************************************************************/
/* マクロ													*/
/************************************************************/
/* 汎用定義 */

#define		OFF					(0)							/* オフ					*/
#define		ON					(1)							/* オン					*/
#define		UNKNOWN				(2)							/* 状態不明 ※初期状態	*/

#define		OK					(0)							/* 正常					*/
#define		NG					(1)							/* 異常					*/

#define		TRUE				(1)							/* 真					*/
#define		FALSE				(0)							/* 偽					*/

#define		VALID				(0)							/* 有効					*/
#define		INVALID				(-1)						/* 無効					*/

#define		LOW					(0)							/* ポート生データLOW	*/
#define		HIGH				(1)							/* ポート生データHIGH	*/

#define		ACT_L				(0)							/* ポート論理(LOW)	*/
#define		ACT_H				(1)							/* ポート論理(HIGH)	*/

#define		SET					(1)							/* セット	*/
//#define		CLR					(0)							/* クリア	*/

/* エッジ定義(論理) */
#define		EDGE_NON				0	/*エッジなし*/
#define		EDGE_ON					1	/*ONエッジ*/
#define		EDGE_OFF				2	/*OFFエッジ*/
#define		EDGE_ON_ONESHOT			3	/*ONエッジ(ワンショット)*/
#define		EDGE_OFF_ONESHOT		4	/*OFFエッジ(ワンショット)*/

/* マイコンビット定義 */
			/* 端子の入出力方向設定*/
#define		PORT_DIR_INPUT		(0)							/* ポート入力設定	*/
#define		PORT_DIR_OUTPUT		(1)							/* ポート出力設定	*/
			/* 端子機能設定 */
#define		PORT_PMR_NORMAL_IO	(0)							/* 端子機能 汎用IO	*/
#define		PORT_PMR_FUNC		(1)							/* 端子機能 周辺機能	*/
			/* ポートのエッジ方向設定*/
#define		PORT_EDGE_UP		(1)							/* ポート立上りエッジ設定	*/
#define		PORT_EDGE_DOWN		(0)							/* ポート立下りエッジ設定	*/
#define		PORT_EDGE_BOTH		PORT_EDGE_DOWN				/* (両エッジ選択時は立下りに設定しておく)	*/
			/* ポートの割り込みセンス設定*/
#define		PORT_SENSE_EDGE		(0)							/* ポートエッジセンス	*/
#define		PORT_SENSE_LEVEL	(1)							/* ポートレベルセンス	*/
			/* ポートエッジの割り込み要因設定*/
#define		INT_FACTOR_ONE_EDGE		(0)						/* 片エッジ設定	*/
#define		INT_FACTOR_BOTH_EDGE	(1)						/* 両エッジ設定	*/
#define		INT_FACTOR_LEVEL		(0)						/* (レベルセンス選択時は0に設定しておく)	*/
			/* 個別割り込みの禁止/許可設定値*/
#define		ENABLE				(1)							/* 許可					*/
#define		DISABLE				(0)							/* 禁止					*/
			/* 主にタイマのカウント開始/停止に使用する*/
#define		REG_CNT_START				(1)					/* 開始	*/
#define		REG_CNT_STOP				(0)					/* 停止	*/


/* ビット定義 */
#define		BIT00				(0x00000001)				/* bit0					*/
#define		BIT01				(0x00000002)				/* bit1					*/
#define		BIT02				(0x00000004)				/* bit2					*/
#define		BIT03				(0x00000008)				/* bit3					*/
#define		BIT04				(0x00000010)				/* bit4					*/
#define		BIT05				(0x00000020)				/* bit5					*/
#define		BIT06				(0x00000040)				/* bit6					*/
#define		BIT07				(0x00000080)				/* bit7					*/
#define		BIT08				(0x00000100)				/* bit8					*/
#define		BIT09				(0x00000200)				/* bit9					*/
#define		BIT10				(0x00000400)				/* bit10				*/
#define		BIT11				(0x00000800)				/* bit11				*/
#define		BIT12				(0x00001000)				/* bit12				*/
#define		BIT13				(0x00002000)				/* bit13				*/
#define		BIT14				(0x00004000)				/* bit14				*/
#define		BIT15				(0x00008000)				/* bit15				*/
#define		BIT16				(0x00010000)				/* bit16				*/
#define		BIT17				(0x00020000)				/* bit17				*/
#define		BIT18				(0x00040000)				/* bit18				*/
#define		BIT19				(0x00080000)				/* bit19				*/
#define		BIT20				(0x00100000)				/* bit20				*/
#define		BIT21				(0x00200000)				/* bit21				*/
#define		BIT22				(0x00400000)				/* bit22				*/
#define		BIT23				(0x00800000)				/* bit23				*/
#define		BIT24				(0x01000000)				/* bit24				*/
#define		BIT25				(0x02000000)				/* bit25				*/
#define		BIT26				(0x04000000)				/* bit26				*/
#define		BIT27				(0x08000000)				/* bit27				*/
#define		BIT28				(0x10000000)				/* bit28				*/
#define		BIT29				(0x20000000)				/* bit29				*/
#define		BIT30				(0x40000000)				/* bit30				*/
#define		BIT31				(0x80000000)				/* bit31				*/

#define		BITS_BY_BYTE	8	/*1バイトあたりのビット数*/

#define		WORD1BYTE			(0xFF000000)				/* ワードデータ内上位1BYTE目 */
#define		WORD2BYTE			(0x00FF0000)				/* ワードデータ内上位2BYTE目 */
#define		WORD3BYTE			(0x0000FF00)				/* ワードデータ内上位3BYTE目 */
#define		WORD4BYTE			(0x000000FF)				/* ワードデータ内上位4BYTE目 */

#define		UH1BYTE				(0xFF00)					/* UHデータ内上位1BYTE目 */
#define		UH2BYTE				(0x00FF)					/* UHデータ内上位2BYTE目 */

/* 汎用エラー定義 */
#define		E_OK		((INT)0)			/* 正常終了 */
#define		E_GENE		((INT)1)			/* 汎用エラー */
#define		E_OBJ		((INT)2)			/* オブジェクト状態エラー */
#define		E_PAR		((INT)3)			/* パラメータエラー */
#define		E_QOVR		((INT)4)			/* キューイングオーバーフロー */



#define		CONV_BEAM_UNIT	1000						/* 桁変換(キロ→メガなど)の単位 */

#define		DIGIT_CHANGE_KILO			1024				/* Kbyte */
#define		DIGIT_CHANGE_MEGA			( 1024 * 1024 )		/* Mbyte */

/* ポートの無効値 ※前回値に使用 */
#define		PORT_INVALID		0xff						/* ポートの無効値 */

/* 時刻定義 */
#define		MIN_30_SEC					( 60 * 30 )			/* 秒換算での30分 */
#define		HOUR_SEC					( 60 * 60 )			/* 秒換算での1時間 */


/* 演算用補正値 ※固定小数点第3位四捨五入し、固定小数点第2位の演算用 */
#define		CALC_HALF_ADJUST			5					/* 四捨五入用 */


/* nopの定義 */
/* 12.000MHz駆動の1クロック83.333‥ns */
#define	Nop1()		NOP();															/* 83.333ns */
#define	Nop5()		NOP();NOP();NOP();NOP();NOP();									/* 416.666ns */
#define	Nop10()		Nop5();Nop5();													/* 833.333ns */
#define	Nop20()		Nop10();Nop10();												/* 1.666μs */
#define	Nop40()		Nop20();Nop20();												/* 3.333μs */
#define	Nop120()	Nop40();Nop40();Nop40();										/* 10μs */

#define WAIT_1US()		Nop10();Nop1();Nop1();										/* 約1us待ち */
#define WAIT_5US()		WAIT_1US();WAIT_1US();WAIT_1US();WAIT_1US();WAIT_1US();		/* 約1us待ち */

#define WAIT_10US()		WAIT_5US();WAIT_5US();										/* 約10us待ち */

#define WAIT_EEP_STOP_AFTER			WAIT_10US();


/* AD変換回数 */
#define	AD_CHANGE_CNT					10

/* サム値関連 */
#define	CALC_SUM_INIT		((UB)0x00)			/* サム値計算の初期値 */
#define	CALC_SUM_INIT_UH	((UH)0x0000)		/* サム値計算の初期値 */
#define	CALC_SUM_INIT_UW	((UW)0x00000000)	/* サム値計算の初期値 */

/* CRC関連 */
#define CRCD_DEFAULT_LIB	((UH)0xFFFF)		/* CRCデータレジスタ 初期値 */

/* エンディアン変換 */
#define	CHANGE_UB2_UW		0			/* UBx2 → UWx1 */
#define	CHANGE_UW_UB2		1			/* UWx1 → UBx2 */

/* P/Sモード */
#define PS_MODE_P			0			/* パチンコ仕様 */
#define PS_MODE_S			1			/* スロット仕様 */
#define PS_MODE_MAX			2			/* 最大種別 */

/* 待ち時間の定義 time_nop_wait() */
/* 注意：各待ち処理は余裕を持っている。計測時は割り込み禁止で計測する事 */
#define TIM_NOP_WAIT_500MS			( 625000 )		/* コールドスタート時500ms以上待ち用 ※正確でなくてよい */
#define TIM_NOP_WAIT_1MS			( 4200 )		/* 1ms以上待ち用 ※正確でなくてよい *///RD1214暫定：値の調整が必要
#define TIM_NOP_WAIT_10US			( 10 )			/* 10us以上待ち用 ※正確でなくてよい */

/* 関数コール種別 */
typedef enum{
	FUNC_CALL_TYPE_POWON = 0,	/* 起動時					*/
	FUNC_CALL_TYPE_NORMAL		/* 通常時					*/
}FUNC_CALL_TYPE;

/* =====汎用マクロ関数===== */
/* maxまでdataをインクリメント */
#define	INC_MAX(data,max)				\
{										\
	if( data < max ){					\
		data++;							\
	}									\
}

/* maxまでdataをインクリメントして上限超えたら初期化 */
#define	INC_MAX_INI(data,max,ini)		\
{										\
	if( data < max ){					\
		data++;							\
	}else{								\
		data = ini;						\
	}									\
}

/* minまでdataをデクリメント */
#define	DEC_MIN(data,min)				\
{										\
	if( data > min ){					\
		data--;							\
	}									\
}

/* minまでdataをデクリメントしてminになったら初期化 */
#define	DEC_MIN_INI(data,min,ini)		\
{										\
	if( data > min ){					\
		data--;							\
	}else{								\
		data = ini;						\
	}									\
}

/* type別のuhの上位桁もしくは下位桁を返す */
#define	UH_U		0		/* 上位アドレス取得 */
#define	UH_D		1		/* 下位アドレス取得 */

/* 単体デバッグ用定義 */
//#define STATIC							static				/* 単体デバッグ実施時はSTATICを無のデファインにしても良い */
#define STATIC							static				/* 単体デバッグ実施時はSTATICを無のデファインにしても良い */

/* =====機能有効無効(ON/OFF)===== */
#define	FUNC_RELEASE					ON					/* リリース */
															/* 正式リリースが始まった後は基本的にONで運用→リリース時の間違いをなくす */
															/* OFFは、デバッグ時にローカルで修正 */

/* リリースオプション */
#if FUNC_RELEASE == ON
	//随時追加すること
#else
															/* ※デバッガ接続時は自己診断ポートがLOWになるため無効化する */
#endif

//アンドロイドデバッグ時はOFFとする
#define FUNC_ADR_COM_ERR_RESET			ON					/* アンドロイド通信異常時のリセット有効 */



#define FUNC_DOWNLOAD					ON				/* ダウンロードの許可 */

/*------------*/
/* デバッグ用 */
/*------------*/
/* デバッグポート機能 */
#define FUNC_DEBUG_PORT				ON				/* デバッグポート機能(ON:デバッグポート有効 OFF:デバッグポート無効) */
#define FUNC_DEBUG_PORT_LOG			ON				/* デバッグポートにログ出力(ON:デバッグポートログ有効 OFF:デバッグポートログ無効) */


/*----------------*/
/* 途中リリース用 */
/*----------------*/
#define FUNC_SELF_CHECK_MODE_TEST	OFF				/* 強制自己診断モード */

#define	FUNC_DISP_LAMP_CHECK_SOFT	OFF				/* 【ランプ動作確認用ソフト】(ON:有効,OFF;無効) */
													/* 　ONにする場合は、232Cログ機能も併せてONにする事 */
													/* 　ONにする場合は、自己診断モードのみリリースも併せてONにする事 */

#define FUNC_HARD_CHECK_SOFT				OFF		/* ハードチェック用ソフト */
													/* 通信関連を定期出力する事でハード担当の方が確認を行う */

/************************************************************/
/* 型定義													*/
/************************************************************/
/* 変数型の再定義 */
typedef signed char		B;		/* 符号付き8ビット整数 */
typedef unsigned char	UB;		/* 符号無し8ビット整数 */
typedef signed short	H;		/* 符号付き16ビット整数 */
typedef unsigned short 	UH;		/* 符号無し16ビット整数 */
typedef signed long		W;		/* 符号付き32ビット整数 */
typedef unsigned long	UW;		/* 符号無し32ビット整数 */
typedef signed int		INT;	/* プロセッサに自然なサイズの符号付き整数 */
typedef unsigned int	UINT;	/* プロセッサに自然なサイズの符号無し整数 */
typedef int				BOOL;	/* 真偽値(TRUE or FALSE) */
typedef float 			FLOAT;	/* 単精度浮動小数点 */
typedef double			DOUBLE;	/* 倍単精度浮動小数点 */
typedef int				ER;		/* 汎用エラー(μiTRON表記) */


/* ビットフィールド */
typedef struct{
    UB bit0 : 1;
    UB bit1 : 1;
    UB bit2 : 1;
    UB bit3 : 1;
    UB bit4 : 1;
    UB bit5 : 1;
    UB bit6 : 1;
    UB bit7 : 1;
}BIT_FIELD;

/* ビットフィールド */
typedef union{
	BIT_FIELD	bit;
	UB	byte;
}BIT_FIELD_UNION;


/* 日時 */
typedef struct{
	INT	tm_sec;		/* 秒 */
	INT	tm_min;		/* 分 */
	INT	tm_hour;	/* 時 */
	INT	tm_mday;	/* 日 */
	INT	tm_mon;		/* 月 */
	INT	tm_year;	/* 年 */
	INT	tm_wday;	/* 曜日 */
	INT	tm_yday;	/* 日(1年の何日目) */
	INT	tm_isdst;	/* サマータイムフラグ */
}DATE;


/* リングバッファ(1バイト用) */
typedef struct{
	UB*	buf;					/* リングバッファ用ポインタ */
	UH	wr_pos;					/* 書き込み位置 */
	UH	rd_pos;					/* 読み出し位置 */
	UH	size;					/* サイズ */
}RING_BUF;

/* リングバッファ(2バイト用) */
typedef struct{
	UH*	buf;					/* リングバッファ用ポインタ */
	UH	wr_pos;					/* 書き込み位置 */
	UH	rd_pos;					/* 読み出し位置 */
	UH	size;					/* サイズ */
}RING_BUF_UH;

/* リモコン用 */
typedef struct{
	UW	time;						/* 時間 */
	UB	key_code;					/* キーコード */
	UB	kind;						/* リモコン種別(セキュリティ/通常) */
	UB	staff_code;					/* スタッフコード */
}REMO;

/* リングバッファ(時間付きバイトデータ) */
typedef struct{
	REMO*	buf;			/* リングバッファ用ポインタ */
	UH	wr_pos;					/* 書き込み位置 */
	UH	rd_pos;					/* 読み出し位置 */
	UH	size;					/* サイズ */
}RING_BUF_REMO;

/* 入出力情報用 */
typedef struct{
	UB	sync;					/* 同期用データ */
	UB	port1;					/* 入出力情報1 */
	UB	port2;					/* 入出力情報2 */
	UB	port3;					/* 入出力情報3 */
}IO_INFO;

/* リングバッファ(3バイト(24bit)用) */
typedef struct{
	IO_INFO*	buf;			/* リングバッファ用ポインタ */
	UH	wr_pos;					/* 書き込み位置 */
	UH	rd_pos;					/* 読み出し位置 */
	UH	size;					/* サイズ */
}RING_BUF_IO;


/* 座標位置 */
typedef struct{
	H	x;			/* x座標 ポジション情報またはAD変換値 */
	H	y;			/* y座標 ポジション情報またはAD変換値 */
}POS_INFO;

/************************************************************/
/* 外部参照宣言												*/
/************************************************************/


#endif
/************************************************************/
/* END OF TEXT												*/
/************************************************************/



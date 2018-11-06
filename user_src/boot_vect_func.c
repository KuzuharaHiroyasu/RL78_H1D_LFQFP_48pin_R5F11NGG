/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : vect_func.c                                                   */
/* 機能         : ブート、製品プログラム共通のベクタテーブル関数                */
/* 変更履歴     : 2013.01.23 Axia Soft Design K.Wada    初版作成                */
/* 注意事項     : なし                                                          */
/********************************************************************************/

#include "iodefine.h"
#include "vect_func.h"

/********************/
/* 割込みベクタ定義 */
/********************/

#pragma interrupt	InterruptWdti	(vect=	INTWDTI	)	
#pragma interrupt 	InterruptLvi	(vect=	INTLVI	)	
#pragma interrupt 	InterruptPort0	(vect=	INTP0	)	
#pragma interrupt 	InterruptPort1	(vect=	INTP1	)	
#pragma interrupt 	InterruptPort2	(vect=	INTP2	)	
#pragma interrupt 	InterruptPort3	(vect=	INTP3	)	
#pragma interrupt 	InterruptPort4	(vect=	INTP4	)	
#pragma interrupt 	InterruptPort5	(vect=	INTP5	)	
#pragma interrupt 	InterruptCsi20	(vect=	INTCSI20	)
//ベクタ番号重複#pragma interrupt 	InterruptI2C20	(vect=	INTIIC20	)
//ベクタ番号重複#pragma interrupt 	InterruptSerialTx2	(vect=	INTST2	)
#pragma interrupt 	InterruptSerialRx2	(vect=	INTSR2	)
#pragma interrupt 	InterruptSerialRxError2	(vect=	INTSRE2	)
//ベクタ番号重複#pragma interrupt 	InterruptCsi00	(vect=	INTCSI00	)
//ベクタ番号重複#pragma interrupt 	InterruptI2C00	(vect=	INTIIC00	)
#pragma interrupt 	InterruptSerialTx0	(vect=	INTST0	)	
#pragma interrupt 	InterruptTimer00	(vect=	INTTM00	)	
#pragma interrupt 	InterruptSerialRx0	(vect=	INTSR0	)
#pragma interrupt 	InterruptSerialRxError0	(vect=	INTSRE0	)	
//ベクタ番号重複#pragma interrupt 	InterruptTimer01h	(vect=	INTTM01H	)
//ベクタ番号重複#pragma interrupt 	InterruptCsi10	(vect=	INTCSI10	)
//ベクタ番号重複#pragma interrupt 	InterruptI2C10	(vect=	INTIIC10	)
#pragma interrupt 	InterruptSerialTx1	(vect=	INTST1	)	
#pragma interrupt 	InterruptSerialRx1	(vect=	INTSR1	)
#pragma interrupt 	InterruptSerialRxError1	(vect=	INTSRE1	)	
//ベクタ番号重複#pragma interrupt 	InterruptTimer03h	(vect=	INTTM03H	)
#pragma interrupt 	InterruptI2CA0	(vect=	INTIICA0	)
#pragma interrupt 	InterruptRtit	(vect=	INTRTIT	)	//
#pragma interrupt 	InterruptTimer01	(vect=	INTTM01	)	
#pragma interrupt 	InterruptTimer02	(vect=	INTTM02	)	
#pragma interrupt 	InterruptTimer03	(vect=	INTTM03	)	
#pragma interrupt	InterruptADconvert	(vect=	INTAD	)	
#pragma interrupt	InterruptRtc	(vect=	INTRTC	)	
#pragma interrupt	InterruptTimerIt	(vect=	INTIT	)
#pragma interrupt	InterruptTimer04	(vect=	INTTM04	)	
#pragma interrupt	InterruptTimer05	(vect=	INTTM05	)	
#pragma interrupt	InterruptPort6	(vect=	INTP6	)	
#pragma interrupt	InterruptTimer06	(vect=	INTTM06	)	
#pragma interrupt	InterruptTimer07	(vect=	INTTM07	)	
#pragma interrupt 	InterruptTimerIt00	(vect=	INTIT00	)
#pragma interrupt 	InterruptTimerIt01	(vect=	INTIT01	)
#pragma interrupt 	InterruptDsad	(vect=	INTDSAD	)
#pragma interrupt 	InterruptDsads	(vect=	INTDSADS	)
#pragma interrupt 	InterruptFl	(vect=	INTFL	)	




#include "./header.h"




/************************************************************************/
/* ベクタテーブル                                                       */
/************************************************************************/
extern ST_VECT_FUNC	ram_vect_tbl[VECT_NUM_COUNT];
/*				 InterruptReset					RESETはベクタなし*/
void	InterruptWdti	(void){	ram_vect_tbl[	VECT_NUM_INTWDTI	].func();	}	// 1
void	InterruptLvi	(void){	ram_vect_tbl[	VECT_NUM_INTLVI	].func();	}	// 2
void	InterruptPort0	(void){	ram_vect_tbl[	VECT_NUM_INTP0	].func();	}	// 3
void	InterruptPort1	(void){	ram_vect_tbl[	VECT_NUM_INTP1	].func();	}	// 4
void	InterruptPort2	(void){	ram_vect_tbl[	VECT_NUM_INTP2	].func();	}	// 5
void	InterruptPort3	(void){	ram_vect_tbl[	VECT_NUM_INTP3	].func();	}	// 6
void	InterruptPort4	(void){	ram_vect_tbl[	VECT_NUM_INTP4	].func();	}	// 7
void	InterruptPort5	(void){	ram_vect_tbl[	VECT_NUM_INTP5	].func();	}	// 8
void	InterruptCsi20	(void){	ram_vect_tbl[	VECT_NUM_INTCSI20	].func();	}	// 9
void	InterruptI2C20	(void){	ram_vect_tbl[	VECT_NUM_INTIIC20	].func();	}	// 10
void	InterruptSerialTx2	(void){	ram_vect_tbl[	VECT_NUM_INTST2	].func();	}	// 11
void	InterruptSerialRx2	(void){	ram_vect_tbl[	VECT_NUM_INTSR2	].func();	}	// 12
void	InterruptSerialRxError2	(void){	ram_vect_tbl[	VECT_NUM_INTSRE2	].func();	}	// 13
void	InterruptCsi00	(void){	ram_vect_tbl[	VECT_NUM_INTCSI00	].func();	}	// 14
void	InterruptI2C00	(void){	ram_vect_tbl[	VECT_NUM_INTIIC00	].func();	}	// 15
void	InterruptSerialTx0	(void){	ram_vect_tbl[	VECT_NUM_INTST0	].func();	}	// 16
void	InterruptTimer00	(void){	ram_vect_tbl[	VECT_NUM_INTTM00	].func();	}	// 17
void	InterruptSerialRx0	(void){	ram_vect_tbl[	VECT_NUM_INTSR0	].func();	}	// 18
void	InterruptSerialRxError0	(void){	ram_vect_tbl[	VECT_NUM_INTSRE0	].func();	}	// 19
void	InterruptTimer01h	(void){	ram_vect_tbl[	VECT_NUM_INTTM01H	].func();	}	// 20
void	InterruptCsi10	(void){	ram_vect_tbl[	VECT_NUM_INTCSI10	].func();	}	// 21
void	InterruptI2C10	(void){	ram_vect_tbl[	VECT_NUM_INTIIC10	].func();	}	// 22
void	InterruptSerialTx1	(void){	ram_vect_tbl[	VECT_NUM_INTST1	].func();	}	// 23
void	InterruptSerialRx1	(void){	ram_vect_tbl[	VECT_NUM_INTSR1	].func();	}	// 24
void	InterruptSerialRxError1	(void){	ram_vect_tbl[	VECT_NUM_INTSRE1	].func();	}	// 25
void	InterruptTimer03h	(void){	ram_vect_tbl[	VECT_NUM_INTTM03H	].func();	}	// 26
void	InterruptI2CA0	(void){	ram_vect_tbl[	VECT_NUM_INTIICA0	].func();	}	// 27
void	InterruptRtit	(void){	ram_vect_tbl[	VECT_NUM_INTRTIT	].func();	}	// 28
void	InterruptTimer01	(void){	ram_vect_tbl[	VECT_NUM_INTTM01	].func();	}	// 29
void	InterruptTimer02	(void){	ram_vect_tbl[	VECT_NUM_INTTM02	].func();	}	// 30
void	InterruptTimer03	(void){	ram_vect_tbl[	VECT_NUM_INTTM03	].func();	}	// 31
void	InterruptADconvert	(void){	ram_vect_tbl[	VECT_NUM_INTAD	].func();	}	// 32
void	InterruptRtc	(void){	ram_vect_tbl[	VECT_NUM_INTRTC	].func();	}	// 33
void	InterruptTimerIt	(void){	ram_vect_tbl[	VECT_NUM_INTIT	].func();	}	// 34
void	InterruptTimer04	(void){	ram_vect_tbl[	VECT_NUM_INTTM04	].func();	}	// 35
void	InterruptTimer05	(void){	ram_vect_tbl[	VECT_NUM_INTTM05	].func();	}	// 36
void	InterruptPort6	(void){	ram_vect_tbl[	VECT_NUM_INTP6	].func();	}	// 37
void	InterruptTimer06	(void){	ram_vect_tbl[	VECT_NUM_INTTM06	].func();	}	// 38
void	InterruptTimer07	(void){	ram_vect_tbl[	VECT_NUM_INTTM07	].func();	}	// 39
void	InterruptTimerIt00	(void){	ram_vect_tbl[	VECT_NUM_INTIT00	].func();	}	// 40
void	InterruptTimerIt01	(void){	ram_vect_tbl[	VECT_NUM_INTIT01	].func();	}	// 41
void	InterruptDsad	(void){	ram_vect_tbl[	VECT_NUM_INTDSAD	].func();	}	// 42
void	InterruptDsads	(void){	ram_vect_tbl[	VECT_NUM_INTDSADS	].func();	}	// 43
void	InterruptFl	(void){	ram_vect_tbl[	VECT_NUM_INTFL	].func();	}	// 44

/*				 InterruptBreak					BREAKはベクタなし*/

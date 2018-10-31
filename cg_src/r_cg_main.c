/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014, 2016 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_main.c
* Version      : Code Generator for RL78/I1E V1.02.02.01 [11 Nov 2016]
* Device(s)    : R5F11CCC
* Tool-Chain   : CCRL
* Description  : This file implements main function.
* Creation Date: 2017/06/01
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_pga_dsad.h"
#include "r_cg_camp.h"
#include "r_cg_dac.h"
#include "r_cg_sau.h"
#include "r_cg_intp.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

#include "sys.h"
#include <stdio.h>
#include <string.h>

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
static void R_MAIN_UserInit(void);

/* ---------------------------------------------------------------------------------------------------------------------
 * define
 * ------------------------------------------------------------------------------------------------------------------ */
#define APP_WAIT_1000MS     (2400000UL)


#define df_reg_data_top      (0xF1C00)	/* for R5F11NGG(BLOCK3) */
#define p_df_reg_enable_ptr    ((__far const struct w_reg8*  )(df_reg_data_top+0x000))//write flag

#define p_df_afepws_ptr        ((__far const struct w_reg8*  )(df_reg_data_top+0x010))
#define p_df_afecks_ptr        ((__far const struct w_reg8*  )(df_reg_data_top+0x011))
#define p_df_vsbias_ptr        ((__far const struct w_reg8*  )(df_reg_data_top+0x012))
#define p_df_dsadmr_ptr        ((__far const struct w_reg8*  )(df_reg_data_top+0x013))
#define p_df_dsadctl_ptr       ((__far const struct w_reg8*  )(df_reg_data_top+0x014))
#define p_df_pga0ctl0_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x015))
#define p_df_pga0ctl1_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x016))
#define p_df_pga0ctl2_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x017))
#define p_df_pga0ctl3_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x018))
#define p_df_pga1ctl0_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x019))
#define p_df_pga1ctl1_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x01A))
#define p_df_pga1ctl2_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x01B))
#define p_df_pga1ctl3_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x01C))
#define p_df_pga2ctl0_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x01D))
#define p_df_pga2ctl1_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x01E))
#define p_df_pga2ctl2_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x01F))
#define p_df_pga2ctl3_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x020))
#define p_df_pga3ctl0_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x021))
#define p_df_pga3ctl1_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x022))
#define p_df_pga3ctl2_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x023))
#define p_df_pga3ctl3_ptr      ((__far const struct w_reg8*  )(df_reg_data_top+0x024))
#define p_df_pgabod_ptr        ((__far const struct w_reg8*  )(df_reg_data_top+0x025))
#define p_df_ampmc_ptr         ((__far const struct w_reg8*  )(df_reg_data_top+0x026))
#define p_df_amptrm_ptr        ((__far const struct w_reg8*  )(df_reg_data_top+0x027))
#define p_df_amptrs_ptr        ((__far const struct w_reg8*  )(df_reg_data_top+0x028))
#define p_df_ampc_ptr          ((__far const struct w_reg8*  )(df_reg_data_top+0x029))
#define p_df_pga1gc_ptr        ((__far const struct w_reg8*  )(df_reg_data_top+0x02A))
#define p_df_pga1s_ptr         ((__far const struct w_reg8*  )(df_reg_data_top+0x02B))
#define p_df_amp0s_ptr         ((__far const struct w_reg8*  )(df_reg_data_top+0x02C))
#define p_df_amp1s_ptr         ((__far const struct w_reg8*  )(df_reg_data_top+0x02D))
#define p_df_amp2s_ptr         ((__far const struct w_reg8*  )(df_reg_data_top+0x02E))
#define p_df_dac0dr_ptr        ((__far const struct w_reg8*  )(df_reg_data_top+0x02F))
#define p_df_dac1dr_ptr        ((__far const struct w_reg16* )(df_reg_data_top+0x030))
#define p_df_dacm0_ptr         ((__far const struct w_reg8*  )(df_reg_data_top+0x032))
#define p_df_dacm1_ptr         ((__far const struct w_reg8*  )(df_reg_data_top+0x033))
#define D_DATAFLASH_SET_REG_NUM    (35)

/***********************************************************************************************************************
* Function Name: main
* Description  : This function implements main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
// 変数宣言
#define			SYSTEM_MODE_NON						0
#define			SYSTEM_MODE_NORMAL					1
#define			SYSTEM_MODE_GET_MODE				2

typedef struct{
	UB main_cyc_req;	/* メイン周期要求(20ms) */
	UB system_mode;	/* メイン周期要求(20ms) */
	
	
	UH eep_wr_record_cnt;	/* 書き込みレコード */
	UH eep_rd_record_cnt;	/* 読み出しレコード */	
	UH dench_val;		
	UH kokyu_val;		
	UH ibiki_val;		
	W sekishoku_val;	// 差動入力の為に符号あり
	W sekigaival;		// 差動入力の為に符号あり
	
	UW err_cnt;			//異常回数(デバッグ用途)
}T_UNIT;




#define EEP_DEVICE_ADR			0xA0				// デバイスアドレス
#define EEP_RECORD_SIZE			16					// 1レコードサイズ
#define EEP_ADRS_SIZE			2					// 1レコードサイズ
#if 0
// 全サイズ
#define EEP_DATA_SIZE_ALL		131072				// EEP全サイズ
#define EEP_DATA_SIZE_PAGE		( 131072 / 2 )		// EEPページサイズ
#else
// 1ページ版(暫定)
//#define EEP_RECORD_1P_MAX			( 4096 )					// 1レコードサイズ(1ページ目)
//#define EEP_RECORD_2P_MAX			( 4096  )					// 1レコードサイズ(2ページ目)
// 4096までいけない
#define EEP_RECORD_1P_MAX			( 4096  )					// 1レコードサイズ(1ページ目)
#define EEP_RECORD_2P_MAX			( 8192  )					// 1レコードサイズ(2ページ目)


#endif

typedef struct{
	union{
		UB	byte[EEP_RECORD_SIZE + EEP_ADRS_SIZE];
		struct{
			UH	wr_adrs;		/* EEPアドレス */
			UH	kokyu_val;		
			UH	ibiki_val;		
			W	sekishoku_val;	// 差動入力の為に符号あり
			W	sekigaival;		// 差動入力の為に符号あり
			UB	valid;			/* レコード有効/無効				*/
			UB	dumyy[3];		/* ダミー */
			
			
		}data;
	}record;
}WR_EEP_RECORD;

typedef struct{
	union{
		UB	byte[EEP_RECORD_SIZE];
		struct{
			UH	kokyu_val;		
			UH	ibiki_val;		
			W	sekishoku_val;	// 差動入力の為に符号あり
			W	sekigaival;		// 差動入力の為に符号あり
			UB	valid;			/* レコード有効/無効				*/
			UB	dumyy[3];		/* ダミー */
		}data;
	}record;
}RD_EEP_RECORD;


STATIC RD_EEP_RECORD		s_eep_read;


#define I2C_RCV_SND_SIZE	50		//RD8001暫定：サイズ要調整
uint8_t eep_rx_data[I2C_RCV_SND_SIZE];        /* iica0 receive buffer */
uint8_t eep_tx_data[I2C_RCV_SND_SIZE];        /* iica0 send buffer */

UH bufferH_dbg_ave;
UH bufferL_dbg_ave;
UH bufferH_dbg;
UH bufferL_dbg;
UH tx_num;
UB wait;

STATIC T_UNIT s_unit;
int i2c_cmplete;

// プロトタイプ宣言　※無いと正常に動かない
void main_send_uart1(void);
void main_send_uart1_powon(void);
MD_STATUS R_IICA0_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait);
MD_STATUS R_IICA0_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait);
void main_eep_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
int main_eep_read(void);
int main_eep_write(void);
void main_eep_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
void wait_ms( int ms );
void err_info( int id );
void main_send_uart1_start(void);
void main_send_uart1_end(void);
void get_mode( void );
void main_eep_erase_all( void );



// 外部参照宣言　※無いと正常に動かない
extern void R_IICA0_Create(void);
extern MD_STATUS R_UART1_Send(uint8_t * const tx_buf, uint16_t tx_num);
extern MD_STATUS R_UART1_Receive(uint8_t * const rx_buf, uint16_t rx_num);

extern void R_PGA_DSAD_Get_AverageResult(uint16_t * const bufferH,uint16_t * const bufferL);
extern void R_PGA_DSAD_Get_Result(uint16_t * const bufferH,uint16_t * const bufferL);
extern void R_DAC_Change_OutputVoltage_12bit(uint16_t outputVoltage);
extern void adc_do( uint16_t* ad1, uint16_t* ad2, uint16_t* ad3 );
extern unsigned short pga_do( void );
extern void R_IICA0_StopCondition(void);

const B		version_product_tbl[]= {0, 0, 0, 4};				/* ソフトウェアバージョン */
																/* バージョン表記ルール */
																/* �@メジャーバージョン：[0 〜 9] */
																/* �Aマイナーバージョン：[0 〜 9]  */
																/* �Bリビジョン：[0 〜 9] */
																/* �Cビルドバージョン：[0 〜 9] */

void main(void)
{
	
	UW work_uw = 0;
    UB sw = OFF;
    UB last_sw = OFF;
    
    R_MAIN_UserInit();
    /* Start user code. Do not edit comment generated here */
    main_send_uart1_powon();

#if 0
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
	eep_tx_data[2] = 11;
	eep_tx_data[3] = 22;
	main_eep_write_sub( 0xA0, &eep_tx_data[0], 4 );
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
	eep_tx_data[2] = 33;
	eep_tx_data[3] = 44;
	main_eep_write_sub( 0xA0, &eep_tx_data[0], 4 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
	eep_tx_data[2] = 55;
	eep_tx_data[3] = 66;
	main_eep_write_sub( 0xA0, &eep_tx_data[0], 4 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
	eep_tx_data[2] = 77;
	eep_tx_data[3] = 88;
	main_eep_write_sub( 0xA0, &eep_tx_data[0], 4 );
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
//	main_eep_write_sub( 0xA0, &eep_tx_data[0], 2 );

	main_eep_read_sub( 0xA0, 0, &eep_rx_data[0], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
//	main_eep_write_sub( 0xA0, &eep_tx_data[0], 2 );

	main_eep_read_sub( 0xA0, 5, &eep_rx_data[2], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
///	main_eep_write_sub( 0xA0, &eep_tx_data[0], 2 );

	main_eep_read_sub( 0xA0, 8, &eep_rx_data[4], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
//	main_eep_write_sub( 0xA0, &eep_tx_data[0], 2 );

	main_eep_read_sub( 0xA0, 13, &eep_rx_data[6], 2 );

	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	

#endif

#if 0
	//デバッグコード
	s_unit.eep_wr_record_cnt = 0;
	
	s_unit.kokyu_val  = 1111;		
	s_unit.ibiki_val  = 2222;		
	s_unit.sekishoku_val = 333333;	// 差動入力の為に符号あり
	s_unit.sekigaival = -444444;		// 差動入力の為に符号あり
    main_eep_write();
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}
	s_unit.eep_wr_record_cnt = 4096;
	
	s_unit.kokyu_val  = 3333;		
	s_unit.ibiki_val  = 4444;		
	s_unit.sekishoku_val = 555555;	// 差動入力の為に符号あり
	s_unit.sekigaival = -666666;		// 差動入力の為に符号あり
    main_eep_write();
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}



	s_unit.eep_wr_record_cnt = 4095;
	
	s_unit.kokyu_val  = 2222;		
	s_unit.ibiki_val  = 3333;		
	s_unit.sekishoku_val = 444444;		// 差動入力の為に符号あり
	s_unit.sekigaival = -555555;		// 差動入力の為に符号あり
    main_eep_write();
    
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}
	s_unit.eep_rd_record_cnt = 0;
    main_eep_read();
	s_unit.eep_rd_record_cnt = 4096;
    main_eep_read();
	s_unit.eep_rd_record_cnt = 4095;
    main_eep_read();
#endif
	
#if 0
	s_unit.eep_rd_record_cnt = 0;
	s_unit.eep_wr_record_cnt = 0;
	s_unit.kokyu_val  = 1111;		
	s_unit.ibiki_val  = 2222;		
	s_unit.sekishoku_val = 333333;	// 差動入力の為に符号あり
	s_unit.sekigaival = -444444;		// 差動入力の為に符号あり
	while(1){
		if( INVALID == main_eep_write() ){
			// 終了
			break;
		}
		wait_ms(5);
//		for(i = 0; i < 500; i++ ){
//			WAIT_10US()
//		}
	}	
	
#endif

	s_unit.eep_rd_record_cnt = 0;
	s_unit.eep_wr_record_cnt = 0;
#if 0
	// 有効レコードチェック探索  ※しない
	while(1){
		if( INVALID == main_eep_read() ){
			// 終了
			break;
		}
		if( 1 == s_eep_read.record.data.valid ){
			// 有効レコード
			s_unit.eep_wr_record_cnt++;
		}else{
			// 終了
			break;
		}
	}
#endif
    
	R_UART1_Receive( &eep_rx_data[0], 10 );

//	s_unit.system_mode = SYSTEM_MODE_NORMAL;
    while (1U)
    {
//		if( 0 == R_UART1_Receive( &eep_rx_data[0], 3 )){
//			if(( eep_rx_data[0] == 0x47 ) && 
//			   ( eep_rx_data[1] == 0x45 ) && 
//			   ( eep_rx_data[2] == 0x54 )){
//				s_unit.system_mode = SYSTEM_MODE_GET_MODE;
//			}
//		}
#if 1
		if( P12 & 0x02 ){
			sw = OFF;
		}else{
			sw = ON;
		}
		if(( ON == sw ) && ( OFF == last_sw )){
			if( SYSTEM_MODE_NON == s_unit.system_mode ){
				s_unit.main_cyc_req = OFF;
				s_unit.system_mode = SYSTEM_MODE_NORMAL;
			}else if( SYSTEM_MODE_NORMAL == s_unit.system_mode ){
				s_unit.system_mode = SYSTEM_MODE_NON;
			}else{
				/* 何もしない */
			}
		}
#endif
		last_sw = sw;
		
		if( SYSTEM_MODE_NON == s_unit.system_mode ){
			/* 何もしない */
		}else if( SYSTEM_MODE_GET_MODE == s_unit.system_mode ){
			// ゲットモード
			
			get_mode();
			
			
			NOP();
		
		}else{
			// 通常モード
			
			// メイン周期(20ms)
			if( ON == s_unit.main_cyc_req ){
				// マイク用(呼吸、イビキ)
				R_DAC_Change_OutputVoltage_12bit( 0x0000 );
				adc_do( &s_unit.dench_val, &s_unit.ibiki_val, &s_unit.kokyu_val );
				// 赤色光
	#if 1
				P7 &= ~0x40;
				P7 |= 0x80;

	//			VSBIAS = 0x0E;		/* 1.0 *///oneA様指示設定　※反応が悪い
				VSBIAS = 0x09;		/* 0.5 */
	//			R_DAC_Change_OutputVoltage_12bit( 0x014A );//oneA様指示設定　※反応が悪い 
				R_DAC_Change_OutputVoltage_12bit( 0x02BD ); 

				pga_do();
				R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);
				R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
				// RD8001暫定：エンディアンがマニュアルを読んでも確証を持てない
				work_uw = bufferH_dbg;
				work_uw <<= 16;
				work_uw += bufferL_dbg;
				s_unit.sekishoku_val = work_uw;
				s_unit.sekishoku_val >>= 8;
	#endif			


				// 赤外光
	#if 1
				P7 &= ~0x80;
				P7 |= 0x40;
				VSBIAS = 0x09;		/* 0.5 */
				R_DAC_Change_OutputVoltage_12bit( 0x07BD );
				pga_do();
				R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);
				R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
				work_uw = bufferH_dbg;
				work_uw <<= 16;
				work_uw += bufferL_dbg;
				s_unit.sekigaival = work_uw;
				s_unit.sekigaival >>= 8;
	#endif			
				
				R_DAC_Change_OutputVoltage_12bit( 0x0000 );		//RD1704暫定：DAC変更後一定時間待たないと電圧が落ち切らない様に思う

				
				// 1レコード送信
				main_send_uart1();
				
				wait_ms(5);			//すぐに書き込むと送信が乱れる
//				for(i = 0; i < 500; i++ ){
//					WAIT_10US()
//				}
				// 1レコード書き込み	//暫定
				main_eep_write();
				
				s_unit.main_cyc_req  = OFF;
			}
		}
    }
    /* End user code. Do not edit comment generated here */
}
/***********************************************************************************************************************
* Function Name: R_MAIN_UserInit
* Description  : This function adds user code before implementing main function.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
static void R_MAIN_UserInit(void)
{
	//データ初期化
	memset( &s_unit, 0, sizeof(s_unit) );
    /* Start user code. Do not edit comment generated here */
    EI();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
void set_req_main_cyc(void)
{
	s_unit.main_cyc_req = ON;
}

//================================
//UART関連
//================================

#define UART1_STR_MAX		100
void main_send_uart1(void)
{
	uint8_t tx_data[UART1_STR_MAX] = {0};
//	uint8_t * const s = &tx_data[0];
	int len;
	
#if 0
	s_unit.sekishoku_val = 1;
	s_unit.sekigaival = 2;
	s_unit.kokyu_val = 3;
	s_unit.ibiki_val = 4;
#endif
	
//	len = sprintf(tx_data, "%d,%d,%ld,%ld\r\n", s_unit.kokyu_val, s_unit.ibiki_val, s_unit.sekishoku_val, s_unit.sekigaival );
	len = sprintf(tx_data, "%ld,%ld,%d,%d\r\n", s_unit.sekishoku_val, s_unit.sekigaival, s_unit.kokyu_val, s_unit.ibiki_val  );
	
//	len = strlen( s );		//RD8001暫定：strlenが使えない理由が不明
	R_UART1_Send( &tx_data[0], len );
//	R_UART1_Send( s, len );
}

// debug
void main_send_uart1_powon(void)
{
	int len;
	char tx_data[UART1_STR_MAX] = {0};
	
	len = sprintf(tx_data, "RD8001 POW_ON\r\n" );
	
	R_UART1_Send( &tx_data[0], len );
}

void main_send_uart1_start(void)
{
	int len;
	char tx_data[UART1_STR_MAX] = {0};
	
	len = sprintf(tx_data, "START\r\n" );
	
	R_UART1_Send( &tx_data[0], len );

	wait_ms(1);			//RD8001暫定：すぐに書き込むと送信が乱れる
}

void main_send_uart1_end(void)
{
	int len;
	char tx_data[UART1_STR_MAX] = {0};
	
	len = sprintf(tx_data, "END\r\n" );
	
	R_UART1_Send( &tx_data[0], len );

	wait_ms(1);			//RD8001暫定：すぐに書き込むと送信が乱れる
}

//================================
//EEP関連
//================================
#define			EEP_WAIT		255		//RD8001暫定値

int main_eep_write(void)
{
	WR_EEP_RECORD		wr;
	UH wr_adrs;
	UB device_adrs;
	int ret = INVALID;
	UH record_offset = 0;

#if 0
	UB adrs[2];

	// ページ処理の断片
	UB page_bit;
	int eep_adrs;
	int eep_page;
	UB  device_adrs;
#endif
	
	// レコード最大
	if( s_unit.eep_wr_record_cnt < EEP_RECORD_1P_MAX ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( s_unit.eep_wr_record_cnt < EEP_RECORD_2P_MAX ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
		record_offset = EEP_RECORD_1P_MAX;
	}else{
		return ret;
	}

#if 0
	// ページ処理の断片
	eep_adrs = eep_wr_record_cnt * EEP_RECORD_SIZE;
	eep_page = eep_adrs / EEP_DATA_SIZE_PAGE;
	eep_adrs = eep_adrs % EEP_DATA_SIZE_PAGE;

	if( 0 == page ){
		device_adrs = EEP_DEVICE_ADR;
	}else{
		device_adrs = EEP_DEVICE_ADR | 0x02;
	}
#endif
	wr_adrs = ( s_unit.eep_wr_record_cnt - record_offset ) * EEP_RECORD_SIZE;
//	wr.record.data.wr_adrs				 = s_unit.eep_wr_record_cnt * EEP_RECORD_SIZE;
	wr.record.byte[0] = (UB)(( wr_adrs >> 8 ) & 0xff );		//アドレス上位
	wr.record.byte[1] = (UB)( wr_adrs & 0x00ff );		//アドレス下位
	wr.record.data.valid				 = ON;			/* レコード有効/無効				*/
	
	wr.record.data.kokyu_val			 = s_unit.kokyu_val;		
	wr.record.data.ibiki_val			 = s_unit.ibiki_val;		
	wr.record.data.sekishoku_val		 = s_unit.sekishoku_val;	// 差動入力の為に符号あり
	wr.record.data.sekigaival			 = s_unit.sekigaival;		// 差動入力の為に符号あり
	
	//書き込み
	main_eep_write_sub( device_adrs, &wr.record.byte[0], sizeof(wr), ON );
//	eep_tx_data[0] = (UB)( wr_adrs & 0x00ff );		//アドレス下位
//	eep_tx_data[1] = (UB)(( wr_adrs >> 8 ) & 0xff );		//アドレス上位
//	eep_tx_data[14]  = 1;
//	main_eep_write_sub( EEP_DEVICE_ADR, &eep_tx_data[0], sizeof(wr) );
	
	s_unit.eep_wr_record_cnt++;
	
	ret = VALID;
	
	return ret;
}

void main_eep_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg )
{
	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Send( device_adrs, wr_data, len, EEP_WAIT)){
		err_info(2);
	}else{
		while(1){
			if( 1 == i2c_cmplete ){
				break;
			}
		}
		R_IICA0_StopCondition();
	}
	while(0U == SPD0)

	/* ストップコンディション後の待ち */
	if( wait_flg == ON ){
		wait_ms(5);
	}
}

int main_eep_read(void)
{
	int ret = INVALID;
	UH read_adrs;
	RD_EEP_RECORD		rd;
	UB device_adrs;
	UH record_offset = 0;
	
	// レコード最大
	if( s_unit.eep_rd_record_cnt < EEP_RECORD_1P_MAX ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( s_unit.eep_rd_record_cnt < EEP_RECORD_2P_MAX ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
		record_offset = EEP_RECORD_1P_MAX;
	}else{
		return ret;
	}
	
	read_adrs = ( s_unit.eep_rd_record_cnt - record_offset ) * EEP_RECORD_SIZE;
	main_eep_read_sub( device_adrs, read_adrs, &s_eep_read.record.byte[0], sizeof(rd) );
//	main_eep_read_sub( EEP_DEVICE_ADR, read_adrs, &eep_rx_data[0], 32 );
	
	s_unit.eep_rd_record_cnt++;
	
	ret = VALID;
	
	return ret;
}

void main_eep_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len )
{
	UB adrs[2];

#if 0
	adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//アドレス上位
	adrs[1] = (UB)( read_adrs & 0x00ff );		//アドレス下位
		
	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Send( device_adrs, &adrs[0], 2, EEP_WAIT )){
		err_info(3);
	}else{
#if 1
		while(1){
			if(( 1 == i2c_cmplete ) || ( lcok_cnt++ >= 10000 )){		//RD8001暫定
//			if( 1 == i2c_cmplete ){		//RD8001暫定
				break;
			}
		}
//		R_IICA0_StopCondition();
#endif
	}
	/* ストップコンディション後の待ち */
//	WAIT_5US();		//RD8001暫定
//	WAIT_5US();		//RD8001暫定
	wait_ms(5);
#else
	//RD8001暫定：ストップコンディションを送らないと読み出し失敗する
	adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//アドレス上位
	adrs[1] = (UB)( read_adrs & 0x00ff );		//アドレス下位
		
	main_eep_write_sub(device_adrs, &adrs[0], 2 , OFF );
#endif
	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Receive(device_adrs, read_data, len, EEP_WAIT)){
		err_info(4);
	}else{
		while(1){
			if( 1 == i2c_cmplete ){
				break;
			}
		}
		R_IICA0_StopCondition();
	}
	while(0U == SPD0)
	/* ストップコンディション後の待ち */
	wait_ms(5);
	WAIT_5US();		//RD8001暫定
}

void getmode_in(void)
{
	s_unit.system_mode = SYSTEM_MODE_GET_MODE;
}

void wait_ms( int ms )
{
	int i,j;
	
	for(i = 0; i < ms; i++ ){
		for(j = 0; j < 100; j++ ){
			WAIT_10US()
		}
	}
}



//void err_info( ERR_ID id )
void err_info( int id )
{
	s_unit.err_cnt++;
#if 0
	while(1){
		// 異常による永久ループ
	}
#endif
}

void get_mode( void )
{
	main_send_uart1_start();	// START送信
	wait_ms(10);

	// ゲットモードループ
	s_unit.eep_rd_record_cnt = 0;
	while(1){
		if( INVALID == main_eep_read() ){
			// 終了
			break;
		}
		if( 1 == s_eep_read.record.data.valid ){
			// 有効レコード
			s_unit.kokyu_val	 = s_eep_read.record.data.kokyu_val;		
			s_unit.ibiki_val	 = s_eep_read.record.data.ibiki_val;		
			s_unit.sekishoku_val = s_eep_read.record.data.sekishoku_val;	// 差動入力の為に符号あり
			s_unit.sekigaival	 = s_eep_read.record.data.sekigaival;		// 差動入力の為に符号あり
			
			wait_ms(5);
			main_send_uart1();
			wait_ms(5);
		}else{
			// 終了
			break;
		}
	}
	main_eep_erase_all();

	s_unit.eep_rd_record_cnt = 0;
	s_unit.eep_wr_record_cnt  =0;
	s_unit.system_mode = SYSTEM_MODE_NON;
	
	main_send_uart1_end();	// END送信
}


void main_eep_erase_all(void)
{
	WR_EEP_RECORD wr;
	UB device_adrs =0;
	UH record_offset = 0;
	UH wr_adrs = 0;
	
	s_unit.eep_wr_record_cnt = 0;
	
	while(1){
		// レコード最大
		if( s_unit.eep_wr_record_cnt < EEP_RECORD_1P_MAX ){
			device_adrs = EEP_DEVICE_ADR;
		}else if( s_unit.eep_wr_record_cnt < EEP_RECORD_2P_MAX ){
			device_adrs = EEP_DEVICE_ADR | 0x02;
			record_offset = EEP_RECORD_1P_MAX;
		}else{
			return;
		}
		
		// 使用分のみで終了
#if 0
		if( s_unit.eep_rd_record_cnt < s_unit.eep_wr_record_cnt ){
			return;
		}
#endif
		
		wr_adrs = ( s_unit.eep_wr_record_cnt - record_offset ) * EEP_RECORD_SIZE;
		wr.record.byte[0] = (UB)(( wr_adrs >> 8 ) & 0xff );		//アドレス上位
		wr.record.byte[1] = (UB)( wr_adrs & 0x00ff );		//アドレス下位
		wr.record.data.valid				 = OFF;			/* レコード有効/無効				*/
		
		wr.record.data.kokyu_val			 = 0;
		wr.record.data.ibiki_val			 = 0;
		wr.record.data.sekishoku_val		 = 0;
		wr.record.data.sekigaival			 = 0;
			
		//書き込み
		main_eep_write_sub( device_adrs, &wr.record.byte[0], sizeof(wr), ON );	
		wait_ms(5);
	
		s_unit.eep_wr_record_cnt++;
	}
	
}

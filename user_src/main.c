/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : main.c														*/
/* �@�\         : �S�̊Ǘ��i�֐�,�ϐ�,�萔��`�j								*/
/* �ύX����     : 2018.03.02 Axia Soft Design ���� ��	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

#include "header.h"

/************************************************************/
/* �v���g�^�C�v�錾											*/
/************************************************************/
void user_main(void);
static void user_main_mode_sensor(void);
static void main_init(void);
static void sw_cyc(void);
static void mode_cyc(void);
void set_req_main_cyc(void);
void main_send_uart1(void);
void main_send_uart1_rtc(void);
void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
void getmode_in(void);
void wait_ms( int ms );
void err_info( int id );
void main_acl_init(void);
void main_acl_read(void);
void main_acl_write(void);
STATIC W main_ad24_to_w( UH bufferH, UH bufferL );
static void stop_in( void );
static void halt_in( void );
STATIC void main_cpu_com_snd_sensor_data( void );
STATIC void main_cpu_com_proc(void);
STATIC void main_cpu_com_rcv_sts( void );
STATIC void main_cpu_com_rcv_date_set( void );
STATIC void main_cpu_com_rcv_pc_log( void );
STATIC void main_cpu_com_rcv_mode_chg( void );
void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );
STATIC void main_cyc(void);
STATIC void main_cpu_com_rcv_prg_hd_ready(void);
STATIC void main_cpu_com_rcv_prg_hd_check(void);

/************************************************************/
/* �ϐ���`													*/
/************************************************************/

UH bufferH_dbg_ave;
UH bufferL_dbg_ave;
UH bufferH_dbg;
UH bufferL_dbg;
UH tx_num;
UB wait;

int i2c_cmplete;

UB dbg_set_clock = 0;



STATIC T_UNIT s_unit;
STATIC DS s_ds;

/************************************************************/
/* �e�[�u����`												*/
/************************************************************/
const B		version_product_tbl[]= {0, 0, 0, 5};				/* �\�t�g�E�F�A�o�[�W���� */
																/* �o�[�W�����\�L���[�� */
																/* �@���W���[�o�[�W�����F[0 �` 9] */
																/* �A�}�C�i�[�o�[�W�����F[0 �` 9]  */
																/* �B���r�W�����F[0 �` 9] */
																/* �C�r���h�o�[�W�����F[0 �` 9] */

/* ��M�f�[�^���� �֐��e�[�u�� */
STATIC const CPU_COM_RCV_CMD_TBL s_cpu_com_rcv_func_tbl[CPU_COM_CMD_MAX] = {
	/* �R�}���h */		/* �֐�  */					/* �����L�� */
	{	0x00,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�R�}���h����				*/
	{	0xE0,			main_cpu_com_rcv_sts,			OFF	},	/* �yCPU�ԒʐM�R�}���h�z�X�e�[�^�X�v��				*/
	{	0xA0,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�Z���T�[�f�[�^�X�V			*/
	{	0xB0,			main_cpu_com_rcv_mode_chg,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z��ԕύX(G1D)				*/
	{	0xF0,			main_cpu_com_rcv_pc_log,		OFF	},	/* �yCPU�ԒʐM�R�}���h�zPC���O���M(�����R�}���h)	*/
	{	0xB1,			main_cpu_com_rcv_date_set,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�����ݒ�					*/
	{	0xD5,			main_cpu_com_rcv_prg_hd_ready,	OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������			*/
	{	0xD2,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���J�n			*/
	{	0xD4,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������			*/
	{	0xD0,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���f�[�^		*/
	{	0xD1,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������			*/
	{	0xD3,			main_cpu_com_rcv_prg_hd_check,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���m�F			*/
};

extern void test_uart_0_send( void );
extern void test_cpu_com_send( void );

#if 0
rtc_counter_value_t dbg_rtc;
rtc_counter_value_t dbg_rtc_set;
UB dbg_rtc_flg = 0;
#endif

/************************************************************************/
/* �֐�     : user_main													*/
/* �֐���   : ���C�����[�v												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.03.02 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* ���C�����[�v															*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void user_main(void)
{
	UW sleep_cnt = 0;
//    int cnt_20ms = 0;
//    int cnt_30sec = 0;
	rtc_counter_value_t rtc;
//	char com_data[10];
    
	vect_set_tbl_apl();	

    main_init();
    /* Start user code. Do not edit comment generated here */
	
	// RTC�ݒ�
#if 1
	if( dbg_set_clock == 7 ){
//		R_RTC_Set_CounterValue( rtc );
	}
#endif
	R_RTC_Get_CounterValue( &rtc );
	s_unit.hour = rtc.hour;
	s_unit.min = rtc.min;
	s_unit.sec = rtc.sec;
//	R_RTC_Start();
	
	// �N�����O
	com_srv_log_title();

	main_acl_init();
	
	while (1U)
	{
		com_srv_cyc();
		sw_cyc();
		mode_cyc();
		cpu_com_proc();
		main_cpu_com_proc();
		main_cyc();
		
		if( SYSTEM_MODE_NON == s_unit.system_mode ){
			drv_o_port_mike( OFF );		//RD8001�b��
			/* �������Ȃ� */
			// RD8001�b��F�X���[�v�˓�����
			if( sleep_cnt++ > 5000 ){
				sleep_cnt = 0;
#if 0			//RTC�f�o�b�O
				R_RTC_Get_CounterValue( &dbg_rtc );
				
				if( 1 == dbg_rtc_flg ){
					dbg_rtc_flg = 0;
					dbg_rtc_set.year = 9;
					dbg_rtc_set.sec = 0;
					dbg_rtc_set.min = 0;
					dbg_rtc_set.hour = 5;
					dbg_rtc_set.day = 0;
					dbg_rtc_set.week = 0;
					dbg_rtc_set.month = 0;
//					R_RTC_Stop();
					R_RTC_Set_CounterValue( dbg_rtc_set );
//					R_RTC_Start();
				}
#endif
//				drv_uart1_send_start();
//				test_cpu_com_send();		//�~�h�����x��CPU�ԒʐM
//				test_uart_0_send();			//�h���C�o���x��CPU�ԒʐM

				//stop_in();
			}
		}else if( SYSTEM_MODE_GET_MODE == s_unit.system_mode ){
			drv_o_port_mike( OFF );		//RD8001�b��
			// �Q�b�g���[�h
			
//			get_mode();
			
			
			NOP();
		
		}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
			user_main_mode_sensor();
		}else{
			drv_o_port_mike( OFF );		//RD8001�b��
			// ���肦�Ȃ�
			
		}
		if( OFF == s_unit.non_wdt_refresh ){
			wdt_refresh();		//WDT���t���b�V��
		}
    }
    /* End user code. Do not edit comment generated here */
}

//#define IBIKI_COUNT			//RD8001�b��F���т��J�E���g�A�b�v
#ifdef	IBIKI_COUNT
UH dbg_ibiki_cnt = 0;
#endif
#if FUNC_DEBUG_RAW_VAL_OUT == ON
UB dbg_send_cnt;
char dbg_tx_data[50] = {0};
int dbg_len;
#endif

static void user_main_mode_sensor(void)
{
	UW time;
	
	
//	MEAS meas;
	
#ifdef	IBIKI_COUNT
	dbg_ibiki_cnt = 0;
#endif
	
	if( s_unit.sensing_cnt_50ms++ >= HOUR12_CNT_50MS ){
		s_unit.sensing_end_flg = ON;
	}
	
	if( ON == s_unit.sensing_end_flg ){
		return;
	}
	
	time_soft_get_10ms( TIME_TYPE_10MS_SENSING_DELAY, &time );
	if( 0 != time ){
		return;
	}
	
	//RD8001�b��F�Z���T�[�����삵�Ȃ�
	// �Z���T�[���[�h
	// ���C������(50ms)
	if( ON == s_unit.main_cyc_req ){
		R_DAC1_Start();			// ��DAC        ON
		R_PGA_DSAD_Start();		// ��PGA0_DSAD  ON
		R_AMP2_Start();			// ��AMP2       ON
		
//RD8001�b��F���������s���Ɠ��삵�Ȃ�
#if FUNC_VALID_AMP == ON
		R_DAC1_Set_ConversionValue( 0x0B00 );
		wait_ms( 2 );
		
		// �ԐF��ON
		drv_o_port_sekigai( OFF );
		drv_o_port_sekishoku( ON );

		pga_do();
//		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);		//����
		R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
		s_unit.meas.info.dat.sekishoku_val = main_ad24_to_w( bufferH_dbg, bufferL_dbg );
		wait_ms( 2 );

		// �ԊO��ON
		drv_o_port_sekigai( ON );
		drv_o_port_sekishoku( OFF );

		R_DAC1_Set_ConversionValue( 0x0500 );
		wait_ms( 2 );
		pga_do();
//		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);		//����
		R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
		s_unit.meas.info.dat.sekigaival = main_ad24_to_w( bufferH_dbg, bufferL_dbg );
		
		R_PGA_DSAD_Stop();		// ��PGA0_DSAD  OFF
		R_AMP2_Stop();		// ��AMP2  OFF
		
		wait_ms( 2 );
		
		// �}�C�N�p(�ċz�A�C�r�L)�J�n
		R_DAC1_Set_ConversionValue( 0x0000 );
		R_AMP0_Start();		// ��AMP0 ON
		R_PGA1_Start();		// ��PGA1 ON

		wait_ms( 2 );
		adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
		
		// RD8001�b��F���т��f�[�^���f�o�b�O�p�J�E���g
#ifdef	IBIKI_COUNT
		s_unit.meas.info.dat.ibiki_val = dbg_ibiki_cnt++;
#endif
		wait_ms( 2 );
		R_DAC1_Stop();		// ��DAC  OFF

#if 0	// RD8001�b��FAMP0,PGA1��OFF����Ɛ���ɃC�r�L�����擾�ł��Ȃ�
		R_AMP0_Stop();		// ��AMP0 OFF
		R_PGA1_Stop();		// ��PGA1 OFF
#endif
		
		// �����x�Z���T�擾
		// RD8001�b��F30ms�����邪50ms���Ɏ擾
#if FUNC_DEBUG_RAW_VAL_OUT == OFF
		main_acl_read();
#else
		//���l�f�o�b�O�o�́@�������x�͖���
		if( dbg_send_cnt++ >= 0 ){	
			dbg_send_cnt = 0;
//			printf( "kokyu%d", s_unit.meas.info.dat.kokyu_val );		//�R���\�[���f�o�b�O�p�@������R���\�[���̎g�������s��
			{
				
				
				
				dbg_len = sprintf((char*)dbg_tx_data, "%ld,%ld,%d,%d,0,0,0\r\n",		 s_unit.meas.info.dat.sekishoku_val
																					,s_unit.meas.info.dat.sekigaival
																					,s_unit.meas.info.dat.kokyu_val
																					,s_unit.meas.info.dat.ibiki_val);
				com_srv_send( &dbg_tx_data[0], dbg_len );
			}
		}
		wait_ms( 5 );
#endif
		
#endif

		main_cpu_com_snd_sensor_data();
		
		s_unit.main_cyc_req  = OFF;
		
		// �ԊO�A�ԐFOFF
		drv_o_port_sekigai( OFF );
		drv_o_port_sekishoku( OFF );

		// RD8001�b��Fstop���[�h����
#if 1
		stop_in();
#endif
	}

}

/************************************************************************/
/* �֐�     : main_ad24_to_w											*/
/* �֐���   : AD24bit4byte(��������)�ϊ�								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.03.02 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* AD24bit4byte(��������)�ϊ�											*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC W main_ad24_to_w( UH bufferH, UH bufferL )
{
	W value = 0;
	UW work_uw = 0;
	W work_w = 0;
	UB m_flg = 0;
	
	// bufferH �����8bit,����8bit
	// bufferL ������8bit,�ϊ�����8bit
	work_uw  = bufferH;
	work_uw <<= 16;
	work_uw += bufferL;
	work_uw >>= 8;
	work_uw &= 0x00FFFFFF;		// 24bit�f�[�^��
	// ��������
	if( work_uw & 0x00800000 ){
		work_w = work_uw;
		work_w *= -1;			// �v���X�ɕϊ�
		m_flg = 1;
	}else{
		work_w = work_uw;
	}
	work_uw = work_w;
	work_uw &= 0x007FFFFF;
	if( 1 == m_flg ){
		value = (W)work_uw * (W)-1;
	}else{
		value = (W)work_uw;
	}
		
	return value;
}

/************************************************************************/
/* �֐�     : main_init													*/
/* �֐���   : ������													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.03.02 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* ������																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
static void main_init(void)
{
	//�f�[�^������
	memset( &s_unit, 0, sizeof(s_unit) );
	/* Start user code. Do not edit comment generated here */
	
	
	// �~�h��������
	com_srv_init();
	cpu_com_init();
	time_init();
	
	EI();
}

/************************************************************************/
/* �֐�     : sw_cyc													*/
/* �֐���   : SW��������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.03.02 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* ������																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
static void sw_cyc(void)
{
	UB pow_sw;
	UW time = 0;
	
	pow_sw = drv_intp_read_pow_sw();
	
	if( ON == pow_sw ){
		// �Ȃɂ����Ȃ�(�d��SW�����^�C�}�[�p��)
	}else{
		if( ON == s_unit.pow_sw_last ){
			// ON��OFF�G�b�W
			time_soft_get_10ms( TIME_TYPE_10MS_POW_SW_ACT_START, &time );
			if( 0 == time ){
				// �K�莞�Ԉȏ�A�������Ɣ��f
				s_unit.sensing_start_trig = ON;
			}
		}
		// �d��SW�����^�C�}�[�ăX�^�[�g 
		time_soft_set_10ms( TIME_TYPE_10MS_POW_SW_ACT_START, TIME_10MS_CNT_POW_SW_ACT_START );
	}
	s_unit.pow_sw_last = pow_sw;
}

/************************************************************************/
/* �֐�     : mode_cyc													*/
/* �֐���   : ���[�h��������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.03.02 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* ������																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
static void mode_cyc(void)
{
	//RD8001�b��F��ԕύX�ӏ�����ӏ�
	if( ON == s_unit.sensing_start_trig ){
		s_unit.sensing_start_trig = OFF;
		if( SYSTEM_MODE_NON == s_unit.system_mode ){
			s_unit.main_cyc_req = OFF;
			s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_SENSING;
			
			
			
		}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
			s_unit.sensing_end_flg = ON;
			s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_IDLE;
//			s_unit.system_mode = SYSTEM_MODE_NORMAL;
//		}else if( SYSTEM_MODE_NORMAL == s_unit.system_mode ){
//			s_unit.system_mode = SYSTEM_MODE_NON;
		}else{
			/* �������Ȃ� */
		}
	}
}

/************************************************************************/
/* �֐�     : main_cyc													*/
/* �֐���   : �A�v����������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* ������																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC void main_cyc(void)
{
	UH dench_val;
	
	
	dench_val = DENCH_ZANRYO_1_VAL;
	
	adc_dench( &dench_val );
	
	if( dench_val >= DENCH_ZANRYO_1_VAL ){
		s_unit.dench_sts = 1;
	}else if( dench_val >= DENCH_ZANRYO_2_VAL ){
		s_unit.dench_sts = 2;
	}else if( dench_val >= DENCH_ZANRYO_3_VAL ){
		s_unit.dench_sts = 3;
	}else{
		s_unit.dench_sts = 4;
	}
}

void set_req_main_cyc(void)
{
	s_unit.main_cyc_req = ON;
}

//================================
//UART�֘A
//================================

#define UART1_STR_MAX		150
void main_send_uart1(void)
{
#if 0
	uint8_t tx_data[UART1_STR_MAX] = {0};
	int len;
	
#if 0
	s_unit.sekishoku_val = 1;
	s_unit.sekigaival = 2;
	s_unit.kokyu_val = 3;
	s_unit.ibiki_val = 4;
#endif

#if 0		//EEP����
#if 1
	len = sprintf((char*)tx_data, "%ld,%ld,%d,%d,%d,%d,%d\r\n", s_unit.eep.record.data.sekishoku_val
															  , s_unit.eep.record.data.sekigaival
															  , s_unit.eep.record.data.kokyu_val
															  , s_unit.eep.record.data.ibiki_val
															  , s_unit.eep.record.data.acl_x
															  , s_unit.eep.record.data.acl_y
															  , s_unit.eep.record.data.acl_z );




#else
	len = sprintf((char*)tx_data, "%d,%f,%f,%f,%f,%f,%d,%d,%d\r\n", s_unit.eep.record.data.state_flg,					/* �t���O(�ċz���,�����X�e�[�W,���т��L�� */
															 s_unit.eep.record.data.ibiki_val,					/* ���т� */
															 s_unit.eep.record.data.myaku_val_sekishoku,		/* ����_�ԊO */
															 s_unit.eep.record.data.myaku_val_sekigai,			/* ����_�ԐF */
															 s_unit.eep.record.data.spo2_val_normal,			/* SPO2_�ʏ� */
															 s_unit.eep.record.data.spo2_val_konica,			/* SPO2 */
															 s_unit.eep.record.data.acl_x,
															 s_unit.eep.record.data.acl_y,
															 s_unit.eep.record.data.acl_z );
	
//	main_acl_read();
//	len = sprintf(tx_data, "%d,%d,%d\r\n", s_unit.acl_x, s_unit.acl_y, s_unit.acl_z );		//�����x
#endif
#endif
	
	
	com_srv_send( &tx_data[0], len );


#if 0
	len = sprintf((char*)tx_data, "%d,%d,%d\r\n", s_unit.hour,
												  s_unit.min,
												  s_unit.sec );
	com_srv_send( &tx_data[0], len );
#endif




//	com_srv_send( s, len );
#endif
}

// debug
void main_send_uart1_rtc(void)
{
	int len;
	UB tx_data[UART1_STR_MAX] = {0};
	rtc_counter_value_t rtc;
	
	R_RTC_Get_CounterValue( &rtc );
	s_unit.hour = rtc.hour;
	s_unit.min = rtc.min;
	s_unit.sec = rtc.sec;
	
	len = sprintf((char*)tx_data, "CLOCK %x,%x,%x\r\n", s_unit.hour, s_unit.min, s_unit.sec );

	com_srv_send( tx_data, len );

//	wait_ms(5);			//RD8001�b��F�����ɏ������ނƑ��M�������

}

//================================
//EEP�֘A
//================================
#define			EEP_WAIT		255		//RD8001�b��l

void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg )
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

	/* �X�g�b�v�R���f�B�V������̑҂� */
	if( wait_flg == ON ){
		wait_ms(5);
	}
}

void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len )
{
	UB adrs[2];
	UB adrs_size = 0;
	UW lcok_cnt = 0;

#if 1		// �X�g�b�v�r�b�g�Ȃ�
	
	if( EEP_DEVICE_ADR == device_adrs ){
		// EEP��
		adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//�A�h���X���
		adrs[1] = (UB)( read_adrs & 0x00ff );			//�A�h���X����
		adrs_size  = 2;
	}else if( ACL_DEVICE_ADR == device_adrs ){
		// �����x�Z���T�[��
		adrs[0] = (UB)( read_adrs & 0x00ff );			//�A�h���X
		adrs_size  = 1;
	}

	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Send( device_adrs, &adrs[0], adrs_size, EEP_WAIT )){
		err_info(3);
	}else{
#if 1
		while(1){
			if(( 1 == i2c_cmplete ) || ( lcok_cnt++ >= 10000 )){		//RD8001�b��
//			if( 1 == i2c_cmplete ){		//RD8001�b��
				break;
			}
		}
//		R_IICA0_StopCondition();
#endif
	}
	/* �X�g�b�v�R���f�B�V������̑҂� */
//	WAIT_5US();		//RD8001�b��
//	WAIT_5US();		//RD8001�b��
//	wait_ms(5);
	R_IICA0_Stop();
	R_IICA0_Create();
#else
	// �X�g�b�v�r�b�g����
	//RD8001�b��F�X�g�b�v�R���f�B�V�����𑗂�Ȃ��Ɠǂݏo�����s����
	adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//�A�h���X���
	adrs[1] = (UB)( read_adrs & 0x00ff );		//�A�h���X����
		
	i2c_write_sub(device_adrs, &adrs[0], 2 , OFF );
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
	/* �X�g�b�v�R���f�B�V������̑҂� */
	wait_ms(5);
	WAIT_5US();		//RD8001�b��
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
		// �ُ�ɂ��i�v���[�v
	}
#endif
}

void main_acl_init(void)
{
	UB rd_data[2];
	UB wr_data[2];
	
	i2c_read_sub( ACL_DEVICE_ADR, 0x0F, &rd_data[0], 1 );
	if( rd_data[0] != 0x35 ){
		err_info( 5 );
	}
	
	wr_data[0] = 0x1B;
	wr_data[1] = 0x00;
	// ���샂�[�h�ݒ�
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	wr_data[0] = 0x1B;
	wr_data[1] = 0x20;
	// ���샂�[�h�ݒ�
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );

	wr_data[0] = 0x1F;
	wr_data[1] = 0xBF;
	// ���샂�[�h�ݒ�
//	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	
	
	wr_data[0] = 0x1B;
	wr_data[1] = 0xA0;
	// ���샂�[�h�ݒ�
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	
}

void main_acl_read(void)
{
	UB rd_data[10];
	
	// INT_SOURCE1		
	i2c_read_sub( ACL_DEVICE_ADR, 0x16, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & 0x10 )){
		// �f�[�^���B
		return;
	}
	
	// �f�[�^�擾
	i2c_read_sub( ACL_DEVICE_ADR, 0x06, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
	// INT_REL�ǂݏo���@�����荞�ݗv���N���A
	i2c_read_sub( ACL_DEVICE_ADR, 0x1A, &rd_data[0], 1 );
}

void main_acl_write(void)
{
//	main_acl_read_sub( ACL_DEVICE_ADR, 0x0C, &rd_data[0], 1 );
}



static void stop_in( void )
{
	//RD8001�b��F�˓����A�����Ɋւ��ėv����
#if 0
	com_srv_puts( (const B *__near)"STOP IN\r\n" );
	wait_ms( 10 );
#endif
	
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	
	// ���荞�ݎ~�߂鏈��
//	TMKAMK = 1U;    	  /* disable INTIT interrupt */
//	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	STOP();
	
	// STOP���[�h��������(�N���b�N������~+�E�F�C�g)�@��CPU�}�j���A���@Page 1066 of 1325
	// �N���b�N������~�F18 ��s �`65 ��s
	// �E�G�C�g�F�x�N�^���荞�ݏ������s���ꍇ(7�N���b�N)
	//             �x�N�^���荞�ݏ������s��Ȃ��ꍇ(1�N���b�N)
	wait_ms( 1 );
//	TMKAMK = 0U;    	  /* disable INTIT interrupt */
//	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
#if 0
	com_srv_puts( (const B *__near)"STOP OUT\r\n" );
#endif
}

static void halt_in( void )
{
	com_srv_puts( (const B *__near)"HALT IN\r\n" );
	wait_ms( 10 );
	//RD8001�b��F�˓����A�����Ɋւ��ėv����
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	
	// ���荞�ݎ~�߂鏈��
	TMKAMK = 1U;    	  /* disable INTIT interrupt */
	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	HALT();
	// �E�F�C�g�@��CPU�}�j���A���@Page 1060 of 1325
	// �x�N�^���荞�ݏ������s���ꍇ
	// ���C���E�V�X�e���E�N���b�N�� �F15�`16�N���b�N
	// �x�N�^���荞�ݏ������s��Ȃ��ꍇ
	// ���C���E�V�X�e���E�N���b�N�� �F9�`10�N���b�N
	wait_ms( 1 );
	TMKAMK = 0U;    	  /* disable INTIT interrupt */
	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	com_srv_puts( (const B *__near)"HALT OUT\r\n" );
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
}


// CPU�ԒʐM�T�[�r�X
// =============================================================
// CPU�ʐM�֘A
// =============================================================
STATIC void main_cpu_com_snd_sensor_data( void )
{
	drv_o_port_g1d_int( ON );

	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_SENSOR_DATA;
	memcpy( &s_ds.cpu_com.order.snd_data[0], &s_unit.meas.info.byte[0], CPU_COM_SND_DATA_SIZE_SENSOR_DATA );
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_SENSOR_DATA;
	
	// �ȍ~�X���[�v�����ׂ̈ɑ��M�I���܂ł̏������Ă�
	cpu_com_send_end();
}

/************************************************************************/
/* �֐�     : main_cpu_com_proc											*/
/* �֐���   : CPU�ԒʐM��������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.06.05  Axia Soft Design �{�{		���ō쐬			*/
/*          : 2014.06.27  Axia Soft Design �g��							*/
/*          :						��Ԃɉ����đ��M�g���K��؂芷����	*/
/* 			: 2016.05.19  Axia Soft Design �����@���g���C�A�E�g���̏����ǉ�(CPU�ԒʐM�ُ�Ή�) */
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_proc(void)
{
	int i = 0;

	/* ��M�f�[�^�`�F�b�N */
	if( 0x00 != s_ds.cpu_com.input.rcv_cmd ){
		//�����f�[�^��M  ���A���C�u
//		s_unit.cpu_com_success_once = ON;
//		time_get_elapsed_time( &s_unit.cpu_com_rcv_int_elapsed_time );
		/* ��M�R�}���h�L�� */
		for( i=CPU_COM_CMD_NONE; i<CPU_COM_CMD_MAX; i++){
			if( s_cpu_com_rcv_func_tbl[i].cmd == s_ds.cpu_com.input.rcv_cmd ){
				/* ��M�R�}���h�ƃR�}���h�e�[�u������v */
				if( NULL != s_cpu_com_rcv_func_tbl[i].func ){
					/* ��M�����L�� */
//					s_cpu_com_rcv_func_tbl[i].func(&s_ds.cpu_com.input.rcv_data[0]);
					s_cpu_com_rcv_func_tbl[i].func();
				}
			}
		}
		// ��M�R�}���h�N���A ���b��
		s_ds.cpu_com.input.rcv_cmd = 0x00;
	}
	
}

STATIC void main_cpu_com_rcv_sts( void )
{
	rtc_counter_value_t rtc_val;
	if( MD_OK != R_RTC_Get_CounterValue( &rtc_val ) ){
		err_info( 13 );
	}
	

	if( SYSTEM_MODE_NON == s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode_chg_req;
		s_ds.cpu_com.order.snd_data[1] = s_unit.system_mode;
		s_ds.cpu_com.order.snd_data[2] = s_unit.info_data;
		s_ds.cpu_com.order.snd_data[3] = rtc_val.year;
		s_ds.cpu_com.order.snd_data[4] = rtc_val.month;
		s_ds.cpu_com.order.snd_data[5] = rtc_val.week;
		s_ds.cpu_com.order.snd_data[6] = rtc_val.day;
		s_ds.cpu_com.order.snd_data[7] = rtc_val.hour;
		s_ds.cpu_com.order.snd_data[8] = rtc_val.min;
		s_ds.cpu_com.order.snd_data[9] = rtc_val.sec;

		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_NON;
	}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode_chg_req;
		s_ds.cpu_com.order.snd_data[1] = s_unit.system_mode;
		s_ds.cpu_com.order.snd_data[2] = s_unit.info_data;
		s_ds.cpu_com.order.snd_data[3] = rtc_val.year;
		s_ds.cpu_com.order.snd_data[4] = rtc_val.month;
		s_ds.cpu_com.order.snd_data[5] = rtc_val.week;
		s_ds.cpu_com.order.snd_data[6] = rtc_val.day;
		s_ds.cpu_com.order.snd_data[7] = rtc_val.hour;
		s_ds.cpu_com.order.snd_data[8] = rtc_val.min;
		s_ds.cpu_com.order.snd_data[9] = rtc_val.sec;
		
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_NON;
	}else{
		// �������Ȃ�
		
	}
}

STATIC void main_cpu_com_rcv_date_set( void )
{
	rtc_counter_value_t rtc_val;
	
	rtc_val.year = s_ds.cpu_com.input.rcv_data[0];
	rtc_val.month = s_ds.cpu_com.input.rcv_data[1];
	rtc_val.week = s_ds.cpu_com.input.rcv_data[2];
	rtc_val.day = s_ds.cpu_com.input.rcv_data[3];
	rtc_val.hour = s_ds.cpu_com.input.rcv_data[4];
	rtc_val.min = s_ds.cpu_com.input.rcv_data[5];
	rtc_val.sec = s_ds.cpu_com.input.rcv_data[6];

	if( MD_OK != R_RTC_Set_CounterValue( rtc_val ) ){
		err_info( 12 );
	}
	
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DATE_SET;
	s_ds.cpu_com.order.snd_data[0] = 0;
	s_ds.cpu_com.order.data_size = 1;
}


STATIC void main_cpu_com_rcv_pc_log( void )
{
	UB	state;			/* ���R�[�h�L��/����				*/
	UH	ibiki_val;
	UB	myaku_val;
	UB	spo2_val;
	UB	kubi;
	UB	dummy;
	int len;
	UB tx_data[UART1_STR_MAX] = {0};
	
	state = s_ds.cpu_com.input.rcv_data[0];			/* ���R�[�h�L��/����				*/
	ibiki_val = (( s_ds.cpu_com.input.rcv_data[2] << 8 )  | s_ds.cpu_com.input.rcv_data[1] );
	myaku_val = s_ds.cpu_com.input.rcv_data[3];
	spo2_val = s_ds.cpu_com.input.rcv_data[4];
	kubi = s_ds.cpu_com.input.rcv_data[5];
	dummy = s_ds.cpu_com.input.rcv_data[6];
	
	len = sprintf((char*)tx_data, "%d,%u,%d,%d,%d,%d\r\n"    , state
															  , ibiki_val
															  , myaku_val
															  , spo2_val
															  , kubi
															  , dummy);

	com_srv_send( tx_data, len );
}

STATIC void main_cpu_com_rcv_mode_chg( void )
{
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_MODE_CHG;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_MODE_CHG;
	
	// �Z���V���O�J�n���̃^�C�}�[
	if(( SYSTEM_MODE_SENSOR != s_unit.system_mode ) &&
	   ( SYSTEM_MODE_SENSOR == s_ds.cpu_com.input.rcv_data[0] )){
		time_soft_set_10ms( TIME_TYPE_10MS_SENSING_DELAY, TIME_10MS_CNT_SENSING_DELAY );
		s_unit.sensing_end_flg = OFF;
		s_unit.sensing_cnt_50ms = 0;
		drv_o_port_mike( ON );		//RD8001�b��
	}
	
	if( 1 ){
		// ��ԕύX
		s_unit.system_mode = s_ds.cpu_com.input.rcv_data[0];
		// ���퉞��
		s_ds.cpu_com.order.snd_data[0] = 0;		// ���퉞��
	}else{
		s_ds.cpu_com.order.snd_data[0] = 1;		// �ُ퉞��
	}
	
	
}

/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���J�n����		*/
STATIC void main_cpu_com_rcv_prg_hd_ready(void)
{
	// ���ʃR�[�h����
	download_change_boot();

	// RD8001�b��F�u�[�g�ւ̐ؑ֏���(WDT�ɂ�鎖�̃��Z�b�g)
	s_unit.non_wdt_refresh = ON;
	
	// ����
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_READY;
	s_ds.cpu_com.order.snd_data[0] = 0;
	s_ds.cpu_com.order.data_size = 1;
}

/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���m�F		*/
STATIC void main_cpu_com_rcv_prg_hd_check(void)
{
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_CHECK;
	s_ds.cpu_com.order.snd_data[0] = version_product_tbl[0];
	s_ds.cpu_com.order.snd_data[1] = version_product_tbl[1];
	s_ds.cpu_com.order.snd_data[2] = version_product_tbl[2];
	s_ds.cpu_com.order.snd_data[3] = version_product_tbl[3];
	s_ds.cpu_com.order.data_size = 4;
}

/************************************************************************/
/* �֐�     : ds_get_cpu_com_order										*/
/* �֐���   : CPU�ԒʐM�p�f�[�^�擾										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.15 Axia Soft Design �g�� �v�a	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�ԒʐM�p�f�[�^�擾												*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data )
{
	*p_data = &s_ds.cpu_com.order;
}
/************************************************************************/
/* �֐�     : ds_get_cpu_com_rcv_cmd									*/
/* �֐���   : CPU�ԒʐM�f�[�^�Z�b�g										*/
/* ����     : CPU�ԒʐM�f�[�^�i�[�|�C���^								*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.06.05 Axia Soft Design �{�{ �a��	���ō쐬			*/
/*          : 2014.06.12 Axia Soft Design �g��							*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�ԒʐM�~�h���f�[�^�Z�b�g�擾										*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data )
{
	s_ds.cpu_com.input = *p_data;
}




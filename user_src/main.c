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
void main_send_uart1(void);
MD_STATUS R_IICA0_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait);
MD_STATUS R_IICA0_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait);
void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
int main_eep_read(void);
int main_eep_write(void);
void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
void wait_ms( int ms );
void err_info( int id );
void get_mode( void );
void main_eep_erase_all( void );
void main_acl_init(void);
void main_acl_read(void);
void test_code_eep(void);
void main_send_uart1_rtc(void);
static void main_init(void);
static void sw_cyc(void);
static void mode_cyc(void);
static void stop_in( void );
static void halt_in( void );
STATIC void main_cpu_com_snd_sensor_data( void );
STATIC void main_cpu_com_proc(void);
STATIC void main_cpu_com_rcv_sts( void );
STATIC void main_cpu_com_rcv_pc_log( void );
STATIC void main_cpu_com_rcv_mode_chg( void );
void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );
static void user_main_mode_sensor(void);

/************************************************************/
/* �ϐ���`													*/
/************************************************************/
//RD8001�b��F�폜�\��
uint8_t eep_rx_data[I2C_RCV_SND_SIZE];        /* iica0 receive buffer */
uint8_t eep_tx_data[I2C_RCV_SND_SIZE];        /* iica0 send buffer */

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
	{	0xFF,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���J�n		*/
	{	0xFF,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���f�[�^�v�� */
	{	0xFF,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]��(��M)	*/
	{	0xFF,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���T���l�v�� */
	{	0xFF,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]�����ʗv��	*/
	{	0xFF,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�t�@�C���]���J�n		*/
	{	0xFF,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�t�@�C���]��			*/
	{	0xFF,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�u���b�N�]�����ʗv��	*/
	{	0xFF,			NULL,							OFF	},	/* �yCPU�ԒʐM�R�}���h�z�t�@�C���]�����ʗv��	*/
};

extern void test_uart_0_send( void );
extern void test_cpu_com_send( void );




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
    int cnt_20ms = 0;
    int cnt_30sec = 0;
	rtc_counter_value_t rtc;
	char com_data[10];
    
    main_init();
    /* Start user code. Do not edit comment generated here */
	
	// RTC�ݒ�
#if 1
	if( dbg_set_clock == 7 ){
		R_RTC_Set_CounterValue( rtc );
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
	
	s_unit.eep_rd_record_cnt = 0;
	s_unit.eep_wr_record_cnt = 0;
#if 0
	// �L�����R�[�h�`�F�b�N�T��  �����Ȃ�
	while(1){
		if( INVALID == main_eep_read() ){
			// �I��
			break;
		}
		if( 1 == s_unit.eep.record.data.valid ){
			// �L�����R�[�h
			s_unit.eep_wr_record_cnt++;
		}else{
			// �I��
			break;
		}
	}
#endif
    
    while (1U)
    {
		com_srv_cyc();
		sw_cyc();
		mode_cyc();
		cpu_com_proc();
		main_cpu_com_proc();
		
		if( SYSTEM_MODE_NON == s_unit.system_mode ){
			/* �������Ȃ� */
			// RD8001�b��F�X���[�v�˓�����
			if( sleep_cnt++ > 5000 ){
				sleep_cnt = 0;
//				drv_uart1_send_start();
//				test_cpu_com_send();		//�~�h�����x��CPU�ԒʐM
//				test_uart_0_send();			//�h���C�o���x��CPU�ԒʐM

				//stop_in();
			}
		}else if( SYSTEM_MODE_GET_MODE == s_unit.system_mode ){
			// �Q�b�g���[�h
			
			get_mode();
			
			
			NOP();
		
		}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
			user_main_mode_sensor();
		}else{
			// ���肦�Ȃ�
			
		}
    }
    /* End user code. Do not edit comment generated here */
}

static void user_main_mode_sensor(void)
{
	UW time;
	UW work_uw = 0;
	MEAS meas;
	
	
	if( ON == s_unit.sensing_end_flg ){
		// �ԊO��
		P7 &= ~0x80;
		// �ԐF��
		P7 &= ~0x40;
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

//RD8001�b��F���������s���Ɠ��삵�Ȃ�
#if FUNC_VALID_AMP == ON
		// �ԊO��
		P7 &= ~0x80;
		P7 |= 0x40;
		VSBIAS = 0x09;		/* 0.5 */
		R_DAC_Change_OutputVoltage_12bit( 0x07BD );
		wait_ms( 1 );
		pga_do();
		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);
		R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
		work_uw = bufferH_dbg;
		work_uw <<= 16;
		work_uw += bufferL_dbg;
		
		s_unit.meas.info.dat.sekigaival = (( work_uw >> 16 ) & 0x0000FFFF );
//		s_unit.meas.info.dat.sekigaival = work_uw;
//		s_unit.meas.info.dat.sekigaival >>= 8;
		
		R_DAC_Change_OutputVoltage_12bit( 0x0000 );		//RD8001�b��FDAC�ύX���莞�ԑ҂��Ȃ��Ɠd���������؂�Ȃ��l�Ɏv��
		
		// �ԐF��
		P7 &= ~0x40;
		P7 |= 0x80;

//			VSBIAS = 0x0E;		/* 1.0 *///oneA�l�w���ݒ�@������������
		VSBIAS = 0x09;		/* 0.5 */
//			R_DAC_Change_OutputVoltage_12bit( 0x014A );//oneA�l�w���ݒ�@������������ 
		R_DAC_Change_OutputVoltage_12bit( 0x02BD ); 
		wait_ms( 1 );
		pga_do();
		R_PGA_DSAD_Get_AverageResult(&bufferH_dbg_ave, &bufferL_dbg_ave);
		R_PGA_DSAD_Get_Result(&bufferH_dbg, &bufferL_dbg);
		// RD8001�b��F�G���f�B�A�����}�j���A����ǂ�ł��m�؂����ĂȂ�
		work_uw = bufferH_dbg;
		work_uw <<= 16;
		work_uw += bufferL_dbg;
		
		s_unit.meas.info.dat.sekishoku_val = (( work_uw >> 16 ) & 0x0000FFFF );
//		s_unit.meas.info.dat.sekishoku_val = work_uw;
//		s_unit.meas.info.dat.sekishoku_val >>= 8;

		// �����x�Z���T�擾
		main_acl_read();
		
		// �}�C�N�p(�ċz�A�C�r�L)�J�n
		R_DAC_Change_OutputVoltage_12bit( 0x0000 );
		adc_do( &s_unit.dench_val, &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
#endif
#if 0
		//�Z���T�[�f�[�^���M
		//200ms�Ɉ�x
		if( s_unit.sensing_cnt++ >= 4 ){
			main_cpu_com_snd_sensor_data();
			s_unit.sensing_cnt = 0;
		}
#else
		main_cpu_com_snd_sensor_data();
#endif
		s_unit.main_cyc_req  = OFF;
		R_DAC_Change_OutputVoltage_12bit( 0x02BD );		//DAC��ԐF�p�ɕύX
	}

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
	
	pow_sw = drv_read_pow_sw();
	
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
	uint8_t tx_data[UART1_STR_MAX] = {0};
//	uint8_t * const s = &tx_data[0];
	int len;
	
#if 0
	s_unit.sekishoku_val = 1;
	s_unit.sekigaival = 2;
	s_unit.kokyu_val = 3;
	s_unit.ibiki_val = 4;
#endif

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
	
	
	com_srv_send( &tx_data[0], len );


#if 0
	len = sprintf((char*)tx_data, "%d,%d,%d\r\n", s_unit.hour,
												  s_unit.min,
												  s_unit.sec );
	com_srv_send( &tx_data[0], len );
#endif




//	com_srv_send( s, len );
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

int main_eep_write(void)
{
	WR_EEP_RECORD		wr;
	UH wr_adrs;
	UB device_adrs;
	int ret = INVALID;
	UH record_offset = 0;

#if 0
	UB adrs[2];

	// �y�[�W�����̒f��
	UB page_bit;
	int eep_adrs;
	int eep_page;
	UB  device_adrs;
#endif
	
	// ���R�[�h�ő�
	if( s_unit.eep_wr_record_cnt < EEP_RECORD_1P_MAX ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( s_unit.eep_wr_record_cnt < EEP_RECORD_2P_MAX ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
		record_offset = EEP_RECORD_1P_MAX;
	}else{
		return ret;
	}

#if 0
	// �y�[�W�����̒f��
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
	wr.record.byte[0] = (UB)(( wr_adrs >> 8 ) & 0xff );		//�A�h���X���
	wr.record.byte[1] = (UB)( wr_adrs & 0x00ff );		//�A�h���X����
	wr.record.data.valid				 = ON;			/* ���R�[�h�L��/����				*/
	
	wr.record.data.kokyu_val			 = s_unit.eep.record.data.kokyu_val;		
	wr.record.data.ibiki_val			 = s_unit.eep.record.data.ibiki_val;		
	wr.record.data.sekishoku_val		 = s_unit.eep.record.data.sekishoku_val;	// �������ׂ͂̈ɕ�������
	wr.record.data.sekigaival			 = s_unit.eep.record.data.sekigaival;		// �������ׂ͂̈ɕ�������
	wr.record.data.acl_x				 = s_unit.eep.record.data.acl_x;
	wr.record.data.acl_y				 = s_unit.eep.record.data.acl_y;
	wr.record.data.acl_z				 = s_unit.eep.record.data.acl_z;
	
	
	//��������
	i2c_write_sub( device_adrs, &wr.record.byte[0], sizeof(wr), ON );	

//	eep_tx_data[0] = (UB)( wr_adrs & 0x00ff );		//�A�h���X����
//	eep_tx_data[1] = (UB)(( wr_adrs >> 8 ) & 0xff );		//�A�h���X���
//	eep_tx_data[14]  = 1;
//	i2c_write_sub( EEP_DEVICE_ADR, &eep_tx_data[0], sizeof(wr) );
	
	s_unit.eep_wr_record_cnt++;
	
	ret = VALID;
	
	return ret;
}

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

int main_eep_read(void)
{
	int ret = INVALID;
	UH read_adrs;
	EEP_RECORD		rd;
	UB device_adrs;
	UH record_offset = 0;
	
	// ���R�[�h�ő�
	if( s_unit.eep_rd_record_cnt < EEP_RECORD_1P_MAX ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( s_unit.eep_rd_record_cnt < EEP_RECORD_2P_MAX ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
		record_offset = EEP_RECORD_1P_MAX;
	}else{
		return ret;
	}
	
	read_adrs = ( s_unit.eep_rd_record_cnt - record_offset ) * EEP_RECORD_SIZE;
	i2c_read_sub( device_adrs, read_adrs, &s_unit.eep.record.byte[0], sizeof(rd) );
//	i2c_read_sub( EEP_DEVICE_ADR, read_adrs, &eep_rx_data[0], 32 );
	
	s_unit.eep_rd_record_cnt++;
	
	ret = VALID;
	
	return ret;
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

void get_mode( void )
{
	com_srv_puts( (const B *__near)"START\r\n" );

//	wait_ms(10);

	// �Q�b�g���[�h���[�v
	s_unit.eep_rd_record_cnt = 0;
	while(1){
		if( INVALID == main_eep_read() ){
			// �I��
			break;
		}
		wait_ms(10);
		if( 1 == s_unit.eep.record.data.valid ){
			// �L�����R�[�h
#if 0		// �b��
			s_unit.kokyu_val	 = s_unit.eep.record.data.kokyu_val;		
			s_unit.ibiki_val	 = s_unit.eep.record.data.ibiki_val;		
			s_unit.sekishoku_val = s_unit.eep.record.data.sekishoku_val;	// �������ׂ͂̈ɕ�������
			s_unit.sekigaival	 = s_unit.eep.record.data.sekigaival;		// �������ׂ͂̈ɕ�������
#endif
			
//			wait_ms(5);
			main_send_uart1();
//			wait_ms(5);
		}else{
			// �I��
			break;
		}
	}
	main_eep_erase_all();

	s_unit.eep_rd_record_cnt = 0;
	s_unit.eep_wr_record_cnt  =0;
	s_unit.system_mode = SYSTEM_MODE_NON;
	
	com_srv_puts( (const B *__near)"END\r\n" );
}


void main_eep_erase_all(void)
{
	WR_EEP_RECORD wr;
	UB device_adrs =0;
	UH record_offset = 0;
	UH wr_adrs = 0;
	
	s_unit.eep_wr_record_cnt = 0;
	
	while(1){
		// ���R�[�h�ő�
		if( s_unit.eep_wr_record_cnt < EEP_RECORD_1P_MAX ){
			device_adrs = EEP_DEVICE_ADR;
		}else if( s_unit.eep_wr_record_cnt < EEP_RECORD_2P_MAX ){
			device_adrs = EEP_DEVICE_ADR | 0x02;
			record_offset = EEP_RECORD_1P_MAX;
		}else{
			return;
		}
		
		// �g�p���݂̂ŏI��
#if 0
		if( s_unit.eep_rd_record_cnt < s_unit.eep_wr_record_cnt ){
			return;
		}
#endif
		
		wr_adrs = ( s_unit.eep_wr_record_cnt - record_offset ) * EEP_RECORD_SIZE;
		wr.record.byte[0] = (UB)(( wr_adrs >> 8 ) & 0xff );		//�A�h���X���
		wr.record.byte[1] = (UB)( wr_adrs & 0x00ff );		//�A�h���X����
		wr.record.data.valid				 = OFF;			/* ���R�[�h�L��/����				*/
		
#if 0		// �b��
		wr.record.data.kokyu_val			 = 0;
		wr.record.data.ibiki_val			 = 0;
		wr.record.data.sekishoku_val		 = 0;
		wr.record.data.sekigaival			 = 0;
#endif
			
		//��������
		i2c_write_sub( device_adrs, &wr.record.byte[0], sizeof(wr), ON );	
		wait_ms(5);
	
		s_unit.eep_wr_record_cnt++;
	}
	
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
	
	
	i2c_read_sub( ACL_DEVICE_ADR, 0x1A, &rd_data[0], 1 );
	
	i2c_read_sub( ACL_DEVICE_ADR, 0x16, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & 0x10 )){
		return;
	}

	i2c_read_sub( ACL_DEVICE_ADR, 0x06, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
}

void main_acl_write(void)
{
//	main_acl_read_sub( ACL_DEVICE_ADR, 0x0C, &rd_data[0], 1 );
}

void test_code_eep(void)
{
#if 0
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
	eep_tx_data[2] = 11;
	eep_tx_data[3] = 22;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 );
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
	eep_tx_data[2] = 33;
	eep_tx_data[3] = 44;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
	eep_tx_data[2] = 55;
	eep_tx_data[3] = 66;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
	eep_tx_data[2] = 77;
	eep_tx_data[3] = 88;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 );
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 0, &eep_rx_data[0], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 5, &eep_rx_data[2], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
///	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 8, &eep_rx_data[4], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 13, &eep_rx_data[6], 2 );

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
	//�f�o�b�O�R�[�h
	s_unit.eep_wr_record_cnt = 0;
	
	s_unit.kokyu_val  = 1111;		
	s_unit.ibiki_val  = 2222;		
	s_unit.sekishoku_val = 333333;	// �������ׂ͂̈ɕ�������
	s_unit.sekigaival = -444444;		// �������ׂ͂̈ɕ�������
    main_eep_write();
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}
	s_unit.eep_wr_record_cnt = 4096;
	
	s_unit.kokyu_val  = 3333;		
	s_unit.ibiki_val  = 4444;		
	s_unit.sekishoku_val = 555555;	// �������ׂ͂̈ɕ�������
	s_unit.sekigaival = -666666;		// �������ׂ͂̈ɕ�������
    main_eep_write();
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}



	s_unit.eep_wr_record_cnt = 4095;
	
	s_unit.kokyu_val  = 2222;		
	s_unit.ibiki_val  = 3333;		
	s_unit.sekishoku_val = 444444;		// �������ׂ͂̈ɕ�������
	s_unit.sekigaival = -555555;		// �������ׂ͂̈ɕ�������
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
	s_unit.sekishoku_val = 333333;	// �������ׂ͂̈ɕ�������
	s_unit.sekigaival = -444444;		// �������ׂ͂̈ɕ�������
	while(1){
		if( INVALID == main_eep_write() ){
			// �I��
			break;
		}
		wait_ms(5);
//		for(i = 0; i < 500; i++ ){
//			WAIT_10US()
//		}
	}	
	
#endif	
	
	
}


static void stop_in( void )
{
	//RD8001�b��F�˓����A�����Ɋւ��ėv����
	com_srv_puts( (const B *__near)"STOP IN\r\n" );
	wait_ms( 10 );
	
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	
	// ���荞�ݎ~�߂鏈��
	TMKAMK = 1U;    	  /* disable INTIT interrupt */
	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	STOP();
	
	// STOP���[�h��������(�N���b�N������~+�E�F�C�g)�@��CPU�}�j���A���@Page 1066 of 1325
	// �N���b�N������~�F18 ��s �`65 ��s
	// �E�G�C�g�F�x�N�^���荞�ݏ������s���ꍇ(7�N���b�N)
	//             �x�N�^���荞�ݏ������s��Ȃ��ꍇ(1�N���b�N)
	wait_ms( 1 );
	TMKAMK = 0U;    	  /* disable INTIT interrupt */
	TMKAIF = 0U;  	    /* clear INTIT interrupt flag */

	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	com_srv_puts( (const B *__near)"STOP OUT\r\n" );
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
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_SENSOR_DATA;
	memcpy( &s_ds.cpu_com.order.snd_data[0], &s_unit.meas.info.byte[0], CPU_COM_SND_DATA_SIZE_SENSOR_DATA );
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_SENSOR_DATA;
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
					s_cpu_com_rcv_func_tbl[i].func(&s_ds.cpu_com.input.rcv_data[0]);
				}
			}
		}
		// ��M�R�}���h�N���A ���b��
		s_ds.cpu_com.input.rcv_cmd = 0x00;
	}
	
}

STATIC void main_cpu_com_rcv_sts( void )
{
	if( SYSTEM_MODE_NON == s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode_chg_req;
		s_ds.cpu_com.order.snd_data[1] = 0;
		s_ds.cpu_com.order.snd_data[2] = 0;
		s_ds.cpu_com.order.snd_data[3] = 0;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_NON;
	}else if( SYSTEM_MODE_SENSOR == s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode_chg_req;
		s_ds.cpu_com.order.snd_data[1] = 0;
		s_ds.cpu_com.order.snd_data[2] = 0;
		s_ds.cpu_com.order.snd_data[3] = 0;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_mode_chg_req = SYSTEM_MODE_HD_CHG_NON;
	}else{
		// �������Ȃ�
		
		
		
	}


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
		time_soft_set_10ms( TIME_TYPE_10MS_POW_SW_ACT_START, TIME_10MS_CNT_POW_SW_ACT_START );
		s_unit.sensing_end_flg = OFF;
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




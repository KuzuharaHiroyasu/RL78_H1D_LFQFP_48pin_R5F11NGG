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
STATIC UB main_sleep_check(void);
STATIC void user_main_mode_sensor(void);
STATIC W main_ad24_to_w( UH bufferH, UH bufferL );
STATIC void main_init(void);
STATIC void sw_cyc(void);
STATIC void mode_cyc(void);
STATIC void main_cyc(void);
STATIC void disp_cyc(void);
STATIC void disp_ptn_order( void );
STATIC void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
STATIC void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
STATIC void main_acl_init(void);
STATIC void main_acl_stop(void);
STATIC void main_acl_start(void);
STATIC void main_acl_read(void);
STATIC void stop_in( void );
STATIC void stop_restore_ram( void );
STATIC void main_cpu_com_snd_sensor_data( void );
STATIC void main_cpu_com_proc(void);
STATIC void main_cpu_com_rcv_sts( void );
STATIC void main_cpu_com_rcv_sensing_order( void );
STATIC void main_cpu_com_rcv_date_set( void );
STATIC void main_cpu_com_rcv_pc_log( void );
STATIC void main_cpu_com_rcv_mode_chg( void );
STATIC void main_cpu_com_rcv_prg_hd_ready(void);
STATIC void main_cpu_com_rcv_prg_hd_check(void);
STATIC void main_cpu_com_rcv_disp(void);
STATIC void main_cpu_com_rcv_version(void);

/************************************************************/
/* �ϐ���`													*/
/************************************************************/
STATIC T_UNIT s_unit;
STATIC DS s_ds;

/************************************************************/
/* �e�[�u����`												*/
/************************************************************/
// �o�[�W����(APL)
const B		version_product_tbl[]= {0, 0, 0, 24};				/* �\�t�g�E�F�A�o�[�W���� */
																/* �o�[�W�����\�L���[�� */
																/* �@���W���[�o�[�W�����F[0 �` 9] */
																/* �A�}�C�i�[�o�[�W�����F[0 �` 9]  */
																/* �B���r�W�����F[0 �` 9] */
																/* �C�r���h�o�[�W�����F[0 �` 9] */

// �o�[�W�����̃A�h���X(BOOT)
const UB	* const version_boot_tbl= (const UB*)0x00006EF0;	/* �u�[�g�o�[�W�����A�h���X */

/* ��M�f�[�^���� �֐��e�[�u�� */
STATIC const CPU_COM_RCV_CMD_TBL s_cpu_com_rcv_func_tbl[CPU_COM_CMD_MAX] = {
	/* �R�}���h */		/* �֐�  */					
	{	0x00,			NULL							},	/* �yCPU�ԒʐM�R�}���h�z�R�}���h����				*/
	{	0xE0,			main_cpu_com_rcv_sts			},	/* �yCPU�ԒʐM�R�}���h�z�X�e�[�^�X�v��				*/
	{	0xA0,			NULL							},	/* �yCPU�ԒʐM�R�}���h�z�Z���T�[�f�[�^�X�V			*/
	{	0xA1,			main_cpu_com_rcv_sensing_order	},	/* �yCPU�ԒʐM�R�}���h�z�Z���T�V���O�w��			*/
	{	0xB0,			main_cpu_com_rcv_mode_chg		},	/* �yCPU�ԒʐM�R�}���h�z��ԕύX(G1D)				*/
	{	0xF0,			main_cpu_com_rcv_pc_log			},	/* �yCPU�ԒʐM�R�}���h�zPC���O���M(�����R�}���h)	*/
	{	0xB1,			main_cpu_com_rcv_date_set		},	/* �yCPU�ԒʐM�R�}���h�z�����ݒ�					*/
	{	0xD5,			main_cpu_com_rcv_prg_hd_ready	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������			*/
	{	0xD2,			NULL							},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���J�n			*/
	{	0xD4,			NULL							},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������			*/
	{	0xD0,			NULL							},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���f�[�^		*/
	{	0xD1,			NULL							},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������			*/
	{	0xD3,			main_cpu_com_rcv_prg_hd_check	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���m�F			*/
	{	0xB2,			main_cpu_com_rcv_disp			},	/* �yCPU�ԒʐM�R�}���h�z�\���w��					*/
	{	0xB3,			main_cpu_com_rcv_version		},	/* �yCPU�ԒʐM�R�}���h�z�o�[�W����					*/
};


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
//	UW time = 0;
    
	vect_set_tbl_apl();	

    main_init();
	
#if 0
	// RTC�ݒ�
	{
		rtc_counter_value_t rtc;
		
		if( dbg_set_clock == 7 ){
	//		R_RTC_Set_CounterValue( rtc );
		}
		R_RTC_Get_CounterValue( &rtc );
		s_unit.hour = rtc.hour;
		s_unit.min = rtc.min;
		s_unit.sec = rtc.sec;
	//	R_RTC_Start();
	}
#endif
	
	// �N�����O
	com_srv_log_title();

	while (1U)
	{
		com_srv_cyc();
		sw_cyc();
		main_cpu_com_proc();
		mode_cyc();				// main_cpu_com_proc()�̌��Ŏ��{ ��CPU�Ԏ�M��̏���
		cpu_com_proc();
		main_cyc();
		disp_cyc();
		
		if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
			user_main_mode_sensor();
		}else if( SYSTEM_MODE_IDLE_REST == s_unit.system_mode ){
			// �X���[�v���Ȃ�
			// RD8001�b��F�c�ʕ\���̈ꕔ�͕\���Ԋu���ׂ����ׂɃX���[�v����Ɛ���ɓ��삵�Ȃ��B�\���̕ύX����]����B
		}else{
			if( ON == main_sleep_check() ){
					stop_in();
			}
		}

		if( OFF == s_unit.non_wdt_refresh ){
			wdt_refresh();		//WDT���t���b�V��
		}
	}
}


/************************************************************************/
/* �֐�     : main_sleep_check											*/
/* �֐���   : �X���[�v�`�F�b�N											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ON		sleep�L��										*/
/*          : OFF		sleep����										*/
/* �ύX���� : 2018.09.12  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* �X���[�v�`�F�b�N														*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
STATIC UB main_sleep_check(void)
{
	UB ret = OFF;
	
	// �X���[�v�˓���������
	if(( OFF == drv_intp_read_g1d_int() ) &&		// G1D����̊��荞��
#if FUNC_DEBUG_LOG == ON 
	   ( OFF == drv_uart0_get_send() )    &&		// ���O�ʐM
#endif
	   ( CPU_COM_CMD_NONE == s_ds.cpu_com.order.snd_cmd_id ) &&		// CPU�ԃ~�h�����M
	   ( CPU_COM_CMD_NONE == s_ds.cpu_com.input.rcv_cmd ) &&		// CPU�ԃ~�h����M
		   ( OFF == drv_uart1_get_send() )){						// CPU�ԃh���C�o���M
		ret = ON;
	}
	return ret;
}




/************************************************************************/
/* �֐�     : user_main_mode_sensor										*/
/* �֐���   : �Z���V���O����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.12  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* �Z���V���O����														*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
#if FUNC_DEBUG_RAW_VAL_OUT == ON
UB dbg_send_cnt;
char dbg_tx_data[50] = {0};
int dbg_len;
#endif
// RD8001�b��F�����^�C�~���O�̐���d�l����
STATIC void user_main_mode_sensor(void)
{
	UW time;
//	UH bufferH_ave;		// ���ϗp
//	UH bufferL_ave;		// ���ϗp		
	UH bufferH;
	UH bufferL;
	
	
//	MEAS meas;
	if( ON == s_unit.sensing_end_flg ){
		return;
	}
	
	// �Z���V���O�x�� ���܂��X���[�v���Ȃ��̂Ń\�t�g�^�C�}�[�͗L��
	time_soft_get_10ms( TIME_TYPE_10MS_SENSING_DELAY, &time );
	if( 0 != time ){
		return;
	}
	
	// 12���Ԕ���
	if( s_unit.sensing_cnt_50ms >= HOUR12_CNT_50MS ){
		s_unit.system_evt = EVENT_COMPLETE;
		s_unit.sensing_end_flg = ON;
	}
	
	// �Z���V���O��SW��������
	if( ON == drv_intp_read_pow_sw() ){
		INC_MAX( s_unit.sensing_sw_on_cnt, UCHAR_MAX );
	}else{
		if( SENSING_SW_LONG_CNT_50MS <= s_unit.sensing_sw_on_cnt ){
			// �K�莞�Ԉȏ�A�������Ɣ��f
			s_unit.system_evt = EVENT_POW_SW_LONG;
			s_unit.sensing_end_flg = ON;
		}
		s_unit.sensing_sw_on_cnt = 0;
	}
	
	// �d�r�c�ʒቺ
	if( DENCH_ZANRYO_STS_MIN == s_unit.dench_sts ){
		s_unit.sensing_end_flg = ON;
	}
	// �[�dON
	if( ON == s_unit.bat_chg ){
		s_unit.sensing_end_flg = ON;
	}
	
	// �Z���T�[���[�h
	// ���C������(50ms)
	if( ON == s_unit.main_cyc_req ){
		s_unit.sensing_cnt_50ms++;		//12���ԃJ�E���g�A�b�v�͂��̉ӏ��ōs�� ���ŏI�����[�X����50ms���ɂ������O���ł��N�������

		R_DAC1_Start();			// ��DAC ON�@�����ʗp�r

		// �����}�C�N�p(�ċz�A�C�r�L)�J�n����
		R_DAC1_Set_ConversionValue( 0x0000 );
		R_AMP0_Start();		// ��AMP0 ON
		R_PGA1_Start();		// ��PGA1 ON

		wait_ms( 2 );
		adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
		
		wait_ms( 2 );
#if 0	// RD8001�b��FAMP0,PGA1��OFF����Ɛ���ɃC�r�L�����擾�ł��Ȃ��B����d�l
		R_AMP0_Stop();		// ��AMP0 OFF
		R_PGA1_Stop();		// ��PGA1 OFF
#endif
		// �����}�C�N�p(�ċz�A�C�r�L)�I������

		R_PGA_DSAD_Start();		// ��PGA0_DSAD  ON
		R_AMP2_Start();			// ��AMP2       ON
		
#if FUNC_VALID_AMP == ON
		R_DAC1_Set_ConversionValue( 0x0500 );
		// �ԐF��ON
		drv_o_port_sekigai( OFF );
		drv_o_port_sekishoku( ON );
		wait_ms( 2 );
		

		pga_do();
//		R_PGA_DSAD_Get_AverageResult(&bufferH_ave, &bufferL_ave);		//����
		R_PGA_DSAD_Get_Result(&bufferH, &bufferL);
		s_unit.meas.info.dat.sekishoku_val = main_ad24_to_w( bufferH, bufferL );
		wait_ms( 2 );

		// �ԊO��ON
		if( ON == s_unit.sensing_sekigai_flg ){
			drv_o_port_sekigai( ON );
			drv_o_port_sekishoku( OFF );

			R_DAC1_Set_ConversionValue( 0x0B00 );
			wait_ms( 2 );
			pga_do();
	//		R_PGA_DSAD_Get_AverageResult(&bufferH_ave, &bufferL_ave);		//����
			R_PGA_DSAD_Get_Result(&bufferH, &bufferL);
			s_unit.meas.info.dat.sekigaival = main_ad24_to_w( bufferH, bufferL );
			
			R_PGA_DSAD_Stop();		// ��PGA0_DSAD  OFF
			R_AMP2_Stop();			// ��AMP2  OFF
			
			wait_ms( 2 );
			// �ԊO��,�ԐF��OFF
			drv_o_port_sekigai( OFF );
			drv_o_port_sekishoku( OFF );
			
			R_DAC1_Stop();			// ��DAC  OFF
		}else{
			// �������̓f�[�^�N���A���Ď~�߂�B�҂��s�v
			s_unit.meas.info.dat.sekigaival = 0;
			R_PGA_DSAD_Stop();		// ��PGA0_DSAD  OFF
			R_AMP2_Stop();			// ��AMP2  OFF
			
			// �ԊO��,�ԐF��OFF
			drv_o_port_sekigai( OFF );
			drv_o_port_sekishoku( OFF );
			
			R_DAC1_Stop();		// ��DAC  OFF
		}
		
		// �����x�Z���T�擾(10��(500ms��)��1��)
#if FUNC_DEBUG_RAW_VAL_OUT == OFF
		if( ++s_unit.acl_sensor_timing >= ACL_TIMING_VAL ){
			s_unit.acl_sensor_timing = 0;
			main_acl_read();
		}
		
#else
		//���l�f�o�b�O�o�́@�������x�͖���
		if( dbg_send_cnt++ >= 0 ){	
			dbg_send_cnt = 0;
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
		
		s_unit.main_cyc_req  = OFF;
		
		// �ԊO�A�ԐFOFF
		drv_o_port_sekigai( OFF );
		drv_o_port_sekishoku( OFF );
		
		// ���M�����҂�
		cpu_com_send_end_wait();		// ���M�����҂�
#if 1
		stop_in();		//�@�X�g�b�v���[�h
#endif
		// �f�[�^���M�J�n ���X�g�b�v���[�h�O���ƃX�g�b�v���[�h�ւ̓˓����o���Ȃ�����Z���V���O���Ȃ��瑗�M����s���čs���B
		main_cpu_com_snd_sensor_data();	
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
STATIC void main_init(void)
{
	wdt_refresh();		//WDT���t���b�V��

	//�f�[�^������
	memset( &s_unit, 0, sizeof(s_unit) );
	memset( &s_ds, 0, sizeof(s_ds) );
	
	//�f�[�^������(0�ȊO)
	s_unit.disp.last_ptn = 0xFF;
	
	// �~�h��������
	com_srv_init();
	cpu_com_init();
	time_init();
	
	EI();
	main_acl_init();	// �����x�Z���T������(���荞�ݎg�p)

}

/************************************************************************/
/* �֐�     : sw_cyc													*/
/* �֐���   : SW��������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.03.02 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* SW��������															*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC void sw_cyc(void)
{
	UB pow_sw;
	UW time = 0;
	
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		// �Z���V���O���̓\�t�g�E�F�A�^�C�}�[���g�p�ł��Ȃ��Z���V���O��������SW���s��
		return;
	}
	
	pow_sw = drv_intp_read_pow_sw();
	
	s_unit.disp.ptn_inital_order = OFF;
	
	time_soft_get_10ms( TIME_TYPE_10MS_POW_SW, &time );
	
	if( ON == pow_sw ){		// ON����
		// �Ȃɂ����Ȃ�(�d��SW�����^�C�}�[�p��)
		if( 0 == time ){
			s_unit.disp.ptn_inital_order = ON;		// 10�b�ȏ㉟�������Ă��鎞�����\��ON
		}
	}else{					// OFF����
		if( ON == s_unit.pow_sw_last ){
			// ON��OFF�G�b�W
			if( time <= ( TIME_10MS_CNT_POW_SW_INITIAL_DISP - TIME_10MS_CNT_POW_SW_LONG )){
				// �K�莞�Ԉȏ�A�������Ɣ��f
				s_unit.system_evt = EVENT_POW_SW_LONG;
//				s_unit.disp.ptn_sw_long_order = ON;		// �������\���w�� ���e��22�FG1D����̒ʒm�ɕύX
				// �d��SW�����^�C�}�[�ăX�^�[�g 
//				time_soft_set_10ms( TIME_TYPE_10MS_SW_LONG_DISP, TIME_10MS_CNT_POW_SW_LONG_DISP );
				// �Z���V���O�̏����Ɉڍs
//				if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
//					s_unit.sensing_end_flg = ON;
//				}
			}else if( time <= ( TIME_10MS_CNT_POW_SW_INITIAL_DISP - TIME_10MS_CNT_POW_SW_SHORT )){
				s_unit.system_evt = EVENT_POW_SW_SHORT;
			}else{
				// �������Ȃ�
			}
		}
		// �d��SW�����^�C�}�[�ăX�^�[�g 
		time_soft_set_10ms( TIME_TYPE_10MS_POW_SW, TIME_10MS_CNT_POW_SW_INITIAL_DISP );
	}
	s_unit.pow_sw_last = pow_sw;
	
	time_soft_get_10ms( TIME_TYPE_10MS_SW_LONG_DISP, &time );
	if( 0 == time ){
		s_unit.disp.ptn_sw_long_order = OFF;		// �������\���w��
	}
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
STATIC void mode_cyc(void)
{
	
	// �V�X�e�����[�h�ύX������
	if( s_unit.last_system_mode != s_unit.system_mode ){
		if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
			time_soft_set_10ms( TIME_TYPE_10MS_SENSING_DELAY, TIME_10MS_CNT_SENSING_DELAY );
			s_unit.sensing_end_flg = OFF;
			s_unit.sensing_cnt_50ms = 0;
			
			// �Z���V���O���L���@�\
			main_acl_start();
			drv_o_port_mike( ON );
		}else{
			
			// �Z���V���O���ȊO�����@�\ ������d���ጸ�̈�
			main_acl_stop();
			drv_o_port_mike( OFF );
		}
		
	}
	
	s_unit.last_system_mode = s_unit.system_mode;
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
	UB bat_chg_1;
	UB bat_chg_2;
	
	UB kensa_1;
	UB kensa_2;
//	UB bat_chg_off_trig = OFF;
	UW time = 0;
	
	if(( ON == s_unit.sensing_end_flg ) && 
	   ( SYSTEM_MODE_SENSING == s_unit.system_mode )){
		// �Z���V���O�I���҂����͈ȉ��̏�Ԃ��X�V���Ȃ� ��G1D�Ƃ̏�ԕs��v��h������
		return;
	}
	
	// �d�r�c��
	dench_val = DENCH_ZANRYO_1_VAL;
	
	adc_dench( &dench_val );
	
	if( dench_val >= DENCH_ZANRYO_1_VAL ){
		s_unit.dench_sts = DENCH_ZANRYO_STS_MAX;
	}else if( dench_val >= DENCH_ZANRYO_2_VAL ){
		s_unit.dench_sts = DENCH_ZANRYO_STS_HIGH;
	}else if( dench_val >= DENCH_ZANRYO_3_VAL ){
		s_unit.dench_sts = DENCH_ZANRYO_STS_LOW;
	}else{
		s_unit.dench_sts = DENCH_ZANRYO_STS_MIN;
	}
	
	
	// �[�d���(�|�[�g) ��2���v
	bat_chg_1 = drv_i_port_bat_chg();
	bat_chg_2 = drv_i_port_bat_chg();
	if( bat_chg_1 == bat_chg_2 ){
		// 2���v�ōX�V
		s_unit.bat_chg = bat_chg_1;
	}
	if(( OFF == s_unit.bat_chg_last ) &&
	   ( ON == s_unit.bat_chg )){	
		time_soft_set_10ms( TIME_TYPE_10MS_CHG_ON_DISP_IVALID, TIME_10MS_CNT_CHG_ON_DISP_IVALID );
	}
	
	// �[�d�����`�F�b�N
	if( ON == s_unit.bat_chg ){
		time_soft_set_10ms( TIME_TYPE_10MS_BAT_CHG_FIN, TIME_10MS_CNT_BAT_CHG_FIN );
		// OFF�G�b�W�̋K�莞�Ԍ�Ȃ̂�ON�̏ꍇ�͈�U�[�d������Ԃ͖���Ƃ���
		s_unit.bat_chg_fin_state = BAT_CHG_FIN_STATE_UNKNOWN;
	}else{
		time_soft_get_10ms( TIME_TYPE_10MS_BAT_CHG_FIN, &time );
		if( 0 == time ){
			if( BAT_CHG_FIN_STATE_UNKNOWN == s_unit.bat_chg_fin_state ){
				// �[�d������Ԃ�����(OFF�G�b�W)�̏ꍇ�ɏ[�d������Ԃ��m�肳����
				if( dench_val >= DENCH_BAT_CHG_FIN_VAL ){
					s_unit.bat_chg_fin_state = BAT_CHG_FIN_STATE_ON;
				}else{
					s_unit.bat_chg_fin_state = BAT_CHG_FIN_STATE_OFF;
				}
			}
		}
	}
	s_unit.bat_chg_last = s_unit.bat_chg;


	
	// ������� ��2���v
	kensa_1 = drv_i_port_kensa();
	kensa_2 = drv_i_port_kensa();
	
	if( kensa_1 == kensa_2 ){
		// 2���v�ōX�V
		s_unit.kensa = kensa_1;
	}
	s_unit.kensa_last = s_unit.kensa;
	
}


/************************************************************************/
/* �֐�     : disp_cyc													*/
/* �֐���   : �\����������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.24 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �\����������															*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC void disp_cyc(void)
{
	UW time = 0;
	
	// �Z���T�[���͐ԐFLED����͔�����
	if(( SYSTEM_MODE_SENSING == s_unit.system_mode ) &&
	   ( OFF == s_unit.sensing_end_flg )){
		return;
	}
	
	// �\���p�^�[���w��
	disp_ptn_order();
	
	if( s_unit.disp.last_ptn != s_unit.disp.ptn ){
		s_unit.disp.seq = 0;
		// OFF ����
		// RD8001�b��FLED�����点��ׂ̃_�b�N�ݒ�̍œK�l�Ƃ́H����d�l����`
		if( DISP_PTN_OFF == s_unit.disp.ptn ){
			R_DAC1_Stop();			// ��DAC  OFF
//			R_PGA_DSAD_Stop();		// ��PGA0_DSAD  OFF
			R_AMP2_Stop();			// ��AMP2  OFF
			R_DAC1_Set_ConversionValue( 0x0500 );
		}else{
			R_DAC1_Start();			// ��DAC        ON
//			R_PGA_DSAD_Start();		// ��PGA0_DSAD  ON
			R_AMP2_Start();			// ��AMP2       ON
			R_DAC1_Set_ConversionValue( 0x0500 );
		}
#if 0
		if( DISP_PTN_OFF == s_unit.disp.last_ptn ){
			R_DAC1_Start();			// ��DAC        ON
//			R_PGA_DSAD_Start();		// ��PGA0_DSAD  ON
			R_AMP2_Start();			// ��AMP2       ON
			R_DAC1_Set_ConversionValue( 0x0500 );
		}
#endif
	}
	
	if( DISP_PTN_SELF_CHECK_FLASH_100MS == s_unit.disp.ptn ){
		drv_o_port_sekishoku( OFF );
		time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
		if( 0 == time ){
			time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
			INC_MAX_INI( s_unit.disp.seq, DISP_PTN_SELF_CHECK_FLASH_100MS_SEQ_MAX, 0 );
		}
		if( 0 == s_unit.disp.seq ){
			drv_o_port_sekishoku( ON );
		}else{
			drv_o_port_sekishoku( OFF );
		}
		
	}else if( DISP_PTN_SELF_CHECK_ON == s_unit.disp.ptn ){
		drv_o_port_sekishoku( ON );
	}else if( DISP_PTN_DENCH_ZANRYO_1 == s_unit.disp.ptn ){
		if( 0 == s_unit.disp.seq ){
			drv_o_port_sekishoku( OFF );
			time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_500MS );
			s_unit.disp.seq = 1;
		}else if( 1 == s_unit.disp.seq ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_3S );
				s_unit.disp.seq = 2;
			}
		}else{
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( OFF );
			}
		}
	}else if( DISP_PTN_DENCH_ZANRYO_2 == s_unit.disp.ptn ){
		if( 0 == s_unit.disp.seq ){
			drv_o_port_sekishoku( OFF );
			time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_500MS );
			s_unit.disp.seq = 1;
		}else if( 1 == s_unit.disp.seq ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 2;
			}
		}else if( 2 == s_unit.disp.seq ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 3;
			}
		}else if( 3 == s_unit.disp.seq ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 4;
			}
		}else{
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( OFF );
			}
		}
	}else if( DISP_PTN_DENCH_ZANRYO_3 == s_unit.disp.ptn ){
		if( 0 == s_unit.disp.seq ){
			drv_o_port_sekishoku( OFF );
			time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_500MS );
			s_unit.disp.seq = 1;
		}else if( s_unit.disp.seq < DISP_PTN_FLASH_DENCH_ZANRYO_3_SEQ_MAX ){
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_50MS );
				INC_MAX( s_unit.disp.seq, DISP_PTN_FLASH_DENCH_ZANRYO_3_SEQ_MAX );
				if( s_unit.disp.seq & 0x01 ){
					drv_o_port_sekishoku( ON );
				}else{
					drv_o_port_sekishoku( OFF );
				}
			}
		}else{
			time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
			if( 0 == time ){
				drv_o_port_sekishoku( OFF );
			}
		}
	}else if( DISP_PTN_BLE_ON == s_unit.disp.ptn ){
		time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
		if( 0 == time ){
			if( 0 == s_unit.disp.seq ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 1;
			}else{
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 0;
			}
		}
	}else if( DISP_PTN_BLE_OFF == s_unit.disp.ptn ){
		time_soft_get_10ms( TIME_TYPE_10MS_DISP_FLASH, &time );
		if( 0 == time ){
			if( 0 == s_unit.disp.seq ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 1;
			}else if( 1 == s_unit.disp.seq ){
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 2;
			}else if( 2 == s_unit.disp.seq ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 3;
			}else if( 3 == s_unit.disp.seq ){
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 4;
			}else if( 4 == s_unit.disp.seq ){
				drv_o_port_sekishoku( ON );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_100MS );
				s_unit.disp.seq = 5;
			}else{
				drv_o_port_sekishoku( OFF );
				time_soft_set_10ms( TIME_TYPE_10MS_DISP_FLASH, TIME_10MS_CNT_DISP_FLASH_1S );
				s_unit.disp.seq = 0;
			}
		}
	}else if( DISP_PTN_INITAL_RESET == s_unit.disp.ptn ){
		drv_o_port_sekishoku( ON );
	}else if( DISP_PTN_PW_SW_LONG == s_unit.disp.ptn ){
		drv_o_port_sekishoku( ON );
	}else if( DISP_PTN_CHG_ON == s_unit.disp.ptn ){
		time_soft_get_10ms( TIME_TYPE_10MS_CHG_ON_DISP_IVALID, &time );
		if( 0 == time ){
			drv_o_port_sekishoku( ON );
		}else{
			drv_o_port_sekishoku( OFF );
		}
	}else{
		drv_o_port_sekishoku( OFF );
//		drv_o_port_sekigai( OFF );
	}
	
	// �O��p�^�[���i�[
	s_unit.disp.last_ptn = s_unit.disp.ptn;
}


/************************************************************************/
/* �֐�     : disp_ptn_order											*/
/* �֐���   : �\���p�^�[���w��											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.24 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �\���p�^�[���w��														*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC void disp_ptn_order( void )
{
	s_unit.disp.ptn = DISP_PTN_OFF;
	
	
	// �D��x���Ƀp�^�[���ݒ�
	if( SYSTEM_MODE_SELF_CHECK == s_unit.system_mode ){
		s_unit.disp.ptn = s_unit.disp.ptn_g1d_order;
	}else if( ON == s_unit.bat_chg ){
		s_unit.disp.ptn = DISP_PTN_CHG_ON;
	}else if( ON == s_unit.disp.ptn_inital_order ){
		s_unit.disp.ptn = DISP_PTN_INITAL_RESET;
	}else if( ON == s_unit.disp.ptn_sw_long_order ){
		s_unit.disp.ptn = DISP_PTN_PW_SW_LONG;
	}else if( SYSTEM_MODE_IDLE_REST == s_unit.system_mode ){
		if( DENCH_ZANRYO_STS_MAX == s_unit.dench_sts ){
			s_unit.disp.ptn = DISP_PTN_DENCH_ZANRYO_1;
		}else if( DENCH_ZANRYO_STS_HIGH == s_unit.dench_sts ){
			s_unit.disp.ptn = DISP_PTN_DENCH_ZANRYO_2;
		}else if( DENCH_ZANRYO_STS_LOW == s_unit.dench_sts ){
			s_unit.disp.ptn = DISP_PTN_DENCH_ZANRYO_3;
		}else{
			// �����Ȃ�
		}
	}else if( SYSTEM_MODE_GET == s_unit.system_mode ){
		s_unit.disp.ptn = DISP_PTN_BLE_ON;
	}else if( SYSTEM_MODE_IDLE_COM == s_unit.system_mode ){
		if( ON == s_unit.g1d.info.bit.ble ){
			s_unit.disp.ptn = DISP_PTN_BLE_ON;
		}else{
			s_unit.disp.ptn = DISP_PTN_BLE_OFF;
		}
	}else{
		//�\���s�v
	}
}


/************************************************************************/
/* �֐�     : set_req_main_cyc											*/
/* �֐���   : ���C���T�C�N���^�C�~���O�ݒ�								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.24 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* ���C���T�C�N���^�C�~���O�ݒ�											*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void set_req_main_cyc(void)
{
	s_unit.main_cyc_req = ON;
}



/************************************************************************/
/* �֐�     : err_info													*/
/* �֐���   : �ُ�ʒm													*/
/* ����     : �ُ�ID(10�i2��)											*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �ُ�����O�o��(�f�o�b�O�@�\)									*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �@�f�o�b�O�@�\�ł��B�G���[�o�͏o���Ȃ��\��������܂��B				*/
/************************************************************************/
void err_info( ERR_ID id )
{
#if FUNC_DEBUG_LOG == ON
	UB tx[COM_STR_MAX] = {0};

	tx[0] = 'H';
	tx[1] = '1';
	tx[2] = 'E';
	tx[3] = 'R';
	tx[4] = 'R';
	tx[5] = (id / 10 ) + 0x30;
	tx[6] = (id % 10 ) + 0x30;
	tx[7] = '\r';
	tx[8] = '\n';
	
	// PC���ɃX���[�o��
	if( s_unit.last_err_id != id ){
		com_srv_send( tx, 9 );
	}


#if 0
	while(1){
		// �ُ�ɂ��i�v���[�v
	}
#endif
#endif
	s_unit.err_cnt++;
	s_unit.last_err_id = id;
}

//================================
//ACL�֘A
//================================

/************************************************************************/
/* �֐�     : i2c_write_sub												*/
/* �֐���   : I2C�������݃T�u											*/
/* ����     : device_adrs	�f�o�C�X�A�h���X							*/
/*          : wr_data		�������݃f�[�^								*/
/*          : len			�f�[�^��									*/
/*          : wait_flg		�������݌�̑҂��t���O[���g�p]				*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : I2C�������݃T�u												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg )
{
	UW lock_cnt = 0;

	if( 0 != R_IICA0_Master_Send( device_adrs, wr_data, len, I2C_WAIT)){
		err_info(ERR_ID_I2C);
	}else{
		while(1){
			if( OFF == s_unit.acl_info.i2c_snd_flg ){
				// ���M����
				break;
			}
			if( lock_cnt++ >= I2C_LOCK_ERR_VAL ){
				// �ُ펞
				err_info(ERR_ID_I2C);
				s_unit.acl_info.i2c_err_flg = ON;
				break;
			}
		}
		if( OFF == s_unit.acl_info.i2c_err_flg ){
			R_IICA0_StopCondition();		//�@�ُ픭�����͗\�����ʏ������݂�h�~����ׂɃX�g�b�v�R���f�B�V�������s���Ȃ�
		}
	}
	while(0U == SPD0){}

	if( ON == s_unit.acl_info.i2c_err_flg ){
		s_unit.acl_info.i2c_err_flg = OFF;
		R_IICA0_Create();		// I2C������
	}
}


/************************************************************************/
/* �֐�     : i2c_read_sub												*/
/* �֐���   : I2C�ǂݏo���T�u											*/
/* ����     : device_adrs	�f�o�C�X�A�h���X							*/
/*          : read_adrs		�ǂݏo���A�h���X							*/
/*          : read_data		�ǂݏo���f�[�^								*/
/*          : len			�f�[�^��									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : I2C�ǂݏo���T�u												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len )
{
	UB adrs[2];
	UB adrs_size = 0;
	UW lock_cnt = 0;

	// �����x�Z���T�[��
	adrs[0] = (UB)( read_adrs & 0x00ff );			//�A�h���X
	adrs_size  = 1;

	if( 0 != R_IICA0_Master_Send( device_adrs, &adrs[0], adrs_size, I2C_WAIT )){
		err_info(ERR_ID_I2C);
	}else{
		while(1){
			if( OFF == s_unit.acl_info.i2c_snd_flg ){
				// ��M����
				break;
			}
			if( lock_cnt++ >= I2C_LOCK_ERR_VAL ){
				// �ُ펞
				err_info(ERR_ID_I2C);
				s_unit.acl_info.i2c_err_flg = ON;
				break;
			}
		}
// RD8001�b��F�ǂ̗l(�o�X�J��or�X�g�b�v�R���f�B�V����)�ɑΉ����邩�⍇����
#if 0
	//�X�g�b�v�R���f�B�V��������
		R_IICA0_StopCondition();
	}
	/* �X�g�b�v�R���f�B�V������̑҂� */
	while(0U == SPD0){}
//	WAIT_5US();
//	WAIT_5US();
//	wait_ms(5);
#else
	}
	//�o�X�J��
	R_IICA0_Stop();
	R_IICA0_Create();
	
#endif

	if( 0 != R_IICA0_Master_Receive(device_adrs, read_data, len, I2C_WAIT)){
		err_info(ERR_ID_I2C);
	}else{
		while(1){
			if( OFF == s_unit.acl_info.i2c_rcv_flg ){
				break;
			}
			if( lock_cnt++ >= I2C_LOCK_ERR_VAL ){
				// �ُ펞
				err_info(ERR_ID_I2C);
				s_unit.acl_info.i2c_err_flg = ON;
				break;
			}
		}
		if( OFF == s_unit.acl_info.i2c_err_flg ){
			R_IICA0_StopCondition();		//�@�ُ픭�����͗\�����ʏ������݂�h�~����ׂɃX�g�b�v�R���f�B�V�������s���Ȃ�
		}
	}
	while(0U == SPD0){}
	
	if( ON == s_unit.acl_info.i2c_err_flg ){
		s_unit.acl_info.i2c_err_flg = OFF;
		R_IICA0_Create();		// I2C������
	}
}

/************************************************************************/
/* �֐�     : i2c_set_snd_flg											*/
/* �֐���   : ���M�t���O�ݒ�											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���M�t���O�ݒ�												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void i2c_set_snd_flg( UB data )
{
	s_unit.acl_info.i2c_snd_flg = data;
}

/************************************************************************/
/* �֐�     : i2c_set_snd_flg											*/
/* �֐���   : ��M�t���O�ݒ�											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ��M�t���O�ݒ�												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void i2c_set_rcv_flg( UB data )
{
	s_unit.acl_info.i2c_rcv_flg = data;
}

/************************************************************************/
/* �֐�     : i2c_set_err_flg											*/
/* �֐���   : �G���[�t���O�ݒ�											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �G���[�t���O�ݒ�												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void i2c_set_err_flg( UB data )
{
	s_unit.acl_info.i2c_err_flg = data;
}

/************************************************************************/
/* �֐�     : main_acl_init												*/
/* �֐���   : �����x�Z���T������										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.13 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����x�Z���T������ ��������Ԃ͒�~�ɂ��Ă���					*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_acl_init(void)
{
	UB rd_data[2];
	
	wait_ms( 30 );		// �����x�Z���T�@���d��ON�҂�

	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_WHO_AM_I, &rd_data[0], 1 );
	if( rd_data[0] != ACL_REG_RECOGNITION_CODE ){
		err_info( ERR_ID_ACL );
	}
	
	main_acl_stop();
}

/************************************************************************/
/* �֐�     : main_acl_stop												*/
/* �֐���   : �����x�Z���T��~											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����x�Z���T��~												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_acl_stop(void)
{
	UB rd_data[2];
	UB wr_data[2];
	
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	// ���샂�[�h�ݒ�
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_CTRL_REG1, &rd_data[0], 1 );
	if( rd_data[0] != 0x00 ){
		err_info( ERR_ID_ACL );
	}
}

/************************************************************************/
/* �֐�     : main_acl_stop												*/
/* �֐���   : �����x�Z���T�X�^�[�g										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����x�Z���T�X�^�[�g											*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_acl_start(void)
{
	UB wr_data[2];
	
	// ���샂�[�h������
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	// ���샂�[�h�ݒ�
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x20;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );

	// ���샂�[�h�J�n
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0xA0;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, ON );
	
	
}

/************************************************************************/
/* �֐�     : main_acl_stop												*/
/* �֐���   : �����x�Z���T�Ǐo��										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����x�Z���T�X�Ǐo��											*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_acl_read(void)
{
	UB rd_data[10];
	
	// INT_SOURCE1		
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_INT_SRC1, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & BIT04 )){
		// �f�[�^���B
		err_info( ERR_ID_ACL );
		return;
	}
	
	// �f�[�^�擾
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_DATA_XYZ, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
	// INT_REL�ǂݏo���@�����荞�ݗv���N���A
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_INT_REL, &rd_data[0], 1 );
}


/************************************************************************/
/* �֐�     : stop_in													*/
/* �֐���   : CPU�X�g�b�v���[�h											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.26 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�X�g�b�v���[�h													*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC void stop_in( void )
{
	//RD8001�b��F�˓����A�����Ɋւ��ėv����
#if FUNC_LOG_STOP == ON
	com_srv_puts( (const B *__near)"STOP IN\r\n" );
	wait_ms( 10 );
#endif
	
	// ���荞�ݎ~�߂鏈��
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		// 10ms�^�C�}�[��~
		R_IT8Bit0_Channel0_Stop();
	}else{
		// 50ms�^�C�}�[��~
		R_IT_Stop();
	}
	
	STOP();
	
	// STOP���[�h��������(�N���b�N������~+�E�F�C�g)�@��CPU�}�j���A���@Page 1066 of 1325
	// �N���b�N������~�F18 ��s �`65 ��s
	// �E�G�C�g�F�x�N�^���荞�ݏ������s���ꍇ(7�N���b�N)
	//             �x�N�^���荞�ݏ������s��Ȃ��ꍇ(1�N���b�N)
	Nop5();Nop1();Nop1();
	
	stop_restore_ram();		//RAM����
	
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		// 10ms�^�C�}�[�J�n
		R_IT8Bit0_Channel0_Create();
	}else{
		// 50ms�^�C�}�[�J�n
		R_IT_Create();
	}
//	R_SAU0_Create();

#if FUNC_LOG_STOP == ON
	com_srv_puts( (const B *__near)"STOP OUT\r\n" );
#endif
}

/************************************************************************/
/* �֐�     : stop_restore_ram											*/
/* �֐���   : CPU�X�g�b�v�����RAM��������								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.26 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�X�g�b�v����̕�������											*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC void stop_restore_ram( void )
{
	// �������K�v�ł���ΐ����ǉ�
	
//	if( SYSTEM_MODE_INITIAL == s_unit.system_mode ){
//		s_unit.pow_sw_last = OFF;	//�d���{�^���̉�����OFF�ɕύX
//	}
	
}


// =============================================================
// CPU�ʐM�֘A
// =============================================================
/************************************************************************/
/* �֐�     : main_cpu_com_snd_sensor_data								*/
/* �֐���   : �Z���T�[�f�[�^���M										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �Z���T�[�f�[�^���M											*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_snd_sensor_data( void )
{

	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_SENSOR_DATA;
	memcpy( &s_ds.cpu_com.order.snd_data[0], &s_unit.meas.info.byte[0], CPU_COM_SND_DATA_SIZE_SENSOR_DATA );
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_SENSOR_DATA;
	
	cpu_com_send_start();
}

/************************************************************************/
/* �֐�     : main_cpu_com_proc											*/
/* �֐���   : CPU�ԒʐM��������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
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
					s_cpu_com_rcv_func_tbl[i].func();
				}
			}
		}
		// ��M�R�}���h�N���A
		s_ds.cpu_com.input.rcv_cmd = 0x00;
	}
	
}

/************************************************************************/
/* �֐�     : main_cpu_com_rcv_sts										*/
/* �֐���   : �X�e�[�^�X�v����M										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �X�e�[�^�X�v����M											*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_sts( void )
{
	rtc_counter_value_t rtc_val;
	rtc_counter_value_t rtc_val_bin;
	H1D_INFO h1d;
	h1d.info.byte = 0;
	
	// ��M
	s_unit.g1d.info.byte = s_ds.cpu_com.input.rcv_data[1];
	
	// ���M
	if( MD_OK != R_RTC_Get_CounterValue( &rtc_val ) ){
		err_info( ERR_ID_MAIN );
	}
	
	// BCD���o�C�i���ϊ�
	bcd2bin(&rtc_val_bin.year, &rtc_val.year);
	bcd2bin(&rtc_val_bin.month, &rtc_val.month);
	bcd2bin(&rtc_val_bin.week, &rtc_val.week);
	bcd2bin(&rtc_val_bin.day, &rtc_val.day);
	bcd2bin(&rtc_val_bin.hour, &rtc_val.hour);
	bcd2bin(&rtc_val_bin.min, &rtc_val.min);
	bcd2bin(&rtc_val_bin.sec, &rtc_val.sec);
	
	// h1D���쐬
	if( ON == s_unit.bat_chg ){
		h1d.info.bit.bat_chg = 1;	//�[�d���m�|�[�g
	}
	if( ON == s_unit.kensa ){
		h1d.info.bit.kensa = 1;		//�����|�[�g
	}

	
	if(( BAT_CHG_FIN_STATE_ON == s_unit.bat_chg_fin_state ) && ( EVENT_NON == s_unit.system_evt )){
		s_unit.system_evt = EVENT_CHG_FIN;	//�[�d�����C�x���g
		s_unit.bat_chg_fin_state = EVENT_CHG_FIN;
	}
	
	if( SYSTEM_MODE_NON != s_unit.system_mode ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_evt;
		s_ds.cpu_com.order.snd_data[1] = s_unit.system_mode;
		s_ds.cpu_com.order.snd_data[2] = h1d.info.byte;
		s_ds.cpu_com.order.snd_data[3] = rtc_val_bin.year;
		s_ds.cpu_com.order.snd_data[4] = rtc_val_bin.month;
		s_ds.cpu_com.order.snd_data[5] = rtc_val_bin.week;
		s_ds.cpu_com.order.snd_data[6] = rtc_val_bin.day;
		s_ds.cpu_com.order.snd_data[7] = rtc_val_bin.hour;
		s_ds.cpu_com.order.snd_data[8] = rtc_val_bin.min;
		s_ds.cpu_com.order.snd_data[9] = rtc_val_bin.sec;
		s_ds.cpu_com.order.snd_data[10] = s_unit.dench_sts;

		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
		s_unit.system_evt = EVENT_NON;
	}else{
		// �������Ȃ�
		
	}
}

/************************************************************************/
/* �֐�     : main_cpu_com_rcv_sensing_order							*/
/* �֐���   : �Z���V���O�w��											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �Z���V���O�w��												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_sensing_order( void )
{
	s_unit.sensing_sekigai_flg = s_ds.cpu_com.input.rcv_data[0];
}


/************************************************************************/
/* �֐�     : main_cpu_com_rcv_date_set									*/
/* �֐���   : �����ݒ��M												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����ݒ��M													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
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
	
	// �o�C�i����BCD�ϊ�
	rtc_val.year  = bin2bcd(rtc_val.year);
	rtc_val.month = bin2bcd(rtc_val.month);
	rtc_val.week  = bin2bcd(rtc_val.week);
	rtc_val.day   = bin2bcd(rtc_val.day);
	rtc_val.hour  = bin2bcd(rtc_val.hour);
	rtc_val.min   = bin2bcd(rtc_val.min);
	rtc_val.sec   = bin2bcd(rtc_val.sec);
	
	
	if( MD_OK != R_RTC_Set_CounterValue( rtc_val ) ){
		err_info( ERR_ID_MAIN );
	}
	
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DATE_SET;
	s_ds.cpu_com.order.snd_data[0] = 0;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DATE;
}

/************************************************************************/
/* �֐�     : main_cpu_com_rcv_pc_log									*/
/* �֐���   : PC���O��M												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : PC���O��M													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_pc_log( void )
{
#if FUNC_DEBUG_LOG == ON
	int len;
	UB tx_data[COM_STR_MAX] = {0};

	memcpy( &tx_data[0], &s_ds.cpu_com.input.rcv_data[0], CPU_COM_RCV_DATA_SIZE_PC_LOG );
	
	len = strlen( (const char *)tx_data );		// singed char(B)�ł��x�����o��̂�char�ŃL���X�g
	tx_data[len]		= '\r';
	tx_data[len + 1]	= '\n';
	
	// PC���ɃX���[�o��
	com_srv_send( tx_data, len + 2 );
#endif
}

/************************************************************************/
/* �֐�     : main_cpu_com_rcv_pc_log									*/
/* �֐���   : ���[�h�ύX��M											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���[�h�ύX��M												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_mode_chg( void )
{
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_MODE_CHG;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_MODE_CHG;
	
	// ��ԕύX
	s_unit.system_mode = (SYSTEM_MODE)s_ds.cpu_com.input.rcv_data[0];
	// ���퉞��
	s_ds.cpu_com.order.snd_data[0] = 0;		// ���퉞��
	// RD8001�Ή��F��ԊǗ���G1D�ōs���Ă���̂ŁA���ُ݈�P�[�X�͖���
	
}

/************************************************************************/
/* �֐�     : main_cpu_com_rcv_pc_log									*/
/* �֐���   : �v���O�����]���J�n������M								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �v���O�����]���J�n������M									*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_prg_hd_ready(void)
{
	// ���ʃR�[�h����
	download_change_boot();

	// RD8001�Ή��F�u�[�g�ւ̐ؑ֏���(WDT�ɂ�鎩�ȃ��Z�b�g)
	s_unit.non_wdt_refresh = ON;
	
	// ����
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_READY;
	s_ds.cpu_com.order.snd_data[0] = 0;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_PRG_READY;
}

/************************************************************************/
/* �֐�     : main_cpu_com_rcv_pc_log									*/
/* �֐���   : �v���O�����]���m�F��M									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �v���O�����]���m�F��M										*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_prg_hd_check(void)
{
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_CHECK;
	s_ds.cpu_com.order.snd_data[0] = version_product_tbl[0];
	s_ds.cpu_com.order.snd_data[1] = version_product_tbl[1];
	s_ds.cpu_com.order.snd_data[2] = version_product_tbl[2];
	s_ds.cpu_com.order.snd_data[3] = version_product_tbl[3];
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_PRG_CHECK;
}

/************************************************************************/
/* �֐�     : main_cpu_com_rcv_disp										*/
/* �֐���   : �\���w��													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �\���w��														*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_disp(void)
{
	UB result = OK;
	
	switch( s_ds.cpu_com.input.rcv_data[0] ){
		case CPU_COM_DISP_ORDER_SELF_CHECK_NON:
			s_unit.disp.ptn_g1d_order = DISP_PTN_OFF;
			break;
		case CPU_COM_DISP_ORDER_SELF_CHECK_ERR:
			s_unit.disp.ptn_g1d_order = DISP_PTN_SELF_CHECK_FLASH_100MS;
			break;
		case CPU_COM_DISP_ORDER_SELF_CHECK_FIN:
			s_unit.disp.ptn_g1d_order = DISP_PTN_SELF_CHECK_ON;
			break;
		case CPU_COM_DISP_TRG_LONG_SW_RECEPTION:
				s_unit.disp.ptn_sw_long_order = ON;		// �������\���w��
				// �d��SW�����^�C�}�[�ăX�^�[�g 
				time_soft_set_10ms( TIME_TYPE_10MS_SW_LONG_DISP, TIME_10MS_CNT_POW_SW_LONG_DISP );
			break;
		default:
			// ���肦�Ȃ�
			result = NG;
		break;
	}
	
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DISP_ORDER;
	s_ds.cpu_com.order.snd_data[0] = result;
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DISP_OREDER;
	
}

/************************************************************************/
/* �֐�     : main_cpu_com_rcv_version									*/
/* �֐���   : �o�[�W����												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �o�[�W����													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_version(void)
{
	UB	boot_ver[4];				/* Boot���o�[�W������� */
	
	// �A�v�����ێ����Ă���A�h���X����u�[�g�o�[�W�����𐶐�
	memcpy((UB *)boot_ver, (UB *)version_boot_tbl, sizeof(boot_ver));

	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_VERSION;
	s_ds.cpu_com.order.snd_data[0] = version_product_tbl[0];	// �A�v���o�[�W����
	s_ds.cpu_com.order.snd_data[1] = version_product_tbl[1];
	s_ds.cpu_com.order.snd_data[2] = version_product_tbl[2];
	s_ds.cpu_com.order.snd_data[3] = version_product_tbl[3];
	s_ds.cpu_com.order.snd_data[4] = boot_ver[0];				// �u�[�g�o�[�W����
	s_ds.cpu_com.order.snd_data[5] = boot_ver[1];
	s_ds.cpu_com.order.snd_data[6] = boot_ver[2];
	s_ds.cpu_com.order.snd_data[7] = boot_ver[3];
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_VERSION;
}

/************************************************************************/
/* �֐�     : ds_get_cpu_com_order										*/
/* �֐���   : CPU�ԒʐM�p�f�[�^�擾										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.03.02 Axia Soft Design ���� ��	���ō쐬			*/
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
/* �ύX���� : 2018.03.02 Axia Soft Design ���� ��	���ō쐬			*/
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


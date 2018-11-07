/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : main.c														*/
/* �@�\         : �S�̊Ǘ�(�}�N����`�A�^��`�A�֐��̊O���Q�Ɛ錾)				*/
/* �ύX����     : 2018.03.02 Axia Soft Design ���� ��	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef		_MAIN_H_			/* ��d��`�h�~ */
#define		_MAIN_H_


// �V�X�e�����[�h
typedef enum{
	SYSTEM_MODE_INITAL,						// �C�j�V����
	SYSTEM_MODE_IDLE_REST,					// �A�C�h��_�c�ʕ\�� ��RD8001�b��FIDLE�𓝍����邩�v����
	SYSTEM_MODE_IDLE_COM,					// �A�C�h��_�ʐM�ҋ@ ��RD8001�b��FIDLE�𓝍����邩�v����
	SYSTEM_MODE_SENSING,					// �Z���V���O
	SYSTEM_MODE_GET,						// �f�[�^�擾
	SYSTEM_MODE_PRG_H1D,					// H1D�v���O�����X�V
	SYSTEM_MODE_PRG_G1D,					// G1D�v���O�����X�V
	SYSTEM_MODE_SELF_CHECK,					// ���Ȑf�f
	//�������@�\�d�l��̏��������
	SYSTEM_MODE_MOVE,						// �ڍs
	SYSTEM_MODE_NON,						// �Ȃ�
	SYSTEM_MODE_MAX
}SYSTEM_MODE;



// �d�r�c��臒l
#define DENCH_ZANRYO_1_VAL						(UH)( 1023.0 * (1.95 / 3.0 ))		// 1.95V�ȏ�
#define DENCH_ZANRYO_2_VAL						(UH)( 1023.0 * (1.72 / 3.0 ))		// 1.72V�ȏ�
#define DENCH_ZANRYO_3_VAL						(UH)( 1023.0 * (1.5 / 3.0 ))		// 1.5V�ȏ�

#define DENCH_BAT_CHG_FIN_VAL					(UH)( 1023.0 * (2.0 / 3.0 ))		// 2.0V�ȏ�



#define			HOUR12_CNT_50MS		(UW)( 12L * 60L * 60L * (1000L / 50L))	//12���Ԃ̃J�E���g�l[50ms]
//#define			HOUR12_CNT_50MS			( 1L * 10L * 60L * (1000L / 50L))	//12���Ԃ̃J�E���g�l[50ms]		�Z�k��10��






// EEP
#define EEP_DEVICE_ADR			0xA0				// �f�o�C�X�A�h���X

// �����x�Z���T
#define ACL_DEVICE_ADR			0x1C				// �����x�Z���T�f�o�C�X�A�h���X
#define ACL_TIMING_VAL			10					// �����x�Z���T�����^�C�~���O


// 30�b[20ms]
#define SEC_30_CNT				( 30 * 1000 / 20 )
//#define SEC_30_CNT				( 30 * 1000 / 200 )	//10�{�f�o�b�O

// 12����[30�b]
#define HOUR_12_CNT				( 12 * 60 * 2 )
//#define HOUR_12_CNT				( 10 )

#define I2C_RCV_SND_SIZE	50		//RD8001�b��F�T�C�Y�v����


typedef struct{
	union{
		UB	byte[CPU_COM_SND_DATA_SIZE_SENSOR_DATA];
		struct{
			W	sekigaival;		// �������ׂ͂̈ɕ�������
			W	sekishoku_val;	// �������ׂ͂̈ɕ�������
			UH	kokyu_val;		
			UH	ibiki_val;		
			B acl_x;
			B acl_y;
			B acl_z;
			B dummy;			// �_�~�[[�p�f�B���O����]
		}dat;
	}info;
}MEAS;

#define			DISP_PTN_OFF						0		// ����
#define			DISP_PTN_ON							1		// �_��
#define			DISP_PTN_FLASH_100MS				2		// �_��(100ms)
#define			DISP_PTN_DENCH_ZANRYO_1				3		// �d�r�c��1
#define			DISP_PTN_DENCH_ZANRYO_2				4		// �d�r�c��2
#define			DISP_PTN_DENCH_ZANRYO_3				5		// �d�r�c��3
#define			DISP_PTN_FLASH_GET					6		// GET���[�h


#define			DISP_PTN_FLASH_100MS_SEQ_MAX		1				// �V�[�P���X�ő�

#define			DISP_PTN_FLASH_DENCH_ZANRYO_3_SEQ_MAX		61		// �V�[�P���X�ő�

#define			DISP_PTN_FLASH_GET_SEQ_MAX		5		// �V�[�P���X�ő�

typedef struct{
	UB seq;					// �V�[�P���X
	UB ptn;					// �p�^�[��
	UB last_ptn;			// �O��p�^�[��


}DISP;

// �[�d�������
#define BAT_CHG_FIN_STATE_OFF			0		// �Ȃ� ���N�������OFF�m��
#define BAT_CHG_FIN_STATE_ON			1		// ����
#define BAT_CHG_FIN_STATE_UNKNOWN		2		// �s�� ����x�[�d���m�|�[�g��ON�ɂȂ�ƕs����ԂɂȂ�


typedef struct{
	union {
		UB	byte;
		/*�ďo�����v���*/
		struct{
			UB	bat_chg			:1;		/* 1  �[�d���m�|�[�g */
			UB	bat_chg_fin		:1;		/* 2  �[�d�����C�x���g */
			UB	dummy1			:1;		/* 3  ����` */
			UB	dummy2			:1;		/* 4  ����` */
			UB	dummy3			:1;		/* 5  ����` */
			UB	dummy4			:1;		/* 6  ����` */
			UB	dummy5			:1;		/* 7  ����` */
			UB	dummy6			:1;		/* 8  ����` */
		}bit;
	}info;				/* 3  �ďo�����v���	Byte	1�o�C�g */
}H1D_INFO;				/* 3  �ďo�����v���	Byte	1�o�C�g */



typedef struct{
	UB main_cyc_req;		/* ���C�������v��(20ms) */

	UB system_mode;			/* �V�X�e�����[�h */
	UB system_mode_chg_req;	/* �V�X�e�����[�h�ύX�v�� */
	
	MEAS meas;				/* �v���l(50ms) */
	UH dench_sts;			/* �d�r�c�ʏ�� */
	UB bat_chg;				/* �[�d���m�|�[�g */
	UB bat_chg_fin_state;	/* �[�d�����C�x���g */
	UB kensa;				/* ������� */
	
	UB hour;
	UB min;
	UB sec;
	
	// �C�x���g
	UB event_sw_long;			// SW����(��)
	UB event_sw_short;			// SW����(�Z)
	
	

	UB sensing_end_flg;			// �Z���V���O�I��
	UB sensing_sekigai_flg;		// �ԊO�t���O(ON:�L���AOFF:����)
	
	
	

	UW sensing_cnt_50ms;		// �Z���V���O�I��[50ms]
	UB acl_sensor_timing;			// �����x�Z���T�^�C�~���O
	
	
	UB non_wdt_refresh;			// WDT���t���b�V���Ȃ�
	
	DISP disp;					// �\��	
	
	// ���[�N
	UB pow_sw_last;				// �d���{�^�����(�O��)
	UB bat_chg_last;			// �[�d���(�O��)
	UB kensa_last;				/* �������(�O��) */
	
	UW err_cnt;			//�ُ��(�f�o�b�O�p�r)
}T_UNIT;


//RD8001�b��FDS���ǂ����邩����
/*##################################################################*/
/*							CPU�ԒʐM��								*/
/*##################################################################*/
typedef struct _DS_CPU_COM_INPUT{
	UB rcv_cmd;											/* ��M�R�}���h */
	UB rcv_data[CPU_COM_DATA_SIZE_MAX];					/* ��M�f�[�^ */
	UB cpu_com_send_status;								/* CPU�ԒʐM���M�X�e�[�^�X */
														/* 	CPU_COM_SND_STATUS_IDLE			�A�C�h����� �����M�\���	*/
														/* 	CPU_COM_SND_STATUS_SEND			���M��						*/
														/* 	CPU_COM_SND_STATUS_COMPLETE		���M����					*/
														/* 	CPU_COM_SND_STATUS_SEND_NG		���g���CNG					*/
}DS_CPU_COM_INPUT;

typedef struct _DS_CPU_COM_ORDER{
	CPU_COM_CMD_ID snd_cmd_id;							/* ���M�R�}���hID */
	UB snd_data[CPU_COM_DATA_SIZE_MAX];					/* ���M�f�[�^ */
	UH data_size;										/* ���M�f�[�^�� */
}DS_CPU_COM_ORDER;

typedef struct _DS_CPU_COM{
	DS_CPU_COM_INPUT input;
	DS_CPU_COM_ORDER order;
}DS_CPU_COM;

typedef struct _CPU_COM_RCV_CMD_TBL{
	UB cmd;							/* ��M�R�}���h */
//	void (*func)(UB *p_data);		/* ��M���� */
	void (*func)(void);		/* ��M���� */
	UB res;							/* �����L(ON)��(OFF) */
}CPU_COM_RCV_CMD_TBL;


/* DS�\���� */
typedef struct{
	DS_CPU_COM 		cpu_com;		/* CPU�ԒʐM�� */
}DS;


/************************************************************/
/* �O���Q�Ɛ錾												*/
/************************************************************/
// �O���Q�Ɛ錾�@�������Ɛ���ɓ����Ȃ�
extern void R_IICA0_Create(void);
//extern MD_STATUS com_srv_send(uint8_t * const tx_buf, uint16_t tx_num);
//extern MD_STATUS R_UART1_Receive(uint8_t * const rx_buf, uint16_t rx_num);

extern void R_PGA_DSAD_Get_AverageResult(uint16_t * const bufferH,uint16_t * const bufferL);
extern void R_PGA_DSAD_Get_Result(uint16_t * const bufferH,uint16_t * const bufferL);
extern void R_DAC_Change_OutputVoltage_12bit(uint16_t outputVoltage);
extern unsigned short pga_do( void );
extern void R_IICA0_StopCondition(void);
extern void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
extern void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );




#endif
/************************************************************/
/* END OF TEXT												*/
/************************************************************/

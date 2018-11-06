/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : main.c														*/
/* �@�\         : �S�̊Ǘ�(�}�N����`�A�^��`�A�֐��̊O���Q�Ɛ錾)				*/
/* �ύX����     : 2018.03.02 Axia Soft Design ���� ��	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef		_MAIN_H_			/* ��d��`�h�~ */
#define		_MAIN_H_


/* �t�@�C����(SD�摜������,�_�E�����[�h) */
#define			SYSTEM_MODE_NON						0
#define			SYSTEM_MODE_SENSOR					1
#define			SYSTEM_MODE_GET_MODE				2

// EEP
#define EEP_DEVICE_ADR			0xA0				// �f�o�C�X�A�h���X
#define EEP_RECORD_SIZE			16					// 1���R�[�h�T�C�Y ��256�̖񐔂ł���K�v������
#define EEP_ADRS_SIZE			2					// 1���R�[�h�T�C�Y

// �����x�Z���T
#define ACL_DEVICE_ADR			0x1C				// �����x�Z���T�f�o�C�X�A�h���X

#define EEP_DATA_SIZE_ALL		131072				// EEP�S�T�C�Y
#define EEP_DATA_SIZE_PAGE		( 131072 / 2 )		// EEP�y�[�W�T�C�Y


#define EEP_RECORD_1P_MAX			( EEP_DATA_SIZE_PAGE / EEP_RECORD_SIZE )			// 1���R�[�h�T�C�Y(1�y�[�W��)
#define EEP_RECORD_2P_MAX			( EEP_RECORD_1P_MAX * 2 )							// 1���R�[�h�T�C�Y(2�y�[�W��)

// 30�b[20ms]
#define SEC_30_CNT				( 30 * 1000 / 20 )
//#define SEC_30_CNT				( 30 * 1000 / 200 )	//10�{�f�o�b�O

// 12����[30�b]
#define HOUR_12_CNT				( 12 * 60 * 2 )
//#define HOUR_12_CNT				( 10 )

#define I2C_RCV_SND_SIZE	50		//RD8001�b��F�T�C�Y�v����




// EEP�������ݗp���R�[�h(�A�h���X�t��)
typedef struct{
	union{
		UB	byte[EEP_RECORD_SIZE + EEP_ADRS_SIZE];
		struct{
			UH		wr_adrs;				/* EEP�A�h���X */
			UH	kokyu_val;		
			UH	ibiki_val;		
			W	sekishoku_val;	// �������ׂ͂̈ɕ�������
			W	sekigaival;		// �������ׂ͂̈ɕ�������
			UB	valid;			/* ���R�[�h�L��/����				*/
			B acl_x;
			B acl_y;
			B acl_z;
		}data;
	}record;
}WR_EEP_RECORD;

// EEP���R�[�h
typedef struct{
	union{
		UB	byte[EEP_RECORD_SIZE];
		struct{
			UH	kokyu_val;		
			UH	ibiki_val;		
			W	sekishoku_val;	// �������ׂ͂̈ɕ�������
			W	sekigaival;		// �������ׂ͂̈ɕ�������
			UB	valid;			/* ���R�[�h�L��/����				*/
			B acl_x;
			B acl_y;
			B acl_z;
#if 0
			double	ibiki_val;				/* ���т� */
			double	myaku_val_sekishoku;	/* ����_�ԊO */
			double	myaku_val_sekigai;		/* ����_�ԐF */
			double	spo2_val_normal;		/* SPO2_�ʏ� */
			double	spo2_val_konica;		/* SPO2 */
			UB state_flg;					/* �t���O(�ċz���,�����X�e�[�W,���т��L�� */
			UB	valid;						/* ���R�[�h�L��/���� */
#endif
		}data;
	}record;
}EEP_RECORD;
typedef struct{
	union{
		UB	byte[CPU_COM_SND_DATA_SIZE_SENSOR_DATA];
		struct{
			H	sekigaival;		// �������ׂ͂̈ɕ�������
			H	sekishoku_val;	// �������ׂ͂̈ɕ�������
			UH	kokyu_val;		
			UH	ibiki_val;		
			B acl_x;
			B acl_y;
			B acl_z;
			B dummy;			// ��[�p�f�B���O����]
		}dat;
	}info;
}MEAS;

typedef struct{
	UB main_cyc_req;		/* ���C�������v��(20ms) */
	UB system_mode;			/* ���C�������v��(20ms) */
	
	UB system_mode_chg_req;	/* �V�X�e�����[�h�ύX�v�� */
	
	
	
	UH eep_wr_record_cnt;	/* �������݃��R�[�h */
	UH eep_rd_record_cnt;	/* �ǂݏo�����R�[�h */	
	// �v���l(20ms)
	MEAS meas;				/* �v���l(50ms) */
	UH dench_val;
#if 0
	UH kokyu_val;		
	UH ibiki_val;		
	W sekishoku_val;	// �������ׂ͂̈ɕ�������
	W sekigaival;		// �������ׂ͂̈ɕ�������
	B acl_x;			// �����x�Z���T(8bit)
	B acl_y;
	B acl_z;
#endif
	EEP_RECORD	eep;		// �f�[�^���R�[�h(EEP�ǂݏ����AUART���M)
	
	UB hour;
	UB min;
	UB sec;
	
	
	
	UB sensing_start_trig;		// �Z���V���O�J�n�g���K
	UB sensing_end_flg;			// �Z���V���O�I��

	UB sensing_cnt;			// �Z���V���O�I��
	
	UB pow_sw_last;				// �d���{�^�����(�O��)
	
	
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
	void (*func)(UB *p_data);		/* ��M���� */
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
extern void adc_do( uint16_t* ad1, uint16_t* ad2, uint16_t* ad3 );
extern unsigned short pga_do( void );
extern void R_IICA0_StopCondition(void);
extern void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
extern void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );




#endif
/************************************************************/
/* END OF TEXT												*/
/************************************************************/

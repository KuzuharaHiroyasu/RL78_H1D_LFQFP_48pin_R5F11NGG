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
	SYSTEM_MODE_INITIAL,					// �C�j�V����
	SYSTEM_MODE_IDLE_REST,					// �A�C�h��_�c�ʕ\��
	SYSTEM_MODE_IDLE_COM,					// �A�C�h��_�ʐM�ҋ@
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

// �C�x���g ��H1D/G1D����
typedef enum{
	EVENT_NON = 0,				// �Ȃ�
	EVENT_POW_SW_SHORT,			// �d��SW����(�Z)
	EVENT_POW_SW_LONG,			// �d��SW����(��)
	EVENT_CHG_PORT_ON,			// �[�d���m�|�[�gON
	EVENT_CHG_PORT_OFF,			// �[�d���m�|�[�gOFF
	EVENT_DENCH_LOW,			// �d�r�c�ʒቺ
	EVENT_CHG_FIN,				// �[�d����
	EVENT_GET_DATA,				// �f�[�^�擾
	EVENT_H1D_PRG,				// �v���O������������(H1D)
	EVENT_G1D_PRG,				// �v���O������������(G1D)
	EVENT_SELF_CHECK_COM,		// ���Ȑf�f(�ʐM)
	EVENT_COMPLETE,				// ����
	EVENT_STOP,					// ���f
	EVENT_TIME_OUT,				// �^�C���A�E�g
	EVENT_KENSA_ON,				// �����|�[�gON
	EVENT_MAX,					// �ő�
}EVENT_NUM;

/* �ُ�ID��`(�f�o�b�O�@�\) */
typedef enum{
	ERR_ID_I2C = 1					,			/* I2C(�ėp) */
	ERR_ID_MAIN						,			/* MAIN(�ėp) */
	ERR_ID_CPU_COM					,			/* CPU�ԒʐM(�ėp) */
	ERR_ID_ACL						,			/* �����x�Z���T(�ėp) */

	// �����x���ُ�
	ERR_ID_LOGIC = 99				,			/* ���W�b�N�s�(�ėp) */
	ERR_ID_MAX									/* �ُ�ID�ő�	*/
}ERR_ID;


// �d�r�c��臒l
#define DENCH_ZANRYO_1_VAL						(UH)( 1023.0 * (1.95 / 3.0 ))		// 1.95V�ȏ�
#define DENCH_ZANRYO_2_VAL						(UH)( 1023.0 * (1.72 / 3.0 ))		// 1.72V�ȏ�
#define DENCH_ZANRYO_3_VAL						(UH)( 1023.0 * (1.5 / 3.0 ))		// 1.5V�ȏ�

#define DENCH_BAT_CHG_FIN_VAL					(UH)( 1023.0 * (2.0 / 3.0 ))		// 2.0V�ȏ�


// �d�r�c�ʏ�� ��H1D/G1D����
#define DENCH_ZANRYO_STS_MAX					1	// �[�d���^��
#define DENCH_ZANRYO_STS_HIGH					2	// ��������
#define DENCH_ZANRYO_STS_LOW					3	// �c��P�������Ȃ�
#define DENCH_ZANRYO_STS_MIN					4	// �d�r�c�ʂȂ�





// �Z���V���O���J�E���g
#define			HOUR12_CNT_50MS		(UW)( 12L * 60L * 60L * (1000L / 50L))	//12���Ԃ̃J�E���g�l[50ms]
//#define			HOUR12_CNT_50MS			( 1L * 10L * 60L * (1000L / 50L))	//12���Ԃ̃J�E���g�l[50ms]		�Z�k��10��



#define			SENSING_SW_LONG_CNT_50MS		60	/* �d��SW_��(3�b) */




// �����x�Z���T
#define ACL_DEVICE_ADR			0x1C				// �����x�Z���T�f�o�C�X�A�h���X
#define ACL_TIMING_VAL			10					// �����x�Z���T�����^�C�~���O
#define I2C_LOCK_ERR_VAL		1000				// ���b�N�ُ픻��臒l

#define I2C_WAIT		255					// �X�^�[�g�R���f�B�V�����҂� ��200us���x�Ȃ̂ōő�l��ݒ肵�Ă���

// ���W�X�^�A�h���X
#define ACL_REG_ADR_WHO_AM_I			0x0F				// WHO AM I
#define ACL_REG_ADR_DATA_XYZ			0x06				// XOUT,YOUT,ZOUT
#define ACL_REG_ADR_INT_SRC1			0x16				// INT_SOURCE1
#define ACL_REG_ADR_INT_REL				0x1A				// INT_REL
#define ACL_REG_ADR_CTRL_REG1			0x1B				// CTRL_REG1
#define ACL_REG_ADR_CTRL_REG2			0x1F				// CTRL_REG2


// ���W�X�^�f�[�^
#define ACL_REG_RECOGNITION_CODE		0x35				// �F���R�[�h(0x35)


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

// �\���p�^�[��
#define			DISP_PTN_OFF						0		// ����
#define			DISP_PTN_SELF_CHECK_ON				1		// ���Ȑf�f�_�� ��G1D����̎w��
#define			DISP_PTN_SELF_CHECK_FLASH_100MS		2		// ���Ȑf�f�_��(100ms) ��G1D����̎w��
#define			DISP_PTN_CHG_ON						3		// �[�d��
#define			DISP_PTN_INITAL_RESET				4		// ���������Z�b�g
#define			DISP_PTN_PW_SW_LONG					5		// �d���{�^��������
#define			DISP_PTN_DENCH_ZANRYO_1				6		// �d�r�c��1
#define			DISP_PTN_DENCH_ZANRYO_2				7		// �d�r�c��2
#define			DISP_PTN_DENCH_ZANRYO_3				8		// �d�r�c��3

#define			DISP_PTN_BLE_ON						9		// �ʐM�ҋ@/�f�[�^�擾 ��BLE�ڑ��ς�
#define			DISP_PTN_BLE_OFF					10		// �ʐM�ҋ@/�f�[�^�擾 ��BLE�ڑ����ڑ�



#define			DISP_PTN_SELF_CHECK_FLASH_100MS_SEQ_MAX		1				// �V�[�P���X�ő�

#define			DISP_PTN_FLASH_DENCH_ZANRYO_3_SEQ_MAX		61		// �V�[�P���X�ő�

#define			DISP_PTN_BLE_OFF_SEQ_MAX		5		// �V�[�P���X�ő�
#define			DISP_PTN_BLE_ON_SEQ_MAX			1		// �V�[�P���X�ő�

typedef struct{
	UB seq;					// �V�[�P���X
	UB ptn;					// �p�^�[��
	UB last_ptn;			// �O��p�^�[��
	
	UB ptn_g1d_order;					// �p�^�[��(G1D����̎w��)
	UB ptn_inital_order;				// �p�^�[��(���������Z�b�g�w��)
	UB ptn_sw_long_order;				// �p�^�[��(�d���������w��)
}DISP;

// �[�d�������
#define BAT_CHG_FIN_STATE_OFF			0		// �Ȃ� ���N�������OFF�m��
#define BAT_CHG_FIN_STATE_ON			1		// ����
#define BAT_CHG_FIN_STATE_UNKNOWN		2		// �s�� ����x�[�d���m�|�[�g��ON�ɂȂ�ƕs����ԂɂȂ�

typedef struct{
	union {
		UB	byte;
		struct{
			UB	ble				:1;		/* 1  BLE    */
			UB	dummy1			:1;		/* 2  ����` */
			UB	dummy2			:1;		/* 3  ����` */
			UB	dummy3			:1;		/* 4  ����` */
			UB	dummy4			:1;		/* 5  ����` */
			UB	dummy5			:1;		/* 6  ����` */
			UB	dummy6			:1;		/* 7  ����` */
			UB	dummy7			:1;		/* 8  ����` */
		}bit;
	}info;
}G1D_INFO;


typedef struct{
	union {
		UB	byte;
		struct{
			UB	bat_chg			:1;		/* 1  �[�d���m�|�[�g */
			UB	kensa			:1;		/* 2  �����|�[�g */
			UB	dummy1			:1;		/* 3  ����` */
			UB	dummy2			:1;		/* 4  ����` */
			UB	dummy3			:1;		/* 5  ����` */
			UB	dummy4			:1;		/* 6  ����` */
			UB	dummy5			:1;		/* 7  ����` */
			UB	dummy6			:1;		/* 8  ����` */
		}bit;
	}info;
}H1D_INFO;



typedef struct{
	// I2C�h���C�o�̃t���O ��i2c�h���C�o�Őݒ肳���
	UB i2c_snd_flg;			//���M�t���O(I2C)
	UB i2c_rcv_flg;			//��M�t���O(I2C)
	// RD8001�b��FACL�ُ̈펞�̓��삪�d�l�㖢��`
	 UB i2c_err_flg;			//�G���[�t���O(I2C)
}ACL_INFO;

typedef struct{
	UB main_cyc_req;		/* ���C�������v��(20ms) */

	SYSTEM_MODE system_mode;			/* �V�X�e�����[�h */
	SYSTEM_MODE last_system_mode;		/* �V�X�e�����[�h */
	EVENT_NUM system_evt;				/* �V�X�e�����[�h�ύX�v�� *///RD8001�b��F����d�l�ɂ���Ă̓C�x���g���o�b�t�@�����O����K�v������
		
	MEAS meas;				/* �v���l(50ms) */
	UH dench_sts;			/* �d�r�c�ʏ�� */
	UB bat_chg;				/* �[�d���m�|�[�g */
	UB bat_chg_fin_state;	/* �[�d�����C�x���g */
	UB kensa;				/* ������� */
		
	G1D_INFO g1d;				// G1D���

	// �Z���V���O�֘A
	UB sensing_end_flg;			// �Z���V���O�I��
	UB sensing_sekigai_flg;		// �ԊO�t���O(ON:�L���AOFF:����)
	
	UW sensing_cnt_50ms;		// �Z���V���O�I��[50ms]
	UB acl_sensor_timing;		// �����x�Z���T�^�C�~���O
	UW sensing_sw_on_cnt;				// �Z���V���OSW[50ms]
	
	
	UB non_wdt_refresh;			// WDT���t���b�V���Ȃ�
	
	DISP disp;					// �\��	
	
	ACL_INFO	acl_info;		// �����x�Z���T���(I2C)
	
	
	// ���[�N
	UB pow_sw_last;				// �d���{�^�����(�O��)
	UB bat_chg_last;			// �[�d���(�O��)
	UB kensa_last;				/* �������(�O��) */
	
	// �ُ�֘A�f�o�b�O�p�r
	UW err_cnt;				//�ُ��(�f�o�b�O�p�r)
	ERR_ID last_err_id;		//�O��ُ�ID(�f�o�b�O�p�r)
}T_UNIT;


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
//	UB res;							/* �����L(ON)��(OFF) */
}CPU_COM_RCV_CMD_TBL;


/* DS�\���� */
typedef struct{
	DS_CPU_COM 		cpu_com;		/* CPU�ԒʐM�� */
}DS;


/************************************************************/
/* �O���Q�Ɛ錾												*/
/************************************************************/
// �h���C�o
extern void R_IICA0_Create(void);
extern void R_PGA_DSAD_Get_AverageResult(uint16_t * const bufferH,uint16_t * const bufferL);
extern void R_PGA_DSAD_Get_Result(uint16_t * const bufferH,uint16_t * const bufferL);
extern void R_DAC_Change_OutputVoltage_12bit(uint16_t outputVoltage);
extern unsigned short pga_do( void );
extern void R_IICA0_StopCondition(void);


// ���C������
extern void user_main(void);
extern void set_req_main_cyc(void);
extern void err_info( ERR_ID id );
extern void i2c_set_snd_flg( UB data );
extern void i2c_set_rcv_flg( UB data );
extern void i2c_set_err_flg( UB data );
extern void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
extern void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );
extern void getmode_in(void);


#endif
/************************************************************/
/* END OF TEXT												*/
/************************************************************/

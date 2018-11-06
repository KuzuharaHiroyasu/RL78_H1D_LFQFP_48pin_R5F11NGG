/************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[									*/
/* �t�@�C����   : cpu_com.h												*/
/* �@�\         : CPU�ԒʐM�~�h��										*/
/* �ύX����     : 2018.04.10 Axia Soft Design ����		���ō쐬		*/
/* ���ӎ���     : �Ȃ�                                                  */
/************************************************************************/
#ifndef _CPU_COM_H_
#define _CPU_COM_H_


/******************/
/* �萔�}�N����` */
/******************/
#define CPU_COM_SND_DATA_SIZE_STATUS_REQ			13				/* �X�e�[�^�X�v��			*/
#define CPU_COM_SND_DATA_SIZE_MODE_CHG				1				/* ��ԕύX(G1D)			*/
#define CPU_COM_SND_DATA_SIZE_PC_LOG				7				/* PC���O���M(�����R�}���h)	*/
#define CPU_COM_SND_DATA_SIZE_SENSOR_DATA			11				/* �Z���T�[�f�[�^	*/

// ���[�h�ύX�v��
#define			SYSTEM_MODE_HD_CHG_NON				0		// �Ȃ�
#define			SYSTEM_MODE_HD_CHG_SENSING			1		// ����(�Z���V���O)
#define			SYSTEM_MODE_HD_CHG_IDLE				2		// ����(�A�C�h��)


/* �f�[�^���͈� */								// RD8001�b��
#define CPU_COM_DATA_SIZE_MAX					(262+1)		/* �f�[�^�̍ő咷 */
															/* �ő�f�[�^ �\���f�[�^�X�V �ڍ׃X�����v�O���t ���No(2)+�f�[�^��(2)+�f�[�^(308) */
// �e�f�[�^�T�C�Y
#define CPU_COM_CMD_SIZE						(1)			/* �R�}���h�f�[�^�T�C�Y 1byte */
#define CPU_COM_SEQ_SIZE						(1)			/* SEQNo�f�[�^�T�C�Y 1byte */
#define CPU_COM_CRC_SIZE						(2)			/* CRC�f�[�^�T�C�Y 2byte */
#define CPU_COM_STX_SIZE						(2)			/* STX�f�[�^�T�C�Y 2byte */
#define CPU_COM_ETX_SIZE						(2)			/* ETX�f�[�^�T�C�Y 2byte */

/* ���b�Z�[�W�̍ŏ� */
#define CPU_COM_MSG_SIZE_MIN					( CPU_COM_CMD_SIZE + \
												  CPU_COM_SEQ_SIZE + \
												  CPU_COM_CRC_SIZE + \
												  CPU_COM_STX_SIZE + \
												  CPU_COM_ETX_SIZE )


/* ���b�Z�[�W�̍ő� �R�}���h(1)+SEQ(2)+�f�[�^(262)+SUM(2)+CRC(2) */
#define CPU_COM_MSG_SIZE_MAX					(CPU_COM_DATA_SIZE_MAX + CPU_COM_MSG_SIZE_MIN)

/* ���M�X�e�[�^�X */
#define CPU_COM_SND_STATUS_IDLE					0			/* ���M�\��� */
#define CPU_COM_SND_STATUS_RCV_WAIT				1			/* ������M�҂���� */
#define CPU_COM_SND_STATUS_RENSOU				2			/* �A������� */
#define CPU_COM_SND_STATUS_COMPLETE				3			/* ���M������� */
#define CPU_COM_SND_STATUS_SEND_NG				4			/* ���g���CNG */
#define CPU_COM_SND_STATUS_SEND_WAIT			5			/* ���M�҂� */

/* CPU�ԒʐM��M�҂����M���N�G�X�g���� */
#define CPU_COM_SND_RES_OK						0			/* ���������M */
#define CPU_COM_SND_RES_BUSY_NG					1			/* CPU�ԒʐMBUSY���(���M��or��M�҂�)NG */
#define CPU_COM_SND_RES_RETRY_OUT_NG			2			/* ���g���C�A�E�g */
#define CPU_COM_SND_RES_COM_NG					3			/* ���M�R�}���hNG */

/* CPU�ԒʐM �R�}���h��� */
/* �v���E�����̃Z�b�g */
typedef enum _CPU_COM_CMD_ID{
	CPU_COM_CMD_NONE=0,							/* �yCPU�ԒʐM�R�}���h�z�R�}���h����				*/
	CPU_COM_CMD_STATUS,							/* �yCPU�ԒʐM�R�}���h�z�X�e�[�^�X�v��				*/
	CPU_COM_CMD_SENSOR_DATA,					/* �yCPU�ԒʐM�R�}���h�z�Z���T�[�f�[�^�X�V			*/
	CPU_COM_CMD_MODE_CHG,						/* �yCPU�ԒʐM�R�}���h�z��ԕύX(G1D)				*/
	CPU_COM_CMD_PC_LOG,							/* �yCPU�ԒʐM�R�}���h�zPC���O���M(�����R�}���h)	*/
	CPU_COM_CMD_DATE_SET,						/* �yCPU�ԒʐM�R�}���h�z�����ݒ�	*/
	
	CPU_COM_CMD_PRG_DOWNLORD_READY,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	CPU_COM_CMD_PRG_DOWNLORD_START,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���J�n		*/
	CPU_COM_CMD_PRG_DOWNLORD_ERASE,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	CPU_COM_CMD_PRG_DOWNLORD_DATA,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���f�[�^	*/
	CPU_COM_CMD_PRG_DOWNLORD_RESLUT,			/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	CPU_COM_CMD_PRG_DOWNLORD_CHECK,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���m�F		*/
//	CPU_COM_CMD_FILE_REQ,						/* �yCPU�ԒʐM�R�}���h�z�t�@�C���]���J�n		*/
//	CPU_COM_CMD_FILE,							/* �yCPU�ԒʐM�R�}���h�z�t�@�C���]��			*/
//	CPU_COM_CMD_FILE_BLOCK_RESULT_REQ,			/* �yCPU�ԒʐM�R�}���h�z�u���b�N�]�����ʗv��	*/
//	CPU_COM_CMD_FILE_RESLUT_REQ,				/* �yCPU�ԒʐM�R�}���h�z�t�@�C���]�����ʗv��	*/
	CPU_COM_CMD_MAX								/* �yCPU�ԒʐM�R�}���h�z�ő�l					*/
}CPU_COM_CMD_ID;


/* �R�}���h�^�C�v */
/* ���}�X�^�[��p */
#define CPU_COM_CMD_TYPE_ONESHOT_SEND			0					/* �P�����M�R�}���h */
#define CPU_COM_CMD_TYPE_RETRY					1					/* ���g���C */
#define CPU_COM_CMD_TYPE_RENSOU					2					/* �A�� */
#define CPU_COM_CMD_TYPE_ONESHOT_RCV			3					/* �P����M�R�}���h */


/* CPU�ԒʐM�_�E�����[�h */
/* RD1402�b�� �u���b�N�ԍ���0�n�܂��SH���ŃI�t�Z�b�g�������� */
/* �u���b�N�ԍ��ő�l�͎b��l�̂��ߗv���� */
#define CPU_COM_BLOCK_NUM_MIN			(0x00000000)		/* CPU�ԒʐM�u���b�N�ԍ� �ŏ��l */
#define CPU_COM_BLOCK_NUM_MAX			(0x00FFFFFF)		/* CPU�ԒʐM�u���b�N�ԍ� �ő�l */

// RD8001�b��F�h���C�o�ֈړ�
/* ���M�X�e�[�^�X */
#define DRV_CPU_COM_STATUS_CAN_SEND		(0)			/* ���M�\��� */
#define DRV_CPU_COM_STATUS_SENDING		(1)			/* ���M�� */
//#define DRV_CPU_COM_STATUS_SEND_END		(2)			/* ���M�I�� */ /* ���M�I����Ԃ���5ms��ɑ��M�\�ƂȂ� */



/* �ǂݏo��(������A�t�@�C��)���� */
typedef enum{
	READ_RESULT_NO_ERROR = 0,		/* ����I�� */
	READ_RESULT_ERROR_READ,			/* �ǂݏo�����s */
	READ_RESULT_FILE_NON,			/* �t�@�C���Ȃ� */
	READ_RESULT_MAX
} ENUM_READ_RESULT;


/* =====�ʐM�d�l�֘A===== */



// �ʐM�f�[�^�ɉ����鐧��R�[�h
#define CPU_COM_CTRL_CODE_STX			0x02					/* STX */
#define CPU_COM_CTRL_CODE_ETX			0x03					/* ETX */
#define CPU_COM_CTRL_CODE_DLE			0x10					/* DLE */

/* �ǂݏo��(������A�t�@�C��)���� */
typedef enum{
	CPU_COM_RCV_MSG_STATE_STX_WAIT = 0,			/* STX�҂� */
	CPU_COM_RCV_MSG_STATE_ETX_WAIT,				/* ETX�҂� */
}CPU_COM_RCV_MSG_STATE;

// ���b�Z�[�W�z��̓Y����
#define CPU_COM_MSG_TOP_POS_CMD					2				/* �R�}���h */
#define CPU_COM_MSG_TOP_POS_SEQ					3				/* �V�[�P���X */
#define CPU_COM_MSG_TOP_POS_DATA				4				/* �f�[�^�� */



//�T�uCPU�ʐM�ُ픻���
#define CPU_COM_SUB_CNT_OPE_START			( 20 )					/* �J�n�p��1�b[10ms] */
#define CPU_COM_SUB_CNT_NORMAL				( 1 * 60 * 100 )		/* �ʏ��1��[10ms] */
#define CPU_COM_SUB_CNT_PRG					( 6 * 60 * 100 )		/* �v���O����������6��[10ms] */

#define CPU_COM_SUB_OPE_END_TIME_PRG		7000	/* ���슮���m�F����[70�b] ���v���O���� */
#define CPU_COM_SUB_OPE_END_TIME			500		/* ���슮���m�F����[5�b] �����̑� */


// ����I�����
typedef enum{
	CPU_COM_SUB_OPE_END_RESULT_OK = 0,			/* 0�F����I��*/
	CPU_COM_SUB_OPE_END_RESULT_NG,				/* 1�F�ُ�I�� */
	CPU_COM_SUB_OPE_END_RESULT_CONT,			/* 2�F������ */
}CPU_COM_SUB_OPE_END_RESULT;

/* �T�u�}�C�R������ */
typedef enum{
	CPU_COM_SUB_OPE_IMAGE = 0,			/* �摜�]��(�������[�J�[�h) */
	CPU_COM_SUB_OPE_PRG,				/* �A���h���C�h(�T�u�}�C�R��)�v���O�����X�V */
	CPU_COM_SUB_OPE_BACKUP,				/* �o�b�N�A�b�v(�A���h���C�h���������[�J�[�h) */
	CPU_COM_SUB_OPE_RECOVERY,			/* ���J�o��(�������[�J�[�h���A���h���C�h) */
	CPU_COM_SUB_OPE_NON					/* �Ȃ� */
}CPU_COM_SUB_OPE;

/* �T�u�}�C�R�������� */
typedef enum{
	CPU_COM_SUB_STATE_NON = 0,			/* �Ȃ� */
	CPU_COM_SUB_STATE_START,			/* ����J�n(�A���h���C�h���������[�J�[�h) */
	CPU_COM_SUB_STATE_END,				/* ����I��(�A���h���C�h���������[�J�[�h) */
	CPU_COM_SUB_STATE_RX_PRG,			/* RX(���C���}�C�R��)�v���O�����X�V�J�n */
}CPU_COM_SUB_STATE;

//�T�uCPU����
typedef struct{
	CPU_COM_SUB_OPE ope;		// ����
	CPU_COM_SUB_STATE state;	// ���
	UW last_time;				// ����
	UW no_res_cnt;				// ��������
}CPU_COM_SUB;

/******************/
/*   �\���̒�`   */
/******************/
typedef struct _T_CPU_COM_CMD_INFO {
	UB cmd;					/* �R�}���h */
	UB cmd_type;			/* �R�}���h�^�C�v */
	UB retry_cnt;			/* ���g���C�� */
	UW retry_time;			/* ���g���C�Ԋu */
	UB rensou_cnt;			/* �A���� */
} T_CPU_COM_CMD_INFO;


typedef struct{
	UH file_num;					/* �t�@�C���ԍ�	2byte */
	UW block_num;					/* �u���b�N�ԍ�	4byte */
	UB* p_data;						/* ���������f�[�^�̐擪�|�C���^	256byte�܂� */
	UH len;							/* �f�[�^�� *///RD1402�b��
}T_CPU_COM_CMD_FILE_DATA;


typedef struct{
	UW block_num;					/* �u���b�N�ԍ�	4byte */
}T_CPU_COM_CMD_FILE_RESULT_BLOCK;

typedef struct{
	UW sum;							/* �T���l	4byte */
}T_CPU_COM_CMD_FILE_RESULT;

// �t�@�C���ǂݏo��
typedef struct{
	UH file_num;					/* �t�@�C���ԍ�	2byte */
	UW block_num;					/* �u���b�N�ԍ�	4byte */
}T_CPU_COM_CMD_READ_FILE;




typedef struct{
	UB buf[CPU_COM_MSG_SIZE_MAX];		/* �o�b�t�@ */
	UH pos;								/* ��M�ʒu */
	UB last_dle_flg;					/* �O���M�f�[�^��DLE */
	UB state;							/* �O���M�f�[�^��DLE */
}CPU_COM_RCV_MSG;



/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void cpu_com_init(void);
extern void cpu_com_proc(void);
extern void cpu_com_send_end( void );
extern UB cpu_com_get_status(void);


#endif
/************************************************************************/
/* END OF TEXT															*/
/************************************************************************/
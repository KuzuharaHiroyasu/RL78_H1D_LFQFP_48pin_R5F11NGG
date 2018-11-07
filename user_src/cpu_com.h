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
// ���M�f�[�^�T�C�Y
#define CPU_COM_SND_DATA_SIZE_STATUS_REQ			13				/* �X�e�[�^�X�v��			*/
#define CPU_COM_SND_DATA_SIZE_MODE_CHG				1				/* ��ԕύX(G1D)			*/
#define CPU_COM_SND_DATA_SIZE_PC_LOG				7				/* PC���O���M(�����R�}���h)	*/
#define CPU_COM_SND_DATA_SIZE_SENSOR_DATA			15				/* �Z���T�[�f�[�^	*/

// ��M�f�[�^�T�C�Y
#define CPU_COM_RCV_DATA_SIZE_PC_LOG				10				/* PC���O */



// ���[�h�ύX�v��
// RD8001�b��F�폜�\��
#if 0
#define			SYSTEM_MODE_HD_CHG_NON				0		// �Ȃ�
#define			SYSTEM_MODE_HD_CHG_SENSING			1		// ����(�Z���V���O)
#define			SYSTEM_MODE_HD_CHG_IDLE				2		// ����(�A�C�h��)
#endif

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
	CPU_COM_CMD_DISP_ORDER,						/* �yCPU�ԒʐM�R�}���h�z�\���w��				*/
	CPU_COM_CMD_MAX								/* �yCPU�ԒʐM�R�}���h�z�ő�l					*/
}CPU_COM_CMD_ID;


/* �R�}���h�^�C�v */
/* ���}�X�^�[��p */
#define CPU_COM_CMD_TYPE_ONESHOT_SEND			0					/* �P�����M�R�}���h */
#define CPU_COM_CMD_TYPE_RETRY					1					/* ���g���C */
#define CPU_COM_CMD_TYPE_RENSOU					2					/* �A�� */
#define CPU_COM_CMD_TYPE_ONESHOT_RCV			3					/* �P����M�R�}���h */



/* =====�ʐM�d�l�֘A===== */
// �ʐM�f�[�^�ɉ����鐧��R�[�h
#define CPU_COM_CTRL_CODE_STX			0x02					/* STX */
#define CPU_COM_CTRL_CODE_ETX			0x03					/* ETX */
#define CPU_COM_CTRL_CODE_DLE			0x10					/* DLE */

// �d����͗p
typedef enum{
	CPU_COM_RCV_MSG_STATE_STX_WAIT = 0,			/* STX�҂� */
	CPU_COM_RCV_MSG_STATE_ETX_WAIT,				/* ETX�҂� */
}CPU_COM_RCV_MSG_STATE;


// ���b�Z�[�W�z��̓Y����
#define CPU_COM_MSG_TOP_POS_CMD					2				/* �R�}���h */
#define CPU_COM_MSG_TOP_POS_SEQ					3				/* �V�[�P���X */
#define CPU_COM_MSG_TOP_POS_DATA				4				/* �f�[�^�� */

// =========================
// �f�[�^���
// =========================
// �\���w��
#define CPU_COM_DISP_ORDER_NON				0	// �\���Ȃ�
#define CPU_COM_DISP_ORDER_SELF_CHECK_ERR	1	// �ُ�
#define CPU_COM_DISP_ORDER_SELF_CHECK_FIN	2	// ����

/******************/
/*   �\���̒�`   */
/******************/
typedef struct _T_CPU_COM_CMD_INFO {
	UB cmd;					/* �R�}���h */
//	UB cmd_type;			/* �R�}���h�^�C�v[�}�X�^�[] */
//	UB retry_cnt;			/* ���g���C��[�}�X�^�[] */
//	UW retry_time;			/* ���g���C�Ԋu[�}�X�^�[] */
//	UB rensou_cnt;			/* �A����[�}�X�^�[] */
} T_CPU_COM_CMD_INFO;


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
extern void test_cpu_com_send( void );

#endif
/************************************************************************/
/* END OF TEXT															*/
/************************************************************************/
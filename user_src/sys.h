/********************************************************************************/
/* �V�X�e����   : RD8001														*/
/* �t�@�C����   : sys.h															*/
/* �@�\         : �V�X�e������(�}�N����`�A�^��`�A�֐��̊O���Q�Ɛ錾)			*/
/* �ύX����     : 2017.12.20 Axia Soft Design ����		���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef		_SYS_H_			/* ��d��`�h�~ */
#define		_SYS_H_

/************************************************************/
/* �}�N��													*/
/************************************************************/
/* �ėp��` */

#define		OFF					(0)							/* �I�t					*/
#define		ON					(1)							/* �I��					*/
#define		UNKNOWN				(2)							/* ��ԕs�� ���������	*/

#define		OK					(0)							/* ����					*/
#define		NG					(1)							/* �ُ�					*/

#define		TRUE				(1)							/* �^					*/
#define		FALSE				(0)							/* �U					*/

#define		VALID				(0)							/* �L��					*/
#define		INVALID				(-1)						/* ����					*/

#define		LOW					(0)							/* �|�[�g���f�[�^LOW	*/
#define		HIGH				(1)							/* �|�[�g���f�[�^HIGH	*/

#define		ACT_L				(0)							/* �|�[�g�_��(LOW)	*/
#define		ACT_H				(1)							/* �|�[�g�_��(HIGH)	*/

#define		SET					(1)							/* �Z�b�g	*/
//#define		CLR					(0)							/* �N���A	*/

/* �G�b�W��`(�_��) */
#define		EDGE_NON				0	/*�G�b�W�Ȃ�*/
#define		EDGE_ON					1	/*ON�G�b�W*/
#define		EDGE_OFF				2	/*OFF�G�b�W*/
#define		EDGE_ON_ONESHOT			3	/*ON�G�b�W(�����V���b�g)*/
#define		EDGE_OFF_ONESHOT		4	/*OFF�G�b�W(�����V���b�g)*/

/* �}�C�R���r�b�g��` */
			/* �[�q�̓��o�͕����ݒ�*/
#define		PORT_DIR_INPUT		(0)							/* �|�[�g���͐ݒ�	*/
#define		PORT_DIR_OUTPUT		(1)							/* �|�[�g�o�͐ݒ�	*/
			/* �[�q�@�\�ݒ� */
#define		PORT_PMR_NORMAL_IO	(0)							/* �[�q�@�\ �ėpIO	*/
#define		PORT_PMR_FUNC		(1)							/* �[�q�@�\ ���Ӌ@�\	*/
			/* �|�[�g�̃G�b�W�����ݒ�*/
#define		PORT_EDGE_UP		(1)							/* �|�[�g�����G�b�W�ݒ�	*/
#define		PORT_EDGE_DOWN		(0)							/* �|�[�g������G�b�W�ݒ�	*/
#define		PORT_EDGE_BOTH		PORT_EDGE_DOWN				/* (���G�b�W�I�����͗�����ɐݒ肵�Ă���)	*/
			/* �|�[�g�̊��荞�݃Z���X�ݒ�*/
#define		PORT_SENSE_EDGE		(0)							/* �|�[�g�G�b�W�Z���X	*/
#define		PORT_SENSE_LEVEL	(1)							/* �|�[�g���x���Z���X	*/
			/* �|�[�g�G�b�W�̊��荞�ݗv���ݒ�*/
#define		INT_FACTOR_ONE_EDGE		(0)						/* �ЃG�b�W�ݒ�	*/
#define		INT_FACTOR_BOTH_EDGE	(1)						/* ���G�b�W�ݒ�	*/
#define		INT_FACTOR_LEVEL		(0)						/* (���x���Z���X�I������0�ɐݒ肵�Ă���)	*/
			/* �ʊ��荞�݂̋֎~/���ݒ�l*/
#define		ENABLE				(1)							/* ����					*/
#define		DISABLE				(0)							/* �֎~					*/
			/* ��Ƀ^�C�}�̃J�E���g�J�n/��~�Ɏg�p����*/
#define		REG_CNT_START				(1)					/* �J�n	*/
#define		REG_CNT_STOP				(0)					/* ��~	*/


/* �r�b�g��` */
#define		BIT00				(0x00000001)				/* bit0					*/
#define		BIT01				(0x00000002)				/* bit1					*/
#define		BIT02				(0x00000004)				/* bit2					*/
#define		BIT03				(0x00000008)				/* bit3					*/
#define		BIT04				(0x00000010)				/* bit4					*/
#define		BIT05				(0x00000020)				/* bit5					*/
#define		BIT06				(0x00000040)				/* bit6					*/
#define		BIT07				(0x00000080)				/* bit7					*/
#define		BIT08				(0x00000100)				/* bit8					*/
#define		BIT09				(0x00000200)				/* bit9					*/
#define		BIT10				(0x00000400)				/* bit10				*/
#define		BIT11				(0x00000800)				/* bit11				*/
#define		BIT12				(0x00001000)				/* bit12				*/
#define		BIT13				(0x00002000)				/* bit13				*/
#define		BIT14				(0x00004000)				/* bit14				*/
#define		BIT15				(0x00008000)				/* bit15				*/
#define		BIT16				(0x00010000)				/* bit16				*/
#define		BIT17				(0x00020000)				/* bit17				*/
#define		BIT18				(0x00040000)				/* bit18				*/
#define		BIT19				(0x00080000)				/* bit19				*/
#define		BIT20				(0x00100000)				/* bit20				*/
#define		BIT21				(0x00200000)				/* bit21				*/
#define		BIT22				(0x00400000)				/* bit22				*/
#define		BIT23				(0x00800000)				/* bit23				*/
#define		BIT24				(0x01000000)				/* bit24				*/
#define		BIT25				(0x02000000)				/* bit25				*/
#define		BIT26				(0x04000000)				/* bit26				*/
#define		BIT27				(0x08000000)				/* bit27				*/
#define		BIT28				(0x10000000)				/* bit28				*/
#define		BIT29				(0x20000000)				/* bit29				*/
#define		BIT30				(0x40000000)				/* bit30				*/
#define		BIT31				(0x80000000)				/* bit31				*/

#define		BITS_BY_BYTE	8	/*1�o�C�g������̃r�b�g��*/

#define		WORD1BYTE			(0xFF000000)				/* ���[�h�f�[�^�����1BYTE�� */
#define		WORD2BYTE			(0x00FF0000)				/* ���[�h�f�[�^�����2BYTE�� */
#define		WORD3BYTE			(0x0000FF00)				/* ���[�h�f�[�^�����3BYTE�� */
#define		WORD4BYTE			(0x000000FF)				/* ���[�h�f�[�^�����4BYTE�� */

#define		UH1BYTE				(0xFF00)					/* UH�f�[�^�����1BYTE�� */
#define		UH2BYTE				(0x00FF)					/* UH�f�[�^�����2BYTE�� */

/* �ėp�G���[��` */
#define		E_OK		((INT)0)			/* ����I�� */
#define		E_GENE		((INT)1)			/* �ėp�G���[ */
#define		E_OBJ		((INT)2)			/* �I�u�W�F�N�g��ԃG���[ */
#define		E_PAR		((INT)3)			/* �p�����[�^�G���[ */
#define		E_QOVR		((INT)4)			/* �L���[�C���O�I�[�o�[�t���[ */



#define		CONV_BEAM_UNIT	1000						/* ���ϊ�(�L�������K�Ȃ�)�̒P�� */

#define		DIGIT_CHANGE_KILO			1024				/* Kbyte */
#define		DIGIT_CHANGE_MEGA			( 1024 * 1024 )		/* Mbyte */

/* �|�[�g�̖����l ���O��l�Ɏg�p */
#define		PORT_INVALID		0xff						/* �|�[�g�̖����l */

/* ������` */
#define		MIN_30_SEC					( 60 * 30 )			/* �b���Z�ł�30�� */
#define		HOUR_SEC					( 60 * 60 )			/* �b���Z�ł�1���� */


/* ���Z�p�␳�l ���Œ菬���_��3�ʎl�̌ܓ����A�Œ菬���_��2�ʂ̉��Z�p */
#define		CALC_HALF_ADJUST			5					/* �l�̌ܓ��p */


/* nop�̒�` */
/* 12.000MHz�쓮��1�N���b�N83.333�dns */
#define	Nop1()		NOP();															/* 83.333ns */
#define	Nop5()		NOP();NOP();NOP();NOP();NOP();									/* 416.666ns */
#define	Nop10()		Nop5();Nop5();													/* 833.333ns */
#define	Nop20()		Nop10();Nop10();												/* 1.666��s */
#define	Nop40()		Nop20();Nop20();												/* 3.333��s */
#define	Nop120()	Nop40();Nop40();Nop40();										/* 10��s */

#define WAIT_1US()		Nop10();Nop1();Nop1();										/* ��1us�҂� */
#define WAIT_5US()		WAIT_1US();WAIT_1US();WAIT_1US();WAIT_1US();WAIT_1US();		/* ��1us�҂� */

#define WAIT_10US()		WAIT_5US();WAIT_5US();										/* ��10us�҂� */

#define WAIT_EEP_STOP_AFTER			WAIT_10US();


/* AD�ϊ��� */
#define	AD_CHANGE_CNT					10

/* �T���l�֘A */
#define	CALC_SUM_INIT		((UB)0x00)			/* �T���l�v�Z�̏����l */
#define	CALC_SUM_INIT_UH	((UH)0x0000)		/* �T���l�v�Z�̏����l */
#define	CALC_SUM_INIT_UW	((UW)0x00000000)	/* �T���l�v�Z�̏����l */

/* CRC�֘A */
#define CRCD_DEFAULT_LIB	((UH)0xFFFF)		/* CRC�f�[�^���W�X�^ �����l */

/* �G���f�B�A���ϊ� */
#define	CHANGE_UB2_UW		0			/* UBx2 �� UWx1 */
#define	CHANGE_UW_UB2		1			/* UWx1 �� UBx2 */

/* P/S���[�h */
#define PS_MODE_P			0			/* �p�`���R�d�l */
#define PS_MODE_S			1			/* �X���b�g�d�l */
#define PS_MODE_MAX			2			/* �ő��� */

/* �҂����Ԃ̒�` time_nop_wait() */
/* ���ӁF�e�҂������͗]�T�������Ă���B�v�����͊��荞�݋֎~�Ōv�����鎖 */
#define TIM_NOP_WAIT_500MS			( 625000 )		/* �R�[���h�X�^�[�g��500ms�ȏ�҂��p �����m�łȂ��Ă悢 */
#define TIM_NOP_WAIT_1MS			( 4200 )		/* 1ms�ȏ�҂��p �����m�łȂ��Ă悢 *///RD1214�b��F�l�̒������K�v
#define TIM_NOP_WAIT_10US			( 10 )			/* 10us�ȏ�҂��p �����m�łȂ��Ă悢 */

/* �֐��R�[����� */
typedef enum{
	FUNC_CALL_TYPE_POWON = 0,	/* �N����					*/
	FUNC_CALL_TYPE_NORMAL		/* �ʏ펞					*/
}FUNC_CALL_TYPE;

/* =====�ėp�}�N���֐�===== */
/* max�܂�data���C���N�������g */
#define	INC_MAX(data,max)				\
{										\
	if( data < max ){					\
		data++;							\
	}									\
}

/* max�܂�data���C���N�������g���ď���������珉���� */
#define	INC_MAX_INI(data,max,ini)		\
{										\
	if( data < max ){					\
		data++;							\
	}else{								\
		data = ini;						\
	}									\
}

/* min�܂�data���f�N�������g */
#define	DEC_MIN(data,min)				\
{										\
	if( data > min ){					\
		data--;							\
	}									\
}

/* min�܂�data���f�N�������g����min�ɂȂ����珉���� */
#define	DEC_MIN_INI(data,min,ini)		\
{										\
	if( data > min ){					\
		data--;							\
	}else{								\
		data = ini;						\
	}									\
}

/* type�ʂ�uh�̏�ʌ��������͉��ʌ���Ԃ� */
#define	UH_U		0		/* ��ʃA�h���X�擾 */
#define	UH_D		1		/* ���ʃA�h���X�擾 */

/* �P�̃f�o�b�O�p��` */
//#define STATIC							static				/* �P�̃f�o�b�O���{����STATIC�𖳂̃f�t�@�C���ɂ��Ă��ǂ� */
#define STATIC							static				/* �P�̃f�o�b�O���{����STATIC�𖳂̃f�t�@�C���ɂ��Ă��ǂ� */

/* =====�@�\�L������(ON/OFF)===== */
#define	FUNC_RELEASE					ON					/* �����[�X */
															/* ���������[�X���n�܂�����͊�{�I��ON�ŉ^�p�������[�X���̊ԈႢ���Ȃ��� */
															/* OFF�́A�f�o�b�O���Ƀ��[�J���ŏC�� */

/* �����[�X�I�v�V���� */
#if FUNC_RELEASE == ON
	//�����ǉ����邱��
#else
															/* ���f�o�b�K�ڑ����͎��Ȑf�f�|�[�g��LOW�ɂȂ邽�ߖ��������� */
#endif

//�A���h���C�h�f�o�b�O����OFF�Ƃ���
#define FUNC_ADR_COM_ERR_RESET			ON					/* �A���h���C�h�ʐM�ُ펞�̃��Z�b�g�L�� */



#define FUNC_DOWNLOAD					ON				/* �_�E�����[�h�̋��� */

/*------------*/
/* �f�o�b�O�p */
/*------------*/
/* �f�o�b�O�|�[�g�@�\ */
#define FUNC_DEBUG_PORT				ON				/* �f�o�b�O�|�[�g�@�\(ON:�f�o�b�O�|�[�g�L�� OFF:�f�o�b�O�|�[�g����) */
#define FUNC_DEBUG_PORT_LOG			ON				/* �f�o�b�O�|�[�g�Ƀ��O�o��(ON:�f�o�b�O�|�[�g���O�L�� OFF:�f�o�b�O�|�[�g���O����) */


/*----------------*/
/* �r�������[�X�p */
/*----------------*/
#define FUNC_SELF_CHECK_MODE_TEST	OFF				/* �������Ȑf�f���[�h */

#define	FUNC_DISP_LAMP_CHECK_SOFT	OFF				/* �y�����v����m�F�p�\�t�g�z(ON:�L��,OFF;����) */
													/* �@ON�ɂ���ꍇ�́A232C���O�@�\��������ON�ɂ��鎖 */
													/* �@ON�ɂ���ꍇ�́A���Ȑf�f���[�h�̂݃����[�X��������ON�ɂ��鎖 */

#define FUNC_HARD_CHECK_SOFT				OFF		/* �n�[�h�`�F�b�N�p�\�t�g */
													/* �ʐM�֘A�����o�͂��鎖�Ńn�[�h�S���̕����m�F���s�� */

/************************************************************/
/* �^��`													*/
/************************************************************/
/* �ϐ��^�̍Ē�` */
typedef signed char		B;		/* �����t��8�r�b�g���� */
typedef unsigned char	UB;		/* ��������8�r�b�g���� */
typedef signed short	H;		/* �����t��16�r�b�g���� */
typedef unsigned short 	UH;		/* ��������16�r�b�g���� */
typedef signed long		W;		/* �����t��32�r�b�g���� */
typedef unsigned long	UW;		/* ��������32�r�b�g���� */
typedef signed int		INT;	/* �v���Z�b�T�Ɏ��R�ȃT�C�Y�̕����t������ */
typedef unsigned int	UINT;	/* �v���Z�b�T�Ɏ��R�ȃT�C�Y�̕����������� */
typedef int				BOOL;	/* �^�U�l(TRUE or FALSE) */
typedef float 			FLOAT;	/* �P���x���������_ */
typedef double			DOUBLE;	/* �{�P���x���������_ */
typedef int				ER;		/* �ėp�G���[(��iTRON�\�L) */

#if 0
/* �r�b�g�t�B�[���h */
typedef struct{
    UB bit0 : 1;
    UB bit1 : 1;
    UB bit2 : 1;
    UB bit3 : 1;
    UB bit4 : 1;
    UB bit5 : 1;
    UB bit6 : 1;
    UB bit7 : 1;
}BIT_FIELD;

/* �r�b�g�t�B�[���h */
typedef union{
	BIT_FIELD	bit;
	UB	byte;
}BIT_FIELD_UNION;
#endif

/* ���� */
typedef struct{
	INT	tm_sec;		/* �b */
	INT	tm_min;		/* �� */
	INT	tm_hour;	/* �� */
	INT	tm_mday;	/* �� */
	INT	tm_mon;		/* �� */
	INT	tm_year;	/* �N */
	INT	tm_wday;	/* �j�� */
	INT	tm_yday;	/* ��(1�N�̉�����) */
	INT	tm_isdst;	/* �T�}�[�^�C���t���O */
}DATE;


/* �����O�o�b�t�@(1�o�C�g�p) */
typedef struct{
	UB*	buf;					/* �����O�o�b�t�@�p�|�C���^ */
	UH	wr_pos;					/* �������݈ʒu */
	UH	rd_pos;					/* �ǂݏo���ʒu */
	UH	size;					/* �T�C�Y */
}RING_BUF;

/* �����O�o�b�t�@(2�o�C�g�p) */
typedef struct{
	UH*	buf;					/* �����O�o�b�t�@�p�|�C���^ */
	UH	wr_pos;					/* �������݈ʒu */
	UH	rd_pos;					/* �ǂݏo���ʒu */
	UH	size;					/* �T�C�Y */
}RING_BUF_UH;

/* �����R���p */
typedef struct{
	UW	time;						/* ���� */
	UB	key_code;					/* �L�[�R�[�h */
	UB	kind;						/* �����R�����(�Z�L�����e�B/�ʏ�) */
	UB	staff_code;					/* �X�^�b�t�R�[�h */
}REMO;

/* �����O�o�b�t�@(���ԕt���o�C�g�f�[�^) */
typedef struct{
	REMO*	buf;			/* �����O�o�b�t�@�p�|�C���^ */
	UH	wr_pos;					/* �������݈ʒu */
	UH	rd_pos;					/* �ǂݏo���ʒu */
	UH	size;					/* �T�C�Y */
}RING_BUF_REMO;

/* ���o�͏��p */
typedef struct{
	UB	sync;					/* �����p�f�[�^ */
	UB	port1;					/* ���o�͏��1 */
	UB	port2;					/* ���o�͏��2 */
	UB	port3;					/* ���o�͏��3 */
}IO_INFO;

/* �����O�o�b�t�@(3�o�C�g(24bit)�p) */
typedef struct{
	IO_INFO*	buf;			/* �����O�o�b�t�@�p�|�C���^ */
	UH	wr_pos;					/* �������݈ʒu */
	UH	rd_pos;					/* �ǂݏo���ʒu */
	UH	size;					/* �T�C�Y */
}RING_BUF_IO;


/* ���W�ʒu */
typedef struct{
	H	x;			/* x���W �|�W�V�������܂���AD�ϊ��l */
	H	y;			/* y���W �|�W�V�������܂���AD�ϊ��l */
}POS_INFO;


/* �}�X�J�u�����荞�݋֎~/���� */
/* �����݂̋֎~/����#pragma�g���@�\�ŃT�|�[�g����Ă��� */
//#ifndef __RL78__
//#define	DI()	asm("FCLR I")
//#define	EI()	asm("FSET I")
//#endif

/* �}�X�J�u�����荞�݋֎~/����(�O���Ԃւ̕��A) */
/* ���ӎ����F�O���Ԃւ̕��A�ƂȂ邽�߂��Ȃ炸����֐����ŃZ�b�g�Ŏg�p���ĉ����� */
#if 0
#define DI_RET()		\
{						\
	__asm("PUSH PSW");	\
	DI();				\
}
#define EI_RET()		\
{						\
	__asm("POP  PSW");	\
}
#else
// RD8001�b�� ��L�ŃG���[���o��B
#define DI_RET()		\
{						\
}
#define EI_RET()		\
{						\
}



#endif

/************************************************************/
/* �O���Q�Ɛ錾												*/
/************************************************************/
void ring_buf_init( RING_BUF* p_ring, UB* p_data, UH size);
INT read_ring_buf( RING_BUF* p_ring, UB* p_data );
INT write_ring_buf( RING_BUF* p_ring ,UB data );
UB	hex2bin(UB num);
UB	uwhex2bin(UW *bin, UB *pstr);
UB bcdbin( UB bin );
INT bcd2bin( UB *bin, const UB *src_bcd );
void dummy( void );

#endif
/************************************************************/
/* END OF TEXT												*/
/************************************************************/


/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : time.h														*/
/* �@�\         : ���ԊǗ�(�}�N����`�A�^��`�A�֐��̊O���Q�Ɛ錾)				*/
/* �ύX����     : 2018.01.25 Axia Soft Design ���� ��	���ō쐬		*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef		_TIME_H_			/* ��d��`�h�~ */
#define		_TIME_H_

/************************************************************/
/* �}�N��													*/
/************************************************************/
/* �^�C�}���[1ms] */
/* ���ӁF�������ׂɗ^����e�����傫���ׂɈ��Ղɑ��₳�Ȃ��� */
typedef enum{
	TIME_TYPE_1MS_DUMMY	= 0,							/* �_�~�[[30ms] */
	TIME_TYPE_1MS_NUM									/* �\�t�g�E�F�A�^�C�}�̐�(1ms) */
}TIME_TYPE_1MS;

/* �^�C�}�J�E���g�l[1ms] */
#define	TIME_TYPE_1MS_DUMMY						(30)		/* �_�~�[ */



/* �^�C�}���[10ms] */
typedef enum{
	TIME_TYPE_10MS_POW_SW = 0				,			/* �d��SW���� */
	TIME_TYPE_10MS_CPU_COM_RETRY			,			/* CPU�ԒʐM���g���C�҂��^�C�} */
	TIME_TYPE_10MS_SENSING_DELAY			,			/* �Z���V���O�J�n�҂� */
	TIME_TYPE_10MS_DISP_FLASH				,			/* �\���_�� */
	TIME_TYPE_10MS_WAIT_SLEEP				,			/* sleep�҂� */
	TIME_TYPE_10MS_BAT_CHG_FIN				,			/* �[�d�����`�F�b�N */
	TIME_TYPE_10MS_SW_LONG_DISP				,			/* �d��SW�������\�� */
	TIME_TYPE_10MS_CHG_ON_DISP_IVALID		,			/* �[�d�\���������� */
	TIME_TYPE_10MS_NUM									/* �\�t�g�E�F�A�^�C�}�̐�(10ms) */
}TIME_TYPE_10MS;

/* �^�C�}�J�E���g�l[10ms] */
#define	TIME_10MS_CNT_POW_SW_INITIAL_DISP	1000			/* �d��SW_���������Z�b�g�\��(10�b) */
#define	TIME_10MS_CNT_POW_SW_LONG			300				/* �d��SW_��(3�b) */
#define	TIME_10MS_CNT_POW_SW_SHORT			100				/* �d��SW_�Z(1�b) */



#define	TIME_10MS_CNT_POW_SW_LONG_DISP		300				/* �d��SW�������\��(3�b) */

#define	TIME_10MS_CNT_CHG_ON_DISP_IVALID	100				/* �[�d�\����������(1�b) */


#define	TIME_10MS_CNT_STATE_STOP			1000		/* �x�~��ԑJ�ڎ���(10�b) */

#define	TIME_10MS_CNT_SENSING_DELAY			100			/* �Z���V���O�J�n��(1�b) */

#define	TIME_10MS_CNT_WAIT_SLEEP			( 2 + 1 )	/* sleep�҂�[10ms�ȏ�] */

#define	TIME_10MS_CNT_BAT_CHG_FIN			( 50 + 1 )	/* �[�d�����`�F�b�N[500ms�ȏ�] */

// �\���_��
#define	TIME_10MS_CNT_DISP_FLASH_50MS		5				/* �\���_��(50ms) */
#define	TIME_10MS_CNT_DISP_FLASH_100MS		10				/* �\���_��(100ms) */
#define	TIME_10MS_CNT_DISP_FLASH_500MS		50				/* �\���_��(500ms) */
#define	TIME_10MS_CNT_DISP_FLASH_1S			100				/* �\���_��(1s) */
#define	TIME_10MS_CNT_DISP_FLASH_3S			300				/* �\���_��(3s) */

/************************************************************/
/* �^��`													*/
/************************************************************/

/************************************************************/
/* �O���Q�Ɛ錾												*/
/************************************************************/
extern void time_init( void );
extern void time_update_elapsed( void );
extern void time_soft_set_1ms( TIME_TYPE_1MS type ,UW cnt );
extern void time_soft_set_10ms( TIME_TYPE_10MS type ,UW cnt );
extern void time_soft_get_1ms( TIME_TYPE_1MS type, UW* p_cnt );
extern void time_soft_get_10ms( TIME_TYPE_10MS type, UW* p_cnt );
extern void time_get_elapsed_time(UW *petm);
extern void time_set_elapsed_time(UW etm);
extern void time_soft_dec_1ms( void );
extern void time_soft_dec_10ms( void );


#endif

/************************************************************/
/* END OF TEXT												*/
/************************************************************/


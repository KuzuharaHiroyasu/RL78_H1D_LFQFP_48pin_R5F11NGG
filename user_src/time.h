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
	TIME_TYPE_10MS_POW_SW_ACT_START = 0		,			/* �d��SW����J�n(1�b) */
	TIME_TYPE_10MS_CPU_COM_RETRY			,			/* CPU�ԒʐM���g���C�҂��^�C�} */
	TIME_TYPE_10MS_SENSING_DELAY			,			/* �Z���V���O�J�n�҂� */
	TIME_TYPE_10MS_NUM									/* �\�t�g�E�F�A�^�C�}�̐�(10ms) */
}TIME_TYPE_10MS;

/* �^�C�}�J�E���g�l[10ms] */
#define	TIME_10MS_CNT_POW_SW_ACT_START		100				/* �d��SW����J�n(1�b) */
#define	TIME_10MS_CNT_STATE_STOP			1000			/* �x�~��ԑJ�ڎ���(10�b) */

#define	TIME_10MS_CNT_SENSING_DELAY				100				/* �Z���V���O�J�n��(1�b) */


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


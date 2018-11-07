/****************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[										*/
/* �t�@�C����   : com_srv.h													*/
/* �@�\         : ���O�ʐM(RS232C)											*/
/* �ύX����		: 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/* ���ӎ���     : �Ȃ�														*/
/****************************************************************************/
#ifndef _COM_SRV_H_
#define _COM_SRV_H_


/************************************************************/
/* �萔��`													*/
/************************************************************/
#define	DRV_UART1_DATA_LENGH					150									/* RS232C�ʐM�f�[�^��(����A����) �w�b�_�W�o�C�g�{�I�v�V�����f�[�^92�o�C�g[�\��] */
#define	DRV_UART1_RCV_RING_LENGTH				( DRV_UART1_DATA_LENGH * 1 + 1 )		/* �����O�o�b�t�@��(1���b�Z�[�W���{�P) ���{�P�̗��R�̓����O�o�b�t�@�̒��ӎ����Q�� */
#define	DRV_UART1_SND_RING_LENGTH				( DRV_UART1_DATA_LENGH * 1 + 1 )		/* �����O�o�b�t�@��(1���b�Z�[�W���{�P) ���{�P�̗��R�̓����O�o�b�t�@�̒��ӎ����Q�� */

#define COM_STR_MAX				DRV_UART1_DATA_LENGH

#define COM_ANA_MSG_MAX			10		// ��͍Œ�10�o�C�g
#define COM_RCV_NON_CNT_MAX		200		// ��M�Ȃ�����J�E���g

/******************/
/*     �^��`     */
/******************/
typedef struct{
	UB len;
	UB data[COM_ANA_MSG_MAX];
	UB rcv_non;
}COM_SRV_ANA;



/******************/
/*  �O���Q�Ɛ錾  */
/******************/

extern void com_srv_init( void );
extern void com_srv_log_title( void );
extern void com_srv_cyc( void );
extern void com_srv_send( UB* tx_data, UB len );
extern void com_srv_puts( const B* pMsg);


/******************/
/* �֐��}�N����` */
/******************/
	
#endif

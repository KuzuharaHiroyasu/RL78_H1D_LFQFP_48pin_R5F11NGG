/************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[									*/
/* �t�@�C����   : com_srv.c                                             */
/* �@�\         : ���O�ʐM�iRS-232C�j				                    */
/* �ύX����     : 2018.01.25 Axia Soft Design ���� ��	���ō쐬		*/
/* ���ӎ���     : �Ȃ�                                                  */
/************************************************************************/
/********************/
/*     include      */
/********************/
#include "header.h"

/********************/
/* �v���g�^�C�v�錾 */
/********************/
void com_srv_init( void );
void com_srv_log_title( void );
void com_srv_cyc( void );
STATIC void com_srv_command( UB data );
void com_srv_send( UB* tx_data, UB len );

/********************/
/*     �����萔     */
/********************/
#define COM_STR_MAX			100

#define COM_ANA_MSG_MAX			10		// ��͍Œ�10�o�C�g
#define COM_RCV_NON_CNT_MAX		200		// ��͍Œ�10�o�C�g

typedef struct{
	UB len;
	UB data[COM_ANA_MSG_MAX];
	UB rcv_non;
}COM_SRV_ANA;

static COM_SRV_ANA	s_com_srv_ana;

/********************/
/*     �����ϐ�     */
/********************/


/********************/
/*     �O���Q��     */
/********************/
extern const B		version_product_tbl[];				/* �\�t�g�E�F�A�o�[�W���� */
extern const UB	* const version_boot_tbl;				/* �u�[�g�o�[�W�����A�h���X */

extern void getmode_in( void );
extern void drv_uart1_data_init( void );

/************************************************************************/
/* �֐�     : com_srv_init                                              */
/* �֐���   : �ێ�A�v������������                                      */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �ێ�A�v������������                                          */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
void com_srv_init( void )
{
#if FUNC_DEBUG_LOG == ON
	memset( &s_com_srv_ana, 0x00, sizeof(s_com_srv_ana) );

	drv_uart1_data_init();
#endif
}

/************************************************************************/
/* �֐�     : com_srv_log_title                                         */
/* �֐���   : �^�C�g���o��			                                    */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �^�C�g���o�͏����̊֐���                                      */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
void com_srv_log_title( void )
{
#if FUNC_DEBUG_LOG == ON
	UB	boot_ver[4];				/* Boot���o�[�W������� */
	

	int len;
	UB tx_data[COM_STR_MAX] = {0};
	
	len = sprintf((char*)tx_data, "RD8001 POW_ON APL Ver.%d.%d.%d.%d\r\n", version_product_tbl[0], version_product_tbl[1],
																	   version_product_tbl[2], version_product_tbl[3] );
	
	com_srv_send( tx_data, len );

	// �A�v�����ێ����Ă���A�h���X����u�[�g�o�[�W�����𐶐�
	memcpy((UB *)boot_ver, (UB *)version_boot_tbl, sizeof(boot_ver));

	len = sprintf((char*)tx_data, "RD8001 POW_ON BOOT Ver.%d.%d.%d.%d\r\n", boot_ver[0], boot_ver[1],
																	   boot_ver[2], boot_ver[3] );
	
	com_srv_send( tx_data, len );
#endif
}

/************************************************************************/
/* �֐�     : com_srv_cyc                                               */
/* �֐���   : �����\������                                              */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����\������                                                  */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
void com_srv_cyc( void )
{
#if FUNC_DEBUG_LOG == ON
	RING_BUF* p_ring_buf = NULL;
	UB rcv_data;
	
	p_ring_buf = drv_uart1_get_rcv_ring();

	if(E_OK == read_ring_buf( p_ring_buf, &rcv_data )){
		// ��M�f�[�^����
		com_srv_command( rcv_data );
	}else{
		if( s_com_srv_ana.rcv_non++ >= COM_RCV_NON_CNT_MAX ){
			// �����Ԗ���M�Ŏ�M��ԃN���A
			s_com_srv_ana.rcv_non = 0;
		}
	}
#endif
}

/************************************************************************/
/* �֐�     : com_srv_command                                           */
/* �֐���   : �R�}���h��͏���                                          */
/* ����     : pInData	�R�}���h��									    */
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �R�}���h��͏���                                              */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
//RD8001�b��F�r�������[�X�p�R�}���h
STATIC void com_srv_command( UB data )
{
#if FUNC_DEBUG_LOG == ON
	if( 0 == s_com_srv_ana.len ){
		if( 'G' == data ){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len++;
		}else{
			s_com_srv_ana.len = 0;
		}
	}else if( 1 == s_com_srv_ana.len ){
		if(( 'E' == data ) && ( 'G' == s_com_srv_ana.data[0] )){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len++;
		}else{
			s_com_srv_ana.len = 0;
		}
	}else if( 2 == s_com_srv_ana.len ){
		if(( 'T' == data ) && ( 'E' == s_com_srv_ana.data[1] )){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len++;
		}else{
			s_com_srv_ana.len = 0;
		}
#if 0
	}else{
		s_com_srv_ana.len = 0;
	}
#else
	}else if( 3 == s_com_srv_ana.len ){
		if(( '\r' == data ) && ( 'T' == s_com_srv_ana.data[2] )){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len++;
		}else{
			s_com_srv_ana.len = 0;
		}
	}else if( 4 == s_com_srv_ana.len ){
		if(( '\n' == data ) && ( '\r' == s_com_srv_ana.data[3] )){
			s_com_srv_ana.data[s_com_srv_ana.len] = data;
			s_com_srv_ana.len = 0;
			getmode_in();
		}else{
			s_com_srv_ana.len = 0;
		}
	}
#endif
#endif
}

void com_srv_send( UB* tx_data, UB len )
{
#if FUNC_DEBUG_LOG == ON
	RING_BUF* p_ring_buf;
	UB i = 0;
	UB ret;
	
	p_ring_buf = drv_uart1_get_snd_ring();
	
	/* ���M�o�b�t�@��S�ď������� */
	for(i = 0; i < len; i++){
		ret = write_ring_buf(p_ring_buf, (UB)tx_data[i] );
		if(ret != E_OK){
			break;
		}
	}
	
	/* ���M�J�n */
	drv_uart1_send_start();
#endif
}


/************************************************************************/
/* �֐�     : com_srv_puts                                              */
/* �֐���   : �W���o�͏���(�Œ蕶����̂�)                              */
/* ����     : ������													*/
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2017.03.02  Axia Soft Design ���� ��   ���ō쐬           */
/************************************************************************/
/* �@�\ : �W���o�͏���(�Œ蕶����̂�)				                    */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
void com_srv_puts( const B* pMsg)
{
#if FUNC_DEBUG_LOG == ON
	RING_BUF* p_ring_buf;
	
	p_ring_buf = drv_uart1_get_snd_ring();
	
	/* ���M�o�b�t�@��S�ď������� */
	while(*pMsg != 0x00){
		if( E_OK != write_ring_buf(p_ring_buf, (UB)(*pMsg) )){
			return;
		}
		pMsg++;
	}
	
	/* ���M�J�n */
	drv_uart1_send_start();
#endif
}

/*****************************************************************************
 *								 end of text
 *****************************************************************************/

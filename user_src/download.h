/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[												*/
/* �t�@�C����   : download.h													*/
/* �@�\         : �v���O�������������Q�̌��J��`����уC���^�t�F�[�X			*/
/* �ύX����     : 2012.02.25 Axia Soft Design K.Wada	���ō쐬				*/
/*              : 2012.12.26 Axia Soft Design K.Wada    RD1201���痬�p�J�n      */
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef		_DOWNLOAD_H_			/* ��d��`�h�~ */
#define		_DOWNLOAD_H_

#include "./header.h"
#include "common_memory_def.h"


/******************/
/* �萔�}�N����` */
/******************/

/******************/
/*     �^��`     */
/******************/

/******************/
/*    ���J�萔    */
/******************/


/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void download_init(void);
extern void download_change_boot(void);
extern INT download_start(void);
extern INT download_set_data(UB *p_data, UW blocknum, UW size, UW *p_sum);
extern void download_finish_ready(void);
extern INT download_finish(void);
extern BOOL download_check_finish(UW com_sum);
extern BOOL download_check_checksum(void);
extern BOOL download_check_product(void);
extern INT download_get_data(UB *p_get_data,UW blocknum ,UW size);

/******************/
/* �֐��}�N����` */
/******************/

#endif /*_DOWNLOAD_H_ */

/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : drv_from.h													*/
/* �@�\         : FROM�̓ǂݏ���												*/
/* �ύX����     : 2012.12.21 Axia Soft Design K.Wada	���ō쐬				*/
/*              : 2012.12.26 Axia Soft Design K.Wada    RD1201���痬�p�J�n      */
/*              : 2018.04.01 Axia Soft Design S.Shima   RD1215���痬�p�J�n      */
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef		_DRV_FROM_H_			/* ��d��`�h�~ */
#define		_DRV_FROM_H_


/* �t���b�V���E�Z���t�E�v���O���~���O�E���C�u���� */
#include "fsl.h"                /* ���C�u�����E�w�b�_�[�t�@�C��             */
#include "fsl_types.h"          /* ���C�u�����E�w�b�_�[�t�@�C��             */

/******************/
/* �萔�}�N����` */
/******************/
/* 臒l */
#define		DRV_FROM_ERASE_TIME_MAX				1000	/* 10s(10ms * DRV_FROM_ERASE_TIME_MAX)	 */
#define		DRV_FROM_CMD_RO_CNT					20000	/* ���g���C�A�E�g��					 */
#define		DRV_FROM_ERASE_CNT					3		/* �n�[�h�E�F�A�}�j���A�����w��		 */

/* �������ݒP�� */
#define 	WRITE_SIZE_WORD						(64)
#define 	WORD_SIZE							(4)		/* �W�����[�h�T�C�Y */
#define 	WRITE_SIZE_BYTE						(WRITE_SIZE_WORD*WORD_SIZE)

#define		BLOCK_SIZE							(0x400l) /* �W���u���b�N�E�T�C�Y         */

/******************/
/*     �^��`     */
/******************/
typedef enum  {
	// �R�[�h�t���b�V���E������:128KB
	DRV_FROM_BLOCK_NUM               = 0x80,	/* �S�� */
	DRV_FROM_BLOCK_BOOTCLUSTER0_SIZE = 0x04,	/* �u�[�g�N���X�^0:4KB */
	DRV_FROM_BLOCK_BOOTCLUSTER1_SIZE = 0x04,	/* �u�[�g�N���X�^1:4KB */
	DRV_FROM_BLOCK_BOOTCLUSTER_SIZE  = DRV_FROM_BLOCK_BOOTCLUSTER0_SIZE + DRV_FROM_BLOCK_BOOTCLUSTER1_SIZE,

	DRV_FROM_BLOCK_TOP               = 0x00,
	DRV_FROM_BLOCK_END               = DRV_FROM_BLOCK_TOP + DRV_FROM_BLOCK_NUM -1,
	
	DRV_FROM_BLOCK_BOOTCLUSTER0_TOP  = 0x00,
	DRV_FROM_BLOCK_BOOTCLUSTER0_END  = DRV_FROM_BLOCK_BOOTCLUSTER0_TOP + DRV_FROM_BLOCK_BOOTCLUSTER0_SIZE -1,
	DRV_FROM_BLOCK_BOOTCLUSTER1_TOP  = DRV_FROM_BLOCK_BOOTCLUSTER0_END + 1,
	DRV_FROM_BLOCK_BOOTCLUSTER1_END  = DRV_FROM_BLOCK_BOOTCLUSTER1_TOP + DRV_FROM_BLOCK_BOOTCLUSTER1_SIZE -1,

	DRV_FROM_BLOCK_PROGRAM_TOP       = 0x01,
	DRV_FROM_BLOCK_PROGRAM_END       = DRV_FROM_BLOCK_NUM - 1,
	
	DRV_FROM_BLOCK_PROGRAM_NUM       = DRV_FROM_BLOCK_NUM - DRV_FROM_BLOCK_BOOTCLUSTER_SIZE
}DRV_FROM_BLOCK_NO;

/* FROM�G���[�R�[�h */
typedef enum  {
	DRV_FROM_ERR_NONE = 0,		/* �G���[�Ȃ� */
	DRV_FROM_ERR_ERASE,			/* �C���[�X�G���[ */
	DRV_FROM_ERR_PROGRAM,		/* �v���O�����G���[ */
	DRV_FROM_ERR_PARAM,			/* �w��p�����[�^�ُ� */
	DRV_FROM_ERR_STATUS_ERROR	/* �����X�e�[�^�X�G���[ */
}DRV_FROM_ERROR;

/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void				drv_from_init(void);
extern void				drv_from_mode_enable(void);
extern void				drv_from_mode_disable(void);
extern DRV_FROM_ERROR	drv_from_read(UB  __far *, UB  __far *, UW );
extern DRV_FROM_ERROR	drv_from_write(UB __far  *addr_src ,const  UB __far  *addr_dest, UW size);
extern DRV_FROM_ERROR	drv_from_erase_block(DRV_FROM_BLOCK_NO );


/******************/
/* �֐��}�N����` */
/******************/

#endif /*_DRV_FROM_H_ */

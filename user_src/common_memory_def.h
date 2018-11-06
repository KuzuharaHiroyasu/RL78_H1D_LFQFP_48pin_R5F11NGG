/********************************************************************************/
/* �V�X�e����   : RD1215 ���L�������̈��`										*/
/* �t�@�C����   : common_memory_def.h											*/
/* �@�\         : �u�[�g�A���i�v���O�������L�̃������̈��`					*/
/* �ύX����     : 2012.12.21 Axia Soft Design K.Wada	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef		_COMMON_MEMORY_DEF_H_			/* ��d��`�h�~ */
#define		_COMMON_MEMORY_DEF_H_

//#include "lamp_iobox_common_def.h"

/******************/
/* �萔�}�N����` */
/******************/
#define PRODUCT_ID_SIZE				(16)
#define PRODUCT_ID_OFFSET_SIZE		(0)
#define PRODUCT_ID_OFFSET_SUM		(4)
#define PRODUCT_ID_OFFSET_ID		(8)
#define PRODUCT_ID_OFFSET_SIZE_PTR	(PRODUCT_ID_OFFSET_SIZE / sizeof(UW))	/* �|�C���^�p */
#define PRODUCT_ID_OFFSET_SUM_PTR	(PRODUCT_ID_OFFSET_SUM / sizeof(UW))	/* �|�C���^�p */
#define PRODUCT_ID_OFFSET_ID_PTR	(PRODUCT_ID_OFFSET_ID / sizeof(UW))		/* �|�C���^�p */

#define COMM_BLOCK_SIZE				(256)	/* �ʐM���1�u���b�N�̃f�[�^�T�C�Y(�P�ʁF�o�C�g) */
#define VERIFY_SIZE_MAX				COMM_BLOCK_SIZE

#define BLOCK_NUM_MIN				(IOBOX_BLOCK_NUM_MIN)			/* �u���b�N�ԍ��Ή��A�h���X : 0x000400�` 256�o�C�g�� */
#define BLOCK_NUM_MAX				(IOBOX_BLOCK_NUM_MAX)			/* �u���b�N�ԍ��Ή��A�h���X : 0x01FB00�` 256�o�C�g�� */
#define BLOCK_NUM_CNT				(IOBOX_BLOCK_NUM_CNT)

#define BLOCK_NUM_MIN_UPDATE		(IOBOX_BLOCK_NUM_MIN_UPDATE)	/* �������Ώ�: 0x00A000�`0x01FBFF  */
#define BLOCK_NUM_MAX_UPDATE		(IOBOX_BLOCK_NUM_MAX_UPDATE)	/*             87KB�� */
#define BLOCK_NUM_CNT_UPDATE		(IOBOX_BLOCK_NUM_CNT_UPDATE)

//RD1201�ƃu�[�g�ƃA�v���̏��Ԃ��ς�����̂ŁA�{�}�N���̈Ӗ��������t�ɂȂ��Ă���̂Œ��ӂ��邱��
//#define BLOCK_NUM_NOT_WRITE			(0x00A0)

#define FIX_VECT_TBL_SIZE			(128)
/* ���i�v���O�����x�N�^�e�[�u���擪 (!����!)vect_tbl.c�� VCT_TBL �̔z�u�ƈ�v�����邱�� */
//#define PRODUCT_VECT_TBL_HEAD_ADDR	(0x01FEF0) /* ���ɕt���邪�AID10h,�x�N�^�e�[�u��100h������w�� */
#define PRODUCT_VECT_TBL_HEAD_ADDR	(0x01FAF0) /* ���ɕt���邪�AID10h,�x�N�^�e�[�u��100h������w�� */

#endif /*_COMMON_MEMORY_DEF_H_ */

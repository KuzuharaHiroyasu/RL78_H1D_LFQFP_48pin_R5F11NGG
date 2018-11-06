/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[												*/
/* �t�@�C����   : download.c                                                    */
/* �@�\         : �v���O�������������~�h��                                      */
/* �ύX����     : 2012.02.25 Axia Soft Design K.Wada    ���ō쐬(RD1201)        */
/*              : 2012.12.26 Axia Soft Design K.Wada    RD1201���痬�p�J�n      */
/* ���ӎ���     : �Ȃ�                                                          */
/********************************************************************************/

#include "header.h"

extern const UB __far ProductIdCode[PRODUCT_ID_SIZE];

/********************/
/*     �����萔     */
/********************/
/* FROM�u���b�N�T�C�Y */
#define	PRODUCT_PRG_BLOCK_SIZE	 	(87)
#define	BOOTLOADER_BLOCK_SIZE	 	(DRV_FROM_BLOCK_PROGRAM_NUM - PRODUCT_PRG_BLOCK_SIZE)

/* FROM�u���b�N�ԍ� */
#define	PRODUCT_PRG_BLOCK_NUM_TOP	(0x28)
#define	PRODUCT_PRG_BLOCK_NUM_END	(PRODUCT_PRG_BLOCK_NUM_TOP + PRODUCT_PRG_BLOCK_SIZE -1)
#define	BOOTLOADER_BLOCK_NUM_TOP	(PRODUCT_PRG_BLOCK_NUM_END +1)
#define	BOOTLOADER_BLOCK_NUM_END	(BOOTLOADER_BLOCK_NUM_TOP + BOOTLOADER_BLOCK_SIZE -1)

const UW ComBlockNumPerFromBlock = (BLOCK_SIZE / COMM_BLOCK_SIZE);	/* FROM��1�u���b�N���ʐM��̉��u���b�N�ɓ����邩 */

const UW ProductIdAddr = (UW)0x01FBF0;		/* ���ʃR�[�h �擪�A�h���X */




/* FROM�A�h���X��` */
const UW ProductIdBlockNum = (UW)0x1FB;		/* ���ʃR�[�h���܂ޒʐM�u���b�N�ԍ� */

const UW ProductPrgAddr = (UW)0x00A000;	/* ���i�p�t�@�[���擪�A�h���X */
const UW ProductPrgSize = (UW)0x015BF0;	/* ���i�p�t�@�[���T�C�Y(87KB-���ʃR�[�h�T�C�Y) */
const UW BootLoaderAddr = (UW)0x000000;	/* �u�[�g���[�_�[�擪�A�h���X */
const UW BootLoaderSize = (UW)0x00A000;	/* �u�[�g���[�_�[�T�C�Y(40KB) */

/********************/
/*     �����ϐ�     */
/********************/

// ID�p�ɌŒ�̃Z�N�V������ݒ肷��K�v����I�I�I
//#pragma SECTION bss bss_id
//STATIC UB id_area[PRODUCT_ID_SIZE];

STATIC UB write_buf[COMM_BLOCK_SIZE];	/* �����ݗp�ꎞ�̈� */
STATIC UB verify_buf[VERIFY_SIZE_MAX];	/* �x���t�@�C�p�ꎞ�̈� */
STATIC UB id_temp_buf[PRODUCT_ID_SIZE];	/* ����ID�ۑ��̈� */


/*********************/
/* �v���g�^�C�v�錾 */
/********************/
STATIC void download_delete_id(void);
STATIC void download_make_id(UB *p_id);
STATIC BOOL download_verify(UB *addr_src , UB *addr_dest, UW size);
STATIC UW download_blocknum2addr(UW blocknum);
STATIC UW download_com_blocknum2from_blocknum(UW blocknum);

/************************************************************************/
/* �֐�     : download_init                                             */
/* �֐���   : �v���O����������������������                              */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.10.03 Axia Soft Design ���� ��	�������f�[�^�ǉ�    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �v���O�����������̃f�[�^������                                */
/************************************************************************/
/* ���ӎ��� :                                                           */
/************************************************************************/
void download_init(void)
{
	memset( &write_buf[0], 0x00, sizeof(write_buf) );
	memset( &verify_buf[0], 0x00, sizeof(verify_buf) );
	memset( &id_temp_buf[0],0x00, sizeof(id_temp_buf) );
}

/************************************************************************/
/* �֐�     : download_change_boot                                      */
/* �֐���   : �u�[�g�v���O�����؂�ւ�����                              */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �u�[�g�v���O�����̃v���O������������Ԃɐ؂�ւ���            */
/************************************************************************/
/* ���ӎ��� : WDT�𗘗p���Ď��ȃ��Z�b�g����                             */
/************************************************************************/
/* �g�p����v���O���� : ���i�v���O����                                  */
/************************************************************************/
void download_change_boot(void)
{
	download_delete_id();

	/* WDT�Ń��Z�b�g���� */
/*	DI();	//���荞�݋֎~		// RD1201�b�� �����Ń��Z�b�g���Ȃ�
/*	while(1){};	*/				// RD1201�b�� �����Ń��Z�b�g���Ȃ�
}

/************************************************************************/
/* �֐�     : download_delete_id                                        */
/* �֐���   : �`�F�b�N�T�����܂�ID�R�[�h���폜���鏈��                  */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : ID�R�[�h���폜����(0����������)                               */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
STATIC void download_delete_id(void)
{
	
	UB clr_src[PRODUCT_ID_SIZE] ={0};
	DRV_FROM_ERROR drv_err;
	
	drv_from_init();
	drv_from_mode_enable();
	
	//���ʃR�[�h�폜
	drv_err = drv_from_write( &clr_src[0],(UB __far *)ProductIdAddr, PRODUCT_ID_SIZE);
	
	drv_from_mode_disable();
}

/************************************************************************/
/* �֐�     : download_start                                            */
/* �֐���   : �v���O���������J�n������(����)                            */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : ��������                                                  */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ :FROM�̐��i�v���O�����̈���C���[�X����                         */
/************************************************************************/
/* ���ӎ��� : �v���O�����]���O�ɕK��1��s������                         */
/************************************************************************/
/* �g�p����v���O���� : �u�[�g                                          */
/************************************************************************/
INT download_start(void)
{
	INT i;
	DRV_FROM_ERROR drv_err;
	INT ret = E_OK;
	
	drv_from_mode_enable();
	
	// �v���O�����S�̈�C���[�X
	for(i=PRODUCT_PRG_BLOCK_NUM_TOP; i<PRODUCT_PRG_BLOCK_NUM_END+1; i++){
		drv_err = drv_from_erase_block((DRV_FROM_BLOCK_NO)i);
		if(drv_err == DRV_FROM_ERR_NONE){
			ret = E_OK;
		}else{
			ret = E_GENE;
			break;
		}
	}
	
	drv_from_mode_disable();

	memset( &id_temp_buf[0],0x00, sizeof(id_temp_buf) );
	
	return ret;
}

/************************************************************************/
/* �֐�     : download_set_data                                         */
/* �֐���   : �v���O�����������ݏ���                                    */
/* ����     : p_data �������ރf�[�^�̃A�h���X                           */
/*          : blocknum �ʐM��̃u���b�N�ԍ�                             */
/*          : size �������ރf�[�^�̃T�C�Y (�ő� COMM_BLOCK_SIZE)        */
/* �߂�l   : ��������                                                  */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/*          : 2013.04.09 Axia Soft Design K.Wada    �`�F�b�N�T���v�Z�ǉ�*/
/************************************************************************/
/* �@�\ : �v���O�����f�[�^(HEX�`�� = ROM�C���[�W���̂܂�)����������     */
/************************************************************************/
/* ���ӎ��� : �J�n�����������O�ɍs���K�v������(����̂�)                */
/************************************************************************/
/* �g�p����v���O���� : �u�[�g                                          */
/************************************************************************/
INT download_set_data(UB *p_data, UW blocknum, UW size, UW *p_sum)
{
	DRV_FROM_ERROR drv_err;
	INT ret = E_OK;
	UW write_addr = download_blocknum2addr(blocknum);
	UW from_blocknum = download_com_blocknum2from_blocknum(blocknum);
	
	/* �p�����[�^�`�F�b�N */
	if( (from_blocknum < PRODUCT_PRG_BLOCK_NUM_TOP)||(PRODUCT_PRG_BLOCK_NUM_END < from_blocknum) ){
		return E_GENE;
	}
	
	/* ����������\��������̂ŁA�R�s�[���g�� */
	if(size <= COMM_BLOCK_SIZE){
		memcpy(write_buf,p_data,(size_t)size);
	}else{
		return E_GENE;
	}
	
	/* ���ʃR�[�h�̈��download_finish�ŏ������߁A�����ł͏������Ȃ�(0xFF������) */
	if( blocknum == ProductIdBlockNum ){
		memset((UB*)(write_buf + (COMM_BLOCK_SIZE-PRODUCT_ID_SIZE)) ,0xFF, PRODUCT_ID_SIZE);
	}
	
	drv_from_mode_enable();
	
	/* �������� */
	drv_err = drv_from_write(write_buf, (UB *)write_addr, size);
	
	drv_from_mode_disable();
	
	if(drv_err == DRV_FROM_ERR_NONE){
		/* �x���t�@�C */
		if(download_verify(write_buf, (UB *)write_addr, size) ){
			ret = E_OK;
			
			/* �`�F�b�N�T���v�Z */
			/* write_buf�̓x���t�@�C���N���A���Ă���̂ŁA����FROM��ǂނ̂Ɠ��ӂƂ��� */
			if( blocknum == ProductIdBlockNum ){
				/* ����ID�̉ӏ���FROM�ɏ����Ȃ��̂ŁA��M�f�[�^���㏑�����Čv�Z���� */
				memcpy((UB*)(write_buf + (COMM_BLOCK_SIZE-PRODUCT_ID_SIZE)) ,(UB*)(p_data + (COMM_BLOCK_SIZE-PRODUCT_ID_SIZE)), PRODUCT_ID_SIZE);
			}
			calc_sum_32_any_times(p_sum, write_buf, size );
			
		}else{
			ret = E_GENE;
		}
	}else{
		ret = E_GENE;
	}
	
	return ret;
}

/************************************************************************/
/* �֐�     : download_finish_ready                                     */
/* �֐���   : �v���O���������I����������(�`�F�b�N�T���v�Z)              */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2012.08.26 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �����I�����Ƀ`�F�b�N�T�����v�Z����                            */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
/* �g�p����v���O���� : �u�[�g                                          */
/************************************************************************/
void download_finish_ready(void)
{
	INT ret = E_OK;
	
	download_make_id(id_temp_buf);
	
}

/************************************************************************/
/* �֐�     : download_finish                                           */
/* �֐���   : �v���O���������I��������(�`�F�b�N�T����������)            */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : ��������                                                  */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �����I�����Ƀ`�F�b�N�T���E����ID���������݁A���i�v���O������  */
/*        ���݂��邱�Ƃ�����                                            */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
/* �g�p����v���O���� : �u�[�g                                          */
/************************************************************************/
INT download_finish(void)
{
	INT ret = E_OK;
	DRV_FROM_ERROR drv_err;
	
	drv_from_mode_enable();
	
	/* �`�F�b�N�T���E����ID�������� */
	drv_err = drv_from_write(&id_temp_buf[0], (UB *)ProductIdAddr, PRODUCT_ID_SIZE);

	if(drv_err == DRV_FROM_ERR_NONE){
		/* �x���t�@�C */
		if(download_verify(&id_temp_buf[0], (UB *)ProductIdAddr, PRODUCT_ID_SIZE) ){
			ret = E_OK;
		}else{
			ret = E_GENE;
		}
	}else{
		ret = E_GENE;
	}	
	
	drv_from_mode_disable();
	
	return ret;
}


/************************************************************************/
/* �֐�     : download_make_id                                          */
/* �֐���   : �`�F�b�N�T���E����ID�쐬����                              */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�                                                      */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �`�F�b�N�T�����v�Z���E����ID��ݒ肷��                        */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
STATIC void download_make_id(UB *p_id)
{
	UW sum = 0;
	UW * p_lwork = NULL;
	UW i;

	//ID�R�s�[
	for(i=0; i<PRODUCT_ID_SIZE; i++){
		p_id[i] = ProductIdCode[i];
	}

	//�`�F�b�N�T���v�Z
	sum = calc_sum_32((UB *)ProductPrgAddr, ProductPrgSize );
	
	//���ʃR�[�h��size,sum��4�o�C�g��������(�G���f�B�A���l��)
	p_lwork = (UW *)(&p_id[0]);
	*(p_lwork + PRODUCT_ID_OFFSET_SIZE_PTR) = ProductPrgSize;
	*(p_lwork + PRODUCT_ID_OFFSET_SUM_PTR) = sum;

}

/************************************************************************/
/* �֐�     : download_check_finish                                     */
/* �֐���   : �v���O���������`�F�b�N(�`�F�b�N�T���ƍ�)                  */
/* ����     : �]�����ꂽ�`�F�b�N�T��                                    */
/* �߂�l   : BOOL TRUE:�`�F�b�NOK,FALSE:�`�F�b�NNG                     */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �v���O�����̏��������ʂ��`�F�b�N����                          */
/*        �E�]�����ꂽ�v���O�������̂�����������                        */
/*        �E�������݂��������������܂ꂽ����                            */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
/* �g�p����v���O���� : �u�[�g                                          */
/************************************************************************/
BOOL download_check_finish(UW com_sum)
{
	UW cal_sum = 0;
	UW memory_sum = 0;

	memory_sum = *(UW *)(ProductIdAddr + PRODUCT_ID_OFFSET_SUM );
	
	/* �v���O������FROM�ɐ������������܂ꂽ���Ƃ��m�F */
	cal_sum = calc_sum_32((UB *)ProductPrgAddr, ProductPrgSize );
	if(cal_sum != memory_sum){
		return FALSE;
	}
	
	/* �]����Ńv���O�����Ɉُ킪�������Ă��Ȃ����Ƃ��m�F */
	if(cal_sum != com_sum){
		return FALSE;
	}
	return TRUE;
}

/************************************************************************/
/* �֐�     : download_check_checksum                                   */
/* �֐���   : �v���O���������`�F�b�N(�`�F�b�N�T���ƍ�)                  */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : BOOL TRUE:�`�F�b�NOK,FALSE:�`�F�b�NNG                     */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : FROM�̃`�F�b�N�T�����`�F�b�N����                              */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
/* �g�p����v���O���� : ���i�v���O����                                  */
/************************************************************************/
BOOL download_check_checksum(void)
{
	UW cal_sum = 0;
	UW memory_sum = 0;

	memory_sum = *(UW *)(ProductIdAddr + PRODUCT_ID_OFFSET_SUM );
	
	/* �v���O������FROM�ɐ������������܂ꂽ���Ƃ��m�F */
	cal_sum = calc_sum_32((UB *)ProductPrgAddr, ProductPrgSize );
	if(cal_sum != memory_sum){
		return FALSE;
	}
	
	return TRUE;
}

/************************************************************************/
/* �֐�     : download_check_product                                    */
/* �֐���   : ���i�v���O�����`�F�b�N(ID�ƍ�)                            */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : BOOL TRUE:���݂���,FALSE:���݂��Ȃ�                       */
/* �ύX���� : 2012.03.10 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : ���i�v���O���������݂��邱�Ƃ��m�F����                        */
/************************************************************************/
/* ���ӎ��� : �N�����Ԃ̒Z�k�̂��߁A�`�F�b�N�T���`�F�b�N�͍s�킸�A      */
/*            ����ID�����Ŕ�����s��                                    */
/************************************************************************/
/* �g�p����v���O���� : �u�[�g                                          */
/************************************************************************/
BOOL download_check_product(void)
{
	INT i;
	
	//���ʃR�[�h�S�̈�擾
	UB * p_id_data = (UB *)ProductIdAddr;
	UB id_buf[PRODUCT_ID_SIZE];
	
	for(i=0; i<PRODUCT_ID_SIZE; i++){
		id_buf[i] = *p_id_data++;
	}
	
	//����ID�`�F�b�N
	for(i=PRODUCT_ID_OFFSET_ID; i<PRODUCT_ID_SIZE; i++){
		if(id_buf[i] != ProductIdCode[i]){
			return FALSE;	/* ���i�v���O���������݂��Ȃ� */
		}
	}
	return TRUE;	/* ���i�v���O���������݂��� */
}

/************************************************************************/
/* �֐�     : download_get_data                                         */
/* �֐���   : �v���O�����ǂݏo��                                        */
/* ����     : p_get_data �擾�����f�[�^�̊i�[�A�h���X                   */
/*          : blocknum �ǂݏo�������ʐM��̃u���b�N�ԍ�                 */
/*          : size �ǂݍ��ރf�[�^�̃T�C�Y                               */
/* �߂�l   : ��������                                                  */
/* �ύX���� : 2012.03.10 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : ���g�̃v���O������ǂݏo��                                    */
/************************************************************************/
/* ���ӎ��� :                                                           */
/************************************************************************/
/* �g�p����v���O���� : ���i�v���O����                                  */
/************************************************************************/
INT download_get_data(UB *p_get_data,UW blocknum ,UW size)
{
	DRV_FROM_ERROR drv_err;
	INT ret = E_OK;
	UW read_addr = download_blocknum2addr(blocknum);
	
	/* �ǂݍ��� */
	drv_err = drv_from_read((UB *)read_addr, p_get_data, size);
	if(drv_err == DRV_FROM_ERR_NONE){
		ret = E_OK;
	}else{
		ret = E_GENE;
	}
	
	return ret;
}

/************************************************************************/
/* �֐�     : download_verify                                           */
/* �֐���   : �x���t�@�C����                                            */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : TRUE:��v / FALSE:�s��v                                  */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �����������Ƃ����f�[�^�Ə������񂾌��ʂ���v���邱��          */
/************************************************************************/
/* ���ӎ��� : �ő�T�C�Y��������                                        */
/************************************************************************/
STATIC BOOL download_verify(UB *addr_src , UB *addr_dest, UW size)
{
	DRV_FROM_ERROR drv_err;
	UW i;
	
	/* �T�C�Y�`�F�b�N */
	if(size > VERIFY_SIZE_MAX){
		return FALSE;
	}
	
	/* �ǂݍ��� */
	drv_err = drv_from_read(addr_dest, &verify_buf[0], size);
	if(drv_err != DRV_FROM_ERR_NONE){
		return FALSE;
	}
	
	/* �x���t�@�C */
	for(i=0; i<size; i++){
		if(*addr_src++ != verify_buf[i]){
			return FALSE;
		}
	}
	
	return TRUE;
}

/************************************************************************/
/* �֐�     : download_blocknum2addr                                    */
/* �֐���   : �u���b�N�ԍ���FROM�A�h���X�ϊ�����                        */
/* ����     : �u���b�N�ԍ�                                              */
/* �߂�l   : �ϊ���A�h���X                                            */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �ʐM��̃u���b�N�ԍ���FROM�̃A�h���X�ɕϊ�����                */
/************************************************************************/
/* ���ӎ��� : �����ł̃u���b�N�ԍ���FROM�u���b�N�}�̃u���b�N�ԍ��Ƃ͈Ⴄ*/
/************************************************************************/
STATIC UW download_blocknum2addr(UW blocknum)
{
	return (UW)(blocknum * COMM_BLOCK_SIZE);
}

/************************************************************************/
/* �֐�     : download_com_blocknum2from_blocknum                       */
/* �֐���   : �ʐM�u���b�N�ԍ���FROM�u���b�N�ԍ��ϊ�����                */
/* ����     : �ʐM�u���b�N�ԍ�                                          */
/* �߂�l   : FROM�u���b�N�ԍ�                                          */
/* �ύX���� : 2012.01.21 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �ʐM��̃u���b�N�ԍ���FROM�̃u���b�N�ԍ��ɕϊ�����            */
/************************************************************************/
/* ���ӎ��� :                                                           */
/************************************************************************/
STATIC UW download_com_blocknum2from_blocknum(UW blocknum)
{
	return (UW)((blocknum / ComBlockNumPerFromBlock));
}

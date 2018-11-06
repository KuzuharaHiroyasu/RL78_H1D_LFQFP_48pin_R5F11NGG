/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[												*/
/* �t�@�C����   : drv_from.c													*/
/* �@�\         : FROM�̓ǂݏ���												*/
/* �ύX����     : 2012.02.18 Axia Soft Design K.Wada	���ō쐬(RD1201)		*/
/*              : 2012.12.26 Axia Soft Design K.Wada    RD1201���痬�p�J�n      */
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

#include "./header.h"

/******************/
/* �萔�}�N����` */
/******************/
#define DUMMY_DATA      0xFF        /* �_�~�[�������݃f�[�^         */

/*------------------------------------------------------------------------------*/
/* external reference parameter                                                 */
/*------------------------------------------------------------------------------*/
/* �f�[�^�E�o�b�t�@�{�� */
fsl_u08 fsl_data_buffer[ WRITE_SIZE_BYTE ];

#define	FSL_DESCRIPTOR1_FULL_SPEED_MODE		0x00
#define	FSL_DESCRIPTOR1_WIDE_VOLTAGE_MODE	0x01

#define	FSL_DESCRIPTOR2_MAIN_CLOCK		12	//������g��[MHz]

#define	FSL_DESCRIPTOR3_STATES_CHECK_USER_MODE		0x00
#define	FSL_DESCRIPTOR3_STATES_CHECK_INTERNAL_MODE		0x01


/* ����N���b�N ����12MHz�ݒ� */
const __far fsl_descriptor_t fsl_descriptor_pstr = { 	FSL_DESCRIPTOR1_FULL_SPEED_MODE,
														FSL_DESCRIPTOR2_MAIN_CLOCK, 
														FSL_DESCRIPTOR3_STATES_CHECK_INTERNAL_MODE };


/********************/
/*     �����ϐ�     */
/********************/
//STATIC DRV_FROM_STATUS	s_drv_from_status;

/*********************/
/* �v���g�^�C�v�錾 */
/********************/
STATIC BOOL drv_from_is_true_area(UW addr,UW *p_get_block_no);
STATIC BOOL drv_from_is_true_block_no(INT no);
STATIC UW   drv_from_addr2blocknum(UW pFrom);

/************************************************************************/
/* �֐�     : drv_from_init                                             */
/* �֐���   : FROM�h���C�o����������                                    */
/* ����     : �Ȃ�                                                      */
/* �߂�l   :  															*/
/* �ύX���� : 2012.02.18 Axia Soft Design K.Wada	���ō쐬(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ :                                                               */
/************************************************************************/
/* ���ӎ��� :                                                           */
/************************************************************************/
void drv_from_init(void)
{
DRV_FROM_ERROR	ercd = DRV_FROM_ERR_NONE;

    fsl_u08 dubRetCode;

    /* �t���b�V���E�Z���t�E�v���O���~���O�E���C�u�������������s */
    dubRetCode = FSL_Init( &fsl_descriptor_pstr );
    
    /* ������������ɏI�������ꍇ */
    if( dubRetCode == FSL_OK )
    {
        // ���O���o�͂���
    }
}

/************************************************************************/
/* �֐�     : drv_from_mode_enable                                      */
/* �֐���   : CPU�����������[�h�J�n�ݒ菈��                             */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2012.12.21 Axia Soft Design K.Wada	���ō쐬(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �����������[�h�J�n�ݒ�                                        */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
void drv_from_mode_enable(void)
{
	DI();
	
    /* �t���b�V���E�Z���t�E�v���O���~���O�E���C�u�����J�n���� */
    FSL_Open();
    
    /* RAM���s���K�v�ȃt���b�V���֐�(�W�����������֐�)���g�p�ł���悤�ɏ��� */
    FSL_PrepareFunctions();
}

/************************************************************************/
/* �֐�     : drv_from_mode_disable                                     */
/* �֐���   : CPU�����������[�h�I���ݒ菈��                             */
/* ����     : �Ȃ�                                                      */
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2012.12.21 Axia Soft Design K.Wada	���ō쐬(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �����������[�h�I���ݒ�                                        */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
void drv_from_mode_disable(void)
{
    /* �t���b�V���E�Z���t�E�v���O���~���O�E���C�u�����I������ */
    FSL_Close();

	EI();
}

/************************************************************************/
/* �֐�     : drv_from_read                                             */
/* �֐���   : FROM�T�C�Y�w��ǂݍ��ݏ���                                */
/* ����     : *addr_src �\�[�X�A�h���X				                    */
/*			: *addr_dest	���Đ�A�h���X								*/
/*			: size	�f�[�^�T�C�Y										*/
/* �߂�l   : DRV_FROM_ERR_NONE 	����								*/
/*			: DRV_FROM_ERR_PARAM 	�p�����[�^�ُ�						*/
/* �ύX���� : 2012.02.18 Axia Soft Design K.Wada	���ō쐬(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : FROM�T�C�Y�w��ǂݍ���                                        */
/************************************************************************/
/* ���ӎ��� : �ǂݍ��݃f�[�^�̕ۑ��̈�͌Ăяo�����Ŋm�ۂ��邱��        */
/************************************************************************/
DRV_FROM_ERROR drv_from_read(UB  __far  *addr_src , UB  __far  *addr_dest, UW size)
{
	DRV_FROM_ERROR	ercd = DRV_FROM_ERR_NONE;
	UW block_num = DRV_FROM_BLOCK_NUM;
	
	/* �p�����[�^�`�F�b�N */
	if(size == 0){
		return DRV_FROM_ERR_PARAM;
	}
	
	if( (!drv_from_is_true_area((UW)     addr_src         , &block_num)) 
	  ||(!drv_from_is_true_area((UW)((UW)addr_src+size -1), &block_num)) ){
		return DRV_FROM_ERR_PARAM;
	}
	
	_COM_memcpy_ff( addr_dest, addr_src, (size_t)size);
	
	return ercd;
}

/************************************************************************/
/* �֐�     : drv_from_write                                            */
/* �֐���   : FROM�T�C�Y�w�菑�����ݏ���                                */
/* ����     : *addr_src �\�[�X�A�h���X				                    */
/*			: *addr_dest	���Đ�A�h���X								*/
/*			: size	�f�[�^�T�C�Y										*/
/* �߂�l   : DRV_FROM_ERR_NONE 	����							    */
/*			: DRV_FROM_ERR_PARAM 	�p�����[�^�ُ�						*/
/*			: DRV_FROM_ERR_PROGRAM	�v���O�����G���[					*/
/* �ύX���� : 2012.02.18 Axia Soft Design K.Wada	���ō쐬(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : FROM�T�C�Y�w�菑������                                        */
/************************************************************************/
/* ���ӎ��� : 256�o�C�g�܂ŏ������݉�									*/
/************************************************************************/
DRV_FROM_ERROR drv_from_write(UB __far *addr_src ,  const UB __far *addr_dest, UW size)
{
	DRV_FROM_ERROR	ercd = DRV_FROM_ERR_NONE;
	
    fsl_u08     dubRetCode = FSL_OK;/* ���^�[���E�R�[�h                   */
	
	UW write_addr = (UW)addr_dest;
	UW offset;
	UW write_unit_size;
	UW block_num_top = DRV_FROM_BLOCK_NUM;
	UW block_num_end = DRV_FROM_BLOCK_NUM;
	
	/* ���C�u�����������ݗp�f�[�^ */
	__near fsl_write_t dtyWriteBuff;
	
	/* ���[�h�A���C���ւ̃I�t�Z�b�g */
	offset = write_addr % WORD_SIZE;
	
	/* �p�����[�^�`�F�b�N */
	/* �I�t�Z�b�g�܂߂ď������݂��� */
	/* �܂�A�h���X�����[�h�A���C�������Ă���ꍇ�̂�Max256�o�C�g������ */
	if( (offset + size > WRITE_SIZE_BYTE) || (size == 0)){
		return DRV_FROM_ERR_PARAM;
	}
	if( (!drv_from_is_true_area((UW)     addr_dest         , &block_num_top)) 
	  ||(!drv_from_is_true_area((UW)((UW)addr_dest+size -1), &block_num_end)) ){
		return DRV_FROM_ERR_PARAM;
	}
	
	/* �f�[�^�E�o�b�t�@������ */
//    memset( fsl_data_buffer,DUMMY_DATA,WRITE_SIZE_BYTE);			// 0xFF����
	_COM_memcpy_ff( fsl_data_buffer, addr_dest-offset, WRITE_SIZE_BYTE);	// ���f�[�^����
    /* �������݃f�[�^���f�[�^�E�o�b�t�@�ɃR�s�[ */
	_COM_memcpy_ff( fsl_data_buffer + offset, addr_src, (size_t)size);
	
	/* �������݃T�C�Y����(���[�h�g��) */
	write_unit_size = (UW)((offset + size) / WORD_SIZE);	// �؂�̂�
	if((offset + size) % WORD_SIZE != 0){
		write_unit_size += 1;	// �؂�グ
	}
	
	/* �p�����[�^��ݒ� */
	dtyWriteBuff.fsl_data_buffer_p_u08       = (fsl_u08 __near *)fsl_data_buffer;
	dtyWriteBuff.fsl_destination_address_u32 = write_addr - offset;
	dtyWriteBuff.fsl_word_count_u08          = (fsl_u08)( write_unit_size );

	/* �������ݎ��s */
	wdt_refresh();
	dubRetCode = FSL_Write( &dtyWriteBuff );
	wdt_refresh();
	
	/* �x���t�@�C */
	if( dubRetCode == FSL_OK ){
		// (���ӎ���)
		// �x���t�@�C��1�u���b�N�̂ݍs���̂ŁC�����̃u���b�N���x���t�@�C����ꍇ�́C
		// FSL_IVerify�𕡐���Ăяo��
        dubRetCode = FSL_IVerify( (fsl_u16)block_num_top );
		if(dubRetCode == FSL_OK){
			if(block_num_top != block_num_end){
				dubRetCode = FSL_IVerify( (fsl_u16)block_num_end );
			}
		}
    }
	/* �G���[���� */
	if(dubRetCode != FSL_OK){
		ercd = DRV_FROM_ERR_PROGRAM;
	}
	
	return ercd;

}

/************************************************************************/
/* �֐�     : drv_from_erase_block                                      */
/* �֐���   : FROM�w��u���b�N��������                                  */
/* ����     : block �u���b�N�ԍ�				                        */
/* �߂�l   : DRV_FROM_ERR_NONE		����								*/
/*			: DRV_FROM_ERR_PARAM 	�p�����[�^�ُ�						*/
/*			: DRV_FROM_ERR_ERASE	�C���[�Y�G���[						*/
/* �ύX���� : 2012.02.18 Axia Soft Design K.Wada	���ō쐬(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : FROM�w��u���b�N����                                          */
/************************************************************************/
/* ���ӎ��� :                                                           */
/************************************************************************/
DRV_FROM_ERROR drv_from_erase_block(DRV_FROM_BLOCK_NO block)
{
	DRV_FROM_ERROR	ercd = DRV_FROM_ERR_NONE;
	fsl_u08 dubRetCode;         /* ���^�[���E�R�[�h                   */
	fsl_u08 block_no = (fsl_u08)block;
		
	/* �p�����[�^�`�F�b�N */
	if(! drv_from_is_true_block_no(block)){
		return DRV_FROM_ERR_PARAM;
	}
	
	/* �Ώۃu���b�N�̏�Ԃ��m�F */
	dubRetCode = FSL_BlankCheck( block_no );
	
	if( dubRetCode == FSL_OK ){
		/* �u�����N��� */
		ercd = DRV_FROM_ERR_NONE;
	}else if( dubRetCode == FSL_ERR_BLANKCHECK ){
		/* �u�����N�ł͂Ȃ���� */
		wdt_refresh();
	    dubRetCode = FSL_Erase( block_no );
		wdt_refresh();
		if( dubRetCode != FSL_OK ){
			ercd = DRV_FROM_ERR_ERASE;
		}
	}else{
		/* �����G���[ */
		ercd = DRV_FROM_ERR_ERASE;
	}

	return ercd;
}

/************************************************************************/
/* �֐�     : drv_from_is_true_area                                     */
/* �֐���   : �A�h���X�`�F�b�N����                                      */
/* ����     : addr	�����������A�h���X                                  */
/*          : p_get_block_no	�擾����u���b�N�ԍ�					*/
/* �߂�l   : TRUE	�u���b�N�͈͓� 										*/
/*			: FALSE	�u���b�N�͈͊O										*/
/* �ύX���� : 2012.02.18 Axia Soft Design K.Wada	���ō쐬(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �w�肵���A�h���X�������E�Ǎ��ł���FROM�̈悩�`�F�b�N����      */
/*      : �u���b�N0�Ԃ̓x�N�^�e�[�u���Ȃǂ����邽�߁A�G��Ȃ��悤�ɂ��� */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
STATIC BOOL drv_from_is_true_area(UW addr,UW *p_get_block_no)
{
	DRV_FROM_BLOCK_NO i = DRV_FROM_BLOCK_NUM;
	
	for(i=DRV_FROM_BLOCK_PROGRAM_TOP; i<DRV_FROM_BLOCK_PROGRAM_END+1; i++){
		if( (i * BLOCK_SIZE <= addr)
		  &&(addr <= ((i+1) * BLOCK_SIZE -1)) ){
		  	*p_get_block_no = i;
			return TRUE;
		}
	}
	return FALSE;
}

/************************************************************************/
/* �֐�     : drv_from_is_true_block_no                                 */
/* �֐���   : �u���b�N�ԍ��`�F�b�N����                                  */
/* ����     : no	�����������u���b�N�ԍ�                              */
/* �߂�l   : TRUE	�u���b�N�͈͓� 										*/
/*			: FALSE	�u���b�N�͈͊O										*/
/* �ύX���� : 2012.02.18 Axia Soft Design K.Wada	���ō쐬(RD1201)	*/
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : �w�肵���u���b�N�������E�Ǎ��ł���FROM�̈悩�`�F�b�N����      */
/*      : �u���b�N0�Ԃ̓x�N�^�e�[�u���Ȃǂ����邽�߁A�G��Ȃ��悤�ɂ��� */
/************************************************************************/
/* ���ӎ��� : �Ȃ�                                                      */
/************************************************************************/
STATIC BOOL drv_from_is_true_block_no(INT no)
{
	if( (DRV_FROM_BLOCK_PROGRAM_TOP <= no) && (no <= DRV_FROM_BLOCK_PROGRAM_END) ){
		return TRUE;
	}
	return FALSE;
}

/************************************************************************/
/* �֐�     : drv_from_addr2blocknum                                    */
/* �֐���   : FROM�A�h���X���u���b�N�ԍ��ϊ�����                        */
/* ����     : FROM�A�h���X                                              */
/* �߂�l   : �ϊ���u���b�N�ԍ�                                        */
/* �ύX���� : 2012.02.25 Axia Soft Design K.Wada	���ō쐬(RD1201)    */
/*          : 2012.12.26 Axia Soft Design K.Wada    ���ō쐬(RD1215)    */
/************************************************************************/
/* �@�\ : FROM�̃A�h���X��FROM�̃u���b�N�ԍ��ɕϊ�����                  */
/************************************************************************/
/* ���ӎ��� : �����ł̃u���b�N�ԍ���FROM�u���b�N�}�̃u���b�N�ԍ�        */
/************************************************************************/
STATIC UW drv_from_addr2blocknum(UW pFrom)
{
	UW block_num = DRV_FROM_BLOCK_NUM;
	
	drv_from_is_true_area((UW)pFrom, &block_num);
	
	return block_num;
}

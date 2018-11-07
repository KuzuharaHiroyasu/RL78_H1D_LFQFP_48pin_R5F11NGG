/************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[									*/
/* �t�@�C����   : cpu_com.c												*/
/* �@�\         : CPU�ԒʐM�~�h��										*/
/* �ύX����     : 2018.04.10 Axia Soft Design ����		���ō쐬		*/
/* ���ӎ���     : �Ȃ�													*/
/************************************************************************/
/********************/
/*     include      */
/********************/
#include	"header.h"

/********************/
/* �v���g�^�C�v�錾 */
/********************/



STATIC void cpu_com_crcset(UB *msg, UH size, UH *crc);
STATIC void cpu_com_rcv_proc(void);
STATIC UB cpu_com_analyze_msg(void);
STATIC UB cpu_com_analyze_msg_check_etx(void);
STATIC UB cpu_com_analyze_msg_check_data(void);
STATIC void cpu_com_send_proc(void);
STATIC UB cpu_com_make_send_data(void);
STATIC UH cpu_com_dle_extension( UB* data, UH size );



/********************/
/*     �����ϐ�     */
/********************/
// ���C��IF
STATIC DS_CPU_COM_INPUT s_cpu_com_ds_input;					/* �f�[�^�Ǘ����̃R�s�[�G���A �A�v���ւ̃f�[�^�󂯓n���p */
STATIC DS_CPU_COM_ORDER *s_p_cpu_com_ds_order;				/* �f�[�^�Ǘ����̃|�C���^ �A�v������̎w���p */


STATIC UB s_cpu_com_snd_cmd;								/* ���M�R�}���h */
STATIC UB s_cpu_com_snd_data[CPU_COM_MSG_SIZE_MAX];			/* ���M���b�Z�[�W */
STATIC UH s_cpu_com_snd_size;								/* ���M���b�Z�[�W�� */
STATIC UB s_cpu_com_snd_seq_no;								/* ���M�V�[�P���XNo */
STATIC UB s_cpu_com_res_seq_no;								/* ��M�V�[�P���XNo */

STATIC CPU_COM_RCV_MSG	s_cpu_com_rcv_msg;					/* ��M���b�Z�[�W */
STATIC UH s_cpu_com_rcv_msg_size;							/* ��M���b�Z�[�W�T�C�Y */




/********************/
/*   �e�[�u����`   */
/********************/
/* �R�}���h�e�[�u��[�X���[�u] */
STATIC const T_CPU_COM_CMD_INFO s_tbl_cmd_info[CPU_COM_CMD_MAX] = {
	/*�R�}���h*/ 
	{	0x00	},	/* �R�}���h����				*/
	{	0xE0	},	/* �X�e�[�^�X�v��			*/
	{	0xA0	},	/* �Z���T�[�f�[�^�X�V		*/
	{	0xB0	},	/* ��ԕύX(G1D)			*/
	{	0xF0	},	/* PC���O���M(�����R�}���h)	*/
	{	0xB1	},	/* �����ݒ�					*/
	{	0xD5	},	/* �v���O�����]������		*/
	{	0xD2	},	/* �v���O�����]���J�n		*/
	{	0xD4	},	/* �v���O�����]������		*/
	{	0xD0	},	/* �v���O�����]���f�[�^		*/
	{	0xD1	},	/* �v���O�����]������		*/
	{	0xD3	},	/* �v���O�����]���m�F		*/
	{	0xB2	},	/* �\���w��					*/
};


/************************************************************************/
/* �֐�     : cpu_com_init												*/
/* �֐���   : ����������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : ����������													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void cpu_com_init(void)
{
	memset(&s_cpu_com_ds_input, 0x00, sizeof(s_cpu_com_ds_input));

	s_cpu_com_snd_cmd = 0x00;
	memset(s_cpu_com_snd_data, 0x00, sizeof(s_cpu_com_snd_data));
	s_cpu_com_snd_size = 0;
	s_cpu_com_snd_seq_no = 0;
	s_cpu_com_res_seq_no = 0xFF;
	
	
	memset(s_cpu_com_rcv_msg.buf, 0x00, sizeof(s_cpu_com_rcv_msg.buf));
	s_cpu_com_rcv_msg_size = 0;
	
#if FUNC_DEBUG_CPU_COM == OFF
	drv_uart0_data_init();
#else
	drv_uart1_data_init();
#endif
}
/************************************************************************/
/* �֐�     : cpu_com_crcset											*/
/* �֐���   : CRC�Z�b�g													*/
/* ����     : UB *msg : �Ώۃf�[�^�A�h���X								*/
/*			: UH size : �Ώۃf�[�^��									*/
/*          : UH crc : CRC�v�Z����										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2012.11.30  Axia Soft Design �Y�v��   ���ō쐬			*/
/*          : 2014.05.13  Axia Soft Design �g��		CPU�ԒʐM�p�ɈڐA	*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� : ���Z�̑Ώۂ́ACMD�ASEQ�ADATA�܂łƂ���B					*/
/************************************************************************/
STATIC void cpu_com_crcset(UB *msg, UH size, UH *crc)
{
	/* CRC�v�Z */
	*crc = crc16(msg, (INT)size);

}

/************************************************************************/
/* �֐�     : cpu_com_proc												*/
/* �֐���   : CPU�ԒʐM��������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
void cpu_com_proc(void)
{
	/* �A�v������f�[�^�擾 */
	ds_get_cpu_com_order( &s_p_cpu_com_ds_order );
	
	/* ��M�f�[�^��� */
	cpu_com_rcv_proc();
	
	/* �������M���� */
	cpu_com_send_proc();
	
	/* �A�v���ւ̃f�[�^�ݒ� */
	ds_set_cpu_com_input( &s_cpu_com_ds_input );
	
	/* �f�[�^�ʒm��̓N���A */
	s_cpu_com_ds_input.rcv_cmd = 0x00;
	memset( s_cpu_com_ds_input.rcv_data, 0x00, sizeof(s_cpu_com_ds_input.rcv_data));
}


/************************************************************************/
/* �֐�     : cpu_com_send_end											*/
/* �֐���   : CPU�ԑ��M(�I���܂�)										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.08  Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : CPU�ԑ��M(�I���܂�)											*/
/************************************************************************/
/* ���ӎ��� :�h���C�o���x���̑��M�I���܂Ŗ{�֐��ōs��					*/
/************************************************************************/
void cpu_com_send_end( void )
{
	/* �A�v������f�[�^�擾 */
	ds_get_cpu_com_order( &s_p_cpu_com_ds_order );
	
	//�X���[�u
	if( CPU_COM_CMD_NONE != s_p_cpu_com_ds_order->snd_cmd_id ){
		cpu_com_make_send_data();
		s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_NONE;
	}
	
	while(1){
		if( OFF == drv_uart0_get_send() ){
			break;
		}
	}
}

/************************************************************************/
/* �֐�     : cpu_com_rcv_proc											*/
/* �֐���   : ������M����												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void cpu_com_rcv_proc(void)
{
	if( ON == cpu_com_analyze_msg() ){
		/* ��M�f�[�^���� */
		s_cpu_com_ds_input.rcv_cmd = s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_CMD];
		/* �A�v���ʒm�p�̃f�[�^�Ɏ�M�f�[�^���Z�b�g */
		memcpy( s_cpu_com_ds_input.rcv_data, &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_DATA], ( s_cpu_com_rcv_msg_size- CPU_COM_MSG_SIZE_MIN ));
	}
}


/************************************************************************/
/* �֐�     : cpu_com_analyze_msg										*/
/* �֐���   : ��M���b�Z�[�W���										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ON :1���b�Z�[�W��M										*/
/*          : OFF:����M												*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UB cpu_com_analyze_msg(void)
{
	//stx����etx�܂Ń`�F�b�N�����b�Z�[�W�o�b�t�@�Ɋi�[
	if( OFF == cpu_com_analyze_msg_check_etx() ){
		return OFF;
	}
	
	// �f�o�b�O
//	com_srv_puts( (const B *__near)"R ETX\r\n" );
	
	if( OFF ==  cpu_com_analyze_msg_check_data() ){
		return OFF;
	}
	// �f�o�b�O
//	com_srv_puts( (const B *__near)"R MSG\r\n" );
	
	return ON;
	
}

/************************************************************************/
/* �֐�     : cpu_com_analyze_msg_check_etx								*/
/* �֐���   : ��M���b�Z�[�W���(ETX)									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ON :1���b�Z�[�W��M										*/
/*          : OFF:����M												*/
/* �ύX���� : 2015.10.23 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��M���b�Z�[�W���(ETX)										*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UB cpu_com_analyze_msg_check_etx(void)
{
	UB ret = OFF;
	RING_BUF *p_ring_buf;
	UB rcv_data;
	
#if FUNC_DEBUG_CPU_COM == OFF
	p_ring_buf = drv_uart0_get_rcv_ring();
#else
	p_ring_buf = drv_uart1_get_rcv_ring();
#endif
	
	
	// 1�o�C�g���Ɏ�M��ETX�܂Ń`�F�b�N
	while(1){
		if( E_OK != read_ring_buf(p_ring_buf, &rcv_data ) ){
			break;
		}
		//�o�b�t�@�I�[�o�[�����΍�
		if( s_cpu_com_rcv_msg.pos >= CPU_COM_MSG_SIZE_MAX ){
			s_cpu_com_rcv_msg.pos = 0;
			s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_STX_WAIT;
		}
		
		// ����R�[�h
		if( rcv_data == CPU_COM_CTRL_CODE_DLE ){
			if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state )){
				//�f�[�^����DLE�d���Ɣ��f�� ���i�[���Ȃ�
				s_cpu_com_rcv_msg.last_dle_flg = OFF;
			}else{
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = rcv_data;
				s_cpu_com_rcv_msg.last_dle_flg = ON;
			}
		}else{
			//����R�[�h�ȊO
			if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_CTRL_CODE_STX == rcv_data )){
				// STX��M�Ɣ��f
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
//				com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"rcv stx\r\n");
#endif
				s_cpu_com_rcv_msg.pos = 0;
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_DLE;
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_STX;
				s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_ETX_WAIT;
			}else if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_CTRL_CODE_ETX == rcv_data ) && ( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state )){
				// ETX�܂Ŏ擾
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
//				com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"rcv etx\r\n");
#endif
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_ETX;
				s_cpu_com_rcv_msg_size = s_cpu_com_rcv_msg.pos;
				s_cpu_com_rcv_msg.pos = 0;
				s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_STX_WAIT;
				ret = ON;
				break;
			}else{
				if( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state ){
					s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = rcv_data;
				}else{
					s_cpu_com_rcv_msg.pos = 0;
				}
			}
			s_cpu_com_rcv_msg.last_dle_flg = OFF;
		}
	}
	return ret;
}

/************************************************************************/
/* �֐�     : cpu_com_analyze_msg_check_data							*/
/* �֐���   : ��M���b�Z�[�W���(CRC,SUM�`�F�b�N)						*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ON :1���b�Z�[�W��M										*/
/*          : OFF:����M												*/
/* �ύX���� : 2015.10.23 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��M���b�Z�[�W���(CRC,SUM�`�F�b�N)							*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UB cpu_com_analyze_msg_check_data(void)
{
//	UH sum_rcv = 0;	/* ��M�f�[�^SUM�l */
	UH crc_rcv = 0; /* ��M�f�[�^CRC */
	UH tmp = 0;
	UH data_size = 0;
	RING_BUF* p_ring;
	UB seq_num;
	int i;
	
	if(( CPU_COM_MSG_SIZE_MIN > s_cpu_com_rcv_msg_size ) ||
		( CPU_COM_MSG_SIZE_MAX < s_cpu_com_rcv_msg_size )){
		/* ���b�Z�[�W�T�C�Y�ُ� */
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
		com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"CPU_COM DATA_SIZE_NG\r\n");
#endif
		return OFF;
	}
	/* STX,ETX,SUM�ACRC���������f�[�^�� */
	data_size = s_cpu_com_rcv_msg_size - ( CPU_COM_STX_SIZE + CPU_COM_CRC_SIZE  + CPU_COM_ETX_SIZE );
	
	/*  CRC�`�F�b�N */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size ];			/* CRC���ʃr�b�g */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size + 1 ] << 8;	/* CRC��ʃr�b�g */
	cpu_com_crcset( &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_CMD], data_size, &tmp );

	if( crc_rcv != tmp ){
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
		com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"CPU_COM CRC_NG\r\n");
#endif
		return OFF;
	}

	/* �V�[�P���X�ԍ��`�F�b�N */
	seq_num = s_cpu_com_rcv_msg.buf[ CPU_COM_MSG_TOP_POS_SEQ ];				/* �V�[�P���X�ԍ����ʃr�b�g */

	if( s_cpu_com_res_seq_no == seq_num ){
		//�������đ�
		// ���M�����O�o�b�t�@��������
		p_ring = drv_uart0_get_snd_ring();
		for( i = 0;i < s_cpu_com_snd_size ; i++ ){
			if( E_OK != write_ring_buf( p_ring, s_cpu_com_snd_data[i] )){	/* �����O�o�b�t�@�������� */
				break;
			}
		}
#if FUNC_DEBUG_CPU_COM == OFF
		drv_uart0_send_start();
#else
		drv_uart1_send_start();
#endif
		return OFF;
	}
	s_cpu_com_res_seq_no = seq_num;		//�V�[�P���X�ԍ��X�V
	
	/* �`�F�b�NOK */
	return ON;
}

/************************************************************************/
/* �֐�     : cpu_com_send_proc											*/
/* �֐���   : �������M����												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/*          : 2016.03.10  Axia Soft Design ����		�V�[�P���X�`�F�b�N�폜(RD1508�Ή�) */
/************************************************************************/
/* �@�\ : �������M����													*/
/* �������M�������s��													*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void cpu_com_send_proc(void)
{
	//�X���[�u
	if( CPU_COM_CMD_NONE != s_p_cpu_com_ds_order->snd_cmd_id ){
		cpu_com_make_send_data();
		s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_NONE;
	}
}


/************************************************************************/
/* �֐�     : cpu_com_make_send_data									*/
/* �֐���   : ���M�f�[�^����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : TRUE:���� FALSE:�ُ�										*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : �A�v������̎w���ɏ]�����M�f�[�^�𐶐�����					*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UB cpu_com_make_send_data(void)
{
	CPU_COM_CMD_ID cmd_id;
	UH size;
	UH crc;
	UH pos = 0;
	RING_BUF* p_ring;
	INT i = 0;
	
	/* �R�}���h�A�f�[�^�A�f�[�^���̃N���A */
	s_cpu_com_snd_cmd = 0x00;
	memset( s_cpu_com_snd_data, 0x00, sizeof(s_cpu_com_snd_data) );
	s_cpu_com_snd_size = 0;
	
	/* ���g���C�񐔁A�^�C���A�E�g���ԁA�A���񐔃N���A */
	if(( CPU_COM_CMD_NONE == s_p_cpu_com_ds_order->snd_cmd_id ) || ( CPU_COM_CMD_MAX <= s_p_cpu_com_ds_order->snd_cmd_id )){
		/* �R�}���hID�ُ� */
		return FALSE;
	}
	if( CPU_COM_DATA_SIZE_MAX <= s_p_cpu_com_ds_order->data_size ){
		/* �f�[�^���ُ� */
		return FALSE;
	}
	
	/* �R�}���hID�A�f�[�^���擾 */
	cmd_id = s_p_cpu_com_ds_order->snd_cmd_id;
	size = s_p_cpu_com_ds_order->data_size;
	
	
	/* �R�}���h�A�f�[�^���ێ� */
	s_cpu_com_snd_cmd = s_tbl_cmd_info[cmd_id].cmd;
	s_cpu_com_snd_size = size + CPU_COM_MSG_SIZE_MIN;
	
	/* STX */
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_DLE;
	pos++;
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_STX;
	pos++;
	
	/* �R�}���h�̃Z�b�g */
	s_cpu_com_snd_data[pos] = s_cpu_com_snd_cmd;
	pos++;
	
	/* SEQ�̃Z�b�g */
	s_cpu_com_snd_data[pos] = s_cpu_com_snd_seq_no;
	pos += CPU_COM_SEQ_SIZE;

	/* �f�[�^�̃Z�b�g */
	memcpy( &s_cpu_com_snd_data[pos], s_p_cpu_com_ds_order->snd_data, size );
	pos += size;
	
	/* CRC�̃Z�b�g */
	cpu_com_crcset( &s_cpu_com_snd_data[CPU_COM_MSG_TOP_POS_CMD], size + CPU_COM_CMD_SIZE + CPU_COM_SEQ_SIZE , &crc );
	s_cpu_com_snd_data[pos] = (UB)( crc & 0x00FF );
	s_cpu_com_snd_data[pos+1] = (UB)( crc >> 8 );
	pos += CPU_COM_CRC_SIZE;
	
	/* ETX */
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_DLE;
	pos++;
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_ETX;
	pos++;

	/* DLE �g�� */
	s_cpu_com_snd_size = cpu_com_dle_extension( &s_cpu_com_snd_data[0], s_cpu_com_snd_size );
	
	/* �V�[�P���XNo���Z */
	s_cpu_com_snd_seq_no++;
	
	// ���M�����O�o�b�t�@��������
	p_ring = drv_uart0_get_snd_ring();
	for( i = 0;i < s_cpu_com_snd_size ; i++ ){
		if( E_OK != write_ring_buf( p_ring, s_cpu_com_snd_data[i] )){	/* �����O�o�b�t�@�������� */
			break;
		}
	}
#if FUNC_DEBUG_CPU_COM == OFF
	drv_uart0_send_start();
#else
	drv_uart1_send_start();
#endif
	
	return TRUE;
}


/************************************************************************/
/* �֐�     : cpu_com_dle_extension										*/
/* �֐���   : DLE�L��													*/
/* ����     : *data	DLE�L������f�[�^									*/
/*          : size	DLE�L������T�C�Y									*/
/* �߂�l   : DLE�L����̃T�C�Y											*/
/* �ύX���� : 2015.10.23 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : DLE�L��														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UH cpu_com_dle_extension( UB* data, UH size )
{
	UB cpu_com_buf_org[CPU_COM_DATA_SIZE_MAX];
	int i = 0;
	UH extension_size = 0;
	
	memcpy( &cpu_com_buf_org[0], data, size );
	
	// STX�Ή� �����g��
	*data++ = cpu_com_buf_org[0];
	*data++ = cpu_com_buf_org[1];
	
	// DLE�g��
	for( i = CPU_COM_STX_SIZE; i < ( size - CPU_COM_ETX_SIZE ); i++ ){
		*data++ = cpu_com_buf_org[i];
		if( CPU_COM_CTRL_CODE_DLE == cpu_com_buf_org[i] ){
			*data++ = CPU_COM_CTRL_CODE_DLE;
			extension_size++;
		}
	}
	// ETX�Ή� �����g��
	*data++ = cpu_com_buf_org[i++];
	*data   = cpu_com_buf_org[i];

	return ( size + extension_size );
}


// ==================================================
// �f�o�b�O�p�����F�e�X�g�R�[�h����������������������
// ==================================================
void test_cpu_com_send( void )
{
#if 0
	s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_STATUS_CHG_REQ;		/* ���M�R�}���hID */
	s_p_cpu_com_ds_order->snd_data[0] = 0xAA;								/* ���M�f�[�^ */
	s_p_cpu_com_ds_order->data_size = 1;										/* ���M�f�[�^�� */
	
	// �f�[�^�쐬
	cpu_com_make_send_data();
#endif
}
/************************************************************************/
/* END OF TEXT															*/
/************************************************************************/
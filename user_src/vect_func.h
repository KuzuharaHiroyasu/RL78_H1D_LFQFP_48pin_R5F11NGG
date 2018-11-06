/********************************************************************************/
/* �V�X�e����   : RD1215 ���o��BOX												*/
/* �t�@�C����   : vect_func.h													*/
/* �@�\         : �x�N�^�e�[�u���֐������Q�̌��J��`����уC���^�t�F�[�X		*/
/* �ύX����     : 2013.01.23 Axia Soft Design K.Wada	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef		_VECT_FUNC_H_			/* ��d��`�h�~ */
#define		_VECT_FUNC_H_


/******************/
/* �萔�}�N����` */
/******************/

/******************/
/*     �^��`     */
/******************/
/* �֐��x�N�^�e�[�u���p�̍\���̒�` */
typedef struct st_vect_func {
	void (*func)(void);
}ST_VECT_FUNC;


/******************/
/*    ���J�萔    */
/******************/
/* �x�N�^�ԍ���` */
enum VectNum {
	VECT_NUM_RESET = 0 // 0
, VECT_NUM_INTWDTI            		//        0x0004
, VECT_NUM_INTLVI                  //        0x0006
, VECT_NUM_INTP0                   //        0x0008
, VECT_NUM_INTP1                   //        0x000A
, VECT_NUM_INTP2                   //        0x000C
, VECT_NUM_INTP3                   //        0x000E
, VECT_NUM_INTP4                   //        0x0010
, VECT_NUM_INTP5                   //        0x0012
, VECT_NUM_INTCSI20                //        0x0014
, VECT_NUM_INTIIC20                //        0x0014
, VECT_NUM_INTST2                  //        0x0014
, VECT_NUM_INTSR2                  //        0x0016
, VECT_NUM_INTSRE2                 //        0x0018
, VECT_NUM_INTCSI00                //        0x001E
, VECT_NUM_INTIIC00                //        0x001E
, VECT_NUM_INTST0                  //        0x001E
, VECT_NUM_INTTM00                 //        0x0020
, VECT_NUM_INTSR0                  //        0x0022
, VECT_NUM_INTSRE0                 //        0x0024
, VECT_NUM_INTTM01H                //        0x0024
, VECT_NUM_INTCSI10                //        0x0026
, VECT_NUM_INTIIC10                //        0x0026
, VECT_NUM_INTST1                  //        0x0026
, VECT_NUM_INTSR1                  //        0x0028
, VECT_NUM_INTSRE1                 //        0x002A
, VECT_NUM_INTTM03H                //        0x002A
, VECT_NUM_INTIICA0                //        0x002C
, VECT_NUM_INTRTIT                 //        0x002E
, VECT_NUM_INTTM01                 //        0x0032
, VECT_NUM_INTTM02                 //        0x0034
, VECT_NUM_INTTM03                 //        0x0036
, VECT_NUM_INTAD                   //        0x0038
, VECT_NUM_INTRTC                  //        0x003A
, VECT_NUM_INTIT                   //        0x003C
, VECT_NUM_INTTM04                 //        0x0042
, VECT_NUM_INTTM05                 //        0x0044
, VECT_NUM_INTP6                   //        0x0046
, VECT_NUM_INTTM06                 //        0x004C
, VECT_NUM_INTTM07                 //        0x004E
, VECT_NUM_INTIT00                 //        0x0050
, VECT_NUM_INTIT01                 //        0x0052
, VECT_NUM_INTDSAD                 //        0x005C
, VECT_NUM_INTDSADS                //        0x0060
, VECT_NUM_INTFL                   //        0x0062

, VECT_NUM_COUNT
};

/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void vect_set_tbl_apl( void );

/******************/
/* �֐��}�N����` */
/******************/

#endif /*_VECT_FUNC_H_ */

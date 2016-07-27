/***************************Copyright BestFu 2014-05-14*************************
��	����	DeviceIDSetAttr.h
˵	����	�����豸��ַ��ͷ�ļ�
��	�룺	uVision V5.12.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-11-22
��  ��:     ����
*******************************************************************************/
#ifndef __DEVICE_ID_SET_H 
#define __DEVICE_ID_SET_H

/**********************************�û��Զ����********************************/
//�����ַ����
#define RE_DEVICE_ID_FLAG						((BF_INT32U)(0xA5C33C5A))			//��������ַ���
//�ж������ַ����Ч��
#define DEVICE_ID_IS_OK(flag,addr,anti_addr)	(((RE_DEVICE_ID_FLAG == (flag))&&\
												(0xffffffff != (addr))&&\
												(0x00000000 != (addr))&&\
												(0x44444444 != (addr))&&\
												(0x88888888 != (addr))&&\
												(0x54534542 != (addr))&&\
												((addr) == ~(anti_addr))))
//FLAH���ܲ���
#define ENCRYPTION_FLAG							((BF_INT32U)(0x5ABD42A5))			//���FLASH���ܱ��

/**********************************���ݽṹ����********************************/
typedef struct
{
	BF_INT32U SetAddrFlag;			//������Ч���õ�ַ���
	BF_INT32U DeviceID;				//������豸��ַ
	BF_INT32U DeviceIDInver;		//�����ַ�ķ���
}DeviceIDSet_t;

typedef struct
{
	BF_INT32U EncryptFlashFlag;		//FLASH���ܱ��
	BF_INT32U EncryptFlashData;		//Flash������Կ
}FlashEncrypt_t;

#endif //DeviceIDSetAttr.h
/**************************Copyright BestFu 2014-05-14*************************/

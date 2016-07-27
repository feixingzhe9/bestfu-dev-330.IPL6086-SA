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
#include <UnitCfg.h>

/**********************************�û��Զ����********************************/
//�����ַ����
#define RE_DEVICE_ID_FLAG						((u32)(0xA5C33C5A))			//��������ַ���
//�ж������ַ����Ч��
#define DEVICE_ID_IS_OK(flag,addr,anti_addr)	(((RE_DEVICE_ID_FLAG == (flag))&&\
												(0xffffffff != (addr))&&\
												(0x00000000 != (addr))&&\
												(0x44444444 != (addr))&&\
												(0x88888888 != (addr))&&\
												(0x54534542 != (addr))&&\
												((addr) == ~(anti_addr))))
//FLAH���ܲ���
#define ENCRYPTION_FLAG							((u32)(0x5ABD42A5))			//���FLASH���ܱ��

/**********************************���ݽṹ����********************************/
typedef struct
{
	u32 SetAddrFlag;			//������Ч���õ�ַ���
	u32 DeviceID;				//������豸��ַ
	u32 DeviceIDInver;			//�����ַ�ķ���
}DeviceIDSet_t;

typedef struct
{
	u32 EncryptFlashFlag;		//FLASH���ܱ��
	u32 EncryptFlashData;		//Flash������Կ
}FlashEncrypt_t;

/**********************************��������************************************/
MsgResult_t Get_ResetAddr_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_ResetAddr_Attr(UnitPara_t *pData);

#endif //DeviceIDSetAttr.h
/**************************Copyright BestFu 2014-05-14*************************/

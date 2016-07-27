/***************************Copyright BestFu 2014-05-14*************************
��	����	UserDataDownload.h
˵	����	�û���������ʵ��ԭ��ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.08.28 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __USERDATADOWNLOAD_H
#define __USERDATADOWNLOAD_H

#include "UnitCfg.h"

#define USER_FLASH					(0x5A)			//�û�Flash��
#define USER_EEPROM					(0xA5)			//�û�EEPRM��
#define USER_EEPROM_START_ADDR		(0x3000)		//�û�EEPROM��ַ��ʼ��ַ
#define USER_EEPROM_END_ADDR		(0x8000)		//�û�EEPROM��ַ������ַ


typedef u8 (*Save_fun)(u32 addr, u32 len, u8 *data);

typedef struct
{
	u16 frameSize;		//ÿ֡���ݴ�С
	u16 frameCount;		//��֡��
	u32 addr;			//���ݴ洢��ַ
	Save_fun save;		//�洢����
	u32 frameFlag[32];	//֡���ձ��
}UserDataDownload_t;	//�û��������������ṹ��

void UserDataDownload_Init(void);
MsgResult_t UserDataDownload_Ready(UnitPara_t *pData);
MsgResult_t UserDataFrame_Save(UnitPara_t *pData);
MsgResult_t UserDataFrame_Check(UnitPara_t *pData, u8 *rlen, u8 *rpara);

#endif //UserDataDownload.h end
/**************************Copyright BestFu 2014-05-14*************************/

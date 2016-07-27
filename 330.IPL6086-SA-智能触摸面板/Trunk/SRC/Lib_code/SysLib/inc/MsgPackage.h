/***************************Copyright BestFu 2014-05-14*************************
��	����	MsgPackage.h
˵	����	�豸������Ϣ������غ���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.24 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __MSGPACKAGE_H
#define __MSGPACKAGE_H

#include "BF_type.h"
#include "command.h"
#include "UnitCfg.h"

#define EVENT						(0x00)
#define COMMUNICATION_VERSION		(0x01)			//ͨ�Ű汾
#define DECRYPT_COMMUNICATION   	(0x02)      //����ͨ�Ű汾
#define MSG_UPLOAD_ID				(0x88888888)	//��Ϣ�ϱ���ַ

void Msg_Feedback(Communication_t *pMsg);
void Msg_Upload(u8 unit, u8 cmd, u8 len, u8 *data);
void Msg_Send(ObjectType_t objecttype, u32 objectID, u16 actNum, u8 unit, u8 cmd, u8 len, u8 *data);
void Msg_UploadUnit(u8 unit, u8 cmd, u8 len, u8 *data);
void SaveSerialNum(void);

#endif //MsgPackage.h end 
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2014-05-14*************************
��	����	Upload.h
˵	����	�豸�����ϱ���غ���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.07.03 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __UPLOAD_H
#define __UPLOAD_H

#include "BF_type.h"

#define MAX_UPLOAD_UNIT  (24)  //����ϱ���Ԫ����  yanhuan adding 2016/01/08

typedef struct
{
	u32 cnt ;
	u32 time ;
	u8	flag ;	
} UploadCondition_t;

typedef enum
{
	RESTART			= 0,	//�ϵ�/����
	USER_DATA_RESET	= 1,	//�û���������/�ָ���������
	EEPROM_FAIL 	= 2,	//�û����ݴ洢�����쳣	
	WIFI_433		= 3, 	//433����ͨ��ʧ��
}Upload_t;

//��������
void Upload_Fault(Upload_t upload);
void Upload_Immediately(u8 unit,u8 cmd,u8* data);
void SysTickUpload(void);
#endif //Upload.h end
/**************************Copyright BestFu 2014-05-14*************************/

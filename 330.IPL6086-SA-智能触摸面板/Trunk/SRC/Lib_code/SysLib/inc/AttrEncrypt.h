/***************************Copyright BestFu 2014-05-14*************************
��	����	AttrEncrypt.h
˵	����	���Լ���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2015-01-26
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __ATTRENCRYPT_H
#define __ATTRENCRYPT_H

#include "BF_type.h"
#include "Command.h"

typedef struct 
{
	u32 srcAddr;	//Դ��ַ
	u8  time;		//�ȴ�ʱ��
	u8  cnt;
	u16 crc;		//
	u16 prv;		//˽����Կ
	u16 prd;		//������Կ
}Encrypt_t;

u8 AttrEncrypt_Check(Communication_t * cmd);	//���Լ��ܺ˶�
void Encrypt_FlagSet(void);

#endif  //Updata.h  end
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2014-05-14*************************
��	����    CRC.H
˵	��:	    CRCУ�鷽��Դ����ͷ�ļ�
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Joey
��	�ڣ�    2013.7.30
��  ��:     ����
*******************************************************************************/
#ifndef _CRC_H
#define _CRC_H

#include "BF_type.h"

u16 ChkCrcValue(u8 *buf, u16 len);
//u16 ChkCrcValueEx(u8 *buf, u16 cksum, u16 len);
u16 ChkCrcValueEx(u8 *buf, u16 len, u16 FirstData);
#endif
/**************************Copyright BestFu 2014-05-14*************************/

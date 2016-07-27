/***************************Copyright BestFu 2014-05-14*************************
��	����	Eeprom.h
˵	����	��Eeprom����,����ȫ�ֲ�����EEPROMӳ���ϵ��������дEEPROM���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Joey
��	�ڣ�	2013.7.23   
��  ��:     ����
*******************************************************************************/
#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "BF_type.h"

#define  A24C256               (256*1024/8)                 // 24C256��С
#define  EEPROMSIZE            (A24C256-EEPROMSTARTADDR)    // EEPROM�ߴ�
#define  PAGESIZE              (64)                         // ҳ�ߴ�
#define  I2CBUFFLEN            (PAGESIZE)                   // д��������С
#define  EEPROMSTARTADDR       (100)                        // EEPROM�����׵�ַ
#define  EEPROMENDADDR         (A24C256)                    // EEPROM����β��ַ
#define  EEPROMSLAVEADDR       (0xA0)                       // EEPROM��������ַ

#define  EEPROMINITED          0xA5

u8 ReadDataFromEEPROM(u32 addr, u32 len, u8 *data);
u8 WriteDataToEEPROM(u32 addr, u32 len, u8 *data);
u8 WriteDataToEEPROMEx(u32 addr, u32 len, u8 data);

#endif
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2014-05-14*************************
��	����    Program.h
˵	����    STM32_Flash����
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2013-11-21
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __PROGRAM_H
#define __PROGRAM_H
/**********************************�Զ����************************************/

/**********************************��������************************************/
BF_BOOLEAN Program_Erase(BF_INT32U addr, BF_INT32U size);
BF_BOOLEAN Program_Write(BF_INT32U addr, BF_INT32U len, BF_INT08U *data);
BF_BOOLEAN Program_Read(BF_INT32U addr, BF_INT32U len, BF_INT08U *data) ;
#if (FLASH_ENCRYPTION_EN > 0u)
void FlashEncryptionInit(void);
#endif
#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
void Get_UniqueDeviceID(BF_INT08U* chipidtable);
#endif
BF_INT32U Flash_DefaultValue(void);
BF_INT32U Flash_PageSize(void);
BF_INT32U Flash_ChipSize(void);

#endif //program.h
/**************************Copyright BestFu 2014-05-14*************************/

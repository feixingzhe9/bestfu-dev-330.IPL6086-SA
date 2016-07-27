/*******************************************************************
文	件：    Flash
说	明：    STM32_Flash操作头文件
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-11-20
********************************************************************/

#ifndef __FLASH_H
#define __FLASH_H

#include "stm32l1xx.h"
#include "BF_type.h"

/*********************************配置功能*************************************/
#define FLASH_ENCRYPTION_EN				(1u)		//使能flash加密功能
#define DEVICE_ADDR_RESET_FEATURE_EN	(1u)		//使能地址重设功能

/****************************************************************************************/
#define DEVICE_ID			 	(*(vu32*)(0x1ff80064))				//设备ID取自IC全球唯一ID   L15X
#define CHIPID_ADDR			 	(0x1FF80050)    					//STM32L15xCX全球96位唯一芯片ID存储首地址
#define USER_FLASH_AREA		 	(0X1FF8004C)						//L15X芯片大小

#if ((FLASH_ENCRYPTION_EN > 0u)||(DEVICE_ADDR_RESET_FEATURE_EN > 0u))
#define RE_DEVICE_START_ADDR	(0x08000430)						//设备重设地址的存储地址	
#endif
								
#if (FLASH_ENCRYPTION_EN > 0u)
/*****************************Flash加密相关函数********************************/
#define CRC_BASE_DR				((u32)(0x40023000))					//CRC_DR寄存器的首地址
#define CRC_BASE_CR				((u32)(0x40023008))					//CRC_CR寄存器的首地址
#define ENCRYPTION_START_ADDR	((u32)(0x0800043C))					//Flash加密存储的地址

__inline u32 FlashEncryptionFunc(u32 NewAddr) //计算加密值使用
{
	*((vu32*)(CRC_BASE_CR))|= (u32)(0x01);
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x00));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x04));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x14));
	*((vu32*)(CRC_BASE_DR)) = NewAddr;
	return (*((vu32*)(CRC_BASE_DR)));
}
__inline u32 DecryptionFlash(void)  //使用加密值在函数中混淆使用
{
	*((vu32*)(CRC_BASE_CR))|=  (u32)(0x01);
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x00));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x04));
	*((vu32*)(CRC_BASE_DR)) = *((vu32*)(CHIPID_ADDR + 0x14));
	*((vu32*)(CRC_BASE_DR)) = *(vu32*)(RE_DEVICE_START_ADDR + 4);
	*((vu32*)(CRC_BASE_DR)) = *(vu32*)(ENCRYPTION_START_ADDR + 4);
	return (*((vu32*)(CRC_BASE_DR)));
}

#endif

/*********************************数据类型定义********************************/  
typedef enum 
{
	FLASH_OK       = 0x00,
	FLASH_ERROR    = 0x01,
	FLASH_BUSY     = 0x02,
	FLASH_TIMEOUT  = 0x03
} FlashStatus_t;

/*******************************函数声明**************************************/
void Flash_Unlock(void);
void Flash_lock(void);
void DATA_EEPROM_Unlock(void);
u8 FLASH_ErasePage(u32 page_add);
u8 Flash_Write(u32 add, s32 len, u8 *data);
//u8 Flash_WriteProtect(u32 addr, u8 sta);
//u8 Flash_EraseOptionByte(void);
//u8 Flash_WriteOptionByte(u32 addr, u8 data);

#endif /*FLASH_H */

/******************* (C) COPYRIGHT 2013 BestFu *****END OF FILE****/


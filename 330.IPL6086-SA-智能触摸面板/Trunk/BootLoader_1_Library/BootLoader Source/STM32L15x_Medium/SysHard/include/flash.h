/***************************Copyright BestFu 2014-05-14*************************
文	件：	Flash.h
说	明：	STM32F103_Flash操作头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2013-11-21
修  改:     暂无
*******************************************************************************/
#ifndef __FLASH_H
#define __FLASH_H

/*******************************用户自定义宏***********************************/
#define DEVICE_ID			 (*(__IO uint32_t*)(0x1ff80064))	//设备ID取自IC全球唯一ID   STM32L15x
#define CHIPID_ADDR			 (0x1FF80050)    					//STM32L15xCX全球96位唯一芯片ID存储首地址

/*
For DEV_ID = 0x416 or 0x427 or 0x429 or 0x437, this field value indicates the Flash memory
size of the device in Kbytes.
Example: 0x0080 = 128 Kbytes.
For DEV_ID = 0x436, the field value can be '0' or '1' with '0' for 384 Kbytes and '1' for 256
Kbytes.
Note: For DEV_ID = 0x429, only LSB part of F_SIZE: F_SIZE[7:0] is valid. The MSB part
F_SIZE[15:8] is reserved and must be ignored.
*/
#define USER_FLASH_AREA		 (0x1FF8004C)						//芯片的大小

#if ((DEVICE_ADDR_RESET_FEATURE_EN > 0u)||(FLASH_ENCRYPTION_EN > 0u))
#define RESET_DEV_ID_FLAG_ADDR	((uint32_t)(0x08000430))		//记录设备重设地址标记的信息地址

#endif
								
#if (FLASH_ENCRYPTION_EN > 0u)
/*****************************Flash加密相关函数********************************/
#define CRC_BASE_DR				((uint32_t)(0x40023000))							//CRC_DR寄存器的首地址
#define CRC_BASE_CR				((uint32_t)(0x40023008))							//CRC_CR寄存器的首地址
#define ENCRYPTION_FLAG_ADDR	((uint32_t)(0x0800043c))							//记录FLASH加密的标识信息地址

__inline uint32_t FlashEncryptionFunc(uint32_t NewAddr)	//计算加密值使用
{
	*((__IO uint32_t*)(CRC_BASE_CR))|= (uint32_t)(0x01);
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x00));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x04));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x14));
	*((__IO uint32_t*)(CRC_BASE_DR)) = NewAddr;
	return (*((__IO uint32_t*)(CRC_BASE_DR)));
}
__inline uint32_t DecryptionFlash(void)				//使用加密值在函数中混淆使用
{
	*((__IO uint32_t*)(CRC_BASE_CR))|=  (uint32_t)(0x01);
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x00));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x04));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(CHIPID_ADDR + 0x14));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(RESET_DEV_ID_FLAG_ADDR + 4));
	*((__IO uint32_t*)(CRC_BASE_DR)) = *((__IO uint32_t*)(ENCRYPTION_FLAG_ADDR + 4));
	return (*((__IO uint32_t*)(CRC_BASE_DR)));
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
uint8_t FLASH_ErasePage(uint32_t page_add);
uint8_t Flash_Write(uint32_t add, int32_t len, uint8_t *data);

#endif /*FLASH_H */

/**************************Copyright BestFu 2014-05-14*************************/

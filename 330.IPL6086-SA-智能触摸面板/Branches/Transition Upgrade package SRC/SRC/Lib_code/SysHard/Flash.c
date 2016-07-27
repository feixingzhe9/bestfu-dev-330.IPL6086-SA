/*******************************************************************
文	件：    Flash.c
说	明：    STM32_Flash操作函数
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-11-21
********************************************************************/
#include "stm32l1xx.h"
#include "Flash.h"
#include "BF_type.h"

/* FLASH Keys */
#define RDP_Key                  ((u16)0x00A5)
#define FLASH_PDKEY1               ((uint32_t)0x04152637) /*!< Flash power down key1 */
#define FLASH_PDKEY2               ((uint32_t)0xFAFBFCFD) /*!< Flash power down key2: used with FLASH_PDKEY1 
                                                              to unlock the RUN_PD bit in FLASH_ACR */
#define FLASH_PEKEY1               ((uint32_t)0x89ABCDEF) /*!< Flash program erase key1 */
#define FLASH_PEKEY2               ((uint32_t)0x02030405) /*!< Flash program erase key: used with FLASH_PEKEY2
                                                               to unlock the write access to the FLASH_PECR register and
                                                               data EEPROM */
#define FLASH_PRGKEY1              ((uint32_t)0x8C9DAEBF) /*!< Flash program memory key1 */
#define FLASH_PRGKEY2              ((uint32_t)0x13141516) /*!< Flash program memory key2: used with FLASH_PRGKEY2
                                                               to unlock the program memory */
#define FLASH_OPTKEY1              ((uint32_t)0xFBEAD9C8) /*!< Flash option key1 */
#define FLASH_OPTKEY2              ((uint32_t)0x24252627) /*!< Flash option key2: used with FLASH_OPTKEY1 to
                                                              unlock the write access to the option byte block */

#define EraseTime 		((u32)0x2000)
#define ProgramTime		((u32)0x3000)

static u8 Flash_EOP(void);
static u8 Flash_Status(u32 timeOut);

/*******************************************************************
函 数 名：	Flash_Unlock
功能说明： 	STM32_Flash解锁
参	  数： 	无
返 回 值：	无
*******************************************************************/
void Flash_Unlock(void)
{
   if((FLASH->PECR & FLASH_PECR_PRGLOCK) != RESET)
  {
    /* Unlocking the data memory and FLASH_PECR register access */
    DATA_EEPROM_Unlock();
  
    /* Unlocking the program memory access */
    FLASH->PRGKEYR = FLASH_PRGKEY1;
    FLASH->PRGKEYR = FLASH_PRGKEY2;  
  }
}

/*******************************************************************
函 数 名：	DATA_EEPROM_Unlock
功能说明： 	Unlocking the data memory and FLASH_PECR register access
参	  数： 	无
返 回 值：	无
*******************************************************************/
void DATA_EEPROM_Unlock(void)
{
	if((FLASH->PECR & FLASH_PECR_PELOCK) != RESET)
	{  
		FLASH->PEKEYR = FLASH_PEKEY1;
		FLASH->PEKEYR = FLASH_PEKEY2;
	}
}
/*******************************************************************
函 数 名：	Flash_Unlock
功能说明： 	STM32_Flash上锁
参	  数： 	无
返 回 值：	无
*******************************************************************/
void Flash_lock(void)
{
     FLASH->PECR |= PECR_PRGLOCK;
}

/*******************************************************************
函 数 名：	FLASH_ErasePage
功能说明： 	Flash页擦除
参	  数： 	page_add:  业地址
返 回 值：  TRUE/FALSE
*******************************************************************/
u8 FLASH_ErasePage(u32 page_add)
{
    u8 flag = Flash_Status(EraseTime);

    if (!flag)//没有flash错误
    {
        FLASH->PECR |= PECR_ERASE;; // Set the ERASE bit 
        FLASH->PECR |= PECR_PROG;//要擦除页地址
		/* Write 00000000h to the first word of the program page to erase */
		*(__IO uint32_t *)page_add = 0x00000000;
        
		flag = Flash_Status(EraseTime);
        FLASH->PECR &= (uint32_t)(~PECR_PROG);
		FLASH->PECR &= (uint32_t)(~PECR_ERASE);
    }
    if (!flag)
    {
        return Flash_EOP(); //返回操作结果
    }
    
    return FALSE;
}

/*******************************************************************
函 数 名：	Flash_Status
功能说明： 	获取Flash状态寄存器值
参	  数：  timeOut:    检测超时时间
返 回 值：	Flash_status
*******************************************************************/
static u8 Flash_Status(u32 timeOut)
{
    for (;(timeOut > 0)&&(FLASH->SR & SR_BSY); timeOut--)//等待Flash空闲
    {}
    if (!timeOut)//等待超时
    {
        return SR_BSY;
    }
//	if (FLASH->SR&SR_WRPERR)
//	{
//		FLASH->SR = SR_WRPERR;
//	}
	if (FLASH->SR&(SR_WRPERR|SR_PGAERR|SR_SIZERR|SR_OPTVERR|SR_OPTVERRUSR))
	{
		FLASH->SR = SR_WRPERR|SR_PGAERR|SR_SIZERR|SR_OPTVERR|SR_OPTVERRUSR;
		return 1;
	}

    return 0;
}

///*******************************************************************
//函 数 名：	Flash_ProgramHalfWord
//功能说明： 	写入半个字节
//参	  数：  add:    写入地址
//            data:   写入数据内容
//返 回 值：	TRUE/FLASE
//*******************************************************************/
//u8 Flash_ProgramHalfWord(u32 add, u16 data)
//{
//    u8 flag = Flash_Status(ProgramTime);

//    if (!flag)
//    {
//        FLASH->CR |= CR_PG;
//        *(vu16*)add = (u16)data;
//        flag = Flash_Status(ProgramTime);
//    
//        FLASH->CR &= ~CR_PG;
//    }
//    if (!flag)
//    {
//        if (Flash_EOP())
//        {
//            flag = TRUE; 
//        }
//    }

//    return flag;
//}

/*******************************************************************
函 数 名：	Flash_Write
功能说明： 	flash写入数据
参	  数：  add:    写入首地址
            len:    写入数据大小
            data:   写入数据内容
返 回 值：	TRUE/FLASE
*******************************************************************/
u8 Flash_Write(u32 add, s32 len, u8 *data)
{
    if (!Flash_Status(ProgramTime))
    {
        while (len > 0)
        {
            *(vu32*)add = *(u32*)data;
            data += 4;
            add  += 4;
            len  -= 4;
            if (Flash_Status(ProgramTime))//未超时
            {
				break;
            }
        }
    } 
    return (len == 0 ? TRUE : FALSE);
}

/*******************************************************************
函 数 名：	Flash_EOP
功能说明： 	获取Flash操作结果标志位
参	  数：  无
返 回 值：	FALSE/TRUE (Flash写入或擦除操作结果)
*******************************************************************/
static u8 Flash_EOP(void)
{
    if (FLASH->SR&SR_EOP)//操作成功
    {
        FLASH->SR = SR_EOP; //清除标志
        return TRUE;
    }

    return FALSE;
}

///*******************************************************************
//函 数 名：	Flash_eraseOptionByte
//功能说明： 	擦除整个Flash选项字节
//参	  数：  无
//返 回 值：	TRUE/FALSE (操作结果)
//*******************************************************************/
//u8 Flash_EraseOptionByte(void)
//{
//    u8 flag = FALSE;
//    
//    if (!Flash_Status(ProgramTime))
//    {
//        Flash_Unlock();
//        FLASH->CR |= CR_OPTER;
//        FLASH->CR |= CR_STRT;
//        if (!Flash_Status(ProgramTime))
//        {
//            if (Flash_EOP())
//            {
//                flag = TRUE;
//            }
//        }
//        FLASH->CR &= ~CR_OPTER;
//        
//    }
//    return flag;
//}


///*******************************************************************
//函 数 名：	Flash_WriteOptionByte
//功能说明： 	写入Flash选项字节
//参	  数：  addr:   选项字节地址
//            data:   内容
//返 回 值：	TRUE/FALSE (操作结果)
//*******************************************************************/
//u8 Flash_WriteOptionByte(u32 addr, u8 data)
//{
//    u8 flag = FALSE;
//    
//    if (!Flash_Status(ProgramTime))
//    {
//        Flash_Unlock();
//        FLASH->CR |= CR_OPTPG;
//        FLASH->CR |= CR_STRT;
//        *(vu8*)addr = data;
//        if (!Flash_Status(ProgramTime))
//        {
//            if (Flash_EOP())
//            {
//                flag = TRUE;
//            }
//        }
//        FLASH->CR &= ~CR_OPTER;    
//    }
//    return flag;
//}

///*******************************************************************
//函 数 名：	Flash_WriteProtect
//功能说明： 	写入Flash选项字节
//参	  数：  addr:   保护的区域地址
//            sta:    0(NO protect)/(~0)(protect)
//返 回 值：	TRUE/FALSE (操作结果)
//注    意:   size <= 32Kb
//*******************************************************************/
//u8 Flash_WriteProtect(u32 addr, u8 sta)
//{
//    u8 seat, flag;
//    
//    addr -= 0x08000000;
//    seat = (u8)(addr>>12)&(0x1f);
//        
//
////    if (sta)
////    {
////        flag = Flash_WriteOptionByte((u32)(&OB->WRP0 + (seat>>3))
////                              ,&OB->WRP0 + ((seat>>3)|(1<<(seat&0x07))));
////    }
////    else
////    {
////       flag = Flash_WriteOptionByte((u32)(&OB->WRP0 + (seat>>3))
////                              ,&OB->WRP0 + ((seat>>3)&(~(1<<(seat&0x07)))));
////    }
//
//    return flag;
//}


/******************* (C) COPYRIGHT 2013 BestFu *****END OF FILE****/



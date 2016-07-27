/*******************************************************************
文	件：    program.c
说	明：    STM32_Flash烧入
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-11-21
********************************************************************/
#include "program.h"
#include "flash.h"
#include "BF_type.h"

#define FLASH_DEFAULT		(0x00000000)

/*******************************************************************
函 数 名：	Program_Erase
功能说明： 	擦除要编程的位置内容
参	  数： 	addr:   写入数据的首地址
            size:   要写入数据的大小
返 回 值：	擦除结果TRUE/FALSE
*******************************************************************/
u8 Program_Erase(u32 addr, u32 size)
{   
	FLASH->SR = SR_WRPERR;
	Flash_Unlock();
    while ((FLASH_ErasePage(addr) == TRUE)&&(size > PAGE_SIZE))
    {
        size -= PAGE_SIZE;
        addr += PAGE_SIZE;
    }
  	Flash_lock();

    return (size > PAGE_SIZE ? FALSE : TRUE);
}

/*******************************************************************
函 数 名：	Program_Write
功能说明： 	写入编程
参	  数： 	addr:    写入数据的首地址
            size:   要写入数据的大小
            data:   写入数据内容
返 回 值：	写入结果TRUE/FALSE
*******************************************************************/
u8 Program_Write(u32 addr, u32 len, u8 *data)
{
	u8 flag;
	
	if (len&0x03)		//内部为字节写入
	{
		len &= ~(0x03);
		len += 4;
	}
	Flash_Unlock();
	flag = Flash_Write(addr, len, data);
	Flash_lock();

	return flag;
}

/*******************************************************************
函 数 名：	Program_Read
功能说明： 	读出编程数据内容
参	  数： 	addr:   读出数据的首地址
            size:   要读出数据的大小
            data:   读出数据内容
返 回 值：	写入结果TRUE/FALSE
*******************************************************************/
u8 Program_Read(u32 addr, u32 len, u8 *data)
{
    while (len > 0)
    {
        *(vu32*)data = *(vu32*)addr;
        data += 4;
        addr += 4;
        len  -= 4;
    }
    
	return TRUE;
}

/*******************************************************************
函 数 名：	Flash_DefaultValue
功能说明： 	False默认值
参	  数： 	void
返 回 值：	默认状态下Flashw值
*******************************************************************/
u32 Flash_DefaultValue(void)
{
	return (FLASH_DEFAULT);
}

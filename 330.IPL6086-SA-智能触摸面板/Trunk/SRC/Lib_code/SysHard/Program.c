/*******************************************************************
��	����    program.c
˵	����    STM32_Flash����
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2013-11-21
********************************************************************/
#include "program.h"
#include "BF_type.h"
#include "DeviceIDSetAttr.h"

#define FLASH_DEFAULT		(0x00000000)
#define STM_SECTOR_SIZE     (PAGE_SIZE)
#define STM32_FLASH_BASE    (0x08000000)      //STM32 FLASH����ַ

/*******************************************************************
�� �� ����	Program_Erase
����˵���� 	����Ҫ��̵�λ������
��	  ���� 	addr:   д�����ݵ��׵�ַ
            size:   Ҫд�����ݵĴ�С
�� �� ֵ��	�������TRUE/FALSE
*******************************************************************/
u8 Program_Erase(u32 addr, u32 size)
{   
	FLASH->SR = FLASH_SR_WRPERR;
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
�� �� ����	Program_Write
����˵���� 	д����
��	  ���� 	addr:    д�����ݵ��׵�ַ
            size:   Ҫд�����ݵĴ�С
            data:   д����������
�� �� ֵ��	д����TRUE/FALSE
*******************************************************************/
u8 Program_Write(u32 addr, u32 len, u8 *data)
{
	u8 flag;
	
	if (len&0x03)		//�ڲ�Ϊ�ֽ�д��
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
�� �� ����	Program_Read
����˵���� 	���������������
��	  ���� 	addr:   �������ݵ��׵�ַ
            size:   Ҫ�������ݵĴ�С
            data:   ������������
�� �� ֵ��	д����TRUE/FALSE
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

/*******************************************************************************
�� �� ����	Program_WriteEx
����˵���� 	����չ��д����
��	  ���� 	addr:    д�����ݵ��׵�ַ
            size:   Ҫд�����ݵĴ�С
            data:   д����������
�� �� ֵ��	д����TRUE/FALSE
*******************************************************************************/
u8 Program_WriteEx(u32 addr, u32 len, u8 *data)			  /**> ����flash�����չ�ӿڣ�Jay Add 2015.11.13 **/
{
	u8 flag;
    u32 secpos;	                                              /**>  ������ַ                           **/
	u16 secoff;	                                              /**>  ������ƫ�Ƶ�ַ                     **/
	u16 secremain;                                            /**>  ������ʣ���ַ                     **/	   
 	u16 i;    
	u32 offaddr;                                              /**>  ȥ��0X08000000��ĵ�ַ             **/
    u8 STMFLASH_BUF[STM_SECTOR_SIZE];
    
	if (len & 0x03)		                                      /**>  �ڲ�Ϊһ����д��                   **/
	{
		len &= ~(0x03);
		len += 4;
	}
	Flash_Unlock();
    if((addr < STM32_FLASH_BASE)||                            /**> ����ַԽ�磬ֱ�ӷ��ش���            **/
       (addr >= (STM32_FLASH_BASE+1536*STM_SECTOR_SIZE)))  return 0;
  
    offaddr   = addr-STM32_FLASH_BASE;		                  /**>  ʵ��ƫ�Ƶ�ַ                       **/
	secpos    = offaddr/STM_SECTOR_SIZE;			          /**>  ������ַ  0~1536 for STM32L152     **/
	secoff    = offaddr%STM_SECTOR_SIZE;		              /**>  �������ڵ�ƫ��                     **/
	secremain = STM_SECTOR_SIZE-secoff;		                  /**>  ����ʣ��ռ��С                   **/   
	if(len <= secremain)  secremain = len;                    /**>  �����ڸ�������Χ                   **/
    while(1) 
	{	
		Program_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE, /**>  ������������������                 **/
                     STM_SECTOR_SIZE,
                     STMFLASH_BUF);
		for(i=0;i<secremain;i++)                              /**>  У������                           **/
		{
			if(((u8)FLASH_DEFAULT) != STMFLASH_BUF[secoff+i]) break;             	  
		}
		if(i<secremain)                                       /**>  ��Ҫ����                           **/  
		{
            flag=FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);
			for(i=0;i<secremain;i++)  
			{
				STMFLASH_BUF[i+secoff] = data[i];	  
			}
            flag=Flash_Write(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE, STM_SECTOR_SIZE, STMFLASH_BUF);
		}else flag=Flash_Write(addr,secremain,data);         /**>  д�Ѿ������˵�,ֱ��д������ʣ������**/ 				   
		if(len==secremain)break;                             /**>  д�������                         **/
		else                                                 /**>  д��δ����                         **/
		{
			secpos++;                                        /**>  ������ַ��1                        **/
			secoff=0;				                         /**>  ƫ��λ��Ϊ0 	                     **/ 
            data += secremain;                               /**>  ָ��ƫ��                           **/
			addr += secremain;	                             /**>  д��ַƫ��                         **/  
            len  -= secremain;	                             /**>  �ֽ����ݼ�                         **/
			if(len>(STM_SECTOR_SIZE)) secremain=STM_SECTOR_SIZE;/**>  ��һ����������д����               **/
			else secremain = len;                            /**>  ��һ����������д����               **/
		}	 
	};
	Flash_lock();
	return flag;
}

#if (FLASH_ENCRYPTION_EN > 0u)
/*******************************************************************************
�� �� ����	void FlashEncryptionInit(void)
����˵���� 	Flash���ܳ�ʼ��
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void FlashEncryptionInit(void)
{
	u32 encryption_info[2] = {ENCRYPTION_FLAG,0};
	RCC->AHBENR 	|= RCC_AHBENR_CRCEN;
	CRC->CR	  		|= CRC_CR_RESET;
	CRC->IDR	  	|= CRC_IDR_IDR;
	if(ENCRYPTION_FLAG != *(vu32*)(ENCRYPTION_START_ADDR))
	{
		encryption_info[1] = FlashEncryptionFunc(*(vu32*)(RE_DEVICE_START_ADDR + 4));
		Program_WriteEx(ENCRYPTION_START_ADDR,sizeof(encryption_info),(u8*)(encryption_info));
	}
}
#endif

#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
/*******************************************************************************
�� �� ����	void Get_UniqueDeviceID(u8* chipidtable)
����˵���� 	��ȡstm32 flashȫ��ΨһID
��	  ����  chipidtable:�洢96λȫ��ΨһID���׵�ַ
�� �� ֵ��	��
*******************************************************************************/
void Get_UniqueDeviceID(u8* chipidtable)
{
	*(u32*)(&chipidtable[0]) = *((vu32*)(CHIPID_ADDR + 0x00));
	*(u32*)(&chipidtable[4]) = *((vu32*)(CHIPID_ADDR + 0x04));
	*(u32*)(&chipidtable[8]) = *((vu32*)(CHIPID_ADDR + 0x14));
}
#endif

/*******************************************************************
�� �� ����	Flash_DefaultValue
����˵���� 	FalseĬ��ֵ
��	  ���� 	void
�� �� ֵ��	Ĭ��״̬��Flashwֵ
*******************************************************************/
u32 Flash_DefaultValue(void)
{
	return (FLASH_DEFAULT);
}

/*******************************************************************************
�� �� ����	Flash_PageSize
����˵���� 	False������С
��	  ���� 	void
�� �� ֵ��	False������С
*******************************************************************************/
u32 Flash_PageSize(void)
{
	return (PAGE_SIZE);
}

/*******************************************************************************
�� �� ����	Flash_ChipSize
����˵���� 	FalseоƬ��С
��	  ���� 	void
�� �� ֵ��	FalseоƬ��С
*******************************************************************************/
u32 Flash_ChipSize(void)
{
	return (*(volatile u16*)(USER_FLASH_AREA));
}

/************************************End of File*******************************/

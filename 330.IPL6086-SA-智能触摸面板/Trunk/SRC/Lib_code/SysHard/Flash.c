/*******************************************************************
��	����    Flash.c
˵	����    STM32_Flash��������
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2013-11-21
********************************************************************/
#include "stm32l1xx.h"
#include "Flash.h"
#include "BF_type.h"

/* FLASH Keys */
#define FLASH_PDKEY1               ((u32)0x04152637) /*!< Flash power down key1 */
#define FLASH_PDKEY2               ((u32)0xFAFBFCFD) /*!< Flash power down key2: used with FLASH_PDKEY1 
                                                              to unlock the RUN_PD bit in FLASH_ACR */
#define FLASH_PEKEY1               ((u32)0x89ABCDEF) /*!< Flash program erase key1 */
#define FLASH_PEKEY2               ((u32)0x02030405) /*!< Flash program erase key: used with FLASH_PEKEY2
                                                               to unlock the write access to the FLASH_PECR register and
                                                               data EEPROM */
#define FLASH_PRGKEY1              ((u32)0x8C9DAEBF) /*!< Flash program memory key1 */
#define FLASH_PRGKEY2              ((u32)0x13141516) /*!< Flash program memory key2: used with FLASH_PRGKEY2
                                                               to unlock the program memory */
#define FLASH_OPTKEY1              ((u32)0xFBEAD9C8) /*!< Flash option key1 */
#define FLASH_OPTKEY2              ((u32)0x24252627) /*!< Flash option key2: used with FLASH_OPTKEY1 to
                                                              unlock the write access to the option byte block */

#define EraseTime 					((u32)0x2000)
#define ProgramTime					((u32)0x3000)

//Priivate function
static u8 Flash_EOP(void);
static FlashStatus_t Flash_Status(u32 timeOut);

/*******************************************************************
�� �� ����	Flash_Unlock
����˵���� 	STM32_Flash����
��	  ���� 	��
�� �� ֵ��	��
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
�� �� ����	DATA_EEPROM_Unlock
����˵���� 	Unlocking the data memory and FLASH_PECR register access
��	  ���� 	��
�� �� ֵ��	��
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
�� �� ����	Flash_Unlock
����˵���� 	STM32_Flash����
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************/
void Flash_lock(void)
{
     FLASH->PECR |= FLASH_PECR_PRGLOCK;
}

/*******************************************************************
�� �� ����	FLASH_ErasePage
����˵���� 	Flashҳ����
��	  ���� 	page_add:  ҵ��ַ
�� �� ֵ��  TRUE/FALSE
*******************************************************************/
u8 FLASH_ErasePage(u32 page_add)
{
    FlashStatus_t flag = Flash_Status(EraseTime);

    if (!flag)//û��flash����
    {
        FLASH->PECR |= FLASH_PECR_ERASE; // Set the ERASE bit 
        FLASH->PECR |= FLASH_PECR_PROG;	//Ҫ����ҳ��ַ
		/* Write 00000000h to the first word of the program page to erase */
		*(vu32 *)page_add = 0x00000000;
        
		flag = Flash_Status(EraseTime);
        FLASH->PECR &= (u32)(~FLASH_PECR_PROG);
		FLASH->PECR &= (u32)(~FLASH_PECR_ERASE);
    }
    if (!flag)
    {
        return Flash_EOP(); //���ز������
    }
    
    return 0;
}

/*******************************************************************
�� �� ����	Flash_Status
����˵���� 	��ȡFlash״̬�Ĵ���ֵ
��	  ����  timeOut:    ��ⳬʱʱ��
�� �� ֵ��	Flash_status
*******************************************************************/
static FlashStatus_t Flash_Status(u32 timeOut)
{
    for (;(timeOut > 0)&&(FLASH->SR & FLASH_SR_BSY); timeOut--)//�ȴ�Flash����
    {}
    if (!timeOut)//�ȴ���ʱ
    {
        return FLASH_TIMEOUT;
    }
//	if(((FLASH->SR) & (FLASH_SR_EOP)) == (FLASH_SR_EOP))
//	{
//		FLASH->SR = FLASH_SR_EOP;
//	}
	if (FLASH->SR&(FLASH_SR_WRPERR|FLASH_SR_PGAERR|FLASH_SR_SIZERR|FLASH_SR_OPTVERR|FLASH_SR_OPTVERRUSR))
	{
		FLASH->SR = FLASH_SR_WRPERR|FLASH_SR_PGAERR|FLASH_SR_SIZERR|FLASH_SR_OPTVERR|FLASH_SR_OPTVERRUSR;
		return FLASH_ERROR;;
	}

    return FLASH_OK;
}

/*******************************************************************
�� �� ����	Flash_Write
����˵���� 	flashд������
��	  ����  add:    д���׵�ַ
            len:    д�����ݴ�С
            data:   д����������
�� �� ֵ��	TRUE/FLASE
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
            if (Flash_Status(ProgramTime))//δ��ʱ
            {
				break;
            }
        }
    } 
    return (len == 0 ? 1 : 0);
}

/*******************************************************************
�� �� ����	Flash_EOP
����˵���� 	��ȡFlash���������־λ
��	  ����  ��
�� �� ֵ��	FALSE/TRUE (Flashд�������������)
*******************************************************************/
static u8 Flash_EOP(void)
{
    if (FLASH->SR&FLASH_SR_EOP)//�����ɹ�
    {
        FLASH->SR = FLASH_SR_EOP; //�����־
        return 1;
    }

    return 0;
}

///*******************************************************************
//�� �� ����	Flash_eraseOptionByte
//����˵���� 	��������Flashѡ���ֽ�
//��	  ����  ��
//�� �� ֵ��	TRUE/FALSE (�������)
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
//�� �� ����	Flash_WriteOptionByte
//����˵���� 	д��Flashѡ���ֽ�
//��	  ����  addr:   ѡ���ֽڵ�ַ
//            data:   ����
//�� �� ֵ��	TRUE/FALSE (�������)
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
//�� �� ����	Flash_WriteProtect
//����˵���� 	д��Flashѡ���ֽ�
//��	  ����  addr:   �����������ַ
//            sta:    0(NO protect)/(~0)(protect)
//�� �� ֵ��	TRUE/FALSE (�������)
//ע    ��:   size <= 32Kb
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



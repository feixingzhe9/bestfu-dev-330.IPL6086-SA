/***************************Copyright BestFu 2014-05-14*************************
��	����	Flash.c
˵	����	Flash��������
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-11-21
��  ��:     ����
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "stm32l1xx.h"
#include "DeviceIDSetAttr.h"
#include "DevPartTab.h"
#include "flash.h"

/* FLASH Keys */
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

#define EraseTime 					((uint32_t)0x2000)
#define ProgramTime					((uint32_t)0x3000)


static uint8_t Flash_EOP(void);
static FlashStatus_t Flash_Status(uint32_t timeOut);

/*******************************************************************************
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
        if((FLASH->PECR & FLASH_PECR_PELOCK) != RESET)
        {
            FLASH->PEKEYR = FLASH_PEKEY1;
            FLASH->PEKEYR = FLASH_PEKEY2;
        }
        /* Unlocking the program memory access */
        FLASH->PRGKEYR = FLASH_PRGKEY1;
        FLASH->PRGKEYR = FLASH_PRGKEY2;
    }
}

/*******************************************************************
�� �� ����	Flash_Unlock
����˵���� 	STM32_Flash����
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void Flash_lock(void)
{
    FLASH->PECR |= FLASH_PECR_PRGLOCK;
}

/*******************************************************************************
�� �� ����	FLASH_ErasePage
����˵���� 	Flashҳ����
��	  ���� 	page_add:  ҵ��ַ
�� �� ֵ��  TRUE/FALSE
*******************************************************************/
uint8_t FLASH_ErasePage(uint32_t page_add)
{
    FlashStatus_t flag = Flash_Status(EraseTime);

    if (!flag)//û��flash����
    {
        FLASH->PECR |= FLASH_PECR_ERASE;	// Set the ERASE bit
        FLASH->PECR |= FLASH_PECR_PROG;		//Ҫ����ҳ��ַ
        /* Write 00000000h to the first word of the program page to erase */
        *(__IO uint32_t *)page_add = 0x00000000;

        flag = Flash_Status(EraseTime);
        FLASH->PECR &= (uint32_t)(~FLASH_PECR_PROG);
        FLASH->PECR &= (uint32_t)(~FLASH_PECR_ERASE);
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
static FlashStatus_t Flash_Status(uint32_t timeOut)
{
    for (; (timeOut > 0)&&(FLASH->SR & FLASH_SR_BSY); timeOut--) //�ȴ�Flash����
    {}
    if (!timeOut)//�ȴ���ʱ
    {
        return FLASH_TIMEOUT;
    }
//	if(((FLASH->SR) & (FLASH_SR_EOP)) == (FLASH_SR_EOP))
//	{
//		FLASH->SR = FLASH_SR_EOP;
//	}
    if((FLASH->SR) & (FLASH_SR_WRPERR|FLASH_SR_OPTVERR|FLASH_SR_SIZERR|FLASH_SR_OPTVERRUSR|FLASH_SR_PGAERR))
    {
        FLASH->SR = (FLASH_SR_WRPERR|FLASH_SR_OPTVERR|FLASH_SR_SIZERR|FLASH_SR_OPTVERRUSR|FLASH_SR_PGAERR);
        return FLASH_ERROR;
    }
    return FLASH_OK;
}

/*******************************************************************************
�� �� ����	Flash_Write
����˵���� 	flashд������
��	  ����  add:    д���׵�ַ
            len:    д�����ݴ�С
            data:   д����������
�� �� ֵ��	TRUE/FLASE
*******************************************************************/
uint8_t Flash_Write(uint32_t add, int32_t len, uint8_t *data)
{
    if (!Flash_Status(ProgramTime))
    {
        while (len > 0)
        {
            *(__IO uint32_t *)add = *(uint32_t*)data;
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
static uint8_t Flash_EOP(void)
{
    if (FLASH->SR&FLASH_SR_EOP)//�����ɹ�
    {
        FLASH->SR = FLASH_SR_EOP; //�����־
        return 1;
    }
    return 0;
}
/******************* (C) COPYRIGHT 2013 BestFu *****END OF FILE****/



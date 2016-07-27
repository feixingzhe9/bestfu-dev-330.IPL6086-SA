/***************************Copyright BestFu 2014-05-14*************************
��	����    Program.c
˵	����    STM32_Flash����
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2013-11-21
�ޡ��ģ�	����
*******************************************************************************/
#include "stm32l1xx.h"
#include "lib_type.h"
#include "lib_config.h"
#include "flash.h"
#include "UserData.h"
#include "DevPartTab.h"
#include "program.h"
#include "DeviceIDSetAttr.h"

//Private macro define
#define FLASH_DEFAULT		(0x00000000)		//Ĭ��FLASH�ĳ�ֵ
#define PAGE_SIZE     		(0x100)				//ҳ��ַ��С
#define STM32_FLASH_BASE    (0x08000000)		//STM32 FLASH����ַ

/*******************************************************************************
�� �� ����	Program_Erase
����˵���� 	����Ҫ��̵�λ������
��	  ���� 	addr:   д�����ݵ��׵�ַ
            size:   Ҫд�����ݵĴ�С
�� �� ֵ��	�������TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN Program_Erase(BF_INT32U addr, BF_INT32U size)
{
    Flash_Unlock();
    while (FLASH_ErasePage(addr)&&(size > PAGE_SIZE))
    {
        size -= PAGE_SIZE;
        addr += PAGE_SIZE;
    }
    Flash_lock();

    return (size > PAGE_SIZE ? FALSE : TRUE);
}

/*******************************************************************************
�� �� ����	Program_Write
����˵���� 	д����
��	  ���� 	addr:    д�����ݵ��׵�ַ
            size:   Ҫд�����ݵĴ�С
            data:   д����������
�� �� ֵ��	д����TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN Program_Write(BF_INT32U addr, BF_INT32U len, BF_INT08U *data)
{
    BF_INT08U flag;

    if (len & 0x01)		//�ڲ�Ϊ���ֽ�д��
    {
        len++;
    }
    Flash_Unlock();
    flag = Flash_Write(addr, len, data);
    Flash_lock();

    return flag;
}

/*******************************************************************************
�� �� ����	Program_Read
����˵���� 	���������������
��	  ���� 	addr:   �������ݵ��׵�ַ
            size:   Ҫ�������ݵĴ�С
            data:   ������������
�� �� ֵ��	д����TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN Program_Read(BF_INT32U addr, BF_INT32U len, BF_INT08U *data)
{
    len = (len&0x03)?(((len&(~0x03))|(1u<<2))):(len);
    while (len > 0)
    {
        *(volatile BF_INT32U*)data = *(volatile BF_INT32U*)addr;
        data += 4;
        addr += 4;
        len  -= 4;
    }

    return TRUE;
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
    BF_INT32U backp_tab[18];
    BF_INT16U encryptflag_pos= 0u;

    RCC->AHBENR |= RCC_AHBENR_CRCEN;
    CRC->CR		|= CRC_CR_RESET;
    CRC->IDR	|= CRC_IDR_IDR;
    if(ENCRYPTION_FLAG != *(volatile BF_INT32U*)(ENCRYPTION_FLAG_ADDR))
    {
        encryptflag_pos=(GetDevPivotalSet())->DevFlashEncryptAddr - DevPartTab->boot0_flag_start_addr;
        backp_tab[encryptflag_pos]		= ENCRYPTION_FLAG;
        backp_tab[encryptflag_pos + 1]	= FlashEncryptionFunc(*(volatile BF_INT32U*)(RESET_DEV_ID_FLAG_ADDR + 4));
        if(*(volatile BF_INT32U*)(ENCRYPTION_FLAG_ADDR) == FLASH_DEFAULT)
        {
            Program_Write(ENCRYPTION_FLAG_ADDR,sizeof(FlashEncrypt_t),(BF_INT08U*)(&backp_tab[encryptflag_pos]));
        }
        else
        {
            Program_Read(DevPartTab->boot0_flag_start_addr,encryptflag_pos + sizeof(FlashEncrypt_t),(BF_INT08U*)&backp_tab);
            Program_Erase(DevPartTab->boot0_flag_start_addr,DevPartTab->boot1_start_addr - DevPartTab->boot0_flag_start_addr);
            Program_Write(DevPartTab->boot0_flag_start_addr,encryptflag_pos + sizeof(FlashEncrypt_t),(BF_INT08U *)&backp_tab);
        }
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
void Get_UniqueDeviceID(BF_INT08U* chipidtable)
{
    *(volatile BF_INT32U*)(&chipidtable[0]) = *((volatile BF_INT32U*)(CHIPID_ADDR + 0x00));
    *(volatile BF_INT32U*)(&chipidtable[4]) = *((volatile BF_INT32U*)(CHIPID_ADDR + 0x04));
    *(volatile BF_INT32U*)(&chipidtable[8]) = *((volatile BF_INT32U*)(CHIPID_ADDR + 0x14));
}
#endif

/*******************************************************************************
�� �� ����	Flash_DefaultValue
����˵���� 	FalseĬ��ֵ
��	  ���� 	void
�� �� ֵ��	Ĭ��״̬��Flashwֵ
*******************************************************************************/
BF_INT32U Flash_DefaultValue(void)
{
    return (FLASH_DEFAULT);
}

/*******************************************************************************
�� �� ����	Flash_PageSize
����˵���� 	False������С
��	  ���� 	void
�� �� ֵ��	False������С
*******************************************************************************/
BF_INT32U Flash_PageSize(void)
{
    return (PAGE_SIZE);
}

/*******************************************************************************
�� �� ����	Flash_ChipSize
����˵���� 	FalseоƬ��С
��	  ���� 	void
�� �� ֵ��	FalseоƬ��С
*******************************************************************************/
BF_INT32U Flash_ChipSize(void)
{
    return (*(volatile BF_INT16U*)(USER_FLASH_AREA));
}

/**************************Copyright BestFu 2014-05-14*************************/

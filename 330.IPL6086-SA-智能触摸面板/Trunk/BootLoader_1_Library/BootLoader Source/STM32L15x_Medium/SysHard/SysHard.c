/***************************Copyright BestFu 2014-05-14*************************
��	����    SysHard.c
˵	����    ϵͳӲ����غ���
��	�룺    Keil uVision4 V4.54.0.0
��	����    v2.0
��	д��    Unarty
��	�ڣ�    2014.06.26
�ޡ��ģ�	����
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "stm32l1xx.h"
#include "i2c.h"
#include "SysTick.h"
#include "flash.h"
#include "userdata.h"
#include "PivotalDataType.h"
#include "DeviceIDSetAttr.h"
#include "DevPartTab.h"
#include "program.h"
#include "usart.h"
#include "DevPartTab.h"
#include "SysHard.h"
#include "SI4432.h"
#include "iwdg.h"

/*******************************************************************************
�� �� ����	SysHard_Init
����˵���� 	��ʼ��ϵͳӲ���������
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void SysHard_Init(void)
{
    SysTick_Init();				//��ʼ��ϵͳʱ��
#if (FLASH_ENCRYPTION_EN > 0u)
    FlashEncryptionInit();
#endif
    I2C_Init();		//IIC��ʼ��
#if (RF_433M_INTERFACE_EN > 0u)
    Si4432_Init();				//433����ģ���ʼ��
#endif
#if (USART_INTERFACE_EN > 0u )
    USART_Init(9600);
#endif
    IWDG_Config(5,2000);
}

/*******************************************************************************
�� �� ����	HardID_Check
����˵���� 	Ӳ����ַ�˶�
��	  ���� 	id: ����ƥ��ID
�� �� ֵ��	0����ַƥ��ɹ�)/���򷵻��豸��ʵID
*******************************************************************************/
BF_INT32U HardID_Check(BF_INT32U id)
{
#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    DeviceIDSet_t* pDeviceIDSet = (DeviceIDSet_t*)(pDevPivotalSet->DevResetParaAddr);

    if(DEVICE_ID_IS_OK(pDeviceIDSet->SetAddrFlag,\
                       pDeviceIDSet->DeviceID,pDeviceIDSet->DeviceIDInver)) 		//��������õ��µ�ַ�Ϸ�
    {
        if(pDeviceIDSet->DeviceID != id)
        {
            return (pDeviceIDSet->DeviceID);
        }
    }
    else
#endif
    {
        if (DEVICE_ID != id)
        {
            return DEVICE_ID;
        }
    }//End of if(DEVICE_ID_IS_OK)
    return 0;
}

/**************************Copyright BestFu 2014-05-14*************************/

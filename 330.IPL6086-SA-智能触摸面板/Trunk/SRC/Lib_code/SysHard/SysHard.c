/***************************Copyright BestFu 2014-05-14*************************
��	����    SysHard.c
˵	����    ϵͳӲ����غ���
��	�룺    Keil uVision4 V4.54.0.0
��	����    v2.0
��	д��    Unarty
��	�ڣ�    2014.06.26
�ޡ��ģ�	����
*******************************************************************************/
#include "SysHard.h"
#include "sys.h"
#include "SysTick.h"
#include "SI4432.h"
#include "I2C.h"
#include "UserData.h"
#include "program.h"
#include "DeviceIDSetAttr.h"
#include "DevPartTab.h"
#include "SysHard.h"
																				
/*******************************************************************************
�� �� ����	SysHard_Init
����˵���� 	��ʼ��ϵͳӲ���������
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void SysHard_Init(void)
{
	DevPivotalSetInit();
	SysTick_Init();		//��ʼ��ϵͳʱ��
#if (FLASH_ENCRYPTION_EN > 0u)
	FlashEncryptionInit();
#endif
	I2C_Init();				//IIC��ʼ��
	Si4432_Init();			//433����ģ���ʼ��
}

/*******************************************************************************
�� �� ����	HardID_Check
����˵���� 	Ӳ����ַ�˶�
��	  ���� 	id: ����ƥ��ID
�� �� ֵ��	0����ַƥ��ɹ�)/���򷵻��豸��ʵID
*******************************************************************************/
u32 HardID_Check(u32 id)
{
#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
	if(DEVICE_ID_IS_OK((*(vu32*)(RE_DEVICE_START_ADDR)),\
		(*(vu32*)(RE_DEVICE_START_ADDR + 4)),(*(vu32*)(RE_DEVICE_START_ADDR + 8))))//��������õ��µ�ַ�Ϸ�
    {
        if((*(vu32*)(RE_DEVICE_START_ADDR + 4)) != id)
        {
            return ((*(vu32*)(RE_DEVICE_START_ADDR + 4)));
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

#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
/*******************************************************************************
�� �� ����	void HardID_Change(void)
����˵���� 	�����¾ɵ�ַ�ĸ���
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void HardID_Change(void)			/**> �����¾ɵ�ַ���£�Jay Add 2015.11.13 **/
{
	if(DEVICE_ID_IS_OK((*(vu32*)(RE_DEVICE_START_ADDR)),\
		(*(vu32*)(RE_DEVICE_START_ADDR + 4)),(*(vu32*)(RE_DEVICE_START_ADDR + 8))))	//��������õ��µ�ַ�Ϸ�
	{
		gSysData.deviceID = (*(vu32*)(RE_DEVICE_START_ADDR + 4));
	}
}
#endif
/**************************Copyright BestFu 2014-05-14*************************/

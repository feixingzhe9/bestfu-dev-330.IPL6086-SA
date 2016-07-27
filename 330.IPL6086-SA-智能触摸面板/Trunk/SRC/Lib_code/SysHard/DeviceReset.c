/***************************Copyright BestFu 2014-05-14*************************
��	����	DeviceReset.c
˵	����	�豸���ýӿں���
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.07.1
��  ��:     ����
*******************************************************************************/
#include "DeviceReset.h"
#include "SysHard.h"
#include "Thread.h"
#include "GPIO.h"
#include "UserData.h"
#include "UnitShare_Attr.h"

#if RESET_USER_DATA
	static void Reset_Check(void);
#endif

/*******************************************************************
�� �� ����	DeviceResetCheck_Init
����˵���� 	�豸���ü���ʼ��
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************/
void DeviceResetCheck_Init(void)
{
#if RESET_USER_DATA
	RESET_KEY_INIT;
	Thread_Login(FOREVER, 0, 2000, &Reset_Check);
#endif
}

#if RESET_USER_DATA
/*******************************************************************
�� �� ����	Reset_Check()
����˵���� 	�û��������ü��
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************/
void Reset_Check(void)
{
	static u32 cnt = 0;
	
	if (!RESET_KEY_VAL)	//������ð�����Ч
	{	
		if (!(cnt++))	//��һ�ν���
		{
			Thread_Login(FOREVER, 0, 20, &Reset_Check);	//�ӿ����ٶ�
		}
		if (cnt == 250)	//5S
		{
			u8 data[] = {0, 0x90, 1, 0};;
			UserEEPROMData_Init(SecondLevel); //��λ�û�����
			Set_UnitChecking_Attr((UnitPara_t*)data);
		}
	}
	else if (0 != cnt)
	{
		DeviceResetCheck_Init();
		cnt = 0;
	}
}
#endif

/**************************Copyright BestFu 2014-05-14*************************/

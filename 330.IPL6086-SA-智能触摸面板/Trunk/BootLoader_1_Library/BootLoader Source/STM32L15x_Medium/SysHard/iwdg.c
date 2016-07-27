/***************************Copyright xxxxx 2015-12-30**************************
**  ��    ����  stm32f4xx_iwdg.c
**  ��    �ܣ�  �������Ź��ײ���������
**  ��    �룺  MDK-ARM Professional Versoin: 5.12.0.0
**  ��    ����  V1.0
**  ��    д��  Jay
**  �������ڣ�  2015-11-26
**  �޸����ڣ�  ��
**  ˵    ����  ����
*******************************************************************************/
#include "stm32l1xx.h"
#include "lib_type.h"
#include "lib_config.h"
#include "SysHard.h"
#include "Thread.h"
#include "gpio.h"
#include "iwdg.h"

/* KR register bit mask */
#define KR_KEY_RELOAD    			((uint16_t)0xAAAA)//Feed dog value
#define KR_KEY_ENABLE    			((uint16_t)0xCCCC)//starts the watchdog
#define IWDG_WriteAccess_Enable     ((uint16_t)0x5555)//enable access to the IWDG_PR and IWDG_RLR registers
#define IWDG_WriteAccess_Disable    ((uint16_t)0x0000)//disable access to the IWDG_PR and IWDG_RLR registers

/*******************************************************************************
�� �� ���� void IWDG_Config(uint8_t IWDG_Prescaler,uint16_t Reload)
����˵���� ���ö������ź���
��    ���� IWDG_Prescaler:Ԥ��Ƶֵ(IWDG_Prescaler_4,IWDG_Prescaler_8,
						  IWDG_Prescaler_16,IWDG_Prescaler_32,IWDG_Prescaler_64,
						  IWDG_Prescaler_128,IWDG_Prescaler_128)
		   Reload:0x0000~0x0fff
�� �� ֵ�� ��
˵    ��:  ��
*******************************************************************************/
void IWDG_Config(uint8_t IWDG_Prescaler,uint16_t Reload)
{
#if (INTERNAL_IWDG_EN > 0u)
    IWDG->KR = IWDG_WriteAccess_Enable;		//ʹ��IWDG_PR��IWDG_RLR�Ĵ����ķ���
    IWDG->PR = IWDG_Prescaler;				//����Ԥ��Ƶֵ
    IWDG->RLR= Reload;						//������װ��ֵ
    IWDG->KR = KR_KEY_RELOAD;				//������װ��ֵ
    IWDG->KR = KR_KEY_ENABLE; 				//�������Ź�
#endif
#if (EXTERNAL_WDG_EN > 0u)
    GPIO_Config(WDG_PORT, WDG_PIN, OUT_PP_2M);
#endif
#if ((INTERNAL_IWDG_EN > 0u)||(EXTERNAL_WDG_EN > 0u))
    Thread_Login(FOREVER, 0, 800, &IWDG_Feed);	//ע��ιӲ�����Ź�
#endif
}

/*******************************************************************************
�� �� ���� void IWDG_Feed(void)
����˵���� �������Ź�ι������
��    ���� ��
�� �� ֵ�� ��
˵    ��:  ��
*******************************************************************************/
void IWDG_Feed(void)
{
#if (INTERNAL_IWDG_EN > 0u)
    IWDG->KR = KR_KEY_RELOAD;
#endif
#if (EXTERNAL_WDG_EN > 0u)
    GPIO_ToggleBits(WDG_PORT, WDG_PIN);//ιӲ����
#endif
}

/*******************************************************************************
�� �� ���� void IWDG_FeedOFF(void)
����˵���� �رն������Ź�ι������
��    ���� ��
�� �� ֵ�� ��
˵    ��:  ��
*******************************************************************************/
void IWDG_FeedOFF(void)
{
#if ((INTERNAL_IWDG_EN > 0u)||(EXTERNAL_WDG_EN > 0u))
    Thread_Logout(&IWDG_Feed);
#endif
}

/**************************************End of File*****************************/

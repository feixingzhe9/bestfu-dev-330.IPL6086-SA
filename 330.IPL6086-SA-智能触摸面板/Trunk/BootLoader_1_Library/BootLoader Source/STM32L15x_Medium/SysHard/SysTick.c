/***************************Copyright xxxxx 2015-12-30**************************
**  ��    ����stm32f10x_SysTick.c
**  ��    �ܣ�ϵͳ�δ�ʱ������
**  ��    �룺MDK-ARM Professional Versoin: 5.12.0.0
**  ��    ����V1.0
**  ��    д��Jay
**  �������ڣ�2015-12-29
**  �޸����ڣ���
**  ˵    ��������
*******************************************************************************/
#include "stm32l1xx.h"
#include "lib_type.h"
#include "lib_config.h"
#include "Thread.h"
#include "SysTick.h"
#include "system_stm32l1xx.h"

//Private macro define
#define TIME_MAX        (400000000) 			//����ʱʱ��
#define NVIC_ST_CTRL    (*((volatile BF_INT32U *)0xE000E010))
#define NVIC_ST_RELOAD  (*((volatile BF_INT32U *)0xE000E014))
#define RELOAD_VAL      ((BF_INT32U)(( (BF_INT32U)RCC_Clocks.HCLK_Frequency) / (BF_INT32U)(1000/THREAD_TIME_SLICE)) -1)

//Private variable define
static volatile BF_INT32U su_timer = 0u;

/*******************************************************************************
�� �� ���� void SysTick_Init(void)
����˵���� ϵͳ�δ�ʱ����ʼ��
��    ���� tick:tick��װֵ
�� �� ֵ�� ��
˵   ����  ��
*******************************************************************************/
void SysTick_Init(void)
{
    RCC_ClocksTypeDef RCC_Clocks;
    RCC_GetClocksFreq(&RCC_Clocks);
    /*!< Initial System tick.	*/
    NVIC_ST_RELOAD =  RELOAD_VAL;
    NVIC_ST_CTRL   =  0x0007;
}

/*******************************************************************************
�� �� ����	Time_Get
����˵���� 	��ȡ��ǰʱ��
��	  ���� 	��
�� �� ֵ��	��ǰ����ֵ
*******************************************************************************/
BF_INT32U Time_Get(void)
{
    return su_timer;
}

/*******************************************************************************
�� �� ���� void SysTick_Handler(void)
����˵���� SysTick�жϷ������
��   ����  ��
�� �� ֵ�� ��
*******************************************************************************/
void SysTick_Handler(void)
{
    su_timer %= TIME_MAX;	//������1000s
    su_timer++;
    Thread_RunCheck();
}


/*********************************************END OF FILE**********************/

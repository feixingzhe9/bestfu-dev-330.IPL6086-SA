/***************************Copyright BestFu 2014-05-14*************************
��	����	Insturct.c
˵	����	ϵͳһЩ����״̬�ģ�ָʾ����
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.30
�ޡ��ģ�	����
*******************************************************************************/
#include "stm32l1xx.h"
#include "lib_type.h"
#include "lib_config.h"
#include "gpio.h"
#include "Thread.h"
#include "SysHard.h"
#include "Instruct.h"

//Private function declaration
#if RUN_INSTRUCT
static void InstructLED_OFF(void);
#endif

/*******************************************************************************
�� �� ����	Instruct_Init
����˵���� 	ָʾ��غ�����ʼ��
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void Instruct_Init(void)
{
#if RUN_INSTRUCT
    GPIO_Config(RUN_PORT, RUN_PIN, OUT_PP_2M);
    Thread_Login(MANY, 16, 250, &Instruct_Run);
#endif
}

#if RUN_INSTRUCT
/*******************************************************************************
�� �� ����	Instruct
����˵���� 	�豸��Ӧָʾ����
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void Instruct(void)
{
    Thread_Login(MANY, 20, 100, &Instruct_Run);
}
#endif

/*******************************************************************************
�� �� ����	Instruct_Run
����˵���� 	ָʾϵͳ�ϵ�����
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void Instruct_Run(void)
{
#if RUN_INSTRUCT
    RUN_ON;
    Thread_Login(ONCEDELAY, 0, 2, &InstructLED_OFF);
#endif
}

#if RUN_INSTRUCT
/*******************************************************************************
�� �� ����	RunLED_OFF
����˵���� 	����ָʾ�ƹر�
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
static void InstructLED_OFF(void)
{
    RUN_OFF;
}
#endif

/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright xxxxx 2015-12-30**************************
**  ��    ����  usart.c
**  ��    �ܣ�  ���ڵײ���������
**  ��    �룺  MDK-ARM Professional Versoin: 5.12.0.0
**  ��    ����  V1.0
**  ��    д��  Jay
**  �������ڣ�  2015-12-29
**  �޸����ڣ�  ��
**  ˵    ����  ����
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "fifo.h"
#include "PivotalDataType.h"
#include "FifoDataProc.h"
#include "FaultManage.h"
#include "PhyMsgProc.h"
#include "stm32l1xx.h"
#include "sys.h"
#include "system_stm32l1xx.h"
#include "usart.h"

#if (USART_INTERFACE_EN > 0u )

//Private function declaration
static BF_INT08U USART_Write(BF_INT08U c);

/*******************************************************************************
�� �� ����	USART_Init
����˵����	���ڳ�ʼ��
��	  ����	pclk2�� ʱ��Ƶ�ʣ���λM��
			bound�� ͨ�Ų�����
�� �� ֵ��	��
*******************************************************************************/
void USART_Init(BF_INT32U bound)
{
    RCC_ClocksTypeDef RCC_ClocksStatus;

    RCC_GetClocksFreq(&RCC_ClocksStatus);//���ô��ڲ�����
    RCC->APB2RSTR |= 1<<14;   	//��λ����1
    RCC->APB2RSTR &= ~(1<<14);	//ֹͣ��λ

    RCC->AHBENR  |= 1<<0;   	//ʹ��PORTA��ʱ��
    RCC->APB2ENR |= 1<<14;  	//ʹ�ܴ���ʱ��

    GPIOA->MODER &= 0xFFC3FFFF; //PA9,PA10�ܽŸ���
    GPIOA->MODER |= 0x00280000;

    GPIOA->OTYPER&= 0xFFFFF9FF; //���ùܽ��������

    GPIOA->OSPEEDR&= 0xFFC3FFFF; //���ùܽ��ٶ�40M
    GPIOA->OSPEEDR|= 0x003C0000;

    GPIOA->PUPDR  &= 0xFFC3FFFF; //�ܽŲ�������

    GPIOA->AFR[1] &= 0xFFFFFFFF; //���ø��ù���
    GPIOA->AFR[1] |= 0x00000770;

    //����������
    USART1->BRR = (RCC_ClocksStatus.PCLK2_Frequency)/(bound); // ����������
    USART1->CR1 |= 0X200C;  	//1λֹͣ,��У��λ.

    USART1->CR1 |= 1<<8;    	//PE�ж�ʹ��
    USART1->CR1 |= 1<<5;    	//���ջ������ǿ��ж�ʹ��
    MY_NVIC_Init(3, 3, USART1_IRQn, 2);//��2��������ȼ�
}

/*******************************************************************************
�� �� ����	USART_Send
����˵����	�������ݷ���
��	  ����	*data: Ҫ���͵�������
			len :	���ݳ���
�� �� ֵ��	���ͽ�� TRUE/FALSE
*******************************************************************************/
BF_INT08U USART_Send(BF_INT08U *data, BF_INT08U len, BF_INT08U level)
{
    while (len--)
    {
        if (!USART_Write(*data++))
        {
            return FALSE;;
        }
    }
    return TRUE;
}

/*******************************************************************************
�� �� ����	UART_WriteBit
����˵����	����д��һ���ֽ�����
��	  ����	ch:	Ҫд�������
�� �� ֵ��	д����TRUE/FALSE
*******************************************************************************/
static BF_INT08U USART_Write(BF_INT08U c)
{
    BF_INT32U  t;

    for (t = 0; ((USART1->SR&0X40) == 0)&&(t < 20000); t++)   //�ȴ����ݷ������
    {}

    if (t < 20000)  //δ��ʱ
    {
        USART1->DR = (BF_INT08U)c;
        return TRUE;
    }

    return FALSE;
}

/*******************************************************************************
�� �� ����	USART1_IRQHandler
����˵����	�����жϴ���
��	  ����	��
�� �� ֵ��	��
*******************************************************************************/
void USART1_IRQHandler(void)
{
    if(USART1->SR&(1<<5))//���յ�����
    {
        CPU_SR_ALLOC();
        ENTER_CRITICAL();
        fifo_putc(&phy_rcvUsartfifo, USART1->DR);
        EXIT_CRITICAL();
    }
    USART1->SR &=~(1<<5);
}
#endif
/***********************************END OF FILE***********************************/

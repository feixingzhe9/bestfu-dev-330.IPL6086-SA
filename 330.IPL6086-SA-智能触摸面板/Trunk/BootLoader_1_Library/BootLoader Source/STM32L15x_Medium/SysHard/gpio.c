/***************************Copyright bestfu 2015-12-30**************************
**  ��    ��: gpio.c
**  ��    �ܣ�stm32l15x GPIO����
**  ��    �룺Keil uVision5 V5.12
**  ��    ����V1.0.0
**  ��    д��Jay
**  �������ڣ�2015.12.29
**  �޸����ڣ���
**  ˵    ��������
*******************************************************************************/
#include "stm32l1xx.h"
#include "lib_type.h"
#include "gpio.h"

/*******************************************************************************
�� �� ����	void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode)
����˵���� 	����GPIO����״̬
��	  ���� 	GPIOx   : ��Ӧ��GPIO_A~GPIO_G
			GPIO_Pin: �ܽ�GPIO_Pin_0~GPIO_Pin_15
			mode    :  ����(IN/OUT/AF/AN)_����((IN_/ /UP|DOWN/FLOATING)
			(OUT/AF_(OP(��©)/PP(����)))_�ٶ�(400K/2M/10M/40M)
			(���������ͷ�ļ�)
�� �� ֵ��	��
˵    ��:   ��
*******************************************************************************/
void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode)
{
    BF_INT32U position = 0x00;
    BF_INT32U temp = 0x00;

    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    /*---------------------------����GPIOʱ��----------------------------------*/
    if(!(RCC->AHBENR & (1u << (GPIO_x))))
    {
        RCC->AHBENR |= (1u << (GPIO_x));
    }
    /*---------------------------- GPIO ģʽ���� --------------------------------*/
    /* ����GPIO�� */
    while (((GPIO_Pin) >> position) != 0)
    {
        if((GPIO_Pin) & ((BF_INT32U)1 << position))
        {
            /*--------------------- GPIO ģʽ���� ------------------------*/
            temp = GPIOx->MODER;
            temp &=~(GPIO_MODER_MODER0 << (position * 2));
            temp |= ((mode&(0x03)) << (position * 2));
            GPIOx->MODER = temp;
            if((0x01 == (mode&0x03))||(0x02 == (mode&0x03)))
            {
                /* ����GPIO���ٶ� */
                temp  = GPIOx->OSPEEDR;
                temp &=~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2));
                temp |= (((mode >> 3)&0x03) << (position * 2));
                GPIOx->OSPEEDR = temp;

                /* ����GPIO��������� */
                temp = GPIOx->OTYPER;
                temp &=~(GPIO_OTYPER_OT_0 << position);
                temp |= (((mode >> 2)&0x01) << position);
                GPIOx->OTYPER = temp;
            }
            /* ���ǰIO������������������ */
            temp = GPIOx->PUPDR;
            temp &=~(GPIO_PUPDR_PUPDR0 << (position * 2));
            temp |= ((mode >> 5) << (position * 2));
            GPIOx->PUPDR = temp;
        }
        position++;
    }
}

/*******************************************************************************
�� �� ����	void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	�����������״̬
��	  ���� 	GPIOx   : ��Ӧ��GPIO_A~GPIO_G
			GPIO_Pin: �ܽ�GPIO_Pin_0~GPIO_Pin_15
�� �� ֵ��	��
ע    ��:   GPIO_Pin = 0xffff; ʱ���Դ˶˿ڵ��������Ž�����1
*******************************************************************************/
void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    GPIOx->BSRRL = (BF_INT16U)GPIO_Pin;
}

/*******************************************************************************
�� �� ����	void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	��λ�������״̬
��	  ���� 	GPIOx   : ��Ӧ��GPIO_A~GPIO_G
			GPIO_Pin: �ܽ�GPIO_Pin_0~GPIO_Pin_15
�� �� ֵ��	��
ע    ��:   ��GPIO_Pin= 0xffff; ʱ���Դ˶˿ڵ��������Ž�����0
*******************************************************************************/
void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    GPIOx->BSRRH = (BF_INT16U)GPIO_Pin;
}

/*******************************************************************************
�� �� ����	void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U pin)
����˵���� 	ȡ���������״̬(reverse)
��	  ���� 	GPIOx   : ��Ӧ��GPIO_A~GPIO_G
			GPIO_Pin: �ܽ�GPIO_Pin_0~GPIO_Pin_15
�� �� ֵ��	��
ע    ��:   ��pin = 0xffff; ʱ���Դ˶˿ڵ��������Ž���ȡ��
*******************************************************************************/
void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    GPIOx->ODR ^= GPIO_Pin;
}

/*******************************************************************************
�� �� ����	BF_INT08U GPIO_ReadOutputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	��ȡ��Ӧ�˿ڸ����ŵ����״ֵ̬
��	  ���� 	GPIOx: ��Ӧ��GPIO_(A~G)
			pin	 : �ܽ�pin_(0~15)
�� �� ֵ��	�����޷���8λ����,�������ű�������Ϊ1,����Ϊ0;
ע    ��: 	��
*******************************************************************************/
BF_INT08U GPIO_ReadOutputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    BF_INT08U bitstatus = 0x00;
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));

    if ((GPIOx->ODR & GPIO_Pin) != (BF_INT32U)RESET)
    {
        bitstatus = (BF_INT08U)SET;
    }
    else
    {
        bitstatus = (BF_INT08U)RESET;
    }
    return bitstatus;
}

/*******************************************************************************
�� �� ����	BF_INT08U GPIO_ReadInputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	��ȡ��Ӧ�˿ڸ����ŵ�����״ֵ̬
��	  ���� 	GPIOx: ��Ӧ��GPIO_(A~G)
			pin	 : �ܽ�pin_(0~15)
�� �� ֵ��	�����޷���8λ����,�������ű�������Ϊ1,����Ϊ0;
ע    ��: 	��
*******************************************************************************/
BF_INT08U GPIO_ReadInputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    BF_INT08U bitstatus = 0x00;
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));

    if ((GPIOx->IDR & GPIO_Pin) != (BF_INT32U)RESET)
    {
        bitstatus = (BF_INT08U)SET;
    }
    else
    {
        bitstatus = (BF_INT08U)RESET;
    }
    return bitstatus;
}

/*******************************************************************************
�� �� ����	void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
����˵���� 	������ӦGPIO����Ӧ�ܽ�
��	  ���� 	GPIOx: ��Ӧ��GPIO_(A~G)
			pin	 : �ܽ�pin_(0~15)
�� �� ֵ��	��
ע    ��: 	��д�����ڼ�,���ܸ���LCKR[15:0] ��ֵ;��LOCK �� �� Ӧ ��
			��ĳ���˿�λ��,��ִ����һ�θ�λ֮ǰ,���޷��Ըö˿�λ��ֵ�����޸�;
*******************************************************************************/
void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    volatile BF_INT32U tmp = 0x00010000; //ʹ�ܶ˿�λ��������
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));

    tmp |= GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LCKR = tmp;
    /* Reset LCKK bit */
    GPIOx->LCKR =  GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LCKR = tmp;
    /* Read LCKK bit*/
    tmp = GPIOx->LCKR;
    /* Read LCKK bit*/
    tmp = GPIOx->LCKR;
}

/*******************************************************************************
�� �� ����	void GPIO_PinRemapConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin, BF_INT08U AF)
����˵���� 	����GPIO����ӳ�书��
��	  ���� 	GPIO_x		: ��Ӧ��GPIO_(A~G)
			GPIO_Pin	: �ܽ�pin_(0~15)
			AF  		: GPIO_AF0_MCO ~ GPIO_AF15_EVENTOUT (������ο�ͷ�ļ�)
�� �� ֵ��	��
ע    ��: 	��
*******************************************************************************/
void GPIO_PinRemapConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin, BF_INT08U AF)
{
    BF_INT32U position = 0x00;
    BF_INT32U temp = 0x00;
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    while (((GPIO_Pin) >> position) != 0)
    {
        if((GPIO_Pin) & ((BF_INT32U)1 << position))
        {
            temp = GPIOx->MODER >> (2 * position);
            if(0x02 == (temp&0x03))
            {
                /* Identify AFRL or AFRH register based on IO position*/
                temp = GPIOx->AFR[position >> 3];
                temp&=~((BF_INT32U)0xF << ((BF_INT32U)(position & (BF_INT32U)0x07) * 4));
                temp|= (BF_INT32U)(AF) << (((BF_INT32U)position & (BF_INT32U)0x07) * 4);
                GPIOx->AFR[position >> 3] = temp;
            }
        }
        position++;
    }
}

/********************************End of File***********************************/

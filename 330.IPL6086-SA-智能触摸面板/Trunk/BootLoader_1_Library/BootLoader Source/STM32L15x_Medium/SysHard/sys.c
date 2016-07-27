/***************************Copyright xxxxx 2015-12-29*************************
**  ��	  ����sys.c
**  ˵	  ����ϵͳ���ô�����
**  ��	  �룺uVision V5.12.0.0
**  ��	  ����V1.0
**  ��	  д��Jay
**  �������ڣ�2015/12/29
**  �޸����ڣ���
**  ˵    ��������
*******************************************************************************/
#include "stm32l1xx.h"
#include "lib_type.h"
#include "lib_config.h"
#include "sys.h"

/*******************************************************************************
�� �� ��:  	void MY_NVIC_SetVectorTable(BF_INT32U NVIC_VectTab, BF_INT32U Offset)
����˵��:  	����������ƫ�Ƶ�ַ
��    ��:  	NVIC_VectTab:��ַ
			Offset:ƫ����
�� �� ֵ:  	��
˵    ��:	��
*******************************************************************************/
void MY_NVIC_SetVectorTable(BF_INT32U NVIC_VectTab, BF_INT32U Offset)
{
    SCB->VTOR = NVIC_VectTab|(Offset & (BF_INT32U)0x1FFFFF80);	//����NVIC��������ƫ�ƼĴ���
}

/*******************************************************************************
�� �� ��:  	void MY_NVIC_PriorityGroupConfig(BF_INT08U NVIC_Group)
����˵��:  	����NVIC����
��    ��:  	NVIC_Group:NVIC���� 0~4 �ܹ�5��,ȡֵNVIC_PriorityGroup_0~NVIC_PriorityGroup_4
�� �� ֵ:  	��
˵    ��:	��
*******************************************************************************/
void MY_NVIC_PriorityGroupConfig(BF_INT08U NVIC_Group)
{
    BF_INT32U temp,temp1;
    temp1   = (~NVIC_Group) & 0x07;	//ȡ����λ
    temp1 <<= 8;
    temp    = SCB->AIRCR;  			//��ȡ��ǰ������
    temp   &= 0X0000F8FF; 			//�����ǰ����
    temp   |= 0X05FA0000; 			//д��Կ��
    temp   |= temp1;
    SCB->AIRCR=temp;  				//���÷���
}

/*******************************************************************************
�� �� ��:  	void MY_NVIC_Init(BF_INT08U NVIC_PreemptionPriority,BF_INT08U NVIC_SubPriority,BF_INT08U NVIC_Channel,BF_INT08U NVIC_Group)
����˵��:  	����NVIC
��    ��:  	NVIC_PreemptionPriority:��ռ���ȼ�
			NVIC_SubPriority       :��Ӧ���ȼ�
			NVIC_Channel           :�жϱ��
			NVIC_Group             :�жϷ��� NVIC_PriorityGroup_0~NVIC_PriorityGroup_4
�� �� ֵ:  	��
˵    ��:	�黮��:
			��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
			��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
			��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
			��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
			��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
			NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����
*******************************************************************************/
void MY_NVIC_Init(BF_INT08U NVIC_PreemptionPriority,BF_INT08U NVIC_SubPriority,BF_INT08U NVIC_IRQChannel,BF_INT08U NVIC_Group)
{
    BF_INT32U tmppriority = 0x00u;

    /* Compute the Corresponding IRQ Priority --------------------------------*/
    MY_NVIC_PriorityGroupConfig(NVIC_Group);					//���÷���
    tmppriority	= NVIC_PreemptionPriority << (0x4 - NVIC_Group);//��ռ���ȼ�����
    tmppriority|= NVIC_SubPriority & (0x0f >> NVIC_Group);		//��Ӧ���ȼ�����
    tmppriority <<= 0x04;
    NVIC->IP[NVIC_IRQChannel] = tmppriority;
    /* Enable the Selected IRQ Channels --------------------------------------*/
    NVIC->ISER[NVIC_IRQChannel >> 0x05] =
        (BF_INT32U)0x01 << (NVIC_IRQChannel & (BF_INT08U)0x1F); //ʹ���ж�λ(Ҫ����Ļ�,�෴������OK)
}

/*******************************************************************************
�� �� ��:  	void EXTI_NVIC_Config(BF_INT08U GPIOx,BF_INT08U BITx,BF_INT08U TRIM)
����˵��:  	�ⲿ�ж����ú���
��    ��:  	GPIOx:ȡֵEXTI_PortSourceGPIOA ~ EXTI_PortSourceGPIOI
			BITx:ȡֵEXTI_PinSource0 ~ EXTI_PinSource15
			TRIM:ȡֵ(EXTI_Trigger_Falling,EXTI_Trigger_Rising,EXTI_Trigger_Rising_Falling)
�� �� ֵ:  	��
˵    ��:	�ú���һ��ֻ������1��IO��,���IO��,���ε���
			�ú������Զ�������Ӧ�ж�,�Լ�������
*******************************************************************************/
void EXTI_NVIC_Config(BF_INT08U GPIOx,BF_INT08U BITx,BF_INT08U TRIM)
{
    BF_INT08U EXTADDR;
    BF_INT08U EXTOFFSET;
    EXTADDR=BITx/4;								//�õ��жϼĴ�����ı��
    EXTOFFSET=(BITx%4)*4;
    RCC->APB2ENR|=0x01;							//ʹ��io����ʱ��

    SYSCFG->EXTICR[EXTADDR] &=~(0x000F<<EXTOFFSET);//���ԭ��������
    SYSCFG->EXTICR[EXTADDR] |= GPIOx<<EXTOFFSET;

    EXTI->IMR|=1<<BITx;							//����line BITx�ϵ��ж�
    //EXTI->EMR|=1<<BITx;//������line BITx�ϵ��¼� (������������,��Ӳ�����ǿ��Ե�,��������������ʱ���޷������ж�!)
    if(TRIM&0x01)EXTI->FTSR|=1<<BITx;			//line BITx���¼��½��ش���
    if(TRIM&0x02)EXTI->RTSR|=1<<BITx;			//line BITx���¼��������ش���
}

/*******************************************************************
�� �� ����	Jump_To_Code
����˵����	��ת����ַ��Ӧ�ĳ���������
��	  ����	addr:	�Դ���洢���׵�ַ
�� �� ֵ��	��
*******************************************************************/
void JumpToCode(BF_INT32U ApplicationAddress)
{
    volatile BF_INT32U* p = (volatile BF_INT32U*)ApplicationAddress;
    if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
    {
        Set_MSP(*p);		//ȡ����ʼ��MSP��ֵ
        ((void (*)(void))(*(p + 1)))();
    }
}
/**********************************End of File*********************************/

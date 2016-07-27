/***************************Copyright BestFu 2016-04-13*************************
��	����	main.c
˵	����	ϵͳ������
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2015-04-13 
�ޡ��ģ�	����
*******************************************************************************/
#include "main.h"

static void JumpToCode(unsigned int addr);
/*******************************************************************************
����  ��:  	main
����˵��:	������
��    ��:  	��
����  ֵ:  	��
*******************************************************************************/
int main(void)
{
	Bootloader0Info_t* pboot0Info = (Bootloader0Info_t*)(DevPartTab->boot0_flag_start_addr);
	JumpToCode((BOOTLOADER_0_JUMP_FLAG == pboot0Info->bootloader_0_flag)?(DevPartTab->user_app_start_addr):(DevPartTab->boot1_start_addr));
	for(;;);
}

/*******************************************************************
�� �� ����	 __set_MSP
����˵����	���ö�ջָ���ַ
��	  ����	��
�� �� ֵ��	��
*******************************************************************/
__asm void __set_MSP(unsigned int mainStackPointer)
{
	msr msp, r0
	bx lr
}

/*******************************************************************
�� �� ����	Jump_To_Code
����˵����	��ת����ַ��Ӧ�ĳ���������
��	  ����	addr:	�Դ���洢���׵�ַ
�� �� ֵ��	��
*******************************************************************/
static void JumpToCode(unsigned int addr)
{
	volatile unsigned int* p = (volatile unsigned int*)addr;
	__set_MSP(*p);		//ȡ����ʼ��MSP��ֵ
	((void (*)(void))(*(p + 1)))();
	
}
/**************************Copyright BestFu 2016-04-13*************************/

/***************************Copyright BestFu 2014-05-14*************************
��	����	WDG.c
˵	����	���Ź���غ���
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.05.14 
�ޡ��ģ�	����
*******************************************************************************/
#include "GPIO.h"
#include "UnitCfg.h"
#include "Thread.h"
#include "wdg.h"

/*******************************************************************************
  ������:  	WDG_Init	
����˵��:  	���Ź���ʼ������
	����:  	prer: 	��Ƶ��:0~7(ֻ�е�3λ��Ч!)	
			rlr:	��װ�ؼĴ���ֵ:(0~2048)
  ����ֵ:  	��
	ע�⣺	ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms)	
			���ⲿ����HardWDG��������Ӳ�����Ź���ι�����ܣ�����ι��ʱ�俴Ӳ������
*******************************************************************************/
void WDG_Init(u8 prer,u16 rlr) 
{
#if (INTERNAL_IWDG_EN > 0u)
	IWDG->KR  = 0X5555;		//ʹ�ܶ�IWDG->PR��IWDG->RLR��д		 										  
  	IWDG->PR  = prer;  		//���÷�Ƶϵ��   
  	IWDG->RLR = rlr;  		//�Ӽ��ؼĴ��� IWDG->RLR  
	IWDG->KR  = 0XAAAA;		//reload											   
  	IWDG->KR  = 0XCCCC;		//ʹ�ܿ��Ź�	
#endif	
#if (EXTERNAL_WDG_EN > 0u)
	GPIOx_Cfg(WDG_PORT, WDG_PIN, OUT_PP_2M);
#endif
#if ((INTERNAL_IWDG_EN > 0u)||(EXTERNAL_WDG_EN > 0u))	
	Thread_Login(FOREVER, 0, 800, &WDG_Feed);	//ע��ι��
#endif
}

/*******************************************************************************
�� �� ����	WDG_Feed
����˵���� 	ι������
��	  ���� 	��
�� �� ֵ��	��
*******************************************************************************/
void WDG_Feed(void)
{
#if (INTERNAL_IWDG_EN > 0u)
	IWDG->KR=0XAAAA;	//ι�����
#endif	
#if (EXTERNAL_WDG_EN > 0u)
	GPIOx_Rvrs(WDG_PORT, WDG_PIN);//ιӲ����
#endif

}

/*******************************************************************************
�� �� ����	WDG_FeedOFF
����˵���� 	�ر�ι��
��	  ���� 	��
�� �� ֵ��	��
ע	  ��:	���ñ�������ϵͳ�������Ź�����λ��������ȡ��
*******************************************************************************/
void WDG_FeedOFF(void)
{
#if ((INTERNAL_IWDG_EN > 0u)||(EXTERNAL_WDG_EN > 0u))
	Thread_Logout(&WDG_Feed);
#endif
}

/*******************************************************************************
�� �� ��:  	HardWDGTest_Attr
����˵��:  	Ӳ�����Ź�����
��    ��:  	data[0]: Ŀ�굥Ԫ
			data[1]: ��������
			rLen:	���ز�������
			rpara:	���ز�������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
MsgResult_t HardWDGTest_Attr(u8 *data)
{
#if (EXTERNAL_WDG_EN > 0u)
	GPIOx_Cfg(WDG_PORT, WDG_PIN, IN_FLOATING);
	return COMPLETE;
#else
	return CMD_EXE_ERR;
#endif
}

/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2016-03-09*************************
��	����    TimeHandle.h
˵	����    ʱ�䴦����ͷ�ļ�
��	�룺    Keil uVision V5.12.0.0
��	����    v1.0
��	д��    jay
��	�ڣ�    2016-03-09
��  ��:     ��
*******************************************************************************/
#ifndef __TIMEHANDLE_H
#define __TIMEHANDLE_H

#include "DlyAttrProcess.h"

#if ((ALINKTIME_EN > 0u) || (SCENCETIME_EN > 0u))
/*******************************�������ݽṹ**********************************/
typedef union
{
	u16 time;						//��ʱ����ʱ�����
	struct
	{
		u16 second	:	6;
		u16 minute	:	6;
		u16 hour	:	2;
		u16 flag 	:	2;
	}SecAnalyze;
	
	struct
	{
		u16 minute	:	6;
		u16 hour	:	5;
		u16 day		:	3;
		u16 flag 	:	2;
	}MinAnalyze;
	
	struct
	{	
		u16 hour	:	5;
		u16 day		:	9;
		u16 flag 	:	2;
	}HourAnalyze;
	
	struct
	{	
		u16 msec	:	4;
		u16 second	:	6;
		u16 minute	:	4;
		u16 flag 	:	2;
	}MsecAnalyze;
}TimeAnalyze_t;

/**********************************��������**********************************/
u32 TransformTimePara(u16* time);
u8 CheckAllTimeParaIsOK(DlyTimeData_t* time,u8 cnt);
u16 CheckTimeParaIsZero(u16 timepara);

#endif
#endif 
/******************************End of File************************************/

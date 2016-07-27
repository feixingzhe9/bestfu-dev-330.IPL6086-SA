/***************************Copyright BestFu 2014-05-14*************************
��	����	LinksimplestTime.h
˵	����	�����������ʱԴ�ļ�
��	�룺	Keil uVision4 V5.12.0.0
��	����	v1.0
��	д��	Joey
��	�ڣ�	2016.1.29 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef _LinkSimplestTime_
#define _LinkSimplestTime_

#include "linkall.h"

#if(ALINKTIME_EN > 0)
//typedef struct //��ʱʱ��ṹ��,5Bytes
//{
//	u8   delaytime[2];  //��ǰ��ʱʱ��
//	u8   keeptime[2];   //��ǰ����ʱ��
//	u8   repeat;        //������Ϣ�ظ�����
//}DlyTimeDat_t;

typedef struct //�������ṹ,32Bytes
{
	u8   UseFlag;      //ʹ�ñ�־
	u8   LinkNum;      //������
	DlyTimeData_t  Time;
}ALinkTimeItem;

typedef struct
{
	ALinkTimeItem   Item[MAX_ALINKTIMEITEM];  //������
	u8       Cnt;           //����
}ALinkTimeTable;

__inline u32 GetALinkTimeInited(void)
{
	return ALINK_TIME_START_ADDR;
}

__inline u32 GetALinkTimeStartAddr(void)
{
	return ALINK_TIME_START_ADDR + 1;
}

void ALinkTimeInit(void);
void AllALinkTimeClr(void);
u8 QueryALinkTime(u8 num);
u8 DelALinkTime(u8 num);
u8 WriteALinkTime(u8 num, u8 linknum, u8 addflag, u8 *data);
u8 ReadALinkTime(u8 num , u8 *len, u8 *data);
u8 WriteALinkTimeToTable(u8 num, u8 linknum, u8 addflag, u8 *data);
u8 DelALinkTimeFromTable(u8 num);
u8 AddALinkTimeCmdX1(u8 num, u8 *data);

#endif
#endif
/**************************Copyright BestFu 2014-05-14*************************/

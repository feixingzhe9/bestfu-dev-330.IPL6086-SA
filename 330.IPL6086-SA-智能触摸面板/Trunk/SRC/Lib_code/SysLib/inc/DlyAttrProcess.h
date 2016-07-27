/***************************Copyright BestFu 2016-01-28*************************
��	����    DlyAttrProcess.h
˵	����    ϵͳ����˳��ִ�д�����ͷ�ļ�
��	�룺    Keil uVision V5.12.0.0
��	����    v1.0
��	д��    jay
��	�ڣ�    2016-01-28
��  ��:     ��
*******************************************************************************/
#ifndef __DLYATTRPROCESS_H
#define __DLYATTRPROCESS_H

#include "BF_type.h"
#include "UnitCfg.h"
#include "linkcommon.h"
#include "UserData.h"

/*****************************�ؼ�������**************************************/
#if (SCENCETIME_EN > 0u)
	#define  DALAY_EXECUTE_EN	(1u)			//����ִ�й���
#else
	#define  DALAY_EXECUTE_EN	(0u)			//����ִ�й���
#endif

#define  TOTAL_Block_NUM        (32)			//�ڴ�������
#define  TRAVERCYCLE  			(50)			//��ʱ����ı�������

#define  DELAY_PRECISION        (100u)    		//��ʱ����100ms
#define  KEEP_PRECISION         (100u)     		//��ʱ����100ms

#define  UNIT_PROPERTY_RELATE	((u8)0xFF)		//ȫ��Ԫ���Թ���


/*******************************�������ݽṹ**********************************/
#if ((ALINKTIME_EN > 0u) || (SCENCETIME_EN > 0u))
#pragma pack(1)
typedef struct									//��ʱʱ��ṹ��,5Bytes
{
	u8 delaytime[2];							//��ǰ��ʱʱ��
	u8 keeptime[2]; 							//��ǰ����ʱ��
	u8 repeat;		   							//������Ϣ�ظ�����
}DlyTimeData_t;

#pragma pack()
#endif

#if (DALAY_EXECUTE_EN > 0u)
#pragma pack(1)
typedef struct									//��ʱִ�����������ݽṹ
{
	u8 unit;									//��Ԫ��
	u8 Cmd;  									//�����
	u8 len;  									//��������
	u8 data[13];								//����������
	u8 remaindelaytime[4];						//ʣ����ʱʱ��ֵ
	u8 remainkeeptime[4]; 						//ʣ�ౣ��ʱ��ֵ
	u8 delaytaskexeflag;						//��ʱ����ִ�б��
	u8 keeptaskexeflag;							//��������ִ�б��
	DlyTimeData_t DlyTimeData;		   			//��ʱʱ��ṹ��
	u8 unitAttrRelate;							//������Ԫ������֮��Ĺ�����ϵ,ע��0��Ϊ����,û�й�������
}DlyExeData_t;

typedef struct									//��ʱִ�л���ṹ
{
	DlyExeData_t DlyExeData[TOTAL_Block_NUM];	//��ʱִ�е�����
	u8 cnt;										//��ʾ��ǰ��Ч�ڵ����
}DlyExeNode_t;

#pragma pack()

typedef enum									//��ʱ��Ϣִ�����Ը�������
{
	SAME_MSG_DEL = 0,							//��ͬ��Ԫ������Ϣ����
	RELATE_MSG_DEL								//��ͬ����������Ϣ����
}DlyMsgDelType_t;

/**********************************��������**********************************/
u8 DlyMsgFilterProcess(UnitPara_t* para,DlyMsgDelType_t active);
u8 CreateBufferNode(u8 *data);
u8 DeleteDlyAssMsg(u8 unit,u8 Cmd);

#endif
#endif 
/******************************End of File************************************/

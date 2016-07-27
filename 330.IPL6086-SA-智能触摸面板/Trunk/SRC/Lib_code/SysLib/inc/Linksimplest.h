/***************************Copyright BestFu 2014-05-14*************************
��	����	Linksimplest.h
˵	����	�������Դ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Joey
��	�ڣ�	2013.7.22 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef _LinkSimplest_
#define _LinkSimplest_

#include "linkall.h"

#define    ALINK_SRC_VALUE_EN  (0)   // ��ʱΪ�ĺ�һ��ͨ�ø�֪��ֵ����
#define    LINKERROR           (255)
#define    CLINKEXED           (1)

#if(ALINK_EN > 0)
typedef struct //�������ṹ,32Bytes
{
	u8   UseFlag;      //ʹ�ñ�־
	u8   LinkNum;      //������
	
	u8   SUnitID;      //Դ��ԪID��
	u8   SPropID;      //Դ����ID��
	u8   SCondition;   //Դ����
	u8   SValue[4];    //Դֵ

	u8   DDevID[4];    //Ŀ���豸ID
	u8   DType;        //Ŀ������
	u8   DAddrT[2];    //Ŀ�����͵�ַ�룬���ڵ�ʱ����
	u8   DMsgType;     //��Ϣ�����룬�����Ƿ���Ҫ��Ӧ
	u8   UnitID;       //Ŀ�굥ԪID
	u8   DestCmd;      //Ŀ������
	u8   DestLen;      //Ŀ�곤��
}ALinkHead;

typedef struct //�������ṹ,33Bytes
{
	u8   UseFlag;      //ʹ�ñ�־
	u8   LinkNum;      //������
	
	u8   SUnitID;      //Դ��ԪID��
	u8   SPropID;      //Դ����ID��
	u8   SCondition;   //Դ����
	u8   SValue[4];    //Դֵ
	
	u8   DDevID[4];    //Ŀ���豸ID
	u8   DType;        //Ŀ������
	u8   DAddrT[2];    //Ŀ�����͵�ַ�룬���ڵ�ʱ����
	u8   DMsgType;     //��Ϣ�����룬�����Ƿ���Ҫ��Ӧ
	u8   UnitID;       //Ŀ�굥ԪID
	u8   DestCmd;      //Ŀ������
	u8   DestLen;      //Ŀ�곤��
	u8   data[13];     //Ŀ�����
}ALinkItem;

typedef struct
{
	ALinkItem   linkitem[MAX_ALINKITEM];  //������
	u8       Cnt;           //����
}ALinkTable;

#if (ALINK_SRC_VALUE_EN > 0)
typedef struct
{
	u8  LinkNum;
	u8  Value;
	u8  FirstFlag ; //Ϊ���ж��Ƿ�Ϊ��һ�α���
	int OldValue;
}ALinkVItem;

typedef struct
{
	ALinkVItem LinkV[MAX_ALINKITEM];
	u8 cnt;
}ALinkVTable;  //������ֵ��Դ�����������������Ƿ�ִ�й�

extern ALinkVTable  ALinkV;
#endif


extern ALinkTable   ALink; 
void ALinkInit(void);
void ALinkClr(void);
u8 CheckALinkFull(void);
u8 ChkALinkIsExist(u8 *linknum, u8 *data);
u8 GetALinkNum(u8 *linknum);
u8 QueryALink(u8 num);
u8 ReadALink(u8 num , u8 *len, u8 *data);
u8 WriteALink(u8 num, u8 linknum, u8 addflag, u8 *data);
u8 DelALink(u8 num);
u8 DelLinkForUnit(u8 unitnum);
u8 CheckALinkValueAndExe(u8 *data, u8 linknum, int newsamp);

void ALinkVInit(void);
u8 FindALinkValueNum(u8 linknum);
u8 GetALinkValue(u8 linknum);
u8 AddALinkValue(u8 linknum);
u8 DelALinkValue(u8 linknum);

__inline u32 GetALinkInited(void)
{
	return ALINK_START_ADDR;
}

__inline u32 GetALinkStartAddr(void)
{
	return ALINK_START_ADDR + 1;
}

#endif
#endif
/**************************Copyright BestFu 2014-05-14*************************/

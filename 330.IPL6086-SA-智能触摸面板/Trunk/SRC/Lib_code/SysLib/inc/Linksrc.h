/***************************Copyright BestFu 2014-05-14*************************
��	����	Linksrc.h
˵	����	Դ��ִ����ں����������ĳ�ʼ������Դ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Joey
��	�ڣ�	2013.7.22 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef    _linksrc_h_
#define    _linksrc_h_

#include "Linkall.h"


//������condition
typedef enum
{
	CLARGE           = 0x01,            //����
	CLARGEOREQUAL    = 0x02,            //���ڻ��ߵ���
	CSMALL           = 0x03,            //С��
	CSMALLOREQUAL    = 0x04,            //С�ڻ��ߵ���
	CEQUAL           = 0x05,            //����
	CNOTEQUAL        = 0x06,            //������
	CINRANGE         = 0x07,            //��Χ��
	COUTRANGE        = 0x08             //��Χ��
}CONDITION;

#define LINKSTD            (0x00)     //��������
#define LINKREV            (0x01)     //����������ֻ���ڸ��������е�Դ��

//Դ����
#define    TBIT              0x01            //λ�ͣ�ֻ��0��1
#define    TINT8S            0x02            //�������ַ��ͣ���Χ-128~127
#define    Tu8            	 0x03            //�޷����ַ��ͣ���Χ0~255
#define    TINT16S           0x04            //�����Ŷ����ͣ���Χ-32768~32767
#define    TINT16U           0x05            //�޷��Ŷ����ͣ���Χ0~65535
#define    TINT32S           0x06            //�����Ŷ����ͣ���Χ-21��~21��
#define    TINT32U           0x07            //�޷��Ŷ����ͣ���Χ0~42��
#define    TFP32             0x08            //�������͸�������
#define    TFP64             0x09            //˫�����͸�������

#define    MAX_SRC_OLD_CNT     10            //��Ҫ������һ�β���ֵ�ĸ���

//typedef struct
//{
//	u8  UnitID;
//	u8  PropID;
//	int    Value;
//}SrcOldValItem;

//typedef struct
//{
//	SrcOldValItem  src[MAX_SRC_OLD_CNT];
//	u8  Cnt;
//}SrcOldValTable;

//extern SrcOldValTable SrcOld;

u8 ExeLinkPackage(u8 *data, u8 type, u8 RevFlag);
u8 LinkSrcExe(u8 sevent, u8 action, u8 type, int num);

void LinkInit(void);
void LinkClr(void);

u8 FindOldValueTableFull(u8 unitID, u8 propID);
u8 FindOldValueTableItem(u8 unitID, u8 propID);
u8 AddOldValueTableItem(u8 unitID, u8 propID);

u8 CmpSampAndLinkValue(u8 Cndt, int Lvtmp1, int Lvtmp2, int Svtmp);
u8 GetConditionReverse(u8 SrcCndt);
u8 GetConditionOffset(u8 SrcCndt, int *sv1, int *sv2);

#endif
/**************************Copyright BestFu 2014-05-14*************************/

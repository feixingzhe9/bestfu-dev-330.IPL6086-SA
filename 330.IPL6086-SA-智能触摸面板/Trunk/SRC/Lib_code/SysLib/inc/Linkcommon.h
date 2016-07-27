/***************************Copyright BestFu 2014-05-14*************************
��	����	Linkcommon.h
˵	����	��������ͷ
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	����
��	�ڣ�	2014.06.30 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef LINKCOMMON_H
#define LINKCOMMON_H
#include "BF_type.h"

//��������
#define    ALINK_EN           (1)  //�����������ģ�飬1��ʾ�����������0��ʾ��
#define    MAX_ALINKITEM      (50) //֧�������������ÿ��32�ֽڣ���Ӳ����Դ��
#define    SLINK_SRC_VALUE_EN (0)  //Դ�Ƿ������ֵ��Դ��1��ʾ����ֵ��Դ��0��ʾ��
#define    SLINK_SRC_EVENT_EN (1)  //Դ�Ƿ�����¼���Դ��1��ʾ���¼���Դ��0��ʾ��
#define    OFFSET             (1)  //ƫ���������ڼ�����ֵʱ������ֵ��Χ������Ҫ����

#define    SIMPLESTLINK       (1)  //�������
#define    SIMPLELINK         (2)  //������
#define    COMPLEXLINK        (3)  //��������

#define    READLINK           (1)  // ��ȡ����
#define    WRITELINK          (2)  // д������
#define    DELLINK            (3)  // ɾ������
#define    QUERYLINK          (4)  // ��ѯ����
#define    MODIFYLINK         (5)  // �޸�����

#define    ALINKTIME_EN       (1)  // ����ʱʱ�������ʹ��
#define    TIMEFLAG           (1)  // ��ʾ��ʱ
#define    MAX_ALINKTIMEITEM  (50) // ��ʱ��Ŀ

typedef struct
{//20BYTES
	u8	DTLast[4];	  //��������ʱ��
	u8	DTDelay[4];   //����ִ����ʱʱ��
	u8	TimeTable;	  //ִ��ʱ���
	u8  UnitID;       //Ŀ�굥ԪID
	u8	DestCmd;	  //Ŀ������
	u8	DestLen;	  //Ŀ�곤��
	u8	data[10];	  //Ŀ�����
}DestTarget;

#define LINKSTD            (0x00)     //��������
#define LINKREV            (0x01)     //����������ֻ���ڸ��������е�Դ��
#define EXEERROR           (0xFF)     //����ʧ��
#define EEPROMDATAINITED   (0xA5)     //�ѳ�ʼ��

#define WCHAR(H,L)            (((H)<<8)+(L))
#define WBYTE(H,L)            (((H)<<4)+(L))
#define WSINT(H,HM,HL,L)      ((H<<12)+(HM<<8)+(HL<<4)+(L<<0))
#define LOWORD(d)             (d)
#define HIWORD(d)             (d>>8)
#define DWCHAR(H, HM, ML, L)  (u32)((H<<24)+(HM<<16)+(ML<<8)+(L<<0))

#endif
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2014-05-14*************************
��	����	LinkWRDC.h
˵	����	�����д�롢��ȡ��ɾ���Ͳ�ѯ����Դ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Joey
��	�ڣ�	2013.7.22 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef _LinkWRDC_
#define _LinkWRDC_

#include "linkall.h"

#define    SIMPLESTLINK       (1)  //�������
#define    SIMPLELINK         (2)  //������
#define    COMPLEXLINK        (3)  //��������

#define    READLINK           (1)  // ��ȡ����
#define    WRITELINK          (2)  // д������
#define    DELLINK            (3)  // ɾ������
#define    QUERYLINK          (4)  // ��ѯ����
#define    MODIFYLINK         (5)  // �޸�����

u8 CheckLinkFull(u8 type);
u8 QueryLink(u8 type, u8 num);
u8 QueryAllLinkFromTable(u8 type, u8 *len, u8 *data);
u8 ReadLinkFromTable(u8 type, u8 num, u8 *len, u8 *data);
u8 WriteLinkToTable(u8 type, u8 num, u8 linknum, u8 addflag, u8 *data);
u8 DelAllLinkFromTable(u8 type);
u8 DelLinkFromTable(u8 type, u8 num);
u8 ChkLinkIsExist(u8 type, u8 *linknum, u8 *data);
u8 GetLinkNum(u8 type, u8 *linknum);
u8 WriteLinkItem(u8 type, u8 *data);
u8 ReadLinkItem(u8 type, u8 num, u8 *len, u8 *data);
u8 QueryLinkItem(u8 type, u8 num, u8 *len, u8 *data);
u8 DelLinkItem(u8 type, u8 num);

u8 ModifyLinkItem(u8 type, u8 linknum, u8 *data);

#if (ALINKTIME_EN > 0)
u8 ModifyLinkItemEx(u8 type, u8 linknum, u8 *data);
u8 WriteLinkItemEx(u8 type, u8 *data);
u8 DelLinkItemEx(u8 type, u8 num);
#endif

#endif
/**************************Copyright BestFu 2014-05-14*************************/

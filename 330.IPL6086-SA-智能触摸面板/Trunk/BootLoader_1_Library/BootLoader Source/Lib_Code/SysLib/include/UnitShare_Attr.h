/***************************Copyright BestFu 2014-05-14*************************
��	����	UnitShare_Attr.h
˵	����	����Ԫ��������ʵ�ֺ���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014-04-08
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __UNITSHARE_ATTR_H
#define __UNITSHARE_ATTR_H


/*********************************�Զ����*************************************/
#define MAX_ATTR_NUM	    (200) //���Ԫ������
#define MAX_ATTRPARA_LEN	(10)  //������Բ�������

/*********************************��������*************************************/
MsgResult_t Set_ManyUnit_Attr(UnitPara_t *pData);
MsgResult_t Get_ManyUnit_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);
MsgResult_t Set_UnitChecking_Attr(UnitPara_t *data);

#endif //UnitShare_Attr.h
/**************************Copyright BestFu 2014-05-14*************************/

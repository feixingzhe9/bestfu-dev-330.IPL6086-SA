/***************************Copyright BestFu 2014-05-14*************************
��	����	MsgPackage.h
˵	����	�豸������Ϣ������غ���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.24 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __MSGPACKAGE_H
#define __MSGPACKAGE_H


/*********************************�Զ����*************************************/



/**********************************��������***********************************/  
void Msg_Upload(BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data);
void Msg_Send(ObjectType_t objecttype, BF_INT32U objectID, BF_INT16U actNum, BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data);
void Msg_UploadUnit(BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data);


#endif //MsgPackage.h end 
/**************************Copyright BestFu 2014-05-14*************************/

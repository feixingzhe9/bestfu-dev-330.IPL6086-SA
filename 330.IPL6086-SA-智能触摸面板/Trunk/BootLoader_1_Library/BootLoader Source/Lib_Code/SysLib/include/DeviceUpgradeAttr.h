/***************************Copyright BestFu 2014-05-14*************************
��	����	DeviceUpgradeAttr.h
˵	����	�豸��������ʵ��ͷ�ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2016-04-15
��  ��:     ����
*******************************************************************************/
#ifndef __DEVICE_UPGRADE_H   
#define __DEVICE_UPGRADE_H

/*********************************�Զ����*************************************/


/********************************���ݽṹ����**********************************/  

/**********************************��������***********************************/
MsgResult_t Set_IAPReady_Attr(UnitPara_t *pData);
MsgResult_t Get_IAPReady_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);
MsgResult_t Set_IAPPackSave_Attr(UnitPara_t *pData);

MsgResult_t Set_433Channel_Attr(UnitPara_t *pData);
MsgResult_t Get_433Channel_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);

#endif //DeviceUpgradeAttr.h
/**************************Copyright BestFu 2014-05-14*************************/


/***************************Copyright BestFu 2014-05-14*************************
��	����	FlaseAttr.h
˵	����	����ICFlash���ݲ���������Բ���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-11-22
��  ��:     ����
*******************************************************************************/
#ifndef __FLASHATTR_H
#define __FLASHATTR_H

#include "program.h"
#include "Updata.h" 
#include "UnitCfg.h"

//
MsgResult_t Set_IAPReady_Attr(UnitPara_t *data);
MsgResult_t Get_IAPReady_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_IAPPackSave_Attr(UnitPara_t *data);
MsgResult_t Set_433Channel_Attr(UnitPara_t *data);
MsgResult_t Get_433Channel_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);

#endif //IAP.h
/**************************Copyright BestFu 2014-05-14*************************/


/***************************Copyright BestFu ***********************************
**  ��    ����  Sign433Attr.h
**  ��    �ܣ�  433�ź�ǿ�Ȳ������Խӿڲ�
**  ��    �룺  Keil uVision5 V4.72
**  ��    ����  V1.1
**  ��    д��  Seven
**  �������ڣ�  2014.10.08
**  �޸����ڣ�  2014.10.08
**  ˵    ����  >>�������������� --- 433�ź�
*******************************************************************************/
#ifndef _SIGN433_ATTR_H_
#define _SIGN433_ATTR_H_

#include "UnitCfg.h"

extern MsgResult_t Set_Sign433_StartSendFrame_Attr(UnitPara_t *pData);
extern MsgResult_t Get_Sign433_StartSendFrame_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);

extern MsgResult_t Set_Sign433_StartRecFrame_Attr(UnitPara_t *pData);
extern MsgResult_t Get_Sign433_StartRecFrame_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);

extern MsgResult_t Set_Sign433_RecFrameStream_Attr(UnitPara_t *pData);

extern MsgResult_t Set_Sign433_TestState_Attr(UnitPara_t *pData);
extern MsgResult_t Get_Sign433_TestState_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);

extern MsgResult_t Get_Sign433_RcvData_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
#endif
/***************************Copyright BestFu **********************************/

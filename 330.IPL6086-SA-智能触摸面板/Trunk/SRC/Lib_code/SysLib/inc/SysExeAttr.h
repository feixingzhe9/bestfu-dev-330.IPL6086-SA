/***************************Copyright BestFu 2014-05-14*************************
��	����    SysExeAttr.h
˵	����    ϵͳ����������ִ�б����������ʵ��
��	�룺    Keil uVision4 V4.54.0.0
��	����    v2.0
��	д��    Unarty
��	�ڣ�    2014.06.26
�ޡ��ģ�	����
*******************************************************************************/
#ifndef	__SYS_EXE_ATTR_H
#define	__SYS_EXE_ATTR_H

#include "UnitCfg.h"
#include "linkall.h"

extern const AttrExe_st SysAttrTable[];

void SysAttr_Init(u8 unitID);
u8 Sys_UnitAttrNum(void);
void SysEEPROM_Init(UnitPara_t unitID);

MsgResult_t Set_Device_Attr(UnitPara_t *data);
MsgResult_t Get_Device_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_Unit_Attr(UnitPara_t *data);
MsgResult_t Get_Unit_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);

MsgResult_t Get_Version_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Get_DeviceType_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_UserID_Attr(UnitPara_t *data);
MsgResult_t Get_UserID_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Get_DeviceAddr_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_CmdAckNum_Attr(UnitPara_t *data);
MsgResult_t Get_CmdAckNum_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_RepeatBandAddr_Attr(UnitPara_t *pData);
MsgResult_t Get_RepeatBandAddr_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_RepeatBroadcast_Attr(UnitPara_t *pData);
MsgResult_t Get_RepeatBroadcast_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_UnitArea_Attr(UnitPara_t *data);
MsgResult_t Get_UnitArea_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_UploadAble_Attr(UnitPara_t *data);
MsgResult_t Get_UploadAble_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_UnitAble_Attr(UnitPara_t *data);
MsgResult_t Get_UnitAble_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_UnitType_Attr(UnitPara_t *data);
MsgResult_t Get_UnitType_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_CommonType_Attr(UnitPara_t *data);
MsgResult_t Get_CommonType_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Get_DeviceThreshold_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_Link_Attr(UnitPara_t *data);
MsgResult_t Get_Link_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
#if (ALINKTIME_EN > 0)
MsgResult_t Set_Link_Time_Attr(UnitPara_t *pData);
#endif
#ifndef   STM32L151 		//���ǵ͹��Ĳ�Ʒ	
	MsgResult_t Set_DelayUpload_Attr(UnitPara_t *data);
#endif	
MsgResult_t Set_DelLinkForUnit_Attr(UnitPara_t *pData);
MsgResult_t Get_Eeprom_Info_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);
MsgResult_t Set_FaultFlag_Attr(UnitPara_t *data);
MsgResult_t Get_FaultFlag_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);

MsgResult_t Set_UserDataFormat_Attr(UnitPara_t *data);
MsgResult_t Set_DeviceAdd_Attr(UnitPara_t *data);
MsgResult_t Get_DeviceInfo_Attr(UnitPara_t *data, u8 *rlen, u8 *rpara);

MsgResult_t Get_DeveiceSysClockFeature_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Get_UserID_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);

/*�豸������Ϣ��������*/
MsgResult_t Get_DeviceChannelFlag_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
MsgResult_t Set_DeviceChannelFlag_Attr(UnitPara_t *pData);

#endif
/**************************Copyright BestFu 2014-05-14*************************/

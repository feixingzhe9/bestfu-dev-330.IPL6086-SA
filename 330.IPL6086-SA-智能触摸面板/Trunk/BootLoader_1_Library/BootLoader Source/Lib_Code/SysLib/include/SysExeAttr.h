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


/*********************************��������*************************************/
extern SysData_t gSysData;
extern const AttrExe_st SysAttrTable[];

/*********************************��������*************************************/
void SysAttr_Init(BF_INT08U unitID);
BF_INT08U Sys_UnitAttrNum(void);
void SysEEPROM_Init(UnitPara_t unitID);
MsgResult_t Set_UserDataFormat_Attr(UnitPara_t *pData);
MsgResult_t Get_DeviceType_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);
MsgResult_t Get_Version_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);

#endif
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2014-05-14*************************
��	����    SysExeAttr.c
˵	����    ϵͳ����������ִ�б����������ʵ��
��	�룺    Keil uVision4 V4.54.0.0
��	����    v2.0
��	д��    Unarty
��	�ڣ�    2014.06.26
�ޡ��ģ�	��ŵ�� 2014.10.22 ������߰����Խӿ� B0��BF
			Unarty 2014.12.01 ɾ��0xCE ��0xCF ���ԣ����˽ӿ���Ϊ����V1.00.00�汾��Ԥ���ģ�������ȫȥ���˽ӿڣ�
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "PivotalDataType.h"
#include "Updata.h"
#include "Thread.h"
#include "DataEncrypt.h"
#include "UserData.h"
#include "DeviceUpgradeAttr.h"
#include "DevPartTab.h"
#include "FaultManage.h"
#include "Upload.h"
#include "UnitCfg.h"
#include "UnitShare_Attr.h"
#include "SysExeAttr.h"


/*���Զ�д�б�*/
const AttrExe_st SysAttrTable[] =
{
    {0xC0, LEVEL_0, NULL							, Get_DeviceType_Attr				},
    {0xC1, LEVEL_0, NULL							, Get_Version_Attr					},
    {0xC9, LEVEL_0, Set_UserDataFormat_Attr			, NULL								},
    {0xF4, LEVEL_0, Set_IAPReady_Attr				, Get_IAPReady_Attr					},
    {0xF5, LEVEL_0, Set_IAPPackSave_Attr			, NULL								},
    {0xF8, LEVEL_0, Set_433Channel_Attr				, Get_433Channel_Attr				},
};

/*******************************************************************************
�� �� ��:  Sys_UnitAttrNum
����˵��:  ����ϵͳ��Ԫ��������
��    ��:  ��
�� �� ֵ:  ��
*******************************************************************************/
BF_INT08U Sys_UnitAttrNum(void)
{
    return (BF_DIM(SysAttrTable));
}

/*******************************************************************************
�� �� ��:  Upload_RESTART
����˵��:  �豸�ϱ�����
��    ��:  ��
�� �� ֵ:  ��
*******************************************************************************/
static void Upload_RESTART(void)
{
    Upload_Fault(RESTART);	//�ϱ��豸����
}

/*******************************************************************************
�� �� ��:  SysAttr_Init
����˵��:  ϵͳ���Գ�ʼ��
��    ��:  unitID:	��Ԫ��
�� �� ֵ:  ��
*******************************************************************************/
void SysAttr_Init(BF_INT08U unitID)
{
    UserData_Init();
    Fault_Upload(FAULT_2 , DEV_RESTART , NULL); //�豸����������¼����     yanhuan adding 2015/10/10
    Thread_Login(ONCEDELAY , 0 ,1000 , Upload_RESTART); //1��֮���ϱ�����  yanhuan adding 2015/12/02
    EncryptInit((BF_INT08U*)&gSysData.deviceID); //433�źż��ܳ�ʼ��
}

/*******************************************************************************
�� �� ����  Set_UserDataFormat_Attr
����˵����  �����û����ݻָ���������
��	  ����  data[0]: Ŀ�굥Ԫ
			data[1]: ��������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
MsgResult_t Set_UserDataFormat_Attr(UnitPara_t *pData)
{
    if((1 == pData->len) && (0 == pData->data[0]))   //�ָ��������� ��Ҫ�ָ�433�ŵ�
    {
        UserEEPROMData_Init(SecondLevel);
    }
    else if (0 == pData->len)  						//���ݳ���Ϊ��  Ĭ�ϴ���ԪΪ0
    {
        UserEEPROMData_Init(FristLevel);
    }
    else if (pData->data[0] < UnitCnt_Get())     	//������ȷ
    {
        UnitPublicEEPROMUnitData_Init(pData->data[0]);
        UnitPrivateEEPROMUnitData_Init(pData->data[0]);
    }
    else
    {
        return PARA_MEANING_ERR;
    }

    return COMPLETE;
}

/*******************************************************************************
�� �� ��:  	Get_Version_Attr
����˵��:  	��ȡ�汾��Ϣ
��    ��:  	data[0]: Ŀ�굥Ԫ
			data[1]: ��������
			rLen:	���ز�������
			rpara:	���ز�������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
MsgResult_t Get_Version_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    BF_INT08U *p = NULL;
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    Soft_t* pSoft = (Soft_t*)(pDevPivotalSet->DevSoftInfoAddr);
    Hard_t* pHard = (Hard_t*)(pDevPivotalSet->DevHardInfoAddr);
    Boot0Info_t* pBoot0Info = (Boot0Info_t*)(pDevPivotalSet->Boot0FlagAddr);

    *rlen = 0;
    *(BF_INT16U*)&rpara[*rlen] = 0xffff;				//ϵͳ��汾
    *rlen += 2;
    *(BF_INT32U*)&rpara[*rlen] = ((BOOTLOADER_1_JUMP_FLAG == pSoft->jumpFlag)?(0xFFFFFFFF):(pSoft->SoftVer));		//����汾
    *rlen += sizeof(pSoft->SoftVer);
    *(BF_INT32U*)&rpara[*rlen] = pBoot0Info->SoftVer;
    *rlen += sizeof(pBoot0Info->SoftVer);
    *(BF_INT32U*)&rpara[*rlen] = pHard->hardVer;		//Ӳ���汾
    *rlen += sizeof(pHard->hardVer);

    for (p = pHard->str; (*p <= 'Z')&&(*p >= '0'); p++)  	//Ӳ��������Ϣ
    {
        rpara[((*rlen)++)] = *p;
    }

    return COMPLETE;
}

/*******************************************************************************
�� �� ��:  	Get_DeviceType_Attr
����˵��:  	��ȡ�豸����
��    ��:  	data[0]: Ŀ�굥Ԫ
			data[1]: ��������
			rLen:	���ز�������
			rpara:	���ز�������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
MsgResult_t Get_DeviceType_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    Soft_t* pSoft = (Soft_t*)(pDevPivotalSet->DevSoftInfoAddr);
    if (!(pData->len)	//ȫ�ֲ���
            || (pSoft->deviceType == *(BF_INT16U*)&pData->data[0])	//���豸���Ͳ���
            || (0xFFFF == *(BF_INT16U*)&pData->data[0]))  			//ȫ���Ͳ���
    {
        *rlen = 0;
        *(BF_INT16U*)&rpara[*rlen] = pSoft->deviceType;
        *rlen += 2;
        *(BF_INT32U*)&rpara[*rlen] = pSoft->SoftVer;
        *rlen += 4;

        return COMPLETE;
    }

    return CMD_EXE_ERR;
}


/**************************Copyright BestFu 2014-05-14*************************/

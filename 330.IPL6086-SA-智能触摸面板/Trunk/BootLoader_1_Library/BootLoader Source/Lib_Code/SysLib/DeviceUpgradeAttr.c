/***************************Copyright BestFu 2014-05-14*************************
��	����	FlaseAttr.c
˵	����	����ICFlash���ݲ����������
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-11-22
��  ��:     2014.12.25 Unarty ���ݲ�ͬ�洢��С��Flash,���·�������������пռ��С
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "PivotalDataType.h"
#include "Updata.h"
#include "Thread.h"
#include "si4432.h"
//#include "sys.h"
#include "iwdg.h"
#include "DevPartTab.h"
#include "DeviceUpgradeAttr.h"

//define private variable
static MsgResult_t Set_IAPDataCRC_Attr(UnitPara_t *pData);
static MsgResult_t Get_IAPPackCheck_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);
static MsgResult_t Get_DevUpgradeReady_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara);

/*******************************************************************************
�� �� ���� 	Set_IAPReady_Attr
����˵����  ��������׼������
��	  ��:   data[0]: Ŀ�굥Ԫ
			data[1]: ��������
�� �� ֵ��  ��Ϣִ�н��
*******************************************************************************/
MsgResult_t Set_IAPReady_Attr(UnitPara_t *pData)
{
    IWDG_Feed();
	switch(pData->data[0])
	{
	case 0xF1:
	{
		return (MsgResult_t)UpgradePreStage(pData);
	}
	case 0xF3:
	{
		return Set_IAPDataCRC_Attr(pData);
	}
	default:
	{
		return PARA_MEANING_ERR;
	}     
	}
}

/*******************************************************************************
�� �� ��:  	Get_IAPReady_Attr
����˵��:  	��ȡ�豸����׼��״̬
��    ��:  	data[0]: Ŀ�굥Ԫ
			data[1]: ��������
			rLen:	���ز�������
			rpara:	���ز�������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
MsgResult_t Get_IAPReady_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    IWDG_Feed();
    switch(pData->data[0])
    {
    case 0xF0:
    {
        return (MsgResult_t)Get_DevUpgradeReady_Attr(pData, rlen, rpara);
    }
    case 0xF2:
    {
        return (MsgResult_t)Get_IAPPackCheck_Attr(pData, rlen, rpara);
    }
    default:
	{
		return PARA_MEANING_ERR;
	}   
    }
}

/*******************************************************************************
�� �� ���� 	Get_DevUpgradeReady_Attr
����˵����  ��ȡ�豸����׼��״̬
��	  ��:   data[0]: Ŀ�굥Ԫ
			data[1]: ��������
			rLen:	���ز�������
			rpara:	���ز�������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
static MsgResult_t Get_DevUpgradeReady_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    Soft_t* pSoft = (Soft_t*)(pDevPivotalSet->DevSoftInfoAddr);
    Hard_t* pHard = (Hard_t*)(pDevPivotalSet->DevHardInfoAddr);

    if(UPGRADE_APP == pData->data[1])  				//����app
    {
        *rlen = 11;
        *(BF_INT16U*)&rpara[0] = pSoft->deviceType;
        *(BF_INT32U*)&rpara[2] = pHard->hardVer;
        *(BF_INT32U*)&rpara[6] = pSoft->SoftVer;
        *(BF_INT08U*)&rpara[10]= pHard->chipType;
        return COMPLETE;
    }
    else if(UPGRADE_BOOTLOADER == pData->data[1])  	//����bootloader
    {
        return BOOT_UPGRADE_AIM_ERR;				//����������������������
    }
    return PARA_MEANING_ERR;
}


/*******************************************************************************
�� �� ��:  	Get_IAPPackCheck_Attr
����˵��:  	��ȡ�豸����
��    ��:  	data[0]: Ŀ�굥Ԫ
			data[1]: ��������
			rLen:	���ز�������
			rpara:	���ز�������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
static MsgResult_t Get_IAPPackCheck_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    if(UpgradeSrc == pData->msgsrc)
	{
		if (0xF1 == UPDATA_LEVEL)
		{
			if (Updata_PackCheck(rlen, (BF_INT16U*)rpara))  	//û�е���
			{
				UPDATA_LEVEL = 0xF2;
			}
			return COMPLETE;
		}
		else if (0xF2 == UPDATA_LEVEL)
		{
			return COMPLETE;
		}
		else
		{
			UPDATA_LEVEL = 0;
			return UPGRADE_STATE_ERR;
		}
	}
	return NO_ACK;
}

/*******************************************************************************
�� �� ��:  	Set_IAPDataCRC_Attr
����˵��:  	��ȡ��������У����
��    ��:  	data[0]: Ŀ�굥Ԫ
			data[1]: ��������
			rLen:	���ز�������
			rpara:	���ز�������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
static MsgResult_t Set_IAPDataCRC_Attr(UnitPara_t *pData)
{
	if(UpgradeSrc == pData->msgsrc)
	{
		if (0xF2  == UPDATA_LEVEL)
		{
			//������������ֵ
			*(BF_INT08U*)&pData->data[5] ^= *(BF_INT08U*)&pData->data[6];
			*(BF_INT08U*)&pData->data[6] ^= *(BF_INT08U*)&pData->data[5];
			*(BF_INT08U*)&pData->data[5] ^= *(BF_INT08U*)&pData->data[6];

			if (Updata_Calibrate(*(BF_INT32U*)&pData->data[1], *(BF_INT16U*)&pData->data[5]) == TRUE)
			{
				if (Updata_Flag())  	//�����������
				{
					UPDATA_LEVEL = 0xF3;
					IWDG_FeedOFF();
					Thread_Login(ONCEDELAY,1,800,SystemReset);
	//				JumpToCode(DevPartTab->user_app_start_addr);
					return COMPLETE;
				}
			}
		}
		else if (0xF3 == UPDATA_LEVEL)
		{
			return COMPLETE;
		}
		else
		{
			UPDATA_LEVEL = 0;
			return UPGRADE_STATE_ERR;
		}
	}
	return NO_ACK;
}


/*******************************************************************************
�� �� ���� 	Set_IAPPackSave_Attr
����˵����  �������ݰ�����
��	  ��:   data[0]: Ŀ�굥Ԫ
			data[1]: ��������
�� �� ֵ�� 	��Ϣִ�н��
*******************************************************************************/
MsgResult_t Set_IAPPackSave_Attr(UnitPara_t *pData)
{
	IWDG_Feed();
	if(UpgradeSrc == pData->msgsrc)
	{
		if (0xF1 == UPDATA_LEVEL)  	//�ж�����״̬�Ƿ񵽴�F1
		{
			return (MsgResult_t)Updata_PackProcess(*(BF_INT16U*)&pData->data[0],\
													   pData->len - 2, (BF_INT08U*)&pData->data[2]);//���ݰ�����
		}
		else if (0xF2 == UPDATA_LEVEL)
		{
			return COMPLETE;
		}
		else
		{
			UPDATA_LEVEL = 0;
			return UPGRADE_STATE_ERR;
		}
	}
	return NO_ACK;
}

/*******************************************************************************
�� �� ���� 	Set_433Channel_Attr
����˵����  ����433�ŵ�����
��	  ��:   data[0]: Ŀ�굥Ԫ
			data[1]: ��������
�� �� ֵ��  ��Ϣִ�н��
*******************************************************************************/
MsgResult_t Set_433Channel_Attr(UnitPara_t *pData)
{
    if ((pData->len != 1))  	//���������Ȳ��Ϸ�
    {
        return PARA_LEN_ERR;
    }
    else if (pData->data[0] < CHANNEL_MIN     //���������ݲ��Ϸ�
             || pData->data[0] > CHANNEL_MAX)
    {
        return PARA_MEANING_ERR;
    }

    if (pData->data[0]  != Channel_Get())
    {
        if (Updata_Channel(pData->data[0]) == FALSE)  	//�����ŵ�ʧ��
        {
            return CMD_EXE_ERR;
        }
        Thread_Login(ONCEDELAY, 0, 800, &Si4432_Init);	//�ر�ι�����豸�Զ���������
    }
    return COMPLETE;
}

/*******************************************************************************
�� �� ��:  	Get_433Channel_Attr
����˵��:  	��ȡ433�ŵ�����
��    ��:  	data[0]: Ŀ�굥Ԫ
			data[1]: ��������
			rLen:	���ز�������
			rpara:	���ز�������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
MsgResult_t Get_433Channel_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    *rpara = Channel_Get();		//�ŵ�ֵ
    *rlen = 1;

    return COMPLETE;
}

/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2016-04-28*************************
��	����	Updata.c
˵	����	�������ݴ�����
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2016-04-28
�ޡ��ģ�	��
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "crc.h"
#include "sys.h"
#include "BestFuLib.h"
#include "EEPROM.h"
#include "PivotalDataType.h"
#include "Updata.h"
#include "DevPartTab.h"
#include "program.h"

//Private variable define

//Globe variable define
Soft_t IAP_FLAG;    	//IAP��־
RadioPara_t RADIO_PARA ; //433ͨ�Ų���
UpgradePre_t UpgradePre = {&IAP_FLAG};
BF_INT08U UPDATA_LEVEL = 0xEF; 				//���������ǣ�������ɼ��� F0/F1/F2/F3/F4
BF_INT08U UpgradeSrc = RF_433M_INTERFACE;	//Ĭ������ԴΪ433

//Private function  declarartiuon
static BF_BOOLEAN UpgradeReady(UpgradePre_t* pUpgradePre);

/*******************************************************************************
�� �� ����  void UpgradePreStage(UnitPara_t *pData)
����˵����  ����׼��F0�׶�
��	  ����  data[0]: ����������
			data[1]: ��������
			data[2~3]: �豸����
			data[4~7]: �����ļ����ܴ�С
			data[8]: ÿ���������Ĵ�С
			data[9~12]: ��������İ汾��
�� �� ֵ��  ��������״̬
*******************************************************************************/
BF_INT32U UpgradePreStage(UnitPara_t *pData)
{
    BF_INT32U result = 0u;

    if(UPGRADE_APP == pData->data[1]) 		//�������Ŀ��,�Ƿ�������APP����
    {
        UpgradeSrc 			   			= pData->msgsrc;		//��¼Ŀ������Դ
		UpgradePre.pSoft->deviceType 	= *(BF_INT16U*)(&pData->data[2]);
        UpgradePre.pSoft->appSize 		= *(BF_INT32U*)(&pData->data[4]);
        UpgradePre.pSoft->SoftVer 		= *(BF_INT32U*)(&pData->data[9]);
        UpgradePre.packetsize	  		= *(BF_INT08U*)(&pData->data[8]);
        UpgradePre.packetnum      		= NUM_ROUND(UpgradePre.pSoft->appSize,UpgradePre.packetsize);

        if(TRUE == (result = UpgradeReady(&UpgradePre)))
        {
            UPDATA_LEVEL = 0xF1;
            return COMPLETE;
        }
        return (result);
    }
    return BOOT_UPGRADE_AIM_ERR;
}


/*******************************************************************************
�� �� ����  UpgradeReady
����˵����  ����׼��
��	  ����  pUpgradePre������׼���Ĵ洢ָ��
�� �� ֵ��  ׼����� TRUE(��ɣ�/FALSE(ʧ�ܣ�
*******************************************************************************/
static BF_INT08U UpgradeReady(UpgradePre_t* pUpgradePre)
{
    BF_INT32U userSize = 0u;	//��ֹ���³���ռ�Խ��

    if (pUpgradePre->packetsize & 0x03)  	//������Ĵ�С���ǰ�4�ֽڶ���
    {
        return PACKET_SIZE_ALIGN_ERR;	//����С�ֽڶ������
    }

    userSize = DevPartTab->user_data_start_addr - DevPartTab->user_app_start_addr;

    if (pUpgradePre->pSoft->appSize > userSize)  	//���Ӧ�ÿռ���ڿ��ÿռ��С
    {
        return APP_SIZE_BOUND_ERR;	//��������СԽ��
    }
    memset(pUpgradePre->data,0,sizeof(UpgradePre_t)-offsetof(UpgradePre_t,data));
    return Program_Erase(DevPartTab->user_app_start_addr, pUpgradePre->pSoft->appSize);  //��Ӧ�洢�ռ�����
}

/*******************************************************************************
�� �� ����  Updata_PackProcess
����˵����  �������ݰ�����
��	  ����  id:		��Ӧ����ID
			len:	�������ݳ���
			data:	��������
�� �� ֵ��  FALSE/TRUE
*******************************************************************************/
BF_BOOLEAN Updata_PackProcess(BF_INT32U id, BF_INT08U len, BF_INT08U *data)
{
    BF_INT32U addr;

    if ((id > (UpgradePre.packetnum - 1))|| (len > 200))
    {
        return FALSE;
    }
    if (!(UpgradePre.data[id >> 5]&(1<<(id & 0x1f))))
    {

        addr = id*UpgradePre.packetsize + DevPartTab->user_app_start_addr;
        if (Program_Write(addr, len, data))
        {

            UpgradePre.data[id >> 5] |= (1<<(id & 0x1f)); //���հ�ID��1
        }
        else
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*******************************************************************************
�� �� ����	Updata_PackCheck
����˵���� 	�������ݰ��˶�
��	  ���� 	*len��	ȱ�����ݳ���
			*data:  ȱ��ID
�� �� ֵ��	TRUE/FLASE
*******************************************************************************/
BF_BOOLEAN Updata_PackCheck(BF_INT08U *num, BF_INT16U *data)
{
    BF_INT32U i;

    for (i = 0, *num = 0; i < UpgradePre.packetnum; i++)
    {
        if (!(UpgradePre.data[i >> 5]&(1 << (i & 0x1f))))   //��δ�յ�
        {
            *data++ = i;
            (*num) += 2;
            if ((*num) > 100)
            {
                break;
            }
        }
    }
    return (*num == 0 ? TRUE : FALSE);
}

/*******************************************************************************
�� �� ����	Updata_Calibrate
����˵���� 	�豸�������У��
��	  ���� 	len:	���ݳ���
			crc:	У����
�� �� ֵ��	TRUE/FLASE
*******************************************************************************/
BF_INT08U Updata_Calibrate(BF_INT32U len, BF_INT16U crc)
{
    crc -= ChkCrcValue((BF_INT08U*)DevPartTab->user_app_start_addr, len);
    return ((crc) ? FALSE : TRUE);
}

/*******************************************************************************
�� �� ����	Updata_Flag
����˵���� 	������ʶ
��	  ���� 	add:	��ʶ���洢��ַ
�� �� ֵ��	TRUE/FLASE
*******************************************************************************/
BF_INT08U Updata_Flag(void)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    BF_INT08U ch = Channel_Get();

    IAP_FLAG.jumpFlag = Flash_DefaultValue();
    Program_Erase(pDevPivotalSet->DevSoftInfoAddr,DevPartTab->user_app_start_addr - pDevPivotalSet->DevSoftInfoAddr);	//�޸�Flag����
    Program_Write(pDevPivotalSet->DevSoftInfoAddr, sizeof(Soft_t), (BF_INT08U*)&IAP_FLAG);

    return Updata_Channel(ch);
}

/*******************************************************************************
�� �� ����	Updata_Channel
����˵���� 	����433ͨ��
��	  ���� 	ch:		�޸ĺ���ŵ�ֵ
�� �� ֵ��	TRUE/FLASE
*******************************************************************************/
BF_INT08U Updata_Channel(BF_INT08U ch)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    RadioPara_t* pRadioPara = (RadioPara_t*)(pDevPivotalSet->DevRadioParaAddr);
    RADIO_PARA.channel = (BF_INT32U)ch ;

    for (; (BF_INT32U)pRadioPara < DevPartTab->user_app_start_addr; pRadioPara++)
    {
        if (*(BF_INT32U*)pRadioPara == Flash_DefaultValue())  	//����û���޸�
        {
            return Program_Write((BF_INT32U)pRadioPara, offsetof(RadioPara_t,Reserve), (BF_INT08U*)&RADIO_PARA); //�����޸Ľ��
        }
        Program_Read((BF_INT32U)pRadioPara + offsetof(RadioPara_t,Reserve),\
                     sizeof(RadioPara_t) - offsetof(RadioPara_t,Reserve),\
                     (BF_INT08U*)RADIO_PARA.Reserve);
    }
    Program_Read(pDevPivotalSet->DevSoftInfoAddr, sizeof(Soft_t), (BF_INT08U*)&IAP_FLAG);
    Program_Erase(pDevPivotalSet->DevSoftInfoAddr, DevPartTab->user_app_start_addr - pDevPivotalSet->DevSoftInfoAddr);	//�������
    Program_Write(pDevPivotalSet->DevSoftInfoAddr, sizeof(Soft_t), (BF_INT08U*)&IAP_FLAG);	//����д��

    return Updata_Channel(ch);
}

/*******************************************************************************
�� �� ����	Channel_Get
����˵���� 	��ȡ433ͨ��ֵ
��	  ���� 	��
�� �� ֵ��	433ͨ��ֵ
*******************************************************************************/
BF_INT08U Channel_Get(void)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    RadioPara_t* pRadioPara = (RadioPara_t*)(pDevPivotalSet->DevRadioParaAddr);

    for (; (BF_INT32U)pRadioPara < DevPartTab->user_app_start_addr; pRadioPara ++)
    {
        if (*(BF_INT32U*)pRadioPara == Flash_DefaultValue())  	//����û���޸�
        {
            break;
        }
    }
    -- pRadioPara;	//ָ��ָ����Ч�ռ�

    Program_Read((BF_INT32U)pRadioPara,sizeof(RadioPara_t),(BF_INT08U*)(&RADIO_PARA));
    if ((pRadioPara->channel >= CHANNEL_MIN) &&\
            (pRadioPara->channel <= CHANNEL_MAX))
    {
        return (pRadioPara->channel);	//�������ͨ������ֵ
    }
    return (RADIO_PARA.channel = 0x53);		//����Ĭ��ͨ��ֵ
}

/*******************************************************************************
�� �� ����	AppJump_Init
����˵���� 	bootloader1��ת��ʼ��
��	  ���� 	��
�� �� ֵ��	������ת״̬
*******************************************************************************/
void AppJump_Init(void)
{
    DevPivotalSet_t* pDevPivotalSet = GetDevPivotalSet();
    Soft_t* pSoft = (Soft_t*)(pDevPivotalSet->DevSoftInfoAddr);

    if(BOOTLOADER_1_JUMP_FLAG != pSoft->jumpFlag)
    {
        JumpToCode(DevPartTab->user_app_start_addr);
    }
}
/**************************Copyright BestFu 2015-04-28*************************/

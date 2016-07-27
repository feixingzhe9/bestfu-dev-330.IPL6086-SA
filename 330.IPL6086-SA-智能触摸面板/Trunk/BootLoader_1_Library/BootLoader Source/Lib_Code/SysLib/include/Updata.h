/***************************Copyright BestFu 2014-05-14*************************
��	����	Updata.h
˵	����	�������ݴ�����
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-11-24
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __UPDATA_H
#define __UPDATA_H

/*********************************�Զ����*************************************/
#define CHANNEL_MIN				(73)
#define CHANNEL_MAX				(93)

#define BOOTLOADER_0_JUMP_FLAG	(0xAA788755)		//���bootloader0����ת����
#define BOOTLOADER_1_JUMP_FLAG	(0x5AC33CA5)		//���bootloader1��ת���
/********************************���ݽṹ����**********************************/   
enum								//����������Ŀ��
{
	UPGRADE_APP 		= 0x01,		//�����û�APP����
	UPGRADE_BOOTLOADER	= 0x02		//�����豸����������
};

typedef struct
{
	BF_INT32U hardVer;				//Ӳ���汾
    BF_INT32U Reserve1;				//����
    BF_INT32U chipType;				//оƬ����
	BF_INT32U channelFlag[4];		//������ʶ
    BF_INT32U data;					//����
    BF_INT32U time;					//ʱ��
    BF_INT08U str[100];
}Hard_t;

typedef  struct
{
	BF_INT32U jumpFlag;				//Bootloader_1������ת���
	BF_INT32U deviceType;			//�豸����
    BF_INT32U SoftVer;				//����汾��
    BF_INT32U appSize;				//APP����Ĵ�С�����ֽ�Ϊ��λ 
}Soft_t;

typedef struct
{
	BF_INT32U jumpFlag;				//Bootloader_0������ת���
	BF_INT32U SoftVer;				//bootloader1����汾
	BF_INT32U appSize;				//bootloader1��״�С
}Boot0Info_t;

typedef  struct
{
    BF_INT32U channel; 				//�ŵ�
	BF_INT32U Reserve[3];			//�����ռ�
}RadioPara_t;

#pragma pack(1)
typedef  struct
{
	Soft_t*   pSoft;				//ָ�������Ϣ��ָ��
	BF_INT16U packetnum;			//������������
	BF_INT08U packetsize;   		//��������С
	BF_INT32U data[64];				//�Ѵ����ݰ��Ĵ�С
}UpgradePre_t;
#pragma pack()

/***********************************************************************/
extern BF_INT08U UPDATA_LEVEL; 	//����������
extern BF_INT08U UpgradeSrc;	//��Ϣ��Դ���

/**********************************��������***********************************/ 
BF_INT32U UpgradePreStage(UnitPara_t *pData);
BF_INT08U Updata_Channel(BF_INT08U ch);
BF_INT08U Channel_Get(void);
BF_BOOLEAN Updata_PackCheck(BF_INT08U *num, BF_INT16U *data);
BF_INT08U Updata_Calibrate(BF_INT32U len, BF_INT16U crc);
BF_BOOLEAN Updata_PackProcess(BF_INT32U id, BF_INT08U len, BF_INT08U *data);
BF_INT08U Updata_Flag(void);
void AppJump_Init(void);

#endif  //Updata.h  end
/**************************Copyright BestFu 2014-05-14*************************/

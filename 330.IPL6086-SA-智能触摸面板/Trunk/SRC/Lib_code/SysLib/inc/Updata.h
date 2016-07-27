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

#include "BF_type.h"
#include "UnitCfg.h"

#define CHANNEL_MIN				(73)
#define CHANNEL_MAX				(93)

#define BOOTLOADER_0_JUMP_FLAG	(0xAA788755)		//���bootloader0����ת����
#define BOOTLOADER_1_JUMP_FLAG	(0x5AC33CA5)		//���bootloader1��ת���

enum								//����������Ŀ��
{
	UPGRADE_APP 		= 0x01,		//�����û�APP����
	UPGRADE_BOOTLOADER	= 0x02		//�����豸����������
};

typedef struct
{
	u32 hardVer;			//Ӳ���汾
	u32 Reserve1;			//����
	u32 chipType; 			//оƬ����
	u32 channelFlag[4];		//������ʶ
	u32 data;				//����
	u32 time;				//ʱ��
	u8  str[100];
}Hard_t;

typedef  struct
{
	u32 jumpFlag;			//Bootloader_1������ת��� 
    u32 deviceType;			//�豸����
    u32	SoftVer;			//����汾��
    u32	appSize;			//APP����Ĵ�С�����ֽ�Ϊ��λ  
}Soft_t;

typedef struct
{
	u32 jumpFlag;			//Bootloader_0��ת���
	u32 SoftVer;			//bootloader1����汾
	u32 appSize;			//bootloader1 APP��С
}Boot0Info_t;

#pragma pack(1)
typedef  struct
{
    u32 channel; 			//�ŵ�
	u32 Reserve[3];			//�����ռ�
}RadioPara_t;
#pragma pack()

typedef  struct
{
	Boot0Info_t* pBootInfo;	//ָ�������Ϣ��ָ��
	u16 packetnum;			//������������
	u8 packetsize;   		//��������С
	u32 data[64];			//�Ѵ����ݰ��Ĵ�С
}UpgradePre_t;

extern u8 UPDATA_LEVEL; 	//��¼����״̬
extern u8 UpgradeAim;		//��¼����Ŀ��

u32 UpgradePreStage(UnitPara_t *pData);
u8 Updata_PackProcess(u32 id, u8 len, u8 *data);
u8 Updata_PackCheck(u8 *num, u16 *data);
u8 Updata_Calibrate(u32 len, u16 crc);
u8 Updata_Flag(void);
u8 Updata_Channel(u8 ch);
u8 Channel_Get(void);
u8 Updata_433EncryptFlag(u8 flag);
u8 Get_433EncryptFlag(void);

#endif  //Updata.h  end
/**************************Copyright BestFu 2014-05-14*************************/

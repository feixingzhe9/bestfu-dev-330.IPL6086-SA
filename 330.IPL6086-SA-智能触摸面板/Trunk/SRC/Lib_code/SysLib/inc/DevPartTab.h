/***************************Copyright BestFu 2016-04-16*************************
��	����	DevPartTab.h
˵	����	�豸����������ͷ�ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2016-04-16
��  ��:     ����
*******************************************************************************/
#ifndef __DEV_PARTITION_TABEL_H
#define __DEV_PARTITION_TABEL_H

/*********************************�Զ����*************************************/
#define DEVICE_PARTION_TABLE_ADDR	((u32)(0x08000380))		//�豸���������ʼ��ַ
	
/********************************���ݽṹ����**********************************/ 
typedef struct		//�豸������
{
	u32 boot0_start_addr;			//bootloader0����ʼ��ַ
	u32 boot0_flag_start_addr;		//bootloader0_flag����ʼ��ַ
	u32 boot1_start_addr;			//bootloader1����ʼ��ַ
	u32 boot1_flag_start_addr;		//bootloader1_flag����ʼ��ַ
	u32 user_app_start_addr;		//�û�app����ʼ��ַ
	u32 user_data_start_addr;		//�û��������ı�����ַ
	u32 flash_end_addr;				//�û�flash�Ľ�����ַ
}DevPartionTable_t;

typedef struct						//�豸�ؼ���Ϣ����
{
	u32 DevHardInfoAddr;			//�豸Ӳ����Ϣ��ַ
	u32 Boot0FlagAddr;				//bootloader0��־����Ϣ��ַ
	u32 DevResetParaAddr;			//�豸�����ַ��Ϣ��ַ
	u32 DevFlashEncryptAddr;		//�豸flash������Ϣ��ַ
	u32 DevSoftInfoAddr;			//��������Ϣ��ַ
	u32 DevRadioParaAddr;			//433ͨѶ�����Ϣ��ַ
}DevPivotalSet_t;

#define DevPartTab			((DevPartionTable_t*)(DEVICE_PARTION_TABLE_ADDR))	//�豸������ĵ�ַ��Ϣ

/********************************�ⲿ��������**********************************/
extern const DevPivotalSet_t DevPivotalSet[3];		//�豸�ؼ���Ϣ��������Ϣ

/**********************************��������***********************************/  
void DevPivotalSetInit(void);
DevPivotalSet_t* GetDevPivotalSet(void);

#endif
/**************************Copyright BestFu 2016-04-16*************************/

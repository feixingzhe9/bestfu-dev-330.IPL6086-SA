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
#define DEVICE_PARTION_TABLE_ADDR	((BF_INT32U)(0x08000380))		//�豸���������ʼ��ַ
	
/********************************���ݽṹ����**********************************/ 
typedef struct		//�豸������
{
	BF_INT32U boot0_start_addr;			//bootloader0����ʼ��ַ
	BF_INT32U boot0_flag_start_addr;	//bootloader0_flag����ʼ��ַ
	BF_INT32U boot1_start_addr;			//bootloader1����ʼ��ַ
	BF_INT32U boot1_flag_start_addr;	//bootloader1_flag����ʼ��ַ
	BF_INT32U user_app_start_addr;		//�û�app����ʼ��ַ
	BF_INT32U user_data_start_addr;		//�û��������ı�����ַ
	BF_INT32U flash_end_addr;			//�û�flash�Ľ�����ַ
}DevPartionTable_t;

typedef struct							//�豸�ؼ���Ϣ����
{
	BF_INT32U DevHardInfoAddr;			//�豸Ӳ����Ϣ��ַ
	BF_INT32U Boot0FlagAddr;			//bootloader0��־����Ϣ��ַ
	BF_INT32U DevResetParaAddr;			//�豸�����ַ��Ϣ��ַ
	BF_INT32U DevFlashEncryptAddr;		//�豸flash������Ϣ��ַ
	BF_INT32U DevSoftInfoAddr;			//��������Ϣ��ַ
	BF_INT32U DevRadioParaAddr;			//433ͨѶ�����Ϣ��ַ
}DevPivotalSet_t;

#define DevPartTab			((DevPartionTable_t*)(DEVICE_PARTION_TABLE_ADDR))	//�豸������ĵ�ַ��Ϣ

/**********************************��������***********************************/  
void DevPivotalSetInit(void);
DevPivotalSet_t* GetDevPivotalSet(void);

#endif
/**************************Copyright BestFu 2016-04-16*************************/

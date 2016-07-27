/***************************Copyright BestFu 2014-05-14*************************
��	����	UserData.h
˵	����	�û����ݹ����ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.24 
�ޡ��ģ�	����

*******************************************************************************/
#ifndef __USERDATA_H
#define __USERDATA_H

/*********************************�Զ����*************************************/
#define DEFAULT_USERID				(0xFFFFFFFF)							//Ĭ���û�ID��
#define SUPER_USERID				(0x54534542)							//�����û�ID��("BEST")
#define DEFAULT_AREA				(0xFFFFFFFF)							//Ĭ�������
#define UNIT_MAX_NUM				(UNITDATA_SPACE/ONCEUNITDATA_SPACE)		//�������Ԫ��
#define AREA_RANK_NUM				(4)										//����ּ���

#define USERDATA_START_ADD			(0x0000)								//�û����ݴ洢��ʼ��ַ
#define USERDATA_END_ADD			(0x3000)								//�û����ݴ洢������ַ(8K)

/*ϵͳ�쳣����EEPROM��¼�ռ�*/
#define FAULT_START_ADD				(USERDATA_START_ADD)					//�쳣�洢�׵�ַ
#define FAULT_SPACE					(0x40)									//�쳣����ռ�ÿռ��С 

/*ϵͳ����EEPROM�ռ����*/
#define SYSDATA_START_ADD			(FAULT_START_ADD + FAULT_SPACE)			//ϵͳ���ݴ洢�׵�ַ
#define SYSDATA_SPACE				(0x40)									//ϵͳ����ռ�ÿռ��С 
#define SYSDATA_RESEVER_SPACE		(SYSDATA_SPACE - sizeof(SysData_t))		//ϵͳ����ʣ��ռ�

/*��Ԫ����EEPROM�ռ����*/
#define UNITDATA_START_ADD			(SYSDATA_START_ADD + SYSDATA_SPACE)		//��Ԫ���ݴ洢�׵�ַ 
#define UNITDATA_SPACE				(0x180)									//�û�����ռ�ÿռ��С 
#define ONCEUNITDATA_SPACE			(0x10)									//�����û���Ԫ���ݿռ�
#define UNITDATA_RESEVER_SPACE		(UNITDATA_SPACE - sizeof(UnitData_t))	//�û�����ʣ��ռ�

/*********************************�������Ͷ���*********************************/
#pragma pack(1)     						//��1�ֽڶ���
typedef struct								//ϵͳ������������
{
	BF_INT32U 	userID;						//�û���
	BF_INT32U 	deviceID;					//�豸ID
	BF_INT08U 	sendCnt;					//���ݷ��ʹ���
	BF_INT08U  	serialNum;  				//��ˮ��
	BF_INT32U 	BandAddr[5];				//ת���󶨵�ַ
	BF_INT08U  	BandAreaEn;					//ת��������㲥ָ��ʹ�ܱ�־
}SysData_t;									//ϵͳ����������

typedef struct 								//��Ԫ��������
{
	BF_INT08U 	area[4];					//�����
	BF_INT16U 	type;						//��������
	BF_INT08U 	able;						//��Ԫʹ�ܱ�־λ
	BF_INT08U   common;						//��Ԫ���ô���
}UnitData_t;

typedef enum 
{
	FristLevel = 1,
	SecondLevel = 2,
}DataInitLevel_t;

/*********************************��������*************************************/
extern SysData_t gSysData;

/*********************************��������*************************************/
void UserData_Init(void);
void UserEEPROMData_Init(DataInitLevel_t level);

/*ϵͳEEPROM���ݲ���*/
void SysEEPROMData_Init(void);
BF_INT08U SysEEPROMData_Set(BF_INT08U size , BF_INT08U *data);

///*��Ԫ����EEPROM���ݲ���*/
void UnitPublicEEPROMData_Init(void);
void UnitPublicEEPROMUnitData_Init(BF_INT08U unit);
void UnitPublicEEPROMData_Get(BF_INT08U unitCnt, UnitData_t *unitData);
BF_INT08U UnitPublicEEPROMData_Set(BF_INT08U size , BF_INT08U *data);

///*��Ԫ����EEPROM���ݲ���*/
void UnitPrivateEEPROMData_Init(void);
void UnitPrivateEEPROMUnitData_Init(BF_INT08U unit);

#endif // UserData.h end 
/**************************Copyright BestFu 2014-05-14*************************/

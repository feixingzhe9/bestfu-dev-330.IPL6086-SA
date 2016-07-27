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

#include "Bf_type.h"

/**************************ϵͳEEPROM�洢����*********************************/
#define SYS_VERSION					(10200)											//ϵͳ��汾, UnartyChen Check

#define DEFAULT_USERID				(0xFFFFFFFF)									//Ĭ���û�ID��
#define SUPER_USERID				(0x54534542)									//�����û�ID��("BEST")
#define DEFAULT_AREA				(0xFFFFFFFF)									//Ĭ�������
#define UNIT_MAX_NUM				(UNITDATA_SPACE/ONCEUNITDATA_SPACE)				//�������Ԫ��
#define AREA_RANK_NUM				(4)												//����ּ���

#define SCENCETIME_EN  				(1u)											//���ó�����ʱִ�й���

#define USERDATA_START_ADD			(0x0000)										//�û����ݴ洢��ʼ��ַ
#define USERDATA_END_ADD			(0x3000)										//�û����ݴ洢������ַ(8K)

/*ϵͳ�쳣����EEPROM��¼�ռ�*/
#define FAULT_START_ADD				(USERDATA_START_ADD)							//�쳣�洢�׵�ַ
#define FAULT_SPACE					(0x40)											//�쳣����ռ�ÿռ��С 

/*ϵͳ����EEPROM�ռ����*/
#define SYSDATA_START_ADD			(FAULT_START_ADD + FAULT_SPACE)					//ϵͳ���ݴ洢�׵�ַ
#define SYSDATA_SPACE				(0x40)											//ϵͳ����ռ�ÿռ��С 
#define SYSDATA_RESEVER_SPACE		(SYSDATA_SPACE - sizeof(SysData_t))				//ϵͳ���ݱ����ռ�
																
/*��Ԫ����EEPROM�ռ����*/
#define UNITDATA_START_ADD			(SYSDATA_START_ADD + SYSDATA_SPACE)				//��Ԫ���ݴ洢�׵�ַ 
#define UNITDATA_SPACE				(0x180)											//�û�����ռ�ÿռ��С 
#define ONCEUNITDATA_SPACE			(0x10)											//�����û���Ԫ���ݿռ�
#define UNITDATA_RESEVER_SPACE		(UNITDATA_SPACE - sizeof(UnitData_t))			//�û�����ʣ��ռ�

/*��Ԫ������EEPROM�ռ����*/
#define GROUP_START_ADD				(UNITDATA_START_ADD + UNITDATA_SPACE)			//�豸�������ݴ洢��ַ(��ʼ��ַ0x200)
#define GROUP_SPACE					(0x400)											//�豸��ռ�ÿռ��С
//#define GROUP_RESEVER_SPACE			(GROUP_SPACE-UNIT_GROUP_SPACE*UNIT_MAX_NUM)		//��ʣ��ռ��С(Ԥ��ÿ��Ԫ400���������)
#define UNIT_GROUP_NUM				(320)											//ÿ����Ԫ�������
#define AREA_GROUP_NUM				(64)											//ÿ�������������
#define PER_GROUP_SPACE				(1.0/8)											//��ռ�ÿռ�
#define UNIT_GROUP_SPACE			(UNIT_GROUP_NUM*PER_GROUP_SPACE)				//ÿ����Ԫ��ռ�õĴ洢�ռ�
#define AREA_GROUP_SPACE			(AREA_GROUP_NUM*PER_GROUP_SPACE)				//ÿ��������ռ�õĴ洢�ռ�

/*��Ԫ��������EEPROM�ռ����*/
#define SCENE_START_ADD				(GROUP_START_ADD + GROUP_SPACE)					//������ʼ��ַ(��ʼ��ַ0x600)
#define SCENE_SPACE					(0x1E00)										//�����ռ�(7.5K,֧��256������,ÿ�������³���������)
#define SCENE_END_ADD				(SCENE_START_ADD + SCENE_SPACE)					//����EEPROM������ַ
#define AREA_SCENE_NUM				(64)											//ÿ�����򳡾���ʶ����

#define SCENE_INDEX_START_ADD		(SCENE_START_ADD)								//�豸�����������ݴ洢��ַ	
#define SCENE_INDEX_SPACE			(0x600)											//�豸���������ռ�洢��С(֧��3����չ)
#define SCENE_DEFAULT_VALUE			(0x00)											//������ʶĬ��ֵ

/*�����ڴ�ʹ�ñ�ʶ�洢��ַ����*/
#define SCENE_USE_FLAG_ADD			(SCENE_START_ADD + SCENE_INDEX_SPACE)			//�����ڴ�ʹ�ñ�ǵ�ַ(��ʼ��ַ0xC00)
#define SCENE_USE_FLAG_TOTAL_SPACE	(0x80)											//�����ڴ�ʹ�ñ����ռ�ÿռ��С
#define SCENE_USE_SPACE				(0x20)											//�����ڴ��ʾռ�ÿռ��С
#if (SCENCETIME_EN > 0u)
#define SCENE_TIME_USE_FLAG_ADD		(SCENE_USE_FLAG_ADD + SCENE_USE_SPACE)			//����ʱ���ڴ�ʹ�ñ�ǵ�ַ	
#define SCENE_TIME_USE_SPACE		(SCENE_TIME_TOTAL_NUM/8)						//����ʱ���ڴ��ʾռ�ÿռ��С
#else
#define SCENE_TIME_USE_SPACE		(0)												//����ʱ���ڴ��ʾռ�ÿռ��С
#endif

/*�������ݴ洢��ַ*/
#define SCENE_MAX_NUM				(SCENE_SPACE1_NUM +\
									 SCENE_SPACE2_NUM +\
									 SCENE_SPACE3_NUM)								//��󳡾���
#define SCENE_DATA_START_ADD		(SCENE_USE_FLAG_ADD+SCENE_USE_FLAG_TOTAL_SPACE)	//�������ݴ洢λ�õ���ʼ��ַ(��ʼ��ַ0xC80)
#define SCENE_DATA_SPACE			(0x1000)										//�������ݴ洢�ռ�,4K
#define SCENE_DATA_SPACE1			(8)												//�������ݿռ�1��С
#define SCENE_DATA_SPACE2			(16)											//�������ݿռ�2��С
#define SCENE_DATA_SPACE3			(32)											//�������ݿռ�3��С

#define SCENE_SPACE1_NUM			(128)											//�����ռ�1������
#define SCENE_SPACE2_NUM			(64)											//�����ռ�2������
#define SCENE_SPACE3_NUM			(64)											//�����ռ�3������

#if (SCENCETIME_EN > 0u)
/*������ʱ����EEPROM�ڴ����,ע����ʱʱ���ڴ��ı�Ŵ�1��ʼ,���֧��56��*/
#define SCENE_TIME_DATA_START_ADD	(SCENE_DATA_START_ADD + SCENE_DATA_SPACE)		//����ʱ�����ݴ洢��ַ(��ʼ��ַ0x1C80)
#define SCENE_TIME_SPACE			(0x500)											//����ʱ������ռ�ÿռ��С
#define SCENE_TIME_TOTAL_NUM		(SCENE_TIME_SPACE1_NUM +\
									 SCENE_TIME_SPACE2_NUM +\
									 SCENE_TIME_SPACE3_NUM)							//����ʱ�䳡�����������

#define SCENE_TIME_DATA_SPACE1      (10)											//����ʱ��ռ�1�Ĵ�С
#define SCENE_TIME_DATA_SPACE2      (35)											//����ʱ��ռ�2�Ĵ�С
#define SCENE_TIME_DATA_SPACE3      (50)											//����ʱ��ռ�3�Ĵ�С

#define SCENE_TIME_SPACE1_NUM		(32)											//������ʱ�ռ�1������
#define SCENE_TIME_SPACE2_NUM		(16)											//������ʱ�ռ�2������
#define SCENE_TIME_SPACE3_NUM		(8)												//������ʱ�ռ�3������
#endif

/*�������ݴ洢��ַ*/
#define ALINK_START_ADDR            (0x2400)										//��������׵�ַ
#define ALINK_SPACE                 (0x0700)                                		//��������ռ�

/*����ʱ�����ݴ洢��ַ*/
#define ALINK_TIME_START_ADDR       (0x2B00)										//��ʱʱ���׵�ַ
#define ALINK_TIME_SPACE            (0x0200)										//��ʱʱ��ռ�

/*�������Ͷ���*/
#pragma pack(1)     				//��1�ֽڶ���
typedef struct						//ϵͳ������������
{
	u32 userID;						//�û���
	u32 deviceID;					//�豸ID
	u8 	sendCnt;					//���ݷ��ʹ���
	u8  serialNum;  				//��ˮ��
	u32 BandAddr[5];				//ת���󶨵�ַ
	u8  BandAreaEn;					//ת��������㲥ָ��ʹ�ܱ�־
}SysData_t;							//ϵͳ����������

typedef struct 						//��Ԫ��������
{
	u8 	area[4];					//�����
	u16 type;						//��������
	u8 	able;						//��Ԫʹ�ܱ�־λ
	u8  common;						//��Ԫ���ô���
	u8  UploadAble; 				//�ϱ�ʹ�ܱ�־
	u8  UploadSetTime;				//�ϱ�������ʱ��
}UnitData_t;

typedef union
{
	struct
	{
		u16 sceneNum: 8;		//������
		u16 area	: 3;		//�����
		u16 unit	: 5;		//��Ԫ��
	}scene;
	u16 flag;
}SceneInfo_t;

typedef struct 
{
	SceneInfo_t SceneInfo;	
#if (SCENCETIME_EN > 0u)	
	u8 sceneTimeSavePos;		//�����洢λ��,1~56
#endif
}SceneIndexes_t;

typedef struct
{
	SceneIndexes_t SceneIndexes[SCENE_MAX_NUM];//�����Ĵ洢��ַ,0~255
	u8 sceneUseFlag[SCENE_USE_SPACE];
#if (SCENCETIME_EN > 0u)
	u8 sceneTimeUseFlag[SCENE_TIME_USE_SPACE];
#endif
}sceneManage_t;

#pragma pack()
typedef enum 
{
	FristLevel = 1,
	SecondLevel = 2,
}DataInitLevel_t;


extern SysData_t gSysData;

void UserData_Init(void);
void UserEEPROMData_Init(DataInitLevel_t level);

/*�豸������Ϣ���ݲ���*/
u32 GetDeviceChannelFlag(void);
u8 SetDeviceChannelFlag(const u32 deviceUseNum,const u8 operateLevel);

/*�쳣ͳ��EEPROM���ݲ���*/
void FaultData_Init(void);
void FaultData_Save(u8 i);
void FaultData_Get(u8 *len, u8 *data);

/*ϵͳEEPROM���ݲ���*/
void SysEEPROMData_Init(void);
u8 SysEEPROMData_Set(u8 size , u8 *data); 

/*��Ԫ����EEPROM���ݲ���*/
void UnitPublicEEPROMData_Init(void);
void UnitPublicEEPROMUnitData_Init(u8 unit);
void UnitPublicEEPROMData_Get(u8 unitCnt, UnitData_t *unitData);
u8 UnitPublicEEPROMData_Set(u8 size , u8 *data);

/*��Ԫ����EEPROM���ݲ���*/
void UnitPrivateEEPROMData_Init(void);
void UnitPrivateEEPROMUnitData_Init(u8 unit);

/*��EEPROM���ݲ���*/
void GroupEEPROM_Init(void);
void GroupEEPROMUnit_Init(const u8 unit);
void GroupEEPROM_Clr(const u8 unit, const  u8 area, const  u8 num);
void GroupEEPROM_Set(const u8 unit,const u8 area,const u8 num);
u8 GroupEEPROM_Get(const u8 unit,const u8 area,const u8 num);

/*����EEPROM���ݲ���*/
void SceneData_Init(void);
void SceneEEPROM_Init(void);
void SceneEEPROMUnit_Init(u8 unit);
void SceneEEPROM_Clr(const u8 unit, const u8 area, const u16 num);
void SceneEEPROM_Get(u8 unit, u8 area, u8 num, u8 *data);
u32 SceneEEPROM_Set(u8 unit, u8 area, u8 num, u8 *data);

#if (SCENCETIME_EN > 0u)
u32 SceneEEPROM_SetEx(u8 unit, u8 area, u8 num, u8 *data);
u8 SceneEEPROM_GetEx(u8 unit, u8 area, u8 num, u8 *data);
void CheckSceneTimeData(u8 unit, u8 area, u8 num);
void CheckSceneDlyTimeEEPROM(void);
#endif

#endif // UserData.h end 
/**************************Copyright BestFu 2014-05-14*************************/

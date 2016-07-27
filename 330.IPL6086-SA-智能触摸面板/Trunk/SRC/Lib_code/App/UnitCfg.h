/***************************Copyright BestFu 2014-05-14*************************
��	����	UnitCgf.h
˵	����	��Ԫģ������ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.07.14 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __UNITCFG_H
#define __UNITCFG_H

#include "BF_type.h"
#include "UserData.h"

#define    SRCEVENT          1               // ԴΪ�¼���
#define    SRCVALUE          2               // ԴΪ��ֵ��

typedef enum
{
	WRITEWITHACK 	= 0x02,		//��Ӧ��д������Ϣ������
    ACKWRITE		= 0x03,     //д����Ӧ����Ϣ������
    WRITENACK       = 0x04,    	//����Ӧ��д������Ϣ������

    READWITHACK     = 0x05,    	//��������Ϣ������
    READACK         = 0x06,    	//������Ӧ����Ϣ������

    WRITESUC        = 0x07,    	// д�����ɹ�ʱ�Ż�Ӧ��Ϣ������
    WRITEACKSUC     = 0x08,    	// д�����ɹ�ʱӦ����Ϣ������
    WRITEFAIL       = 0x09,    	// д����ʧ��ʱ�Ż�Ӧ��Ϣ������
    WRITEACKFAIL    = 0x0A,   	// д����ʧ��ʱӦ����Ϣ������

    READSUC         = 0x0B,    	// �������ɹ�ʱ�Ż�Ӧ��Ϣ������
    READACKSUC      = 0x0C,    	// �������ɹ�ʱӦ����Ϣ������
    READFAIL        = 0x0D,    	// ������ʧ��ʱ�Ż�Ӧ��Ϣ������
    READACKFAIL     = 0x0E,    	// ������ʧ��ʱӦ����Ϣ������
}MsgRcve_t;

typedef enum
{
	SINGLE_ACT 			         = 0X01,		//���ڵ����
	GROUP_ACT 			         = 0X02,		//��㲥
	SCENE_ACT			         = 0X03,		//�龰�㲥
	DEVIVCE_TYPE_ACT 	         = 0x04,		//�豸���͹㲥 
	DEV_ALL_ACT      	         = 0x05,    	//���й㲥
	UNIT_USER_TYPE_ACT	         = 0x06,		//��ԪӦ�����͹㲥
	UNIT_COMMON_TYPE_ACT         = 0x07,		//��ԪӦ�ô���㲥
	SPEC_AREA_DEV_TYPE_ACT  	 = 0x14,		//�ض������豸���͹㲥				/**>  Jay Add 2015.11.17*/
	SPEC_AREA_UNIT_USER_TYPE_ACT = 0x16			//�ض�����ԪӦ�����͹㲥			/**>  Jay Add 2015.11.17*/
}ObjectType_t;

typedef enum
{
	COMPLETE = 1, 			//�����������
	
	OWN_RETURN_PARA,		//ӵ�з��ز���	
	NO_ACK,					//����Ӧ��
	USER_ID_ERR,			//�û��Ŵ���
	ADDS_TYPE_ERR,			//Ŀ���ַ���ʹ���	
	ID_ERR,					//Ŀ���ַ����
	
	AREA_ERR,				//�������
	MSG_REPEAT,				//��Ϣ�ظ�
	UNIT_DISABLE,			//��Ԫ����
	NO_WRITE,				//����û��дȨ��
	NO_READ,				//����û��Ȩ��
	
	MSG_TYPE_ERR,			//��Ϣ���ʹ���
	NO_CMD,					//��Ԫ���Բ�����
	PARA_MEANING_ERR,		//�����������
	PARA_LEN_ERR,			//�������ȴ���
	EEPROM_ERR,				//EEPROM��������
	
	EEPROM_RAND_ERR,		//EEPROM������ΧԽ��
	CMD_EXE_ERR,			//ָ��ִ�д���
	
	LINKFULL,           	//���������޷���д
	LINKNUM_ERR,			//�����Ż�ȡʧ��
	LINKMAP_ERR,         	//���������������������������ƥ�����
	LINK_MODIFY_ERR,    	//�����޸Ĵ���
	LINK_WRITE_ERR,     	//����д��ʧ��
	
	CMD_EXE_OVERTIME,		//ָ��ִ�г�ʱ	
	DATA_SAVE_OVER,			//���ݴ洢��
	DATA_NO_FIND,			//����û���ҵ�	
	VOICE_POWER_BAT,		//�������ڵ�ع���״̬	
	
	DLY_BUFFER_FULL,		//��ʱ�����������	
	
	TIME_PARA_ERR,			//ʱ���������
	
	BOOT_UPGRADE_AIM_ERR,	//����������������������
	BOOT_LOSE_ERR,			//bootloader��������ʧ
	
	APP_SIZE_BOUND_ERR,		//����APP��СԽ��
	PACKET_SIZE_ALIGN_ERR,	//����С�ֽڶ������
	
	UPGRADE_STATE_ERR,		//����״̬����
}MsgResult_t;				//��Ϣ�������ö����

typedef enum
{
	LEVEL_0,		//�ȼ����
	LEVEL_1,		//
}Level_t;
#pragma pack(1)
typedef struct
{
	const 	u8 unit;
	const 	u8 cmd;
			u8 len;
	const 	u8 data[200];
}UnitPara_t;
#pragma pack()

typedef struct
{
	u32 cmd;                                   	    //ָ���
	Level_t level;									//������Ҫ����
	MsgResult_t (*pSet_Attr)(UnitPara_t *pData);    //set�������
	MsgResult_t (*pGet_Attr)(UnitPara_t *pData,\
						u8 *rlen, u8 *rpara);  		//get�������
	u8 unitAttrRelate;								//������Ԫ������֮��Ĺ�����ϵ,ע��0��Ϊ����,û�й������壬Ϊ0xffʱΪȫ��Ԫ�������
}AttrExe_st;

typedef struct
{
	void (*Init)(u8 unitID);        // ��Ԫ��ʼ������������ȫ�ֲ�����������ʼ��
	void (*DefEE)(u8 unitID);       // EEPROM�����趨Ĭ��ֵ
	void (*Instruct)(void);			//��Ԫ��֤����
	const AttrExe_st *attrExe;		//�����б�ָ��
	u8 (*ExeAttrNum)(void);			//��Ԫִ�����Ը���
}Unit_st;

extern const Unit_st UnitTab[];
extern UnitData_t gUnitData[];

void Unit_Init(void);
u8 UnitCnt_Get(void);

/*�û�EEPROM���ݶ�д��������*/
extern MsgResult_t EEPROM_Write(u32 addr, u32 len, u8 *data);
extern MsgResult_t EEPROM_Read(u32 addr, u32 len, u8 *data);
extern void Upload(u8);
extern void PropEventFifo(unsigned char unitID, unsigned char propID, 
					unsigned char type, int value);
#endif
/**************************Copyright BestFu 2014-05-14*************************/

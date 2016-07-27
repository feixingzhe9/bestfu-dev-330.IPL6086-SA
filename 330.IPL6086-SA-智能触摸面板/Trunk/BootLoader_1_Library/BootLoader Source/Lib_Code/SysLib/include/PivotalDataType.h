/***************************Copyright BestFu 2014-05-14*************************
��	����    PivotalDataType.h
˵	��:		��ؼ���������ͷ�ļ�
��	�룺    uVision V5.12.0
��	����    v1.0
��	д��    jay
��	�ڣ�    2016-04-14
��  ��:     ����
*******************************************************************************/
#ifndef __PIVOTAL_DATA_TYPE_H   
#define __PIVOTAL_DATA_TYPE_H   

/*********************************�Զ����*************************************/
#define CRC_SIZE					(0x02)			//CRC����ռ�ÿռ䣬��λ��byte
#define MAX_DATA_FRAME_LEN			(0xFA)			//һ��ָ��������
#define HEAD_SIZE					(6)				//ͷ���ݳ���
#define TAIL_SIZE					(6)				//β���ݳ���
#define ARGE_SEAT					(1)				//������λ��

#define EVENT						(0x00)
#define COMMUNICATION_VERSION		(0x01)			//ͨ�Ű汾
#define DECRYPT_COMMUNICATION   	(0x02)      	//����ͨ�Ű汾
#define MSG_UPLOAD_ID				(0x88888888)	//��Ϣ�ϱ���ַ

//�������ݵ�ͨѶ��Դ
#define RF_433M_INTERFACE			((BF_INT08U)(1))//����433ͨѶ��Դ���	
#define USART_INTERFACE				((BF_INT08U)(2))//���崮��ͨѶ��Դ���
#define USB_INTERFACE				((BF_INT08U)(3))//����USBͨѶ��Դ���
#define INTERNET_INTERFACE			((BF_INT08U)(4))//����������ͨѶ��Դ���

/*********************************�������Ͷ���*********************************/
typedef enum
{
	COMPLETE 				= 1,	//�����������
	
	OWN_RETURN_PARA			= 2,	//ӵ�з��ز���	
	NO_ACK					= 3,	//����Ӧ��
	USER_ID_ERR				= 4,	//�û��Ŵ���
	ADDS_TYPE_ERR			= 5,	//Ŀ���ַ���ʹ���	
	ID_ERR					= 6,	//Ŀ���ַ����
	
	AREA_ERR				= 7,	//�������
	MSG_REPEAT				= 8,	//��Ϣ�ظ�
	UNIT_DISABLE			= 9,	//��Ԫ����
	NO_WRITE				= 10,	//����û��дȨ��
	NO_READ					= 11,	//����û��Ȩ��
	
	MSG_TYPE_ERR			= 12,	//��Ϣ���ʹ���
	NO_CMD					= 13,	//��Ԫ���Բ�����
	PARA_MEANING_ERR		= 14,	//�����������
	PARA_LEN_ERR			= 15,	//�������ȴ���
	EEPROM_ERR				= 16,	//EEPROM��������
	
	EEPROM_RAND_ERR			= 17,	//EEPROM������ΧԽ��
	CMD_EXE_ERR				= 18,	//ָ��ִ�д���
	
	LINKFULL				= 19,   //���������޷���д
	LINKNUM_ERR				= 20,	//�����Ż�ȡʧ��
	LINKMAP_ERR				= 21,   //���������������������������ƥ�����
	LINK_MODIFY_ERR			= 22,	//�����޸Ĵ���
	LINK_WRITE_ERR			= 23,	//����д��ʧ��
	
	CMD_EXE_OVERTIME		= 24,	//ָ��ִ�г�ʱ	
	DATA_SAVE_OVER			= 25,	//���ݴ洢��
	DATA_NO_FIND			= 26,	//����û���ҵ�	
	VOICE_POWER_BAT			= 27,	//�������ڵ�ع���״̬	
	
	DLY_BUFFER_FULL			= 28,	//��ʱ�����������		
	TIME_PARA_ERR			= 29,	//ʱ���������
	
	BOOT_UPGRADE_AIM_ERR	= 30,	//����������������������
	BOOT_LOSE_ERR			= 31,	//bootloader��������ʧ	
	APP_SIZE_BOUND_ERR		= 32,	//����APP��СԽ��
	PACKET_SIZE_ALIGN_ERR	= 33,	//����С�ֽڶ������	
	UPGRADE_STATE_ERR		= 34,	//����״̬����
	
	FLASH_SPACE_FULL_ERR	= 35,	//flash�ռ���������
	
}MsgResult_t;						//��Ϣ�������ö����

//ͨѶЭ����ؽṹ�嶨��
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

#pragma pack(1)
typedef union
{
    BF_INT32U 	id;
    BF_INT08U	area[4];
}Object_t;      //Ŀ��������ͬ��

typedef struct 
{
	BF_INT32U	userID;     	// �û���
	BF_INT08U	objectType;    	// Ŀ���ַ����
	Object_t 	object;			// Ŀ���ַ
	BF_INT16U	actNum;  		// �㲥���ͺ�
	BF_INT32U 	sourceID;   	// Դ��ַ
	BF_INT08U	serialNum;      // ��ˮ��
}CMDAim_t;	//ͨ��Ŀ��������

typedef struct 
{
	BF_INT08U	msgType;		// ��Ϣ������
	BF_INT08U	unit;           // ��ԪID
	BF_INT08U	cmd; 			// ������
	BF_INT08U	len;		  	// ��������
}CMDPara_t;						//ͨ�Ų�������

typedef struct
{
	BF_INT08U	version;   		// �汾��
	CMDAim_t 	aim;			//Ŀ����������
	CMDPara_t	para;		
}Communication_t;
#pragma pack()

//��Ԫ���ݴ�������������Ͷ���
typedef enum
{
	LEVEL_0,					//�ȼ����
	LEVEL_1,					//
}Level_t;
#pragma pack(1)
typedef struct
{
	const	BF_INT08U msgsrc;	//��Ϣ��Դ
	const 	BF_INT08U unit;
	const 	BF_INT08U cmd;
			BF_INT08U len;
	const 	BF_INT08U data[200];
}UnitPara_t;
#pragma pack()

typedef struct
{
	BF_INT32U cmd;                                   		//ָ���
	Level_t level;											//������Ҫ����
	MsgResult_t (*pSet_Attr)(UnitPara_t *pData);    		//set�������
	MsgResult_t (*pGet_Attr)(UnitPara_t *pData,\
						BF_INT08U *rlen, BF_INT08U *rpara);	//get�������
}AttrExe_st;

typedef struct
{
	void (*Init)(BF_INT08U unitID);			// ��Ԫ��ʼ������������ȫ�ֲ�����������ʼ��
	void (*DefEE)(BF_INT08U unitID);		// EEPROM�����趨Ĭ��ֵ
	void (*Instruct)(void);					//��Ԫ��֤����
	const AttrExe_st *attrExe;				//�����б�ָ��
	BF_INT08U (*ExeAttrNum)(void);			//��ȡ��Ԫ���Ե�����
}Unit_st;

/*********************************��������*************************************/
extern const BF_INT08U HeadCMD[HEAD_SIZE];
extern const BF_INT08U TailCMD[TAIL_SIZE];

#endif 	//ComStructure.h
/**************************Copyright BestFu 2014-05-14*************************/

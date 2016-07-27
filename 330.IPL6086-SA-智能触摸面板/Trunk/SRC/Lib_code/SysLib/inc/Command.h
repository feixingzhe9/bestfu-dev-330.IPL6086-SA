/***************************Copyright BestFu 2014-05-14*************************
��	����    command.h
˵	��:	    �Ӷ����л�ȡָ��ͷ�ļ�
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2013-10-14
��  ��:     ����
*******************************************************************************/
#ifndef __COMMAND_H
#define __COMMAND_H

#include "BF_type.h"
#include "fifo.h"

#define CRC_SIZE	(0x02)			//CRC����ռ�ÿռ䣬��λ��byte
#define CMD_SIZE	(0xFA)			//һ��ָ��������
#define HEAD_SIZE	(6)				//ͷ���ݳ���
#define TAIL_SIZE	(6)				//β���ݳ���
#define ARGE_SEAT	(1)				//������λ��

//Լ��433��֡���ɳ���50bytes
#define COMMUNICATION_SHORT_FRAME		(1) //ͨѶ��֡��0��������ͨѶ��֡ ��1������ͨѶ��֡
	
	#if (COMMUNICATION_SHORT_FRAME == 1)
#define SYSTICK_SHORT_VERSION       (0x03)  //ͨѶ�汾01Ϊ����֡��02Ϊ����֡����֡��03��ʼ
#define ACK_SHORT_VERSION           (0x04)  
	#endif
	
extern const u8 HeadCMD[HEAD_SIZE];
extern const u8 TailCMD[TAIL_SIZE];

#pragma pack(1)
typedef union
{
    u32 id;
    u8  area[4];
}Object_t;      //Ŀ��������ͬ��

typedef struct 
{
	u32	 		userID;     	// �û���
	u8 			objectType;    	// Ŀ���ַ����
	Object_t 	object;			// Ŀ���ַ
	u16 		actNum;  		// �㲥���ͺ�
	u32 		sourceID;   	// Դ��ַ
	u8			serialNum;      // ��ˮ��
}CMDAim_t;	//ͨ��Ŀ��������

typedef struct 
{
	u8	msgType;       // ��Ϣ������
	u8  unit;           // ��ԪID
	u8  cmd;              // ������
	u8  len;		  // ��������
}CMDPara_t;				//ͨ�Ų�������

typedef struct
{
	u8		version;   	// �汾��
	CMDAim_t aim;		//Ŀ����������
	CMDPara_t para;		
}Communication_t;

typedef struct
{
	u8 version ;
	u32 userID ;
	u32 sourceID ;
	u8 serialNum ;
}ShortCMDAim_t ;//��֡��ʽ�ı�����Ϣ�����ڶ�֡��ͷ��

typedef struct
{
	ShortCMDAim_t		aim; // ͨѶĿ����������
	u8	  data[200];	  //ͨѶ��������
}ShortCommunication_t; //��֡ͨѶ�ṹ�� yanhuan adding 2015/12/29

#pragma pack()

#if (COMMUNICATION_HEAD_TAIL == 0)
u8 head_put(FIFO_t *fifo);
u8 tail_put(FIFO_t *fifo);
#endif

#if 0
u32 cmd_get(u8 *cmdBuf, FIFO_t *fifo);//�Ӷ��г�һ������ָ��
u32 cmd_put(FIFO_t *fifo, u8 *cmdBuf, u32 cmdLen);	//����һ��ָ�Ŀ�������
#endif

u32 short_get(u8 *cmdBuf, FIFO_t *fifo);//�Ӷ��г�һ����ָ֡��
u32 short_put(FIFO_t *fifo, u8 *cmdBuf, u32 cmdLen);

u32 EncryptCmd_get(u8 *cmdBuf, FIFO_t *fifo);
u32 EncryptCmd_getEx(u8 *cmdBuf, FIFO_t *fifo);
u32 EncryptCmd_put(FIFO_t *fifo, u8 *cmdBuf, u32 cmdLen);

#endif 	//command.h
/**************************Copyright BestFu 2014-05-14*************************/

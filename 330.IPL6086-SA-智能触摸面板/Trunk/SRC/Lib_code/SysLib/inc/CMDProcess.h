/***************************Copyright BestFu 2014-05-14*************************
��	����	cmdProcess.h
˵	����	FIFOָ�����ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-11-01
��  ��:     ����
*******************************************************************************/
#ifndef __CMDPROCESS_H
#define __CMDPROCESS_H

#include "FIFO.h"
#include "UnitCfg.h"
#include "command.h"

#define FIFOBUFSZE     (0x400) // 1KB 
#define ADDFIFOBUFSZ   (0x80)  // 128B

//����Process��ʼ����
#define   PROCESS_INIT(pStruct,_flag,_con,_type,_prcfun,_Data_Get)	do{(pStruct)->flag = (_flag);\
																	(pStruct)->con = (_con);\
																	(pStruct)->type = (_type);\
																	(pStruct)->fun = (_prcfun);\
																	(pStruct)->Data_Get = (_Data_Get);\
																	}while(0)

typedef u8 (*ProcessFunction)(u8 *buf, u8 len, u8 level);
typedef u32 (*FIFOGetFun)(u8 *cmdBuf, FIFO_t *fifo);

typedef struct
{
	u8 len; 	//sendBuf�������ݳ���
	u8 flag;	//�����־
	u8 con;		//��������
	u8 mean;	//��������
	u8 type;  //��Ϣ���� yanhuan adding 2015/12/30
	u8 level;	//��Ϣ�ȼ�
	u8 data[255];	//Ҫ�����ʵ����������
	ProcessFunction fun;
	FIFOGetFun		Data_Get;	//�������ݻ�ȡ����
}Process_t;

typedef enum
{
	NORMAL,		//����ģʽ
	MANY_P,		//��δ���
}CMDcon_t;

typedef enum
{
	COMMON,//һ����Ϣ
	SHORT,//��֡��Ϣ
}CMDtype_t ;

extern FIFO_t rcv433fifo;
extern FIFO_t send433fifo;
extern Process_t rcv_433;
extern Process_t send_433;
#if (COMMUNICATION_SHORT_FRAME == 1)
	extern FIFO_t rcv433_shortfifo;
	extern FIFO_t send433_shortfifo;
	extern Process_t rcv_433short;
	extern Process_t send_433short;
#endif
void FIFOInit(void);

void ProcessSend_Fun(ProcessFunction sendFun);
void Process_FIFOData(FIFO_t *fifo, Process_t *prc);
void Process_FIFODataShort(FIFO_t *fifo, Process_t *prc); //��֡���� yanhuan adding 2015/12/28
u8 Msg_Change(u8 *data, u8 len, u8 level);
MsgResult_t Msg_Execute(CMDPara_t *para);

#endif
/**************************Copyright BestFu 2014-05-14*************************/

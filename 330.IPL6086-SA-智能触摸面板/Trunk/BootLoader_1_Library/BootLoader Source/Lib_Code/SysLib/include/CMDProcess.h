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

/********************************�궨��****************************************/
//����Process��ʼ����
#define   PROCESS_INIT(pStruct,_flag,_con,_msgfrom,_msggetfun,_prcfun)	do{(pStruct)->flag = (_flag);\
																		(pStruct)->con = (_con);\
																		(pStruct)->msgsrc = (_msgfrom);\
																		(pStruct)->msggetfun = (_msggetfun);\
																		(pStruct)->procfun = (_prcfun);\
																		}while(0)

/*********************************�������Ͷ���*********************************/
typedef BF_BOOLEAN (*ProcessFunction)(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);
typedef BF_INT32U  (*DecryptGetFunction)(BF_INT08U *cmdBuf, FIFO_t *fifo);

typedef struct
{
	BF_INT08U len; 			//sendBuf�������ݳ���
	BF_INT08U flag;			//�����־
	BF_INT08U con;			//��������
	BF_INT08U mean;			//��������
	BF_INT08U level;		//��Ϣ�ȼ�
	BF_INT08U msgsrc;		//��Ϣ��Դ
	BF_INT08U data[255];	//Ҫ�����ʵ����������
	ProcessFunction procfun;
	DecryptGetFunction msggetfun;
}Process_t;

typedef enum
{
	NORMAL,		//����ģʽ
	MANY_P,		//��δ���
}CMDcon_t;

/*********************************��������*************************************/


/*********************************��������*************************************/
void FIFOInit(void);
void Process_FIFOData(FIFO_t *fifo, Process_t *prc);
void MsgReviceProc(void);
BF_INT08U Msg_Change(BF_INT08U *data, BF_INT08U len, BF_INT08U level);
MsgResult_t Msg_Execute(CMDPara_t *para,const BF_INT08U msgfrom);
void Msg_Feedback(Communication_t *pMsg,const BF_INT08U msgfrom);

#endif
/**************************Copyright BestFu 2014-05-14*************************/

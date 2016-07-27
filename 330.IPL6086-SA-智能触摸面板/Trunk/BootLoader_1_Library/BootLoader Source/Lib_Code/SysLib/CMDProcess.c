/***************************Copyright BestFu 2014-05-14*************************
��	����    cmdProcess.c
˵	����    FIFOָ�����
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2013-11-01
��  ��:     2014.12.28 Unarty �޸ĵ�Ԫ����ִ�б߽������ж�ֵ����ֹ�൥Ԫִ��ʱ����Խ��
			2014.01.08 Unarty ���ӵ�Ԫ��ֵ�˶� ��ֹ����Խ��
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "PivotalDataType.h"
#include "fifo.h"
#include "Thread.h"
#include "UserData.h"
#include "BestFuLib.h"
#include "Object.h"
#include "FifoDataProc.h"
#include "FaultManage.h"
#include "PhyMsgProc.h"
#include "UnitCfg.h"
#include "CMDProcess.h"

//Globe variable define


//Private variable define
#if (RF_433M_INTERFACE_EN > 0u)
Process_t rcv433Prc;
Process_t snd433Prc;

#endif
#if (USART_INTERFACE_EN > 0u )
Process_t rcvUsartPrc;
Process_t sndUsartPrc;

#endif
#if (USB_INTERFACE_EN > 0u)
Process_t rcvUSBPrc;
Process_t sndUSBPrc;

#endif
#if (INTERNET_INTERFACE_EN > 0u)
Process_t rcvIntnetPrc;
Process_t sndIntnetPrc;

#endif

//Private function declareation
static BF_INT08U   Msg_ReturnType(BF_INT08U msgType);
static void DataSend_Thread(void);
//static BF_INT08U   Msg_Level(BF_INT32U object);		////����ͨ��ƥ�䣨Unarty 2014.08.07)
static BF_BOOLEAN Msg_Process(BF_INT08U *sdata, BF_INT08U len, BF_INT08U level);

/*****************************************************************************
  ������:  void sendFIFOInit(void)
����˵��: 	����FIFO���г�ʼ��
    ����:  	void
  ����ֵ:  	void
*****************************************************************************/
void FIFOInit(void)
{
    PHY_RcvFIFO_Init();//   MSG_FROM_433

#if (RF_433M_INTERFACE_EN > 0u)
    PROCESS_INIT(&rcv433Prc,0,NORMAL,RF_433M_INTERFACE,decryptfifo_getEx,Msg_Process);			//����������������
    PROCESS_INIT(&snd433Prc,0,MANY_P,RF_433M_INTERFACE,decryptfifo_get,PHY_433Send);			//���ʹ���

#endif
#if (USART_INTERFACE_EN > 0u )
    PROCESS_INIT(&rcvUsartPrc,0,NORMAL,USART_INTERFACE,decryptfifo_getEx,Msg_Process);			//����������������
    PROCESS_INIT(&sndUsartPrc,0,NORMAL,USART_INTERFACE,decryptfifo_get,PHY_USARTSend);			//���ʹ���

#endif
#if (USB_INTERFACE_EN > 0u)
    PROCESS_INIT(&rcvUSBPrc,0,NORMAL,USB_INTERFACE,decryptfifo_getEx,Msg_Process);				//����������������
    PROCESS_INIT(&sndUSBPrc,0,NORMAL,USB_INTERFACE,decryptfifo_get,PHY_USBTSend);				//���ʹ���

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    PROCESS_INIT(&rcvIntnetPrc,0,NORMAL,INTERNET_INTERFACE,decryptfifo_getEx,Msg_Process);		//����������������
    PROCESS_INIT(&sndIntnetPrc,0,NORMAL,INTERNET_INTERFACE,decryptfifo_get,PHY_INTERNETTSend);	//���ʹ���

#endif
    Thread_Login(FOREVER, 0, 5, &DataSend_Thread);
}

/*****************************************************************************
  ������:  	DataSend_Thread
����˵��: 	���ݷ����߳�
	����:  	void
  ����ֵ:  	void
*****************************************************************************/
void MsgReviceProc(void)
{
#if (RF_433M_INTERFACE_EN > 0u)
    Process_FIFOData(&phy_rcv433fifo, &rcv433Prc);		//�������FIFO

#endif
#if (USART_INTERFACE_EN > 0u )
    Process_FIFOData(&phy_rcvUsartfifo, &rcvUsartPrc);	//�������FIFO

#endif
#if (USB_INTERFACE_EN > 0u)
    Process_FIFOData(&phy_rcvUSBfifo, &rcvUSBPrc);		//�������FIFO

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    Process_FIFOData(&rcvIntnetPrc, &phy_rcvIntnetfifo);//�������FIFO

#endif
}

/*****************************************************************************
  ������:  	DataSend_Thread
����˵��: 	���ݷ����߳�
	����:  	void
  ����ֵ:  	void
*****************************************************************************/
void DataSend_Thread(void)
{
#if (RF_433M_INTERFACE_EN > 0u)
    Process_FIFOData(&phy_snd433fifo, &snd433Prc);		//������FIFO

#endif
#if (USART_INTERFACE_EN > 0u )
    Process_FIFOData(&phy_sndUsartfifo, &sndUsartPrc);	//������FIFO

#endif
#if (USB_INTERFACE_EN > 0u)
    Process_FIFOData(&phy_sndUSBfifo, &sndUSBPrc);		//������FIFO

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    Process_FIFOData(&sndIntnetPrc, &phy_sndIntnetfifo);//������FIFO

#endif
}

/*******************************************************************
�� �� ����	Send_FIFOData
����˵���� 	��433�����е�����ͨ��433���ͳ�ȥ
��	  ���� 	fifo:	����Ŀ��FIFO
			prc:	���̴�����
�� �� ֵ��	TRUE/FALSE
*******************************************************************/
void Process_FIFOData(FIFO_t *fifo, Process_t *prc)
{
    switch (prc->flag)
    {
    case 0:
    {
        if (0 == (prc->len = prc->msggetfun(prc->data, fifo)))   			//��FIFO�л�ȡһ��ָ��ʧ��
        {
            break;
        }
        prc->level = 0;//Msg_Level(((Communication_t*)&prc->data[HEAD_SIZE])->aim.object.id);
        prc->mean = (prc->con == MANY_P ? gSysData.sendCnt : 1);			//��ȡ���ݴ������
    }
    case 1:
    {
        if (FALSE == (prc->procfun(prc->data, prc->len, prc->level)) || --prc->mean)   //���ݴ���δ���
        {
            prc->flag = 1;
            break;
        }
    }
    default:
    {
        prc->flag = 0;
    }
    break;
    }
}

/*******************************************************************
�� �� ����	Msg_Process
����˵���� 	��Ϣ������
��	  ���� 	sdata	:	����ͨѶ֡���������׵�ַ
			len		:	����֡�ĳ���
			level	:������Ϣ����ʱ����
�� �� ֵ��	TRUE/FALSE
*******************************************************************/
static BF_BOOLEAN Msg_Process(BF_INT08U *sdata, BF_INT08U len, BF_INT08U level)
{
    MsgResult_t result;
    const BF_INT08U msgfrom = *(sdata - 1);
    Communication_t *pMsg;
    pMsg = (Communication_t*)&sdata[HEAD_SIZE];
    //�˶�ͨ�Ű汾	pMsg->version;
    if ((result = Object_Check(sdata, len, &pMsg->aim, &pMsg->para,\
                               &sdata[HEAD_SIZE+sizeof(Communication_t)])) == COMPLETE)   //Ŀ��˶Գɹ�
    {
        /*ָ��ִ�� pMsg->para ; data*/
        result =  Msg_Execute(&pMsg->para, msgfrom);
        /*ָ���*/
        if (result != NO_ACK)
        {
            if (WRITEWITHACK == pMsg->para.msgType									//дӦ��
                    || READWITHACK == pMsg->para.msgType							//��Ӧ��
                    || ((WRITESUC == pMsg->para.msgType
                         || READSUC == pMsg->para.msgType) && result == COMPLETE) 	//д/���ɹ�Ӧ���Ҳ����ɹ�
                    || ((WRITEFAIL == pMsg->para.msgType
                         || READFAIL == pMsg->para.msgType) && result != COMPLETE)) //д/��ʧ��Ӧ���Ҳ���ʧ��
            {
                //��Ϣ��ҪӦ��
                pMsg->para.msgType = Msg_ReturnType(pMsg->para.msgType);			//��Ϣִ�н��
                Msg_Feedback(pMsg, msgfrom);
            }
        }
    }
    return TRUE;
}

/*******************************************************************************
�� �� ����	Msg_Feedback
����˵���� 	��Ϣ����
��	  ���� 	pMsg:	��Ϣ����
�� �� ֵ��	TRUE(�ظ�)/FALSE(���ظ�)
*******************************************************************************/
void Msg_Feedback(Communication_t *pMsg,const BF_INT08U msgfrom)
{
    const MsgSrcFIFOPtr_t* pMsgSrcFIFO = GetCurrentSrcFifoPtr(msgfrom);
	
	pMsg->version = COMMUNICATION_VERSION;
    pMsg->aim.userID = gSysData.userID;
    pMsg->aim.sourceID = gSysData.deviceID;

	//��Ϊ�㲥��Ϣ���߷���Ŀ�����Լ������,��Ҫѹ���Լ��Ľ��ն���
    if ((pMsg->aim.object.id == gSysData.deviceID) ||  (pMsg->aim.objectType > SINGLE_ACT))
    {
		if(!EncryptCmd_put(pMsgSrcFIFO->rcvfifoPtr, (BF_INT08U*)pMsg, sizeof(Communication_t) + pMsg->para.len))
		{
			Fault_Upload(FAULT_2 , pMsgSrcFIFO->rcvfault , NULL);	//��ʱ��������
		}
    }
	//Ŀ��ID�����Լ�
    if(pMsg->aim.object.id != gSysData.deviceID)  
    {
        if(!EncryptCmd_put(pMsgSrcFIFO->sndfifoPtr, (BF_INT08U*)pMsg, sizeof(Communication_t) + pMsg->para.len))
		{
			Fault_Upload(FAULT_2 , pMsgSrcFIFO->sndfault , NULL);	//��ʱ��������
		}
    }
}

/*******************************************************************
�� �� ����	Msg_Execute
����˵���� 	��Ϣִ��
��	  ���� 	data:	Ҫִ�е���������
			len;	���ݳ���
�� �� ֵ��	TRUE(�ظ�)/FALSE(���ظ�)
*******************************************************************/
MsgResult_t Msg_Execute(CMDPara_t *para,const BF_INT08U msgfrom)
{
    MsgResult_t result = NO_CMD;
    const AttrExe_st *pAttrExe = UnitTab[para->unit].attrExe;
    BF_INT08U ExeAttrNum = 0u;
    BF_INT08U ackLen = 0;

    if (para->unit < UnitCnt_Get())  		//ָ�Ԫû��Խ��	2014.01.08 Unarty add ��ֹ����Խ��
    {
        do   //���������б�
        {
            if (pAttrExe->cmd == para->cmd)  	//���Ժ�ƥ��OK
            {
                /*д����*/
                if (WRITEWITHACK == para->msgType
                        || WRITENACK == para->msgType
                        || WRITESUC  == para->msgType
                        || WRITEFAIL == para->msgType)
                {
                    if (pAttrExe->pSet_Attr != NULL)
                    {
                        if (!gUnitData[para->unit].able)  	//��Ԫ������
                        {
                            result = UNIT_DISABLE;
                        }
                        else
                        {
                            memmove(&para->cmd,&para->unit,3 + para->len);
							para->unit = msgfrom;
							result =  pAttrExe->pSet_Attr((UnitPara_t*)&para->unit);
							memmove(&para->unit,&para->cmd, 3);
                            if (result == OWN_RETURN_PARA)   //���д���������ز���
                            {
                                if (pAttrExe->cmd > 0xCF)
                                {
                                    result = COMPLETE;
                                }
                                ackLen = para->len;                          
                            }
                        }
                    }
                    else
                    {
                        result = NO_WRITE;	//û��дȨ��
                    }
                }
                /*������*/
                else if (READWITHACK == para->msgType
                         || READSUC == para->msgType
                         || READFAIL == para->msgType)
                {
                    if (pAttrExe->pGet_Attr != NULL)
                    {
                        memmove(&para->cmd,&para->unit,3 + para->len);
						para->unit = msgfrom;
						result = pAttrExe->pGet_Attr((UnitPara_t*)&para->unit, &ackLen, ((&para->len) + 2));
						memmove(&para->unit, &para->cmd, 3);
                    }
                    else
                    {
                        result = NO_READ;	//û�ж�Ȩ��
                    }
                }
                else
                {
                    result = MSG_TYPE_ERR;	//��Ϣ���ʹ���
                }
                break;
            }
        }
        while (((++ExeAttrNum) < UnitTab[para->unit].ExeAttrNum())    //2016.01.29 jay modify
                &&((++pAttrExe)->cmd < 0x0100));
    }

    para->len = ackLen + 1;
    *((&para->len)+1) = result;		//V1.8 ADD

    return result;
}

/*******************************************************************
�� �� ����	Msg_ReturnType
����˵���� 	��Ϣ��������
��	  ���� 	ִ����Ϣ����
�� �� ֵ��	������ϢӦ������
*******************************************************************/
BF_INT08U Msg_ReturnType(BF_INT08U msgType)
{
    switch (msgType)
    {
    case WRITEWITHACK:
    case WRITESUC:
    case WRITEFAIL:
        return ACKWRITE;
    case READWITHACK:
    case READSUC:
    case READFAIL:
        return READACK;
    default :
        break;
    }
    return 0;
}

///*******************************************************************
//�� �� ����	Msg_Level
//����˵���� 	��Ϣִ�еȼ�����Ҫ�����ݷ���
//��	  ���� 	��ϢĿ��
//�� �� ֵ��	��Ϣ�ȼ�
//*******************************************************************/
//BF_INT08U Msg_Level(BF_INT32U object)
//{
//    if (MSG_UPLOAD_ID == object)  	//�����ϱ�
//    {
//        return 2;
//    }
//    else if (DelayID == object)  	//Ŀ�����������ʱӦ��
//    {
//        return 3;
//    }
//    return 0;
//}
/**************************Copyright BestFu 2014-05-14*************************/

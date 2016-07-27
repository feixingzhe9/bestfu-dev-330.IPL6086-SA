/***************************Copyright BestFu 2014-05-14*************************
��	����	MsgPackage.c
˵	����	�豸������Ϣ������غ���
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.24
�ޡ��ģ�	����
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "BestFuLib.h"
#include "fifo.h"
#include "PivotalDataType.h"
#include "UserData.h"
#include "FifoDataProc.h"
#include "CMDProcess.h"
#include "MsgPackage.h"

//Private function declaration
static void Msg_Package(CMDAim_t *aim, CMDPara_t *para, BF_INT08U *data);

/*******************************************************************************
�� �� ����	Msg_Upload
����˵���� 	��Ϣ�ϱ�
��	  ���� 	unit:	��Ԫ��
			cmd��	���Ժ�
			len:	��������
			*data:	��������
�� �� ֵ��	��
*******************************************************************************/
void Msg_Upload(BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data)
{
    BF_INT08U buf[64];

    buf[0] = COMPLETE;	//����ͨ�ųɹ���ʶ
    buf[1] = unit;
    buf[2] = cmd;
    buf[3] = len;
    memcpy(&buf[4], data, len);

    Msg_UploadUnit( 0x00, 0x00, len + 4, buf);
}

/*******************************************************************************
�� �� ����	Msg_UploadUnit
����˵���� 	��Ԫ��Ϣ�ϱ�
��	  ���� 	unit:	��Ԫ��
			cmd��	���Ժ�
			len:	��������
			*data:	��������
�� �� ֵ��	��
*******************************************************************************/
void Msg_UploadUnit(BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data)
{
    CMDAim_t aim;
    CMDPara_t para;

    aim.objectType = SINGLE_ACT;
    aim.object.id = MSG_UPLOAD_ID;
    aim.actNum = 0;

    para.msgType = EVENT;
    para.unit = unit;
    para.cmd = cmd;
    para.len = len;
    Msg_Package(&aim, &para, data);
}
/*******************************************************************************
�� �� ����	Msg_Send
����˵���� 	��Ϣ����
��	  ���� 	objectType:		Ŀ���ַ����
			objectID��		Ŀ���ַ
			actNum:			�㲥���ͺ�
			unit:			��Ԫ��
			cmd��			���Ժ�
			len:			��������
			*data:			��������
�� �� ֵ��	��
*******************************************************************************/
void Msg_Send(ObjectType_t objecttype, BF_INT32U objectID, BF_INT16U actNum, BF_INT08U unit, BF_INT08U cmd, BF_INT08U len, BF_INT08U *data)
{
    CMDAim_t aim;
    CMDPara_t para;

    aim.objectType = objecttype;
    aim.object.id = objectID;			//����Ŀ���ַ
    aim.actNum = actNum;

    /*��Ϣ��������ֵ*/
    para.msgType = WRITENACK;			//��Ϣ���ͣ��¼�
    para.cmd = cmd;
    para.unit = unit;
    para.len = len;
    Msg_Package(&aim, &para, data);
}

/*******************************************************************************
�� �� ����	Msg_Package
����˵���� 	��Ϣ���
��	  ���� 	aim:	Ŀ������
			para:	ָ������
			*data:	��������
�� �� ֵ��	��
*******************************************************************************/
void Msg_Package(CMDAim_t *aim, CMDPara_t *para, BF_INT08U *data)
{
    BF_INT08U msg[250];
    Communication_t *pMsg = (Communication_t*)msg;

    memcpy(&pMsg->aim, aim, sizeof(CMDAim_t));
    pMsg->aim.serialNum = ++gSysData.serialNum;		//������Ϣ��ˮ��

    /*��Ϣ��������ֵ*/
    memcpy(&pMsg->para, para, sizeof(CMDPara_t));
    memcpy(&msg[sizeof(Communication_t)], data, para->len);
    Msg_Feedback(pMsg, RF_433M_INTERFACE);
}

/**************************Copyright BestFu 2014-05-14*************************/

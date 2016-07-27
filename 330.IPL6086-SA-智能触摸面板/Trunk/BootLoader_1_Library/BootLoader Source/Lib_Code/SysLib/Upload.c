/***************************Copyright BestFu 2014-05-14*************************
��	����	Upload.c
˵	����	�豸�����ϱ���غ���
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.07.03
�ޡ��ģ�	����
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "Thread.h"
#include "PivotalDataType.h"
#include "MsgPackage.h"
#include "Upload.h"

//Private variable define
static BF_INT08U Fault;

//Private Function declaration
static void FaultMsg(void);

/*******************************************************************************
�� �� ����	Upload_Fault
����˵���� 	�����쳣�ϱ�
��	  ���� 	upload: �쳣���������쳣ö�ٸ��
�� �� ֵ��	��
*******************************************************************************/
void Upload_Fault(Upload_t upload)
{
    BF_INT32U time;

    Fault = (BF_INT08U)upload;
    if (upload == USER_DATA_RESET)
    {
        time = 1000;
    }
    else
    {
        time = 5;
    }
    Thread_Login(ONCEDELAY, 0, time, &FaultMsg);
}
/*******************************************************************************
�� �� ����	FaultMsg
����˵���� 	�쳣��Ϣ��װ
��	  ���� 	upload: �쳣���������쳣ö�ٸ��
�� �� ֵ��	��
*******************************************************************************/
static void FaultMsg(void)
{
    Msg_Upload(0, 0xFF, 1, (BF_INT08U*)&Fault);
}
/**************************Copyright BestFu 2014-05-14*************************/

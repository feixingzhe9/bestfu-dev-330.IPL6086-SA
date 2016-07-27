/***************************Copyright BestFu 2014-05-14*************************
��	����	FaultManage.c
˵	����	�쳣�������
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.30
��  ��:     ����
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "Thread.h"
#include "Instruct.h"
#include "EEPROM.h"
#include "UserData.h"
#include "iwdg.h"
#include "Upload.h"
#include "FaultManage.h"

/*******************************************************************************
�� �� ����  FaultData_Init
����˵����  �쳣�¼��洢�ռ��ʼ��
��   ����   data:   �쳣���ش洢��ַ
�� �� ֵ��  ��
*******************************************************************************/
void FaultData_Init(void)
{
    WriteDataToEEPROMEx(FAULT_START_ADD, FAULT_SPACE, 0);
}

/*******************************************************************************
�� �� ����  FaultData_Save
����˵����  �쳣�¼��洢
��   ����   event�� �쳣�¼���
�� �� ֵ��  ��
*******************************************************************************/
void FaultData_Save(BF_INT08U event)
{
    BF_INT08U name;

    ReadDataFromEEPROM(FAULT_START_ADD + sizeof(name)*event,
                       sizeof(name), (BF_INT08U *)&name);     //��ȡ��ʶ
    name++;
    WriteDataToEEPROM(FAULT_START_ADD + sizeof(name)*event,
                      sizeof(name), (BF_INT08U *)&name);     //��ȡ��ʶ
}

/*******************************************************************************
�� �� ����  FaultData_Get
����˵����  �쳣�¼��洢
��   ����   data:   �쳣���ش洢��ַ
�� �� ֵ��  ��
*******************************************************************************/
void FaultData_Get(BF_INT08U *len, BF_INT08U *data)
{
    *len = FAULT_SPACE;
    ReadDataFromEEPROM(FAULT_START_ADD, *len, data);    //��ȡ��ʶ
}

/*******************************************************************
�� �� ����	Fault_Upload
����˵���� 	�쳣�ϱ�����
��	  ���� 	grade:	�쳣�ȼ�
			event:	�쳣�¼�
			fun:	������
�� �� ֵ��	��
*******************************************************************/
void Fault_Upload(FaultGrade_t grade, Fault_t event, void *fun)
{
    switch (grade)
    {
    case FAULT_0: 	//��߼������
        Upload_Fault(EEPROM_FAIL);
        Thread_Logout(&Instruct_Run);	//ע������ָʾ��
        break;	//��������
    case FAULT_1:	//ͨ�����ÿ��Իָ�
        IWDG_FeedOFF();		//����ι�����豸�Զ���λ
        break;
    case FAULT_2:	//ͨ��ע�ắ�����ûָ�
        if (fun != NULL)
        {
            Thread_Login(ONCEDELAY, 0, 5, fun);
        }
        break;
    default :
        break;
    }

    if (event != EEPROM_W_ERR && event != EEPROM_R_ERR)
    {
        FaultData_Save(event);
    }
}

/**************************Copyright BestFu 2014-05-14*************************/

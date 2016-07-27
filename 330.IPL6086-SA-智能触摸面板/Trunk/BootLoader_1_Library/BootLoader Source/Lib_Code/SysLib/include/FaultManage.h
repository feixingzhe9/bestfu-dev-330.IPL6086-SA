/***************************Copyright BestFu 2014-05-14*************************
��	����	FaultManage.h
˵	����	�쳣����ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.30  
��  ��:     ����
*******************************************************************************/
#ifndef __FAULTMANAGE_H
#define __FAULTMANAGE_H

/*********************************�������Ͷ���*********************************/
typedef enum
{
	FAULT_0,	//�쳣�ȼ����, �����޷�����ʹ��
	FAULT_1,	//ͨ�����ÿ��Իָ�
	FAULT_2,	//ͨ��fun����ָ������ݿ��Ը�λ
}FaultGrade_t;	//�쳣�ȼ�

typedef enum
{ 
	EEPROM_W_ERR,
	EEPROM_R_ERR,
	SEND_433_FIFO_OVER,			//����433�������
	RECV_433_FIFO_OVER,			//����433�������
	SI4432_TX_ERR,				//�������ݷ��ʹ���
	SI4432_RX_ERR,				//�������ݽ��մ���
	PVD_ERR,	       			//PVD�쳣����δ���룩
	DEV_RESTART,       			//����λ
	SEND_USART_FIFO_OVER,		//����433�������
	RECV_USART_FIFO_OVER,		//����433�������
	SEND_USB_FIFO_OVER,			//����433�������
	RECV_USB_FIFO_OVER,			//����433�������
	SEND_INTERNET_FIFO_OVER,	//����433�������
	RECV_INTERNET_FIFO_OVER,	//����433�������	
}Fault_t;

/**********************************��������***********************************/
void Fault_Upload(FaultGrade_t grade, Fault_t event, void *fun);
void FaultData_Get(BF_INT08U *len, BF_INT08U *data);
void FaultData_Save(BF_INT08U event);
void FaultData_Init(void);

#endif		//FaultManage.h end
/**************************Copyright BestFu 2014-05-14*************************/

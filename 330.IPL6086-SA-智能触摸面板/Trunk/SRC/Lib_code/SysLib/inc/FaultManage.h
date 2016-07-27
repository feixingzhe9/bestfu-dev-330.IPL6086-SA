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

#include "BF_type.h"

typedef enum
{
	FAULT_0,	//�쳣�ȼ����, �����޷�����ʹ��
	FAULT_1,	//ͨ�����ÿ��Իָ�
	FAULT_2,	//ͨ��fun����ָ������ݿ��Ը�λ
}FaultGrade_t;	//�쳣�ȼ�

typedef enum
{  //����ʱ����8���쳣��Ǳ��棬�������쳣�ɼ�������   yanhuan adding 2015/10/09
	EEPROM_W_ERR,
	EEPROM_R_ERR,
	SEND_433_FIFO_OVER,		//����433�������
	RECV_433_FIFO_OVER,		//����433�������
	SI4432_TX_ERR,	//�������ݷ��ʹ���
	SI4432_RX_ERR,	//�������ݽ��մ���
	PVD_ERR,	       //PVD�쳣����δ���룩
	DEV_RESTART,       //����λ
}Fault_t;

void Fault_Upload(FaultGrade_t grade, Fault_t event, void *fun);

#endif		//FaultManage.h end
/**************************Copyright BestFu 2014-05-14*************************/

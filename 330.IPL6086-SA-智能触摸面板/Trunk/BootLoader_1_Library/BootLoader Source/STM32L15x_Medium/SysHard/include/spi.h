/***************************Copyright BestFu 2014-05-14*************************
��	����    SPI.h
˵	����    SPI����Ĵ�������
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2013-08-24
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __SPI_H
#define __SPI_H

/*********************************�Զ����*************************************/				    
// SPI�����ٶ����� 
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7

/**********************************��������************************************/ 						  	    													  
void SPI2_Init(void);			 //��ʼ��SPI2��
void SPI2_SetSpeed(BF_INT08U SpeedSet); //����SPI2�ٶ�   
BF_INT08U SPI2_ReadWriteByte(BF_INT08U TxData);//SPI2���߶�дһ���ֽ�
		 
#endif
/**************************Copyright BestFu 2014-05-14*************************/

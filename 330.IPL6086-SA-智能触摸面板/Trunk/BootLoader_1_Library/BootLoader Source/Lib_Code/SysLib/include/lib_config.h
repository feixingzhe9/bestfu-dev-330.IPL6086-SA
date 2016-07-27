/***************************Copyright BestFu 2016-04-13*************************
��	����	lib_config.h
˵	����	�������ļ��������ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	Jay
��	�ڣ�	2016-04-13 
�ޡ��ģ�	None
*******************************************************************************/
#ifndef LIB_CONFIG_H
#define LIB_CONFIG_H

/*Max Thread number is MAX_THREAD_NUM*/
#define MAX_THREAD_NUM      			(20)		//����߳����� 

/* ����flash���ܹ��� */
#define FLASH_ENCRYPTION_EN				(0u)		//ʹ��flash���ܹ���

/* �����豸��ַ���蹦�� */
#define DEVICE_ADDR_RESET_FEATURE_EN	(1u)		//ʹ�ܵ�ַ���蹦��

/* ������Ϣ����Դ */
#define RF_433M_INTERFACE_EN			(1u)		//����433MͨѶ
#define USART_INTERFACE_EN				(1u)		//��������ͨѶ
#define USB_INTERFACE_EN				(0u)		//����USBͨѶ
#define INTERNET_INTERFACE_EN			(0u)		//��������ͨѶ

/* ͨѶ�Ļ�����д�С���� */
#if (RF_433M_INTERFACE_EN > 0u)
#define RF_433M_INTERFACE_BUF_SIZE		(0x400)		//1k
#endif
#if (USART_INTERFACE_EN > 0u )
#define USART_INTERFACE_BUF_SIZE		(0x400)		//1k
#endif
#if (USB_INTERFACE_EN > 0u)
#define USB_INTERFACE_BUF_SIZE			(0x400)		//1k
#endif
#if (INTERNET_INTERFACE_EN > 0u)
#define INTERNET_INTERFACE_BUF_SIZE		(0x400)		//1k
#endif


#if (!(defined(RF_433M_INTERFACE_EN)||(defined(USART_INTERFACE_EN))||\
	(defined(USB_INTERFACE_EN))||(defined(INTERNET_INTERFACE_EN))||\
	((0u == RF_433M_INTERFACE_EN)&&(0u == USART_INTERFACE_EN)&&\
	(0u == USB_INTERFACE_EN)&&(0u == INTERNET_INTERFACE_EN))))
#error "Please define Interface mode!"
#endif

#if (MAX_THREAD_NUM >= 25)
#error  "Max MAX_THREAD_NUM is 25"
#endif

#endif

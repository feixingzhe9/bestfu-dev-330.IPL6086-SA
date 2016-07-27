/***************************Copyright BestFu 2016-04-14*************************
��	����	PhyMsgProc.c
˵	����	������շ���ϢԤ�����ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2016-04-14
��  ��:     NONE
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "SI4432.h"
#include "fifo.h"
#include "crc.h"
#include "iwdg.h"
#include "DataEncrypt.h"
#include "PivotalDataType.h"
#include "FaultManage.h"
#include "BestFuLib.h"
#include "FifoDataProc.h"
#include "PhyMsgProc.h"
#include "usart.h"

//using save message source pointer
static MsgSrcFIFOPtr_t MsgSrcFIFOPtr[4];

//variable define
#if (RF_433M_INTERFACE_EN > 0u)
FIFO_t phy_rcv433fifo;
FIFO_t phy_snd433fifo;
static BF_INT08U phy_rcv433Buf[RF_433M_INTERFACE_BUF_SIZE];
static BF_INT08U phy_snd433Buf[RF_433M_INTERFACE_BUF_SIZE];

#endif
#if (USART_INTERFACE_EN > 0u )
FIFO_t phy_rcvUsartfifo;
FIFO_t phy_sndUsartfifo;
static BF_INT08U phy_rcvUsartBuf[USART_INTERFACE_BUF_SIZE];
static BF_INT08U phy_sndUsartBuf[USART_INTERFACE_BUF_SIZE];

#endif
#if (USB_INTERFACE_EN > 0u)
FIFO_t phy_rcvUSBfifo;
FIFO_t phy_sndUSBfifo;
static BF_INT08U phy_rcvUSBBuf[USB_INTERFACE_BUF_SIZE];
static BF_INT08U phy_sndUSBBuf[USB_INTERFACE_BUF_SIZE];

#endif
#if (INTERNET_INTERFACE_EN > 0u)
FIFO_t phy_rcvIntnetfifo;
FIFO_t phy_sndIntnetfifo;
static BF_INT08U phy_rcvInternetBuf[INTERNET_INTERFACE_BUF_SIZE];
static BF_INT08U phy_sndInternetBuf[INTERNET_INTERFACE_BUF_SIZE];

#endif

/*******************************************************************************
�� �� ����  PHY_RcvFIFO_Init
����˵����  PHY����ն��г�ʼ��
��	  ����  NONE
�� �� ֵ��  ��ʼ����� TRUE/FALSE
*******************************************************************************/
void PHY_RcvFIFO_Init(void)
{
#if (RF_433M_INTERFACE_EN > 0u)
    fifo_Init(&phy_rcv433fifo,phy_rcv433Buf,RF_433M_INTERFACE_BUF_SIZE);
    fifo_Init(&phy_snd433fifo,phy_snd433Buf,RF_433M_INTERFACE_BUF_SIZE);

#endif
#if (USART_INTERFACE_EN > 0u )
    fifo_Init(&phy_rcvUsartfifo, phy_rcvUsartBuf,USART_INTERFACE_BUF_SIZE);
    fifo_Init(&phy_sndUsartfifo, phy_sndUsartBuf,USART_INTERFACE_BUF_SIZE);

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    fifo_Init(&phy_rcvUSBfifo, phy_rcvUSBBuf,USB_INTERFACE_BUF_SIZE);
    fifo_Init(&phy_sndUSBfifo, phy_sndUSBBuf,USB_INTERFACE_BUF_SIZE);

#endif
#if (INTERNET_INTERFACE_EN > 0u)
    fifo_Init(&phy_rcvIntnetfifo, phy_rcvInternetBuf,INTERNET_INTERFACE_BUF_SIZE);
    fifo_Init(&phy_sndIntnetfifo, phy_sndInternetBuf,INTERNET_INTERFACE_BUF_SIZE);

#endif
}

/*******************************************************************************
�� �� ����	GetCurrentSrcFifoPtr
����˵���� 	��ȡ��ǰ��ϢԴ���շ���fifo
��	  ���� 	msgfrom	:��Ϣ��Դ		
�� �� ֵ��	���ر��浱ǰ��Ϣ��Դfifo�����ݽṹ��ַ
˵    ����	�˺����жϲ���ʹ��
*******************************************************************************/
MsgSrcFIFOPtr_t*  GetCurrentSrcFifoPtr(const BF_INT08U msgfrom)
{
	MsgSrcFIFOPtr_t* pMsgSrcFIFO;
	
	switch(msgfrom)
	{
#if (USART_INTERFACE_EN > 0u )
	case USART_INTERFACE:
	{
		MsgSrcFIFOPtr[0].rcvfault	 = RECV_USART_FIFO_OVER;
		MsgSrcFIFOPtr[0].sndfault	 = SEND_USART_FIFO_OVER;
		MsgSrcFIFOPtr[0].rcvfifoPtr = (FIFO_t*)&phy_rcvUsartfifo;
		MsgSrcFIFOPtr[0].sndfifoPtr = (FIFO_t*)&phy_sndUsartfifo;
		pMsgSrcFIFO = &MsgSrcFIFOPtr[0];
	}
	break;
#endif
#if (USB_INTERFACE_EN > 0u)
	case USB_INTERFACE:
	{
		MsgSrcFIFOPtr[1].rcvfault	 = RECV_USB_FIFO_OVER;
		MsgSrcFIFOPtr[1].sndfault	 = SEND_USB_FIFO_OVER;
		MsgSrcFIFOPtr[1].rcvfifoPtr = (FIFO_t*)&phy_rcvUSBfifo;
		MsgSrcFIFOPtr[1].sndfifoPtr = (FIFO_t*)&phy_sndUSBfifo;
		pMsgSrcFIFO = &MsgSrcFIFOPtr[1];
	} break;
#endif
#if (INTERNET_INTERFACE_EN > 0u)
	case INTERNET_INTERFACE:
	{
		MsgSrcFIFOPtr[2].rcvfault	 = RECV_INTERNET_FIFO_OVER;
		MsgSrcFIFOPtr[2].sndfault	 = SEND_INTERNET_FIFO_OVER;
		MsgSrcFIFOPtr[2].rcvfifoPtr = (FIFO_t*)&phy_rcvIntnetfifo;
		MsgSrcFIFOPtr[2].sndfifoPtr = (FIFO_t*)&phy_sndIntnetfifo;
		pMsgSrcFIFO = &MsgSrcFIFOPtr[2];
	} break;
#endif
#if (RF_433M_INTERFACE_EN > 0u)
	case RF_433M_INTERFACE:
#endif
	default:
	{
		MsgSrcFIFOPtr[3].rcvfault	 = RECV_433_FIFO_OVER;
		MsgSrcFIFOPtr[3].sndfault	 = SEND_433_FIFO_OVER;
		MsgSrcFIFOPtr[3].rcvfifoPtr = (FIFO_t*)&phy_rcv433fifo;
		MsgSrcFIFOPtr[3].sndfifoPtr = (FIFO_t*)&phy_snd433fifo;
		pMsgSrcFIFO = &MsgSrcFIFOPtr[3];
	}
	break;
	}
	return ((MsgSrcFIFOPtr_t*)pMsgSrcFIFO);
}

#if (RF_433M_INTERFACE_EN > 0u)
/*******************************************************************************
�� �� ����  BF_BOOLEAN PHY_433Send(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
����˵����  PHY��433��Ϣ���ͺ���
��	  ����  buf:  ��������Ϣ����ָ��
            len:  ��Ϣ�ĳ���
            level:  ��Ϣ�ȼ�
�� �� ֵ��  ��� TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN PHY_433Send(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
{
    return Si4432_Send(&buf[HEAD_SIZE], len - HEAD_SIZE - TAIL_SIZE, level);
}

#endif

#if (USART_INTERFACE_EN > 0u )
/*******************************************************************************
�� �� ����  BF_BOOLEAN PHY_USARTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
����˵����  PHY�㴮����Ϣ���ͺ���
��	  ����  buf:  ��������Ϣ����ָ��
            len:  ��Ϣ�ĳ���
            level:  ��Ϣ�ȼ�
�� �� ֵ��  ��� TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN PHY_USARTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
{

    return USART_Send(buf,len,level);
}

#endif


#if (INTERNET_INTERFACE_EN > 0u)
/*******************************************************************************
�� �� ����  BF_BOOLEAN PHY_USBTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
����˵����  PHY��USB��Ϣ���ͺ���
��	  ����  buf:  ��������Ϣ����ָ��
            len:  ��Ϣ�ĳ���
            level:  ��Ϣ�ȼ�
�� �� ֵ��  ��� TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN PHY_USBTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
{


}
#endif


#if (INTERNET_INTERFACE_EN > 0u)
/*******************************************************************************
�� �� ����  BF_BOOLEAN PHY_INTERNETTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
����˵����  PHY��������Ϣ���ͺ���
��	  ����  buf:  ��������Ϣ����ָ��
            len:  ��Ϣ�ĳ���
            level:  ��Ϣ�ȼ�
�� �� ֵ��  ��� TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN PHY_INTERNETTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level)
{


}

#endif

/**************************Copyright BestFu 2016-05-14*************************/

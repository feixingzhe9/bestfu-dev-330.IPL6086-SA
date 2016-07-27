/***************************Copyright BestFu 2016-04-14*************************
��	����	PhyMsgProc.h
˵	����	������շ���ϢԤ����ͷ�ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2016-04-14
��  ��:     NONE
*******************************************************************************/

#ifndef __PHY_MSG_PROC_H	
#define __PHY_MSG_PROC_H

/********************************���ݽṹ����**********************************/
#pragma pack(1)
typedef struct
{
	Fault_t rcvfault;
	Fault_t sndfault;
	FIFO_t* rcvfifoPtr;
	FIFO_t* sndfifoPtr;	
}MsgSrcFIFOPtr_t;
#pragma pack()

/********************************�ⲿ��������**********************************/
//variable define 
#if (RF_433M_INTERFACE_EN > 0u)
extern FIFO_t phy_rcv433fifo;
extern FIFO_t phy_snd433fifo;

#endif
#if (USART_INTERFACE_EN > 0u )
extern FIFO_t phy_rcvUsartfifo;
extern FIFO_t phy_sndUsartfifo;
	
#endif
#if (USB_INTERFACE_EN > 0u)
extern FIFO_t phy_rcvUSBfifo;
extern FIFO_t phy_sndUSBfifo;
	
#endif
#if (INTERNET_INTERFACE_EN > 0u)
extern FIFO_t phy_rcvIntnetfifo;
extern FIFO_t phy_sndIntnetfifo;

#endif

/**********************************��������***********************************/                         
void PHY_RcvFIFO_Init(void);
BF_INT32U PHY_RcvMsgProc(FIFO_t *fifo);
MsgSrcFIFOPtr_t*  GetCurrentSrcFifoPtr(const BF_INT08U msgfrom);

#if (RF_433M_INTERFACE_EN > 0u)
BF_BOOLEAN PHY_433Send(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);

#endif

#if (USART_INTERFACE_EN > 0u )
BF_BOOLEAN PHY_USARTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);

#endif


#if (INTERNET_INTERFACE_EN > 0u)
BF_BOOLEAN PHY_USBTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);

#endif

#if (INTERNET_INTERFACE_EN > 0u)
BF_BOOLEAN PHY_INTERNETTSend(BF_INT08U *buf, BF_INT08U len, BF_INT08U level);
	
#endif

#endif //PreRcvMsg.h end
/**************************Copyright BestFu 2016-04-14*************************/

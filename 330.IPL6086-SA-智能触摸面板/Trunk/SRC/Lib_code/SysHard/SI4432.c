/***************************Copyright BestFu 2014-05-14*************************
��	����    SI4432.c
˵	����    SI4432�����ļ�
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2013-09-30
�ޡ��ģ�	2014.07.23	Seven  amend	���ݾɿ�
            **2014.07.25  Seven Amed:
	        ��������ݰ����ȣ�250  (ע���ڴ�ռ�,��ֹ���)
			2014.10.08 UnartyChen 	����ָ���֮��Ĺ̶����ʱ��
			2014.10.17 UnartyChen	�޸��豸�ϱ������ֹ���߼����߼�
			2014.11.13 UnartyChen	ȥ�����ݽ���CRC�쳣��¼����ΪCRC�����			�ʷǳ��ߣ���ʱ��¼������û�����壩��
*******************************************************************************/

#include "si4432.h"
#include "GPIO.h"
#include "sys.h"
#include "delay.h"
#include "SPI.h"
#include "cmdProcess.h"
#include "BF_type.h"
#include "Updata.h"
#include "SysHard.h"
#include "Thread.h"
#include "FaultManage.h"
#include "SysTick.h"
#include "Sign433.h"
#include "MsgPackage.h"
#include "DataEncrypt.h"

#define OVERTIME	0x1000	//���ͳ�ʱʱ�� 
#define BUSYTIME	1000	//�������æ���ʱ��
#define IDLETIME	100		//����������ʱ��
#define SPACETIME	200		//ָ��ͼ��ʱ������λ��ms
#define CENTER_FREQ  (82) //��׼Ƶ�ʣ�82-64��*10+240+(3) = 423MHZ
#define Crystal_C         0x79////Cint = 1.8 pF + 0.085 pF x xlc[6:0] + 3.7 pF x xtalshift

#define SHORT_SIZE    (35-12) //Լ����֡��̳���ΪС��23bytes

static u8 SI4432_RSSI;			//
static u8 SI4432SendEn;		//����ʹ�ܱ�ʶ��2014.10.08 Unarty Add
#ifndef SI4432_THRESHOLD
	static u8 Threshold = 160;			//�����ż�ֵ
#endif 
//������ӣ�����RSSI��������
u8 TestRecRSSI[MAX_FRAME_NUM];
u8 TestRecFlag = 0;

//Freq_433M_920KHz
const u8 Si4432Cfg_Tab[21] = {0x2B,0x40,0x0A,0x03,0x68,0x01,0x3A,0x93,0x02,0xF6,
							  0x1D,0x80,0x60,0x4E,0xA5,0x2D,0x23,0x0D,0x53,0x62,0x00};

static u8 Check_433M_Busy(u8 level);
static void Si4432_PortInit(void);				  
static u8   SPI_RW_Reg(u8 addr , u8 data);
static void Si4432_PortInit(void);
static void SetMode_RX(void);
static void SetMode_TX(void);
#ifndef SI4432_THRESHOLD
static u8 Si4432_Threshold(void);
#endif
static void Si4432_Fault(u8 event);
static void SI4432_SendEn(void);	//2014.10.08 Unarty Add
						
//�ź�ָʾ��
#if SIGNAL == 1
	#define TX		(0x55)
	#define RX		(0xaa)
	static void Si4332_Signal_Insturct(u8 state);
	static void Signal_Run(void);
	/*******************************************************************************
	�� �� ��:  	Signal_Run
	����˵��: 	�ź�ָʾ
	��    ��:  	��		
	�� �� ֵ:  	��
	*******************************************************************************/
	void Signal_Run(void)
	{
		GPIOx_Rvrs(SIGNAL_PORT, SIGNAL_PIN);
	}
	/*******************************************************************************
	�� �� ��:  	SI4332_Signal_Insturct
	����˵��: 	433�����ź�ָʾ
	��    ��:  	state��	433ģ��״̬ö��ֵ			
	�� �� ֵ:  	��
	*******************************************************************************/
	static void Si4332_Signal_Insturct(u8 state)
	{
		SIGNAL_OFF;
		if (state == TX)
		{
			SIGNAL_ON;
			Thread_Login(ONCEDELAY, 0, 500, &Signal_Run);
		}
		else
		{
			Thread_Login(MANY, 20, 25, &Signal_Run);
		}		
	}
#endif

/*******************************************************************************
�� �� ��:  	SetRecTestStatus
����˵��: 	�������߲��Ա�־
��    ��:  	flagΪ��0ʱ��׼���ñ�־��flagΪ0ʱ��δ׼���ñ�־	
�� �� ֵ:  	��
*******************************************************************************/
void SetRecTestStatus(u8 flag)
{
	TestRecFlag = flag ? 1 : 0;
}
/*******************************************************************************
�� �� ��:  GetRSSIPara
����˵��: 	��ȡRSSI����
��    ��:  	��
�� �� ֵ:  	RSSI���ݴ洢�׵�ַ
*******************************************************************************/
u8  *GetRSSIPara(void)
{
	return TestRecRSSI;
}
/*******************************************************************************
�� �� ��:  GetThresholdPara
����˵��: 	��ȡͨ���ż�ֵ
��    ��:  	��
�� �� ֵ:  ͨ���ż�ֵ
*******************************************************************************/
u8  GetThresholdPara(void)
{
#ifndef SI4432_THRESHOLD
	return Threshold ;
#else
	return SI4432_THRESHOLD;//�����ż�ֵ
#endif 	
}
/*******************************************************************************
�� �� ��:  	u8 SPI_RW_Reg(u8 addr,u8 data)
����˵��:  	ͨ��SPI���������ֽ�
��    ��:  	u8 addr - �Ĵ�����ַ
			u8 data - �Ĵ����趨ֵ 			
�� �� ֵ:  	��
*******************************************************************************/
static u8 SPI_RW_Reg(u8 addr,u8 data)
{
	u8 status;
    
	nSEL(0);	//��433Ƭѡ 
	SPI2_ReadWriteByte(addr);
	status = SPI2_ReadWriteByte(data);
	nSEL(1);    //��433Ƭѡ
        
	return status;
}

/*******************************************************************************
�� �� ��:  	void Si4432_PortInit(void)
����˵��: 	��ʼ��si4432��Ҫ�õ���GPIO��SPI�ܽ�
��    ��:  	�� 			
�� �� ֵ:  	��
*******************************************************************************/
static void Si4432_PortInit(void)
{
	EXTI->IMR &= ~(nIRQ_PIN);
	GPIOx_Cfg(SDN_PORT, SDN_PIN, OUT_PP_2M);
    GPIOx_Cfg(nSEL_PORT, nSEL_PIN, OUT_PP_2M);
    GPIOx_Cfg(nIRQ_PORT, nIRQ_PIN, IN_FLOATING);
    GPIOx_Cfg(nGPIO2_PORT, nGPIO2_PIN, IN_FLOATING);
    GPIOx_Set(nIRQ_PORT, nIRQ_PIN);		//����
    GPIOx_Set(nGPIO2_PORT, nGPIO2_PIN);	//����
#if SIGNAL == 1
	GPIOx_Cfg(SIGNAL_PORT, SIGNAL_PIN, OUT_PP_2M);
	SIGNAL_OFF;
#endif
#if	RF_SWITCH == 1
	GPIOx_Cfg(RF_SWITCH_PORT, RF_SWITCH_PIN, OUT_PP_2M);
	RF_SWITCH_ON;
#endif
	
	SPI2_Init();
	SPI2_SetSpeed(4);		//32M/(1<<(n+1)) 	
}

/*******************************************************************************
�� �� ��:  void Si4432_Init(void)
����˵��:  ��ʼ��SI4432������MCU�ܽ����ã���SI4432ͨ����������Ϊ9600bps
��    ��:  ��
�� �� ֵ:  ��
*******************************************************************************/
void Si4432_Init(void)
{
    Si4432_PortInit();
    nSEL(1);
    SDN(1);			//433ģ��Ĵ�������
	delay_ms(2);
	SDN(0);
	delay_ms(50);
	SPI_RW_Reg(R_REG | 0x03, 0x00);	 //��RFģ���ж�	
	SPI_RW_Reg(R_REG | 0x04, 0x00);	 
	SPI_RW_Reg(W_REG | 0x07, 0x80);  //Si4432���мĴ����ָ���ʼֵ 
	delay_ms(2);
	SPI_RW_Reg(W_REG | 0x07, 0x00);  //Set sRFStandBy mode
	SPI_RW_Reg(W_REG | 0x05, 0x00);	 //Disenable all Interrupt1
	SPI_RW_Reg(W_REG | 0x06, 0x00);  //Disenable all Interrupt2
    SPI_RW_Reg(R_REG | 0x03, 0x00);	 
	SPI_RW_Reg(R_REG | 0x04, 0x00);	 //��RFģ���ж�	
	
	SPI_RW_Reg(W_REG | 0x09, Crystal_C);  //���ص���  Cint = 1.8 pF + 0.085 pF x xlc[6:0] + 3.7 pF x xtalshift
	SPI_RW_Reg(W_REG | 0x0a, 0x06);  //GPIOclock = 1M
	SPI_RW_Reg(W_REG | 0x0b, 0xea);  //GPIO 0 ������ͨ�����
	SPI_RW_Reg(W_REG | 0x0c, 0xea);  //GPIO 1 ������ͨ�����
	SPI_RW_Reg(W_REG | 0x0d, 0xfc);  //Clear Channel Assessment	
	SPI_RW_Reg(W_REG | 0x0e, 0x00);	 //����GPIO�ܽ����״̬
	
	SPI_RW_Reg(W_REG | 0x1C, Si4432Cfg_Tab[0]);  //IFFilterBandwidth
	SPI_RW_Reg(W_REG | 0x1D, Si4432Cfg_Tab[1]);  //AFCLoopGearshiftOverride
	SPI_RW_Reg(W_REG | 0x1E, Si4432Cfg_Tab[2]);  //AFCTimingControl
	SPI_RW_Reg(W_REG | 0x1F, Si4432Cfg_Tab[3]);  //ClockRecoveryGearshiftOverride
	SPI_RW_Reg(W_REG | 0x20, Si4432Cfg_Tab[4]);  //ClockRecoveryOversamplingRatio
	SPI_RW_Reg(W_REG | 0x21, Si4432Cfg_Tab[5]);  //ClockRecoveryOffset2
	SPI_RW_Reg(W_REG | 0x22, Si4432Cfg_Tab[6]);  //ClockRecoveryOffset1
	SPI_RW_Reg(W_REG | 0x23, Si4432Cfg_Tab[7]);  //ClockRecoveryOffset0
	SPI_RW_Reg(W_REG | 0x24, Si4432Cfg_Tab[8]);  //ClockRecoveryTimingLoopGain1
	SPI_RW_Reg(W_REG | 0x25, Si4432Cfg_Tab[9]);  //ClockRecoveryTimingLoopGain0
	SPI_RW_Reg(W_REG | 0x2A, Si4432Cfg_Tab[10]); //AFCLimiter
	
	//��������ݰ���Preamble| Syn Word | Tx Header| Packlength| Data |CRC
	SPI_RW_Reg(W_REG | 0x30, 0xAC);	 //CRC Data Only Enable CCITT
	SPI_RW_Reg(W_REG | 0x32, 0x00);  //No broadcast address enable / No Received Header check
	SPI_RW_Reg(W_REG | 0x33, 0x02);  //No TX/RX header / Synchronization Word 3 and 2
//	SPI_RW_Reg(W_REG | 0x32, 0xff);  //byte 0,1,2,3 ��Ϊͷ��
//	SPI_RW_Reg(W_REG | 0x33, 0x42);  //ʹ��Header byte 0,1,2,3 Ϊͷ��  /  ͬ����ʹ�� 3,2 

	SPI_RW_Reg(W_REG | 0x34, 16);	 //���÷��� ��ͷPreamble  16*4bit = 64bit  = 8byte
	SPI_RW_Reg(W_REG | 0x35, 5<<3);  //���ý��� ��ͷPreamble �����Ŀ,����:   5*4bit=20bit=2.5byte

	//ͬ����Ϊ 0x2DD4
	SPI_RW_Reg(W_REG | 0x36, 0x2D);  //Synchronization Word 3   OK
	SPI_RW_Reg(W_REG | 0x37, 0xD4);  //Synchronization Word 2   OK
	SPI_RW_Reg(W_REG | 0x38, 0x00);  //Synchronization Word 1   NULL
	SPI_RW_Reg(W_REG | 0x39, 0x00);	 //Synchronization Word 0   NULL
	//ͷ�볤���ѹر� -- ����Ч
	SPI_RW_Reg(W_REG | 0x3a, 'B');   //�����ͷ��Ϊ�� ��BEST"
	SPI_RW_Reg(W_REG | 0x3b, 'E');
	SPI_RW_Reg(W_REG | 0x3c, 'S');
	SPI_RW_Reg(W_REG | 0x3d, 'T');
	SPI_RW_Reg(W_REG | 0x3f, 'B');   //��ҪУ���ͷ��Ϊ����BEST"
	SPI_RW_Reg(W_REG | 0x40, 'E');
	SPI_RW_Reg(W_REG | 0x41, 'S');
	SPI_RW_Reg(W_REG | 0x42, 'T');

	SPI_RW_Reg(W_REG | 0x43, 0xff);  //ͷ��1,2,3,4 ������λ����ҪУ��
	SPI_RW_Reg(W_REG | 0x44, 0xff);  // 
	SPI_RW_Reg(W_REG | 0x45, 0xff);  // 
	SPI_RW_Reg(W_REG | 0x46, 0xff);  // 
	
	SPI_RW_Reg(W_REG | 0x58, Si4432Cfg_Tab[11]);  //ChargepumpCurrentTrimming_Override
	SPI_RW_Reg(W_REG | 0x69, Si4432Cfg_Tab[12]);  //AGCOverride1
	SPI_RW_Reg(W_REG | 0x6E, Si4432Cfg_Tab[13]);  //TXDataRate1
	SPI_RW_Reg(W_REG | 0x6F, Si4432Cfg_Tab[14]);  //TXDataRate0
	SPI_RW_Reg(W_REG | 0x70, Si4432Cfg_Tab[15]);  //ModulationModeControl1
	SPI_RW_Reg(W_REG | 0x71, Si4432Cfg_Tab[16]);  //ModulationModeControl2
	SPI_RW_Reg(W_REG | 0x72, Si4432Cfg_Tab[17]);  //FrequencyDeviation	
	
#ifndef CHANNEL_10M_EN
	//433�ŵ�ƫ������,1M
		SPI_RW_Reg(W_REG | 0x75, CENTER_FREQ); //��׼Ƶ��423MHZ
	#ifdef SI4432_CH
		SPI_RW_Reg(W_REG | 0x79, (SI4432_CH-73)*10);
	#else
		SPI_RW_Reg(W_REG | 0x79, (Channel_Get()-73)*10);  //Ƶ������ 434
	#endif
		SPI_RW_Reg(W_REG | 0x7A, 0x0A);  //FrequencyHoppingStepSize 100KHZ
#else
	//433�ŵ�ƫ������,10M
	#ifdef SI4432_CH
		SPI_RW_Reg(W_REG | 0x75, SI4432_CH);
	#else
		SPI_RW_Reg(W_REG | 0x75, Channel_Get());  //Ƶ������ 434
	#endif
		SPI_RW_Reg(W_REG | 0x7A, 0x00);  //FrequencyHoppingStepSize
#endif
	
	SPI_RW_Reg(W_REG | 0x73, 0x00);  //FrequencyOffset1
	SPI_RW_Reg(W_REG | 0x74, 0x00);  //FrequencyOffset2
	SPI_RW_Reg(W_REG | 0x76, Si4432Cfg_Tab[19]);  //NominalCarrierFrequency1
	SPI_RW_Reg(W_REG | 0x77, Si4432Cfg_Tab[20]);  //NominalCarrierFrequency0
	SPI_RW_Reg(W_REG | 0x6D, 0x07);	 //���͹��ʣ�0��7�� --max
#ifdef SI4432_THRESHOLD 	
	SPI_RW_Reg(W_REG | 0x27, SI4432_THRESHOLD);    //���ͨ���Ƿ�æ���ź�ǿ���ż�	
#else
	SPI_RW_Reg(W_REG | 0x27, Threshold);
	Thread_Login(RESULT, 0, 400, &Si4432_Threshold);	//���¼����ź��ż�
#endif

    SPI_RW_Reg(W_REG | 0x7c, 30);   //TX FIFO Almost Full Threshold   = 30
	SPI_RW_Reg(W_REG | 0x7d, 12);   //TX FIFO Almost empty Threshold  = 12
    SPI_RW_Reg(W_REG | 0x7E, 50);   //RX FIFO Almost Full Threshold   = 50

	Ex_NVIC_Config(nIRQ_PORT, 10, FTIR); 	//�½��ش��� PB10 - nIRQ
	MY_NVIC_Init(2, 0, EXTI15_10_IRQn, 2);	//��ռ2�������ȼ�0����2	  
	EXTI->IMR &= ~(nIRQ_PIN);

	Si4432_Idle();
	SetMode_RX();
	SI4432_SendEn();
}

/*******************************************************************************
�� �� ��:  Si4432_Idle
����˵��:  ����433ģ��������״̬
��    ��:  ��
�� �� ֵ:  ��
*******************************************************************************/
void Si4432_Idle(void)
{
	EXTI->IMR &= ~(nIRQ_PIN);
	SPI_RW_Reg(W_REG | 0x0e, 0x00);				//�ر������л�����   GPIO0=0,GPIO1=0
	SPI_RW_Reg(W_REG | 0x07, 0x01); 			//Ready mode(Xtal is on)
	SPI_RW_Reg(W_REG | 0x05, 0x00);	 			//���ж�
	SPI_RW_Reg(W_REG | 0x06, 0x00);
	SPI_RW_Reg(R_REG | 0x03,0x00);	
	SPI_RW_Reg(R_REG | 0x04,0x00);
	delay_ms(2);
}

/*******************************************************************************
�� �� ��:  SetMode_RX
����˵��:  ����433ģ��������״̬
��    ��:  ��
�� �� ֵ:  ��
*******************************************************************************/
static void SetMode_RX(void)
{
	SPI_RW_Reg(W_REG | 0x07, 0x05); //RXon=1  Xton=1  ��������ģʽ
	SPI_RW_Reg(W_REG | 0x0e, 0x02); //�л�����  GPIO0=0,GPIO1=1
	SPI_RW_Reg(W_REG | 0x08, 0x02); 
	SPI_RW_Reg(W_REG | 0x08, 0x00); //Clear RX FIFO			
	SPI_RW_Reg(W_REG | 0x05, 0x13);	//Enable RX FIFO Almost Full/Valid Packet Received/CRC Error
	SPI_RW_Reg(W_REG | 0x06, 0x80); //Enable Sync Word Detected.
	SPI_RW_Reg(R_REG | 0x03, 0x00);	
	SPI_RW_Reg(R_REG | 0x04, 0x00); //Clear IRQ	flag
	EXTI->IMR |= (nIRQ_PIN);
}

/*******************************************************************************
�� �� ��:  SetMode_TX
����˵��:  ����433ģ����뷢��״̬
��    ��:  ��
�� �� ֵ:  ��
*******************************************************************************/
static void SetMode_TX(void)
{	
	SPI_RW_Reg(W_REG | 0x08, 0x03); 
	SPI_RW_Reg(W_REG | 0x08, 0x08); //Clear TX FIFO and set auto send
	SPI_RW_Reg(W_REG | 0x0e, 0x01); //�л�����  GPIO0=1,GPIO1=0
}

/*******************************************************************************
�� �� ��:  SI4432_SendData
����˵��:  ���ݷ���
��    ��:  *cptBuf    �����ݻ�����ָ��
            sendLength��Ҫ���͵����ݳ��� ���MAX_PAGE(0xf3)�ֽڳ���
            uiOvertime �����ͳ�ʱʱ���趨ֵ
�� �� ֵ:   ��
*******************************************************************************/
static void SI4432_Send_All_Data(u8 *cptBuf, u8 sendLength, u16 uiOvertime)
{
	u32 i, t = 0; 

	Si4432_Idle();
	SetMode_TX(); 						
#if SIGNAL == 1
	Si4332_Signal_Insturct(TX);
#endif 
    SPI_RW_Reg(W_REG | 0x3E, sendLength); //д�뷢�����ݳ���
	while (sendLength) //�ȴ����ݷ�����ȫ��
	{
		for (t = OVERTIME; (!(SPI_RW_Reg(R_REG | 0x03,0x00)&0x20)) && t; t--) //�ȴ�TX FIFO ��Ϊ��
		{
			delay_us(20);
		}
		/* 433FIFo��СΪ64���ֽڡ����������ݴ���64ʱ����������ݽ�����ǰ������
			��Ȼ����FIFO�����Զ����ͣ������Խ���Ǻ������ݻ��ǻḲ��ǰ������ */
		for (i = 0; (i < 50) && sendLength; i++)//
		{
	    	SPI_RW_Reg(W_REG | 0x7F, *cptBuf++);//Ҫ���͵�����д��FIFO
		   	sendLength--;
		}
	}
	SPI_RW_Reg(W_REG | 0x07, 0x09); //��������
	delay_us(400);                              
	for (t = OVERTIME; (!(SPI_RW_Reg(R_REG | 0x03,0x00)&0x04))&&t; )//�ȴ����ݷ������
	{
		if (!(--t))
		{
			Si4432_Fault( SI4432_TX_ERR);
			break;
		}
	}
	Si4432_Idle();
	SetMode_RX();      //�������״̬
}
/******************************************************************************
�� �� ��:   Sign433Data_Process
����˵��:   433�źŲ���ʱ�����ݴ���
��    ��:   *data    �����ݻ�����ָ��
             len    �����ݳ���   
�� �� ֵ:  	TRUE�����ݽ���OK)/FALSE
*******************************************************************************/
static u8 Sign433Data_Process(u8 *data , u8 len)
{
	Communication_t *pMsg ;
	UnitPara_t *pData ;
		
	pMsg = (Communication_t*)data;
	pData =(UnitPara_t*)&pMsg->para.unit; //����ͨѶ�ṹ�����
					
	if(len <= 50)//���ڲ��ź�ʱ������50bytes���ϵ�����
	{
		if (0 != DecryptData((u8*)pMsg))		//��������ʧ��
		{
			return FALSE;
		}
					
		if(pMsg->para.cmd == 0xB2 && pData->data[0] == 0xBB \
			&& (Get_Sign433_MasterAddr()  == pMsg->aim.sourceID))
		{                                  
			if(pData->data[1] < MAX_FRAME_NUM)
			 TestRecRSSI[pData->data[1]] = SI4432_RSSI;
		}
		if(Get_Sign433_MasterAddr() != pMsg->aim.sourceID)
		{
			u8 temp = Get_Sign433_TestPackNum();
			TestRecRSSI[temp]++;  //NUM���������ݣ���NUM����Ÿ��Ű�����
		}					
		EncryptData((u8*)pMsg,sizeof(Communication_t)+pMsg->para.len+CRC_SIZE);
	}
	return TRUE ;
}
/*******************************************************************************
�� �� ��:   RecDataPacket
����˵��:   �������ݷ�fifo
��    ��:   *cptBuf    �����ݻ�����ָ��
            cLength    ��Ҫ���͵����ݳ��� ���255�ֽڳ���   
�� �� ֵ:  	TRUE�����ݽ���OK)/FALSE
*******************************************************************************/
#if (COMMUNICATION_SHORT_FRAME == 1)
static void RecDataToFifo(FIFO_t *fifo , FIFO_t *fifo_short)
#else
static void RecDataToFifo(FIFO_t *fifo)
#endif
{
	u8 resVal1, resVal2, i = 0,fifo_success = TRUE; //���ն��������־
	u8 data[64]; //433���ջ���
	static u8 seat = 0;
	
	while (!nIRQ)
	{ 
		if (i++ > 5)	//ģ�����ݶ�ȡ����
		{
			Si4432_Fault( SI4432_RX_ERR);//���ݽ��մ���	
			seat = 0;
			break;
		}
		resVal1 = SPI_RW_Reg(R_REG | 0x03,0x00);
		resVal2 = SPI_RW_Reg(R_REG | 0x04,0x00);  //���жϼĴ���2
	
		if (resVal2&0x80)//�˶��ж�״̬�Ĵ���2
		{
			SI4432_RSSI = SPI_RW_Reg(R_REG | 0x26, 0x00);
		}
		/*�˶��ж�״̬�Ĵ���1��ֵ*/
		if ((resVal1&0x01))  	//CRC Error
		{
			seat = 0;
//			Fault_Upload(FAULT_2, SI4432_CRC_ERR, NULL); 2014.11.13 Unarty ���Σ����쳣��¼��ʧȥ����	
			break;
		}	
		if ((resVal1&0x10)) //RX FIFO almost full interrupt occured
		{
			u8 cLen ;

			if (0 == seat)
			{
				fifo_success = head_put(fifo);
			}
			
			for (cLen = 0; cLen < 50; cLen++)//read 32bytes from the FIFO				
			{
				fifo_success = fifo_putc(fifo, SPI_RW_Reg(R_REG|0x7F, 0x00));
			}
			if (seat > (MAX_PAGE - 50))
			{
				seat = 0;
				break;
			}
			else
			{
				seat += 50;
			}
		}   //end  if((resVal&0x10)==0x10)
		if ((resVal1&0x02)) //packet received interrupt occured
		{							
			resVal1=SPI_RW_Reg(R_REG|0x4B,0x00);
			for ( ; seat < resVal1; seat++)		//����433����		
			{
				data[seat%50] = SPI_RW_Reg(R_REG|0x7F, 0x00);
			}
			if(resVal1 >= SHORT_SIZE) //����֡
			{	
				if(TestRecFlag)//�ж�ʱ����RSSI�ź�ֵ���棬2015.6.11�µķ���
				{
					if(FALSE == Sign433Data_Process(data,resVal1))
					{
						seat = 0;
						SetMode_RX();
						return ;
					}						
				}				
				if(resVal1 <=50)  
				{				
					fifo_success = head_put(fifo);
				}
				fifo_success = fifo_puts(fifo, data, (resVal1 % 50 != 0)?(resVal1%50):50);	
				fifo_success = tail_put(fifo);
			}
#if (COMMUNICATION_SHORT_FRAME == 1) //���ö�֡			
			else  //��ֱ֡�ӷ���short_fifo    
			{
				fifo_success = short_put(fifo_short, data, resVal1);				
			}
#endif
			seat = 0;
			if(fifo_success == FALSE) //�������ʧ�� yanhuan adding 2015/10/10
				Fault_Upload(FAULT_2 , RECV_433_FIFO_OVER , NULL);//��ʱ��������
		}//end if((resVal&0x02)==0x02)
	} //end if(nIRQ == 0)
	if (!seat)	//���ݽ������
	{
		SetMode_RX();
	}
}

/*******************************************************************************
�� �� ��:  void EXTI15_10_IRQHandler(void)
����˵��:  433ģ���ж��¼�����
��    ��:  ��
�� �� ֵ:  ��
*******************************************************************************/
void EXTI15_10_IRQHandler(void)
{
#if (COMMUNICATION_SHORT_FRAME == 1)	
	RecDataToFifo(&rcv433fifo ,&rcv433_shortfifo);
#else 
	RecDataToFifo(&rcv433fifo);
#endif
#if SIGNAL == 1
	Si4332_Signal_Insturct(RX);
#endif
	EXTI->PR = 1<<10;  //���LINE0�ϵ��жϱ�־λ  
}   
/*******************************************************************************
�� �� ��:   SI4432_SendEn
����˵��:   ���߷���ʹ��
��    ��:   ��
�� �� ֵ:  	��
ע    �⣺	�޸�ȫ�ֱ���SI4432SendEn = TRUE: 2014.10.08 Unarty Add
*******************************************************************************/
void SI4432_SendEn(void)
{
	SI4432SendEn = TRUE;
}
/*******************************************************************************
�� �� ��:   SI4432_Send_Buf_AT_SpareTime
����˵��:   ���433M�ŵ�����״̬���趨ʱ���ڳ��ֿ��н����ݷ��ͳ�ȥ,ÿ���5ms���һ��
��    ��:   u8 *cptBuf - ��Ҫ�������ݰ�ָ��
            u8 cLength - ��Ҫ�������ݰ�����
		    u16 uiOvertime - ��Ҫ�������������ʱ  ��λ����
		    RxTxStatus_btst *btstFlag - ���ͱ�־
�� �� ֵ:  	0 - ���趨ʱ���ڣ�δ���ͳɹ�
			1 - ���趨ʱ���ڣ����ͳɹ�
*******************************************************************************/
u8 Si4432_Send(u8 *cptBuf, u8 cLength, u8 level)             
{
	if (SI4432SendEn == TRUE
		&& Check_433M_Busy(level))	//2014.10.08 Unarty Add
	{
		SI4432SendEn = FALSE; 
		Thread_Login(ONCEDELAY, 0, SPACETIME, SI4432_SendEn);	

		if(cLength >= SHORT_SIZE)
		{
			cptBuf += HEAD_SIZE;
			cLength -= HEAD_SIZE + TAIL_SIZE;
		}
		SI4432_Send_All_Data(cptBuf, cLength, level);//yanhuan adding 2015/12/23		
		return TRUE;
	}
	return FALSE;
}
/*******************************************************************************
�� �� ��:  u8 Check_433M_Busy(void)
����˵��:  ���433M�ŵ�����״̬
��    ��:  	level:	����ȴ�����
�� �� ֵ:  	1 - BUSY	æµ
			0 - SPARE	����
*******************************************************************************/
static u8 Check_433M_Busy(u8 level)
{
	const static u16 randWait_Tab[] = {5, 5, 5, 5};
	static u16 busyTime = 0;		//�������æ��ʱ��
	static u16 idleTime = 0;		//����������ʱ��		
	static u16 rand = 0;			//���ݷ�������ȴ�ʱ��
	
	if (!rand)
	{
		if (level >= sizeof(randWait_Tab))
		{
			level = 1;
		}
		rand = (Time_Get()%randWait_Tab[level] + 2);
	}
	
	if(!nGPIO2)	//����źŲ�æ	
	{
		if(!(--rand))		//��������ʱ��
		{
			if (++idleTime > IDLETIME)
			{
				idleTime = 0;
#ifndef SI4432_THRESHOLD
				Thread_Login(RESULT, 0, 400, &Si4432_Threshold);	//���¼����ź��ż�
#else
				Si4432_Fault( SI4432_TX_ERR);
#endif
			}
			rand = 0;
			busyTime = 0;
			return 1;	
		}
	}
	else
	{
		rand = 0;
		idleTime = 0;
		if (++busyTime > BUSYTIME)
		{
			busyTime = 0;
#ifndef SI4432_THRESHOLD
			Thread_Login(RESULT, 0, 400, &Si4432_Threshold);	//���¼����ź��ż�
#else
			Si4432_Fault( SI4432_TX_ERR);
#endif
		}
	}
		
	return 0;
}

#ifndef SI4432_THRESHOLD 

/*******************************************************************************
�� �� ��:  	Si4432_Threshold
����˵��: 	Si4332_ģ�鶯̬�ż�����
��    ��:  	void
�� �� ֵ:  	0���ż�������ɣ�/1���ż������У�
*******************************************************************************/
static u8 Si4432_Threshold(void)
{
	static u8 buf[4] = {0};
	static u32 time = 0;	//����ʱ��ֵ
	static u32 calculTime = 0;	 //�ϴμ���ʱ��
	u8 i;
	
	i = SPI_RW_Reg(R_REG | 0x26, 0x00);
	if (ABS(i , buf[0]) < 8)	//�����ǰ����ֵ������
	{
		i >>= 1;
		buf[0] = (buf[0] >> 1) + i;			//ͳ��ƽ��ֵ
		if (++buf[3] > 20)		//����ͳ�����
		{
			if (buf[0] < 10|| buf[0] > 220)	//ȡֵƽ���쳣
			{
				Si4432_Fault( SI4432_TX_ERR);
			}
			else
			{
				Threshold = buf[0] + 20;
  				SPI_RW_Reg(W_REG | 0x27, Threshold);    //���ͨ���Ƿ�æ���ź�ǿ���ż�	
				if (0 != calculTime)	//���ǵ�һ�μ���
				{
					if (ABS(calculTime, Time_Get()) < 60000*7)	//���μ���ʱ����û�г���5����
					{
						Si4432_Fault( SI4432_TX_ERR);	//��������ģ��
						calculTime = 0;	
					}
					else
					{
						calculTime = Time_Get();	
					}
				}
				else
				{
					calculTime = Time_Get();	
				}	
			}
			time = 0;
			     
			return 0;
		}
	}
	else if (ABS(i, buf[1]) < 50)	//������쳣״ֵ̬���
	{
		if (++buf[2] > 4)
		{
			buf[0] = i;					//���²���ֵ���
			buf[3] = 0;					//ͳ�ƴ�������
			buf[2] = 0;
		}
	}
	else
	{
		buf[1] = i;
		buf[2] = 0;
	}
	
	if (++time > 600)	//ÿ400ms����һ�Σ� �ܼ���ʱ�䲻�ܳ���2����
	{
		time = 0;
		Si4432_Fault( SI4432_TX_ERR);
		
		return 0;
	}
	
	return 1;
}

#endif		

/*******************************************************************************
�� �� ��:  	Si4432_Fault
����˵��: 	Si4332�쳣
��    ��:  	event:	�쳣ԭ��
�� �� ֵ:  	��
*******************************************************************************/
static void Si4432_Fault(u8 event)
{
	Fault_Upload(FAULT_2, (Fault_t)event, &Si4432_Init);
}
	
/**************************Copyright BestFu 2014-05-14*************************/

#include "stm32l1xx.h"
#include "spi.h"
#include "gpio.h"
#include "si4432.h"
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ������SD Card/W25Q64/NRF24L01						  
//SPI�ڳ�ʼ��
//�������Ƕ�SPI1�ĳ�ʼ��
void SPI2_Port_Init(void)
{
	GPIOx_Cfg(GPIO_B, pin_13 | pin_14 | pin_15, AF_PU_2M);      /*��������*/
    GPIOx_AF(GPIO_B, pin_13 | pin_14 | pin_15, AF_SPI2);
}
void SPI2_Init(void)
{

/* Configure SPIx: direction, NSS management, first transmitted bit, BaudRate prescaler
     master/salve mode, CPOL and CPHA */
  /* Set BIDImode, BIDIOE and RxONLY bits according to SPI_Direction value */
  /* Set SSM, SSI and MSTR bits according to SPI_Mode and SPI_NSS values */
  /* Set LSBFirst bit according to SPI_FirstBit value */
  /* Set BR bits according to SPI_BaudRatePrescaler value */
  /* Set CPOL bit according to SPI_CPOL value */
  /* Set CPHA bit according to SPI_CPHA value */	
//	*	SPI ����Ϊ˫��˫��ȫ˫�� 
//	*  	SPI ���ͽ��� 8 λ֡�ṹ 
//	*  	ʱ�����յ� 
//	*  	���ݲ����ڵڶ���ʱ����*/
//	*	�ڲ� NSS �ź��� SSI λ����
//	*	������Ԥ��ƵֵΪ 4
//	*	���ݴ���� LSB λ��ʼ
//	*	���� CRC ֵ����Ķ���ʽ 
	
	RCC->APB1RSTR |= 1<<14; //��λSPI2
	RCC->APB1RSTR &= ~(1<<14); //
	
	RCC->APB1ENR |= 1<<14;  //ʹ��SPI2ʱ��
	
	
	RCC->AHBENR    |= 1<<1;//ʹ��GPIOB
	
	SPI2_Port_Init();       //����PB13��PB14��PB15��ʱ��
	
	SPI2->CR1    |= 0<<10;		//ȫ˫��ģʽ	
	SPI2->CR1    |= 1<<9; 		//���nss����
	SPI2->CR1    |= 1<<8;  

	SPI2->CR1    |= 1<<2; 		//SPI����
	SPI2->CR1    |= 0<<11;		//8bit���ݸ�ʽ	
//	SPI2->CR1|=1<<1; 		    //����ģʽ��SCKΪ1 CPOL=1
//	SPI2->CR1|=0<<0; 		    //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1  
	//��SPI2����APB1������.ʱ��Ƶ�����Ϊ36M.
	SPI2->CR1    |= 1<<3; 		//Fsck=Fpclk1/256
	SPI2->CR1    |= 0<<7; 		//MSBfirst   
	SPI2->CR1    |= 1<<6; 		//SPI�豸ʹ��
	
	SPI2_ReadWriteByte(0xff);   //��������		
	
}   
//APB1ʱ��һ��Ϊ36Mhz
void SPI2_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//���Ʒ�Χ
	SPI2->CR1 &= ~(7<<3); //����
	SPI2->CR1 |= SpeedSet<<3;	//����SPI1�ٶ�  
	SPI2->CR1 |= 1<<6; 		//SPI�豸ʹ��	  
} 
//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u16 retry=0;				 
	while (!(SPI2->SR&(1<<1)))		//�ȴ���������	
	{
		if (retry++>=0XFFFE)
		{
			return 0; 	//��ʱ�˳�
		}
	}
	SPI2->DR = TxData;	 	  		//����һ��byte 
	retry = 0;
	while (!(SPI2->SR&(1<<0))) 		//�ȴ�������һ��byte  
	{
		if (retry++>=0XFFFE)
		{
			return 0;	//��ʱ�˳�
		}
	}

	return SPI2->DR;   //���ؽ��ջ��������� 
}












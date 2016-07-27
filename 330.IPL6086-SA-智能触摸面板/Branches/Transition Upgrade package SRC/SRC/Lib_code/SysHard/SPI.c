#include "stm32l1xx.h"
#include "spi.h"
#include "gpio.h"
#include "si4432.h"
//以下是SPI模块的初始化代码，配置成主机模式，访问SD Card/W25Q64/NRF24L01						  
//SPI口初始化
//这里针是对SPI1的初始化
void SPI2_Port_Init(void)
{
	
	GPIOx_Cfg(GPIO_B, pin_13 | pin_14 | pin_15, AF_PU_2M);      /*复用上拉*/
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
//	*	SPI 设置为双线双向全双工 
//	*  	SPI 发送接收 8 位帧结构 
//	*  	时钟悬空低 
//	*  	数据捕获于第二个时钟沿*/
//	*	内部 NSS 信号由 SSI 位控制
//	*	波特率预分频值为 4
//	*	数据传输从 LSB 位开始
//	*	用于 CRC 值计算的多项式 
	
	RCC->APB1RSTR |= 1<<14; //复位SPI2
	RCC->APB1RSTR &= ~(1<<14); //
	
	RCC->APB1ENR |= 1<<14;  //使能SPI2时钟
	
	
	RCC->AHBENR    |= 1<<1;//使能GPIOB
	
	SPI2_Port_Init();       //配置PB13、PB14、PB15开时钟
	
	SPI2->CR1    |= 0<<10;		//全双工模式	
	SPI2->CR1    |= 1<<9; 		//软件nss管理
	SPI2->CR1    |= 1<<8;  

	SPI2->CR1    |= 1<<2; 		//SPI主机
	SPI2->CR1    |= 0<<11;		//8bit数据格式	
//	SPI2->CR1|=1<<1; 		    //空闲模式下SCK为1 CPOL=1
//	SPI2->CR1|=0<<0; 		    //数据采样从第二个时间边沿开始,CPHA=1  
	//对SPI2属于APB1的外设.时钟频率最大为36M.
	SPI2->CR1    |= 1<<3; 		//Fsck=Fpclk1/256
	SPI2->CR1    |= 0<<7; 		//MSBfirst   
	SPI2->CR1    |= 1<<6; 		//SPI设备使能
	
	SPI2_ReadWriteByte(0xff);   //启动传输		
	
}   
//APB1时钟一般为36Mhz
void SPI2_SetSpeed(u8 SpeedSet)
{
	SpeedSet&=0X07;			//限制范围
	SPI2->CR1 &= ~(7<<3); //清零
	SPI2->CR1 |= SpeedSet<<3;	//设置SPI1速度  
	SPI2->CR1 |= 1<<6; 		//SPI设备使能	  
} 
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u16 retry=0;				 
	while (!(SPI2->SR&(1<<1)))		//等待发送区空	
	{
		if (retry++>=0XFFFE)
		{
			return 0; 	//超时退出
		}
	}
	SPI2->DR = TxData;	 	  		//发送一个byte 
	retry = 0;
	while (!(SPI2->SR&(1<<0))) 		//等待接收完一个byte  
	{
		if (retry++>=0XFFFE)
		{
			return 0;	//超时退出
		}
	}

	return SPI2->DR;   //返回接收缓冲区内容 
}












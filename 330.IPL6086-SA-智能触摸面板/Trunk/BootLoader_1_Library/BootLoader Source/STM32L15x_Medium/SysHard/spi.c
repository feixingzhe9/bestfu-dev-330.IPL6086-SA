/***************************Copyright BestFu 2014-05-14*************************
文	件：    SPI.c
说	明：    SPI外设寄存器配置
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-08-24
修　改：	暂无
*******************************************************************************/
#include "stm32l1xx.h"
#include "lib_type.h"
#include "lib_config.h"
#include "spi.h"

/*******************************************************************************
** 函数名称:    SPI2_Init
** 功能描述:    SPI口初始化代码
** 输　入:      无
** 返  回:      无
** 注  意:      配置成主机模式 ,对SPI2属于APB1的外设.时钟频率最大为36M.
*******************************************************************************/
void SPI2_Init(void)
{
    RCC->APB1RSTR |= 1<<14;
    RCC->APB1RSTR &= ~(1<<14);

    RCC->AHBENR  |= 1<<1;  	    //PORTB时钟使能
    RCC->APB1ENR |= 1<<14;   	//SPI2时钟使能

    GPIOB->MODER &= 0x03FFFFFF; //PB13,PB14,PB15管脚复用
    GPIOB->MODER |= 0xa8000000;

    GPIOB->OTYPER&= 0xFFFF1FFF; //设置管脚推挽输出

    GPIOB->OSPEEDR&= 0x03FFFFFF; //设置管脚速度40M
    GPIOB->OSPEEDR|= 0xFC000000;

    GPIOB->PUPDR  &= 0x03FFFFFF; //管脚上拉
    GPIOB->PUPDR  |= 0x54000000;

    GPIOB->AFR[1] &= 0x000FFFFF; //设置复用功能
    GPIOB->AFR[1] |= 0x55500000;

    SPI2->CR1    |= 0<<10;		//全双工模式
    SPI2->CR1    |= 1<<9; 		//软件nss管理
    SPI2->CR1    |= 1<<8;

    SPI2->CR1    |= 1<<2; 		//SPI主机
    SPI2->CR1    |= 0<<11;		//8bit数据格式
//	SPI2->CR1	 |= 1<<1; 		//空闲模式下SCK为1 CPOL=1
//	SPI2->CR1	 |= 0<<0; 		//数据采样从第二个时间边沿开始,CPHA=1
    //对SPI2属于APB1的外设.时钟频率最大为32M.
    SPI2->CR1    |= 1<<3; 		//Fsck=Fpclk1/4
    SPI2->CR1    |= 0<<7; 		//MSBfirst
    SPI2->CR1    |= 1<<6; 		//SPI设备使能

    SPI2_ReadWriteByte(0xff);   //启动传输
}

/*******************************************************************************
** 函数名称:    SPI2_SetSpeed
** 功能描述:    SPI2速度设置函数
** 输　入:      SpeedSet: 0~7
** 返  回:      无
** 注  意:      SPI速度=fAPB1/(1<<(SpeedSet+1))  /APB1时钟一般为36Mhz
*******************************************************************************/
void SPI2_SetSpeed(BF_INT08U SpeedSet)
{
    SpeedSet  &= 0X07;			//限制范围
    SPI2->CR1 &= ~(7<<3);
    SPI2->CR1 |= SpeedSet<<3;	//设置SPI2速度
    SPI2->CR1 |= 1<<6; 		    //SPI设备使能
}

/*******************************************************************************
** 函数名称:    SPI2_ReadWriteByte
** 功能描述:    SPI2 读写一个字节
** 输　入:      无
** 返  回:      SPI接收缓冲区内容
*******************************************************************************/
BF_INT08U SPI2_ReadWriteByte(BF_INT08U TxData)
{
    BF_INT16U retry=0;
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


/**************************Copyright BestFu 2014-05-14*************************/


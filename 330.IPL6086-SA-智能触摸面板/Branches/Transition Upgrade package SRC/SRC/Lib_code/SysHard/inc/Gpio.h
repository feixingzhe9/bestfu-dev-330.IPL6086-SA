/*******************************************************************
文	件：GPIO.h
说	明：GPIO端口配置文件
编	译：Keil uVision4 V4.54.0.0
版	本：v1.0
编	写：Unarty
日	期：2013-08-09
********************************************************************/
#ifndef __GPIO_H
#define __GPIO_H

#include "BF_type.h"
#include "stm32l1xx.h"


typedef enum{
	GPIO_A = 0,
	GPIO_B = 1,
	GPIO_C = 2,
	GPIO_D = 3,
	GPIO_E = 4,
	GPIO_F = 5,
	GPIO_G = 6
}GPIO_Port;  //GPIO端口定义

typedef enum{
	pin_0  = (1<<0),
	pin_1  = (1<<1),
	pin_2  = (1<<2),
	pin_3  = (1<<3),
	pin_4  = (1<<4),
	pin_5  = (1<<5),
	pin_6  = (1<<6),
	pin_7  = (1<<7),
	pin_8  = (1<<8),
	pin_9  = (1<<9),
	pin_10 = (1<<10),
	pin_11 = (1<<11),
	pin_12 = (1<<12),
	pin_13 = (1<<13),
	pin_14 = (1<<14),
	pin_15 = (1<<15)
}GPIO_Pin;  //GPIO引脚定义

typedef enum{
	IN_ANALOG   = 0xc0,	 //模拟输入模式
	IN_FLOATING = 0x00,	 //浮空输入	
	IN_UP       = 0x01,	 //上拉输入模式
	IN_DOWN     = 0x02,	 //下拉输入模式

	OUT_PP_400K = 0x40,		//推挽输出
	OUT_PP_2M   = 0x44,	   
	OUT_PP_10M  = 0x48,
	OUT_PP_40M  = 0x4C,
	OUT_PU_400K = 0x41,		//推挽输出上拉
	OUT_PU_2M   = 0x45,	  
	OUT_PU_10M  = 0x49,
	OUT_PU_40M  = 0x4D,
	OUT_PD_400K = 0x42,		//推挽输出下拉
	OUT_PD_2M   = 0x46,	  
	OUT_PD_10M  = 0x4A,
	OUT_PD_40M  = 0x4E,

	OUT_OP_400K = 0x50,
	OUT_OP_2M   = 0x54,	   //开漏输出
	OUT_OP_10M  = 0x58,
	OUT_OP_40M  = 0x5C,
	OUT_OU_400K = 0x51,
	OUT_OU_2M   = 0x55,	  //开漏输出上拉
	OUT_OU_10M  = 0x59,
	OUT_OU_40M  = 0x5D,
	OUT_OD_400K = 0x52,		//开漏输出下拉
	OUT_OD_2M   = 0x56,	  
	OUT_OD_10M  = 0x5A,
	OUT_OD_40M  = 0x5E,

	AF_PP_400K  = 0x80,
	AF_PP_2M    = 0x84,	   //复用推挽输出
	AF_PP_10M   = 0x88,
	AF_PP_40M   = 0x8C,
	AF_PU_400K  = 0x81,
	AF_PU_2M    = 0x85,	  //复用推挽输出上拉
	AF_PU_10M   = 0x89,
	AF_PU_40M   = 0x8D,
	AF_PD_400K  = 0x82,
	AF_PD_2M    = 0x86,	  //复用推挽输出下拉
	AF_PD_10M   = 0x8A,
	AF_PD_40M   = 0x8E,

	AF_OP_400K  = 0x90,
	AF_OP_2M    = 0x94,	   //复用开漏输出
	AF_OP_10M   = 0x98,
	AF_OP_40M   = 0x9C,
	AF_OU_400K  = 0x91,
	AF_OU_2M    = 0x95,	  //复用开漏输出上拉
	AF_OU_10M   = 0x99,
	AF_OU_40M   = 0x9D,
	AF_OD_400K  = 0x92,
	AF_OD_2M    = 0x96,	  //复用开漏输出下拉
	AF_OD_10M   = 0x9A,
	AF_OD_40M   = 0x9E,	
}GPIO_Mode;  //GPIO模式定义

typedef enum
{
    AF_RTC_50Hz   = 0x00,    //< RTC 50/60 Hz Alternate Function mapping */
    AF_MCO        = 0x00,    //< MCO Alternate Function mapping */
    AF_RTC_AF1    = 0x00,    //< RTC_AF1 Alternate Function mapping */
    AF_WKUP       = 0x00,    //< Wakeup (WKUP1, WKUP2 and WKUP3) Alternate Function mapping */
    AF_SWJ        = 0x00,    //< SWJ (SW and JTAG) Alternate Function mapping */
    AF_TRACE      = 0x00,    //< TRACE Alternate Function mapping */
    AF_TIM2       = 0x01,    //< TIM2 Alternate Function mapping */
    AF_TIM3       = 0x02,    //< TIM3 Alternate Function mapping */
    AF_TIM4       = 0x02,    //< TIM4 Alternate Function mapping */
    AF_TIM5       = 0x02,    //< TIM5 Alternate Function mapping */
    AF_TIM9       = 0x03,    //< TIM9 Alternate Function mapping */
    AF_TIM10      = 0x03,    //< TIM10 Alternate Function mapping */
    AF_TIM11      = 0x03,    //< TIM11 Alternate Function mapping */
    AF_I2C1       = 0x04,    //< I2C1 Alternate Function mapping */
    AF_I2C2       = 0x04,    //< I2C2 Alternate Function mapping */
    AF_SPI1       = 0x05,    //< SPI1 Alternate Function mapping */
    AF_SPI2       = 0x05,    //< SPI2 Alternate Function mapping */
    AF_SPI3       = 0x06,    //< SPI3 Alternate Function mapping */
    AF_USART1     = 0x07,    //< USART1 Alternate Function mapping */
    AF_USART2     = 0x07,    //< USART2 Alternate Function mapping */
    AF_USART3     = 0x07,    //< USART3 Alternate Function mapping */
    AF_UART4      = 0x08,    //< UART4 Alternate Function mapping */
    AF_UART5      = 0x08,    //< UART5 Alternate Function mapping */
    AF_USB        = 0x0A,    //< USB Full speed device  Alternate Function mapping */
    AF_LCD        = 0x0B,    //< LCD Alternate Function mapping */
    AF_FSMC       = 0x0C,    //< FSMC Alternate Function mapping */
    AF_SDIO       = 0x0C,    //< SDIO Alternate Function mapping */
    AF_RI         = 0x0E,    //< RI Alternate Function mapping */
    AF_EVENTOUT   = 0x0F,    //< EVENTOUT Alternate Function mapping */
}GPIO_AF;

//#ifndef GPIO_TypeDef 
//
//typedef struct
//{
//  vu32 MODER;        /*!< GPIO port mode register,                     Address offset: 0x00      */
//  vu16 OTYPER;       /*!< GPIO port output type register,              Address offset: 0x04      */
//  vu16 RESERVED0;         /*!< Reserved,                                    0x06                      */
//  vu32 OSPEEDR;      /*!< GPIO port output speed register,             Address offset: 0x08      */
//  vu32 PUPDR;        /*!< GPIO port pull-up/pull-down register,        Address offset: 0x0C      */
//  vu16 IDR;          /*!< GPIO port input data register,               Address offset: 0x10      */
//  vu16 RESERVED1;         /*!< Reserved,                                    0x12                      */
//  vu16 ODR;          /*!< GPIO port output data register,              Address offset: 0x14      */
//  vu16 RESERVED2;         /*!< Reserved,                                    0x16                      */
//  vu16 BSRRL;        /*!< GPIO port bit set/reset low registerBSRR,    Address offset: 0x18      */
//  vu16 BSRRH;        /*!< GPIO port bit set/reset high registerBSRR,   Address offset: 0x1A      */
//  vu32 LCKR;         /*!< GPIO port configuration lock register,       Address offset: 0x1C      */
//  vu32 AFR[2];       /*!< GPIO alternate function low register,        Address offset: 0x20-0x24 */
//  vu16 BRR;          /*!< GPIO bit reset register,                     Address offset: 0x28      */
//  vu16 RESERVED3;         /*!< Reserved,                                    0x2A                      */
//} GPIO_TypeDef;

#define    GPIO_BASE (0X40000000 + 0x20000) //GPIO端口基址值


void GPIOx_Cfg(GPIO_Port GPIOx, u16 pin, GPIO_Mode mode);    //GPIO配置
void GPIOx_AF(GPIO_Port GPIOx, u16 pin, GPIO_AF AF);			//配置GPIO映射到复用功能引脚
u16 GPIOx_Set(GPIO_Port GPIOx, u16 pin);                  //GPIO引脚输出电平置1
void GPIOx_Rst(GPIO_Port GPIOx, u16 pin);                    //GPIO引脚输出电平置0
u16 GPIOx_Get(GPIO_Port GPIOx, u16 pin);                 	//获取GPIO引脚输入电平
void GPIOx_Rvrs(GPIO_Port GPIOx, u16 pin);
//INT32U GPIOx_Lock(GPIO_Port GPIOx, u16 pin, INT8U sta);      //给相应GPIO引脚配置上锁/去锁


#endif //GPIO.h end
/**************************FILE*END********************************/

/***************************Copyright BestFu ***********************************
**  文    件：  WTC6216.c
**  功    能：  <<驱动层>> WTC6106 六通道电容触摸芯片
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.08.20
**  修改日期：  2014.08.20
**  说    明：  数据接口: INT_Flag线+四线BCD码
**  
*******************************************************************************/
#ifndef _WTC6106_H_
#define _WTC6106_H_

//#include "stm32l1xx.h"
#include "GPIO.h"
#include "BF_type.h"	

#define GPIO_WTC_TouchFlag		GPIOA
#define PORT_WTC_TouchFlag		GPIO_A
#define PIN_WTC_TouchFlag		(1<<0)

#define GPIO_WTC_DATA1			GPIOA
#define PORT_WTC_DATA1			GPIO_A
#define DATA1 		            (1<<1)	//PA1
#define DATA2 		            (1<<2)	//PA2
#define DATA3 		            (1<<3)	//PA3

#define GPIO_WTC_DATA2			GPIOC
#define PORT_WTC_DATA2			GPIO_C
#define DATA4 		            (1<<13)	//PC13
#define DATA5 		            (1<<14)	//PC14
#define DATA6 		            (1<<15)	//PC15


	
#define Get_TouchFlag()		( GPIO_WTC_TouchFlag->IDR & PIN_WTC_TouchFlag)
#define Get_TouchKey1()   	((GPIO_WTC_DATA1 ->IDR & 0x000E)<<2	)		    //高三位  摁键
#define Get_TouchKey2()   	((GPIO_WTC_DATA2 ->IDR & 0xE000)>>13)			//低三位  摁键

void WTC6106_Init(void);
u8   WTC6106_ReadKey(void);

#endif	   

/********************************* END FILE ***********************************/

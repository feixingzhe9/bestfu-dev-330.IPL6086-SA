/*******************************************************************************
**  文    件:   USART1.h
**  功    能：  串口1 驱动 （寄存器操作）
**  编    译：  Keil uVision5 V5.10
**  芯    片:   STM32L151xx
**  版    本：  V1.0.1
**  编    写：  Seven
**  创建日期：  2014.04.15
**  修改日期：  2014.08.22
**  说    明：  1.串口1打印函数      OK
                2.发送函数加入timeout
                串口接收没有做完整测试
********************************************************************************/ 
#ifndef _USART1_H_
#define _USART1_H_

#include <stdio.h>	
#include "BF_type.h"

#define EN_USART1_RX 			0		//使能串口1接收
#define USART1_SUPPORT_PRINTF	0		//使能串口1支持printf()函数
	  	
extern void USART1_Init(u32 pclk2 , u32 bound);
extern void USART1_SendByte(u8 ch);

#endif	   

/********************************* END FILE ***********************************/

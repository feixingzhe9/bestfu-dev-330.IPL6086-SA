/*******************************************************************************
**  文    件:   USART2.h
**  功    能：  串口2 驱动 （寄存器操作）
**  编    译：  Keil uVision5 V5.10
**  芯    片:   STM32L151xx
**  版    本：  V1.0.1
**  编    写：  Seven
**  创建日期：  2014.04.15
**  修改日期：  2014.08.22
**  说    明：  1.串口2 printf()函数测试OK
                2.发送函数加入timeout
                %%串口接收没有做完整测试
********************************************************************************/ 
#ifndef _USART2_H_
#define _USART2_H_

#include <stdio.h>	
#include "BF_type.h"

#define EN_USART2_RX 			0		//使能串口2接收
#define USART2_SUPPORT_PRINTF	0		//使能串口2支持printf()函数

extern void USART2_Init(u32 pclk1 , u32 bound);
extern void USART2_SendByte(u8 ch);

#endif	   

/********************************* END FILE ***********************************/

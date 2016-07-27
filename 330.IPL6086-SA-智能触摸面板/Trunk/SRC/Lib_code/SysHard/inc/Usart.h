/***************************Copyright BestFu 2014-05-14*************************
文	件：	UART.h
说	明：	串口操作相关函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.07.03 
修　改：	暂无
*******************************************************************************/
#ifndef __USART_H
#define __USART_H

#include "BF_type.h"
#include "stdio.h"

/******************************定义调试宏**************************************/
#define DEBUG_EN      (0u)			//配置debug功能<1u打开/0u关闭>

#if(DEBUG_EN > 0u)
//#define  DEBUG(format,...)	printf("File:"__FILE__", Line:%04d:"format"\n",__LINE__,##__VA_ARGS__)
#define  DEBUG(format,...)	printf(format,##__VA_ARGS__)
#else
	#define DEBUG(format,...)	
#endif

//function declaration
void UART_Init(u32 pclk2, u32 bound);
u8 UART_Write(u8 c);
u8 UART_Send(u8 *data, u8 len, u8 level);

#endif	   
/**************************Copyright BestFu 2014-05-14*************************/

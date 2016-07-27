/***************************Copyright BestFu ***********************************
**  文    件：  74HC595.h
**  功    能：  <<驱动层>> 串转并74HC595驱动
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.08.22
**  修改日期：  2014.08.25
**  说    明：  
*******************************************************************************/

#ifndef _74HC595_H_
#define _74HC595_H_

#include "GPIO.h"
//extern u16 data_temp;

#ifndef IO_STATUS
#define IO_STATUS0(GPIOx, pin)  GPIOx_Rst(GPIOx, pin) //相应管脚置0
#define IO_STATUS1(GPIOx, pin)  GPIOx_Set(GPIOx, pin) //相应管脚置1) 
#endif
               
/*===============74HC595引脚功能说明===========================
14(SDI/DS)------------------------------ 串行数据输入端
12(SHCP/SCK)---------------------------- 串行数据时钟信号（上升沿有效）   Storage Register Clock Input
11(STCP/LCK) --------------------------- 移位寄存器 锁存到 输出寄存器 时钟信号（上升沿有效） Shift Register Clock Input
13(OE)---------------------------------- 输出使能（低电平有效，高电平高阻状态）
10(MR)---------------------------------- 主复位 清除移位寄存器 （低电平有效）
1.2.3.4.5.6.7.15(Q0~Q7)----------------- 并行数据输出
9 (Q7’)--------------------------------- 串行数据输出
==============================================================*/

#define GPIO_595_SDI        GPIOA
#define PORT_595_SDI        GPIO_A
#define  PIN_595_SDI        pin_12

#define GPIO_595_SHCP       GPIOC
#define PORT_595_SHCP       GPIO_C
#define  PIN_595_SHCP       pin_7

#define GPIO_595_STCP       GPIOC
#define PORT_595_STCP       GPIO_C
#define  PIN_595_STCP       pin_8

#define GPIO_595_OE         GPIOA
#define PORT_595_OE         GPIO_A
#define  PIN_595_OE         pin_11

#define GPIO_595_MR         GPIOA
#define PORT_595_MR         GPIO_A
#define  PIN_595_MR         pin_8

#define PORT_LED_ON_OFF     GPIO_C
#define  PIN_LED_ON_OFF     pin_12


#define HC595_SDI(str)       IO_STATUS##str(PORT_595_SDI , PIN_595_SDI)             //串行数据输入
#define HC595_SHCP(str)      IO_STATUS##str(PORT_595_SHCP, PIN_595_SHCP)            //移位寄存器移位时钟信号，上升沿移位
#define HC595_STCP(str)      IO_STATUS##str(PORT_595_STCP, PIN_595_STCP)            //上升沿将移位寄存器状态值锁存到输出寄存器
#define HC595_OE(str)        IO_STATUS##str(PORT_595_OE  , PIN_595_OE)              //低电平，输出状态值；高电平，输出高阻抗    str=0,输出
#define HC595_MR(str)        IO_STATUS##str(PORT_595_MR  , PIN_595_MR)              //清除移位寄存器

extern void _74HC595_Init(void);
extern void _74HC595_MainReset(void);
extern void _74HC595_WriteData(u16 data);

#endif 

/********************************* END FILE ***********************************/

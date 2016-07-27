/******************************************************************************
**  文    件：  IIC.h
**  功    能：  <<驱动层>> 模拟IIC驱动
**  编    译：  Keil uVision5 V5.10
**  版    本：  V2.0
**  编    写：  Seven
**  创建日期：  2014.03.21
**  修改日期：  2014.08.20
**  说    明：  模拟IIC协议
**  V2.0
    >> 所有函数重新优化，删除额外操作
    >> 优化移植接口
*******************************************************************************/
#ifndef _IIC_H_
#define _IIC_H_ 

#include "stm32l1xx.h"
#include "BF_type.h"
#include "GPIO.h"

//#define DEVICE_ADDR   0xD0    // MPU6050 PIN AD0 =0   : 1101 000x
#define DEVICE_ADDR		0x78    // OLED

/************************IIC Inteface STM32L151C8T6 I/O***********************/
#define GPIO_IIC_SCL        GPIOB
#define PORT_IIC_SCL        GPIO_B
#define  PIN_IIC_SCL        (1<<10)

#define GPIO_IIC_SDA        GPIOB
#define PORT_IIC_SDA        GPIO_B
#define  PIN_IIC_SDA        (1<<11)
#define  NUM_IIC_SDA        11
/*******************************************************************************/

#define SCL1        GPIO_IIC_SCL->BSRRL |= PIN_IIC_SCL
#define SCL0        GPIO_IIC_SCL->BSRRH |= PIN_IIC_SCL

#define SDA1        GPIO_IIC_SDA->BSRRL |= PIN_IIC_SDA
#define SDA0        GPIO_IIC_SDA->BSRRH |= PIN_IIC_SDA

#define SDA_IN()    GPIOx_Cfg(PORT_IIC_SDA, PIN_IIC_SDA, IN_UP)
#define SDA_OUT()   GPIOx_Cfg(PORT_IIC_SDA, PIN_IIC_SDA, OUT_PU_40M)

//#define SDA_IN()  GPIO_IIC_SDA->MODER &= ~(3<<(NUM_IIC_SDA<<1))       //输入模式 00
//#define SDA_OUT() GPIO_IIC_SDA->MODER |=  (1<<(NUM_IIC_SDA<<1))       //输出模式 01

#define Get_SDA()   (GPIO_IIC_SDA->IDR & PIN_IIC_SDA)
//======================================================

/*接口函数*/
extern void  IIC_InitPort(void);
extern void  IIC_Start(void);
extern void  IIC_Stop(void);

extern void  IIC_ACK(void); 
extern void  IIC_NACK(void); 
extern u8    IIC_WaitAck(void);

extern void  IIC_SendByte(u8 dat);
extern u8    IIC_GetByte(void);

extern void  IIC_WriteByte(u8 Addr,u8 data);
extern u8    IIC_ReadByte( u8 Addr);

extern void  IIC_WriteDataN(u8 Addr,u8 *pData ,u8 len);
extern void  IIC_ReadDataN( u8 Addr,u8 *pData ,u8 len);

#endif
/********************************* END FILE ***********************************/

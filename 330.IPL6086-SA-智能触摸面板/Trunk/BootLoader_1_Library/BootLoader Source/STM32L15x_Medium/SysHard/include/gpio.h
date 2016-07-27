/***************************Copyright bestfu 2015-12-30**************************
**  文    件: gpio.h
**  功    能：stm32f10x GPIO驱动头文件
**  编    译：Keil uVision5 V5.12
**  版    本：V1.0.0
**  编    写：Jay
**  创建日期：2015.12.29
**  修改日期：无
**  说    明：初版
*******************************************************************************/
#ifndef __GPIO_H
#define __GPIO_H

/********************************GPIO PIN 定义*********************************/
#define GPIO_Pin_0                 ((BF_INT16U)0x0001)  /* Pin 0 selected */
#define GPIO_Pin_1                 ((BF_INT16U)0x0002)  /* Pin 1 selected */
#define GPIO_Pin_2                 ((BF_INT16U)0x0004)  /* Pin 2 selected */
#define GPIO_Pin_3                 ((BF_INT16U)0x0008)  /* Pin 3 selected */
#define GPIO_Pin_4                 ((BF_INT16U)0x0010)  /* Pin 4 selected */
#define GPIO_Pin_5                 ((BF_INT16U)0x0020)  /* Pin 5 selected */
#define GPIO_Pin_6                 ((BF_INT16U)0x0040)  /* Pin 6 selected */
#define GPIO_Pin_7                 ((BF_INT16U)0x0080)  /* Pin 7 selected */
#define GPIO_Pin_8                 ((BF_INT16U)0x0100)  /* Pin 8 selected */
#define GPIO_Pin_9                 ((BF_INT16U)0x0200)  /* Pin 9 selected */
#define GPIO_Pin_10                ((BF_INT16U)0x0400)  /* Pin 10 selected */
#define GPIO_Pin_11                ((BF_INT16U)0x0800)  /* Pin 11 selected */
#define GPIO_Pin_12                ((BF_INT16U)0x1000)  /* Pin 12 selected */
#define GPIO_Pin_13                ((BF_INT16U)0x2000)  /* Pin 13 selected */
#define GPIO_Pin_14                ((BF_INT16U)0x4000)  /* Pin 14 selected */
#define GPIO_Pin_15                ((BF_INT16U)0x8000)  /* Pin 15 selected */
#define GPIO_Pin_All               ((BF_INT16U)0xFFFF)  /* All pins selected */

/*****************************GPIO 复用选择功能定义****************************/ 
/* AF 0 selection */ 
#define GPIO_AF0_MCO           ((BF_INT08U)0x00)  /*!< MCO Alternate Function mapping                 */
#define GPIO_AF0_TAMPER        ((BF_INT08U)0x00)  /*!< TAMPER  Alternate Function mapping             */
#define GPIO_AF0_SWJ           ((BF_INT08U)0x00)  /*!< SWJ (SWD and JTAG) Alternate Function mapping  */
#define GPIO_AF0_TRACE         ((BF_INT08U)0x00)  /*!< TRACE Alternate Function mapping               */
#define GPIO_AF0_RTC_50Hz      ((BF_INT08U)0x00)  /*!< RTC_OUT Alternate Function mapping             */
   
/* AF 1 selection */ 
#define GPIO_AF1_TIM2          ((BF_INT08U)0x01)  /*!< TIM2 Alternate Function mapping */

/* AF 2 selection */ 
#define GPIO_AF2_TIM3          ((BF_INT08U)0x02)  /*!< TIM3 Alternate Function mapping */
#define GPIO_AF2_TIM4          ((BF_INT08U)0x02)  /*!< TIM4 Alternate Function mapping */
#define GPIO_AF2_TIM5          ((BF_INT08U)0x02)  /*!< TIM5 Alternate Function mapping */

/* AF 3 selection */ 
#define GPIO_AF3_TIM9          ((BF_INT08U)0x03)  /*!< TIM9 Alternate Function mapping */
#define GPIO_AF3_TIM10         ((BF_INT08U)0x03)  /*!< TIM10 Alternate Function mapping */
#define GPIO_AF3_TIM11         ((BF_INT08U)0x03)  /*!< TIM11 Alternate Function mapping */


/* AF 4 selection */ 
#define GPIO_AF4_I2C1          ((BF_INT08U)0x04)  /*!< I2C1 Alternate Function mapping */
#define GPIO_AF4_I2C2          ((BF_INT08U)0x04)  /*!< I2C2 Alternate Function mapping */

/* AF 5 selection */ 
#define GPIO_AF5_SPI1          ((BF_INT08U)0x05)  /*!< SPI1/I2S1 Alternate Function mapping      */
#define GPIO_AF5_SPI2          ((BF_INT08U)0x05)  /*!< SPI2/I2S2 Alternate Function mapping */

/* AF 6 selection */ 
#define GPIO_AF6_SPI3          ((BF_INT08U)0x06)  /*!< SPI3/I2S3 Alternate Function mapping */

/* AF 7 selection */ 
#define GPIO_AF7_USART1        ((BF_INT08U)0x07)  /*!< USART1 Alternate Function mapping  */
#define GPIO_AF7_USART2        ((BF_INT08U)0x07)  /*!< USART2 Alternate Function mapping  */
#define GPIO_AF7_USART3        ((BF_INT08U)0x07)  /*!< USART3 Alternate Function mapping  */

/* AF 8 selection */ 
#define GPIO_AF8_UART4         ((BF_INT08U)0x08)  /*!< UART4 Alternate Function mapping  */
#define GPIO_AF8_UART5         ((BF_INT08U)0x08)  /*!< UART5 Alternate Function mapping  */

/* AF 9 selection */ 

/* AF 10 selection */ 

/* AF 11 selection */ 
#define GPIO_AF11_LCD            ((BF_INT08U)0x0B)  /*!< LCD Alternate Function mapping */

/* AF 12 selection */   
#define GPIO_AF12_FSMC            ((BF_INT08U)0x0C)  /*!< FSMC Alternate Function mapping */
#define GPIO_AF12_SDIO            ((BF_INT08U)0x0C)  /*!< SDIO Alternate Function mapping */

/* AF 13 selection */ 

/* AF 14 selection */ 
#define GPIO_AF14_TIM_IC1         ((BF_INT08U)0x0E)  /*!< TIMER INPUT CAPTURE Alternate Function mapping */
#define GPIO_AF14_TIM_IC2         ((BF_INT08U)0x0E)  /*!< TIMER INPUT CAPTURE Alternate Function mapping */
#define GPIO_AF14_TIM_IC3         ((BF_INT08U)0x0E)  /*!< TIMER INPUT CAPTURE Alternate Function mapping */
#define GPIO_AF14_TIM_IC4         ((BF_INT08U)0x0E)  /*!< TIMER INPUT CAPTURE Alternate Function mapping */

/* AF 15 selection */ 
#define GPIO_AF15_EVENTOUT      ((BF_INT08U)0x0F)  /*!< EVENTOUT Alternate Function mapping */

/********************************数据结构定义*********************************/
typedef enum{
	GPIO_A = 0,
	GPIO_B = 1,
	GPIO_C = 2,
	GPIO_D = 3,
	GPIO_E = 4,
	GPIO_F = 5,
	GPIO_G = 6
}GPIO_Port;  //GPIO端口定义

typedef enum
{
	AN_IN			= 0x03,		/* 模拟输入 */
	IN_FLOATING 	= 0x00,		/* 浮空输入 */	
	IN_UP   		= 0x20,		/* 上拉输入 */
	IN_DOWN 		= 0x40,		/* 下拉输入 */
	
	OUT_PP_400K		= 0x01,		/* 通用推挽输出 */
	OUT_PP_2M		= 0x09,
	OUT_PP_10M		= 0x11,
	OUT_PP_40M		= 0x19,	
	OUT_PU_400K		= 0x21,		/* 通用上拉推挽输出 */
	OUT_PU_2M		= 0x29,
	OUT_PU_10M		= 0x31,
	OUT_PU_40M		= 0x39,		
	OUT_PD_400K		= 0x41,		/* 通用下拉推挽输出 */
	OUT_PD_2M		= 0x49,
	OUT_PD_10M		= 0x51,
	OUT_PD_40M		= 0x59,	
	
	OUT_OP_400K		= 0x05,		/* 开漏输出 */
	OUT_OP_2M		= 0x0d,
	OUT_OP_10M		= 0x15,
	OUT_OP_40M		= 0x1d,	
	OUT_OU_400K		= 0x25,		/* 上拉开漏输出 */
	OUT_OU_2M		= 0x2d,
	OUT_OU_10M		= 0x35,
	OUT_OU_40M		= 0x3d,		
	OUT_OD_400K		= 0x45,		/* 下拉开漏输出 */
	OUT_OD_2M		= 0x4d,
	OUT_OD_10M		= 0x55,
	OUT_OD_40M		= 0x5d,	

	AF_PP_400K		= 0x02,		/* 复用推挽输出 */
	AF_PP_2M		= 0x0a,
	AF_PP_10M		= 0x12,
	AF_PP_40M		= 0x1a,	
	AF_PU_400K		= 0x22,		/* 上拉复用推挽输出 */
	AF_PU_2M		= 0x2a,
	AF_PU_10M		= 0x32,
	AF_PU_40M		= 0x3a,	
	AF_PD_400K		= 0x42,		/* 下拉复用推挽输出 */
	AF_PD_2M		= 0x4a,
	AF_PD_10M		= 0x52,
	AF_PD_40M		= 0x5a,	
	
	AF_OP_400K		= 0x06,		/* 复用开漏输出 */
	AF_OP_2M		= 0x0e,
	AF_OP_10M		= 0x16,
	AF_OP_40M		= 0x1e,
	AF_OU_400K		= 0x26,		/* 上拉复用开漏输出 */
	AF_OU_2M		= 0x2e,
	AF_OU_10M		= 0x36,
	AF_OU_40M		= 0x3e,
	AF_OD_400K		= 0x46,		/* 下拉复用开漏输出 */
	AF_OD_2M		= 0x4e,
	AF_OD_10M		= 0x56,
	AF_OD_40M		= 0x5e
}GPIO_Mode;  //GPIO模式定义
/*********************************函数声明**************************************/
void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode);
void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
BF_INT08U GPIO_ReadOutputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
BF_INT08U GPIO_ReadInputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin);
void GPIO_PinRemapConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin, BF_INT08U AF);

#endif


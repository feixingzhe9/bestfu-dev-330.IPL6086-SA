/***************************Copyright BestFu ***********************************
**  文    件：  ShowLED.c
**  功    能：  <<逻辑层>> 玻璃面板（74HC595驱动LED灯）制作灯光闪烁效果
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1
**  编    写：  Seven
**  创建日期：  2014.08.26
**  修改日期：  2014.08.27
**  说    明：  >>LED灯逻辑位置 与 物理位置 映射  最大支持16个灯
**              >>完成灯光圈左右渐灭渐亮效果
*******************************************************************************/

#ifndef _ShowLED_H_
#define _ShowLED_H_

#include "BF_type.h"

#define STATUS0(GPIOx, pin)  GPIOx_Rst(GPIOx, pin) //相应管脚置0
#define STATUS1(GPIOx, pin)  GPIOx_Set(GPIOx, pin) //相应管脚置1 

#define GPIO_LED_B    			GPIOB
#define PORT_LED_B	    		GPIO_B
#define PIN_L1 		            (1<<3)	//PB3
#define PIN_L2 		            (1<<4)	//PB4
#define PIN_L3 		            (1<<5)	//PB5
#define PIN_L4 		            (1<<8)	//PB8
#define PIN_L5 		            (1<<9)	//PB9

#define GPIO_LED_A    			GPIOA
#define PORT_LED_A	    		GPIO_A
#define PIN_L6 		            (1<<15)	//PA15

#define LED1_ON     STATUS1(PORT_LED_B, PIN_L1)
#define LED2_ON     STATUS1(PORT_LED_B, PIN_L2)
#define LED3_ON     STATUS1(PORT_LED_B, PIN_L3)
#define LED4_ON     STATUS1(PORT_LED_B, PIN_L4)
#define LED5_ON     STATUS1(PORT_LED_B, PIN_L5)
#define LED6_ON     STATUS1(PORT_LED_A, PIN_L6)

#define LED1_OFF     STATUS0(PORT_LED_B, PIN_L1)
#define LED2_OFF     STATUS0(PORT_LED_B, PIN_L2)
#define LED3_OFF     STATUS0(PORT_LED_B, PIN_L3)
#define LED4_OFF     STATUS0(PORT_LED_B, PIN_L4)
#define LED5_OFF     STATUS0(PORT_LED_B, PIN_L5)
#define LED6_OFF     STATUS0(PORT_LED_A, PIN_L6)

void LED_Init(void);
extern void ShowLED_OpenAll(void);
extern void ShowLED_CloseAll(void);
//extern void ShowLED_LightON( u8 keyNum , u16 lightState);
extern void ShowLED_LightON(u8 keyNum);
extern void ShowLED_LightOFF(u8 keyNum , u16 lightState);
extern void ShowLED_CheckDevice(void);
extern void ShowLED_CycleLED(void);
extern void ShowLED_StartCycleLED(u8 key);
extern void ShowLED_FlashLED(u8 key);
#endif 

/********************************* END FILE ***********************************/

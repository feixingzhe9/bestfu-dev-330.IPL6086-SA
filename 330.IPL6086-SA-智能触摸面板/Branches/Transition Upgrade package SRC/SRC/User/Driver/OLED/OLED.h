/***************************Copyright BestFu ***********************************
**  文    件：  OLED.c
**  功    能：  <<驱动层>> 1.3寸OLED液晶驱动
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.06.30
**  修改日期：  2014.08.20
**  说    明：  SH1106 驱动程序
**              OLED 即有机发光二极管
**              优点：自发光，无需背光，对比度高，厚度薄，视角广
*******************************************************************************/

/******************************************************************************
**        OLED Module Message
** LCD Module:        VGM12864C3W01 -- 1.3 inch
** Display Size:      132x64
** Driver/Controller: SH1106
** Interface:         IIC串行接口(400KHz) (还可支持 8080/SPI接口)
******************************************************************************/
#ifndef _OLED_H_
#define _OLED_H_

#include "BF_type.h"
#include "GPIO.h"

#define SUPPORT_OLED_PUT_ASCII 1    //OLED驱动是否支持 字符及汉字打印   0 不支持   1支持

//====================================================
//*******OLED12864 Inteface STM32L151C8T6 I/O**********
//OLED电源开关
#define GPIO_OLED_POW		GPIOA
#define PORT_OLED_POW		GPIO_A
#define  PIN_OLED_POW		(1<<7)

//RST 低电平复位  高电平工作
#define GPIO_OLED_RST		GPIOB
#define PORT_OLED_RST		GPIO_B
#define  PIN_OLED_RST		(1<<0)
//SCL  串行时钟线
//#define GPIO_OLED_SCL		GPIOB
//#define PORT_OLED_SCL		GPIO_B
//#define  PIN_OLED_SCL		(1<<6)
//SDA  串行数据线
//#define GPIO_OLED_SDA		GPIOB
//#define PORT_OLED_SDA		GPIO_B
//#define  PIN_OLED_SDA		(1<<7)

//=====================================================

#define OLED_POW_1      GPIO_OLED_POW->BSRRL |= PIN_OLED_POW
#define OLED_POW_0      GPIO_OLED_POW->BSRRH |= PIN_OLED_POW

#define OLED_RST_1      GPIO_OLED_RST->BSRRL |= PIN_OLED_RST
#define OLED_RST_0      GPIO_OLED_RST->BSRRH |= PIN_OLED_RST
//#define OLED_SCL_1		GPIO_OLED_SCL->BSRRL |= PIN_OLED_SCL
//#define OLED_SCL_0		GPIO_OLED_SCL->BSRRH |= PIN_OLED_SCL
//#define OLED_SDA_1		GPIO_OLED_SDA->BSRRL |= PIN_OLED_SDA
//#define OLED_SDA_0		GPIO_OLED_SDA->BSRRH |= PIN_OLED_SDA

extern void Init_OLED(void);
extern void OLED_ClearScreen(void);

#if SUPPORT_OLED_PUT_ASCII
extern void OLED_ShowChar(  unsigned char X0, unsigned char Y0,char DspChar		,unsigned char DspMode);
extern void OLED_ShowCN(	unsigned char X0, unsigned char Y0,const char *DspCN,unsigned char DspMode);
extern void OLED_ShowString(unsigned char X0, unsigned char Y0,const char *DspStr,unsigned char DspMode);
//extern void OLED_ShowNumber(unsigned char X0, unsigned char Y0,unsigned int Data,unsigned char len , unsigned char DspMode);
#endif
//extern void OLED_ShowPicture(const u8 *p_Pic);
extern void OLED_ShowPicture(const u32 *p_Pic);
extern void OLED_ShowPicAt(u8 x0, u8 y0, u8 xlen, u8 ylen,const u8 *p_Pic);
extern void OLED_ShowHalfPicAt( u8 x0,const u32 *p_Pic);   //显示1/2屏图片
extern void OLED_ShowQuartPicAt(u8 x0,u8 y0,const u32 *p_Pic);  //显示1/4屏图片

extern void OLED_DrawLine(unsigned char x0);
extern void OLED_ShowNum18x32(unsigned char x0, unsigned char y0,  const u32 *p_Pic);
extern void OLED_Show32x323PicAt(u8 x0,u8 y0,const u32 *p_Pic);

#endif

/********************************* END FILE ***********************************/

/***************************Copyright BestFu ***********************************
**  文    件：  LowPower.c
**  功    能：  <<驱动层>> STM32L151 低功耗模式驱动 
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.07.10
**  修改日期：  2014.08.20
**  说    明：  
**  V1.0
    >> 
*******************************************************************************/
#include "stm32l1xx.h"             
#include "BF_type.h"
#include "core_cm3.h"
#include "LowPower.h"
#include "GPIO.h"
#include "Sys.h"
#include "SysHard.h"
#include "key.h"
#include "74HC595.h"
#include "OLED.h"
#include "ShowLED.h"
#include "usart.h"
#include "AirConditionPad.h"

//Macro definition
#define CR_DS_MASK              ((uint32_t)0xFFFFFFFC)
#define STANDBY_PREPARE_COUNT	(200u)					//进入休眠准备阶段计数
#define STANDBY_TIME_COUNT		(300u)					//进入休眠阶段计数


//Public Variable definition
u8 gSleepFlag = 0u;

//private Variable definition
static u16 LowPowerTimeCount = 0;      //进入低功耗计时器

//extern function declaration
extern void SaveSerialNum(void);

/*******************************************************************************
**函    数： WKUP_PortInit
**功    能： 唤醒引脚初始化
**参    数： void
**返    回： void
**说    明： 上升沿唤醒
*******************************************************************************/
void WKUP_PortInit(void)
{
    GPIOx_Cfg(GPIO_A, pin_0, IN_DOWN);
}

/*******************************************************************************
**函    数： WKUP_GetState
**功    能： 唤醒引脚状态读取
**参    数： void
**返    回： void
*******************************************************************************/
void WKUP_GetState(void)
{
    if( pin_0 == GPIOx_Get(GPIO_A, pin_0))  //高电平激活状态
    {
        StandbyCountReset();                // 睡眠等待时间重新计时
    }
}

/*******************************************************************************
**函    数： StandbyCountReset
**功    能： 清低功耗计时器
**参    数： void
**返    回： void
**说    明： 接口，供无线升级时使用
*******************************************************************************/
void StandbyCountReset(void)
{
    LowPowerTimeCount = 0;
}

/*******************************************************************************
**函    数： LowPower_CheckTime
**功    能： 进入低功耗倒计时 （ 10秒 ）
**参    数： void
**返    回： void
**说    明： 需要100毫秒运行一次  100ms*100 = 10S
*******************************************************************************/
void LowPower_CheckTime(void)
{
    if(LowPowerTimeCount++ >= STANDBY_TIME_COUNT)
    {
        LowPowerTimeCount = 0;
        OLED_ClearScreen();
        SaveSerialNum();
        AirCondition_SaveData();    //保存空调配置数据
        Standby_Mode();           
    }    
}

/*******************************************************************************
**函    数： Standby_Mode
**功    能： 开启待机模式
**参    数： void
**返    回： void
**说    明： 片外模块电源需要在此函数内部关闭
*******************************************************************************/
void Standby_Mode(void)
{
    /*关中断*/
    SysTick->CTRL = 0;                  //禁止Systick中断
    NVIC_DisableIRQ(EXTI15_10_IRQn);    //禁止Si4432中断  
    DisableIRQ();                       //禁止所有用户中断
    
    //PowerOFF_LED;           //关闭背光灯电源
    //OLED_POW_0;             //关闭OLED电源
    RF_SWITCH_OFF;          //关闭433电源    
    
    SPI2->CR1    &= ~(1<<6);            //关闭SPI2
    RCC->APB1ENR &= ~(1<<14);           //关闭SPI2时钟
    RCC->APB2ENR &= ~(1<<14);           //关闭USART1时钟
    RCC->AHBRSTR |= 1<<0;               //Reset GPIOA
    
    RCC->APB1ENR |= 1<<28;              //使能电源时钟    
    /* Set SLEEPDEEP bit    : Select SleepDeep mode*/
    SCB->SCR|= SCB_SCR_SLEEPDEEP;   
    /* Set PDDS bit         : Select STANDBY mode */
    PWR->CR |= PWR_CR_PDDS; 
    /* Set CWUF bit         : Clear Wakeup flag */
    PWR->CR |= PWR_CR_CWUF;     
    /* Set EWUP1 bit        : Enable EWUP pin 1 :PA0*/
    PWR->CSR|= PWR_CSR_EWUP1;
    /* Request Wait For Interrupt */	
		__WFI();
//		SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP);
}

/*******************************************************************************
**函    数： EXTI0_IRQHandler
**功    能： 外部唤醒中断
**说    明： 
*******************************************************************************/
void EXTI0_IRQHandler(void)
{   
	EXTI->PR = 1<<0;  //清除LINE0上的中断标志位 
}  

/***************************Copyright BestFu **********************************/


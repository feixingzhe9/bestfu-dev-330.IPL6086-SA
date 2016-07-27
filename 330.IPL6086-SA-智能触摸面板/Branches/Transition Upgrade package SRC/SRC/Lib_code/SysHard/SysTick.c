/***************************Copyright BestFu 2014-05-14*************************
文	件：	SysTick.c
说	明：	系统滴答
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2013-12-20
修　改：	暂无
*******************************************************************************/
#include "stm32l1xx.h"
#include "SysTick.h"
#include "Thread.h"
#include "Queue.h"
#include "sys.h"
#include "key.h"

static TimeFlag timer;
extern Queue_t  gQueue;
extern void ShowLED_FlashLED(u8 key);
extern void ShowLED_OpenAll(void);
/*******************************************************************************
函 数 名：	SysTick_Init
功能说明： 	Cotex_M3 系统滴答初始化
参	  数： 	sysClock： 系统时钟(单位：M)
返 回 值：	无
注    意:   系统初始化后将产生1ms系统滴答中断
*******************************************************************************/
void SysTick_Init(u32 sysClock)
{
	SysTick->CTRL &= 0xfffffffb;   //bit2清空,选择外部时钟  HCLK/8
	SysTick->LOAD = 1000*sysClock/8; //1ms	  		 
	SysTick->VAL = 0x00;           //清空计数器
	SysTick->CTRL = 0x03;          //开启中断响应,开始倒数
	timer._1ms = 0; 
}

/*******************************************************************************
函 数 名：	Time_Get
功能说明： 	获取当前时间
参	  数： 	无
返 回 值：	当前计数值
*******************************************************************************/
u32 Time_Get(void)
{
    return timer._1ms;
}

/*******************************************************************************
** 函数名称: 	SysTick_Handler
** 功能描述: 	系统滴答中断服务程序
** 输　  入:   	无
** 返    回:   	无
*******************************************************************************/
void SysTick_Handler(void)
{
    static u8 KeyCount = 0;
    static u8 FisrtFlag = 1;
    KeySta_t keyTemp;
    timer._1ms %= TIME_MAX;	//最大记数1000s
	timer._1ms++;
    KeyCount++;
	Thread_RunCheck(timer._1ms);
    if(KeyCount >= 5)
    {
        KeyCount = 0;
        if(FisrtFlag)
        {
            keyTemp.KeyValue=Key_FirstHandleScan(&keyTemp.KeyType);
            if(0xFF != keyTemp.KeyValue)
            {
                CPU_SR_ALLOC();
                cpu_sr = CPU_SR_Save();
                Enqueue(&gQueue,keyTemp);
                CPU_SR_Restore(cpu_sr);

                FisrtFlag = 0;
                ShowLED_OpenAll();
            }
        }
        else
        {
            keyTemp.KeyValue=Key_Scan(&keyTemp.KeyType);
            if((0xFF != keyTemp.KeyValue))                  //&&(keyTemp.KeyType == 0X01)
            {
                CPU_SR_ALLOC();
                cpu_sr = CPU_SR_Save();
                Enqueue(&gQueue,keyTemp);
                CPU_SR_Restore(cpu_sr);
                if(FisrtFlag)
                {
                    FisrtFlag = 0;
                    ShowLED_OpenAll();
                }
                else
                {
                    ShowLED_FlashLED(keyTemp.KeyValue);
                }
            }
        }
    }
}

/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu ***********************************
**  文    件：  LowPower.h
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
#ifndef _LOW_POWER_H_
#define _LOW_POWER_H_

extern void StandbyCountReset(void);		//清睡眠计时器
extern void LowPower_CheckTime(void);		//进入低功耗倒计时(100ms执行一次)
extern void Standby_Mode(void);
#endif	

/***************************Copyright BestFu **********************************/

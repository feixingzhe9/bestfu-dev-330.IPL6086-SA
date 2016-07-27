/***************************Copyright BestFu ***********************************
**  文    件：  TIMx.h
**  功    能：  <<驱动层>> 通用定时器驱动 STM32L151 TIM2/TIM3/TIM4/TIM5
**  编    译：  Keil uVision5 V5.10
**  芯    片：  STM32L151
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.08.20
**  修改日期：  2014.08.20
**  说    明：  
**  V1.0
    >> 
*******************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32l1xx.h"
#include "BF_TYPE.H"    

//fuction declaration
void TIMx_Init(TIM_TypeDef *TIMx , u16 psc , u16 arr);

#endif

/********************************* END FILE ***********************************/

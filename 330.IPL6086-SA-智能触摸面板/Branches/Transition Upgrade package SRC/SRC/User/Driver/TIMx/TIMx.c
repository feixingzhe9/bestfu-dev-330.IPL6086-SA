/***************************Copyright BestFu ***********************************
**  文    件：  TIMx.c
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
#include "sys.h"
#include "TIMx.h"
#include "middle_led.h"
#include "adc.h"
#include "UnitCfg.h"

volatile u16 gTIMx_Unit=0;          //周期计数器器

/*******************************************************************************
**函    数:  Init_TIMx()
**功    能:  通用定时器TIM2/TIM3/TIM4/TIM5 初始化
**参    数:  *TIMx    -- TIM2/TIM3/TIM4/TIM5
**           psc     -- 预分频值
**           arr     -- 自动重装值
**返    回:  null
**说    明： Init_TIMx(TIM2,31,Count);   //32Mhz  1us分频单位 默认向上计数           
********************************************************************************/
void Init_TIMx(TIM_TypeDef *TIMx , u16 psc , u16 arr)
{
    u8 x=0;

    x = ((u32)TIMx - TIM2_BASE)/(0x0400);
    RCC->APB1ENR |= (1<<x); 

    TIMx->ARR  = arr;           //自动重装值
    TIMx->PSC  = psc;           //预分配器    CK_CNT = fck_psc/(psc+ 1) = 32M/(psc+1)   
                                // 100Khz = 10us  : psc = 320
    TIMx->CR2 &= ~(0xF<<4);     //清主模式
    TIMx->CR2 |= 4<<4;          //

    TIMx->DIER|=1<<0;           //使能中断  
    MY_NVIC_Init(2,3,TIM2_IRQn,2);  
    
    TIMx->CNT  = 0;             //清计数器  
    //TIMx->CR1 |= 1<<0;        //使能定时器
}

/*******************************************************************************
**函    数： TIMx_Start
**功    能： 启动定时器 TIMx
**参    数： *TIMx    -- TIM2/TIM3/TIM4/TIM5
**           arr      -- 自动重装值
**返    回： void
*******************************************************************************/
void TIMx_Start(TIM_TypeDef *TIMx , u16 arr)
{
    TIM2_MS = 0;
    TIMx->ARR  = arr;           //自动重装值
    TIMx->CNT  = 0;             //清计数器  
    TIMx->CR1 |= 1<<0;          //使能定时器 
}

/*******************************************************************************
**函    数： TIMx_Stop
**功    能： 停止定时器 TIMx
**参    数： *TIMx    -- TIM2/TIM3/TIM4/TIM5
**返    回： 返回计数值
*******************************************************************************/
u32 TIMx_Stop(TIM_TypeDef *TIMx)
{
    u32 count=0;
    count = TIMx->CNT;
    TIMx->CR1 &=~(1<<0);        //关闭定时器
    count += TIM2_MS*1000;
    return  count;
}

/******************************************************************************
**函    数: TIM2_IRQHandler
**功    能: 中断服务程序
******************************************************************************/
void TIM2_IRQHandler(void)
{   
    if(TIM2->SR&0X0001)
    {   
        gTIMx_Unit++;
        //TIM2->CR1 &=~(1<<0);          //关闭定时器
    }                  
    TIM2->SR&=~(1<<0);
}
void TIM3_IRQHandler(void)
{   
    if(TIM3->SR&0X0001)
    {   
        
    }                  
    TIM3->SR&=~(1<<0);
}
void TIM4_IRQHandler(void)
{   
    if(TIM4->SR&0X0001)
    {   
        
    }                  
    TIM4->SR&=~(1<<0);
}
void TIM5_IRQHandler(void)
{   
    if(TIM5->SR&0X0001)
    {   
        
    }                  
    TIM5->SR&=~(1<<0);
}

/********************************* END FILE ***********************************/

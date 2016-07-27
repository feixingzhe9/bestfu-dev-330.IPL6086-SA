/***************************Copyright BestFu ***********************************
**  文    件：  ADC.c
**  功    能：  <<驱动层>> ADC驱动 
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.08.20
**  修改日期：  2014.08.20
**  说    明：  
**  V1.0
    >> STM32L151C8T6  ADC1 驱动
*******************************************************************************/
#include "ADC.h"
#include "delay.h"

/*******************************************************************************
**函    数： ADC_PortInit
**功    能： ADC端口初始化
**参    数： void
**返    回： void
*******************************************************************************/
static void ADC_PortInit(void)
{
    GPIOx_Cfg(PORT_ADC_SWITCH, PIN_ADC_SWITCH, OUT_PP_400K);    
    GPIOx_Set(PORT_ADC_SWITCH, PIN_ADC_SWITCH);                 //开启ADC测量电路供电  

    GPIOx_Cfg(PORT_CHRG,PIN_CHRG,IN_FLOATING);    
    GPIOx_Cfg(PORT_STDBY,PIN_STDBY,IN_FLOATING);   
    
    GPIOx_Cfg(PORT_ADC, PIN_ADC, IN_ANALOG);                    //设成模拟输入
}

/*******************************************************************************
**函    数： ADC_TestPowerON
**功    能： ADC测量电路 电源开
**参    数： void
**返    回： void
*******************************************************************************/
void ADC_TestPowerON(void)
{
    GPIOx_Set(PORT_ADC_SWITCH, PIN_ADC_SWITCH); 
}

/*******************************************************************************
**函    数： ADC_TestPowerOFF
**功    能： ADC测量电路 电源关
**参    数： void
**返    回： void
*******************************************************************************/    
void ADC_TestPowerOFF(void)
{
    GPIOx_Rst(PORT_ADC_SWITCH, PIN_ADC_SWITCH); 
}

/*******************************************************************************
**函    数： ADC_Init
**功    能： ADC1初始化
**参    数： void
**返    回： void
**说    明： >> ADC时钟为内部高速时钟
             >> 单次转换模式
*******************************************************************************/
void  ADC_Init(void)
{ 
    u32 cnt=0;
    
    RCC->CR |= 1<<0;                    //开启HSI
    ADC_PortInit();
    while(!(RCC->CR&(1<<1)))            //等待HSI时钟开启完
    {
        if(++cnt>1000) break;
    }
    RCC->APB2ENR  |= 1<<9;              //ADC1时钟使能  
    RCC->APB2RSTR |= 1<<9;              //ADC1复位
    RCC->APB2RSTR &= ~(1<<9);           //复位结束  
    /* CR1寄存器配置 */
    ADC1->CR1 &= ~((3<<24)|(1<<8));     //Clear < RES , SCAN > bits
    ADC1->CR1 |= (0<<24);               //设置精度   0:12bit  1:10bit  2:8bit  3:6bit
    ADC1->CR1 |= (0<<8);                //扫描模式   0:关闭   1:打开    
    /* CR2寄存器配置 */
    ADC1->CR2 &= ~((3<<28)|(15<<24)|(1<<11)|(1<<1)); //Clear < EXTEN,EXTSEL,ALIGN,CONT > bits
    ADC1->CR2 |= (0<<28);               //沿触发模式  0:禁止  1:上升沿  2:下降沿  3:上升或下降沿
    ADC1->CR2 |= (0<<24);               //外部触发事件 
    ADC1->CR2 |= (0<<11);               //对齐方式   0:右对齐 1:左对齐    
    ADC1->CR2 |= (0<<1);                //转换模式   0:单次     1:连续      
    /* SQR1寄存器配置 */
    ADC1->SQR1|= (0<<20);               //规则通道序列长度： 0~27 ：1~28个转换通道
    //SMPR 1.2.3   采样时钟配置
    //ADC1->SMPR3 |= (1<<24);           //默认 4个采样周期
    
    ADC1->SQR5 &= ~(0X1F<<0);           //清规则序列
    ADC1->SQR5 |= (ADC_CH<<0);          //转换通道  0~31 : 0~31通道

    ADC1->CR2  |= 1<<0;                 //ADCON
    
    //MY_NVIC_Init(1,1,ADC1_IRQn,2);    //抢占2，子优先级0，组2  
}   

/*******************************************************************************
**函    数： ADC_Read
**功    能： 读取一次ADC转换值
**参    数： *ADC_Result     -- 返回读取的ADC转换值
**返    回： 1：成功     0：失败
*******************************************************************************/
u8 ADC_Read(u16 *adcResult)   
{
    u16 cnt = 0;

    ADC1->CR2 |= 1<<30;             //软件启动转换    
    while(!(ADC1->SR&(1<<1)))       //等待转换结束
    {
        if(++cnt>2000)  return 0;   //超时退出
    }
    *adcResult = ADC1->DR;          //返回adc值 
    return 1;       
}

/*******************************************************************************
**函    数： ADC_ReadAverage
**功    能： 读取ADC转换平均值
**参    数： times          -- 平均值次数
**           *ADC_Result    -- 返回读取的ADC平均值
**返    回： 1：成功     0：失败
*******************************************************************************/
u8 ADC_ReadAverage(u8 times,u16 *adcAverage)
{
    u32 temp_val=0;
    u16 ADC_val = 0;
    u8  t;

    ADC1->CR2  |= 1<<0;                 //ADCON  POWER ON 
    
    for(t=0;t<times;t++)
    {
        if(1==ADC_Read(&ADC_val))
        {
            temp_val += ADC_val;
            delay_us(500);
        }
        else
        {
            ADC1->CR2 &= ~(1<<0);       //ADCON   POWER OFF
            return 0;   
        }           
    }
    *adcAverage = temp_val/times;
    ADC1->CR2 &= ~(1<<0);               //ADCON   POWER OFF
    return 1;
} 

/*******************************************************************************
**函    数： ADC1_IRQHandler
**功    能： ADC1中断服务程序
*******************************************************************************/     
//void ADC1_IRQHandler(void)
//{
//  battery_voltage = Get_Adc_Average(8,50);
//  ADC1->SR &= ~(1<<2); //清除中断
//  
//}

/***************************Copyright BestFu **********************************/

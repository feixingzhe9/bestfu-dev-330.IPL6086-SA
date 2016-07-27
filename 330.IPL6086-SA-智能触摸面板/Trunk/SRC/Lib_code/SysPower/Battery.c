/***************************Copyright BestFu ***********************************
**  文    件：  Battery.c
**  功    能：  <<逻辑层>> 电池逻辑接口
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.08.20
**  修改日期：  2014.08.20
**  说    明：  
**  V1.0
    >> 电量检测  1% 精度
*******************************************************************************/
#include "ADC.h"
#include "GPIO.h"
#include "Unitcfg.h"
#include "Battery.h"

/* 锂电池 电压与 电量分度表*/
                     // 100  90   80   70   60   50   40   30   20   10   1  %
const u16 VoltageTab[]={4100,4030,3970,3900,3830,3780,3730,3690,3650,3620,3400};

/*全局变量*/
u8 gPowerPercent;       //剩余电量
u8 gPowerLv=0;            //剩余电等级

/*******************************************************************************
**函    数： Battery_GetVoltage
**功    能： 获取电池电压值
**参    数： *voltage        -- 读取电压值
**返    回： 1：成功     0：失败
*******************************************************************************/
u8 Battery_GetVoltage(u16 *voltage)
{
    u16 adc_value=0;
    u32 tmp_voltage=0;
    
    ADC_TestPowerON();
    if(1==ADC_ReadAverage(10,&adc_value))
    {
        /* 计算公式(电路1/2分压) ： Voltage = ADC_Result *3300 *2 / 4096 */
        tmp_voltage = adc_value*825;        
        *voltage    = tmp_voltage/512; 
        ADC_TestPowerOFF();     
        return 1;
    }
    ADC_TestPowerOFF();     
    return 0;
}

/*******************************************************************************
**函    数： Battery_GetPercent
**功    能： 获取电池百分比 (精度 1%)
**参    数： *powerPercent        -- 返回电量百分值
**返    回： 1：成功       powerPercent = [1,100]
**           0：失败       powerPercent = 0xFF;
*******************************************************************************/
u8 Battery_GetPercent(u8 *powerPercent)
{
    u16  vtg=0;
    u8   numb=0,precent=0;
    
    if(1 == Battery_GetVoltage(&vtg))
    {
        if(vtg>=VoltageTab[0])      //100%
        {
            precent=100;
        }       
        else if(vtg>=VoltageTab[1]) //90%
        {
            numb = (vtg - VoltageTab[1])/((VoltageTab[0] - VoltageTab[1])/10);
            precent=90+numb;
        }
        else if(vtg>=VoltageTab[2])//80%
        {
            numb = (vtg - VoltageTab[2])/((VoltageTab[1] - VoltageTab[2])/10);
            precent=80+numb;
        }
        else if(vtg>=VoltageTab[3])//70%
        {
            numb = (vtg - VoltageTab[3])/((VoltageTab[2] - VoltageTab[3])/10);
            precent=70+numb;
        }
        else if(vtg>=VoltageTab[4])//60%
        {
            numb = (vtg - VoltageTab[4])/((VoltageTab[3] - VoltageTab[4])/10);
            precent=60+numb;
        }
        else if(vtg>=VoltageTab[5])//50%
        {
            numb = (vtg - VoltageTab[5])/((VoltageTab[4] - VoltageTab[5])/10);
            precent=50+numb;
        }
        else if(vtg>=VoltageTab[6])//40%
        {
            numb = (vtg - VoltageTab[6])/((VoltageTab[5] - VoltageTab[6])/10);
            precent=40+numb;
        }
        else if(vtg>=VoltageTab[7])//30%
        {
            numb = (vtg - VoltageTab[7])/((VoltageTab[6] - VoltageTab[7])/10);
            precent=30+numb;
        }
        else if(vtg>=VoltageTab[8])//20%
        {
            numb = (vtg - VoltageTab[8])/((VoltageTab[7] - VoltageTab[8])/10);
            precent=20+numb;
        }
        else if(vtg>=VoltageTab[9])//10%
        {
            numb = (vtg - VoltageTab[9])/((VoltageTab[8] - VoltageTab[9])/10);
            precent=10+numb;
        }
        else if(vtg>=VoltageTab[10])//1%
        {
            numb = (vtg - VoltageTab[10])/((VoltageTab[9] - VoltageTab[10])/10);
            precent=1+numb;
        }
        else precent=1;
        
        *powerPercent = precent;
         
        if(gPowerPercent>80)        gPowerLv = 4;   //4格  
        else if(gPowerPercent>60)   gPowerLv = 3;   //3格
        else if(gPowerPercent>40)   gPowerLv = 2;   //2格
        else if(gPowerPercent>20)   gPowerLv = 1;   //1格
        else                        gPowerLv = 0;   //0格
               
        return  1;
    }
    else 
    {
        *powerPercent = 0xFF;
        gPowerLv = 0;
        return 0;
    }
}

/***************************Copyright BestFu **********************************/

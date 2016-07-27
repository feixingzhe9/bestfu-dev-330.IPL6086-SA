/***************************Copyright BestFu ***********************************
**  文    件:   ShakeMotor.c
**  功    能：  <<驱动层>>震动马达驱动
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1.0
**  编    写：  Seven
**  创建日期：  2014.08.20
**  修改日期：  2014.08.20
**  说    明：  
*******************************************************************************/
#include "ShakeMotor.h"
#include "BF_type.h"
#include "GPIO.h"
#include "delay.h"

u8 gMotorRunFlag;

/*******************************************************************************
**函    数:  ShakeMotor_Init()
**功    能:  震动马达初始化
**参    数:  void                  
**返    回:  void
*******************************************************************************/
void ShakeMotor_Init(void)
{   
    GPIOx_Cfg(MOTOR_PORT, MOTOR_PIN, OUT_PP_2M);
    GPIOx_Rst(MOTOR_PORT, MOTOR_PIN);
}

/*******************************************************************************
**函    数:  ShakeMotor_Start()
**功    能:  震动马达
**参    数:  void                  
**返    回:  void
*******************************************************************************/
void ShakeMotor_Start(void)
{   
    GPIOx_Set(MOTOR_PORT, MOTOR_PIN);
}

/*******************************************************************************
**函    数:  ShakeMotor_Stop()
**功    能:  停止马达
**参    数:  void                  
**返    回:  void
*******************************************************************************/
void ShakeMotor_Stop(void)
{   
    GPIOx_Rst(MOTOR_PORT, MOTOR_PIN);
}
/*******************************************************************************
**函    数:  ShakeMotor_RunState()
**功    能:  马达运行状态机
**参    数:  void               
**返    回:  void
**说    明： 该函数 每隔 10ms 调用一次
             不带延时函数的 马达震动
*******************************************************************************/
void ShakeMotor_RunState(void)
{
    static u8 run_count;
    static u8 stop_count;
    
    if(gMotorRunFlag==1)
    {
        gMotorRunFlag = 0;
        run_count = 2;
        stop_count= 3;
    }
    else if(gMotorRunFlag==2)
    {
        gMotorRunFlag = 0;
        run_count = 6;
        stop_count= 3;
    }
    if(run_count)
    {
        run_count--;
        GPIOx_Set(MOTOR_PORT, MOTOR_PIN);
    }
    else if(stop_count)
    {
        stop_count--;
        GPIOx_Rst(MOTOR_PORT, MOTOR_PIN);
        //if(stop_count==0) GetMPU6050Flag=0;
    }
}

/***************************Copyright BestFu **********************************/

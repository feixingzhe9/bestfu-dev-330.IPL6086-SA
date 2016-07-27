/***************************Copyright BestFu ***********************************
**  文    件:   ShakeMotor.h
**  功    能：  <<驱动层>>震动马达驱动
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1.0
**  编    写：  Seven
**  创建日期：  2014.08.20
**  修改日期：  2014.08.20
**  说    明：  
*******************************************************************************/
#ifndef _SHAKE_MOTOR_H_
#define _SHAKE_MOTOR_H_

#include "BF_type.h"

#define MOTOR_PORT GPIO_A
#define MOTOR_PIN  pin_3

extern u8 gMotorRunFlag;

extern void ShakeMotor_Init(void);
extern void ShakeMotor_Run(void);
extern void ShakeMotor_Start(void);
extern void ShakeMotor_Stop(void);

#endif

/***************************Copyright BestFu **********************************/

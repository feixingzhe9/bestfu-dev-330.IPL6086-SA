/***************************Copyright BestFu ***********************************
**  文    件：  Battery.h
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
#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "BF_TYPE.h"

//全局变量
extern u8 gPowerPercent;
extern u8 gPowerLv; 

extern u8 Battery_GetPercent(u8 *energyPercent);

#endif

/***************************Copyright BestFu **********************************/

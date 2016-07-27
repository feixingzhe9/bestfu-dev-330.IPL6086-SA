/***************************Copyright BestFu ***********************************
**  文    件：  Cube6.h
**  功    能：  <<逻辑层>> 魔方六面体逻辑层接口
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1
**  编    写：  Seven
**  创建日期：  2014.05.26
**  修改日期：  2014.08.20
**  说    明：  
**  V1.1
    >> 
*******************************************************************************/
#ifndef _CUBE_6_H_
#define _CUBE_6_H_

#include "MPU6050.h"

#define CUBE6_SUPPORT_PRINTF    0       //是否支持 调试打印

extern u8 gEnsureFace;       //确认面  

extern u8    Cube6_SearchFace(void);
extern void  Cube6_SaveEeprom(void);
extern void  Cube6_ReadEeprom(void);

#endif

/***************************Copyright BestFu **********************************/

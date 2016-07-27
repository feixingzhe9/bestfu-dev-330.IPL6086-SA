/***************************Copyright BestFu ***********************************
**  文    件：  BatteryExeAttr.h
**  功    能：  <<属性层>> 电池属性接口 
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.08.20
**  修改日期：  2014.08.20
**  说    明：  
**  V1.0
    >>
*******************************************************************************/
#ifndef _BATTERY_EXE_ATTR_H_
#define _BATTERY_EXE_ATTR_H_

#include "BF_TYPE.h"
#include "UnitCfg.h"

extern void Battery_Init(void);
extern MsgResult_t Get_PowerPercent_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
extern MsgResult_t Get_ChargeState_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);
extern void Battery_Show_Fun(void );
extern void Battery_Show(void );

#endif
/***************************Copyright BestFu **********************************/


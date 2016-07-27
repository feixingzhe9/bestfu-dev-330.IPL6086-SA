/***************************Copyright BestFu ***********************************
**  文    件：  Cube6ExeAttr.h
**  功    能：  <<属性层>> 魔方六面体属性命令接口
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1
**  编    写：  Seven
**  创建日期：  2014.05.26
**  修改日期：  2014.08.20
**  说    明：  
**  V1.1
    >> 
*******************************************************************************/
#ifndef _CUBE20_EXE_ATTR_H_
#define _CUBE20_EXE_ATTR_H_

#include "BF_type.h"
#include "UnitCfg.h"

extern const AttrExe_st Cube6AttrTab[];

void Cube6Init(u8 unitID);
void Cube6EepromInit(u8 unitID);
void Cube6Checking_Attr(void);
u8   Get_Cube6Face_Attr(UnitPara_t *pdata, u8 *rlen, u8 *rpara);

#endif

/***************************Copyright BestFu **********************************/

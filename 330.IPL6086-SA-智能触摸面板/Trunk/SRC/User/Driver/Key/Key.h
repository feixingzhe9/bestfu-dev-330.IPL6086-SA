/***************************Copyright BestFu ***********************************
**  文    件：  Key.c
**  功    能：  <<逻辑层>> 摁键处理 
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1
**  编    写：  Seven
**  创建日期：  2014.08.20
**  修改日期：  2014.08.20
**  说    明：  
**  V1.1
    >> 短按键为释放时才执行，并添加长按键响应
*******************************************************************************/
#ifndef _KEY_H_
#define _KEY_H_

#include "BF_type.h"

#define MAX_TOUCH_KEY       (6+1)   //通道0 保留
#define KEY_SUPPORT_PRINTF   0      //是否支持串口打印

/*按键状态*/
typedef enum
{
    KEY_NONE  = 0,
    KEY_SHORT = 1,
    KEY_LONG  = 2
}KeyState_e;

typedef struct
{
    KeyState_e  Status;
}KeyProp_t;

extern KeyProp_t KeyCh[MAX_TOUCH_KEY];          //按键属性状态保存

extern void Key_Init(void);                     //按键初始化
extern u8   Key_Scan(u8 *keyFlag);              //正常按键扫描
extern void Key_Handle(void);                   //普通按键处理
extern u8   Key_FirstScan(void);                //首次按键扫描
extern void Key_FirstHandle(u8 Key);            //首次按键处理

#endif

/***************************Copyright BestFu **********************************/

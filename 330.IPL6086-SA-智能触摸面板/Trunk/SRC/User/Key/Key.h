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

#define MAX_TOUCH_KEY       (6+1)  			//通道0 保留
#define MAX_KEY_NUM			(MAX_TOUCH_KEY)	//定义面板支持的最大按键数
#define NONE_KEY			(0xff)			//定义无键的键值
#define LONG_KEY_TIMES		(250)			//长按键计数值250 * 5ms = 1.25s

extern u8 gPassword[4];
extern u8 gPswFlag;

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

typedef struct
{
    u8    KeyValue;
    u8    KeyType; 
}KeySta_t,*pKeySta_t;

//variable declaration
extern u8 gPassword[4];
extern u8 gPswFlag;

//fuction declaration
void Key_Init(void);                     //按键初始化
u8 Key_Scan(u8 *keyFlag,u8 keyAccuracy);              //正常按键扫描
void Key_Handle(void);                   //普通按键处理
u8   Key_FirstScan(void);                //首次按键扫描
void KeyStateScan(void);
void KeyTimer_Init(void);
#endif

/***************************Copyright BestFu **********************************/

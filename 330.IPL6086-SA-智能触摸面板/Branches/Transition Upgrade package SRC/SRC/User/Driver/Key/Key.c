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
#include "Key.h"
#include "LinkAll.h"
#include "delay.h"
#include "LowPower.h"
#include "WTC6106.h"
#include "USART1.h"
#include "MsgPackage.h"
#include "ShakeMotor.h"

/*定义摁键值*/
const u8 NONE_KEY=63;       
const u8 KEY6    =62;
const u8 KEY5    =61;
const u8 KEY4    =59;
const u8 KEY3    =55;
const u8 KEY2    =47;
const u8 KEY1    =31;

/*定义摁键值--提供给属性层使用*/
KeyProp_t  KeyCh[MAX_TOUCH_KEY];        //KeyCh[0] 保留

static u8  NewKey,OldKey,LstKey;
static u16 LongKeyCount;                //长按键计数器
const  u16 LONG_KEY_TIMES = 100;        //长按键计数时间 100 *10ms = 1S

/*******************************************************************************
**函    数： Key_Init()
**功    能： 按键模块初始化
**参    数： void
**返    回： void
*******************************************************************************/
void Key_Init(void)
{
    WTC6106_PortInit();
    NewKey=NONE_KEY;
    OldKey=NONE_KEY;
    LstKey=NONE_KEY;
}
/*******************************************************************************
**函    数： Key_Scan()
**功    能： 状态机方式扫描摁键
**参    数： *keyFlag    按键标志    0-无按键  1-短按键    2-长按键 
**返    回： 摁键值
**说    明： 该函数 每隔 5~20ms 调用一次
*******************************************************************************/
u8 Key_Scan(u8 *keyFlag)
{
    static u8 MidValidKey = NONE_KEY;                               //有效按键中间保存值

    *keyFlag = 0;
    NewKey = WTC6106_ReadKey();
    if(NewKey == OldKey)
    {
        if(NewKey==NONE_KEY)                                        //无按键 或 释放按键    
        {   
            if((LstKey != NONE_KEY)&&(LongKeyCount<LONG_KEY_TIMES)) // 释放按键 且 非长按键
            {
                if(LongKeyCount>6)  //过滤超短按键 灵敏度可调
                {
                    LstKey  = NONE_KEY;
                    LongKeyCount = 0;
                    *keyFlag =1;
                    return MidValidKey;                             //返回短键释放
                }
            }
            else{                                                   
                LstKey = NONE_KEY;
                LongKeyCount = 0;
                return NONE_KEY;                    
            }
        }
        else if(NewKey==LstKey)                                     //连续按键
        {
            if(LongKeyCount++ == LONG_KEY_TIMES)
            {
                *keyFlag =2;
                return MidValidKey;                                 //返回长键确认
            }
        }
        else{                                                       //单键
            LstKey = NewKey;
            switch(NewKey)
            {
                case KEY1:
                    MidValidKey = 3;
                    break;
                case KEY2:
                    MidValidKey = 2;
                    break;
                case KEY3:
                    MidValidKey = 1;
                    break;
                case KEY4:
                    MidValidKey = 4;
                    break;
                case KEY5:
                    MidValidKey = 5;
                    break;
                case KEY6:
                    MidValidKey = 6;
                    break;
                default:
                    MidValidKey = NONE_KEY;
                    break;
            }
            return NONE_KEY ;         
        }
    }
    else OldKey = NewKey;               //抖动处理
    return NONE_KEY;
}

/*******************************************************************************
**函    数： Key_Handle()
**功    能： 摁键处理
**参    数： void 
**返    回： void
**说    明： 该函数 每隔 5~20ms 调用一次
*******************************************************************************/
void Key_Handle(void)
{
    u8 i,ch,key_flag;
    
    ch = Key_Scan(&key_flag);

    if( ch != NONE_KEY)
    {
        /*清除上次状态*/
        for(i=0;i<MAX_TOUCH_KEY;i++)
        {
            KeyCh[i].Status = KEY_NONE; 
        }                   
        StandbyCountReset();
        ShakeMotor_Start();
        Thread_Login(ONCEDELAY, 1, 200 , &ShakeMotor_Stop);
        
        if(1 == key_flag)
        {
            KeyCh[ch].Status = KEY_SHORT; 
            PropEventFifo(1, ch, SRCEVENT , KEY_SHORT);     //50ms          
            Upload();                                       //2.5s//上报
        }
        else if(2 == key_flag)
        {
            KeyCh[ch].Status = KEY_LONG;    
            PropEventFifo(1, ch, SRCEVENT , KEY_LONG);        
            Upload();                                       //上报
        }
        #if KEY_SUPPORT_PRINTF
        printf("New Key = %d ,flag = %d \r\n",ch,key_flag);
        #endif
    }
}

/*******************************************************************************
**函    数:  Key_FirstScan()
**功    能:  按键唤醒扫描
**参    数:  void
**返    回:  首次有效按键值 
*******************************************************************************/
u8 Key_FirstScan(void)
{
    u8 first_key = NONE_KEY;
    u8 temp_key  = NONE_KEY;
    
    first_key = WTC6106_ReadKey();
    delay_us(200);
    if(first_key == WTC6106_ReadKey())
    {
        switch(first_key)
        {
            case KEY1:
                temp_key = 3;
                break;
            case KEY2:
                temp_key = 2;
                break;
            case KEY3:
                temp_key = 1;
                break;
            case KEY4:
                temp_key = 4;
                break;
            case KEY5:
                temp_key = 5;
                break;
            case KEY6:
                temp_key = 6;
                break;
            default:
                temp_key = NONE_KEY;
                break;
        }
    }
    return temp_key;
}

/*******************************************************************************
**函    数:  Key_FirstHandle()
**功    能:  首次按键处理函数
**参    数:  按键值
**返    回:  void
********************************************************************************/
void Key_FirstHandle(u8 Key)
{
    if( Key != NONE_KEY)
    {   
        ShakeMotor_Start();
        Thread_Login(ONCEDELAY, 1, 200 , &ShakeMotor_Stop);
        KeyCh[Key].Status = KEY_SHORT; 
        PropEventFifo(1, Key, SRCEVENT , KEY_SHORT);
        Upload(); 
        #if KEY_SUPPORT_PRINTF
        printf("First Key = %d ,ShortKey \r\n",Key);
        #endif        
    }
}

/***************************Copyright BestFu **********************************/

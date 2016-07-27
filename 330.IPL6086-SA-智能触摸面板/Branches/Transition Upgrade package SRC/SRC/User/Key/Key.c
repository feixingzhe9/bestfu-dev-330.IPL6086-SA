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
#include "WTC6216.h"
#include "USART1.h"
#include "MsgPackage.h"
#include "ShakeMotor.h"
#include "OLED.h"
#include "ShowLED.h"
#include "TouchPadPic.h"
#include "AirConditionPad.h"
#include "Queue.h"
#include "sys.h"
//#include "stdio.h"


/*定义摁键值*/
const u8 NONE_KEY=0xFF;       
static u8 FirstPassWord = 0;  
/*定义摁键值--提供给属性层使用*/
KeyProp_t  KeyCh[MAX_TOUCH_KEY];        //KeyCh[0] 保留

static u8  NewKey,OldKey,LstKey;
static u16 LongKeyCount;                //长按键计数器
const  u16 LONG_KEY_TIMES = 100;        //长按键计数时间 200 *5ms = 1S
//extern u8 Sleep_key;

#define MAX_KEY_NUM 12
//按键映射表 最大16个按键   表位置0保留
//顺序按键 匹配的 实际按键值
//                             1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
const u8  KeyMapTab[17] = {0,  31,59,62,47,55,61,0, 0,0, 0,0, 0, 0, 0, 0, 0};
//const u8  KeyMapTab[17] = {0,  1, 2, 3, 4, 5, 6, 8, 8, 9,10,11,12,13,14, 0, 0};

u8 gPassword[4];
u8 gPswFlag=1;
    
u8 InputPassword[4];
u8 PswBit=0;

Queue_t  gQueue;
    
/*******************************************************************************
**函    数： Key_Init()
**功    能： 按键模块初始化
**参    数： void
**返    回： void
*******************************************************************************/
void Key_Init(void)
{
    WTC6106_Init();
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
    u8 i;
    static u8 MidValidKey = NONE_KEY;                               //有效按键中间保存值
	*keyFlag = 0;
	NewKey = WTC6106_ReadKey();
    if(NewKey == OldKey)
    {
        if(NewKey==NONE_KEY)                                        //无按键 或 释放按键    
        {   
            if((LstKey != NONE_KEY)&&(LongKeyCount<LONG_KEY_TIMES)) // 释放按键 且 非长按键
            {
                //if(LongKeyCount>2)  //过滤超短按键 灵敏度可调
                {
                    LstKey  = NONE_KEY;
                    LongKeyCount = 0;
                    *keyFlag = KEY_SHORT;
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
                *keyFlag = KEY_LONG;
				LongKeyCount = 0;
                return MidValidKey;                                 //返回长键确认
            }
        }
        else{                                                       //单键
            LstKey = NewKey;
            for(i=1 ; i<=MAX_KEY_NUM ; i++)
            {
                if( KeyMapTab[i] == NewKey )  
                {
                    MidValidKey = i;
                    break;
                }
            }
            if( i > MAX_KEY_NUM)              MidValidKey = NONE_KEY;
            return NONE_KEY ;         
        }
    }
    else OldKey = NewKey;               //抖动处理
    return NONE_KEY;
}

/*******************************************************************************
**函    数： Key_FirstHandleScan()
**功    能： 状态机方式扫描摁键
**参    数： *keyFlag    按键标志    0-无按键  1-短按键    2-长按键 
**返    回： 摁键值
**说    明： 该函数 每隔 5~20ms 调用一次
*******************************************************************************/
u8 Key_FirstHandleScan(u8 *keyFlag)
{
    u8 i;
    static u8 MidValidKey = NONE_KEY;                               //有效按键中间保存值
	*keyFlag = 0;
	NewKey = WTC6106_ReadKey();
    if(NewKey == OldKey)
    {
        if(NewKey==NONE_KEY)                                        //无按键 或 释放按键    
        {   
            if(LstKey != NONE_KEY) // 释放按键 
            {
                if(LongKeyCount>4)  //过滤超短按键 灵敏度可调
                {               
                    LstKey  = NONE_KEY;
                    LongKeyCount = 0;
                    *keyFlag = KEY_LONG;
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
            LongKeyCount++ ;
        }
        else{                                                       //单键
            LstKey = NewKey;
            for(i=1 ; i<=MAX_KEY_NUM ; i++)
            {
                if( KeyMapTab[i] == NewKey )  
                {
                    MidValidKey = i;
                    break;
                }
            }
            if( i > MAX_KEY_NUM)              MidValidKey = NONE_KEY;
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
    u8 i,ch;
    static u8 FirstFlag = 1;
//    u8 key_flag;
    u8 res = 0x00;
    KeySta_t keyTemp;
    CPU_SR_ALLOC();
    cpu_sr = CPU_SR_Save();
    res = Dequeue(&gQueue,&keyTemp);
    CPU_SR_Restore(cpu_sr);
    if(res)
    {   
        ch = keyTemp.KeyValue;
//        key_flag = keyTemp.KeyType;
        if(FirstFlag)
        {
            FirstFlag = 0;
            StandbyCountReset();
//          AirCondition_FirstShowPicture();
        }
        else
        {           
            if(gPswFlag)        //密码开启
            {
                switch(FirstPassWord)
                {
                    case 0:
                    {
                        FirstPassWord = 1;
                        OLED_ClearScreen();
                        OLED_ShowString(2,0,"请输入密码",0);
                        OLED_ShowString(6,2,"----",0);
                    }
                    break;
                    case 1:
                    {
                        if(ch <= 6)      //123456按键有效
                        {
                            StandbyCountReset();            
                            OLED_ShowChar(6+PswBit,2,ch+'0',0);
                            InputPassword[PswBit] = ch;
                            if(PswBit++ >= 3)
                            {
                                PswBit=0;
                                if((gPassword[0] == InputPassword[0])&& \
                                   (gPassword[1] == InputPassword[1])&& \
                                   (gPassword[2] == InputPassword[2])&& \
                                   (gPassword[3] == InputPassword[3]) )
                                {
                                    gPswFlag = 0;
                                    OLED_ClearScreen();
    //                                OLED_ShowString(4,1,"欢迎使用",0);
                                    
                                    FirstFlag = 0;
                                    StandbyCountReset();
                                    AirCondition_FirstShowPicture();
                                }
                                else 
                                {
                                    OLED_ShowString(1,0,"请重新输入密码",0);
                                    OLED_ShowString(4,1,"密码错误",0);
                                    OLED_ShowString(6,2,"----",0);
                                }
                            }
                        }  
                    }
                    break;
                    default:break;
                }
                
            }
            else  if((ch>0)&&(ch<MAX_TOUCH_KEY))   //!= NONE_KEY
            {
                /*清除上次状态*/
                for(i=0;i<MAX_TOUCH_KEY;i++)
                {
                    KeyCh[i].Status = KEY_NONE; 
                }    
                StandbyCountReset();
                if(ch == 2)   //空调电源按键
                {
                    if(gAirContionData.PowerSwitch == AC_POWER_ON)
                    {
                        gAirContionData.PowerSwitch = AC_POWER_OFF;
                        OLED_ClearScreen();
                        OLED_ShowHalfPicAt(4,PicTab[19]);           //空调关闭图片
                        AirCondition_SendData();
                    }
                    else 
                    {
                        gAirContionData.PowerSwitch = AC_POWER_ON;
                        AirCondition_FullShow();
                    }
                }
                else if(ch <= 6)   //空调面板所属按键 (除开电源按键)
                {
                    if(gAirContionData.PowerSwitch == AC_POWER_ON)
                    {
                        AirCondition_KeyHander(ch); 
                    }
                    else if(gAirContionData.PowerSwitch == AC_POWER_OFF)  //电源关
                    {
//                        OLED_ClearScreen();
//                        OLED_ShowHalfPicAt(4,PicTab[19]);           //空调关闭图片
                        
//                        gAirContionData.PowerSwitch = AC_POWER_ON;
//                        AirCondition_FullShow();
//                        AirCondition_KeyHander(ch);
                        gAirContionData.PowerSwitch = AC_POWER_ON;
                        AirCondition_FullShow();
                    }
                }
               Upload(); 
            }
        }
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
    u8 i;
    u8 first_key = NONE_KEY;
    u8 temp_key  = NONE_KEY;
    
    first_key = WTC6106_ReadKey();
    delay_us(10);                     /**> 时间过长，会造成误触发 **/
    if(first_key == WTC6106_ReadKey())
    {
        for(i=1 ; i<=MAX_KEY_NUM ; i++)
        {
            if( KeyMapTab[i] == first_key )  
            {
                temp_key = i;
                break;
            }
        }
        if( i > MAX_KEY_NUM)              temp_key = NONE_KEY;           
    }
    return temp_key;
}
#if 0
/*******************************************************************************
**函    数:  Key_FirstHandle()
**功    能:  首次按键处理函数
**参    数:  按键值
**返    回:  void
********************************************************************************/
void Key_FirstHandle(u8 Key)
{
    if(gPswFlag)
    {
        OLED_ShowString(2,0,"请输入密码",0);
        OLED_ShowString(6,2,"----",0);
    }
    else if( Key != NONE_KEY)
    {   
        gUIMode = UIMODE_AIRCON;            
        AirCondition_FullShow();
        Upload(); //上报         
    }
}
#endif 

/***************************Copyright BestFu **********************************/


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
#include "TIMx.h"
#include "sys.h"
//#include "stdio.h"

//Public Variable definitions
u8 gPassword[4] = {0};									//定义真实的密码存储数组
u8 gPswFlag = 0u; 										//用于记录密码存在与否属性值

//Private Variable definition
static const u8  KeyMapTab[7] = {0,31,59,62,47,55,61};	//按键映射表 最大16个按键,表位置0保留
static u8 InputPassword[4] = {0};						//定义用户输入的密码值缓冲数组
static Queue_t  Queue;									//定义按键接收队列
static u8 PswBit = 0; 									//记录密码已输入的位数

//Private function declaration
static void CheckUserPassWordInput(u8 keynum);

/*******************************************************************************
**函    数： Key_Init()
**功    能： 按键模块初始化
**参    数： void
**返    回： void
*******************************************************************************/
void Key_Init(void)
{
	WTC6106_Init();
    InitQueue(&Queue);
}

/*******************************************************************************
**函    数： void KeyTimer_Init(void)
**功    能： 按键定时器模块初始化
**参    数： void
**返    回： void
*******************************************************************************/
void KeyTimer_Init(void)
{
	u32 count = 0xffffffff;
	while((Get_TouchFlag())||(0 == count))
	{
		count--;
	} 
	TIMx_Init(TIM2 ,32, 6399);		//初始化中断频率5ms
}

/*******************************************************************************
**函    数： void KeyStateScan(void)
**功    能： 按键状态扫描
**参    数： void
**返    回： void
*******************************************************************************/
void KeyStateScan(void)
{
	KeyCurStatus_t keyTemp;
	
	keyTemp.KeyValue=Key_Scan((u8*)&keyTemp.KeyType , 0);	
	if((NONE_KEY != keyTemp.KeyValue))                 
	{                
		CPU_SR_ALLOC();
		cpu_sr = CPU_SR_Save();
		Enqueue(&Queue,keyTemp);
		CPU_SR_Restore(cpu_sr);
		ShowLED_FlashLED(keyTemp.KeyValue);
	}
}

/*******************************************************************************
**函    数： Key_Scan()
**功    能： 状态机方式扫描摁键
**参    数： *keyFlag    按键标志    0-无按键  1-短按键    2-长按键 
**返    回： 摁键值
**说    明： 该函数 每隔 5~20ms 调用一次
*******************************************************************************/
u8 Key_Scan(u8 *keyFlag,u8 keyAccuracy)
{
    u8 i;
    static u8 MidValidKey = NONE_KEY;       //有效按键中间保存值
	static u8  NewKey = NONE_KEY;
	static u8  OldKey = NONE_KEY;
	static u8  LstKey = NONE_KEY;
	static u16 LongKeyCount = 0;            //长按键计数器
	
    *keyFlag = 0;
    NewKey = WTC6106_ReadKey();
    //OLED_ShowNumber(0,1,NewKey,3,0);
    if(NewKey == OldKey)
    {
        if(NewKey==NONE_KEY)                                        //无按键 或 释放按键    
        {   
            if((LstKey != NONE_KEY)&&(LongKeyCount<LONG_KEY_TIMES)) // 释放按键 且 非长按键
            {
                if(LongKeyCount > keyAccuracy)  					//过滤超短按键 灵敏度可调
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
    KeyCurStatus_t keyTemp;
    u8 result = 0x00;
	
	CPU_SR_ALLOC();
    cpu_sr = CPU_SR_Save();
    result = Dequeue(&Queue,&keyTemp);
    CPU_SR_Restore(cpu_sr);	
	
    if(result)
    {   
//        key_flag = keyTemp.KeyType;           
        if(gPswFlag > 0u)        //密码开启
		{
			CheckUserPassWordInput(keyTemp.KeyValue);
		}
		else if(NONE_KEY != keyTemp.KeyValue)
		{    
			StandbyCountReset();
			if(2 == keyTemp.KeyValue)   //空调电源按键
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
			else		//空调面板所属按键 (除开电源按键)
			{
				if(gAirContionData.PowerSwitch == AC_POWER_ON)
				{
					AirCondition_KeyHander(keyTemp.KeyValue); 
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
		   Upload(1); 
		}
    }
}

/*******************************************************************************
**函    数： static void CheckUserPassWordInput(u8 keynum)
**功    能： 用户密码处理函数
**参    数： keynum：用户输入的按键值 
**返    回： void
**说    明： 该函数 每隔 5~20ms 调用一次
*******************************************************************************/
static void CheckUserPassWordInput(u8 keynum)
{
	static u8 FirstPassWord = 0u;  
	
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
			if(keynum <= 6)      //123456按键有效
			{
				StandbyCountReset();
				OLED_ShowChar(6+PswBit,2,keynum + '0',0);
				InputPassword[PswBit] = keynum;
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
//                      OLED_ShowString(4,1,"欢迎使用",0);
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
    delay_us(10);                         /**> 时间过长，会造成误触发 **/
	
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
        if( i > MAX_KEY_NUM)              
			temp_key = NONE_KEY;           
    }
    return temp_key;
}

/***************************Copyright BestFu **********************************/


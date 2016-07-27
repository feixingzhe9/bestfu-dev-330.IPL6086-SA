/***************************Copyright BestFu ***********************************
**  文    件：  AirContionPad.c
**  功    能：  设备关联--空调面板部分
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1
**  编    写：  Seven
**  创建日期：  2014.08.26
**  修改日期：  2014.09.01
**  说    明：  >>
*******************************************************************************/
#include "AirConditionPad.h"
#include "EEPROM.h"
#include "MSGPackage.h"
#include "OLED.h"
#include "TouchPadPic.h"
#include "Thread.h"

#define DEVICE_RELATE_START_ADDR (0x4000)     //EEPROM存储地址

AirConditionData_t gAirContionData;         //空调面板数据
DeveceRelate_s     gDeveceRelate;           //设备关联结构体
u8  gUIMode;    

void Show_Temper(void);
void AirCondition_SaveData(void);
void AirCondition_ReadSaveData(void);
void AirCondition_SendData(void);
//void AirCondition_CMDSend(ObjectType_t relateType);
static void AirCondition_CMDSend(void);
//const u8  AC_KeyMapTab[5] = {11,  9, 1,14, 6,10, 2,12, 4, 7,11, 3,13, 5, 0, 0, 0};

/*******************************************************************************
**函    数： Init_AirCondition
**功    能： 从EEPROM读取并初始化空调面板数据
**参    数： void
**返    回： void
*******************************************************************************/
void Init_AirCondition(void)
{
    AirCondition_ReadSaveData();

    //设备关联相关数据初始化
    if(gDeveceRelate.UseFlag>0x02)         gDeveceRelate.UseFlag    = 0;           //使用标志-默认关闭
    if(gDeveceRelate.RelateType>0x04)      gDeveceRelate.RelateType = 0;           //关联类型-默认无节点
    if(*(u32*)&gDeveceRelate.ObjectID > 0xfffffff0)  
                                   *(u32*)&gDeveceRelate.ObjectID   = 0;           //目标地址
    if(*(u16*)&gDeveceRelate.AssistID > 0xfff0)      
                                   *(u16*)&gDeveceRelate.AssistID   = 0;           //辅助地址
    if(gDeveceRelate.UnitNum>0x80)         gDeveceRelate.UnitNum    = 0;           //单元号
    
    //空调类数据初始化
	if(gAirContionData.PowerSwitch>0x01)   gAirContionData.PowerSwitch  = 0;       //电源开关
    
	if((gAirContionData.SetTemper>gAirContionData.SetMaxTemp)||
        (gAirContionData.SetTemper<gAirContionData.SetMinTemp))
    {
                                           gAirContionData.SetTemper    = 25;      //设置温度
    }
    
	if(gAirContionData.SetMode>0x04)       gAirContionData.SetMode      = 0;       //设置模式		
	if(gAirContionData.SetWindSpeed>0x04)  gAirContionData.SetWindSpeed = 0;	   //设置风速
	if(gAirContionData.SetWindSweep>0x03)  gAirContionData.SetWindSweep = 0;	   //扫风开关
    
//    Thread_Login(ONCEDELAY, 0, 50 ,&AirCondition_FirstShowPicture); 
    AirCondition_FirstShowPicture();
}

/*******************************************************************************
**函    数： AirCondition_SaveData
**功    能： 空调面板保存数据到EEPROM
**参    数： void
**返    回： void
*******************************************************************************/
void AirCondition_SaveData(void)
{
    WriteDataToEEPROM(DEVICE_RELATE_START_ADDR,sizeof(DeveceRelate_s), (u8 *)&gDeveceRelate);
    WriteDataToEEPROM(DEVICE_RELATE_START_ADDR+sizeof(DeveceRelate_s),sizeof(gAirContionData), (u8 *)&gAirContionData);
}

/*******************************************************************************
**函    数： AirCondition_ReadSaveData
**功    能： 从EEPROM读取保存数据
**参    数： void
**返    回： void
*******************************************************************************/
void AirCondition_ReadSaveData(void)
{
    ReadDataFromEEPROM(DEVICE_RELATE_START_ADDR,sizeof(DeveceRelate_s),(u8 *)&gDeveceRelate);
    ReadDataFromEEPROM(DEVICE_RELATE_START_ADDR+sizeof(DeveceRelate_s),sizeof(gAirContionData), (u8 *)&gAirContionData); 

    if(gDeveceRelate.UseFlag > 0x02)    gDeveceRelate.UseFlag       = 1 ; //默认使能
    if(gDeveceRelate.RelateType > 0x04) gDeveceRelate.RelateType    = 0 ; //未关联
    if(gDeveceRelate.UnitNum > 0xF0)    gDeveceRelate.UnitNum       = 1 ; //单元号1
    
    if(gAirContionData.PowerSwitch > AC_POWER_ON)     gAirContionData.PowerSwitch = AC_POWER_OFF;
    if(gAirContionData.SetMode > AC_MODE_MAX)         gAirContionData.SetMode     = AC_MODE_AUTO;
    
//    if((gAirContionData.SetTemper > AC_MAX_TEMPER)|| \
//        (gAirContionData.SetTemper < AC_MIN_TEMPER))  gAirContionData.SetTemper   = AC_MAX_TEMPER;
    
    if(gAirContionData.SetWindSpeed > AC_WIND_MAX)    gAirContionData.SetWindSpeed= AC_WIND_AUTO;
    
    if((gAirContionData.SetMaxTemp>MAX_TEMP)||(gAirContionData.SetMinTemp<MIN_TEMP)||  
        (gAirContionData.SetMinTemp>=gAirContionData.SetMaxTemp)) 
    {
        gAirContionData.SetMaxTemp = 32;
        gAirContionData.SetMinTemp = 16;
        AirCondition_SaveData();
    }
}

/*******************************************************************************
**函    数： AirCondition_KeyHander
**功    能： 空调面板按键数据处理
**参    数： keyNum        --按键号
**返    回： void
*******************************************************************************/
void  AirCondition_KeyHander(u8 keyNum)
{
    switch(keyNum)
    {         
        case 1:                     // - 温度 -
            if(gAirContionData.SetMode != AC_MODE_AUTO)     //自动模式不可以设置温度
            {
                if(gAirContionData.SetTemper > gAirContionData.SetMinTemp)  gAirContionData.SetTemper--;
            }
            Show_Temper();
            break;        
        case 2:                     //电源按键
            if(gAirContionData.PowerSwitch == AC_POWER_ON)
            {
                gAirContionData.PowerSwitch = AC_POWER_OFF;
                OLED_ClearScreen();
                OLED_ShowHalfPicAt(4,PicTab[18]);           //空调关闭图片
            }
            else 
            {
                gAirContionData.PowerSwitch = AC_POWER_ON;
                OLED_ClearScreen();
                OLED_ShowHalfPicAt(4,PicTab[19]);           //空调打开图片
            }
            break;
        case 3:                     //+ 温度 +
            if(gAirContionData.SetMode != AC_MODE_AUTO)     //自动模式不可以设置温度
            {
                if(gAirContionData.SetTemper < gAirContionData.SetMaxTemp) gAirContionData.SetTemper++;  
            }
            Show_Temper();
            break;
        case 4:                     // 模式
            if(gAirContionData.SetMode < AC_MODE_MAX)    gAirContionData.SetMode ++;
            else gAirContionData.SetMode = AC_MODE_AUTO;
        
            OLED_ShowQuartPicAt(0,0,PicTab_AC[gAirContionData.SetMode]);    //显示 空调模式图片
            break;
        case 5:                     // 风速
            if(gAirContionData.SetWindSpeed < AC_WIND_MAX)
                gAirContionData.SetWindSpeed ++;
            else gAirContionData.SetWindSpeed = AC_WIND_AUTO;
            
            OLED_ShowQuartPicAt(0,4,PicTab_AC[gAirContionData.SetWindSpeed+5]);     //显示 风速模式图片
            break;  
        case 6:                     //扫风开关
            if ( gAirContionData.SetWindSweep < AC_WINDSWEEP_All )
            {
                gAirContionData.SetWindSweep++;
            }
            else gAirContionData.SetWindSweep = AC_WINDSWEEP_OFF;
            // 液晶屏显示
            
            switch(gAirContionData.SetWindSweep)
            {
                case AC_WINDSWEEP_OFF:
                {
                    OLED_ShowString(9,3,"     ",0);
                }break;
                case AC_WINDSWEEP_OpDown:
                {
                    OLED_ShowPicAt( 9, 6, 23, 16, &PicTab_Wind[0]);
                    OLED_ShowString(11,3,"  ",0);
                }break;
                case AC_WINDSWEEP_LeftWright:
                {
                    OLED_ShowString(9, 3,"  ",0);                
                    OLED_ShowPicAt(11, 6, 23, 16, &PicTab_Wind[1*46]);
                }break;
                case AC_WINDSWEEP_All:
                {
                    OLED_ShowPicAt( 9, 6, 23, 16, &PicTab_Wind[0*46]);
                    OLED_ShowPicAt(11, 6, 23, 16, &PicTab_Wind[1*46]);
                }break;
                default:break;
            }
            break;
        default : 
            return;//break;        
    } 
    //发送数据
    AirCondition_SendData();
}

/*******************************************************************************
**函    数： AirCondition_SendData
**功    能： 发送空调面板设备关联数据
**参    数： void
**返    回： void
*******************************************************************************/
void  AirCondition_SendData(void)
{
    if(1 == gDeveceRelate.UseFlag)                      //使能状态
    {
       /* AirCondition_CMDSend((ObjectType_t)gDeveceRelate.RelateType);  */
        Thread_Login(ONCEDELAY, 0, 600 ,&AirCondition_CMDSend);
    }
}

/*******************************************************************************
**函    数： AirCondition_CMDSend
**功    能： 空调面板自定义命令发送
**参    数： relateType            --关联类型
**返    回： void
*******************************************************************************/
static void AirCondition_CMDSend(void)
{
    u8 pData[10];   
    
    pData[0] = gAirContionData.PowerSwitch; 
    pData[1] = gAirContionData.SetTemper;
    pData[2] = gAirContionData.SetMode;
    pData[3] = gAirContionData.SetWindSpeed;
    pData[4] = gAirContionData.SetWindSweep;
    
    //              目录类型/组/场景          目标ID       组、场景号   单元号   属性号   数据长度   数据  
    //void Msg_Send(ObjectType_t objecttype, u32 objectID, u16 actNum, u8 unit, u8 cmd, u8 len, u8 *data);
    
    Msg_Send((ObjectType_t)gDeveceRelate.RelateType, *(u32*)&gDeveceRelate.ObjectID,*(u16*)&gDeveceRelate.AssistID, \
             gDeveceRelate.UnitNum , 0x50 , 5 , pData);
}


#if 0
/*******************************************************************************
**函    数： AirCondition_CMDSend
**功    能： 空调面板自定义命令发送
**参    数： relateType            --关联类型
**返    回： void
*******************************************************************************/
void AirCondition_CMDSend(ObjectType_t relateType)
{
    u8 pData[10];   
    
    pData[0] = gAirContionData.PowerSwitch; 
    pData[1] = gAirContionData.SetTemper;
    pData[2] = gAirContionData.SetMode;
    pData[3] = gAirContionData.SetWindSpeed;
    pData[4] = gAirContionData.SetWindSweep;
    
    //              目录类型/组/场景          目标ID       组、场景号   单元号   属性号   数据长度   数据  
    //void Msg_Send(ObjectType_t objecttype, u32 objectID, u16 actNum, u8 unit, u8 cmd, u8 len, u8 *data);
    
    Msg_Send(relateType, *(u32*)&gDeveceRelate.ObjectID,*(u16*)&gDeveceRelate.AssistID, \
             gDeveceRelate.UnitNum , 0x50 , 5 , pData);
}
#endif

//显示温度
void Show_Temper(void)
{
    u8 a[3];
    a[0] = gAirContionData.SetTemper/10;
    a[1] = gAirContionData.SetTemper%10;
    OLED_ShowNum18x32(66,2,PicTab_NUM[a[0]]);
    OLED_ShowNum18x32(66+18,2,PicTab_NUM[a[1]]);
}
/*******************************************************************************
**函    数： AirCondition_FirstCMD
**功    能： 显示界面从普通按键切换过来
**参    数： relateType            --关联类型
**返    回： void
*******************************************************************************/
void AirCondition_FullShow(void)
{
    if(gAirContionData.PowerSwitch == AC_POWER_ON)  //开状态
    {
        OLED_ClearScreen();
        OLED_ShowQuartPicAt(0,0,PicTab_AC[gAirContionData.SetMode]);        //显示 空调模式图片
        OLED_ShowQuartPicAt(0,4,PicTab_AC[gAirContionData.SetWindSpeed+5]); //显示 风速模式图片
        
        OLED_DrawLine(64);  //画竖线
        OLED_Show32x323PicAt(12,2,PicTab_32x32[0]);//摄氏度
        Show_Temper();//显示温度
        switch(gAirContionData.SetWindSweep)
        {
            case AC_WINDSWEEP_OFF:
            {
                OLED_ShowString(9,3,"     ",0);
            }break;
            case AC_WINDSWEEP_OpDown:
            {
                OLED_ShowPicAt( 9, 6, 23, 16, &PicTab_Wind[0]);
                OLED_ShowString(11,3,"  ",0);
            }break;
            case AC_WINDSWEEP_LeftWright:
            {
                OLED_ShowString(9, 3,"  ",0);                
                OLED_ShowPicAt(11, 6, 23, 16, &PicTab_Wind[1*46]);
            }break;
            case AC_WINDSWEEP_All:
            {
                OLED_ShowPicAt( 9, 6, 23, 16, &PicTab_Wind[0*46]);
                OLED_ShowPicAt(11, 6, 23, 16, &PicTab_Wind[1*46]);
            }break;
            default:break;
        }
    }
    else OLED_ShowHalfPicAt(4,PicTab[19]);           //空调关闭图片
    
    
    //发送数据
    AirCondition_SendData();
    
}
#if 0
/*******************************************************************************
**函    数： void AirCondition_FullShowPicture(void)
**功    能： 显示界面从普通按键切换过来画面
**参    数： 无
**返    回： void
*******************************************************************************/
void AirCondition_FullShowPicture(void)
{
    if(gAirContionData.PowerSwitch == AC_POWER_ON)  //开状态
    {
        OLED_ClearScreen();
        OLED_ShowQuartPicAt(0,0,PicTab_AC[gAirContionData.SetMode]);        //显示 空调模式图片
        OLED_ShowQuartPicAt(0,4,PicTab_AC[gAirContionData.SetWindSpeed+5]); //显示 风速模式图片
        
        OLED_DrawLine(64);  //画竖线
        OLED_Show32x323PicAt(12,2,PicTab_32x32[0]);//摄氏度
        Show_Temper();//显示温度
        
        //扫风
        switch(gAirContionData.SetWindSweep)
        {
            case AC_WINDSWEEP_OFF:
            {
                OLED_ShowString(9,3,"     ",0);
            }break;
            case AC_WINDSWEEP_OpDown:
            {
                OLED_ShowPicAt( 9, 6, 23, 16, &PicTab_Wind[0]);
                OLED_ShowString(11,3,"  ",0);
            }break;
            case AC_WINDSWEEP_LeftWright:
            {
                OLED_ShowString(9, 3,"  ",0);                
                OLED_ShowPicAt(11, 6, 23, 16, &PicTab_Wind[1*46]);
            }break;
            case AC_WINDSWEEP_All:
            {
                OLED_ShowPicAt( 9, 6, 23, 16, &PicTab_Wind[0*46]);
                OLED_ShowPicAt(11, 6, 23, 16, &PicTab_Wind[1*46]);
            }break;
            default:break;
        }
    }
    else OLED_ShowHalfPicAt(4,PicTab[19]);           //空调关闭图片    
}
#endif
/*******************************************************************************
**函    数： void AirCondition_FirstShowPicture(void)
**功    能： 空调显示上次休眠前的状态
**参    数： 无
**返    回： void
*******************************************************************************/
void AirCondition_FirstShowPicture(void)
{
    if(gAirContionData.PowerSwitch == AC_POWER_ON)  //开状态
    {
        OLED_DrawLine(64);  //画竖线
        OLED_Show32x323PicAt(12,2,PicTab_32x32[0]);//摄氏度
        Show_Temper();
        OLED_ShowQuartPicAt(0,0,PicTab_AC[gAirContionData.SetMode]);    //显示 空调模式图片
        OLED_ShowQuartPicAt(0,4,PicTab_AC[gAirContionData.SetWindSpeed+5]);     //显示 风速模式图片
        switch(gAirContionData.SetWindSweep)
        {
            case AC_WINDSWEEP_OFF:
            {
                OLED_ShowString(9,3,"     ",0);
            }break;
            case AC_WINDSWEEP_OpDown:
            {
                OLED_ShowPicAt( 9, 6, 23, 16, &PicTab_Wind[0]);
                OLED_ShowString(11,3,"  ",0);
            }break;
            case AC_WINDSWEEP_LeftWright:
            {
                OLED_ShowString(9, 3,"  ",0);                
                OLED_ShowPicAt(11, 6, 23, 16, &PicTab_Wind[1*46]);
            }break;
            case AC_WINDSWEEP_All:
            {
                OLED_ShowPicAt( 9, 6, 23, 16, &PicTab_Wind[0*46]);
                OLED_ShowPicAt(11, 6, 23, 16, &PicTab_Wind[1*46]);
            }break;
            default:break;
        }
    }
    else
    {        
        OLED_ShowHalfPicAt(4,PicTab[19]);           //空调关闭图片 
    }
}

/***************************Copyright BestFu **********************************/

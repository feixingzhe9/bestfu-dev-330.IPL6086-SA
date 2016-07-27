/***************************Copyright BestFu ***********************************
**  文    件：  AirContionPad.h
**  功    能：  设备关联--空调面板部分
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.1
**  编    写：  Seven
**  创建日期：  2014.08.26
**  修改日期：  2014.09.01
**  说    明：  >>
*******************************************************************************/
#ifndef _AIR_CONDITION_PAD_H
#define _AIR_CONDITION_PAD_H

#include "BF_type.h"

#define AC_POWER_OFF     0      //电源关
#define AC_POWER_ON      1      //电源开

#define AC_MIN_TEMPER   16      //温度最小值
#define AC_MAX_TEMPER   30      //温度最大值

#define AC_MODE_MAX         4   //模式最大值
    #define AC_MODE_AUTO    0   //自动        自动
    #define AC_MODE_COOL    1   //制冷
    #define AC_MODE_WATER   2   //除湿
    #define AC_MODE_WIND    3   //送风
    #define AC_MODE_HOT     4   //制热

#define AC_WIND_MAX         3   //分模式最大值
    #define AC_WIND_AUTO    0   //自动风速
    #define AC_WIND_LOW     1   //低风速
    #define AC_WIND_MID     2   //中风速
    #define AC_WIND_HIGH    3   //高风速
    //#define AC_WIND_CLOSE   4   //关闭    ...遥控器类无效

#define AC_WINDSWEEP_OFF        0   //扫风关
#define AC_WINDSWEEP_OpDown     1   //扫风开  上下摆风
#define AC_WINDSWEEP_LeftWright 2   //扫风开  左右摆风
#define AC_WINDSWEEP_All        3   //扫风开  上下 + 左右摆风


#define MAX_TEMP (35)
#define MIN_TEMP (10)
//空调类数据结构体
typedef struct
{
    u8      PowerSwitch;    //电源开关
    u8      SetTemper;      //设置温度
    u8      SetMode;        //设置模式      
    u8      SetWindSpeed;   //设置风速
    u8      SetWindSweep;   //扫风开关
    u8		SetMaxTemp;		//高温度
    u8		SetMinTemp;		//低温度
} AirConditionData_t;

//设备关联结构体
typedef  struct 
{
    u8 UseFlag;         //使用标志  0x00.未使用   0x01.使能          0x02.禁能              
    u8 RelateType;      //对象类型  0x00.未关联   0x01.设备单节点    0x02组播         0x04.设备类型广播                      
    u8 ObjectID[4];     //对象地址                |--设备节点ID      |-- 组区域地址   |-- 设备类型广播地址
    u8 AssistID[2];     //辅助地址                |--设备类型        |-- 组号         |-- 设备类型
    u8 UnitNum;			//单元号
}DeveceRelate_s;


extern AirConditionData_t gAirContionData;
extern DeveceRelate_s     gDeveceRelate;
extern u8  gUIMode;  

#define UIMODE_NORMAL     0     //普通模式
#define UIMODE_AIRCON     1     //空调模式

extern void  AirCondition_SaveData(void);
extern void  Init_AirCondition(void);
extern void  AirCondition_KeyHander(u8 keyNum);
extern void  AirCondition_FullShow(void);
extern void  AirCondition_SendData(void);
//extern void AirCondition_FullShowPicture(void);
extern void AirCondition_FirstShowPicture(void);
#endif

/***************************Copyright BestFu **********************************/

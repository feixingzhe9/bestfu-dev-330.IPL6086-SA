/***************************Copyright BestFu ***********************************
**  文    件：  Cube6.c
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
#include "Cube6.h"
#include "USART2.h"
#include "ShakeMotor.h"
#include "LowPower.h"
#include "UnitCfg.h"
#include "Thread.h" 

#define CUBE_EEPROM_START_ADDR       0x4000

u8 gEnsureFace;              //确认面值
u8 gGetMPUFlag;              //MPU6050读保护（防止马达震动干扰）

static u8 OldEnsureFace;
const  s8 ACCE_OFFSRT = 30;  //面值误差范围允许值  

static s8 UpFaceTab[6][3]=   //六面体匹配表
{
    {   0,   0,-100},   
    {   0, 100,   0},   
    { 100,   0,   0},     
    {-100,   0,   0},
    {   0,-100,   0},   
    {   0,   0, 100}
};

/*******************************************************************************
**函    数： SearchFace
**功    能： 寻找及确认面值
**参    数： void
**返    回： 匹配面(1-6)      0xFF   未匹配
*******************************************************************************/
u8 Cube6_SearchFace(void)
{
    u8  i;
    s16 CurTmp[3];  
    static u8 KeepTimes=0;
    static u8 OldFace=0xff;
    static u8 EnsureFlag;
    
    if(1 == gGetMPUFlag) return 0xff;     //震动保护
    
    MPU6050_ReadAcce(CurTmp);             //读取加速度 
    
    for(i=0;i<6;i++)
    {
        if((CurTmp[0]>=UpFaceTab[i][0]-ACCE_OFFSRT)&&(CurTmp[0] < UpFaceTab[i][0]+ACCE_OFFSRT))
        {   
            if( (CurTmp[1] >=UpFaceTab[i][1]-ACCE_OFFSRT)&& \
                (CurTmp[1] < UpFaceTab[i][1]+ACCE_OFFSRT)&& \
                (CurTmp[2] >=UpFaceTab[i][2]-ACCE_OFFSRT)&& \
                (CurTmp[2] < UpFaceTab[i][2]+ACCE_OFFSRT))
            {
                if(i == OldFace)        //判断数据是否保持
                {
                    if((KeepTimes++ >=5)&&(EnsureFlag==0))
                    {
                        KeepTimes  = 0;
                        EnsureFlag = 1;
                        gEnsureFace = i+1;
                        if(OldEnsureFace != gEnsureFace)
                        {
                            StandbyCountReset();                            
                            OldEnsureFace = gEnsureFace; 
                            gGetMPUFlag   = 1;                             
                            PropEventFifo(1, 0x01,SRCEVENT,gEnsureFace);
                            ShakeMotor_Start();
                            Thread_Login(ONCEDELAY, 0 ,50, &ShakeMotor_Stop);
                            Upload();
                            #if  CUBE6_SUPPORT_PRINTF
                            printf("Face OK is %d \r\n",gEnsureFace);
                            #endif                           
                        }                                                           
                    }
                }
                else { 
                    KeepTimes=0;
                    EnsureFlag=0;
                }
                OldFace = i;
                return i;
            }       
        }
    }
    /*The face is not match...*/
    KeepTimes = 0;
    OldFace   = 0xff;
    return 0xff;    
}

/*******************************************************************************
**函    数： Cube6_SaveEeprom
**功    能： 保存最后面值
**参    数： void
**返    回： void
*******************************************************************************/
void  Cube6_SaveEeprom(void)
{
    EEPROM_Write(CUBE_EEPROM_START_ADDR, 1, &OldEnsureFace);
}

/*******************************************************************************
**函    数： Cube6_ReadEeprom
**功    能： 读取上次保存的面值
**参    数： void
**返    回： 匹配面(1-6)      0xFF   未匹配
*******************************************************************************/
void  Cube6_ReadEeprom(void)
{
    EEPROM_Read(CUBE_EEPROM_START_ADDR, 1, &OldEnsureFace);
    if(OldEnsureFace>6)
    {
        OldEnsureFace = 0;
    }
}

/***************************Copyright BestFu **********************************/

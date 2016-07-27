/***************************Copyright BestFu ***********************************
**  文    件：  MPU6050.c
**  功    能：  <<驱动层>> MPU6050驱动 
**  编    译：  Keil uVision5 V5.10
**  版    本：  V1.0
**  编    写：  Seven
**  创建日期：  2014.03.21
**  修改日期：  2014.08.20
**  说    明：  通过IIC 从 MPU6050 芯片获取 加速度和角速度 
**  V1.0
    >> 接口   SDA -> PB11    SCL -> PB10
*******************************************************************************/
#include "IIC.h"
#include "MPU6050.h"
#include "delay.h"
#include "usart.h"

//static s32   BAISACCX,BAISACCY,BAISACCZ;
//static float SCALEACCX,SCALEACCY,SCALEACCZ;

static void MPU6050_Write(u8 regAddr , u8 data);
static u8   MPU6050_Read(u8 regAddr);

/*******************************************************************************
**函    数： MPU6050_Write
**功    能： 给MPU6050寄存器写一个字节
**参    数： regAddr       --寄存器地址
**           data          --数据
**返    回： void
*******************************************************************************/
static void MPU6050_Write(u8 regAddr , u8 data)
{
    IIC_Start();
    IIC_SendByte(MPU6050_ADDR);         //发送器件地址和写命令
    IIC_WaitAck();
    IIC_SendByte(regAddr);              //发送寄存器首地址
    IIC_WaitAck();
    IIC_SendByte(data);
    IIC_WaitAck();                     
    IIC_Stop();
}

/*******************************************************************************
**函    数： MPU6050_Read
**功    能： 给MPU6050寄存器读一个字节
**参    数： regAddr       --寄存器地址 
**返    回： data          --数据
*******************************************************************************/
static u8   MPU6050_Read(u8 regAddr)
{
    u8 data=0;
    IIC_Start();
    IIC_SendByte(MPU6050_ADDR);         //发送器件地址和写命令
    IIC_WaitAck();
    IIC_SendByte(regAddr);              //发送寄存器首地址
    IIC_WaitAck();

    IIC_Start();
    IIC_SendByte(MPU6050_ADDR + 1);     //发送器件地址和写命令
    IIC_WaitAck();
    data=IIC_GetByte();
    IIC_NACK();
    IIC_Stop();                         //产生一个停止条件

    return data;
}

/*******************************************************************************
**函    数： MPU6050_Init
**功    能： MPU6050 初始化
**参    数： void
**返    回： 1         --成功
**           0         --失败
*******************************************************************************/
u8   MPU6050_Init(void)
{   
    u8 sta=0;

    IIC_InitPort();                    //IIC端口初始化
    MPU6050_Read(MPU6050_WHO_AM_I);     //First IIC read possible wrong !!
    if(MPU6050_ADDRESS_AD0_LOW == MPU6050_Read(MPU6050_WHO_AM_I)) sta=1;        //硬件连接正确
                        
    MPU6050_Write(0x19 , 0x07); //SMPLRT_DIV   -- SMPLRT_DIV = 0  Sample Rate = Gyroscope Output Rate / (1 + SMPLRT_DIV)
    MPU6050_Write(0x1A , (2<<0));//CONFIG      -- EXT_SYNC_SET 0 (disable input pin for data sync) ; default DLPF_CFG =2 => ACC bandwidth = 94Hz  GYRO bandwidth = 98Hz)
    MPU6050_Write(0x1B , (3<<3));//GYRO_CONFIG -- FS_SEL = 3: Full scale set to +-2000 deg/sec                          
    MPU6050_Write(0x1C , (0<<3));//ACCE_CONFIG -- AFS_SEL= 0: Full scale set to +-2g
    MPU6050_Write(0x6B , (1<<3));//PWR_MGMT_1  -- SLEEP 0; CYCLE 0; TEMP_DIS 1; CLKSEL 0 (内部 8M晶振)  
    
    return sta;
}

/*******************************************************************************
**函    数： MPU6050_ReadGyro
**功    能： MPU6050读取角速度数据
**参    数： GyroData[0]       --角速度X
**           GyroData[1]       --角速度Y
**           GyroData[2]       --角速度Z
**返    回： void
*******************************************************************************/
void MPU6050_ReadGyro(s16 *GyroData)
{
    u8 buf[6];
    IIC_ReadDataN(MPU6050_GYRO_OUT,buf,6);
    GyroData[0] = (buf[0] << 8) | buf[1];
    GyroData[1] = (buf[2] << 8) | buf[3];
    GyroData[2] = (buf[4] << 8) | buf[5];
}

/*******************************************************************************
**函    数： MPU6050_ReadAcce
**功    能： MPU6050读取加速度数据
**参    数： AccData[0]       --加速度X
**           AccData[1]       --加速度Y
**           AccData[2]       --加速度Z
**返    回： void
*******************************************************************************/
void MPU6050_ReadAcce(s16 *AccData)
{
    u8 buf[6];

    IIC_ReadDataN(MPU6050_ACC_OUT,buf,6);                           //use 1.5ms
    AccData[0] = (float)((s16)(buf[0]<<8 | buf[1]))/32768.0*200;    //扩大100倍
    AccData[1] = (float)((s16)(buf[2]<<8 | buf[3]))/32768.0*200;
    AccData[2] = (float)((s16)(buf[4]<<8 | buf[5]))/32768.0*200;
}

/***************************Copyright BestFu **********************************/

/***************************Copyright BestFu ***********************************
**  文    件：  IIC.c
**  功    能：  <<驱动层>> 模拟IIC驱动
**  编    译：  Keil uVision5 V5.10
**  版    本：  V2.0
**  编    写：  Seven
**  创建日期：  2014.03.21
**  修改日期：  2014.08.20
**  说    明：  模拟IIC协议
**  V2.0
    >> 所有函数重新优化，删除额外操作
    >> 优化移植接口
*******************************************************************************/
#include "IIC.h"
#include "delay.h"
                                                                  
/*******************************************************************************
**函    数： IIC_delay
**功    能： IIC时钟延时  -- 决定IIC传输速度
**参    数： void
**返    回： void
**说    明： 1bit cost double delay
**           if delay is 5us , IIC Speed is 100Kbps
*******************************************************************************/
static void IIC_delay(void) 
{
    //delay_us(5);    
}

/*******************************************************************************
**函    数： IIC_InitPort
**功    能： IIC接口初始化
**参    数： void
**返    回： void
*******************************************************************************/
void IIC_InitPort(void)
{                        
    GPIOx_Cfg(PORT_IIC_SCL, PIN_IIC_SCL, OUT_PU_40M);   
    GPIOx_Cfg(PORT_IIC_SDA, PIN_IIC_SDA, OUT_PU_40M);

    GPIOx_Set(PORT_IIC_SCL, PIN_IIC_SCL);   
    GPIOx_Set(PORT_IIC_SDA, PIN_IIC_SDA);
}

/*******************************************************************************
**函    数： IIC_Start
**功    能： IIC开始信号
**说    明： When SCL is high , SDA change form high to low.
*******************************************************************************/
void IIC_Start(void)
{
    SDA1;
    SCL1;
    IIC_delay();
    SDA0;
    IIC_delay();
    SCL0;
    IIC_delay();
}

/*******************************************************************************
**函    数： IIC_Stop
**功    能： IIC停止信号
**说    明： When SCL is high , SDA change form low to high.
*******************************************************************************/
void IIC_Stop(void)
{
   SDA0;
   SCL1;
   IIC_delay();
   SDA1;
   IIC_delay();
}

/*******************************************************************************
**函    数： IIC_WaitAck
**功    能： IIC等待ACK信号
**参    数： void
**返    回： 0：正常       1：等待超时
**说    明： 该函数加入等待超时处理，防止死等
*******************************************************************************/
u8 IIC_WaitAck(void)
{
    u16 undead = 10000;
    SDA1;
    SDA_IN();       //输入
    SCL1; 
    IIC_delay();
    while(undead--)
    {   
        if(0 == Get_SDA())
        {
            SCL0; 
            SDA_OUT();
            SDA1;           
            IIC_delay();            
            return 0;
        } 
    }
    SCL0;   
    SDA_OUT();
    SDA1;
    IIC_delay();        
    return 1;       
}

/*******************************************************************************
**函    数： IIC_ACK
**功    能： IIC发送ACK信号
*******************************************************************************/
void IIC_ACK(void)                       
{
    SDA0;         
    SCL1;   
    IIC_delay();
    SCL0;
    SDA0;
    IIC_delay();
}

/*******************************************************************************
**函    数： IIC_NACK
**功    能： IIC发送NACK信号
*******************************************************************************/
void IIC_NACK(void)                       
{
    SDA1;          
    SCL1;   
    IIC_delay();
    SCL0;
    SDA0;
    IIC_delay();
}

/*******************************************************************************
**函    数： IIC_SendByte
**功    能： IIC发送一个字节
**参    数： dat       --数据
**返    回： void
**说    明： 高位先发
*******************************************************************************/
void IIC_SendByte(u8 dat)
{
    u8 i;
    for(i=0;i<8;i++)
    {   
        if(dat&0x80)    SDA1;
        else            SDA0;
        dat<<=1;
        SCL1;
        IIC_delay();
        SCL0;
        IIC_delay();    
    }
}

/*******************************************************************************
**函    数： IIC_GetByte
**功    能： IIC读取一个字节
**参    数： void
**返    回： dat       --数据
**说    明： 高位先收
*******************************************************************************/
u8 IIC_GetByte(void)
{   
    u8 i,dat=0;
    SDA_IN();   
    for(i=0;i<8;i++)
    {   
       SCL1;
       IIC_delay();
       dat=dat<<1;
       if(Get_SDA())    dat |= 0x01;
       SCL0;
       IIC_delay();
    }   
    SDA_OUT();   
    SDA1;   
    return dat;     
}

/*******************************************************************************
**函    数： IIC_WriteByte
**功    能： IIC往指定地址写一字节数据
**参    数： Addr    --地址
**           data    --发送的数据
**返    回： void
*******************************************************************************/
void IIC_WriteByte(u8 Addr,u8 data)   
{
    IIC_Start();
    IIC_SendByte(DEVICE_ADDR);  //WriteDeviceAddr
    IIC_WaitAck();
    IIC_SendByte(Addr);
    IIC_WaitAck();
    IIC_SendByte(data);   
    IIC_WaitAck();  
    IIC_Stop(); 
}

/*******************************************************************************
**函    数： IIC_ReadByte
**功    能： IIC读指定地址字节数据
**参    数： Addr    --地址    
**返    回： 接收的数据
*******************************************************************************/
u8 IIC_ReadByte(u8 Addr)         
{      
    u8 data;
    IIC_Start();
    IIC_SendByte(DEVICE_ADDR);
    IIC_WaitAck();
    IIC_SendByte(Addr); 
    IIC_WaitAck();  
    IIC_Start();
    IIC_SendByte(DEVICE_ADDR+1);    //ReadDeviceAddr
    IIC_WaitAck();
    data=IIC_GetByte();
    IIC_NACK();
    IIC_Stop();
    return data;  
}

/*******************************************************************************
**函    数： IIC_WriteDataN
**功    能： IIC往指定地址连续写N个数据
**参    数： Addr    --地址   
**           pData   --发送数据指针
**           len     --发送数据长度
**返    回： void
*******************************************************************************/
void IIC_WriteDataN(u8 Addr , u8 *pData , u8 len)
{
    u8 i,*pTemp= pData;
    IIC_Start();
    IIC_SendByte(DEVICE_ADDR);
    IIC_WaitAck();
    IIC_SendByte(Addr);
    IIC_WaitAck();
    for(i=0;i<len;i++)  
    {
        IIC_SendByte(*pTemp);
        IIC_WaitAck();
        pTemp++ ;
    }
    IIC_Stop(); 
}
                                                                
/*******************************************************************************
**函    数： IIC_ReadDataN
**功    能： IIC在指定地址连续读N个数据
**参    数： Addr    --地址   
**           pData   --接收数据指针
**           len     --接收数据长度
**返    回： void
*******************************************************************************/
void IIC_ReadDataN(u8 Addr,u8 *pData,u8 len)         
{      
    u8 i,*pTemp= pData;
    IIC_Start();
    IIC_SendByte(DEVICE_ADDR);
    IIC_WaitAck();  
    IIC_SendByte(Addr);
    IIC_WaitAck();  
    IIC_Start();
    IIC_SendByte(DEVICE_ADDR+1);
    IIC_WaitAck();
    for(i=0;i<len-1;i++)  
    {    
        *(pTemp++) = IIC_GetByte();
        IIC_ACK();  
    }
    *(pTemp++) = IIC_GetByte();
    IIC_NACK();
    IIC_Stop();
}

/********************************* END FILE ***********************************/

/*******************************************************************************
**  文    件:   USART1.c
**  功    能：  串口1 驱动 （寄存器操作）
**  编    译：  Keil uVision5 V5.10
**  芯    片:   STM32L151xx
**  版    本：  V1.0.1
**  编    写：  Seven
**  创建日期：  2014.04.15
**  修改日期：  2014.08.22
**  说    明：  1.串口1 printf()函数测试OK
                2.发送函数加入timeout
                %%串口接收没有做完整测试
********************************************************************************/                                                                      
#include "STM32L1xx.h"                 
#include "sys.h"
#include "USART1.h"  
#include "GPIO.h"

/*>>加入以下代码,串口1支持printf函数,而不需要选择use MicroLIB*/  
#if USART1_SUPPORT_PRINTF
    #pragma import(__use_no_semihosting)                           
    struct __FILE 
    { 
        int handle; 
    }; 
    FILE __stdout;        
    void _sys_exit(int x) 
    { 
        x = x; 
    } 
    int fputc(int ch, FILE *f)
    {   
        unsigned int timeout=0xFFFF;
        while((USART1->SR&0X40)==0)
        {
            if(0==timeout--) break;
        }
        USART1->DR = (u8) ch;      
        return ch;
    }
#endif 

#if EN_USART1_RX   //如果使能了接收

/*******************************************************************************
**函    数:  USART1_IRQHandler()
**功    能:  中断服务程序
**说    明： 接收一帧数据
********************************************************************************/
void USART1_IRQHandler(void)
{
    if((USART1->SR&(1<<5)) != 0)            //接收到数据
    {    
//      MPU6050_RX_BUF[MPU6050_RX_Counter]=USART1->DR; 
//      if((MPU6050_RX_Counter == 0) && (MPU6050_RX_BUF[0] != 0x55)) return;      //第 0 号数据不是帧头，跳过
//      MPU6050_RX_Counter++; 
//      if(11 == MPU6050_RX_Counter)        //接收到 11 个数据
//      { 
//          MPU6050_RX_Counter=0;           //重新赋值，准备下一帧数据的接收
//          if(0x53==MPU6050_RX_BUF[1])     MPU6050_RX_Flag=1;//只接收处理姿态角数据
//      } 
    }   
} 
#endif                                       

/*******************************************************************************
**函    数:  USART1_Init()
**功    能:  串口1初始化
**参    数:  pclk2       --PCLK2时钟频率(Mhz)  默认使用32M   
**           bound       --波特率
**返    回:  void
**说    明： STM32L151串口驱动
**           默认数据格式--8位数据,1位停止,无校验位.
********************************************************************************/
void USART1_Init(u32 pclk2,u32 bound)
{    
    float temp;
    u16 mantissa;
    u16 fraction;      
    temp=(float)(pclk2*1000000)/(bound*16); //得到USARTDIV
    mantissa=temp;                          //得到整数部分
    fraction=(temp-mantissa)*16;            //得到小数部分     
    mantissa<<=4;
    mantissa+=fraction; 
    
    RCC->AHBENR |=1<<0;                     //使能PORTA口时钟  
    RCC->APB2ENR|=1<<14;                    //使能串口1时钟 
    RCC->APB2RSTR|=  1<<14;                 //复位串口1
    RCC->APB2RSTR&=~(1<<14);                //停止复位      
    
    GPIOx_Cfg(GPIO_A,pin_9,AF_PP_40M);      //TX
    GPIOx_AF (GPIO_A,pin_9,AF_USART1);  
    GPIOx_Cfg(GPIO_A,pin_10,AF_PP_40M);     //RX 变态设置 和F103最大差别
    GPIOx_AF (GPIO_A,pin_10,AF_USART1);
      
    USART1->BRR=mantissa;                   // 波特率设置      
    USART1->CR1|=(1<<13)|(1<<3)|(1<<2);     //(UE)使能usart1 (TE)使能发送 (RE)使能接收 >>8bit数据 1bit停止,无校验位.
    //while((USART1->SR&(1<<6))== 0){}      //等待发送清空
#if EN_USART1_RX                            //使能接收中断
    USART1->CR1|=(1<<5);                    //(RXNEIE)接收中断使能        
    MY_NVIC_Init(2,1,USART1_IRQn,2);
#endif
}

/*******************************************************************************
**函    数:  USART1_SendByte()
**功    能:  串口1发送单个字节
**参    数:  ch          --发送的字符
**返    回:  null
**说    明： 
********************************************************************************/
void USART1_SendByte(u8 ch)
{
    unsigned int timeout=0xFFFF;
    while((USART1->SR&0X40)==0)//等待发送完成
    {
        if(0==timeout--) break;
    }
    USART1->DR = (u8) ch;  
}


/********************************* END FILE ***********************************/

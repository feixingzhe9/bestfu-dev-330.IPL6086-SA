/*******************************************************************************
**  文    件:   USART2.c
**  功    能：  串口2 驱动 （寄存器操作）
**  编    译：  Keil uVision5 V5.10
**  芯    片:   STM32L151xx
**  版    本：  V1.0.1
**  编    写：  Seven
**  创建日期：  2014.04.15
**  修改日期：  2014.08.22
**  说    明：  1.串口2 printf()函数测试OK
                2.发送函数加入timeout
                串口接收没有做完整测试
********************************************************************************/ 

#include "STM32L1xx.h"                 
#include "sys.h"
#include "GPIO.h"
#include "USART2.h"

/*>>加入以下代码,串口2支持printf函数,而不需要选择use MicroLIB*/  
#if USART2_SUPPORT_PRINTF
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
        while((USART2->SR&0X40)==0)
        {
            if(0==timeout--) break;
        }
        USART2->DR = (u8) ch;      
        return ch;
    }
#endif 

#if EN_USART2_RX   //如果使能了接收

/*******************************************************************************
**函    数:  USART2_IRQHandler()
**功    能:  中断服务程序
**说    明： 接收一帧数据
********************************************************************************/
void USART2_IRQHandler(void)
{
    if((USART2->SR&(1<<5)) != 0)//接收到数据
    {    
        //MPU6050_RX_BUF[MPU6050_RX_Counter]=USART2->DR; 
    }   
} 
#endif                                       


/*******************************************************************************
**函    数:  USART2_Init()
**功    能:  串口2初始化
**参    数:  pclk2       --PCLK2时钟频率(Mhz)  默认使用32M   
**           bound       --波特率
**返    回:  void
**说    明： STM32L151串口驱动
**           默认数据格式--8位数据,1位停止,无校验位.
********************************************************************************/
void USART2_Init(u32 pclk1,u32 bound)
{    
    float temp;
    u16 mantissa;
    u16 fraction;      
    temp=(float)(pclk1*1000000)/(bound*16);//得到USARTDIV
    mantissa=temp;               //得到整数部分
    fraction=(temp-mantissa)*16; //得到小数部分    
    mantissa<<=4;
    mantissa+=fraction; 
    
    RCC->AHBENR |=1<<0;                 //使能PORTA口时钟  
    RCC->APB1ENR|=1<<17;                //使能串口2时钟 
    RCC->APB1RSTR|=  1<<17;             //复位串口1
    RCC->APB1RSTR&=~(1<<17);            //停止复位  
    
    GPIOx_Cfg(GPIO_A,pin_2,AF_PP_40M);  //TX
    GPIOx_AF( GPIO_A,pin_2,AF_USART2);  
    GPIOx_Cfg(GPIO_A,pin_3,AF_PP_40M);  //RX     变态设置
    GPIOx_AF( GPIO_A,pin_3,AF_USART2);
  
    USART2->BRR = mantissa;             // 波特率设置     
    USART2->CR1|=(1<<13)|(1<<3)|(1<<2); //(UE)使能usart2 (TE)使能发送 (RE)使能接收 >>8bit数据 1bit停止,无校验位.
    //while((USART1->SR&(1<<6))==0)  {} //等待发送完成    
#if EN_USART2_RX                        //如果使能了接收
    USART2->CR2|=(1<<5);                //(RXNEIE)接收中断使能         
    MY_NVIC_Init(2,2,USART2_IRQn,2);
#endif
}

/*******************************************************************************
**函 数:      USART1_SendByte()
**功 能:      串口1发送单个字节
**参    数:   ch          --发送的字符
**返 回:      null
**说 明：  
********************************************************************************/
void USART2_SendByte(u8 ch)
{
    unsigned int timeout=0xFFFF;
    while((USART2->SR&0X40)==0)     //等待发送清空
    {
        if(0==timeout--) break;
    }
    USART2->DR = (u8) ch;  
}

/********************************* END FILE ***********************************/

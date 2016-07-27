/***************************Copyright bestfu 2015-12-30**************************
**  文    件: gpio.c
**  功    能：stm32l15x GPIO驱动
**  编    译：Keil uVision5 V5.12
**  版    本：V1.0.0
**  编    写：Jay
**  创建日期：2015.12.29
**  修改日期：无
**  说    明：初版
*******************************************************************************/
#include "stm32l1xx.h"
#include "lib_type.h"
#include "gpio.h"

/*******************************************************************************
函 数 名：	void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode)
功能说明： 	配置GPIO引脚状态
参	  数： 	GPIOx   : 对应的GPIO_A~GPIO_G
			GPIO_Pin: 管脚GPIO_Pin_0~GPIO_Pin_15
			mode    :  作用(IN/OUT/AF/AN)_功能((IN_/ /UP|DOWN/FLOATING)
			(OUT/AF_(OP(开漏)/PP(推挽)))_速度(400K/2M/10M/40M)
			(详情请查阅头文件)
返 回 值：	无
说    明:   无
*******************************************************************************/
void GPIO_Config(GPIO_Port GPIO_x,BF_INT16U GPIO_Pin,GPIO_Mode mode)
{
    BF_INT32U position = 0x00;
    BF_INT32U temp = 0x00;

    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    /*---------------------------配置GPIO时钟----------------------------------*/
    if(!(RCC->AHBENR & (1u << (GPIO_x))))
    {
        RCC->AHBENR |= (1u << (GPIO_x));
    }
    /*---------------------------- GPIO 模式配置 --------------------------------*/
    /* 配置GPIO口 */
    while (((GPIO_Pin) >> position) != 0)
    {
        if((GPIO_Pin) & ((BF_INT32U)1 << position))
        {
            /*--------------------- GPIO 模式配置 ------------------------*/
            temp = GPIOx->MODER;
            temp &=~(GPIO_MODER_MODER0 << (position * 2));
            temp |= ((mode&(0x03)) << (position * 2));
            GPIOx->MODER = temp;
            if((0x01 == (mode&0x03))||(0x02 == (mode&0x03)))
            {
                /* 配置GPIO口速度 */
                temp  = GPIOx->OSPEEDR;
                temp &=~(GPIO_OSPEEDER_OSPEEDR0 << (position * 2));
                temp |= (((mode >> 3)&0x03) << (position * 2));
                GPIOx->OSPEEDR = temp;

                /* 配置GPIO口输出类型 */
                temp = GPIOx->OTYPER;
                temp &=~(GPIO_OTYPER_OT_0 << position);
                temp |= (((mode >> 2)&0x01) << position);
                GPIOx->OTYPER = temp;
            }
            /* 激活当前IO的上拉或者下拉电阻 */
            temp = GPIOx->PUPDR;
            temp &=~(GPIO_PUPDR_PUPDR0 << (position * 2));
            temp |= ((mode >> 5) << (position * 2));
            GPIOx->PUPDR = temp;
        }
        position++;
    }
}

/*******************************************************************************
函 数 名：	void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	设置引脚输出状态
参	  数： 	GPIOx   : 对应的GPIO_A~GPIO_G
			GPIO_Pin: 管脚GPIO_Pin_0~GPIO_Pin_15
返 回 值：	无
注    意:   GPIO_Pin = 0xffff; 时即对此端口的所有引脚进行置1
*******************************************************************************/
void GPIO_SetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    GPIOx->BSRRL = (BF_INT16U)GPIO_Pin;
}

/*******************************************************************************
函 数 名：	void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	复位引脚输出状态
参	  数： 	GPIOx   : 对应的GPIO_A~GPIO_G
			GPIO_Pin: 管脚GPIO_Pin_0~GPIO_Pin_15
返 回 值：	无
注    意:   当GPIO_Pin= 0xffff; 时即对此端口的所有引脚进行置0
*******************************************************************************/
void GPIO_ResetBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    GPIOx->BSRRH = (BF_INT16U)GPIO_Pin;
}

/*******************************************************************************
函 数 名：	void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U pin)
功能说明： 	取反引脚输出状态(reverse)
参	  数： 	GPIOx   : 对应的GPIO_A~GPIO_G
			GPIO_Pin: 管脚GPIO_Pin_0~GPIO_Pin_15
返 回 值：	无
注    意:   当pin = 0xffff; 时即对此端口的所有引脚进行取反
*******************************************************************************/
void GPIO_ToggleBits(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    GPIOx->ODR ^= GPIO_Pin;
}

/*******************************************************************************
函 数 名：	BF_INT08U GPIO_ReadOutputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	获取相应端口各引脚的输出状态值
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	返回无符号8位整数,若该引脚被设置则为1,否则为0;
注    意: 	无
*******************************************************************************/
BF_INT08U GPIO_ReadOutputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    BF_INT08U bitstatus = 0x00;
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));

    if ((GPIOx->ODR & GPIO_Pin) != (BF_INT32U)RESET)
    {
        bitstatus = (BF_INT08U)SET;
    }
    else
    {
        bitstatus = (BF_INT08U)RESET;
    }
    return bitstatus;
}

/*******************************************************************************
函 数 名：	BF_INT08U GPIO_ReadInputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	获取相应端口各引脚的输入状态值
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	返回无符号8位整数,若该引脚被设置则为1,否则为0;
注    意: 	无
*******************************************************************************/
BF_INT08U GPIO_ReadInputDataBit(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    BF_INT08U bitstatus = 0x00;
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));

    if ((GPIOx->IDR & GPIO_Pin) != (BF_INT32U)RESET)
    {
        bitstatus = (BF_INT08U)SET;
    }
    else
    {
        bitstatus = (BF_INT08U)RESET;
    }
    return bitstatus;
}

/*******************************************************************************
函 数 名：	void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
功能说明： 	锁定相应GPIO的相应管脚
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	无
注    意: 	在写序列期间,不能更改LCKR[15:0] 的值;将LOCK 序 列 应 用
			到某个端口位后,在执行下一次复位之前,将无法对该端口位的值进行修改;
*******************************************************************************/
void GPIO_PinLockConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin)
{
    volatile BF_INT32U tmp = 0x00010000; //使能端口位锁定功能
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));

    tmp |= GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LCKR = tmp;
    /* Reset LCKK bit */
    GPIOx->LCKR =  GPIO_Pin;
    /* Set LCKK bit */
    GPIOx->LCKR = tmp;
    /* Read LCKK bit*/
    tmp = GPIOx->LCKR;
    /* Read LCKK bit*/
    tmp = GPIOx->LCKR;
}

/*******************************************************************************
函 数 名：	void GPIO_PinRemapConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin, BF_INT08U AF)
功能说明： 	配置GPIO的重映射功能
参	  数： 	GPIO_x		: 对应的GPIO_(A~G)
			GPIO_Pin	: 管脚pin_(0~15)
			AF  		: GPIO_AF0_MCO ~ GPIO_AF15_EVENTOUT (详情请参看头文件)
返 回 值：	无
注    意: 	无
*******************************************************************************/
void GPIO_PinRemapConfig(GPIO_Port GPIO_x, BF_INT16U GPIO_Pin, BF_INT08U AF)
{
    BF_INT32U position = 0x00;
    BF_INT32U temp = 0x00;
    GPIO_TypeDef *GPIOx = (GPIO_TypeDef*)(GPIOA_BASE + (GPIO_x<<10));
    while (((GPIO_Pin) >> position) != 0)
    {
        if((GPIO_Pin) & ((BF_INT32U)1 << position))
        {
            temp = GPIOx->MODER >> (2 * position);
            if(0x02 == (temp&0x03))
            {
                /* Identify AFRL or AFRH register based on IO position*/
                temp = GPIOx->AFR[position >> 3];
                temp&=~((BF_INT32U)0xF << ((BF_INT32U)(position & (BF_INT32U)0x07) * 4));
                temp|= (BF_INT32U)(AF) << (((BF_INT32U)position & (BF_INT32U)0x07) * 4);
                GPIOx->AFR[position >> 3] = temp;
            }
        }
        position++;
    }
}

/********************************End of File***********************************/

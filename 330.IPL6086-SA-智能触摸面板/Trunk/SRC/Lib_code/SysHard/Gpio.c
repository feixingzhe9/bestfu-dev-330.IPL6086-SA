/*******************************************************************
文	件：GPIO.c
说	明：GPIO端口配置文件
编	译：Keil uVision4 V4.54.0.0
版	本：v1.0
编	写：Unarty
日	期：2013-08-09
********************************************************************/

#include "GPIO.h"


/*******************************************************************
函 数 名：	void GPIOx_Cfg(GPIO_Port GPIOx, u16 pin, GPIO_Mode mode)
功能说明： 	配置GPIO引脚状态
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
			mode :  作用(IN/OUT/AF)_功能((IN_/ /UP|DOWN/FLOATING)(OUT/AF_(OP(开漏)/PP(推挽)))
					_速度(2M/10M/50M)  如：复用推挽输出模式50M: AF_PP_50M
返 回 值：	无
*******************************************************************/
void GPIOx_Cfg(GPIO_Port GPIOx, u16 pin, GPIO_Mode mode)
{
	u32 i;
	GPIO_TypeDef *p = (GPIO_TypeDef*)(GPIO_BASE + (GPIOx<<10)); //指向对应GPIO的基址地址
	 
	RCC->AHBENR |= (1<<(GPIOx));	   //GPIO口时钟使能
	
	for (i = 0; pin > 0; i++)
	{
		if (pin&0x01)  //对应pin脚要设置
		{
            p->MODER   &= ~(0x03<<(i<<1));  //模式复位 
			p->OTYPER  &= ~(0x01<<i);
			p->OSPEEDR &= ~(0x03<<(i<<1));
			p->PUPDR   &= ~(0x03<<(i<<1));	
			p->MODER   |= (((mode&0xC0)>>6)<<(i<<1));   //模式配置 
			p->OTYPER  |= (((mode&0x10)>>4)<<i);;
			p->OSPEEDR |= (((mode&0x0C)>>2)<<(i<<1));;
			p->PUPDR   |= (((mode&0x03)>>0)<<(i<<1));;		
		}
		pin >>= 1;
	}
}

/*******************************************************************
函 数 名：	GPIOx_AF
功能说明： 	配置GPIO映射到复用功能引脚
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
			AF	:  作用(IN/OUT/AF)_功能((IN_/ /UP|DOWN/FLOATING)(OUT/AF_(OP(开漏)/PP(推挽)))
					_速度(2M/10M/50M)  如：复用推挽输出模式50M: AF_PP_50M
返 回 值：	无
*******************************************************************/
void GPIOx_AF(GPIO_Port GPIOx, u16 pin, GPIO_AF AF)
{
	u32 i;
	GPIO_TypeDef *p = (GPIO_TypeDef*)(GPIO_BASE + (GPIOx<<10)); //指向对应GPIO的基址地址
	
	for (i = 0; pin > 0; i++)
	{
		if (pin&0x01)  //对应pin脚要设置
		{
            p->AFR[(i>>3)] &= ~(0x0f<<((i&0x07)<<2));  //模式复位 	
			p->AFR[(i>>3)] |= (AF<<((i&0x07)<<2));  //模式配置 	
		}
		pin >>= 1; 
	}
}

/*******************************************************************
函 数 名：	u16 GPIOx_Set(GPIO_Port GPIOx, u16 pin)
功能说明： 	设置引脚输出状态
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	设置后GPIO 输出寄存器值 
注    意:   当pin = 0xffff; 时即对此端口的所有引脚进行置1
            当pin = 0x0000; 返回值即为当前输出寄存器内的值
*******************************************************************/
u16 GPIOx_Set(GPIO_Port GPIOx, u16 pin)
{
    GPIO_TypeDef *p = (GPIO_TypeDef*)(GPIO_BASE + (GPIOx<<10)); //指向对应GPIO的基址地址

    p->ODR |= pin;

    return p->ODR;

}

/*******************************************************************
函 数 名：	void GPIOx_Rst(GPIO_Port GPIOx, u16 pin)
功能说明： 	复位引脚输出状态
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	无
注    意:   当pin = 0xffff; 时即对此端口的所有引脚进行置0
*******************************************************************/
void GPIOx_Rst(GPIO_Port GPIOx, u16 pin)
{
    GPIO_TypeDef *p = (GPIO_TypeDef*)(GPIO_BASE + (GPIOx<<10)); //指向对应GPIO的基址地址

    p->ODR &= ~pin;
}

/*******************************************************************
函 数 名：	void GPIOx_Rvrs(GPIO_Port GPIOx, u16 pin)
功能说明： 	取反引脚输出状态(reverse)
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	无
注    意:   当pin = 0xffff; 时即对此端口的所有引脚进行取反
*******************************************************************/
void GPIOx_Rvrs(GPIO_Port GPIOx, u16 pin)
{
    GPIO_TypeDef *p = (GPIO_TypeDef*)(GPIO_BASE + (GPIOx<<10)); //指向对应GPIO的基址地址

     p->ODR ^= pin;
}

/*******************************************************************
函 数 名：	void GPIOx_Set(GPIO_Port GPIOx)
功能说明： 	获取相应端口各引脚的输入状态值
参	  数： 	GPIOx: 对应的GPIO_(A~G)
			pin	 : 管脚pin_(0~15)
返 回 值：	一个16位整形数据，每一位对应各和的引脚
注    意: 	当pin = 0xffff; 返回所有引脚对应的状态值
*******************************************************************/
u16 GPIOx_Get(GPIO_Port GPIOx, u16 pin)
{
    GPIO_TypeDef *p = (GPIO_TypeDef*)(GPIO_BASE + (GPIOx<<10)); //指向对应GPIO的基址地址

    return (u16)(p->IDR&pin);
}

/*******************************************************************
函 数 名：	u32 GPIOx_Lock(GPIO_Port GPIOx, u16 pin)
功能说明： 	将对应端口引脚锁定与解锁
参	  数： 	GPIOx: 对应的GPIO_(A~G)
            pin	 : 管脚pin_(0~15)
            sta  :  LOCK/UNLOCK
返 回 值：	0/(!0) : 0 FAIL, !0 OK 
注    意:   当相应GPIO口的引脚锁定后，在下次上电复位前将不能在更改的引的配置
			此功能存在问题
*******************************************************************/
//u32 GPIOx_Lock(GPIO_Port GPIOx, u16 pin, INT8U sta)
//{
//    GPIO_TypeDef *p = (GPIO_TypeDef*)(GPIO_BASE + (GPIOx<<10)); //指向对应GPIO的基址地址
//
//    p->LCKR &= ~(1<<16); //锁定位置零
//
//    if (sta)
//    {
//        p->LCKR |= pin;
//    }
//    else
//    {
//        p->LCKR &= ~(pin);
//    }
//
//    /*GPIO锁激活时序*/
//    p->LCKR |= (1<<16);
//    p->LCKR &= ~(1<<16);
//    p->LCKR |= (1<<16);
//    if (!(p->LCKR & (1<<16)))
//    {
//        return (p->LCKR & (1<<16));
//    }
//    
//    return 0;
//}
/**************************FILE*END********************************/

/***************************Copyright BestFu 2014-05-14*************************
��	����	i2c.h
˵	����	I2Cģʽͨ������
��	�룺	uVision V5.18.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.05.04  
�ޡ��ģ�	2016.04.23 Modify by jay
*******************************************************************************/
#ifndef __I2C_H
#define __I2C_H

/*********************************�Զ����*************************************/
#define GPIO_OUT0(GPIOx, pin)  	GPIO_ResetBits(GPIOx, pin)	//GPIO����0
#define GPIO_OUT1(GPIOx, pin)  	GPIO_SetBits(GPIOx, pin)	//GPIO����1

#if (SCL_PORT == SCL_PORT)
#define	SCL_SDA_INIT			GPIO_Config(SDA_PORT,SDA_PIN | SCL_PIN,OUT_PU_40M);
#else
#define	SCL_SDA_INIT			GPIO_Config(SDA_PORT,SDA_PIN,OUT_PU_40M);\
								GPIO_Config(SCL_PORT,SCL_PIN,OUT_PU_40M)
#endif

#define SDA(f)        			GPIO_OUT##f(SCL_PORT, SDA_PIN)     
#define SCL(f)                  GPIO_OUT##f(SCL_PORT, SCL_PIN)


#define SDA_OUT                 GPIO_Config(SDA_PORT, SDA_PIN, OUT_PU_40M);\
								GPIO_SetBits(SDA_PORT, SDA_PIN)
								
#define SDA_IN                  GPIO_Config(SDA_PORT, SDA_PIN, IN_UP)

#define SDA_READ                GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN)

#ifdef	WP_PORT		//�������д�����˿�
	#define WP_INIT				GPIO_Config(WP_PORT, WP_PIN, OUT_PU_40M);\
								GPIO_SetBits(WP_PORT, WP_PIN)
									
	#define WP_UN				GPIO_ResetBits(WP_PORT, WP_PIN)
	#define WP_EN				GPIO_SetBits(WP_PORT, WP_PIN)
#else
	#define WP_INIT	
	#define WP_UN
	#define WP_EN
#endif

/*********************************��������*************************************/
void I2C_Init(void);
BF_BOOLEAN IIC_GetState(BF_INT08U sla, BF_INT32U overTime);
BF_BOOLEAN IIC_Read(BF_INT08U sla, BF_INT16U suba, BF_INT08U *data, BF_INT08U len);
BF_BOOLEAN IIC_Write(BF_INT08U sla, BF_INT16U suba, BF_INT08U *data, BF_INT08U len);
BF_BOOLEAN IIC_WriteSame(BF_INT08U sla, BF_INT16U suba, BF_INT08U data, BF_INT08U len);
BF_BOOLEAN IIC_GetState(BF_INT08U sla, BF_INT32U overTime);

#endif
/**************************Copyright BestFu 2014-05-14*************************/

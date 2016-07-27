/***************************Copyright BestFu 2014-05-14*************************
��	����	I2C.c
˵	����	I2Cģʽͨ������
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.05.04  
�ޡ��ģ�	����
*******************************************************************************/
#include "I2C.h"
#include "SysHard.h"

//Private macro define
#define WaitTime	(2)		//�ȴ�ʱ�� 
#define NOP			(5)		//����ʱ��

//Private function declaration
static void Delay_nNOP(u16 n);
static void I2C_Start(void);
static void  I2C_Stop(void);
static void  I2C_ACK(u8 ack);
static u8 I2C_WaitAck(void);
static u8 I2C_SendB_Ack(u8 data);
static u8 I2C_RecvB_Ack(u8 ack);

/*******************************************************************************
�� �� ����	Delay_nNOP
����˵���� 	��ʱn���ղ���
��	  ���� 	n-�ղ����ĸ���
�� �� ֵ��	void
*******************************************************************************/
static void Delay_nNOP(u16 n)
{
	u16 i = 0;

	for ( ; n > 0; n--)
	{
		for(i = NOP; i > 0; i--)
		{}
	}
}

/*******************************************************************************
�� �� ����	I2C_Init
����˵���� 	I2C�ĳ�ʼ��
��	  ���� 	void
�� �� ֵ��	void
*******************************************************************************/
void I2C_Init(void)
{
    SCL_SDA_OUT;
    SCL_SDA(1);
	WP_INIT;
}

/*******************************************************************************
�� �� ����	I2C_Start
����˵���� 	I2C�Ŀ�ʼ
��	  ���� 	void
�� �� ֵ��	void
*******************************************************************************/
static void I2C_Start(void)
{
    SDA(1);         
    SCL(1);         
    Delay_nNOP(WaitTime);
    SDA(0);
    Delay_nNOP(WaitTime);
    SCL(0);
}

/*******************************************************************************
�� �� ����	I2C_Stop
����˵���� 	I2C��ֹͣ
��	  ���� 	void
�� �� ֵ��	void
*******************************************************************************/
static void I2C_Stop(void)
{
    SDA(0);
    Delay_nNOP(WaitTime);
    SCL(1);
    Delay_nNOP(WaitTime);
    SDA(1);
    Delay_nNOP(WaitTime);
}

/*******************************************************************************
�� �� ����	I2C_ACK
����˵���� 	I2C������Ӧ
��	  ���� 	ack-��Ӧ��־��0��ʾ������ȡ��1��ʾ��ȡ����
�� �� ֵ��	void
*******************************************************************************/
static void I2C_ACK(u8 ack)
{
    SCL(0);
    Delay_nNOP(WaitTime);
    if (ack) 
    {
        SDA(1);
    }
    else 
    {
        SDA(0);
    }
    Delay_nNOP(WaitTime);
    SCL(1);
    Delay_nNOP(WaitTime);
    SCL(0);
}

/*******************************************************************************
�� �� ����	I2C_WaitAck
����˵���� 	I2C�ȴ���������Ӧ
��	  ���� 	void
�� �� ֵ��	u8-��Ӧ���ݣ�1��ʾ�л�Ӧ��0��ʾ��ʱ���޻�Ӧ
*******************************************************************************/
static u8 I2C_WaitAck(void)
{
	u16 OutTime=0;
	
	SDA_IN;       //SDA����Ϊ����  
	
    Delay_nNOP(WaitTime);	   
	SCL(1);
    Delay_nNOP(WaitTime); 
	while (SDA_READ)
	{
		if ((++OutTime) > 500)
		{
			SDA_OUT;
			return FALSE;
		}
	}
	SCL(0);
    Delay_nNOP(WaitTime); 
	SDA_OUT;
	Delay_nNOP(WaitTime);
	
	return TRUE;
}

/*******************************************************************************
�� �� ����	I2C_SendB_Ack
����˵���� 	I2C����һ���ֽ����ݲ��ҵȴ���Ӧ
��	  ���� 	data-��Ҫ���͵��ֽ�
�� �� ֵ��	u8-��Ӧ���ݣ�1��ʾ�л�Ӧ��0��ʾ��ʱ���޻�Ӧ
*******************************************************************************/
static u8 I2C_SendB_Ack(u8 data)
{
	u8 i;
	
	SCL(0);
	Delay_nNOP(WaitTime);
	for (i = 0; i < 8; i++)
	{
		if(data&0x80)
		  SDA(1);
		else
		  SDA(0);
		data <<= 1;
		Delay_nNOP(WaitTime);
		SCL(1);
		Delay_nNOP(WaitTime);
		SCL(0);
		Delay_nNOP(WaitTime);
	}

	return I2C_WaitAck();
}

/*******************************************************************************
�� �� ����	I2C_RecvB_Ack
����˵���� 	I2C����һ���ֽ����ݲ��һ�Ӧack
��	  ���� 	ack-��Ӧ��Ϣ��0��ʾ������ȡ��1-��ʾֹͣ��ȡ
�� �� ֵ��	u8-�յ�������
*******************************************************************************/
static u8 I2C_RecvB_Ack(u8 ack)
{
	u8 i,rcvdata=0;
	
	SDA_IN; 	  //SDA����Ϊ����  
	for (i = 0; i < 8; i++)
	{
		SCL(0);
		Delay_nNOP(WaitTime);
		SCL(1);
		Delay_nNOP(WaitTime);
		rcvdata <<= 1;

		if(SDA_READ)
		{
			rcvdata++;
		}
		Delay_nNOP(WaitTime);
	}
	SCL(0);
	Delay_nNOP(WaitTime);
	SDA_OUT;
	Delay_nNOP(WaitTime);
	I2C_ACK(ack);
	
	return (rcvdata);
}

/*******************************************************************************
�� �� ����  IIC_GetState
����˵����  IICͨ���豸״̬
��	  ����  add:	�豸��ַ
			overtime:�ȴ���ʱʱ��
  ����ֵ:  TRUE(��æ)/FALSE(æ)
*******************************************************************************/
u8 IIC_GetState(u8 sla, u32 overTime)
{
	do 
	{	
		I2C_Start();
	}while((!I2C_SendB_Ack(sla)) && --overTime);
	
	I2C_Stop();
	
	return (overTime ?  TRUE : FALSE);
}

/*******************************************************************************
�� �� ����	IIC_Read
����˵���� 	IIC���ݶ�ȡ
��	  ���� 	sla-��������ַ
            suba-�����׵�ַ
            data-��ȡ�������ݴ���ڸ�ָ��
            len-���ȣ���಻�ܳ���һҳ��С
�� �� ֵ��	1��ʾ�ɹ���0��ʾʧ��
*******************************************************************************/
u8 IIC_Read(u8 sla, u16 suba, u8 *data, u8 len)
{
	I2C_Start();
	if (I2C_SendB_Ack(sla))
	{	
		if (I2C_SendB_Ack((u8)(suba>>8)))
		{
			if (I2C_SendB_Ack((u8)(suba)))
			{
				I2C_Start();
				if (I2C_SendB_Ack(sla | 0x01))
				{
					for (; len > 1; len--)
					{
						*data++ = I2C_RecvB_Ack(0);
					}
					len--;
					*data = I2C_RecvB_Ack(1);
				}
			}
		}
	}
	I2C_Stop();
	
	return (len ? FALSE : TRUE);
}

/*******************************************************************************
�� �� ����	I2C_Write
����˵���� 	��һ������д�뵽EEPROM�е�ָ��λ��
��	  ���� 	sla-��������ַ
            suba-�����׵�ַ
            data-Ҫд������ݴ���ڸ�ָ��
            len-���ȣ���಻�ܳ���һҳ��С
�� �� ֵ��	1��ʾ�ɹ���0��ʾʧ��
*******************************************************************************/
u8 IIC_Write(u8 sla, u16 suba, u8 *data, u8 len)
{
	WP_UN;
	I2C_Start();
	if(I2C_SendB_Ack(sla))
	{
		if (I2C_SendB_Ack((u8)(suba>>8)))
		{
			if (I2C_SendB_Ack((u8)(suba)))
			{
				for (; len > 0; len--)
				{
					if (!I2C_SendB_Ack(*data++))
					{
						break;
					}
				}
			}
		}
    }
	I2C_Stop();
	WP_EN;
	
	return (len ? FALSE : TRUE);
}

/*******************************************************************************
�� �� ����	I2C_WriteSame
����˵���� 	��һ����ͬ�����ݣ�д�뵽EEPROM�е�ָ��λ��
��	  ���� 	sla-��������ַ
            suba-�����׵�ַ
            data-Ҫд�������
            len-���ȣ���಻�ܳ���һҳ��С
�� �� ֵ��	1��ʾ�ɹ���0��ʾʧ��
*******************************************************************************/
u8 IIC_WriteSame(u8 sla, u16 suba, u8 data, u8 len)
{
	WP_UN;
	I2C_Start();
	if (I2C_SendB_Ack(sla))
	{
		if (I2C_SendB_Ack((u8)(suba>>8)))
		{
			if (I2C_SendB_Ack((u8)(suba)))
			{
				for	(; len > 0; len--)
				{
					if (!I2C_SendB_Ack(data))
					{
						break;
					}
				}
			}
		}
    }
	I2C_Stop();
	WP_EN;
	
	return (len ? FALSE : TRUE);
}
/**************************Copyright BestFu 2014-05-14*************************/

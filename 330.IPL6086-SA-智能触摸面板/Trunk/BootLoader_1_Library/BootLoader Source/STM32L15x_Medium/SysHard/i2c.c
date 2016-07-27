/***************************Copyright BestFu 2014-05-14*************************
��	����	i2c.c
˵	����	I2Cģʽͨ������
��	�룺	uVision V5.18.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.05.04
�ޡ��ģ�	2016.04.23 Modify by jay
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "gpio.h"
#include "SysHard.h"
#include "i2c.h"

//Private macro define
#define WaitTime	(2)		//�ȴ�ʱ�� 
#define NOP			(5)		//����ʱ��

//Private function declaration
static void Delay_nNOP(BF_INT16U n);
static void I2C_Start(void);
static void  I2C_Stop(void);
static void  I2C_ACK(BF_INT08U ack);
static BF_BOOLEAN I2C_WaitAck(void);
static BF_BOOLEAN I2C_SendB_Ack(BF_INT08U data);
static BF_INT08U I2C_RecvB_Ack(BF_INT08U ack);

/*******************************************************************************
�� �� ����	Delay_nNOP
����˵���� 	��ʱn���ղ���
��	  ���� 	n-�ղ����ĸ���
�� �� ֵ��	void
*******************************************************************************/
static void Delay_nNOP(BF_INT16U n)
{
    BF_INT16U i = 0;

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
    SCL_SDA_INIT;
    SDA(1);
    SCL(1);
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
static void I2C_ACK(BF_INT08U ack)
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
�� �� ֵ��	BF_INT08U-��Ӧ���ݣ�1��ʾ�л�Ӧ��0��ʾ��ʱ���޻�Ӧ
*******************************************************************************/
static BF_BOOLEAN I2C_WaitAck(void)
{
    BF_INT16U OutTime=0;

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
�� �� ֵ��	BF_INT08U-��Ӧ���ݣ�1��ʾ�л�Ӧ��0��ʾ��ʱ���޻�Ӧ
*******************************************************************************/
static BF_BOOLEAN I2C_SendB_Ack(BF_INT08U data)
{
    BF_INT08U i;

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
�� �� ֵ��	BF_INT08U-�յ�������
*******************************************************************************/
static BF_INT08U I2C_RecvB_Ack(BF_INT08U ack)
{
    BF_INT08U i,rcvdata=0;

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
BF_BOOLEAN IIC_GetState(BF_INT08U sla, BF_INT32U overTime)
{
    do
    {
        I2C_Start();
    }
    while((!I2C_SendB_Ack(sla)) && --overTime);

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
BF_BOOLEAN IIC_Read(BF_INT08U sla, BF_INT16U suba, BF_INT08U *data, BF_INT08U len)
{
    I2C_Start();
    if (I2C_SendB_Ack(sla))
    {
        if (I2C_SendB_Ack((BF_INT08U)(suba>>8)))
        {
            if (I2C_SendB_Ack((BF_INT08U)(suba)))
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
BF_BOOLEAN IIC_Write(BF_INT08U sla, BF_INT16U suba, BF_INT08U *data, BF_INT08U len)
{
    WP_UN;
    I2C_Start();
    if(I2C_SendB_Ack(sla))
    {
        if (I2C_SendB_Ack((BF_INT08U)(suba>>8)))
        {
            if (I2C_SendB_Ack((BF_INT08U)(suba)))
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
BF_BOOLEAN IIC_WriteSame(BF_INT08U sla, BF_INT16U suba, BF_INT08U data, BF_INT08U len)
{
    WP_UN;
    I2C_Start();
    if (I2C_SendB_Ack(sla))
    {
        if (I2C_SendB_Ack((BF_INT08U)(suba>>8)))
        {
            if (I2C_SendB_Ack((BF_INT08U)(suba)))
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

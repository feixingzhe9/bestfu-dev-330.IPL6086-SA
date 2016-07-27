/***************************Copyright BestFu 2014-05-14*************************
��	����	Object.c
˵	����	Ŀ������ƥ����غ���
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.23
�ޡ��ģ�	����
*******************************************************************************/
#include "lib_type.h"
#include "fifo.h"
#include "PivotalDataType.h"
#include "UserData.h"
#include "object.h"


//public variable define
BF_INT32U DelayID;	//����ͨ��ƥ��

//Private variable define
static RepeatSt RData[REPEAT_MAX]= {0}; //���������ظ���ˮ�ţ�����ʱ���жϡ�2015.7.30

//Private function declaration
static BF_INT08U Check_Repeat(const CMDAim_t *aim);    //��Ϣ�ظ��˶�

/*******************************************************************************
�� �� ����	Object_Check
����˵���� 	Ŀ��˶�
��	  ���� 	aim:		��ϢĿ������
			*validUnit:	Ŀ�������Ч��Ԫ
�� �� ֵ��	0(��Ϣƥ��ɹ���/ 1(�û��Ų�ͬ��/2(ָ���ظ�)/3 (Ŀ�겻ƥ�䣩
            /4(����ƥ��)/5(��Ϣ����Խ��)
*******************************************************************************/
MsgResult_t Object_Check(BF_INT08U *sdata, BF_INT08U len, CMDAim_t *aim, CMDPara_t *para, BF_INT08U *data)
{
    if ( aim->userID == gSysData.userID				//У�������û���
            || aim->userID == SUPER_USERID			//У�鳬���û���
            || gSysData.userID == DEFAULT_USERID	//�û���ΪĬ��ֵ
       )
    {
        if (Check_Repeat(aim) == FALSE)  			//ָ���ظ�
        {
            return MSG_REPEAT;						//����ָ��������
        }
        if (aim->objectType == SINGLE_ACT)			//���ڵ����
        {
            if (aim->object.id != gSysData.deviceID)
            {
                return ID_ERR;						//����Ŀ��ƥ�����
            }
            DelayID = 0;							//����ͨ��ƥ�䣨Unarty 2014.08.07)
        }
        else if (aim->objectType == DEV_ALL_ACT && aim->object.id == 0)
        {
            DelayID = aim->sourceID;				//����ͨ��ƥ�䣨Unarty 2014.08.07)
        }
		else
		{
			return NO_CMD;
		}
		
		aim->object.id = aim->sourceID;      		//������ַ
        return COMPLETE;
    }

    return USER_ID_ERR;
}


/*******************************************************************************
�� �� ����	Check_Repeat
����˵���� 	�˶�ָ���Ƿ��ظ�
��	  ���� 	data:	Ҫ�˶Ե�ָ������
�� �� ֵ��	TRUE(�ظ�)/FALSE(���ظ�)
*******************************************************************************/
static BF_INT08U Check_Repeat(const CMDAim_t *aim)
{
    static BF_INT08U seat;
    BF_INT08U i,j;

    for (i = 0; i < REPEAT_MAX; i++)
    {
        if (RData[i].srcID == aim->sourceID)  	//����Դ��ַ���
        {
            for(j=0; j < 5; j++)
            {
                if(RData[i].serial[j] == aim->serialNum)
                {
                    return FALSE;	//�����Ѵ���
                }
            }

            RData[i].serialSeat = ++RData[i].serialSeat%5;   //��ˮ��ƫ��λ��
            RData[i].serial[RData[i].serialSeat] = aim->serialNum;	//��¼��ˮ��
            return TRUE;
        }
    }
    //ԭ��ַ����ͬ
    seat = ++seat%REPEAT_MAX; //ԭ��ַƫ��λ��
    RData[seat].serial[0] = aim->serialNum; //��ˮ��
    RData[seat].srcID     = aim->sourceID;  //ԭ��ַ

    return TRUE;
}
/**************************Copyright BestFu 2014-05-14*************************/

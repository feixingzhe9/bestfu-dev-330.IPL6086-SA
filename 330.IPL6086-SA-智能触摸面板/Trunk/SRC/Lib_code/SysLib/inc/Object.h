/***************************Copyright BestFu 2014-05-14*************************
��	����	Object.h
˵	����	Ŀ������ƥ��ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.23 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __OBJECT_H
#define __OBJECT_H

#include "command.h"
#include "UnitCfg.h"

#define AREA_ALL    (0xFFFFFFFF)    //ȫ����
#define AREA_1      (0x00FFFFFF)    //����ȼ�һ
#define AREA_2      (0x0000FFFF)    //����ȼ���
#define AERA_3      (0x000000FF)    //����ȼ���
#define AERA_4      (0x00000000)    //����ȼ���
#define AREA_MAX	(5)				//���������
#define AREA_VALUE  (0xFF)          //��������Ĭ��ֵ

extern u32 DelayID;		//����ͨ��ƥ�䣨Unarty 2014.08.07)

//�����޸��ظ���ˮ�ţ�����ʱ���жϡ�2015.7.30
#define REPEAT_MAX (20)       //��¼10�����ݵ���ˮ��
#define REPEAT_OVERT (10000)  //��ʱʱ��Ϊ10��
typedef struct 
{
	u32 srcID;//�ж��ظ�Դ��ַ
	u32 time;//��ʱʱ��
	u8  serial[5];//��ˮ��
	u8  serialSeat;//��ˮ��λ��
}RepeatSt;

//void ClearRepeatSerialNum(void);//����ˮ�ź���
u8 Check_BandAddr(CMDAim_t *aim);//���aim���������Ƿ��а�ת������
MsgResult_t Object_Check(u8 *sdata, u8 len, CMDAim_t *aim, CMDPara_t *para, u8 *data);
#if (COMMUNICATION_SHORT_FRAME == 1)
	MsgResult_t ShortObject_Check(ShortCommunication_t *pMsg, u8 len); // yanhuan adding 2015/12/29
#endif
u8 Check_Area(const u8 *area, u32 *validUnit);           //����˶�

#endif //Object.h end
/**************************Copyright BestFu 2014-05-14*************************/

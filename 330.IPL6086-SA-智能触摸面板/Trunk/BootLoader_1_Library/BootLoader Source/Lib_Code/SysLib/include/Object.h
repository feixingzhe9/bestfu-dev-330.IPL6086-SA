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

/**********************************�����Զ����*******************************/
#define AREA_ALL    (0xFFFFFFFF)    //ȫ����
#define AREA_1      (0x00FFFFFF)    //����ȼ�һ
#define AREA_2      (0x0000FFFF)    //����ȼ���
#define AERA_3      (0x000000FF)    //����ȼ���
#define AERA_4      (0x00000000)    //����ȼ���
#define AREA_MAX	(5)				//���������
#define AREA_VALUE  (0xFF)          //��������Ĭ��ֵ

/********************************���ݽṹ����**********************************/
//�����޸��ظ���ˮ�ţ�����ʱ���жϡ�2015.7.30
#define REPEAT_MAX (20)       //��¼10�����ݵ���ˮ��
#define REPEAT_OVERT (10000)  //��ʱʱ��Ϊ10��
typedef struct 
{
	BF_INT32U  srcID;//�ж��ظ�Դ��ַ
	BF_INT32U  time;//��ʱʱ��
	BF_INT08U  serial[5];//��ˮ��
	BF_INT08U  serialSeat;//��ˮ��λ��
}RepeatSt;

/************************************��������**********************************/
extern BF_INT32U DelayID;		//����ͨ��ƥ�䣨Unarty 2014.08.07)

/**********************************��������***********************************/
MsgResult_t Object_Check(BF_INT08U *sdata, BF_INT08U len, CMDAim_t *aim, CMDPara_t *para, BF_INT08U *data);

#endif //Object.h end
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu ***********************************
**  ��    ����  Sign433.h
**  ��    �ܣ�  433�ź�ǿ�Ȳ���
**  ��    �룺  Keil uVision5 V4.72
**  ��    ����  V1.1
**  ��    д��  Seven
**  �������ڣ�  2014.10.08
**  �޸����ڣ�  2014.10.22
**  ˵    ����  >>
*******************************************************************************/
#ifndef _SIGN433_H_
#define _SIGN433_H_

#include "BF_type.H"
#define MAX_FRAME_NUM   100       //����������֡��Ŀ  100֡
typedef struct
{
    u8  Number;      //���԰�����
    u8  Length;      //���԰�����
    u16 TimeSpace;   //���԰�ʱ����(ms)
}TestPkgInfo_t;      //433�źŲ�����Ϣͷ

typedef struct
{
    u32  MasterID;                  //������ַ
    u32  SlaveID;                   //�ӻ���ַ
    TestPkgInfo_t TestPkgInfo;      //433�źŲ�����Ϣͷ
}Sign433Comm_t;                     //433�źŲ���ͨѶ�ṹ��


extern Sign433Comm_t gSign433Comm;
extern u8   RecRSSI[100];           //ÿ֡�ź�ǿ��ֵ
extern u8   Sign433_State;          // FLASE ����   TURE ������

extern void Sign433_SendTestFrame(void);
extern void Sign433_SendTestFrameStream(void);
extern void ClearRecTestStatus(void);
extern void Sign433_SendTestReport(void);
//������ӣ�����RSSI��������
extern void SetRecTestStatus(u8 flag);
extern u8 *GetRSSIPara(void);
//�ƻ���ӣ��豸ͨ���ż�ֵ��ȡ
extern u8  GetThresholdPara(void);

#endif

/***************************Copyright BestFu **********************************/ 

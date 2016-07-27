/*******************************************************************************
�� ����    queue.h
˵ ����    ��������ͷ�ļ�
�� �룺    MDK-ARM Professional Version: 5.12.0.0
�� ����    v1.0
�� д��    Jay
�� �ڣ�    2015.07.04
�ޡ��ģ�   ����
*******************************************************************************/
#include "BF_type.h"
#include "key.h"

#define MAX_SIZE  (7)               /*���еĴ�С*/

/******************************************************************************/
//typedef struct                    /*����״̬�ṹ��*/
// {
//    unsigned char KeyValue;
//    unsigned char KeyType;
// }KeySta_t,*pKeySta_t;


typedef struct                    /*�������*/
 {
    KeySta_t KeySta[MAX_SIZE];
    u8       front;          /*��ͷ*/
    u8       rear;           /*��β*/
 }Queue_t,*pQueue_t;

/*******************************************************************************/
void InitQueue(pQueue_t pQ);
void TravelQueue(pQueue_t pQ);
u8 Enqueue(pQueue_t pQ, KeySta_t KeySt);
u8 Dequeue(pQueue_t pQ, pKeySta_t pKeySt);
/*******************************************************************************/

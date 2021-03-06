/*******************************************************************************
文 件：    queue.h
说 明：    按键队列头文件
编 译：    MDK-ARM Professional Version: 5.12.0.0
版 本：    v1.0
编 写：    Jay
日 期：    2015.07.04
修　改：   暂无
*******************************************************************************/
#include "BF_type.h"
#include "key.h"

#define MAX_SIZE  (7)               /*队列的大小*/

/******************************************************************************/
//typedef struct                    /*按键状态结构体*/
// {
//    unsigned char KeyValue;
//    unsigned char KeyType;
// }KeySta_t,*pKeySta_t;


typedef struct                    /*定义队列*/
 {
    KeySta_t KeySta[MAX_SIZE];
    u8       front;          /*队头*/
    u8       rear;           /*队尾*/
 }Queue_t,*pQueue_t;

/*******************************************************************************/
void InitQueue(pQueue_t pQ);
void TravelQueue(pQueue_t pQ);
u8 Enqueue(pQueue_t pQ, KeySta_t KeySt);
u8 Dequeue(pQueue_t pQ, pKeySta_t pKeySt);
/*******************************************************************************/

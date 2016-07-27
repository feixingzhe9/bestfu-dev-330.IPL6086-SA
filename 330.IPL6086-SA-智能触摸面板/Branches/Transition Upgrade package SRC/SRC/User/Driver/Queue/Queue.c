/*******************************************************************************
文 件：    queue.c
说 明：    按键队列源文件
编 译：    MDK-ARM Professional Version: 5.12.0.0
版 本：    v1.0
编 写：    Jay
日 期：    2015.07.04
修　改：   暂无
*******************************************************************************/
#include "Queue.h"

static u8 IsFull(pQueue_t pQ);
static u8 IsEmpty(pQueue_t pQ);
/*******************************************************************************
 - 函 数 名:   void InitQueue(pQueue_t pQ)
 - 功能说明:   初始化队列
 - 参    数:   pQueue:队列的首地址
 - 返 回 值:   无
 - 说    明：  无
*******************************************************************************/
void InitQueue(pQueue_t pQ)
{
    u8 count;
    for(count=0;count<MAX_SIZE;count++)
    {
        pQ->KeySta[count].KeyType = KEY_NONE;
        pQ->KeySta[count].KeyValue= 0xff;
    }
    pQ->rear     = 0;
    pQ->front    = 0;
}

 /*******************************************************************************
 - 函 数 名:   u8 IsFull(pQueue_t pQ)
 - 功能说明:   判断队列是否已满
 - 参    数:   pQueue:队列的首地址
 - 返 回 值:   1：队列已满；0：队列未满
 - 说    明：  无
*******************************************************************************/
static u8 IsFull(pQueue_t pQ)
{
    if(((pQ->rear+1) % MAX_SIZE) == (pQ->front))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

 /*******************************************************************************
 - 函 数 名:   static u8 IsEmpty(pQueue_t pQ)
 - 功能说明:   判断队列是否为空
 - 参    数:   pQueue:队列的首地址
 - 返 回 值:   1：队列为空；0：队列非空
 - 说    明：  无
*******************************************************************************/
static u8 IsEmpty(pQueue_t pQ)
{
    if(pQ->rear == pQ->front)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
#if 0
/*******************************************************************************
 - 函 数 名:   void TravelQueue(pQueue_t pQ)
 - 功能说明:   遍历整个队列
 - 参    数:   pQueue:队列的首地址
 - 返 回 值:   无
 - 说    明：  无
*******************************************************************************/
void TravelQueue(pQueue_t pQ)
{
    u8 i;
    i = pQ->front;
    while(i != pQ->rear)
    {
        printf("elements KeyType is %d \t",pQ->KeySta[i].KeyType);
        printf("elements KeyValue is %d \r\n",pQ->KeySta[i].KeyValue);
        i = (i + 1) % MAX_SIZE;
    }
    printf("\r\n----------------------------------------------------------------\r\n");
}
#endif
/*******************************************************************************
 - 函 数 名:   u8 Enqueue(pQueue_t pQ, KeySta_t KeySt)
 - 功能说明:   入队
 - 参    数:   pQueue:队列的首地址；KeySt：要入队的值
 - 返 回 值:   1:队列正常，0：表示入队溢出
 - 说    明：  无
*******************************************************************************/
u8 Enqueue(pQueue_t pQ, KeySta_t KeySt)
{
    if(IsFull(pQ))    /**<如果队列已满，则不断覆盖 */
    {
        pQ->front = (pQ->front + 1) % MAX_SIZE;
        pQ->KeySta[pQ->rear].KeyValue = KeySt.KeyValue;
        pQ->KeySta[pQ->rear].KeyType  = KeySt.KeyType;
        pQ->rear = (pQ->rear + 1) % MAX_SIZE;
        return 0;
    }
    else
    {
        pQ->KeySta[pQ->rear].KeyValue = KeySt.KeyValue;
        pQ->KeySta[pQ->rear].KeyType  = KeySt.KeyType;
        pQ->rear = (pQ->rear + 1) % MAX_SIZE;
        return 1;
    }
}

/*******************************************************************************
 - 函 数 名:   u8 Dequeue(pQueue_t pQ, pKeySta_t pKeySt)
 - 功能说明:   出队
 - 参    数:   pQueue:队列的首地址；KeySt：要入队值的指针
 - 返 回 值:   1:出队成功，0：表示出队失败
 - 说    明：  无
*******************************************************************************/
u8 Dequeue(pQueue_t pQ, pKeySta_t pKeySt)
{
    if(IsEmpty(pQ))
    {
        return 0;
    }
    else
    {
        pKeySt->KeyType = pQ->KeySta[pQ->front].KeyType;
        pKeySt->KeyValue = pQ->KeySta[pQ->front].KeyValue;
        pQ->front = (pQ->front + 1) % MAX_SIZE;
        return 1;
    }
}

/******************************************************************************/


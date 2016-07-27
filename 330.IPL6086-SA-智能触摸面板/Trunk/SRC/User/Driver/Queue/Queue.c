/*******************************************************************************
�� ����    queue.c
˵ ����    ��������Դ�ļ�
�� �룺    MDK-ARM Professional Version: 5.12.0.0
�� ����    v1.0
�� д��    Jay
�� �ڣ�    2015.07.04
�ޡ��ģ�   ����
*******************************************************************************/
#include "Queue.h"
#include "usart.h"

static u8 IsFull(pQueue_t pQ);
static u8 IsEmpty(pQueue_t pQ);
/*******************************************************************************
 - �� �� ��:   void InitQueue(pQueue_t pQ)
 - ����˵��:   ��ʼ������
 - ��    ��:   pQueue:���е��׵�ַ
 - �� �� ֵ:   ��
 - ˵    ����  ��
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
 - �� �� ��:   u8 IsFull(pQueue_t pQ)
 - ����˵��:   �ж϶����Ƿ�����
 - ��    ��:   pQueue:���е��׵�ַ
 - �� �� ֵ:   1������������0������δ��
 - ˵    ����  ��
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
 - �� �� ��:   static u8 IsEmpty(pQueue_t pQ)
 - ����˵��:   �ж϶����Ƿ�Ϊ��
 - ��    ��:   pQueue:���е��׵�ַ
 - �� �� ֵ:   1������Ϊ�գ�0�����зǿ�
 - ˵    ����  ��
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

/*******************************************************************************
 - �� �� ��:   u8 Enqueue(pQueue_t pQ, KeySta_t KeySt)
 - ����˵��:   ���
 - ��    ��:   pQueue:���е��׵�ַ��KeySt��Ҫ��ӵ�ֵ
 - �� �� ֵ:   1:����������0����ʾ������
 - ˵    ����  ��
*******************************************************************************/
u8 Enqueue(pQueue_t pQ, KeyCurStatus_t KeySt)
{
    if(IsFull(pQ))    /**<��������������򲻶ϸ��� */
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
 - �� �� ��:   u8 Dequeue(pQueue_t pQ, pKeySta_t pKeySt)
 - ����˵��:   ����
 - ��    ��:   pQueue:���е��׵�ַ��KeySt��Ҫ���ֵ��ָ��
 - �� �� ֵ:   1:���ӳɹ���0����ʾ����ʧ��
 - ˵    ����  ��
*******************************************************************************/
u8 Dequeue(pQueue_t pQ, KeyCurStatus_t* pKeySt)
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


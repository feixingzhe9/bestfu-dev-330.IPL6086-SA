/***************************Copyright BestFu 2016-04-13*************************
��	����	Fifo.c
˵	����	ͨ�ö��д���ԭ����
��	�룺	uVision V5.12.0
��	����	v1.1
��	д��	Unarty
��	�ڣ�	2016-04-13
��  ��:     jay: 2016-04-13���޸�
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "BestFuLib.h"
#include "fifo.h"

//Private function declaration
static BF_INT32U fifo_surplusSize(FIFO_t *head); 	//����ʣ��ռ��ж�
static BF_BOOLEAN fifo_full(FIFO_t *head);          //�����ж�
/*******************************************************************************
�� �� ����  fifo_Init
����˵����  ���г�ʼ��
��	  ����  *head:  ����ͷ
            *data:  ���ݴ洢�׵�ַ
            len:    ���ݴ洢���򳤶�
�� �� ֵ��  ��ʼ����� TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN fifo_Init(FIFO_t *head, BF_INT08U *buf, BF_INT32U len)
{
    ERRR(head == NULL, return FALSE);
    head->data = buf;
    head->size = len;
    head->front = head->rear = 0;

    return TRUE;
}

/*******************************************************************************
�� �� ����  fifo_Clr
����˵����  �������
��	  ����  *head:  ����ͷ
�� �� ֵ��  ��
*******************************************************************************/
void fifo_Rst(FIFO_t *head)
{
    ERRR(head == NULL, return);
    head->front = 0;
    head->rear = 0;
}
/*******************************************************************************
�� �� ����  fifo_empty
����˵����  �ж϶����Ƿ�Ϊ��
��	  ����  *head:  ����ͷ
�� �� ֵ��  TRUE(����Ϊ��)/FALSE
*******************************************************************************/
BF_INT32U fifo_empty(FIFO_t *head)
{

    return ((head->front == head->rear) ? TRUE : FALSE);
}

/*******************************************************************************
�� �� ����  fifo_full
����˵����  �ж϶����Ƿ�����
��	  ����  *head:  ����ͷ
�� �� ֵ��  TRUE(��������)/FALSE
*******************************************************************************/
static BF_BOOLEAN fifo_full(FIFO_t *head)
{
    return ((head->front == ((head->rear+1)%head->size)) ? TRUE : FALSE);
}

/*******************************************************************************
�� �� ����  fifo_surplusSize
����˵����  �ж϶���ʣ��ռ��С
��	  ����  *head:  ����ͷ
�� �� ֵ��  ʣ��ռ��С(�ַ�Ϊ��λ)
*******************************************************************************/
static BF_INT32U fifo_surplusSize(FIFO_t *head)
{
    return ((head->front > head->rear)
            ? (head->front - head->rear - 1)
            : (head->size + head->front - head->rear - 1));
}

/*******************************************************************************
�� �� ����  fifo_validSize
����˵����  ��ѯ������Ч�ռ��С
��	  ����  *head:  ����ͷ
�� �� ֵ��  ������Ч���ݳ���(�ַ�Ϊ��λ)
*******************************************************************************/
BF_INT32U fifo_validSize(FIFO_t *head)
{

    return ((head->rear < head->front)
            ? (head->rear + head->size - head->front)
            : (head->rear - head->front));
}

/*******************************************************************************
�� �� ����  kfifo_puts
����˵����  ���
��	  ����  *head:  ����ͷ
            *data:  �������
            len:    ���ݳ���
�� �� ֵ��  ��ӽ�� TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN fifo_puts(FIFO_t *head, BF_INT08U *data, BF_INT32U len)
{
    BF_INT32U size;

    ERRR(head == NULL, return FALSE);
    ERRR(len > fifo_surplusSize(head), return FALSE); //�ж϶������Ƿ��д洢�ռ�

    size = MIN(len, head->size - head->rear);
    memcpy(head->data + head->rear, data, size);
    memcpy(head->data, data + size, len - size);

    head->rear = (head->rear + len)%head->size;

    return TRUE;
}

/*******************************************************************************
�� �� ����  kfifo_gets
����˵����  ����
��	  ����  *head:  ����ͷ
            *data:  ��������
            len:    �������ݳ���
�� �� ֵ��  ���ӽ�� TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN fifo_gets(FIFO_t *head, BF_INT08U *data, BF_INT32U len)
{
    BF_INT32U size;

    ERRR(head == NULL, return FALSE);
    ERRR(fifo_empty(head) == TRUE, return FALSE); //����Ϊ��
    ERRR(len > fifo_validSize(head), return FALSE); //�洢����С��Ҫ��ȡ����

    size = MIN(len, head->size - head->front);
    memcpy(data, head->data + head->front, size);
    memcpy(data+size, head->data, len - size);

    head->front = (head->front + len)%head->size;

    return TRUE;
}

/*******************************************************************************
�� �� ����  fifo_putc
����˵����  ���һ���ַ�
��	  ����  *head:  ����ͷ
            data:   Ҫ��ӵ�����
�� �� ֵ��  ��ӽ�� TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN fifo_putc(FIFO_t *head, BF_INT08U data)
{
    ERRR(head == NULL, return FALSE);
    ERRR(fifo_full(head) == TRUE, return FALSE); //�ж϶������Ƿ��д洢�ռ�

    head->data[head->rear] = data;

    head->rear = (++head->rear)%head->size;

    return TRUE;
}

/*******************************************************************************
�� �� ����  kfifo_getc
����˵����  ����һ���ַ�
��	  ����  *head:  ����ͷ
            data:  ��������
�� �� ֵ��  ���ӽ�� TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN fifo_getc(FIFO_t *head, BF_INT08U *data)
{
    ERRR(head == NULL, return FALSE);
    ERRR(fifo_empty(head) == TRUE, return FALSE); //����Ϊ��

    *data = head->data[head->front];
    head->front = (++head->front)%head->size;

    return TRUE;
}

/*******************************************************************************
�� �� ����  fifo_Find
����˵����  �������ݲ���
��	  ����  *head:  ����ͷ
            *data:  ����ƥ������
            len:    ƥ�����ݳ���
�� �� ֵ��  ����ƥ��ָ�룬���ҵ���λ��
*******************************************************************************/
BF_INT32U fifo_find(FIFO_t *head, const BF_INT08U *data, BF_INT32U len)
{
    BF_INT32U i, n;

    ERRR((NULL == head)||(NULL == data)||(0 == len), return 0);

    for (i = head->front, n = 0; i != head->rear; i = (++i)%head->size)
    {
        if (head->data[i] == data[n])  									//�����������
        {
            if (++n == len)  											//ƥ�䳤�����
            {
                n--;
                break;
            }
        }
        else
        {
            n = 0;
        }
    }

    return ((i+head->size - n)%head->size);
}

/*******************************************************************************
�� �� ����  fifo_cmp
����˵����  �������ݱȽ�
��	  ����  fifo:	�Ƚ�Դ
            seat:   λ��fifo�Ŀ�ʼ�Ƚ�λ��
			cmp��	���Ƚ���
			cmpsize:�Ƚϳ���
�� �� ֵ��  TRUE/FALSE
*******************************************************************************/
BF_BOOLEAN fifo_cmp(const FIFO_t *fifo, BF_INT32U seat, const BF_INT08U *cmp, BF_INT32U cmpsize)
{
    BF_INT32U i;

    ERRR((fifo->data == NULL)||(cmp == NULL), return FALSE);

    for (i = 0; i < cmpsize; i++)  	//�������в���
    {
        if (fifo->data[(seat+i)%fifo->size] != *cmp++)
        {
            return FALSE;
        }
    }

    return TRUE;
}


/**************************Copyright BestFu 2014-05-14*************************/


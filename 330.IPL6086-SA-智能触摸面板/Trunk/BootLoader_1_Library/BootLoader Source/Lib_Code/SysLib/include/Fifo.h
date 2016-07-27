/***************************Copyright BestFu 2016-04-13*************************
��	����	Fifo.h
˵	����	ͨ�ö��д���ͷ�ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-09-03
��  ��:     jay: 2016-04-13���޸�
*******************************************************************************/

#ifndef __FIFO_H
#define __FIFO_H

/********************************���ݽṹ����**********************************/
typedef struct{
    BF_INT08U   *data;
    BF_INT32U   size;
    BF_INT32U   front;
    BF_INT32U   rear;
}FIFO_t;

/**********************************��������***********************************/                         
BF_BOOLEAN fifo_Init(FIFO_t *head, BF_INT08U *buf, BF_INT32U len);//���г�ʼ��
void fifo_Rst(FIFO_t *head); //��ն���
BF_INT32U fifo_empty(FIFO_t *head); //�ӿ��ж�
BF_INT32U fifo_validSize(FIFO_t *head);//���ݿ��ÿռ��С
BF_BOOLEAN fifo_puts(FIFO_t *head, BF_INT08U *data, BF_INT32U len);
BF_BOOLEAN fifo_gets(FIFO_t *head, BF_INT08U *data, BF_INT32U len);
BF_BOOLEAN fifo_putc(FIFO_t *head, BF_INT08U data);
BF_BOOLEAN fifo_getc(FIFO_t *head, BF_INT08U *data);
BF_INT32U fifo_find(FIFO_t *head, const BF_INT08U *data, BF_INT32U len);
BF_BOOLEAN fifo_cmp(const FIFO_t *fifo, BF_INT32U seat, const BF_INT08U *cmp, BF_INT32U cmpsize);
							
#endif //queue.h end
/**************************Copyright BestFu 2016-04-13*************************/

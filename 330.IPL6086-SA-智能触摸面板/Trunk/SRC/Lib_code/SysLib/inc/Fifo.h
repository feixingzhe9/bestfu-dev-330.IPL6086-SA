/***************************Copyright BestFu 2014-05-14*************************
��	����	Fifo.h
˵	����	ͨ�ö��д���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2013-09-03
��  ��:     ����
*******************************************************************************/

#ifndef __FIFO_H
#define __FIFO_H

#include "BF_type.h"

typedef struct{
    u8   *data;
    u32   size;
    u32   front;
    u32   rear;
}FIFO_t;

/*�ݴ��*/
#define ERRR(conf, ret)      do              \
                            {                \
                                if (conf)    \
                                {            \
                                    ret;     \
                                }            \
                            } while(0) 
                            
#define MIN(a, b)           ((a) > (b) ? (b) : (a))


u8 fifo_Init(FIFO_t *head, u8 *buf, u32 len); //���г�ʼ��
void fifo_Rst(FIFO_t *head);                        //��ն���
u32 fifo_validSize(FIFO_t *head);     //���ݿ��ÿռ��С
u8 fifo_empty(FIFO_t *head);        //�ӿ��ж�
u8 fifo_puts(FIFO_t *head, u8 *data, u32 len);
u8 fifo_gets(FIFO_t *head, u8 *data, u32 len);
u8 fifo_putc(FIFO_t *head, u8 data);
u8 fifo_getc(FIFO_t *head, u8 *data);
u32 fifo_find(FIFO_t *head, const u8 *data, u32 len);
u8 fifo_cmp(const FIFO_t *fifo, u32 seat, const u8 *cmp, u32 cmpsize);
							
#endif //queue.h end
/**************************Copyright BestFu 2014-05-14*************************/

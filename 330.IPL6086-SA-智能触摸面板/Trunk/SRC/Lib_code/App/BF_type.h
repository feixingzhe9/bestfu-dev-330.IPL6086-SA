/***************************Copyright BestFu 2014-05-14*************************
��	����	BF_type.h
˵	����	����ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Joey
��	�ڣ�	2013.8.9 
��  ��:     Unary (2014.08.15, ɾ���˴󲿷ֶ��壬ֻ�����������)
*******************************************************************************/
#ifndef _BF_TYPE_H_
#define _BF_TYPE_H_

#include "BestFuLib.h"


#define TRUE                      		(1)
#define FALSE                     		(0)

#define NULL                      		((void*)(0))
#define CLEAR                       	(0xFF)

#define offsetof(TYPE, MEMBER) 			((u32) &((TYPE *)0)->MEMBER)                //���ڻ�ȡ�ṹ���г�Ա��ƫ����
#define BF_DIM(array_) 					(sizeof(array_) / sizeof((array_)[0]))		//���������е�Ԫ�ظ���
#define NUM_ROUND(a,b)					(((a)/(b))+ ((0==((a)%(b)))?(0):(1)))		//�������ݰ��ĸ���

typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;
typedef signed long    s32;
typedef signed short   s16;
typedef signed char    s8;

typedef volatile unsigned int   vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned int  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

#endif
/**************************Copyright BestFu 2014-05-14*************************/

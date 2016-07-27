#ifndef   LIB_TYPE_H
#define   LIB_TYPE_H

#include "cpu.h"

/**********************************�����Զ����*******************************/
/*���ݴ��*/
#define ERRR(conf, ret)      			do{\
											if (conf){\
												ret;\
											}\
										} while(0) 
							
#define ABS(i, j)						(((i) > (j)) ? ((i) - (j)) : ((j) - (i))) 	//��ȡ����ֵ
#define MIN(a, b)           			((a) > (b) ? (b) : (a))						//ȡ����������Сֵ
#define TRUE                      		((BF_BOOLEAN)(1))
#define FALSE                     		((BF_BOOLEAN)(0))
#define NULL                      		((void*)(0))

#define offsetof(TYPE, MEMBER) 			((unsigned int) &((TYPE *)0)->MEMBER)		//���ڻ�ȡ�ṹ���г�Ա��ƫ����
#define BF_DIM(array_) 					(sizeof(array_) / sizeof((array_)[0]))		//���������е�Ԫ�ظ���

#define NUM_ROUND(a,b)					(((a)/(b))+ ((0==((a)%(b)))?(0):(1)))		//�������ݰ��ĸ���
#define ROUND(a,b)						(((a)/(b))*(b))								//a��bȡ��

/**********************************�Զ������ݽṹ*****************************/
typedef	  CPU_CHAR			BF_CHAR;				/*  8-bit character */
typedef   CPU_INT08S		BF_INT08S;              /*  8-bit character */
typedef   CPU_BOOLEAN       BF_BOOLEAN;				/*  8-bit boolean or logical */
typedef   CPU_INT08U        BF_INT08U;				/*  8-bit unsigned integer  */

typedef   CPU_INT16U		BF_INT16U;				/* 16-bit unsigned integer */
typedef   CPU_INT16S		BF_INT16S;				/* 16-bit   signed integer  */
typedef   CPU_INT32U        BF_INT32U;				/* 32-bit unsigned integer */
typedef   CPU_INT32S        BF_INT32S;				/* 32-bit   signed integer  */
typedef   CPU_INT64U		BF_INT64U;              /* 64-bit   usigned integer  */
typedef   CPU_INT64S		BF_INT64S;              /* 64-bit   usigned integer  */

#define  A_ATTR                                     /*rom para 1*/
#define  B_ATTR                                     /*rom para 2*/

typedef   void(*BF_FNCT_VOID)(void);				/* See Note #2a.             */
typedef   unsigned char(*BF_FNCT_PTR)(void);		/* See Note #2b.             */


#endif
/***************************************************End of File*****************************************************/

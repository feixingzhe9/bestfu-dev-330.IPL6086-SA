/***************************Copyright BestFu 2014-05-14*************************
��	����	BestFulib.h
˵	����	BestFu�Խ�Lib��ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014-05-30
*******************************************************************************/
#ifndef __BESTFULIB_H
#define __BESTFULIB_H

/**********************************�����Զ����*******************************/

/**********************************��������************************************/
void memcpy(void *dest,  const void *src, BF_INT32U num);
BF_BOOLEAN memcmp(void *dest,  const void *src, BF_INT32U num);
void memset(void *dest,const BF_INT08U val, BF_INT32U num);
void memmove(void *dest,  const void *src, BF_INT32U num);
void* memfind(void *dest, BF_INT32U dLen, void *src, BF_INT32U sLen);

#endif //BestFuLib.h end
/**************************Copyright BestFu 2014-05-14*************************/
							

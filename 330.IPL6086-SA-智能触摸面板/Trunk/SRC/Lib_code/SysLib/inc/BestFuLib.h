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

/*�ݴ��*/
#define ERRR(conf, ret)      do              \
                            {                \
                                if (conf)    \
                                {            \
                                    ret;     \
                                }            \
                            } while(0) 
							
#define ABS(i, j)		(((i) > (j)) ? ((i) - (j)) : ((j) - (i))) 		//��ȡ����ֵ

                                
void memcpy(void *dest, const void *src, unsigned int num); //�ڴ濼��
char memcmp(void *dest,  const void *src, unsigned int num);	//�ڴ�Ƚ�
void memmove(void *dest,  const void *src, unsigned int num);	//�ڴ��ƶ�
void memset(void *dest, unsigned char const val, unsigned int num);	//�ڴ����� 
void* memfind(void *dest, unsigned int dLen, void *src, unsigned int sLen);							
void memcharget(void *dest, unsigned char *len, void const *src, unsigned int size);

#endif //BestFuLib.h end
/**************************Copyright BestFu 2014-05-14*************************/
							

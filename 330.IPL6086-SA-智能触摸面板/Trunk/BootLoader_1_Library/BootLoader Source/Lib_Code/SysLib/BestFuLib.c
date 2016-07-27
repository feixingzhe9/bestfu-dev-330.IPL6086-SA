/***************************Copyright BestFu 2014-05-14*************************
��	����	BestFulib.c
˵	����	BestFu�Խ�Lib���ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014-05-30
��  ��:     2014-09-27 Unarty Add memfind && memcharget
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "BestFuLib.h"

/*******************************************************************************
�� �� ����	memcpy
����˵���� 	�ڴ濼��
��	  ���� 	dest:	Ŀ���ڴ��ַ
			src:	Դ�ڴ��ַ
			num:	�����ڴ��С
�� �� ֵ��	��
*******************************************************************************/
void memcpy(void *dest,  const void *src, BF_INT32U num)
{
    BF_CHAR *pd, *ps;

    if (dest != src)  	//Ŀ����Դ����ͬһλ��
    {
        for (pd = (BF_CHAR*)dest, ps = (BF_CHAR*)src; num > 0; num--)
        {
            *pd++ = *ps++;
        }
    }
}

/*******************************************************************************
�� �� ����	memcmp
����˵���� 	�ڴ�Ƚ�
��	  ���� 	dest:	Ŀ���ڴ��ַ
			src:	Դ�ڴ��ַ
			num:	�Ƚ��ڴ��С
�� �� ֵ��	TRUE(��ͬ)/FALSE
*******************************************************************************/
BF_BOOLEAN memcmp(void *dest,  const void *src, BF_INT32U num)
{
    BF_CHAR *pd, *ps;

    if (dest != src) 	//Ŀ����Դ����ͬһλ��
    {
        for (pd = (BF_CHAR*)dest, ps = (BF_CHAR*)src; num > 0; num--)
        {
            if (*pd++ != *ps++)
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

/*******************************************************************************
�� �� ����	memset
����˵���� 	�ڴ�����
��	  ���� 	dest:	Ŀ���ڴ��ַ
			val:	Ŀ��ֵ
			num:	�Ƚ��ڴ��С
�� �� ֵ��	��
*******************************************************************************/
void memset(void *dest,const BF_INT08U val, BF_INT32U num)
{
    BF_CHAR *pd;

    for (pd = (BF_CHAR*)dest; num > 0; num--)
    {
        *pd++ = val;
    }
}

/*******************************************************************************
�� �� ����	memmove
����˵���� 	�ڴ��ƶ�
��	  ���� 	dest:	Ŀ���ڴ��ַ
			src:	Դ�ڴ��ַ
			num:	�ƶ��ڴ��С
�� �� ֵ��	��
*******************************************************************************/
void memmove(void *dest,  const void *src, BF_INT32U num)
{
    BF_CHAR *pd, *ps;

    if (dest < src)
    {
        for (pd = (BF_CHAR*)dest, ps = (BF_CHAR*)src; num > 0; num--)
        {
            *pd++ = *ps++;
        }
    }
    else if (dest > src)
    {
        for (pd = (BF_CHAR*)((BF_INT32U)dest + num),\
                ps = (BF_CHAR*)((BF_INT32U)src + num); num > 0; num--)
        {
            *(--pd) = *(--ps);
        }
    }
}

/*******************************************************************************
�� �� ��:  	memfind
����˵��:  	�ڴ����
��    ��:  	dest:	����Ŀ��
			dLen��	Ŀ����Ч����
			src:	����Դ
			sLen:	Դ���ݴ�С
�� �� ֵ:  	NULL(δ�ҵ�)/Ŀ��λ��Դ�ĵ�ַ
*******************************************************************************/
void* memfind(void *dest, BF_INT32U dLen, void *src, BF_INT32U sLen)
{
    BF_CHAR *pDest, *pSrc;
    BF_INT32U i;

    for (pDest = dest, pSrc = src; sLen > 0; sLen--, pSrc++)
    {
        if (*pDest == *pSrc++) 									//Դ���ҵ���Ŀ����ͬ����
        {
            for (i = 1; i < dLen && pDest[i] == *pSrc++; i++);	//�ȽϺ��������Ƿ����
            if (i == dLen) 										//Դ���ҵ�����Ŀ����ȫ��ȵ�����Ƭ
            {
                return (pSrc - i);
            }
        }
    }
    return (NULL);
}
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2016-02-23*************************
��	����    ImportFilter.h
˵	����    �ź������˲�ͷ�ļ�
��	�룺    Keil uVision5 V5.12
��	����    V1.0
��	д��    Jay
��	�ڣ�    2016-02-23
�ޡ��ģ�	��
*******************************************************************************/
#ifndef __IMPORT_FILTER_H
#define __IMPORT_FILTER_H

/************************************�������********************************/
#define SIMPLE_TIME_IMPORT_FILTER_EN		(0u)	//���ü�ʱ���˲���(1��ʾ����,0��ʾͣ��)	
#define SIMPLE_TIME_NUM_IMPORT_FILTER_EN	(0u)	//���ü򵥲�������ʱ���˲���(1��ʾ����,0��ʾͣ��)
#define COMPLEX_TIME_NUM_IMPORT_FILTER_EN	(1u)	//���ø��Ӳ�������ʱ���˲���(1��ʾ����,0��ʾͣ��)

/************************************�궨��************************************/
#define UNVALID_IMPORT_VALUE 		(0xFFFFFFFF)	//��Ч����ֵ

/************************************�������Ͷ���******************************/
typedef void(*void_func_ptr)(void);					//����Ƶ������ʱִ����ʾ���ܵĺ���ָ��

#if (SIMPLE_TIME_IMPORT_FILTER_EN > 0u)
typedef struct										//�����ʱ���˲��������ݽṹ
{
	unsigned int	 	 import_travel_cycle;		//�������ı�������	 
	unsigned char  		 import_num;				//����Ŀ��ĸ���	
	unsigned int	 	 import_space_threshold;	//��������������������Ƶ�ʻ������ֵ
	unsigned int* 		 import_valid_space_ptr;	//���������ͬ����Դ��ʱ�����������ָ��
	unsigned int*  		 import_map;				//�����������������׵�ַ
}SimpleTimeImportFilter_t,*pSimpleTimeImportFilter_t;
#endif

#if (SIMPLE_TIME_NUM_IMPORT_FILTER_EN > 0u)
typedef struct										//����򵥲�������ʱ���˲��������ݽṹ
{
	unsigned int	 	 import_travel_cycle;		//�������ı������� 
	unsigned int	 	 import_space_threshold;	//��������������������Ƶ�ʻ������ֵ
	unsigned char  		 import_count_threshold;	//����������Ч�Ĳ�������
	unsigned int   		 import_all_space_threshold;//���ó���������������ֵʱ������ʱ����
	unsigned int   		 import_valid_keep_time;	//��������������ʱЧ��
	unsigned int 		 import_valid_space_count;	//���������ͬ����Դ��ʱ�����������ָ��
	unsigned int  		 import_unvalid_count;		//�ﵽ����Ĳ��������󲻿��õ�ʱ����
	unsigned char 		 import_valid_count;		//����������Ч�����Ĵ���
	unsigned char 		 import_unvalid_flag;		//���벻��ʹ�ñ��	
}SimpleTimeNumImportFilter_t,*pSimpleTimeNumImportFilter_t;
#endif

#if (COMPLEX_TIME_NUM_IMPORT_FILTER_EN > 0u)
typedef struct										//���帴�Ӳ�������ʱ���˲��������ݽṹ
{
	unsigned int	 	 import_travel_cycle;		//�������ı�������
	unsigned char  		 import_num;				//����Ŀ��ĸ���	 
	unsigned int	 	 import_space_threshold;	//��������������������Ƶ�ʻ������ֵ
	unsigned char  		 import_count_threshold;	//����������Ч�Ĳ�������
	unsigned int   		 import_all_space_threshold;//���ó���������������ֵʱ������ʱ����
	unsigned int   		 import_valid_keep_time;	//��������������ʱЧ��
	unsigned int* 		 import_valid_space_ptr;	//���������ͬ����Դ��ʱ�����������ָ��
	unsigned int*  		 import_map;				//�����������������׵�ַ
	unsigned int  		 import_unvalid_count;		//�ﵽ����Ĳ��������󲻿��õ�ʱ����
	unsigned char 		 import_valid_count;		//����������Ч�����Ĵ���
	unsigned char 		 import_unvalid_flag;		//���벻��ʹ�ñ��	
}ComplexTimeNumImportFilter_t,*pComplexTimeNumImportFilter_t;
#endif

/************************************��������**********************************/
#if (SIMPLE_TIME_IMPORT_FILTER_EN > 0u)
unsigned int SimpleTimeImportFilter(SimpleTimeImportFilter_t* pImportFilter,unsigned int value);
#endif

#if (SIMPLE_TIME_NUM_IMPORT_FILTER_EN > 0u)
unsigned int SimpleTimeNumImportFilter(SimpleTimeNumImportFilter_t* pImportFilter,unsigned int value,void_func_ptr exe_fun1_ptr,void_func_ptr exe_fun2_ptr);
#endif

#if (COMPLEX_TIME_NUM_IMPORT_FILTER_EN > 0u)
unsigned int ComplexTimeNumImportFilter(ComplexTimeNumImportFilter_t* pImportFilter,unsigned int value,void_func_ptr exe_fun1_ptr,void_func_ptr exe_fun2_ptr);
#endif

#endif 
/**************************Copyright BestFu 2016-02-23*************************/

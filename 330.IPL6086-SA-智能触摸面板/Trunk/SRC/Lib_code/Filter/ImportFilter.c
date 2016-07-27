/***************************Copyright BestFu 2016-02-23*************************
��	����    Import_Filter.c
˵	����    �ź������˲�
��	�룺    Keil uVision5 V5.12
��	����    V1.0
��	д��    Jay
��	�ڣ�    2016-02-23
�ޡ��ģ�	��
*******************************************************************************/
#include "ImportFilter.h"

//Pravite function declaration
#if ((SIMPLE_TIME_IMPORT_FILTER_EN > 0u)||(COMPLEX_TIME_NUM_IMPORT_FILTER_EN > 0u))
static unsigned char Find_ArrayLabal(unsigned int* Arrary_Addr,unsigned char arrary_element_num,unsigned int value);
#endif

#if (SIMPLE_TIME_IMPORT_FILTER_EN > 0u)
static void SimpleTimeFilterTime_Handle(SimpleTimeImportFilter_t* pImportFilter);
#endif

#if (SIMPLE_TIME_NUM_IMPORT_FILTER_EN > 0u)
static void SimpleTimeNumFilterTime_Handle(SimpleTimeNumImportFilter_t* pImportFilter);
#endif

#if (COMPLEX_TIME_NUM_IMPORT_FILTER_EN > 0u)
static void ComplexTimeNumFilterTime_Handle(ComplexTimeNumImportFilter_t* pImportFilter);
#endif

#if (SIMPLE_TIME_IMPORT_FILTER_EN > 0u)
/*******************************************************************************
�� �� ����	unsigned int SimpleTimeImportFilter(SimpleTimeImportFilter_t* pImportFilter,unsigned int value)
����˵���� 	��ʱ���˲�
��	  ���� 	pImportFilter	:��ʱ���˲�
			value			:��������ֵ
�� �� ֵ��	����0xffffffff��ʾ��Ч������,����ֵ��ʾ��Ч
˵    ��:	value��ֵ����Ϊ0xffffffff
*******************************************************************************/
unsigned int SimpleTimeImportFilter(SimpleTimeImportFilter_t* pImportFilter,unsigned int value)
{
	unsigned int return_result = UNVALID_IMPORT_VALUE;
	unsigned char label = 0xff;
	SimpleTimeFilterTime_Handle(pImportFilter);											//���¼�����
	label = Find_ArrayLabal(pImportFilter->import_map,pImportFilter->import_num,value);
	if(pImportFilter->import_valid_space_ptr[label] >\
			(pImportFilter->import_space_threshold/pImportFilter->import_travel_cycle))	//�жϵ�ǰ����ֵ�Ƿ���Ч 
	{
		return_result = value;
		pImportFilter->import_valid_space_ptr[label] = 0u;
	}
	return (return_result);
}

/*******************************************************************************
�� �� ����	static void SimpleTimeFilterTime_Handle(SimpleTimeImportFilter_t* pImportFilter)
����˵���� 	���˲�ʱ�䴦����
��	  ���� 	pImportFilter	:���˲�������ָ��
�� �� ֵ��	��
˵    ��:	��
*******************************************************************************/
static void SimpleTimeFilterTime_Handle(SimpleTimeImportFilter_t* pImportFilter)
{
	unsigned char cnt = 0u;
	for(cnt = 0u;cnt < pImportFilter->import_num;cnt ++)
	{
		pImportFilter->import_valid_space_ptr[cnt] ++;
		/*****��ֹ��ʱ��Խ��,���ĳЩʱ�򰴼��ȵ������豸�������������	******/
		if(pImportFilter->import_valid_space_ptr[cnt] > (2*pImportFilter->import_space_threshold/pImportFilter->import_travel_cycle))	
		{											
			pImportFilter->import_valid_space_ptr[cnt] = (2*pImportFilter->import_space_threshold/pImportFilter->import_travel_cycle);
		}
	}
}
#endif

#if (SIMPLE_TIME_NUM_IMPORT_FILTER_EN > 0u)
/*******************************************************************************
�� �� ����	unsigned int SimpleTimeNumImportFilter(SimpleTimeNumImportFilter_t* pImportFilter,unsigned int value,void_func_ptr exe_fun_ptr)
����˵���� 	���Ӳ�������ʱ���˲���
��	  ���� 	pImportFilter	:��ʱ���˲�
			value			:��������ֵ
			exe_fun1_ptr	:���뷱æ��ʾִ�к���ָ��
			exe_fun2_ptr	:���뷱����ææ��ʾִ�к���ָ��
�� �� ֵ��	����0xffffffff��ʾ��Ч������,����ֵ��ʾ��Ч
˵    ��:	value��ֵ����Ϊ0xffffffff
*******************************************************************************/
unsigned int SimpleTimeNumImportFilter(SimpleTimeNumImportFilter_t* pImportFilter,unsigned int value,void_func_ptr exe_fun1_ptr,void_func_ptr exe_fun2_ptr)
{
	unsigned int return_result = UNVALID_IMPORT_VALUE;
	
	if(!pImportFilter->import_unvalid_flag)
	{
		SimpleTimeNumFilterTime_Handle(pImportFilter);
		if(UNVALID_IMPORT_VALUE == value)
		{
			return UNVALID_IMPORT_VALUE;
		}
		if(pImportFilter->import_valid_space_count >\
				(pImportFilter->import_space_threshold/pImportFilter->import_travel_cycle))	//�жϵ�ǰ����ֵ�Ƿ���Ч 
		{
			pImportFilter->import_valid_space_count = 0u;
			pImportFilter->import_valid_count ++;
			if(pImportFilter->import_valid_count > pImportFilter->import_count_threshold)
			{
				pImportFilter->import_valid_count  = 0u;
				pImportFilter->import_unvalid_flag = 1u;
				pImportFilter->import_unvalid_count= 0u;	
				if((void*)0 != exe_fun1_ptr)
				{
					exe_fun1_ptr();
				}
			}
			else
			{
				return_result = value;
			}
		}
	}
	else
	{
		pImportFilter->import_unvalid_count ++;
		if(pImportFilter->import_unvalid_count > (pImportFilter->import_all_space_threshold/pImportFilter->import_travel_cycle))
		{
			pImportFilter->import_unvalid_flag  = 0u;
			pImportFilter->import_unvalid_count = 0u;
			if((void*)0 != exe_fun2_ptr)
			{
				exe_fun2_ptr();
			}
		}
	}
	return (return_result);
}

/*******************************************************************************
�� �� ����	static void SimpleTimeNumFilterTime_Handle(ComplexTimeNumImportFilter_t* pImportFilter)
����˵���� 	�˲�ʱ�䴦����
��	  ���� 	pImportFilter	:�˲�������ָ��
�� �� ֵ��	��
˵    ��:	��
*******************************************************************************/
static void SimpleTimeNumFilterTime_Handle(SimpleTimeNumImportFilter_t* pImportFilter)
{	
	pImportFilter->import_valid_space_count ++;
	if(pImportFilter->import_valid_space_count > (pImportFilter->import_valid_keep_time/pImportFilter->import_travel_cycle))
	{
		pImportFilter->import_valid_space_count = (pImportFilter->import_valid_keep_time/pImportFilter->import_travel_cycle);
		pImportFilter->import_valid_count = 0u;
	}
}
#endif

#if (COMPLEX_TIME_NUM_IMPORT_FILTER_EN > 0u)
/*******************************************************************************
�� �� ����	unsigned int ComplexTimeNumImportFilter(ComplexTimeNumImportFilter_t* pImportFilter,unsigned int value,void_func_ptr exe_fun_ptr)
����˵���� 	���Ӳ�������ʱ���˲���
��	  ���� 	pImportFilter	:��ʱ���˲�
			value			:��������ֵ
			exe_fun1_ptr	:���뷱æ��ʾִ�к���ָ��
			exe_fun2_ptr	:���뷱����ææ��ʾִ�к���ָ��
�� �� ֵ��	����0xffffffff��ʾ��Ч������,����ֵ��ʾ��Ч
˵    ��:	value��ֵ����Ϊ0xffffffff
*******************************************************************************/
unsigned int ComplexTimeNumImportFilter(ComplexTimeNumImportFilter_t* pImportFilter,unsigned int value,void_func_ptr exe_fun1_ptr,void_func_ptr exe_fun2_ptr)
{
	unsigned int return_result = UNVALID_IMPORT_VALUE;
	unsigned char label = 0xff;
	
	if(!pImportFilter->import_unvalid_flag)
	{
		ComplexTimeNumFilterTime_Handle(pImportFilter);										//���¼�����
		label = Find_ArrayLabal(pImportFilter->import_map,pImportFilter->import_num,value);
		if(label == pImportFilter->import_num)
		{
			return UNVALID_IMPORT_VALUE;
		}
		if(pImportFilter->import_valid_space_ptr[label] >\
				(pImportFilter->import_space_threshold/pImportFilter->import_travel_cycle))
		{
			pImportFilter->import_valid_space_ptr[label] = 0u;
			pImportFilter->import_valid_count ++;
			if(pImportFilter->import_valid_count > pImportFilter->import_count_threshold)	//�жϵ�ǰ����ֵ�Ƿ���Ч 
			{
				pImportFilter->import_valid_count  = 0u;
				pImportFilter->import_unvalid_flag = 1u;
				pImportFilter->import_unvalid_count= 0u;	
				if((void*)0 != exe_fun1_ptr)
				{
					exe_fun1_ptr();
				}
			}
			else
			{
				return_result = value;
			}
		}
	}
	else
	{
		pImportFilter->import_unvalid_count ++;
		if(pImportFilter->import_unvalid_count > (pImportFilter->import_all_space_threshold/pImportFilter->import_travel_cycle))
		{
			pImportFilter->import_unvalid_flag  = 0u;
			pImportFilter->import_unvalid_count = 0u;
			if((void*)0 != exe_fun2_ptr)
			{
				exe_fun2_ptr();
			}
		}
	}
	return (return_result);
}

/*******************************************************************************
�� �� ����	static void ComplexTimeNumFilterTime_Handle(ComplexTimeNumImportFilter_t* pImportFilter)
����˵���� 	�˲�ʱ�䴦����
��	  ���� 	pImportFilter	:�˲�������ָ��
�� �� ֵ��	��
˵    ��:	��
*******************************************************************************/
static void ComplexTimeNumFilterTime_Handle(ComplexTimeNumImportFilter_t* pImportFilter)
{
	unsigned char cnt = 0u,count = 0u;
	for(cnt = 0u;cnt < pImportFilter->import_num;cnt ++)
	{
		pImportFilter->import_valid_space_ptr[cnt] ++;
		/*****��ֹ��ʱ��Խ��,���ĳЩʱ�򰴼��ȵ������豸�������������	******/
		if(pImportFilter->import_valid_space_ptr[cnt] > (pImportFilter->import_valid_keep_time/pImportFilter->import_travel_cycle))	
		{
			count ++;											
			pImportFilter->import_valid_space_ptr[cnt] = (pImportFilter->import_valid_keep_time/pImportFilter->import_travel_cycle);
			if(count < pImportFilter->import_num)
			{
				continue;
			}
			pImportFilter->import_valid_count = 0u;
		}
	}
}
#endif

#if ((SIMPLE_TIME_IMPORT_FILTER_EN > 0u)||(COMPLEX_TIME_NUM_IMPORT_FILTER_EN > 0u))
/*******************************************************************************
�� �� ����	static unsigned char Find_ArrayLabal(unsigned int* Arrary_Addr,unsigned int arrary_element_num,unsigned int value)
����˵���� 	���ҵ�ǰֵ�������е��±�λ��
��	  ���� 	Arrary_Addr			:������׵�ַ
			arrary_element_num	:����Ԫ�صĸ���
			value				:����ѯ��ֵ
�� �� ֵ��	��
˵    ��:	��
*******************************************************************************/
static unsigned char Find_ArrayLabal(unsigned int* Arrary_Addr,unsigned char arrary_element_num,unsigned int value)
{
	unsigned char i = 0u;
	for(i = 0;i < arrary_element_num;i ++)
	{
		if(value == *(Arrary_Addr + i))
		{
			break;
		}
	}
	return (i);
}
#endif

/**************************Copyright BestFu 2016-02-23*************************/

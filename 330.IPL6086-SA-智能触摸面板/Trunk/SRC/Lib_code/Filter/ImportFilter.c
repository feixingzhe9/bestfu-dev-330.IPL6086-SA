/***************************Copyright BestFu 2016-02-23*************************
文	件：    Import_Filter.c
说	明：    信号输入滤波
编	译：    Keil uVision5 V5.12
版	本：    V1.0
编	写：    Jay
日	期：    2016-02-23
修　改：	无
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
函 数 名：	unsigned int SimpleTimeImportFilter(SimpleTimeImportFilter_t* pImportFilter,unsigned int value)
功能说明： 	简单时间滤波
参	  数： 	pImportFilter	:简单时间滤波
			value			:输入量的值
返 回 值：	返回0xffffffff表示无效输入量,其他值表示有效
说    明:	value的值不可为0xffffffff
*******************************************************************************/
unsigned int SimpleTimeImportFilter(SimpleTimeImportFilter_t* pImportFilter,unsigned int value)
{
	unsigned int return_result = UNVALID_IMPORT_VALUE;
	unsigned char label = 0xff;
	SimpleTimeFilterTime_Handle(pImportFilter);											//更新计数器
	label = Find_ArrayLabal(pImportFilter->import_map,pImportFilter->import_num,value);
	if(pImportFilter->import_valid_space_ptr[label] >\
			(pImportFilter->import_space_threshold/pImportFilter->import_travel_cycle))	//判断当前计数值是否有效 
	{
		return_result = value;
		pImportFilter->import_valid_space_ptr[label] = 0u;
	}
	return (return_result);
}

/*******************************************************************************
函 数 名：	static void SimpleTimeFilterTime_Handle(SimpleTimeImportFilter_t* pImportFilter)
功能说明： 	简单滤波时间处理函数
参	  数： 	pImportFilter	:简单滤波处理函数指针
返 回 值：	无
说    明:	无
*******************************************************************************/
static void SimpleTimeFilterTime_Handle(SimpleTimeImportFilter_t* pImportFilter)
{
	unsigned char cnt = 0u;
	for(cnt = 0u;cnt < pImportFilter->import_num;cnt ++)
	{
		pImportFilter->import_valid_space_ptr[cnt] ++;
		/*****防止定时器越界,造成某些时候按键等等输入设备操作不灵的问题	******/
		if(pImportFilter->import_valid_space_ptr[cnt] > (2*pImportFilter->import_space_threshold/pImportFilter->import_travel_cycle))	
		{											
			pImportFilter->import_valid_space_ptr[cnt] = (2*pImportFilter->import_space_threshold/pImportFilter->import_travel_cycle);
		}
	}
}
#endif

#if (SIMPLE_TIME_NUM_IMPORT_FILTER_EN > 0u)
/*******************************************************************************
函 数 名：	unsigned int SimpleTimeNumImportFilter(SimpleTimeNumImportFilter_t* pImportFilter,unsigned int value,void_func_ptr exe_fun_ptr)
功能说明： 	复杂操作数、时间滤波器
参	  数： 	pImportFilter	:简单时间滤波
			value			:输入量的值
			exe_fun1_ptr	:输入繁忙提示执行函数指针
			exe_fun2_ptr	:输入繁到不忙忙提示执行函数指针
返 回 值：	返回0xffffffff表示无效输入量,其他值表示有效
说    明:	value的值不可为0xffffffff
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
				(pImportFilter->import_space_threshold/pImportFilter->import_travel_cycle))	//判断当前计数值是否有效 
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
函 数 名：	static void SimpleTimeNumFilterTime_Handle(ComplexTimeNumImportFilter_t* pImportFilter)
功能说明： 	滤波时间处理函数
参	  数： 	pImportFilter	:滤波处理函数指针
返 回 值：	无
说    明:	无
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
函 数 名：	unsigned int ComplexTimeNumImportFilter(ComplexTimeNumImportFilter_t* pImportFilter,unsigned int value,void_func_ptr exe_fun_ptr)
功能说明： 	复杂操作数、时间滤波器
参	  数： 	pImportFilter	:简单时间滤波
			value			:输入量的值
			exe_fun1_ptr	:输入繁忙提示执行函数指针
			exe_fun2_ptr	:输入繁到不忙忙提示执行函数指针
返 回 值：	返回0xffffffff表示无效输入量,其他值表示有效
说    明:	value的值不可为0xffffffff
*******************************************************************************/
unsigned int ComplexTimeNumImportFilter(ComplexTimeNumImportFilter_t* pImportFilter,unsigned int value,void_func_ptr exe_fun1_ptr,void_func_ptr exe_fun2_ptr)
{
	unsigned int return_result = UNVALID_IMPORT_VALUE;
	unsigned char label = 0xff;
	
	if(!pImportFilter->import_unvalid_flag)
	{
		ComplexTimeNumFilterTime_Handle(pImportFilter);										//更新计数器
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
			if(pImportFilter->import_valid_count > pImportFilter->import_count_threshold)	//判断当前计数值是否有效 
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
函 数 名：	static void ComplexTimeNumFilterTime_Handle(ComplexTimeNumImportFilter_t* pImportFilter)
功能说明： 	滤波时间处理函数
参	  数： 	pImportFilter	:滤波处理函数指针
返 回 值：	无
说    明:	无
*******************************************************************************/
static void ComplexTimeNumFilterTime_Handle(ComplexTimeNumImportFilter_t* pImportFilter)
{
	unsigned char cnt = 0u,count = 0u;
	for(cnt = 0u;cnt < pImportFilter->import_num;cnt ++)
	{
		pImportFilter->import_valid_space_ptr[cnt] ++;
		/*****防止定时器越界,造成某些时候按键等等输入设备操作不灵的问题	******/
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
函 数 名：	static unsigned char Find_ArrayLabal(unsigned int* Arrary_Addr,unsigned int arrary_element_num,unsigned int value)
功能说明： 	查找当前值在数组中的下表位置
参	  数： 	Arrary_Addr			:数组的首地址
			arrary_element_num	:数组元素的个数
			value				:待查询的值
返 回 值：	无
说    明:	无
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

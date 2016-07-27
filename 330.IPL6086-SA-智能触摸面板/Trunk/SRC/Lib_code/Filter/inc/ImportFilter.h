/***************************Copyright BestFu 2016-02-23*************************
文	件：    ImportFilter.h
说	明：    信号输入滤波头文件
编	译：    Keil uVision5 V5.12
版	本：    V1.0
编	写：    Jay
日	期：    2016-02-23
修　改：	无
*******************************************************************************/
#ifndef __IMPORT_FILTER_H
#define __IMPORT_FILTER_H

/************************************配置项定义********************************/
#define SIMPLE_TIME_IMPORT_FILTER_EN		(0u)	//配置简单时间滤波器(1表示启用,0表示停用)	
#define SIMPLE_TIME_NUM_IMPORT_FILTER_EN	(0u)	//配置简单操作数、时间滤波器(1表示启用,0表示停用)
#define COMPLEX_TIME_NUM_IMPORT_FILTER_EN	(1u)	//配置复杂操作数、时间滤波器(1表示启用,0表示停用)

/************************************宏定义************************************/
#define UNVALID_IMPORT_VALUE 		(0xFFFFFFFF)	//无效输入值

/************************************数据类型定义******************************/
typedef void(*void_func_ptr)(void);					//定义频繁操作时执行提示功能的函数指针

#if (SIMPLE_TIME_IMPORT_FILTER_EN > 0u)
typedef struct										//定义简单时间滤波器的数据结构
{
	unsigned int	 	 import_travel_cycle;		//输入量的遍历周期	 
	unsigned char  		 import_num;				//输入目标的个数	
	unsigned int	 	 import_space_threshold;	//配置相邻两次输入量的频率或次数阈值
	unsigned int* 		 import_valid_space_ptr;	//允许出现相同输入源的时间间隔或次数的指针
	unsigned int*  		 import_map;				//输入量的穷举数组表首地址
}SimpleTimeImportFilter_t,*pSimpleTimeImportFilter_t;
#endif

#if (SIMPLE_TIME_NUM_IMPORT_FILTER_EN > 0u)
typedef struct										//定义简单操作数、时间滤波器的数据结构
{
	unsigned int	 	 import_travel_cycle;		//输入量的遍历周期 
	unsigned int	 	 import_space_threshold;	//配置相邻两次输入量的频率或次数阈值
	unsigned char  		 import_count_threshold;	//配置连续有效的操作次数
	unsigned int   		 import_all_space_threshold;//配置超过连续操作的阈值时不可用时间间隔
	unsigned int   		 import_valid_keep_time;	//配置连续操作的时效性
	unsigned int 		 import_valid_space_count;	//允许出现相同输入源的时间间隔或次数的指针
	unsigned int  		 import_unvalid_count;		//达到允许的操作次数后不可用的时间间隔
	unsigned char 		 import_valid_count;		//允许连续有效操作的次数
	unsigned char 		 import_unvalid_flag;		//输入不可使用标记	
}SimpleTimeNumImportFilter_t,*pSimpleTimeNumImportFilter_t;
#endif

#if (COMPLEX_TIME_NUM_IMPORT_FILTER_EN > 0u)
typedef struct										//定义复杂操作数、时间滤波器的数据结构
{
	unsigned int	 	 import_travel_cycle;		//输入量的遍历周期
	unsigned char  		 import_num;				//输入目标的个数	 
	unsigned int	 	 import_space_threshold;	//配置相邻两次输入量的频率或次数阈值
	unsigned char  		 import_count_threshold;	//配置连续有效的操作次数
	unsigned int   		 import_all_space_threshold;//配置超过连续操作的阈值时不可用时间间隔
	unsigned int   		 import_valid_keep_time;	//配置连续操作的时效性
	unsigned int* 		 import_valid_space_ptr;	//允许出现相同输入源的时间间隔或次数的指针
	unsigned int*  		 import_map;				//输入量的穷举数组表首地址
	unsigned int  		 import_unvalid_count;		//达到允许的操作次数后不可用的时间间隔
	unsigned char 		 import_valid_count;		//允许连续有效操作的次数
	unsigned char 		 import_unvalid_flag;		//输入不可使用标记	
}ComplexTimeNumImportFilter_t,*pComplexTimeNumImportFilter_t;
#endif

/************************************函数声明**********************************/
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

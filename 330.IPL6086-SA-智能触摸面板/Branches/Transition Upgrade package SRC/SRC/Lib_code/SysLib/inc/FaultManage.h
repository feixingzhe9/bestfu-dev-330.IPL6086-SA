/***************************Copyright BestFu 2014-05-14*************************
文	件：	FaultManage.h
说	明：	异常处理头文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.06.30  
修  改:     暂无
*******************************************************************************/
#ifndef __FAULTMANAGE_H
#define __FAULTMANAGE_H

#include "BF_type.h"

typedef enum
{
	FAULT_0,	//异常等级最高, 设置无法正常使用
	FAULT_1,	//通过重置可以恢复
	FAULT_2,	//通过fun函数指向的内容可以复位
}FaultGrade_t;	//异常等级

typedef enum
{
	FIFO_OVER,		//队列溢出
	EEPROM_W_ERR,	//EEPROM写错误
	EEPROM_R_ERR,	//EEPROM读错误
	SI4432_TX_ERR,	//无线数据发送错误
	SI4432_RX_ERR,	//无线数据接收错误
	SI4432_CRC_ERR,	//无线数据硬件校验错误
}Fault_t;

void Fault_Upload(FaultGrade_t grade, Fault_t event, void *fun);

#endif		//FaultManage.h end
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2014-05-14*************************
文	件：	Upload.c
说	明：	设备主动上报相关函数
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Unarty
日	期：	2014.07.03 
修　改：	暂无
*******************************************************************************/
#include "Upload.h"
#include "MsgPackage.h"
#include "UnitShare_Attr.h"
#include "Thread.h"
#include "SysTick.h"

static u8 Fault;

static void Upload_Attr(void);
static void FaultMsg(void);

/*******************************************************************************
函 数 名：	Upload_Fault
功能说明： 	主动异常上报
参	  数： 	upload: 异常情况具体见异常枚举歌词
返 回 值：	无
*******************************************************************************/
void Upload_Fault(Upload_t upload)
{
	u32 time;
	
	Fault = (u8)upload;
	if (upload == USER_DATA_RESET)
	{
		time = 1000;
	}
	else
	{
		time = 5;
	}
	
	Thread_Login(ONCEDELAY, 0, time, &FaultMsg);
}

/*******************************************************************************
函 数 名：	FaultMsg
功能说明： 	异常消息封装
参	  数： 	upload: 异常情况具体见异常枚举歌词
返 回 值：	无
*******************************************************************************/
void FaultMsg(void)
{
	Msg_Upload(0, 0xFF, 1, (u8*)&Fault);
}

/*******************************************************************************
函 数 名：	Upload
功能说明： 	设备属性状态上报
参	  数： 	无
返 回 值：	无
注    意:	根据单元代码编写者的注册，确定要上报的内容
*******************************************************************************/
void Upload(void)
{
	u32 time = 2500 + (Time_Get()%300)*10;
	Thread_Login(ONCEDELAY, 0, time, &Upload_Attr);
}

/*******************************************************************************
函 数 名：	Upload_Attr
功能说明： 	设备属性状态上报
参	  数： 	无
返 回 值：	无
*******************************************************************************/
void Upload_Attr(void)
{
	u8 data[200];
	
	(*(UnitPara_t*)&data).len = 0; //数据长度等于零
	data[3] = Get_ManyUnit_Attr((UnitPara_t*)data, &data[2], &data[4]);
	data[2]++; //数据长度加1（通信成功与否标识）
	Msg_UploadUnit(0, 0x00, data[2], &data[3]);	
}

/**************************Copyright BestFu 2014-05-14*************************/

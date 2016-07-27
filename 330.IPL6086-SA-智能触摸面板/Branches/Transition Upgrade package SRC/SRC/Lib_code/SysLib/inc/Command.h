/***************************Copyright BestFu 2014-05-14*************************
文	件：    command.h
说	明:	    从队列中获取指令头文件
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-10-14
修  改:     暂无
*******************************************************************************/
#ifndef __COMMAND_H
#define __COMMAND_H

#include "BF_type.h"
#include "fifo.h"

#define CRC_SIZE	(0x02)			//CRC数据占用空间，单位：byte
#define CMD_SIZE	(0xFF)			//一串指令的最长长度
#define HEAD_SIZE	(6)				//头数据长度
#define TAIL_SIZE	(6)				//尾数据长度
#define ARGE_SEAT	(1)				//参数据位置

#define SENDBUFSIZE (1900)          /**> 定义发送定长缓冲区1700bytes，可存储24条消息 **/
#define SENDMSGSIZE (24)            /**> 定义发送消息内存块大小，可存储24条消息 **/


#define MEMORYSIZE_50BYTES    (16)  /**> 定义发送消息50bytes内存块个数 **/
#define MEMORYSIZE_100BYTES   (6)   /**> 定义发送消息100bytes内存块个数 **/
#define MEMORYSIZE_250BYTES   (2)   /**> 定义发送消息250bytes内存块个数 **/

#define EMER_LEVEL_MSG        (0)   /**> 紧急级消息 **/
#define LINK_LEVEL_MSG        (1)   /**> 联动级消息 **/
#define UPDATA_LEVEL_MSG      (2)   /**> 上报消息   **/

extern const u8 HeadCMD[HEAD_SIZE];
extern const u8 TailCMD[TAIL_SIZE];

#pragma pack(1)
typedef union
{
    u32 id;
    u8  area[4];
}Object_t;      //目标描述共同体

typedef struct 
{
	u32	 		userID;     	// 用户号
	u8 			objectType;    	// 目标地址类型
	Object_t 	object;			// 目标地址
	u16 		actNum;  		// 广播类型号
	u32 		sourceID;   	// 源地址
	u8			serialNum;      // 流水号
}CMDAim_t;	//通信目类型描述

typedef struct 
{
	u8	msgType;       // 消息类型码
	u8  unit;           // 单元ID
	u8  cmd;              // 命令码
	u8  len;		  // 参数长度
}CMDPara_t;				//通信参数描述

typedef struct
{
	u8		version;   	// 版本号
	CMDAim_t aim;		//目标类型描述
	CMDPara_t para;		
}Communication_t;
#pragma pack()



u32 cmd_get(u8 *cmdBuf, FIFO_t *fifo);//从队列出一串指令
u32 cmd_put(FIFO_t *fifo, u8 *cmdBuf, u32 cmdLen);	//放置一串指令到目标队列中
u32 cmd_put_data(Communication_t *Msg, u32 cmdLen);
u32 cmd_get_data(u8 *cmdBuf);
u8 Put_Data_SendBuf(u8 *sdata,u32 cmdLen) ;
#endif 	//command.h
/**************************Copyright BestFu 2014-05-14*************************/

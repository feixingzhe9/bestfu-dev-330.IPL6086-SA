/***************************Copyright BestFu 2014-05-14*************************
文	件：    command.c
说	明:	    从队列中获取一条指令
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-10-14
修  改:     暂无
*******************************************************************************/
#include "command.h"
#include "fifo.h"
#include "crc.h"
#include "UserData.h"
#include "UnitCfg.h"

const u8 HeadCMD[HEAD_SIZE] = {0xF1, 0xF1, 0xF1, 0xF2, 0xF1, 0xF1};
const u8 TailCMD[TAIL_SIZE] = {0xF2, 0xF2, 0xF2, 0xF1, 0xF2, 0xF2};

static u8 sendBuf[SENDBUFSIZE]; 
static u8 gSendBufUseFlag[3]={0};
//                        0  1  2   3   4   5   6   7   8   9   9  10  11  12  13  14  15  16   17   18   19   20   21   22
static u32 MemoryTab[24]={0,50,100,150,200,250,300,350,400,450,500,550,600,650,700,750,800,900,1000,1100,1200,1300,1400,1650};

static u8 EmerMsgFlag[SENDMSGSIZE]  ={0};     /**> 紧急消息 **/
static u8 LinkMsgFlag[SENDMSGSIZE]  ={0};     /**> 联动消息 **/
static u8 UpdataMsgFlag[SENDMSGSIZE]={0};     /**> 上报消息 **/

static u8 Msg_SortStorge(Communication_t *pMsg,u8 *pSendBuf,u32 cmdLen,u8 *pMsgFlag,u8 *pSendBufUseFlag);
static u32 cmd_get_handle(u8 *pDataBuf,u8 *pSendBuf,u8 *pMsgFlag,u8 *pSendBufUseFlag);
/*******************************************************************************
函 数 名：  cmd_get
功能说明：  从队列中获取一条指令
参	  数：  fifo:	获取源
            cmdBuf: 指令存储缓冲区	
返 回 值：  指令长度。为零，没有可读指令
*******************************************************************************/
u32 cmd_get(u8 *cmdBuf, FIFO_t *fifo)
{
	u32 i;
     
	ERRR(fifo == NULL, goto ERR1);
	
	ERRR(fifo_empty(fifo) == TRUE, goto ERR1);        //FIFO为空
	i = fifo_find(fifo, HeadCMD, sizeof(HeadCMD));
	fifo->front = i;
	ERRR(fifo_empty(fifo) == TRUE, goto ERR1);        //FIFO为空

	i = sizeof(Communication_t) + sizeof(HeadCMD);
	ERRR(i > fifo_validSize(fifo), goto ERR1); //数据空间不够

	i += fifo->data[(fifo->front + i - ARGE_SEAT)%fifo->size] + CRC_SIZE + sizeof(TailCMD);
	ERRR(i > CMD_SIZE, goto ERR2);	//数据指令长度错误
	ERRR(i > fifo_validSize(fifo), goto ERR1); //数据空间不够
	
	ERRR(fifo_cmp(fifo, (fifo->front + i - sizeof(TailCMD))%fifo->size
					, TailCMD, sizeof(TailCMD)) == FALSE, goto ERR2);	//核对数据尾
					
	ERRR(fifo_gets(fifo, cmdBuf, i) == FALSE, goto ERR1);	//数据内容出队
	ERRR(ChkCrcValue(&cmdBuf[sizeof(HeadCMD)], (i - sizeof(HeadCMD) - sizeof(TailCMD))), goto ERR1);	//CRC错误
	
	return i;

ERR2:	fifo->front = (fifo->front+sizeof(HeadCMD))%fifo->size; //数据错误将头移出
ERR1:	return 0;	
}

/*******************************************************************************
函 数 名：  cmd_put
功能说明：  放置一条指令到FIFO中
参	  数：  fifo:	目标地址
            cmdBuf: 指令存储缓冲区	
			cmdLen:	放置内容长度
返 回 值：  指令长度。为零，没有可读指令
*******************************************************************************/
u32 cmd_put(FIFO_t *fifo, u8 *cmdBuf, u32 cmdLen)
{
	u16 crc;
	
	ERRR(fifo == NULL, goto ERR1);
	ERRR(fifo_puts(fifo, (u8*)HeadCMD, sizeof(HeadCMD)) == FALSE, goto ERR1);	//放置通信标志头
	crc = ChkCrcValue(cmdBuf, cmdLen);	//获取CRC
	ERRR(fifo_puts(fifo, cmdBuf, cmdLen) == FALSE, goto ERR1);	//放置参数区
	ERRR(fifo_putc(fifo, (u8)(crc>>8)) == FALSE, goto ERR1);	//放置CRC高位	
	ERRR(fifo_putc(fifo, (u8)(crc)) == FALSE, goto ERR1);		//放置CRC底位	
	ERRR(fifo_puts(fifo, (u8*)TailCMD, sizeof(TailCMD)) == FALSE, goto ERR1);	//放置通信标尾
	
	return TRUE;
ERR1:	return FALSE;
}

/*******************************************************************************
函 数 名：  static u32 cmd_get_handle(u8 *pDataBuf,u8 *pSendBuf,u8 *pMsgFlag,u8 *pSendBufUseFlag)
功能说明：  从数据缓冲区获取一条指令
参	  数：  pSendBufUseFlag:发送缓冲区标志组存储区
            pSendBuf:发送缓冲区首地址
            pMsgFlag：优先级消息标志组存储区
            pDataBuf: 指令存储缓冲区	
返 回 值：  指令长度。为零，没有可读指令
*******************************************************************************/
static u32 cmd_get_handle(u8 *pDataBuf,u8 *pSendBuf,u8 *pMsgFlag,u8 *pSendBufUseFlag)
{
	u32 i,k;
    u32 len=0;
    
    len =sizeof(HeadCMD)+sizeof(Communication_t)+((Communication_t *)(pSendBuf + MemoryTab[pMsgFlag[0]-1]+sizeof(HeadCMD)))->para.len+sizeof(u16) + sizeof(TailCMD);
    
    if(memcmp(pSendBuf+MemoryTab[pMsgFlag[0]-1],HeadCMD,sizeof(HeadCMD)))                                                       /**> 检验队列头是否正确 **/
    {
        pSendBufUseFlag[(pMsgFlag[0]-1)/8] &=~(1<<((pMsgFlag[0]-1)%8));
        for(i=1;i<24;i++)
        {
            if(!pMsgFlag[i])
            {
                break;
            }
        }
        for(k=0;k<i;k++)
        {
            pMsgFlag[k] = pMsgFlag[k+1];
        }
        return 0;
    }                           
    
    if(memcmp(pSendBuf + MemoryTab[pMsgFlag[0]-1]+len-sizeof(TailCMD),TailCMD,sizeof(TailCMD)))        /**> 检验队列尾是否正确 **/
    {
        pSendBufUseFlag[(pMsgFlag[0]-1)/8] &=~(1<<((pMsgFlag[0]-1)%8));
        for(i=1;i<24;i++)
        {
            if(!pMsgFlag[i])
            {
                break;
            }
        }
        for(k=0;k<i;k++)
        {
            pMsgFlag[k] = pMsgFlag[k+1];
        }
        return 0;
    } 
    
    memcpy(pDataBuf,pSendBuf+MemoryTab[pMsgFlag[0]-1],len);
    if(ChkCrcValue(&pDataBuf[sizeof(HeadCMD)], (len - sizeof(HeadCMD) - sizeof(TailCMD))))
    {
        pSendBufUseFlag[(pMsgFlag[0]-1)/8] &=~(1<<((pMsgFlag[0]-1)%8));
        for(i=1;i<24;i++)
        {
            if(!pMsgFlag[i])
            {
                break;
            }
        }
        for(k=0;k<i;k++)
        {
            pMsgFlag[k] = pMsgFlag[k+1];
        }
        return 0;
    }
    
    pSendBufUseFlag[(pMsgFlag[0]-1)/8] &= ~(1<<((pMsgFlag[0]-1)%8));             /**> 先清除发送缓冲区标志组存储区标志位，再清除优先级消息标志组存储区 **/
    for(i=1;i<24;i++)
    {
        if(!pMsgFlag[i])
        {
            break;
        }
    }
    for(k=0;k<i;k++)
    {
        pMsgFlag[k] = pMsgFlag[k+1];
    }    
    
	return len;
}

/*******************************************************************************
函 数 名：  cmd_get_data
功能说明：  从队列中获取一条指令
参	  数：  fifo:	获取源
            cmdBuf: 指令存储缓冲区	
返 回 值：  指令长度。为零，没有可读指令
*******************************************************************************/
u32 cmd_get_data(u8 *cmdBuf)
{
    u8 *pTempMsgFlag=NULL;
    
    if(NULL == cmdBuf)
    {
        return 0;
    }
    if(!(EmerMsgFlag[0]||LinkMsgFlag[0]||UpdataMsgFlag[0]))
    {
        return 0;
    }
    if(EmerMsgFlag[0])
    {  
        pTempMsgFlag = EmerMsgFlag;
    }
    else if(LinkMsgFlag[0])
    {
        pTempMsgFlag = LinkMsgFlag;
    }
    else if(UpdataMsgFlag[0])
    {
        pTempMsgFlag = UpdataMsgFlag;
    } 
	return (cmd_get_handle(cmdBuf,sendBuf,pTempMsgFlag,gSendBufUseFlag));
}

/*******************************************************************************
函 数 名：  cmd_put
功能说明：  放置一条指令到FIFO中
参	  数：  fifo:	目标地址
            cmdBuf: 指令存储缓冲区	
			cmdLen:	放置内容长度
返 回 值：  指令长度。为零，没有可读指令
*******************************************************************************/
u32 cmd_put_data(Communication_t *pMsg, u32 cmdLen)
{
    u8 *pTempMsgFlag=NULL;
    if( NULL == pMsg)
    {
        return 0;
    }
    if((ACKWRITE == pMsg->para.msgType)||(READACK == pMsg->para.msgType))       /**> 区分出紧急消息 **/
    {
        pTempMsgFlag = EmerMsgFlag;
    }
    else if(WRITENACK == pMsg->para.msgType)                                    /**> 区分出联动消息 **/
    {
        pTempMsgFlag = LinkMsgFlag;
    }
    else if(0x88888888 == pMsg->aim.object.id)                                  /**> 区分出状态上报消息 **/
    {
        pTempMsgFlag = UpdataMsgFlag;
    }

	return (Msg_SortStorge(pMsg,sendBuf,cmdLen,pTempMsgFlag,gSendBufUseFlag));  
}

/*******************************************************************************
函 数 名：  static u8 Msg_SortStorge(Communication_t *pMsg,u8 *pSendBuf,u32 cmdLen,u8 *pMsgFlag,u8 *pSendBufUseFlag) 
功能说明：  将消息分类存储
参	  数：  pMsg:	消息内容
            pSendBuf: 消息缓冲区的首地址	
            pMsgFlag：消息标志组的首地址
            pSendBufUseFlag：消息缓冲区标志组的首地址
			cmdLen:	放置内容长度
返 回 值：  指令长度。为零，没有可读指令
*******************************************************************************/
static u8 Msg_SortStorge(Communication_t *pMsg,u8 *pSendBuf,u32 cmdLen,u8 *pMsgFlag,u8 *pSendBufUseFlag) 
{
    u8 i,j,k,m,n,t,h;
    u16 crc,crc_temp = 0;
    u16 DataLen;
    crc = ChkCrcValue((u8*)pMsg, cmdLen);	          /**> 获取CRC **/
    
    crc_temp = (crc<<8) + (crc>>8);
    
    DataLen = cmdLen + sizeof(HeadCMD) + sizeof(TailCMD) + sizeof(u16);
    if(DataLen <= 50)
    {
        t = 0;h = 18;                                /**> 当消息内容大小小于50bytes时，可有18条存储空间，包括16条50bytes  和 2条100个字节的空间 **/
    }
    else if((DataLen > 50)&&(DataLen <=100))
    {
        t = 16;h = 24;                               /**> 当消息内容大小小于100bytes而大于50bytes时，可有6条存储空间，包括6条100bytes  和 2条250个字节的空间 **/
    }
    else if((DataLen <= 250)&&(DataLen > 100))                
    {
        t = 22;h = 24;                               /**> 当消息内容大小小于250bytes而大于100bytes时，可有2条存储空间，2条250个字节的空间 **/
    }
    else                                             /**> 注意：1条消息长度不能超过250bytes **/
    {
        return 0;
    }
    
    for(i=t;i<h;i++)
    {
         if(!(gSendBufUseFlag[i/8]&(1<<(i%8))))     /**> 查找首个符合要求的内存块 **/
         {
             memcpy(pSendBuf + MemoryTab[i],(u8*)HeadCMD,sizeof(HeadCMD));
             memcpy(pSendBuf + MemoryTab[i] + sizeof(HeadCMD),(u8*)pMsg,cmdLen);
             memcpy(pSendBuf + MemoryTab[i] + sizeof(HeadCMD) + cmdLen,(u8*)(&crc_temp),sizeof(u16));
             memcpy(pSendBuf + MemoryTab[i] + sizeof(HeadCMD) + cmdLen + sizeof(u16),(u8*)(TailCMD),sizeof(TailCMD));
             gSendBufUseFlag[i/8] |= (1<<(i%8));
            
            for(j=0;j<24;j++)
            {
                if(!pMsgFlag[j])
                {
                    pMsgFlag[j] = i+1;
                    n = j;
                    for(k=0;k<n;k++)
                    {
                        if((0xffffffff != ((Communication_t *)(pSendBuf + MemoryTab[i]+sizeof(HeadCMD)))->aim.userID)&&                          /**> 如何消息相同，则覆盖 **/
                        (!((memcmp(pSendBuf + MemoryTab[pMsgFlag[k]-1]+sizeof(HeadCMD),pSendBuf + MemoryTab[i]+sizeof(HeadCMD),sizeof(CMDAim_t)))||
                           (memcmp(pSendBuf + MemoryTab[pMsgFlag[k]-1]+sizeof(HeadCMD)+sizeof(Communication_t)-sizeof(CMDPara_t),pSendBuf + MemoryTab[i]+sizeof(HeadCMD)+sizeof(Communication_t)-sizeof(CMDPara_t),sizeof(CMDPara_t)-sizeof(u8))))))                     
                        {
                            pSendBufUseFlag[(pMsgFlag[k]-1)/8] &=~(1<<((pMsgFlag[k]-1)%8));                                                     /**> 清除消息缓冲区标志数组相应位 **/
                            for(m=k;m<n;m++)
                            {
                                pMsgFlag[m] = pMsgFlag[m+1];
                            }
                            pMsgFlag[n] = 0;
                            n = n-1;
                        }
                    }                                
                    break;
                }                                
            }
            break;
         }
    }
    if(i>23)
    {
        return 0;
    }
    return 1;
}

/*******************************************************************************
函 数 名：  void Put_Data_SendBuf(u8 *pSendBuf,u32 cmdLen) 
功能说明：  用于消息转发，将要转发的消息存于发送BUF中 
参	  数：  pMsg:	消息内容
			cmdLen:	放置内容长度
返 回 值：  无
说    明：  消息转发使用了紧急优先级(最高发送优先级)
*******************************************************************************/
u8 Put_Data_SendBuf(u8 *sdata,u32 cmdLen) 
{
    u8 i,j,t,h;
    if(cmdLen <= 50)
    {
        t = 0;h = 18; 
    }
    else if((cmdLen > 50)&&(cmdLen <=100))
    {
        t = 16;h = 24;
    }
    else if((cmdLen <= 250)&&(cmdLen > 100))                                          
    {
        t = 22; h = 24;
    }
    else                                            /**> 注意：1条消息长度不能超过250bytes **/
    {
        return 0;
    }
    
    for(i=t;i<h;i++)
    {
         if(!(gSendBufUseFlag[i/8]&(1<<(i%8))))     /**> 查找首个符合要求的内存块 **/
         {
             memcpy(sendBuf + MemoryTab[i],(u8*)sdata,cmdLen);
             gSendBufUseFlag[i/8] |= (1<<(i%8));
            
            for(j=0;j<24;j++)
            {
                if(!EmerMsgFlag[j])
                {
                    EmerMsgFlag[j] = i+1;                               
                    break;
                }                                
            }
            break;
         }
    }
    if(i>23)
    {
        return 0;
    }
    return 1;
}

/**************************Copyright BestFu 2014-05-14*************************/

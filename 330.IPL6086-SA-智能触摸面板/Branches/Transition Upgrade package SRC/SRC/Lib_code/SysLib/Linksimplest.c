/***************************Copyright BestFu 2014-05-14*************************
文	件：	Linksimplest.c
说	明：	最简联动源文件
编	译：	Keil uVision4 V4.54.0.0
版	本：	v1.0
编	写：	Joey
日	期：	2013.7.22 
修　改：	Unarty 2014.11.11 ALinkInit中将EEPROM第一次使用判断去除
*******************************************************************************/

#include "Linkall.h"
#if (ALINK_EN > 0)
ALinkTable   ALink;   //最简联动的RAM映射变量

/*******************************************************************************
函 数 名：	ALinkInit
功能说明： 	最简联动表的初始化
参	  数： 	void
返 回 值：	void
*******************************************************************************/
void ALinkInit(void)
{
//	u8 data=0;
//	ReadDataFromEEPROM(GetALinkInited(), 1, &data);

//	if( data == EEPROMINITED )
//	{
	if (FALSE == ReadDataFromEEPROM(GetALinkStartAddr(), sizeof(ALinkTable), 
							(u8 *)&ALink))
	{
		return;
	}
//	}
//	else
//	{
//		data = EEPROMINITED;
//		WriteDataToEEPROM(GetALinkInited(), 1, &data);
//		memset((u8 *)&ALink, 0, sizeof(ALinkTable));
//		WriteDataToEEPROM(GetALinkStartAddr(), sizeof(ALinkTable), 
//							(u8 *)&ALink);
//	}
	if(ALink.Cnt > MAX_ALINKITEM)
	{
//		data = EEPROMINITED;
//		WriteDataToEEPROM(GetALinkInited(), 1, &data);
//		memset((u8 *)&ALink, 0, sizeof(ALinkTable));
//		WriteDataToEEPROM(GetALinkStartAddr(), sizeof(ALinkTable), 
//			(u8 *)&ALink);
		ALinkClr();
	}
}

/*******************************************************************************
函 数 名：	ALinkClr
功能说明： 	清除最简联动表
参	  数： 	void
返 回 值：	void
*******************************************************************************/
void ALinkClr(void)
{
	u8 data;
	data = EEPROMINITED;
	WriteDataToEEPROM(GetALinkInited(), 1, &data);
	if(ALink.Cnt != 0)
	{
		memset((u8 *)&ALink, 0, sizeof(ALinkTable));
		WriteDataToEEPROM(GetALinkStartAddr(), sizeof(ALinkTable), (u8 *)&ALink);
	}
}

/*******************************************************************************
函 数 名：	ChkALinkIsExist
功能说明： 	匹配联动数据在联动表中是否存在，如果存在把联动号存于linknum指针
参	  数： 	 *linknum-从联动表中找到的联动号存放于此
             *data-联动数据
返 回 值：	1表示成功，0表示失败
*******************************************************************************/
u8 ChkALinkIsExist(u8 *linknum, u8 *data)
{
	u8 i,cnt;
	
	for(i=0;i<ALink.Cnt;i++)
	{
		if(ALink.linkitem[i].UseFlag)
		{// 匹配是否配有相同目标?前2字节和后面13个参数不需要匹配(参数区也匹配，识别一模一样的联动)
			cnt = sizeof(ALinkHead)- 2 + ALink.linkitem[i].DestLen;
			if(!memcmp(data, &ALink.linkitem[i].SUnitID, cnt))
			{
				*linknum = ALink.linkitem[i].LinkNum;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*******************************************************************************
函 数 名：	GetALinkNum
功能说明： 	获取联动号，从小到大顺序，找最小的值为最新联动号
参	  数： 	*linknum-找到的最小联动号存放于此
返 回 值：	1表示成功，0表示失败
*******************************************************************************/
u8 GetALinkNum(u8 *linknum)
{
	u8 i,j;
	for(i=1;i<254;i++)//联动号范围1-254
	{
		for(j=0;j<ALink.Cnt;j++)
		{
			if(ALink.linkitem[j].UseFlag)
			{
				if(i == ALink.linkitem[j].LinkNum)
				{
					break;
				}
			}
		}
		if(j == ALink.Cnt)
		{
			*linknum = i;
			return TRUE;
		}
	}
	return FALSE;
}

/*******************************************************************************
函 数 名：	QueryALink
功能说明： 	查询data联动号在最简联动表中的位置
参	  数： 	num-要查询的联动号
返 回 值：	EXEERROR表示没找到，0-254表示找到的实际位置
*******************************************************************************/
u8 QueryALink(u8 num)
{
	u8 i;
	for(i=0;i<ALink.Cnt;i++)
	{
		if(ALink.linkitem[i].UseFlag)
		{
			if(num == ALink.linkitem[i].LinkNum)
			{
				return i;
			}
		}
	}
	return EXEERROR;
}

/*******************************************************************************
函 数 名：	CheckALinkFull
功能说明： 	检查最简联动表是否满
参	  数： 	void
返 回 值：	1表示已满，0表示不满
*******************************************************************************/
u8 CheckALinkFull(void)
{
	if(ALink.Cnt >= MAX_ALINKITEM)
		return TRUE;
	else
		return FALSE;
}

/*******************************************************************************
函 数 名：	ReadALink
功能说明： 	从最简联动表中读取联动
参	  数： 	num-要查询的联动位置，读取到的联动数据存放于link指针
            *len-读取到的数据长度
            *data-读取到的数据
返 回 值：	1表示读取成功，0表示读取失败
*******************************************************************************/
u8 ReadALink(u8 num , u8 *len, u8 *data)
{
	u8 cnt;
	cnt = sizeof(ALinkHead) - 2 + ALink.linkitem[num].DestLen;
	memcpy(data, (u8 *)&ALink.linkitem[num].SUnitID, cnt);
	*len = cnt;
	return TRUE;
}

/*******************************************************************************
函 数 名：	WriteALink
功能说明： 	把联动写入最简联动表中
参	  数： 	num-要写入的联动位置，把data指针数据写入num位置
返 回 值：	1表示写入成功，0表示写入失败
*******************************************************************************/
u8 WriteALink(u8 num, u8 linknum, u8 addflag, u8 *data)
{
	ALinkItem *pLink;
	u8 cnt;
	pLink = (ALinkItem *)(data-2);
	if(num <= MAX_ALINKITEM)		//2014.10.08 Unarty Change 解决联动最大数只能是49条不是50条的问题
	{
		cnt = sizeof(ALinkHead)-2+pLink->DestLen;
		memcpy(&ALink.linkitem[num].SUnitID, data, cnt);
		ALink.linkitem[num].UseFlag = 1;
		ALink.linkitem[num].LinkNum = linknum;
		WriteDataToEEPROM(GetALinkStartAddr() + sizeof(ALinkItem)*num, 
				sizeof(ALinkItem), (u8 *)&ALink.linkitem[num]);

		if(addflag)
		{
			ALink.Cnt++;
			WriteDataToEEPROM(GetALinkStartAddr() + ((u32)&ALink.Cnt - (u32)&ALink), 
							sizeof(u8), &ALink.Cnt);
		}
		return TRUE;
	}
	return FALSE;
}

/*******************************************************************************
函 数 名：	DelALink
功能说明： 	把联动从最简联动表中删除
参	  数： 	num-要删除的联动位置
返 回 值：	1表示删除成功，0表示删除失败
*******************************************************************************/
u8 DelALink(u8 num)
{
	if(num != (ALink.Cnt-1))
	{
		memcpy((u8 *)&ALink.linkitem[num],
			(u8 *)&ALink.linkitem[ALink.Cnt-1],sizeof(ALinkItem));
		WriteDataToEEPROM(GetALinkStartAddr() + sizeof(ALinkItem)*num, 
				sizeof(ALinkItem), (u8 *)&ALink.linkitem[num]);
		memset((u8 *)&ALink.linkitem[ALink.Cnt-1], 0, sizeof(ALinkItem));
		WriteDataToEEPROM(GetALinkStartAddr() + sizeof(ALinkItem)*(ALink.Cnt-1), 
				sizeof(ALinkItem), (u8 *)&ALink.linkitem[num]);
	}
	else
	{
		memset((u8 *)&ALink.linkitem[num], 0, sizeof(ALinkItem));
		WriteDataToEEPROM(GetALinkStartAddr() + sizeof(ALinkItem)*num, 
				sizeof(ALinkItem), (u8 *)&ALink.linkitem[num]);
	}
	
	if (ALink.Cnt > 0)
	{
		ALink.Cnt--;
		WriteDataToEEPROM(GetALinkStartAddr() + ((u32)&ALink.Cnt - (u32)&ALink), 
				sizeof(u8), (u8 *)&ALink.Cnt);
		return TRUE;
	}
	
	return FALSE;
}

#if (SLINK_SRC_VALUE_EN > 0)
/*******************************************************************************
函 数 名：	CheckALinkValueAndExe
功能说明： 	判断ALINK中条件值，是否满足条件，满足条件执行
参	  数：  num-要删除的联动位置
返 回 值：	1表示删除成功，0表示删除失败
*******************************************************************************/
u8 CheckALinkValueAndExe(u8 *data, u8 unitID, u8 propID, int value)
{
	int sv1,sv2,sv3,sv4,oldvalue=0,vtmp=0;
	u8 screv,num;
	ALinkItem *atmp;

	if((num = FindOldValueTableItem(unitID, propID)) == 0xFF)
	{
		if(AddOldValueTableItem(unitID, propID))
		{
			oldvalue = SrcOld.src[SrcOld.Cnt-1].Value;//得到本次相同的值
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		oldvalue = SrcOld.src[num].Value;
	}

	atmp  = (ALinkItem *)data;
	screv = GetConditionReverse(atmp->SCondition);
	
	memcpy((u8 *)&sv1, atmp->SValue, sizeof(int));
	sv3 = sv1;
	sv2 = 0;
	sv4 = sv2;
	GetConditionOffset(atmp->SCondition, &sv1, &sv2);

	if(CmpSampAndLinkValue(atmp->SCondition, sv1, sv2, value))//判断这次是否满足条件
	{
		if(CmpSampAndLinkValue(screv, sv1, sv2, oldvalue))//判断上一次是否不满足条件
		{
			vtmp = DWCHAR( atmp->DDevID[3],
							atmp->DDevID[2],
							atmp->DDevID[1],
							atmp->DDevID[0]);

			Msg_Send((ObjectType_t)atmp->DType, 
					vtmp, 
					WCHAR(atmp->DAddrT[1],atmp->DAddrT[0]), 
					atmp->UnitID, 
					atmp->DestCmd, 
					atmp->DestLen, 
					atmp->data);
		}
		SrcOld.src[num].Value = value;//保存上一次值
		return TRUE;
	}

	GetConditionOffset(atmp->SCondition, &sv3, &sv4);

	if(CmpSampAndLinkValue(screv, sv3, sv4, value))//判断这次反面是否满足条件
	{
		if(CmpSampAndLinkValue(atmp->SCondition, sv3, sv4, oldvalue))//判断上一次反面是否不满足条件
		{
			//发送反向联动码
			atmp->data[0] = !atmp->data[0];
			vtmp = DWCHAR( atmp->DDevID[3],
							atmp->DDevID[2],
							atmp->DDevID[1],
							atmp->DDevID[0]);
			Msg_Send((ObjectType_t)atmp->DType, 
					vtmp, 
					WCHAR(atmp->DAddrT[1],atmp->DAddrT[0]), 
					atmp->UnitID, 
					atmp->DestCmd, 
					atmp->DestLen, 
					atmp->data);
		}
	}
	
	SrcOld.src[num].Value = value;//保存上一次值
	return TRUE;
}
#endif
#endif

/**************************Copyright BestFu 2014-05-14*************************/


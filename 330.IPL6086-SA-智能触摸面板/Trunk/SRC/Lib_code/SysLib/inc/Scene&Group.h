/***************************Copyright BestFu 2014-05-14*************************
��	����    Scene&Group.h
˵	����    �����������ͷ�ļ�
��	�룺    Keil uVision4 V4.54.0.0
��	����    v1.0
��	д��    Unarty
��	�ڣ�    2014.06.26 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __SCENEGROUP_H
#define __SCENEGROUP_H

#include "UnitCfg.h"

//data structure define
typedef struct
{
	u8 start;	//��ʼֵ
	u8 cond;	//����1
}Condition_t;

//function declaration
MsgResult_t Set_Scene_Attr(UnitPara_t *pData);
MsgResult_t Get_Scene_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);

MsgResult_t Set_Group_Attr(UnitPara_t *pData);
MsgResult_t Get_Group_Attr(UnitPara_t *pData, u8 *rlen, u8 *rpara);

#endif //Scene&Group.h end
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2014-05-14*************************
��	����	UnitCgf.h
˵	����	��Ԫģ������ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.07.14 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __UNITCFG_H
#define __UNITCFG_H

/*********************************��������*************************************/
extern const Unit_st UnitTab[];
extern UnitData_t gUnitData[];

/*********************************��������*************************************/
void Unit_Init(void);
BF_INT08U UnitCnt_Get(void);

/*�û�EEPROM���ݶ�д��������*/
extern MsgResult_t EEPROM_Write(BF_INT32U addr, BF_INT32U len, BF_INT08U *data);
extern MsgResult_t EEPROM_Read(BF_INT32U addr, BF_INT32U len, BF_INT08U *data);

#endif
/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2014-05-14*************************
��	����    PivotalDataType.c
˵	��:	    ��ؼ���������
��	�룺    uVision V5.12.0
��	����    v1.0
��	д��    jay
��	�ڣ�    2016-04-14
��  ��:     ����
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "PivotalDataType.h"

//private variable define
const BF_INT08U HeadCMD[HEAD_SIZE] = {0xF1, 0xF1, 0xF1, 0xF2, 0xF1, 0xF1};
const BF_INT08U TailCMD[TAIL_SIZE] = {0xF2, 0xF2, 0xF2, 0xF1, 0xF2, 0xF2};


/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2016-04-14*************************
��	����	FifoDataProc.h
˵	����	fifo���ݴ���ͷ�ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2016-04-14
��  ��:     NONE
*******************************************************************************/

#ifndef __FIFO_DATA_PROC_H
#define __FIFO_DATA_PROC_H

/********************************���ݽṹ����**********************************/


/**********************************��������***********************************/   
BF_BOOLEAN head_put(FIFO_t *fifo);
BF_BOOLEAN tail_put(FIFO_t *fifo);
BF_INT32U decryptfifo_get(BF_INT08U *cmdBuf, FIFO_t *fifo);
BF_INT32U EncryptCmd_put(FIFO_t *fifo, BF_INT08U *cmdBuf, BF_INT32U cmdLen);
BF_INT32U decryptfifo_getEx(BF_INT08U *cmdBuf, FIFO_t *fifo);

#endif //FifoDataProc.h end
/**************************Copyright BestFu 2016-04-14*************************/

/***************************Copyright BestFu 2016-04-14*************************
��	����	FifoDataProc.c
˵	����	����FIFO�ӿ��ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2016-04-14
��  ��:     NONE
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "crc.h"
#include "fifo.h"
#include "BestFuLib.h"
#include "DataEncrypt.h"
#include "PivotalDataType.h"
#include "FifoDataProc.h"


/*******************************************************************************
�� �� ����  head_put
����˵����  �����з���һ�����ݱ�ʶͷ
��	  ����  fifo:	Ŀ�����
�� �� ֵ��  ���ý����TRUR(���óɹ�)/FASLE
*******************************************************************************/
BF_BOOLEAN head_put(FIFO_t *fifo)
{
    return fifo_puts(fifo, (BF_INT08U*)HeadCMD, HEAD_SIZE);
}

/*******************************************************************************
�� �� ����  tail_put
����˵����  �����з���һ�����ݱ�ʶβ
��	  ����  fifo:	Ŀ�����
�� �� ֵ��  ���ý����TRUR(���óɹ�)/FASLE
*******************************************************************************/
BF_BOOLEAN tail_put(FIFO_t *fifo)
{
    return fifo_puts(fifo, (BF_INT08U*)TailCMD, TAIL_SIZE);
}

/*******************************************************************************
����  ��:  BF_INT32U decryptfifo_get(BF_INT08U *cmdBuf, FIFO_t *fifo)
����˵��:  �Ӽ��ܶ����л�ȡһ֡����
��    ��:  fifo��������е��׵�ַ
		   cmdBuf���洢�������ݵ��׵�ַ
����  ֵ:  ��
*******************************************************************************/
BF_INT32U decryptfifo_get(BF_INT08U *cmdBuf, FIFO_t *fifo)
{
    BF_INT32U pos = 0u;

    ERRR(fifo == NULL, goto ERR1);
    ERRR(fifo_validSize(fifo) <= HEAD_SIZE, goto ERR1);				//�жϿ�����Ч�����Ƿ����֡ͷ

    pos = fifo_find(fifo, HeadCMD, HEAD_SIZE);
    fifo->front = pos;

    ERRR(fifo_empty(fifo) == TRUE, goto ERR1);        				//FIFOΪ��
    pos = fifo->data[(fifo->front + HEAD_SIZE)%fifo->size];			//��ȡ��Чָ���
    ERRR(COMMUNICATION_VERSION >= pos , goto ERR2);	 				//��ͨ��Э��ֱ֡���Ƴ� yanhuan adding 2016/01/13
    pos += HEAD_SIZE + TAIL_SIZE;									//��ȡ�������ܳ���
    ERRR(pos > MAX_DATA_FRAME_LEN, goto ERR2);						//����ָ��ȴ���
    ERRR(pos > fifo_validSize(fifo), goto ERR1); 					//���ݿռ䲻��

    ERRR(fifo_cmp(fifo, (fifo->front + pos - TAIL_SIZE)%fifo->size\
                  , TailCMD, TAIL_SIZE) == FALSE, goto ERR2);		//�˶�����β
    ERRR(fifo_gets(fifo, cmdBuf, pos) == FALSE, goto ERR1);			//�������ݳ���

    return pos;

ERR2:
    fifo->front = (fifo->front + HEAD_SIZE)%fifo->size; 		//���ݴ���ͷ�Ƴ�
ERR1:
    return 0;
}

/*******************************************************************************
����  ��:  BF_INT32U decryptfifo_getEx(BF_INT08U *cmdBuf, FIFO_t *fifo)
����˵��:  �Ӽ��ܶ����л�ȡһ֡����,����������
��    ��:  fifo��������е��׵�ַ
		   cmdBuf���洢�������ݵ��׵�ַ
����  ֵ:  ��
*******************************************************************************/
BF_INT32U decryptfifo_getEx(BF_INT08U *cmdBuf, FIFO_t *fifo)
{
    BF_INT32U pos = decryptfifo_get(cmdBuf, fifo);

	if(pos && (!DecryptData((BF_INT08U*)(&cmdBuf[HEAD_SIZE]))))	//�����ܳɹ�
	{
		pos = HEAD_SIZE + sizeof(Communication_t) + ((Communication_t*)&cmdBuf[HEAD_SIZE])->para.len + CRC_SIZE;
		memcpy(&cmdBuf[pos], (BF_INT08U*)TailCMD, TAIL_SIZE); 
		pos += TAIL_SIZE;
	}
    return (pos);
}

/*******************************************************************************
�� �� ����  EncryptCmd_put
����˵����  ����һ��ָ�����õ�FIFO��
��	  ����  fifo:	Ŀ���ַ
            cmdBuf: ָ��洢������
			cmdLen:	�������ݳ���
�� �� ֵ��  ָ��ȡ�Ϊ�㣬û�пɶ�ָ��
*******************************************************************************/
BF_INT32U EncryptCmd_put(FIFO_t *fifo, BF_INT08U *cmdBuf, BF_INT32U cmdLen)
{
    BF_INT16U crc;
	BF_INT08U cmdBuf_temp[255];
	
	CPU_SR_ALLOC();
    ERRR(NULL == fifo, goto ERR1);
    ERRR(cmdLen > MAX_DATA_FRAME_LEN, goto ERR1);

	memcpy(cmdBuf_temp, cmdBuf, cmdLen);
    crc = ChkCrcValue(cmdBuf_temp, cmdLen);				//��ȡCRC
	ENTER_CRITICAL();
    ERRR(fifo_puts(fifo, (BF_INT08U*)HeadCMD, sizeof(HeadCMD)) == FALSE, goto ERR1);	//����ͨ�ű�־ͷ
    cmdBuf_temp[cmdLen++] = (BF_INT08U)(crc >> 8);
    cmdBuf_temp[cmdLen++] = (BF_INT08U)(crc);			//�������CRC
	
    ERRR(!EncryptData(cmdBuf_temp, cmdLen), goto ERR1);	//����ʧ��,���ؼ�
    cmdLen = cmdBuf_temp[0];
    ERRR(cmdLen > MAX_DATA_FRAME_LEN, goto ERR1);
    ERRR(fifo_puts(fifo, cmdBuf_temp, cmdLen) == FALSE, goto ERR1);	//���ò�����
    ERRR(fifo_puts(fifo, (BF_INT08U*)TailCMD, sizeof(TailCMD)) == FALSE, goto ERR1);	//����ͨ�ű�β
	EXIT_CRITICAL();
	
    return TRUE;
ERR1:
    return FALSE;
}

/**************************Copyright BestFu 2016-05-14*************************/

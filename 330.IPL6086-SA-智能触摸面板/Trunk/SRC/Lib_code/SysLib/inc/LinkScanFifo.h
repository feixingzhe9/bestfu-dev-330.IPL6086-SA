/***************************Copyright BestFu 2014-05-14*************************
��	����	LinkScanFifo.h
˵	����	���Ա仯�������������
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Joey
��	�ڣ�	2013.7.22 
�ޡ��ģ�	����
*******************************************************************************/
#ifndef    _linkscanfifo_h_
#define    _linkscanfifo_h_

#define PROPBUFSIZE       128
#pragma pack(1) //��1�ֽڶ���
typedef struct
{
	unsigned char UnitID;
	unsigned char PropID;
	unsigned char Type;
	int           Value;
}PropFifoItem;
#pragma pack()  //ȡ����1�ֽڶ���

void PropFifoInit(void);
unsigned char PropChangeScanAndExe(void);

#endif
/**************************Copyright BestFu 2014-05-14*************************/

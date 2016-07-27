/***************************Copyright BestFu 2014-05-14*************************
��	����	Thread.h
˵	����	�̹߳���ͷ�ļ�
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.18  
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __THREAD_H
#define __THREAD_H

#include "BF_type.h"


#define THREAD_NUM      20      //����߳����� 

typedef struct
{
    u8 flag;    //���б�ʶ
    u8 con;     //��������
    u16 cnt;     //���д���
    u32 frep;   //��������
	u32 countTime ;//�߳�ִ�еĵȴ�ʱ�� yanhuan adding 2015/10/27
    void *fun;  //���к���ָ��
}Thread_t;

typedef struct
{
	u16	runCnt;		//��ǰ�߳�������
	u16 hangCnt;	//�̹߳�����
	
    Thread_t thread[THREAD_NUM];
}ThreadManage_t;

typedef enum
{
    FINISH  = 0,     //���
    REPEAT  = 1,     //�߳���ע��
    FULL    = 2,     //�߳�ע��ռ�����
}ThreadSta_t;       //�߳�ע����

typedef enum
{
    ONCEDELAY	= 0,    //������ʱ����
    MANY		= 1,   	//���
    FOREVER     = 2,    //����
    RESULT      = 3,    //�����
}ThreadCon_t;     //�߳�פ������
 
typedef enum
{
	NO_EXIST_STATE = 0,			//�������������� 	
	RUNNING_STATE,   			//����̬ 	
	PENDING_STATE				//����̬ 
}ThreadState_t;

typedef void (*fun1)(void); 
typedef u8 (*fun2)(void);
    
void Thread_Init(void);     //�̳߳�ʼ������
u32 Thread_Login(ThreadCon_t con, u16 cnt, u32 frep, void * fun);   //�߳�ע��
void Thread_Logout(void *fun);  //�߳�ע��
void Thread_Process(void);      //�߳�����
void Thread_RunCheck(void); //�߳�����ʱ��˶�
void Thread_Hang(void * fun);   //�̹߳���
void Thread_Wake(void * fun);   //�̻߳���    
ThreadState_t Thread_Query(void *fun);//��ѯ�̵߳�ǰ״̬   //Jay Add 2015.11.17

#endif //Thread.h end
/**************************Copyright BestFu 2014-05-14*************************/

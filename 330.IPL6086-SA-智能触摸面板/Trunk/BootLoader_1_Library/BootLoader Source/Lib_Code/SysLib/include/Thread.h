/***************************Copyright BestFu 2016-04-13*************************
��	����	Thread.h
˵	����	�̹߳���ͷ�ļ�
��	�룺	uVision V5.12.0.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2014.06.18  
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __THREAD_H
#define __THREAD_H

/*************************************�Զ����*********************************/
#define THREAD_TIME_SLICE  		(1u)	//�߳�ʱ��Ƭ��С,1ms

/**********************************���ݽṹ����********************************/
typedef struct
{
	BF_INT08U	run_con;	//��������
	BF_INT08U 	flag;		//������б��
	BF_INT16U	run_cnt;	//���д���
	BF_INT32U	run_frep;	//����Ƶ��
	CPU_TS_TMR	tick_ctr; 	//��������
	BF_FNCT_PTR fun;		//���к���ָ��	
}Thread_t;

typedef struct
{
	BF_INT16U	runCnt;		//��ǰ�߳�������
	BF_INT16U	hangCnt;	//��ǰ�̹߳�����
    Thread_t thread[MAX_THREAD_NUM];
}ThreadManage_t;

typedef enum
{
    ONCEDELAY	= 0,    //������ʱ����
    MANY		= 1,   	//���
    FOREVER     = 2,    //����
    RESULT      = 3,    //�����
}ThreadCon_t;     		//�߳�פ������
 
typedef enum
{
	BF_ERR_THREAD_NONE = 1,		//�߳��״�ע��
	BF_ERR_THREAD_EXHAUSTED,	//��ǰ�߳�����
	BF_ERR_THREAD_EXIST,		//�߳��Ѵ���
}Thread_ERR_t;
 
/**********************************��������************************************/
void Thread_Init(void);     //�̳߳�ʼ������
Thread_ERR_t Thread_Login(ThreadCon_t con, BF_INT16U cnt, BF_INT32U frep, void * fun);   //�߳�ע��
void Thread_Logout(void *fun);	//�߳�ע��
void Thread_Process(void);		//�߳�����
void Thread_RunCheck(void); 		//�߳�����ʱ��˶�  
void Thread_Hang(void * fun);
void Thread_Wake(void * fun);

#endif //Thread.h end
/**************************Copyright BestFu 2016-04-13*************************/

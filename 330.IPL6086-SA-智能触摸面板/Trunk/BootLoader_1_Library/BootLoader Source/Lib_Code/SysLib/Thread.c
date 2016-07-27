/***************************Copyright BestFu 2014-05-14*************************
��	����	Thread.c
˵	����	�߳�ʵ�ֺ���
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014.06.18
�ޡ��ģ�	2014.08.30 //�޸ĵ�����ʱע���̣߳���ʱ����벻��ִ������
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "BestFuLib.h"
#include "Thread.h"

//Private variable define
#define THREAD_TOTAL    (thread.runCnt + thread.hangCnt)    	//�����߳�����

//define Private variable
static ThreadManage_t   thread;

//Private function declaration
static BF_INT32U Thread_Check(BF_INT32U start, BF_INT32U end, void *fun);

/*******************************************************************************
����  ��:  Thread_Init
����˵��:  �߳����ݳ�ʼ��
��    ��:  ��
����  ֵ:  ��
*******************************************************************************/
void Thread_Init(void)
{
    *(BF_INT32U*)&thread.runCnt = 0;   //�����߳������ÿ�
}

/*******************************************************************************
����  ��:  Thread_Login
����˵��:  �߳�ע��
��    ��:  fun: ִ�к�����ָ��
           ִ������
����  ֵ:  ע����
*******************************************************************************/
Thread_ERR_t Thread_Login(ThreadCon_t con,BF_INT16U cnt,BF_INT32U frep, void * fun)
{
    BF_INT32U pos = 0u;
    Thread_ERR_t err = BF_ERR_THREAD_NONE;

    ERRR(THREAD_TOTAL >= MAX_THREAD_NUM, return BF_ERR_THREAD_EXHAUSTED); 			//�߳̿ռ�
    if (0 == Thread_Check(0, THREAD_TOTAL, fun))  									//�߳�û�г������̱߳���
    {

        memmove(&thread.thread[thread.runCnt+1], &thread.thread[thread.runCnt],   //����һ���ռ䣬���½��߳�
                sizeof(Thread_t)*(thread.hangCnt));
        pos = thread.runCnt;
        thread.runCnt++;
    }
    else
    {
        Thread_Wake(fun);	//�̻߳���
        pos = Thread_Check(0, THREAD_TOTAL, fun) - 1;
        err = BF_ERR_THREAD_EXIST;
    }
    thread.thread[pos].run_con = con;
    thread.thread[pos].fun = (BF_FNCT_PTR)fun;

    if(ONCEDELAY == con)  															//������ʱ�߳�
    {
        frep = ((frep > 0u)?(frep):(1u));
        cnt = 1;
    }
    thread.thread[pos].flag = 0;
    thread.thread[pos].run_cnt = cnt;
    thread.thread[pos].run_frep = frep;
    thread.thread[pos].tick_ctr = 0u;

    return (err);
}

/*******************************************************************************
����  ��:  Thread_Logout
����˵��:  �߳�ע��
��    ��:  fun: ִ�к�����ָ��
����  ֵ:  ��
*******************************************************************************/
void Thread_Logout(void * fun)
{
    BF_INT32U pos = Thread_Check(0, THREAD_TOTAL, fun);

    if (pos)      													//�ҵ��߳�λ��
    {
        if(pos < THREAD_TOTAL)
        {
            memmove(&thread.thread[pos-1], &thread.thread[pos],	//��ɾȥ��������������еĶ���,����������ǰ��
                    sizeof(Thread_t)*(THREAD_TOTAL - pos));
        }
        thread.runCnt--;
    }
}

/*******************************************************************************
����  ��:  Thread_Hang
����˵��:  �̹߳���
��    ��:  fun: ִ�к�����ָ��
����  ֵ:  ��
*******************************************************************************/
void Thread_Hang(void * fun)
{
    BF_INT32U i = Thread_Check(0, thread.runCnt, fun);

    if (i)     //�߳�λ�������߳̿ռ�
    {

        Thread_t th;

        memcpy(&th, &thread.thread[i-1], sizeof(Thread_t));
        memmove(&thread.thread[i-1], &thread.thread[i],
                sizeof(Thread_t)*(thread.runCnt - i));
        memcpy(&thread.thread[--thread.runCnt], &th, sizeof(Thread_t));
        thread.hangCnt++;
    }
}

/*******************************************************************************
����  ��:  Thread_Wake
����˵��:  �̻߳���
��    ��:  fun: ִ�к�����ָ��
����  ֵ:  ��
ע    ��:  ������ֻ�ܶ������̵߳Ĳ���
*******************************************************************************/
void Thread_Wake(void * fun)
{
    BF_INT32U i = Thread_Check(thread.runCnt, THREAD_TOTAL, fun);

    if (i)     //�ҵ��߳�λ��
    {

        if ((i - thread.runCnt) > 1)     //i���ǽ��������߳�
        {

            Thread_t th;
            memcpy(&th, &thread.thread[i-1], sizeof(Thread_t));
            memmove(&thread.thread[thread.runCnt + 1], &thread.thread[thread.runCnt], //�ڳ��洢�ռ�
                    sizeof(Thread_t)*(i - thread.runCnt));
            memcpy(&thread.thread[thread.runCnt], &th,  sizeof(Thread_t));
        }
        thread.runCnt++;
        thread.hangCnt--;
    }
}

/*******************************************************************************
����  ��:  Thread_Process
����˵��:  �̴߳���
��    ��:  ��
����  ֵ:  ��
*******************************************************************************/
void Thread_Process(void)
{
    Thread_t *pThread = NULL;
    void* fun = NULL;

    CPU_SR_ALLOC();
    for (pThread = &thread.thread[0]; pThread < &thread.thread[thread.runCnt]; pThread++)
    {
        if (pThread->flag)
        {

            ENTER_CRITICAL();
            fun = pThread->fun;
            pThread->flag = 0;
            switch (pThread->run_con)
            {
            case ONCEDELAY:
                pThread->flag = 1;
            case MANY:
            {
                if (!pThread->run_cnt --)
                {

                    Thread_Logout(fun);
                    pThread --;
                    EXIT_CRITICAL();
                    break;
                }
            }
            case FOREVER:
            {

                EXIT_CRITICAL();
                ((BF_FNCT_VOID)(fun))();
            }
            break;
            case RESULT:
            {
                EXIT_CRITICAL();
                if (0 == ((BF_FNCT_PTR)(fun))())   //�߳����з���ʧ��
                {

                    Thread_Logout(fun);
                    pThread --;
                }
            }
            break;
            default :
            {

                EXIT_CRITICAL();
            }
            break;
            }
        }
    }
}

/*******************************************************************************
����  ��:  Thread_RunCheck
����˵��:  �߳����������˶�
��    ��:  ��
����  ֵ:  ��
*******************************************************************************/
void Thread_RunCheck(void)
{
    Thread_t *pThread = NULL;
    CPU_SR_ALLOC();

    ENTER_CRITICAL();
    for (pThread = &thread.thread[0]; pThread < &thread.thread[thread.runCnt]; pThread++)
    {

        if ((++pThread->tick_ctr) >= (pThread->run_frep))
        {

            pThread->tick_ctr = 0;
            pThread->flag = 1;
        }
    }
    EXIT_CRITICAL();
}

/*******************************************************************************
����  ��:   Thread_Check
����˵��:   �̺߳˶�
��    ��:   fun:     �߳���
            start:  ��ʼ�߳�λ��
            end:    ��ֹ�߳�λ��
����  ֵ:   0(û��)/(1~THREAD_UNM)�߳�����λ��
*******************************************************************************/
static BF_INT32U Thread_Check(BF_INT32U start, BF_INT32U end, void *fun)
{
    BF_INT32U i;

    for (i = start; i < end;)
    {

        if (thread.thread[i++].fun == fun)
        {

            return i;
        }
    }
    return 0;
}

/**************************Copyright BestFu 2014-05-14*************************/


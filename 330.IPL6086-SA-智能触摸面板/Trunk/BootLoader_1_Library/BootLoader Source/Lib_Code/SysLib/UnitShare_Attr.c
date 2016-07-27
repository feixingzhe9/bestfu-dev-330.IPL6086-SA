/***************************Copyright BestFu 2014-05-14*************************
��	����	UnitShare_Attr.c
˵	����	����Ԫ��������ʵ�ֺ���
��	�룺	Keil uVision4 V4.54.0.0
��	����	v1.0
��	д��	Unarty
��	�ڣ�	2014-04-08
�ޡ��ģ�	Unarty 2014.10.28,�޸����е�Ԫ��֤�ӿ��쳣Bug��
			2014.12.28 Unarty �޸ĵ�Ԫ����ִ�б߽������ж�ֵ����ֹ�൥Ԫִ��ʱ����Խ��
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "PivotalDataType.h"
#include "BestFuLib.h"
#include "Thread.h"
#include "Instruct.h"
#include "UserData.h"
#include "fifo.h"
#include "cmdProcess.h"
#include "UnitCfg.h"
#include "UnitShare_Attr.h"

/*******************************************************************************
�� �� ��:  	Set_ManyUnit_Attr
����˵��:  	���ö����Ԫ����
��    ��:  	data[0]: Ŀ�굥Ԫ
			data[1]: ��������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
MsgResult_t Set_ManyUnit_Attr(UnitPara_t *pData)
{
    if (pData->len > 0)  	//�˶Բ�������
    {
        MsgResult_t result;
        BF_INT08U buf[128];
        BF_INT08U paraLen = pData->len;	//��������
        BF_INT08U *pPara = (BF_INT08U*)&pData->data;	//������ָ��

        while (pPara < &pData->data[paraLen])   //������û��ʹ����
        {
            if (pPara[2] > 128)  	//������Բ������ȴ���128
            {
                return PARA_LEN_ERR;
            }
            buf[0] = WRITEWITHACK;
            memcpy(&buf[1], pPara, (3 + pPara[2]));
            result = Msg_Execute((CMDPara_t*)buf, pData->msgsrc);

            if (result != COMPLETE)  	//ָ��ִ�в���ȷ
            {
                *(BF_INT16U*)&pData->data[0] = *(BF_INT16U*)&buf[1];  //��ֵ��Ԫ�������Ժ�
                *(BF_INT08U*)&pData->data[2] = (BF_INT08U)result;
                pData->len = 3;
                return OWN_RETURN_PARA;
            }
            pPara += (3 + pPara[2]);
        }
        return COMPLETE;
    }

    return PARA_LEN_ERR;
}

/*******************************************************************************
�� �� ��:  	Get_ManyUnit_Attr
����˵��:  	��ȡ�����Ԫ����ֵ
��    ��:  	pData->unit:	������Ԫ
			pData->cmd:		����ָ��
			pData->len��	��������
			pData->data:	��������
			rLen:	���ز�������
			rpara:	���ز�������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
MsgResult_t Get_ManyUnit_Attr(UnitPara_t *pData, BF_INT08U *rlen, BF_INT08U *rpara)
{
    *rlen = 0;	//�������ݳ�������

    if (pData->len > 0)  	//��ȡĳЩ��Ԫ������
    {
        BF_INT08U para[255]; //������
        BF_INT08U paraLen = pData->len;
        BF_INT08U *pPara = &para[paraLen];	//������ָ��
        BF_INT08U *aimPara = para;			//�´���Ϣ����

        memcpy(aimPara, pData->data, paraLen);

        while (aimPara < &para[paraLen])
        {
            ((CMDPara_t*)pPara)->msgType = READWITHACK;
            ((CMDPara_t*)pPara)->unit = *aimPara++;
            ((CMDPara_t*)pPara)->cmd = *aimPara++;
            ((CMDPara_t*)pPara)->len = 0;						//�����´���
            if (COMPLETE == Msg_Execute((CMDPara_t*)pPara, pData->msgsrc))  	//�����Ϣ���
            {
                rpara[(*rlen)++] = ((CMDPara_t*)pPara)->unit;	//��Ԫ��
                rpara[(*rlen)++] = ((CMDPara_t*)pPara)->cmd;	//���Ժ�
                rpara[(*rlen)++] = --((CMDPara_t*)pPara)->len; 	//�������ȣ�����Ϊÿ�λ�ȡ����ǰ���һ������Ϊ������ִ�н������������жϣ������ڴ��Ƴ���
                memcpy(&rpara[*rlen], &pPara[sizeof(CMDPara_t)+1], ((CMDPara_t*)pPara)->len); //����ȡ�ĵ�Ԫ�������ݷ��õ����ز�������
                *rlen += ((CMDPara_t*)pPara)->len;
                if (*rlen > MAX_ATTR_NUM)  	//�����ȡ�ķ��ز������ݱȽϴ�
                {
                    break;
                }
            }
        }
    }
    else     // ��ȡĳһ����Ԫ����ֵ
    {
        BF_INT08U data[5],ExeAttrNum = 0u; 		//������
        const AttrExe_st *pAttrExe = UnitTab[pData->unit].attrExe;
        do
        {
            if (LEVEL_1 == pAttrExe->level)  	//������Եȼ��ϸ�
            {
                data[0] = pData->msgsrc;	//��ϢԴ
				data[1] = pData->unit;		//��Ԫ
                data[2] = pAttrExe->cmd;	//����
                data[3] = 0;				//��������
                if ((pAttrExe->pGet_Attr != NULL)
                        && (COMPLETE == pAttrExe->pGet_Attr((UnitPara_t*)data, &data[4], &rpara[(*rlen)+3]))	//�����ȡ�����������
                   )
                {
                    rpara[(*rlen)++] = pData->unit;		//��Ԫ
                    rpara[(*rlen)++] = pAttrExe->cmd;	//����
                    rpara[(*rlen)++] = data[4];			//��������
                    *rlen += data[4];

                    if (*rlen > MAX_ATTR_NUM)  		//�����ȡ�ķ��ز������ݱȽϴ�
                    {
                        break;
                    }
                }
            }
        }
        while(((++ExeAttrNum) < UnitTab[pData->unit].ExeAttrNum())    //2016.01.29 jay modify
                &&((++pAttrExe)->cmd < 0x0100));
    }
    return COMPLETE;
}

/*******************************************************************************
�� �� ��:  	Set_UnitChecking_Attr
����˵��:  	���õ�Ԫ��֤
��    ��:  	data[0]: Ŀ�굥Ԫ
			data[1]: ��������
�� �� ֵ:  	��Ϣִ�н��
*******************************************************************************/
MsgResult_t Set_UnitChecking_Attr(UnitPara_t *pData)
{
    BF_INT32U i;

    i = UnitCnt_Get();	//��ȡ��Ч��Ԫ��

    if (1 != pData->len)
    {
        return PARA_LEN_ERR;	//�������ȴ���
    }
    else if (pData->data[0] >= i)
    {
        return PARA_MEANING_ERR;//�����������
    }

    if (0 == pData->data[0])  	//��֤���е�Ԫ
    {
        for (i = UnitCnt_Get(); i > 0;)
        {
            i--;
            if (UnitTab[i].Instruct != NULL)
            {
                UnitTab[i].Instruct(); //ע�ᵥԪ��֤
            }
        }
    }
    else
    {
        if (UnitTab[pData->data[0]].Instruct != NULL)
        {
            UnitTab[pData->data[0]].Instruct(); //ע�ᵥԪ��֤
        }
    }

    Thread_Login(MANY, 200, 50, &Instruct_Run);	//ע���豸��֤

    return COMPLETE;
}

/**************************Copyright BestFu 2014-05-14*************************/

/***************************Copyright BestFu 2016-04-16*************************
��	����	DevPartTab.h
˵	����	�豸�����������ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2016-04-16
��  ��:     ����
*******************************************************************************/
#include "lib_type.h"
#include "lib_config.h"
#include "program.h"
#include "DevPartTab.h"

//�豸�ؼ���Ϣ��������Ϣ
const DevPivotalSet_t DevPivotalSet[3] =
{
    /*    Ӳ����Ϣ ����0��־��  �����ַ   flash����  �����Ϣ   433��Ϣ*/
    {0x080002F0,0x08000400,0x08000430,0x0800043C,0x08005000,0x08005040},	//64K������Ϣ
    {0x080002F0,0x08000400,0x08000430,0x0800043C,0x08006000,0x08006040},	//128K������Ϣ
    {0x080002F0,0x08000800,0x08000830,0x0800083C,0x08006800,0x08006840}		//>=256������Ϣ
};

static DevPivotalSet_t* pDevPivotalSet = NULL;	//�����ȡ�豸�ؼ���Ϣ��ָ��
/*******************************************************************************
�� �� ���� 	DevPivotalSetInit
����˵����  �豸�ؼ���Ϣ�ĳ�ʼ��
��	  ��:   ��
�� �� ֵ��  ��
*******************************************************************************/
void DevPivotalSetInit(void)
{
    BF_INT16U chip_size = Flash_ChipSize();

    if((64   != chip_size)&&(128 != chip_size)&&\
            (256  != chip_size)&&(384 != chip_size)&&\
            (512  != chip_size)&&(768 != chip_size)&&\
            (1024 != chip_size))
    {
        chip_size = (DevPartTab->flash_end_addr - DevPartTab->boot0_start_addr)/1024;
    }
    switch(chip_size)
    {

    case 64:
    {
        pDevPivotalSet  = ((DevPivotalSet_t*)&DevPivotalSet[0]);
    }
    break;
    case 128:
    {
        pDevPivotalSet  = ((DevPivotalSet_t*)&DevPivotalSet[1]);
    }
    break;
    case 256:
    case 384:
    case 512:
    case 768:
    case 1024:
    {
        pDevPivotalSet  = ((DevPivotalSet_t*)&DevPivotalSet[2]);
    }
    break;
    default:
    {
        pDevPivotalSet  = NULL;
    }
    break;
    }
}

/*******************************************************************************
�� �� ���� 	GetDevPivotalSet
����˵����  ��ȡ�豸�ؼ���Ϣ��ָ��
��	  ��:   ��
�� �� ֵ��  ��ȡ�豸�ؼ���Ϣ��ָ��
*******************************************************************************/
DevPivotalSet_t* GetDevPivotalSet(void)
{
    return ((DevPivotalSet_t*)(pDevPivotalSet));
}
/**************************Copyright BestFu 2016-04-16*************************/

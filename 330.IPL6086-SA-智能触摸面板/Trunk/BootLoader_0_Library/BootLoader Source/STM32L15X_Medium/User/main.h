/***************************Copyright BestFu 2016-04-21 *************************
��	����	main.h
˵	����	ϵͳ������ͷ�ļ�
��	�룺	uVision V5.12.0
��	����	v1.0
��	д��	jay
��	�ڣ�	2016-04-21 
�ޡ��ģ�	����
*******************************************************************************/

/*********************************�Զ����*************************************/
#define DEVICE_PARTION_TABLE_ADDR	((unsigned int)(0x08000380))		//�豸���������ʼ��ַ
#define BOOTLOADER_0_JUMP_FLAG		((unsigned int)(0xAA788755))		//���bootloader0����ת����

/********************************���ݽṹ����**********************************/ 
typedef struct		//bootloader_0_flag����Ϣ
{
	unsigned int bootloader_0_flag;		//Bootloader_0������ת���
	unsigned int bootloader_1_Softver;	//bootloader1����汾
	unsigned int bootloader_1_AppSize;	//bootloader1��״�С
}Bootloader0Info_t;

typedef struct		//�豸������
{
	unsigned int boot0_start_addr;			//bootloader0����ʼ��ַ
	unsigned int boot0_flag_start_addr;		//bootloader0_flag����ʼ��ַ
	unsigned int boot1_start_addr;			//bootloader1����ʼ��ַ
	unsigned int boot1_flag_start_addr;		//bootloader1_flag����ʼ��ַ
	unsigned int user_app_start_addr;		//�û�app����ʼ��ַ
	unsigned int user_data_start_addr;		//�û��������ı�����ַ
	unsigned int flash_end_addr;			//�û�flash�Ľ�����ַ
}DevPartionTable_t;


#define DevPartTab			((DevPartionTable_t*)(DEVICE_PARTION_TABLE_ADDR))	//�豸������ĵ�ַ��Ϣ
/**************************Copyright BestFu 2016-04-21*************************/

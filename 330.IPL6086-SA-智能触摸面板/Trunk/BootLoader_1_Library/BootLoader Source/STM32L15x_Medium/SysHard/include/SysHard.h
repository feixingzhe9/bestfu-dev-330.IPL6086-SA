/***************************Copyright BestFu 2014-05-14*************************
��	����    SysHard.h
˵	����    ϵͳӲ����غ���
��	�룺    Keil uVision4 V4.54.0.0
��	����    v2.0
��	д��    Unarty
��	�ڣ�    2014.06.26
�ޡ��ģ�	����
*******************************************************************************/
#ifndef __SYSHARD_H
#define __SYSHARD_H

/*********************************�Զ����*************************************/
#define STATUS0(GPIOx, pin)  GPIO_ResetBits(GPIOx, pin)	//��Ӧ�ܽ���0
#define STATUS1(GPIOx, pin)  GPIO_SetBits(GPIOx, pin)  //��Ӧ�ܽ���1 
#define STATUS_(GPIOx, pin)  GPIO_ToggleBits(GPIOx, pin) //��Ӧ�ܽ�ȡ��

//EEPROM IICͨ�Žӿ�
#define SCL_PORT    		GPIO_B      //SCL�˿�
#define SCL_PIN    			GPIO_Pin_6  //SCL���� 
													    
#define SDA_PORT    		GPIO_B      //SDA�˿�
#define SDA_PIN     		GPIO_Pin_7  //SDA����

#define EEPROM_WP			(0) 		//EEPROMд������ 1: ��дʹ�ܱ����ܽţ�0����дʹ�ܱ����ܽ�

#if EEPROM_WP
	#define WP_PORT			GPIO_B		//Write Protect port
	#define WP_PIN			GPIO_Pin_5	//Write Protect pin
#endif 


//�������ģʽ����
#define TEST_KEY_PORT 		GPIO_A
#define TEST_KEY_PIN  		GPIO_Pin_7
#define TEST_VALID_LEVEL	(0)			//��Ч��ƽ
#if TEST_VALID_LEVEL == 1
	#define TEST_KEY_INIT 	GPIO_Config(TEST_KEY_PORT, TEST_KEY_PIN, IN_DOWN);
	#define TEST_KEY_VAL	(1)//(GPIO_ReadInputDataBit(TEST_KEY_PORT, TEST_KEY_PIN))
#else
	#define TEST_KEY_INIT 	GPIO_Config(TEST_KEY_PORT, TEST_KEY_PIN, IN_UP);
	#define TEST_KEY_VAL	(1)//(GPIO_ReadInputDataBit(TEST_KEY_PORT, TEST_KEY_PIN))
#endif


//�û��������ö˿ڶ��� : ע���ڲ���⣬RESET_KEY_VAL ���أ�����Ч�� ������Ч
#define RESET_USER_DATA			(0)   //1�����������û����ݰ�ť��0���ر������û����ݰ�ť
#if RESET_USER_DATA
	#define RESET_KEY_PORT 		GPIO_C
	#define RESET_KEY_PIN  		GPIO_Pin_14
	#define RESET_VALID_LEVEL	(0)		//��Ч��ƽ
	#if (RESET_VALID_LEVEL == 1)
		#define RESET_KEY_INIT 	GPIO_Config(RESET_KEY_PORT, RESET_KEY_PIN, IN_DOWN);							
		#define RESET_KEY_VAL	(GPIO_ReadInputDataBit(RESET_KEY_PORT, RESET_KEY_PIN))
	#else
		#define RESET_KEY_INIT 	GPIO_Config(RESET_KEY_PORT, RESET_KEY_PIN, IN_UP);					
		#define RESET_KEY_VAL	(GPIO_ReadInputDataBit(RESET_KEY_PORT, RESET_KEY_PIN))
	#endif
#endif


//ϵͳ����ָʾ�ƶ˿�
#define RUN_INSTRUCT		(0)   //1����������ָʾ��0���ر�����ָʾ
#if RUN_INSTRUCT
	#define RUN_PORT		GPIO_A    
	#define RUN_PIN			GPIO_Pin_8
	#define RUN_ON			STATUS0(RUN_PORT, RUN_PIN)
	#define RUN_OFF			STATUS1(RUN_PORT, RUN_PIN)
#endif

//�ڲ����Ź�����
#define INTERNAL_IWDG_EN	(1u)		//1:�����ڲ����Ź�;0���ر��ڲ����Ź�

//�ⲿ���Ź��˿�
#define EXTERNAL_WDG_EN		(0u) 		//1�������ⲿ���Ź���0���������ⲿι��
#if (EXTERNAL_WDG_EN > 0u)
	#define	WDG_PORT		GPIO_A			//Ӳ��ι���˿�
	#define WDG_PIN			GPIO_Pin_12		//Ӳ��ι������
#endif

//433 ͨ�Žӿ�
#define nGPIO2_PORT     	GPIO_B			//ͨ��æ�����˿�
#define nGPIO2_PIN      	GPIO_Pin_1 		//ͨ��æ��������

#define SDN_PORT        	GPIO_A			//433��λ�˿�
#define SDN_PIN         	GPIO_Pin_6		//433��λ����

#define nIRQ_PORT       	GPIO_A  		//�����ж϶˿�  ע�⣺�ж�IO�޸ĺ���Ҫ�޸��ڲ���һЩ����
#define nIRQ_PIN        	GPIO_Pin_10		//�����ж�����

#define nSEL_PORT       	GPIO_B  		//SPIƬѡ�˿�
#define nSEL_PIN        	GPIO_Pin_12		//SPIƬѡ����

#define RF_SWITCH			(1)				//���ߵ�Դ���� 1:(�����е�Դ����), 0(����û�п���)
#if RF_SWITCH
#define RF_SWITCH_PORT		GPIO_A			//���ߵ�Դ���ض˿�
#define RF_SWITCH_PIN		GPIO_Pin_11		//���ߵ�Դ��������
#define RF_SWITCH_ON		STATUS1(RF_SWITCH_PORT, RF_SWITCH_PIN)	//���ߵ�Դ��
#define RF_SWITCH_OFF		STATUS0(RF_SWITCH_PORT, RF_SWITCH_PIN)	//���ߵ�Դ��
#endif

#define SI4432_CH			(0)				//����433�����ŵ�: 86 / 85 / 84 / 83 / 82 / 81 //������ʹ�����������ֵ
#if (SI4432_CH < 81 || SI4432_CH > 86)
	#undef SI4432_CH
#endif

#define SI4432_THRESHOLD 	(0)	//����433�����ŵ��ż���Threshold��: 60 �� 220. ��ע�� ��0 ʹ���豸�Զ��ż����ã�
#if (SI4432_THRESHOLD == 0)
	#undef SI4432_THRESHOLD 
#endif

//433�����ź�ָʾ�˿�*/
#define SIGNAL				(0)		//�Ƿ���Ҫ�ź�ָʾ���� 1: ��Ҫ�� 0������Ҫ
#define SIGNAL_PORT	GPIO_A
#define SIGNAL_PIN	GPIO_Pin_11
#define SIGNAL_ON	STATUS0(RUN_PORT, RUN_PIN)
#define SIGNAL_OFF	STATUS1(RUN_PORT, RUN_PIN)

/***********************************��������***********************************/
void SysHard_Init(void);
BF_INT32U HardID_Check(BF_INT32U id);
#if (DEVICE_ADDR_RESET_FEATURE_EN > 0u)
void HardID_Change(void);
#endif

#endif 		//SysHard.h end 
/**************************Copyright BestFu 2014-05-14*************************/

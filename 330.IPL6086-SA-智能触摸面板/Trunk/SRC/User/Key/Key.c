/***************************Copyright BestFu ***********************************
**  ��    ����  Key.c
**  ��    �ܣ�  <<�߼���>> �������� 
**  ��    �룺  Keil uVision5 V5.10
**  ��    ����  V1.1
**  ��    д��  Seven
**  �������ڣ�  2014.08.20
**  �޸����ڣ�  2014.08.20
**  ˵    ����  
**  V1.1
    >> �̰���Ϊ�ͷ�ʱ��ִ�У�����ӳ�������Ӧ
*******************************************************************************/
#include "Key.h"
#include "LinkAll.h"
#include "delay.h"
#include "LowPower.h"
#include "WTC6216.h"
#include "USART1.h"
#include "MsgPackage.h"
#include "ShakeMotor.h"
#include "OLED.h"
#include "ShowLED.h"
#include "TouchPadPic.h"
#include "AirConditionPad.h"
#include "Queue.h"
#include "TIMx.h"
#include "sys.h"
//#include "stdio.h"

//Public Variable definitions
u8 gPassword[4] = {0};									//������ʵ������洢����
u8 gPswFlag = 0u; 										//���ڼ�¼��������������ֵ

//Private Variable definition
static const u8  KeyMapTab[7] = {0,31,59,62,47,55,61};	//����ӳ��� ���16������,��λ��0����
static u8 InputPassword[4] = {0};						//�����û����������ֵ��������
static Queue_t  Queue;									//���尴�����ն���
static u8 PswBit = 0; 									//��¼�����������λ��

//Private function declaration
static void CheckUserPassWordInput(u8 keynum);

/*******************************************************************************
**��    ���� Key_Init()
**��    �ܣ� ����ģ���ʼ��
**��    ���� void
**��    �أ� void
*******************************************************************************/
void Key_Init(void)
{
	WTC6106_Init();
    InitQueue(&Queue);
}

/*******************************************************************************
**��    ���� void KeyTimer_Init(void)
**��    �ܣ� ������ʱ��ģ���ʼ��
**��    ���� void
**��    �أ� void
*******************************************************************************/
void KeyTimer_Init(void)
{
	u32 count = 0xffffffff;
	while((Get_TouchFlag())||(0 == count))
	{
		count--;
	} 
	TIMx_Init(TIM2 ,32, 6399);		//��ʼ���ж�Ƶ��5ms
}

/*******************************************************************************
**��    ���� void KeyStateScan(void)
**��    �ܣ� ����״̬ɨ��
**��    ���� void
**��    �أ� void
*******************************************************************************/
void KeyStateScan(void)
{
	KeyCurStatus_t keyTemp;
	
	keyTemp.KeyValue=Key_Scan((u8*)&keyTemp.KeyType , 0);	
	if((NONE_KEY != keyTemp.KeyValue))                 
	{                
		CPU_SR_ALLOC();
		cpu_sr = CPU_SR_Save();
		Enqueue(&Queue,keyTemp);
		CPU_SR_Restore(cpu_sr);
		ShowLED_FlashLED(keyTemp.KeyValue);
	}
}

/*******************************************************************************
**��    ���� Key_Scan()
**��    �ܣ� ״̬����ʽɨ������
**��    ���� *keyFlag    ������־    0-�ް���  1-�̰���    2-������ 
**��    �أ� ����ֵ
**˵    ���� �ú��� ÿ�� 5~20ms ����һ��
*******************************************************************************/
u8 Key_Scan(u8 *keyFlag,u8 keyAccuracy)
{
    u8 i;
    static u8 MidValidKey = NONE_KEY;       //��Ч�����м䱣��ֵ
	static u8  NewKey = NONE_KEY;
	static u8  OldKey = NONE_KEY;
	static u8  LstKey = NONE_KEY;
	static u16 LongKeyCount = 0;            //������������
	
    *keyFlag = 0;
    NewKey = WTC6106_ReadKey();
    //OLED_ShowNumber(0,1,NewKey,3,0);
    if(NewKey == OldKey)
    {
        if(NewKey==NONE_KEY)                                        //�ް��� �� �ͷŰ���    
        {   
            if((LstKey != NONE_KEY)&&(LongKeyCount<LONG_KEY_TIMES)) // �ͷŰ��� �� �ǳ�����
            {
                if(LongKeyCount > keyAccuracy)  					//���˳��̰��� �����ȿɵ�
                {
                    LstKey  = NONE_KEY;
                    LongKeyCount = 0;
                    *keyFlag =1;
                    return MidValidKey;                             //���ض̼��ͷ�
                }
            }
            else{                                                   
                LstKey = NONE_KEY;
                LongKeyCount = 0;
                return NONE_KEY;                    
            }
        }
        else if(NewKey==LstKey)                                     //��������
        {
            if(LongKeyCount++ == LONG_KEY_TIMES)
            {
                *keyFlag =2;
                return MidValidKey;                                 //���س���ȷ��
            }
        }
        else{                                                       //����
            LstKey = NewKey;
            for(i=1 ; i<=MAX_KEY_NUM ; i++)
            {
                if( KeyMapTab[i] == NewKey )  
                {
                    MidValidKey = i;
                    break;
                }
            }
            if( i > MAX_KEY_NUM)              MidValidKey = NONE_KEY;
            return NONE_KEY ;         
        }
    }
    else OldKey = NewKey;               //��������
    return NONE_KEY;
}


/*******************************************************************************
**��    ���� Key_Handle()
**��    �ܣ� ��������
**��    ���� void 
**��    �أ� void
**˵    ���� �ú��� ÿ�� 5~20ms ����һ��
*******************************************************************************/
void Key_Handle(void)
{	
    KeyCurStatus_t keyTemp;
    u8 result = 0x00;
	
	CPU_SR_ALLOC();
    cpu_sr = CPU_SR_Save();
    result = Dequeue(&Queue,&keyTemp);
    CPU_SR_Restore(cpu_sr);	
	
    if(result)
    {   
//        key_flag = keyTemp.KeyType;           
        if(gPswFlag > 0u)        //���뿪��
		{
			CheckUserPassWordInput(keyTemp.KeyValue);
		}
		else if(NONE_KEY != keyTemp.KeyValue)
		{    
			StandbyCountReset();
			if(2 == keyTemp.KeyValue)   //�յ���Դ����
			{
				if(gAirContionData.PowerSwitch == AC_POWER_ON)
				{
					gAirContionData.PowerSwitch = AC_POWER_OFF;
					OLED_ClearScreen();
					OLED_ShowHalfPicAt(4,PicTab[19]);           //�յ��ر�ͼƬ
					AirCondition_SendData();
				}
				else 
				{
					gAirContionData.PowerSwitch = AC_POWER_ON;
					AirCondition_FullShow();
				}
			}
			else		//�յ������������ (������Դ����)
			{
				if(gAirContionData.PowerSwitch == AC_POWER_ON)
				{
					AirCondition_KeyHander(keyTemp.KeyValue); 
				}
				else if(gAirContionData.PowerSwitch == AC_POWER_OFF)  //��Դ��
				{
//                        OLED_ClearScreen();
//                        OLED_ShowHalfPicAt(4,PicTab[19]);           //�յ��ر�ͼƬ
					
//                        gAirContionData.PowerSwitch = AC_POWER_ON;
//                        AirCondition_FullShow();
//                        AirCondition_KeyHander(ch);
					gAirContionData.PowerSwitch = AC_POWER_ON;
					AirCondition_FullShow();
				}
			}
		   Upload(1); 
		}
    }
}

/*******************************************************************************
**��    ���� static void CheckUserPassWordInput(u8 keynum)
**��    �ܣ� �û����봦����
**��    ���� keynum���û�����İ���ֵ 
**��    �أ� void
**˵    ���� �ú��� ÿ�� 5~20ms ����һ��
*******************************************************************************/
static void CheckUserPassWordInput(u8 keynum)
{
	static u8 FirstPassWord = 0u;  
	
	switch(FirstPassWord)
	{
		case 0:
		{
			FirstPassWord = 1;
			OLED_ClearScreen();
			OLED_ShowString(2,0,"����������",0);
			OLED_ShowString(6,2,"----",0);
		}
		break;
		case 1:
		{
			if(keynum <= 6)      //123456������Ч
			{
				StandbyCountReset();
				OLED_ShowChar(6+PswBit,2,keynum + '0',0);
				InputPassword[PswBit] = keynum;
				if(PswBit++ >= 3)
				{
					PswBit=0;
					if((gPassword[0] == InputPassword[0])&& \
					   (gPassword[1] == InputPassword[1])&& \
					   (gPassword[2] == InputPassword[2])&& \
					   (gPassword[3] == InputPassword[3]) )
					{
						gPswFlag = 0;
						OLED_ClearScreen();
//                      OLED_ShowString(4,1,"��ӭʹ��",0);
						AirCondition_FirstShowPicture();
					}
					else 
					{
						OLED_ShowString(1,0,"��������������",0);
						OLED_ShowString(4,1,"�������",0);
						OLED_ShowString(6,2,"----",0);
					}
				}
			}
		}
		break;
		default:break;
	}
}

/*******************************************************************************
**��    ��:  Key_FirstScan()
**��    ��:  ��������ɨ��
**��    ��:  void
**��    ��:  �״���Ч����ֵ 
*******************************************************************************/
u8 Key_FirstScan(void)
{
    u8 i;
    u8 first_key = NONE_KEY;
    u8 temp_key  = NONE_KEY;
    first_key = WTC6106_ReadKey();
    delay_us(10);                         /**> ʱ�������������󴥷� **/
	
    if(first_key == WTC6106_ReadKey())
    {
        for(i=1 ; i<=MAX_KEY_NUM ; i++)
        {
            if( KeyMapTab[i] == first_key )  
            {
                temp_key = i;
                break;
            }
        }
        if( i > MAX_KEY_NUM)              
			temp_key = NONE_KEY;           
    }
    return temp_key;
}

/***************************Copyright BestFu **********************************/


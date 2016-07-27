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
#include "sys.h"
//#include "stdio.h"


/*��������ֵ*/
const u8 NONE_KEY=0xFF;       
static u8 FirstPassWord = 0;  
/*��������ֵ--�ṩ�����Բ�ʹ��*/
KeyProp_t  KeyCh[MAX_TOUCH_KEY];        //KeyCh[0] ����

static u8  NewKey,OldKey,LstKey;
static u16 LongKeyCount;                //������������
const  u16 LONG_KEY_TIMES = 100;        //����������ʱ�� 200 *5ms = 1S
//extern u8 Sleep_key;

#define MAX_KEY_NUM 12
//����ӳ��� ���16������   ��λ��0����
//˳�򰴼� ƥ��� ʵ�ʰ���ֵ
//                             1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
const u8  KeyMapTab[17] = {0,  31,59,62,47,55,61,0, 0,0, 0,0, 0, 0, 0, 0, 0};
//const u8  KeyMapTab[17] = {0,  1, 2, 3, 4, 5, 6, 8, 8, 9,10,11,12,13,14, 0, 0};

u8 gPassword[4];
u8 gPswFlag=1;
    
u8 InputPassword[4];
u8 PswBit=0;

Queue_t  gQueue;
    
/*******************************************************************************
**��    ���� Key_Init()
**��    �ܣ� ����ģ���ʼ��
**��    ���� void
**��    �أ� void
*******************************************************************************/
void Key_Init(void)
{
    WTC6106_Init();
    NewKey=NONE_KEY;
    OldKey=NONE_KEY;
    LstKey=NONE_KEY;
}
/*******************************************************************************
**��    ���� Key_Scan()
**��    �ܣ� ״̬����ʽɨ������
**��    ���� *keyFlag    ������־    0-�ް���  1-�̰���    2-������ 
**��    �أ� ����ֵ
**˵    ���� �ú��� ÿ�� 5~20ms ����һ��
*******************************************************************************/
u8 Key_Scan(u8 *keyFlag)
{
    u8 i;
    static u8 MidValidKey = NONE_KEY;                               //��Ч�����м䱣��ֵ
	*keyFlag = 0;
	NewKey = WTC6106_ReadKey();
    if(NewKey == OldKey)
    {
        if(NewKey==NONE_KEY)                                        //�ް��� �� �ͷŰ���    
        {   
            if((LstKey != NONE_KEY)&&(LongKeyCount<LONG_KEY_TIMES)) // �ͷŰ��� �� �ǳ�����
            {
                //if(LongKeyCount>2)  //���˳��̰��� �����ȿɵ�
                {
                    LstKey  = NONE_KEY;
                    LongKeyCount = 0;
                    *keyFlag = KEY_SHORT;
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
                *keyFlag = KEY_LONG;
				LongKeyCount = 0;
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
**��    ���� Key_FirstHandleScan()
**��    �ܣ� ״̬����ʽɨ������
**��    ���� *keyFlag    ������־    0-�ް���  1-�̰���    2-������ 
**��    �أ� ����ֵ
**˵    ���� �ú��� ÿ�� 5~20ms ����һ��
*******************************************************************************/
u8 Key_FirstHandleScan(u8 *keyFlag)
{
    u8 i;
    static u8 MidValidKey = NONE_KEY;                               //��Ч�����м䱣��ֵ
	*keyFlag = 0;
	NewKey = WTC6106_ReadKey();
    if(NewKey == OldKey)
    {
        if(NewKey==NONE_KEY)                                        //�ް��� �� �ͷŰ���    
        {   
            if(LstKey != NONE_KEY) // �ͷŰ��� 
            {
                if(LongKeyCount>4)  //���˳��̰��� �����ȿɵ�
                {               
                    LstKey  = NONE_KEY;
                    LongKeyCount = 0;
                    *keyFlag = KEY_LONG;
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
            LongKeyCount++ ;
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
    u8 i,ch;
    static u8 FirstFlag = 1;
//    u8 key_flag;
    u8 res = 0x00;
    KeySta_t keyTemp;
    CPU_SR_ALLOC();
    cpu_sr = CPU_SR_Save();
    res = Dequeue(&gQueue,&keyTemp);
    CPU_SR_Restore(cpu_sr);
    if(res)
    {   
        ch = keyTemp.KeyValue;
//        key_flag = keyTemp.KeyType;
        if(FirstFlag)
        {
            FirstFlag = 0;
            StandbyCountReset();
//          AirCondition_FirstShowPicture();
        }
        else
        {           
            if(gPswFlag)        //���뿪��
            {
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
                        if(ch <= 6)      //123456������Ч
                        {
                            StandbyCountReset();            
                            OLED_ShowChar(6+PswBit,2,ch+'0',0);
                            InputPassword[PswBit] = ch;
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
    //                                OLED_ShowString(4,1,"��ӭʹ��",0);
                                    
                                    FirstFlag = 0;
                                    StandbyCountReset();
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
            else  if((ch>0)&&(ch<MAX_TOUCH_KEY))   //!= NONE_KEY
            {
                /*����ϴ�״̬*/
                for(i=0;i<MAX_TOUCH_KEY;i++)
                {
                    KeyCh[i].Status = KEY_NONE; 
                }    
                StandbyCountReset();
                if(ch == 2)   //�յ���Դ����
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
                else if(ch <= 6)   //�յ������������ (������Դ����)
                {
                    if(gAirContionData.PowerSwitch == AC_POWER_ON)
                    {
                        AirCondition_KeyHander(ch); 
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
               Upload(); 
            }
        }
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
    delay_us(10);                     /**> ʱ�������������󴥷� **/
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
        if( i > MAX_KEY_NUM)              temp_key = NONE_KEY;           
    }
    return temp_key;
}
#if 0
/*******************************************************************************
**��    ��:  Key_FirstHandle()
**��    ��:  �״ΰ���������
**��    ��:  ����ֵ
**��    ��:  void
********************************************************************************/
void Key_FirstHandle(u8 Key)
{
    if(gPswFlag)
    {
        OLED_ShowString(2,0,"����������",0);
        OLED_ShowString(6,2,"----",0);
    }
    else if( Key != NONE_KEY)
    {   
        gUIMode = UIMODE_AIRCON;            
        AirCondition_FullShow();
        Upload(); //�ϱ�         
    }
}
#endif 

/***************************Copyright BestFu **********************************/


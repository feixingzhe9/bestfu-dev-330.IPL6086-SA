/***************************Copyright xxxxx 2015-12-30**************************
**  ��    ����  iwdg.h
**  ��    �ܣ�  �������Ź��ײ���������ͷ�ļ�
**  ��    �룺  MDK-ARM Professional Versoin: 5.12.0.0
**  ��    ����  V1.0
**  ��    д��  Jay
**  �������ڣ�  2015-12-30
**  �޸����ڣ�  ��
**  ˵    ����  ����
*******************************************************************************/
#ifndef __IWDG_H
#define __IWDG_H

#ifdef __cplusplus
 extern "C" {
#endif

/********************************�Զ����*************************************/	
//defgroup IWDG_prescaler 
#define IWDG_Prescaler_4            ((uint8_t)0x00)
#define IWDG_Prescaler_8            ((uint8_t)0x01)
#define IWDG_Prescaler_16           ((uint8_t)0x02)
#define IWDG_Prescaler_32           ((uint8_t)0x03)
#define IWDG_Prescaler_64           ((uint8_t)0x04)
#define IWDG_Prescaler_128          ((uint8_t)0x05)
#define IWDG_Prescaler_256          ((uint8_t)0x06)


/******************************��������***************************************/
void IWDG_Config(BF_INT08U IWDG_Prescaler,BF_INT16U Reload);
void IWDG_Feed(void);
void IWDG_FeedOFF(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IWDG_H */
/********************************End of File**********************************/

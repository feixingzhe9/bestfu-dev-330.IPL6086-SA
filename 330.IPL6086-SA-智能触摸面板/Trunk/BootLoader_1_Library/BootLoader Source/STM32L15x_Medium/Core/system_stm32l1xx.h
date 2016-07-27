/***************************Copyright xxxxx 2015-12-29*************************
**  ��	����system_stm32l1xx.h
**  ˵	����ϵͳʱ�ӳ�ʼ�����Դ����ͷ�ļ�
**  ��	�룺uVision V5.12.0.0
**  ��	����V1.0
**  ��	д��Jay
**  ��	�ڣ�2015/12/29
**  ��	�ģ�����
*******************************************************************************/
#ifndef __SYSTEM_STM32L1XX_H
#define __SYSTEM_STM32L1XX_H

#ifdef __cplusplus
 extern "C" {
#endif 

/*******************************���ݽṹ����**********************************/
typedef struct
{
  uint32_t SYSCLK_Frequency;  //ϵͳʱ��Ƶ��
  uint32_t HCLK_Frequency;    //HCLKʱ��Ƶ��
  uint32_t PCLK1_Frequency;   //PCLK1ʱ��Ƶ��
  uint32_t PCLK2_Frequency;   //PCLK2ʱ��Ƶ��
  uint32_t ADCCLK_Frequency;  //ADCʱ��Ƶ��
}RCC_ClocksTypeDef;

/*******************************��������**************************************/
extern void SystemInit(void);
extern void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks);

#ifdef __cplusplus
}
#endif
#endif /*__SYSTEM_STM32L1XX_H */
/**********************************End of File*********************************/

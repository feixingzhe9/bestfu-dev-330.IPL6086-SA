/***************************Copyright xxxxx 2015-12-29*************************
**  ��	����system_stm32l1xx.h
**  ˵	����ϵͳʱ�ӳ�ʼ�����Դ����ͷ�ļ�
**  ��	�룺uVision V5.12.0.0
**  ��	����V1.0
**  ��	д��Jay
**  ��	�ڣ�2015/12/29
**  �ޡ��ģ�����
*******************************************************************************/
#ifndef __SYSTEM_STM32L1XX_H
#define __SYSTEM_STM32L1XX_H

#ifdef __cplusplus
 extern "C" {
#endif 


  
extern void SystemInit(void);
extern unsigned char Get_SysClk_FrqAndSrc (unsigned int *frq,unsigned char *src);


#ifdef __cplusplus
}
#endif

#endif /*__SYSTEM_STM32L1XX_H */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

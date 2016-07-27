/***************************Copyright xxxxx 2015-12-29*************************
**  ��	����system_stm32l1xx.c
**  ˵	����ϵͳʱ�ӳ�ʼ�����Դ����
**  ��	�룺uVision V5.12.0.0
**  ��	����V1.0
**  ��	д��Jay
**  ��	�ڣ�2015/12/29
**  �ޡ��ģ�����
**  ˵  ��: 
  * This file configures the system clock as follows:  
  *=============================================================================
  *                         System Clock Configuration
  *=============================================================================
  *        System Clock source          | PLL(HSE)
  *-----------------------------------------------------------------------------
  *        SYSCLK                       | 32000000 Hz
  *-----------------------------------------------------------------------------
  *        HCLK                         | 32000000 Hz
  *-----------------------------------------------------------------------------
  *        AHB Prescaler                | 1
  *-----------------------------------------------------------------------------
  *        APB1 Prescaler               | 1
  *-----------------------------------------------------------------------------
  *        APB2 Prescaler               | 1
  *-----------------------------------------------------------------------------
  *        HSE Frequency                | 8000000 Hz
  *-----------------------------------------------------------------------------
  *        PLL DIV                      | 3
  *-----------------------------------------------------------------------------
  *        PLL MUL                      | 12
  *-----------------------------------------------------------------------------
  *        VDD                          | 3.3 V
  *-----------------------------------------------------------------------------
  *        Vcore                        | 1.8 V (Range 1)
  *-----------------------------------------------------------------------------
  *        Flash Latency                | 1 WS
  *-----------------------------------------------------------------------------
  *        SDIO clock (SDIOCLK)         | 48000000 Hz
  *-----------------------------------------------------------------------------
  *        Require 48MHz for USB clock  | Disabled
  *-----------------------------------------------------------------------------
*******************************************************************************/
#include "stm32l1xx.h"
#include "system_stm32l1xx.h"
#include <lib_type.h>
#include <DevPartTab.h>

//pravite macros define
#define VECT_TAB_START_ADDRESS  (DevPartTab->boot1_start_addr) 	//�ж����������ʼ��ַ

//pravite variable define
__IO const uint8_t PLLMulTable[9] = {3, 4, 6, 8, 12, 16, 24, 32, 48};
__IO const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
__IO const uint8_t APBAHBPrescTable[8] = {0, 0, 0, 0, 1, 2, 3, 4};

//pravite function declaration
static void SetSysClock(void);

/*******************************************************************************
�� �� ��:  	void SystemInit (void)
����˵��:  	ϵͳʱ�ӳ�ʼ��
��    ��:  	��
�� �� ֵ:  	��
˵    ��:	�����ⲿsram�����ʼ��
*******************************************************************************/
void SystemInit (void)
{
  /*!< Set MSION bit */
  RCC->CR |= (uint32_t)0x00000100;

  /*!< Reset SW[1:0], HPRE[3:0], PPRE1[2:0], PPRE2[2:0], MCOSEL[2:0] and MCOPRE[2:0] bits */
  RCC->CFGR &= (uint32_t)0x88FFC00C;
  
  /*!< Reset HSION, HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xEEFEFFFE;

  /*!< Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /*!< Reset PLLSRC, PLLMUL[3:0] and PLLDIV[1:0] bits */
  RCC->CFGR &= (uint32_t)0xFF02FFFF;

  /*!< Disable all interrupts */
  RCC->CIR = 0x00000000;
   
  /* Configure the System clock frequency, AHB/APBx prescalers and Flash settings */
  SetSysClock();

#ifdef VECT_TAB_SRAM
  SCB->VTOR = VECT_TAB_START_ADDRESS; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR = VECT_TAB_START_ADDRESS; /* Vector Table Relocation in Internal FLASH. */
#endif
}

/*******************************************************************************
�� �� ��:  	static void SetSysClock(void)
����˵��:  	����ʱ�Ӳ���
��    ��:  	��
�� �� ֵ:  	��
˵    ��:	��
*******************************************************************************/
static void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration ---------------------------*/
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }
  
  if (HSEStatus == (uint32_t)0x01)
  {
    /* Enable 64-bit access */
    FLASH->ACR |= FLASH_ACR_ACC64;
    
    /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_PRFTEN;

    /* Flash 1 wait state */
    FLASH->ACR |= FLASH_ACR_LATENCY;
    
    /* Power enable */
    RCC->APB1ENR |= RCC_APB1ENR_PWREN;
  
    /* Select the Voltage Range 1 (1.8 V) */
    PWR->CR = PWR_CR_VOS_0;
  
    /* Wait Until the Voltage Regulator is ready */
    while((PWR->CSR & PWR_CSR_VOSF) != RESET)
    {
    }
        
    /* HCLK = SYSCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
  
    /* PCLK2 = HCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
    
    /* PCLK1 = HCLK /1*/
    RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV1;
    
    /*  PLL configuration */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL |
                                        RCC_CFGR_PLLDIV));
    RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMUL12 | RCC_CFGR_PLLDIV3);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
        
    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL)
    {
    }
  }
  else
  {
    /* If HSE fails to start-up, the application will have wrong clock
       configuration. User can add here some code to deal with this error */
  }
}

/*******************************************************************************
�� �� ��:  	void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
����˵��:  	��ȡ����ϵͳ�ĸ���ʱ��
��    ��:  	RCC_Clocks��ʱ�Ӽ��ϵĽṹ��ָ��
�� �� ֵ:  	��
˵    ��:	��
*******************************************************************************/
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
    uint32_t tmp = 0, pllmull = 0, plldiv = 0, presc = 0 , msiclkrange = 0;;

    /* ��ȡ SYSCLK ��Դ -------------------------------------------------------*/
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_SWS_PLL);

    switch (tmp)
    {
		case RCC_CFGR_SWS_HSI:  /* HSI ��Ϊϵͳʱ����Դ */
			RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
			break;
		case RCC_CFGR_SWS_HSE:  /* HSE ��Ϊϵͳʱ����Դ */
			RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
			break;
		case RCC_CFGR_SWS_PLL: /* PLL ��Ϊϵͳʱ����Դ */

			/* ��ȡPLLʱ��Դ�ͱ�Ƶ���� ----------------------*/
			pllmull	= PLLMulTable[(RCC->CFGR & RCC_CFGR_PLLMUL) >> 18];
			plldiv	= ((RCC->CFGR & RCC_CFGR_PLLDIV) >> 22) + 1;
			if(RCC_CFGR_PLLSRC_HSI == (RCC->CFGR & RCC_CFGR_PLLSRC)) //��HSIΪPLL��ʱ��Դ
			{
				RCC_Clocks->SYSCLK_Frequency = (HSI_VALUE * pllmull) / plldiv;
			}
			else
			{
				RCC_Clocks->SYSCLK_Frequency = (HSE_VALUE * pllmull) / plldiv;
			}
			break;
		case RCC_CFGR_SWS_MSI:	/* MSI ��Ϊϵͳʱ����Դ */
		default:
			msiclkrange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE ) >> 13;	
			RCC_Clocks->SYSCLK_Frequency = (32768 * (1 << (msiclkrange + 1)));
			break;
    }
    /* ���� HCLK, PCLK1, PCLK2 and ADCCLK clocks Ƶ�� ----------------*/
    /* ��ȡ HCLK Ԥ��Ƶ */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_HPRE);
    tmp = tmp >> 4;
    presc = AHBPrescTable[tmp];
    /* HCLK ʱ��Ƶ�� */
    RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
    /* ��ȡ PCLK1 Ԥ��Ƶֵ */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_PPRE1);
    tmp = tmp >> 8;
    presc = APBAHBPrescTable[tmp];
    /* PCLK1 ʱ��Ƶ�� */
    RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
    /* ��ȡ PCLK2 Ԥ��Ƶֵ */
    tmp = RCC->CFGR & ((uint32_t)RCC_CFGR_PPRE2);
    tmp = tmp >> 11;
    presc = APBAHBPrescTable[tmp];
    /* PCLK2 ʱ��Ƶ�� */
    RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
    /* ADCCLK ʱ��Ƶ�� */
    RCC_Clocks->ADCCLK_Frequency = HSI_VALUE;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

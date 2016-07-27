/***************************Copyright xxxxx 2015-12-29*************************
**  文	件：system_stm32l1xx.c
**  说	明：系统时钟初始化相关源代码
**  编	译：uVision V5.12.0.0
**  版	本：V1.0
**  编	写：Jay
**  日	期：2015/12/29
**  修　改：暂无
**  说  明: 
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
#include "BF_type.h"
#include "stm32l1xx.h"
#include "DevPartTab.h"

//pravite macros define
#define VECT_TAB_START_ADDRESS  (DevPartTab->user_app_start_addr) 	//中断向量表的其实地址

//pravite variable define
__I uint8_t PLLMulTable[9] = {3, 4, 6, 8, 12, 16, 24, 32, 48};
__I uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

//pravite function declaration
static void SetSysClock(void);

/*******************************************************************************
函 数 名:  	void SystemInit (void)
功能说明:  	系统时钟初始化
参    数:  	无
返 回 值:  	无
说    明:	若有外部sram必须初始化
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
  SCB->VTOR = VECT_TAB_START_ADDRESS;//SOFT->appAddress;
#endif
}

/*******************************************************************************
函 数 名:  	static void SetSysClock(void)
功能说明:  	设置时钟参数
参    数:  	无
返 回 值:  	无
说    明:	无
*******************************************************************************/
static void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0,HSIStatus = 0;
  
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
		RCC->CR &= (uint32_t)(~RCC_CR_HSEON);//关闭HSE
		RCC->CR |= ((uint32_t)RCC_CR_HSION);//打开 HSI
		StartUpCounter = 0;	
		do
		{
			HSIStatus = RCC->CR & RCC_CR_HSIRDY;
			StartUpCounter++;
		} while((HSIStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));
			
		if ((RCC->CR & RCC_CR_HSIRDY) != RESET)
		{
			HSIStatus = (uint32_t)0x01;
		}
		else
		{
			HSIStatus = (uint32_t)0x00;
		}
		
		if (HSIStatus == (uint32_t)0x01)
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
			RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL | RCC_CFGR_PLLDIV));//  HSI 作为PLL的输入时钟 | VCO not allowed | PLL输出不分频
			RCC->CFGR &= (u32)~(1<<16);// HSI 作为PLL的输入时钟
			RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLMUL6 | RCC_CFGR_PLLDIV3);// 6倍频  | 3分频

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
  }
}

/*******************************************************************************
** 函数名称: 	Get_SysClk_FrqAndSrc
** 功能描述: 	获取系统主时钟频率和时钟源
** 输　  入:    u32 *frq：返回主时钟频率
				u8 *src ：返回时钟源 0：内部时钟  1：外部时钟
** 返    回:   	无
*******************************************************************************/
u8  Get_SysClk_FrqAndSrc (u32 *frq,u8 *src)
{
	uint32_t tmp = 0, pllmul = 0, plldiv = 0, pllsource = 0, msirange = 0;
	uint32_t SystemCoreClock = 0;
  /* Get SYSCLK source -------------------------------------------------------*/
  tmp = RCC->CFGR & RCC_CFGR_SWS;
  
  switch (tmp)
  {
    case 0x00:  /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
    case 0x04:  /* HSI used as system clock */
      SystemCoreClock = HSI_VALUE;
      break;
    case 0x08:  /* HSE used as system clock */
      SystemCoreClock = HSE_VALUE;
      break;
    case 0x0C:  /* PLL used as system clock */
      /* Get PLL clock source and multiplication factor ----------------------*/
      pllmul = RCC->CFGR & RCC_CFGR_PLLMUL;
      plldiv = RCC->CFGR & RCC_CFGR_PLLDIV;
      pllmul = PLLMulTable[(pllmul >> 18)];
      plldiv = (plldiv >> 22) + 1;
      
      pllsource = RCC->CFGR & RCC_CFGR_PLLSRC;

      if (pllsource == 0x00)
      {
        /* HSI oscillator clock selected as PLL clock entry */
        SystemCoreClock = (((HSI_VALUE) * pllmul) / plldiv);
				*src = 0;
      }
      else
      {
        /* HSE selected as PLL clock entry */
        SystemCoreClock = (((HSE_VALUE) * pllmul) / plldiv);
				*src = 1;
      }
      break;
    default: /* MSI used as system clock */
      msirange = (RCC->ICSCR & RCC_ICSCR_MSIRANGE) >> 13;
      SystemCoreClock = (32768 * (1 << (msirange + 1)));
      break;
  }
  /* Compute HCLK clock frequency --------------------------------------------*/
  /* Get HCLK prescaler */
  tmp = AHBPrescTable[((RCC->CFGR & RCC_CFGR_HPRE) >> 4)];
  /* HCLK clock frequency */
  SystemCoreClock >>= tmp;
	*frq = SystemCoreClock;//////////////////////
	
	return 1;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

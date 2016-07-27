/*******************************************************************
文	件：    Flash
说	明：    STM32_Flash操作头文件
编	译：    Keil uVision4 V4.54.0.0
版	本：    v1.0
编	写：    Unarty
日	期：    2013-11-20
********************************************************************/

#ifndef __FLASH_H
#define __FLASH_H

#include "stm32l1xx.h"
#include "BF_type.h"



/*******************  Bit definition for FLASH_ACR register  ******************/
#define  ACR_LATENCY                   ((uint32_t)0x00000001)        /*!< Latency */
#define  ACR_PRFTEN                    ((uint32_t)0x00000002)        /*!< Prefetch Buffer Enable */
#define  ACR_ACC64                     ((uint32_t)0x00000004)        /*!< Access 64 bits */
#define  ACR_SLEEP_PD                  ((uint32_t)0x00000008)        /*!< Flash mode during sleep mode */
#define  ACR_RUN_PD                    ((uint32_t)0x00000010)        /*!< Flash mode during RUN mode */

/*******************  Bit definition for PECR register  ******************/
#define PECR_PELOCK                    ((uint32_t)0x00000001)        /*!< PECR and Flash data Lock */
#define PECR_PRGLOCK                   ((uint32_t)0x00000002)        /*!< Program matrix Lock */
#define PECR_OPTLOCK                   ((uint32_t)0x00000004)        /*!< Option byte matrix Lock */
#define PECR_PROG                      ((uint32_t)0x00000008)        /*!< Program matrix selection */
#define PECR_DATA                      ((uint32_t)0x00000010)        /*!< Data matrix selection */
#define PECR_FTDW                      ((uint32_t)0x00000100)        /*!< Fixed Time Data write for Word/Half Word/Byte programming */
#define PECR_ERASE                     ((uint32_t)0x00000200)        /*!< Page erasing mode */
#define PECR_FPRG                      ((uint32_t)0x00000400)        /*!< Fast Page/Half Page programming mode */
#define PECR_PARALLBANK                ((uint32_t)0x00008000)        /*!< Parallel Bank mode */
#define PECR_EOPIE                     ((uint32_t)0x00010000)        /*!< End of programming interrupt */ 
#define PECR_ERRIE                     ((uint32_t)0x00020000)        /*!< Error interrupt */ 
#define PECR_OBL_LAUNCH                ((uint32_t)0x00040000)        /*!< Launch the option byte loading */ 

/******************  Bit definition for FLASH_PDKEYR register  ******************/
#define  PDKEYR_PDKEYR                 ((uint32_t)0xFFFFFFFF)       /*!< FLASH_PEC and data matrix Key */

/******************  Bit definition for FLASH_PEKEYR register  ******************/
#define  PEKEYR_PEKEYR                 ((uint32_t)0xFFFFFFFF)       /*!< FLASH_PEC and data matrix Key */

/******************  Bit definition for FLASH_PRGKEYR register  ******************/
#define  PRGKEYR_PRGKEYR               ((uint32_t)0xFFFFFFFF)        /*!< Program matrix Key */

/******************  Bit definition for FLASH_OPTKEYR register  ******************/
#define  OPTKEYR_OPTKEYR               ((uint32_t)0xFFFFFFFF)        /*!< Option bytes matrix Key */

/******************  Bit definition for FLASH_SR register  *******************/
#define  SR_BSY                        ((uint32_t)0x00000001)        /*!< Busy */
#define  SR_EOP                        ((uint32_t)0x00000002)        /*!< End Of Programming*/
#define  SR_ENHV                       ((uint32_t)0x00000004)        /*!< End of high voltage */
#define  SR_READY                      ((uint32_t)0x00000008)        /*!< Flash ready after low power mode */

#define  SR_WRPERR                     ((uint32_t)0x00000100)        /*!< Write protected error */
#define  SR_PGAERR                     ((uint32_t)0x00000200)        /*!< Programming Alignment Error */
#define  SR_SIZERR                     ((uint32_t)0x00000400)        /*!< Size error */
#define  SR_OPTVERR                    ((uint32_t)0x00000800)        /*!< Option validity error */
#define  SR_OPTVERRUSR                 ((uint32_t)0x00001000)        /*!< Option User validity error */

/******************  Bit definition for FLASH_OBR register  *******************/
#define  OBR_RDPRT                     ((uint16_t)0x000000AA)        /*!< Read Protection */
#define  OBR_BOR_LEV                   ((uint16_t)0x000F0000)        /*!< BOR_LEV[3:0] Brown Out Reset Threshold Level*/
#define  OBR_USER                      ((uint32_t)0x00700000)        /*!< User Option Bytes */
#define  OBR_IWDG_SW                   ((uint32_t)0x00100000)        /*!< IWDG_SW */
#define  OBR_nRST_STOP                 ((uint32_t)0x00200000)        /*!< nRST_STOP */
#define  OBR_nRST_STDBY                ((uint32_t)0x00400000)        /*!< nRST_STDBY */
#define  OBR_nRST_BFB2                 ((uint32_t)0x00800000)        /*!< BFB2 */

/******************  Bit definition for FLASH_WRPR register  ******************/
#define  WRPR_WRP                      ((uint32_t)0xFFFFFFFF)        /*!< Write Protect */

/******************  Bit definition for FLASH_WRPR1 register  *****************/
#define  WRPR1_WRP                     ((uint32_t)0xFFFFFFFF)        /*!< Write Protect */

/******************  Bit definition for FLASH_WRPR2 register  *****************/
#define  WRPR2_WRP                     ((uint32_t)0xFFFFFFFF)        /*!< Write Protect */

void Flash_Unlock(void);
void Flash_lock(void);
void DATA_EEPROM_Unlock(void);
u8 FLASH_ErasePage(u32 page_add);
u8 Flash_Write(u32 add, s32 len, u8 *data);
u8 Flash_WriteProtect(u32 addr, u8 sta);
u8 Flash_EraseOptionByte(void);
u8 Flash_WriteOptionByte(u32 addr, u8 data);

#endif /*FLASH_H */

/******************* (C) COPYRIGHT 2013 BestFu *****END OF FILE****/


/***********************COPYRIGHT BestFu****************************
?	?:SPI.H
?	?:SPI???????
?	?:Keil uVision4 V4.54.0.0
?	?:v1.0
?	?:Unarty
?	?:2013-08-24
********************************************************************/
#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

				    
// SPI?????? 
#define SPI_SPEED_2   		0
#define SPI_SPEED_4   		1
#define SPI_SPEED_8   		2
#define SPI_SPEED_16  		3
#define SPI_SPEED_32 		4
#define SPI_SPEED_64 		5
#define SPI_SPEED_128 		6
#define SPI_SPEED_256 		7
						  	    													  
void SPI2_Init(void);			 //???SPI2?
void SPI2_SetSpeed(u8 SpeedSet); //??SPI2??   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI2????????
		 
#endif


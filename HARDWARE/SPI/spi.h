#ifndef __SPI_H
#define __SPI_H
#include "sys.h"

extern SPI_HandleTypeDef hspi1;

void Error_Handler(void);
void MX_SPI2_Init(void);
void SPI2_SetSpeed(u32 SPI_BaudRatePrescaler);
//void MX_SPI4_Init(void);
u8 SPI2_ReadWriteByte(u8 TxData);
#endif

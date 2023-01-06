#ifndef __DMA_H__
#define __DMA_H__


//#include "main.h"
#include "stm32g4xx_hal.h"

DMA_HandleTypeDef hdma_adc5;
DMA_HandleTypeDef hdma_usart3_rx;

void dma_init(void);

#endif /* __DMA_H__ */

#include "dma.h"

void dma_init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMAMUX1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	/* DMA1_Channel2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

void DMA1_Channel1_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_adc5);
}

void DMA1_Channel2_IRQHandler(void) {
	HAL_DMA_IRQHandler(&hdma_usart3_rx);
}

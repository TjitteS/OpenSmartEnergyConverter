/*
 **	Copyright 2023 Tjitte van der Ploeg, tjitte@tpee.nl
 **
 **	This file is part of the OpenBoost firmware.
 **	The Open-SEC firmware is free software: you can redistribute
 **	it and/or modify it under the terms of the GNU General Public License
 **	as published by the Free Software Foundation, either version 3 of the
 **	License, or (at your option) any later version. The Open-SEC firmware is
 **	distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 **	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 **	PARTICULAR PURPOSE. See the GNU General Public License for more details.
 **
 **	You should have received a copy of the GNU General Public License along
 **	with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "uart.h"
#include "dma.h"

uint32_t consoleStatusLastTick;

#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256
uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t  txbuffer[TX_BUFFER_SIZE];

uint32_t wr_pointer = 0;
uint32_t available = 0;
uint32_t lastnewline = 0;
bool HasOverflown = false;

UART_HandleTypeDef huart3;

void modUARTInit(void) {

    __HAL_RCC_USART3_CLK_ENABLE();

	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	hdma_usart3_rx.Instance = DMA1_Channel2;
	hdma_usart3_rx.Init.Request = DMA_REQUEST_USART3_RX;
	hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
	hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart3_rx.Init.Mode = DMA_CIRCULAR;
	hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;

	HAL_DMA_Init(&hdma_usart3_rx);
	__HAL_LINKDMA(&huart3,hdmarx,hdma_usart3_rx);

	HAL_UART_Init(&huart3);
	HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_DisableFifoMode(&huart3);

	HAL_UART_Receive_DMA(&huart3, rxBuffer, RX_BUFFER_SIZE);
	libPacketInit(modUARTSendPacket, modUARTProcessPacket, PACKET_HANDLER_UART);																				// Configure the UART driver
}

void modUARTTask(void) {
	uint32_t n_wr_pointer = RX_BUFFER_SIZE - DMA1_Channel2->CNDTR;
	uint32_t new = 0;
	if(n_wr_pointer == wr_pointer){
		//No new character received. return for speed.
		return;
	}

	if(n_wr_pointer < wr_pointer){
		//Buffer went round
		new = n_wr_pointer + (RX_BUFFER_SIZE - wr_pointer);
		HasOverflown = true;

	}
	else{
		new = n_wr_pointer - wr_pointer;
	}
	available += new;

	if(available > RX_BUFFER_SIZE){
		available = RX_BUFFER_SIZE;
		//Overflow has occurred!
	}

	//Process incoming chars
	for(int i = 0; i < new; i++){
		uint32_t index = wr_pointer + i;
		if (index >= RX_BUFFER_SIZE){
			index -= RX_BUFFER_SIZE;
		}
		libPacketProcessByte(rxBuffer[index],PACKET_HANDLER_UART);

	}
	wr_pointer = n_wr_pointer;
}


void modUARTSendPacketWrapper(unsigned char *data, unsigned int len) {
	libPacketSendPacket(data, len, PACKET_HANDLER_UART);
}

void modUARTProcessPacket(unsigned char *data, unsigned int len) {
	modCommandsSetSendFunction(modUARTSendPacketWrapper);
	modCommandsProcessPacket(data, len);
}

void modUARTSendPacket(unsigned char *data, unsigned int len) {
	HAL_UART_Transmit(&huart3, data, len, 100);
}

#include "modUART.h"

uint32_t consoleStatusLastTick;

extern UART_HandleTypeDef huart3;
#define RX_BUFFER_SIZE 256
#define TX_BUFFER_SIZE 256
char rxBuffer[RX_BUFFER_SIZE];
char txbuffer[TX_BUFFER_SIZE];

uint32_t wr_pointer = 0;
uint32_t available = 0;
uint32_t lastnewline = 0;
bool HasOverflown = false;


void modUARTInit(void) {	
	HAL_UART_Receive_DMA(&huart3, rxBuffer, RX_BUFFER_SIZE);
	libPacketInit(modUARTSendPacket, modUARTProcessPacket, PACKET_HANDLER_UART);																				// Configure the UART driver
};

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

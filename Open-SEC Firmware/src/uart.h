#ifndef __MODMESSAGE_H
#define __MODMESSAGE_H

#include <stdint.h>
#include <stdbool.h>

#include "delay.h"
#include "config.h"
#include "packet.h"
#include "commands.h"

#define PACKET_HANDLER_UART		1

UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart3_rx;

typedef enum {
	MESSAGE_DEBUG = 0,
	MESSAGE_ERROR,
	MESSAGE_NORMAL,
} modMessageMessageTypeDef;

void modUARTInit(void);
void modUARTTask(void);
void modUARTSendPacketWrapper(unsigned char *data, unsigned int len);
void modUARTProcessPacket(unsigned char *data, unsigned int len);
void modUARTSendPacket(unsigned char *data, unsigned int len);

#endif

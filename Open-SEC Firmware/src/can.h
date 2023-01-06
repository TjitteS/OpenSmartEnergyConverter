/*
 * modCAN.h
 *
 *  Created on: 1 aug. 2020
 *      Author: Tjitte
 */

#ifndef SRC_CAN_H_
#define SRC_CAN_H_

#include "hardware.h"
#include "stdbool.h"
#include "string.h"
#include "delay.h"
#include "crc.h"
#include "packet.h"
#include "buffer.h"
#include "control.h"
#include "mppt.h"



// Settings
#define CAN_STATUS_MSG_INT_MS		1
#define RX_CAN_FRAMES_SIZE	    	255       // max 255
#define RX_CAN_BUFFER_SIZE	    	PACKET_MAX_PL_LEN

typedef enum {
	CAN_PACKET_Power, CAN_PACKET_status, //(Limitl, Fault, Temperatures,)
	CAN_PACKET_FILL_RX_BUFFER = 0x05,
	CAN_PACKET_FILL_RX_BUFFER_LONG,
	CAN_PACKET_PROCESS_RX_BUFFER,
	CAN_PACKET_PROCESS_SHORT_BUFFER,

	//TODO Add can interface commands here

	CAN_PACKET_MSGCOUNT,
} CAN_PACKET_ID;

typedef struct modCanSettings_t {
	int baudrate;
	float samplepoint;
	uint16_t generalCanId;
} modCanSettings_t;

void modCANinit(modCanSettings_t *s);
void modCANtask(void);

uint32_t modCANGetDestinationID(FDCAN_RxHeaderTypeDef canMsg);
CAN_PACKET_ID modCANGetPacketID(FDCAN_RxHeaderTypeDef canMsg);

uint32_t modCANGetCANEXID(uint32_t destinationID, CAN_PACKET_ID packetID);
uint32_t modCANGetCANID(uint32_t destinationID, CAN_PACKET_ID packetID);

void modCANTransmitExtID(uint32_t id, uint8_t *data, uint8_t len);
void modCANTransmitStandardID(uint32_t id, uint8_t *data, uint8_t len);
void modCANSendBuffer(uint8_t controllerID, uint8_t *data, unsigned int len, bool send);
void modCANSendPacketWrapper(unsigned char *data, unsigned int len);

#endif /* SRC_CAN_H_ */

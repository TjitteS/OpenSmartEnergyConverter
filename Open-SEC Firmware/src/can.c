/*
 * modCAN.h
 *
 *  Created on: 1 aug. 2020
 *      Author: Tjitte
 */

#include "can.h"
#include "commands.h"

uint32_t lasttick_canmsg_power;
uint32_t lasttick_canmsg_stat;

FDCAN_HandleTypeDef hfdcan2;
uint16_t canid = 0;

modCanSettings_t *cansettings;
uint8_t modCANRxBufferLastID;

#define MODCAN_RXBUFFER_SIZE 512
uint8_t modCANRxBuffer[RX_CAN_BUFFER_SIZE];

extern ConverterPhase_t phase;
extern ConverterMueasurements_t meter;

uint16_t modGetCanGetID() {
	uint16_t id = 0;
	id |= (uint16_t) HAL_GPIO_ReadPin(ID0_GPIO_Port, ID0_Pin);
	id |= (uint16_t) (HAL_GPIO_ReadPin(ID1_GPIO_Port, ID1_Pin) << 1);
	id |= (uint16_t) (HAL_GPIO_ReadPin(ID2_GPIO_Port, ID2_Pin) << 2);
	id |= (uint16_t) (HAL_GPIO_ReadPin(ID3_GPIO_Port, ID3_Pin) << 3);
	id = (~id) & 0x0f;

	id += (cansettings->generalCanId);

	return id;
}

typedef struct {
	FDCAN_TxHeaderTypeDef txmsg;
	uint8_t data[8];
} modCanTxQue_t;
typedef struct {
	FDCAN_RxHeaderTypeDef rxmsg;
	uint8_t data[8];
} modCanRxQue_t;

#define MODCAN_TXTIMEOUT 10
#define MODCAN_TXBUFFER_SIZE 32

modCanTxQue_t TXQue[MODCAN_TXBUFFER_SIZE];
modCanRxQue_t RXQue[MODCAN_TXBUFFER_SIZE];

int tx_wr = 0;
int tx_rd = 0;
int tx_available = 0;
int rx_wr = 0;
int rx_rd = 0;
int rx_available = 0;

uint32_t lasttick;
uint32_t dt;

void modCANinit(modCanSettings_t *s) {
	cansettings = s;
	//modCanSettingsLocal.GeneralCanId = 0;
	//modCanSettingsLocal.busSpeed = 500;
	//modCanSettingsLocal.samplepoint = 0.75;

	__HAL_RCC_FDCAN_CLK_ENABLE();

	canid = modGetCanGetID();

	int pre = 0;
	int S1 = 0;
	int S2 = 0;

	int clockspeed = 170e3;	//kHz

	if(cansettings->baudrate < 1){
		return;
	}

	for (int i = 9; i < 25; i++) {
		pre = (int) round(clockspeed / (cansettings->baudrate * i));
		if (pre == 0){
			return;
		}

		S1 = (int) round((cansettings->samplepoint * (float) i) - 1);
		S2 = i - S1 - 1;

		int ActualBuspeed = clockspeed / ((S1 + S2 + 1) * pre);

		if ((ActualBuspeed - cansettings->baudrate) < 1) {
			break;
		}

	}

	hfdcan2.Instance = FDCAN2;
	hfdcan2.Init.ClockDivider = FDCAN_CLOCK_DIV1;
	hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan2.Init.AutoRetransmission = ENABLE;
	hfdcan2.Init.TransmitPause = DISABLE;
	hfdcan2.Init.ProtocolException = DISABLE;
	hfdcan2.Init.NominalPrescaler = (uint32_t) pre;
	hfdcan2.Init.NominalSyncJumpWidth = 1;
	hfdcan2.Init.NominalTimeSeg1 = (uint32_t) S1;
	hfdcan2.Init.NominalTimeSeg2 = (uint32_t) S2;
	hfdcan2.Init.DataPrescaler = (uint32_t) pre;
	hfdcan2.Init.DataSyncJumpWidth = 1;
	hfdcan2.Init.DataTimeSeg1 = (uint32_t) S1;
	hfdcan2.Init.DataTimeSeg2 = (uint32_t) S2;
	hfdcan2.Init.StdFiltersNbr = 1;
	hfdcan2.Init.ExtFiltersNbr = 1;
	hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;

	if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK) {
		Error_Handler();
	}

	//GeneralCanId;
	FDCAN_FilterTypeDef sFilterConfig;
	sFilterConfig.IdType = FDCAN_STANDARD_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = modCANGetCANID(canid, CAN_PACKET_FILL_RX_BUFFER);
	sFilterConfig.FilterID2 = modCANGetCANID(canid, CAN_PACKET_MSGCOUNT - 1);
	if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}

	sFilterConfig.IdType = FDCAN_EXTENDED_ID;
	sFilterConfig.FilterIndex = 0;
	sFilterConfig.FilterType = FDCAN_FILTER_RANGE;
	sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	sFilterConfig.FilterID1 = modCANGetCANEXID(canid, CAN_PACKET_FILL_RX_BUFFER);
	sFilterConfig.FilterID2 = modCANGetCANEXID(canid, CAN_PACKET_MSGCOUNT - 1);

	if (HAL_FDCAN_ConfigFilter(&hfdcan2, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan2, FDCAN_REJECT, FDCAN_REJECT,
			FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_IT_RX_FIFO0_NEW_MESSAGE,
			0) != HAL_OK) {
		Error_Handler();
	}

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(FDCAN2_IT0_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(FDCAN2_IT0_IRQn);
	HAL_NVIC_SetPriority(FDCAN2_IT1_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(FDCAN2_IT1_IRQn);


	HAL_FDCAN_Start(&hfdcan2);

}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs) {
	if (RxFifo0ITs | FDCAN_IT_RX_FIFO0_NEW_MESSAGE) {
		while (HAL_FDCAN_GetRxFifoFillLevel(&hfdcan2, FDCAN_RX_FIFO0) > 0) {
			HAL_FDCAN_GetRxMessage(&hfdcan2, FDCAN_RX_FIFO0,
					&(RXQue[rx_wr].rxmsg), (RXQue[rx_wr].data));
			rx_wr++;
			rx_available++;
			if (rx_wr >= MODCAN_TXBUFFER_SIZE) {
				rx_wr = 0;
			}
		}
	}
}

void modCanHandleRxMsg(modCanRxQue_t *rxmsg) {
	CAN_PACKET_ID packetid = (CAN_PACKET_ID) rxmsg->rxmsg.Identifier & 0xf;
	uint16_t destinaitonId;

	if (rxmsg->rxmsg.IdType == FDCAN_STANDARD_ID) {
		destinaitonId = (uint16_t) (rxmsg->rxmsg.Identifier & 0x7F0) >> 4;
	} else {
		destinaitonId = (uint16_t) (rxmsg->rxmsg.Identifier & 0x7F00) >> 8;
	}

	uint32_t DLC = rxmsg->rxmsg.DataLength >> 16;
	if (DLC > 8)
		return;
	unsigned int rxbuf_ind;

	uint8_t commands_send;
	unsigned int rxbuf_len;
	uint16_t crc;

	float sweep_start, sweep_end;
	int32_t sweep_ind;
	uint8_t sweep_size;

	if (destinaitonId == canid) {
		switch (packetid) {
		case CAN_PACKET_Power:
		case CAN_PACKET_status:
			//These messages are send, not receifed.
			break;
		case CAN_PACKET_FILL_RX_BUFFER:

			//Check Data length
			if(DLC <= 1){
				break;
			}

			if ((rxmsg->data[0] + DLC) < MODCAN_RXBUFFER_SIZE) {
				memcpy(modCANRxBuffer + rxmsg->data[0], rxmsg->data + 1,
						(DLC - 1) * sizeof(uint8_t));
			}



			break;

		case CAN_PACKET_FILL_RX_BUFFER_LONG:
			rxbuf_ind = (unsigned int) rxmsg->data[0] << 8;
			rxbuf_ind |= rxmsg->data[1];


			//Check Data length
			if(DLC <= 2){
				break;
			}

			if (rxbuf_ind < RX_CAN_BUFFER_SIZE) {
				memcpy(modCANRxBuffer + rxbuf_ind, rxmsg->data + 2,
						(DLC - 2) * sizeof(uint8_t));
			}


			break;

		case CAN_PACKET_PROCESS_RX_BUFFER:
			//Check Data length
			if(DLC < 6){
				break;
			}

			modCANRxBufferLastID = rxmsg->data[0];
			commands_send = rxmsg->data[1];
			rxbuf_len = (unsigned int) rxmsg->data[2] << 8;
			rxbuf_len |= (unsigned int) rxmsg->data[3];
			crc = (((uint16_t)rxmsg->data[4]) << 8) | (uint16_t) rxmsg->data[5];

			if (rxbuf_len > RX_CAN_BUFFER_SIZE) {
				break;
			}


			if (libCRCCalcCRC16(modCANRxBuffer, rxbuf_len) == crc) {

				if (commands_send) {
					modCommandsSendPacket(modCANRxBuffer, rxbuf_len);
				} else {
					modCommandsSetSendFunction(modCANSendPacketWrapper);
					modCommandsProcessPacket(modCANRxBuffer, rxbuf_len);
				}
			}
			break;

		case CAN_PACKET_PROCESS_SHORT_BUFFER:
			//Check Data length
			if(DLC <= 2){
				break;
			}

			modCANRxBufferLastID = rxmsg->data[0];
			commands_send = rxmsg->data[1];

			if (commands_send) {
				modCommandsSendPacket(rxmsg->data + 2, DLC - 2);
			} else {
				modCommandsSetSendFunction(modCANSendPacketWrapper);
				modCommandsProcessPacket(rxmsg->data + 2, DLC - 2);
			}
			break;

		case CAN_CMD_SETMODE:
			//Check Data length
			if(DLC < 1){
				break;
			}

			modMpptSetMode((modMPPTmode_t)rxmsg->data[0]);
			break;

		case CAN_CMD_SWEEP:
			sweep_ind = 0;
			sweep_start = 1.0e3f*buffer_get_float16(rxmsg->data, 1.0e2f,&sweep_ind);
			sweep_end   = 1.0e3f*buffer_get_float16(rxmsg->data, 1.0e2f,&sweep_ind);
			sweep_size = buffer_get_uint8(rxmsg->data,&sweep_ind);

			if (sweep_size > MPPT_SWEEP_SIZE){
				sweep_size = MPPT_SWEEP_SIZE;
			}

			modMpptStartSweep(sweep_start, sweep_end, sweep_size);
			break;

		default:
		case CAN_PACKET_MSGCOUNT:
			break;
		}
	}
}


uint32_t lastAcktion;

void modCANtask(void) {

	if (HAL_GetTick() < (lastAcktion + 50)){
		LED3_GPIO_Port->BSRR = LED3_Pin;
	}else{
		LED3_GPIO_Port->BRR = LED3_Pin;
	}

	if (HAL_FDCAN_GetError(&hfdcan2)) {

	}



	uint32_t tnow = HAL_GetTick();
	dt = tnow - lasttick;
	lasttick = tnow;

	/*if(tx_available) {
		if (HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2)) {
			lastAcktion = HAL_GetTick();

			HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &(TXQue[tx_rd].txmsg),
					TXQue[tx_rd].data);
			tx_rd++;
			tx_available--;
			if (tx_rd >= MODCAN_TXBUFFER_SIZE) {
				tx_rd = 0;
			}
		}
	}
	else
	*/

	if (rx_available) {
		do {
			lastAcktion = HAL_GetTick();

			modCanHandleRxMsg(&RXQue[rx_rd]);
			rx_rd++;
			rx_available--;
			if (rx_rd >= MODCAN_TXBUFFER_SIZE) {
				rx_rd = 0;
			}
		} while(rx_available);//while (rx_available >= (MODCAN_TXBUFFER_SIZE - 10));
	}

	if (modDelayTick1ms(&lasttick_canmsg_stat, 1000)) {
		uint8_t data[5];
		int32_t index = 0;

		buffer_append_uint8(data, (uint8_t) phase.mode, &index);
		buffer_append_uint8(data, (uint8_t) phase.fault, &index);
		buffer_append_uint8(data, (uint8_t) phase.enabled, &index);
		buffer_append_int8(data, (int8_t) meter.TemperatureAmbient, &index);
		buffer_append_int8(data, (int8_t) meter.TemperatureHeatsink, &index);

		modCANTransmitStandardID(modCANGetCANID(canid, CAN_PACKET_status), data,
				(uint32_t) index);
	}

	//HAL_Delay(10);

	if (modDelayTick1ms(&lasttick_canmsg_power, 500)) {
		uint8_t data[8];
		int32_t index = 0;

		buffer_append_float16(data, meter.Vlow, 1e2f, &index);
		buffer_append_float16(data, meter.Iind, 2.0e3f, &index);
		buffer_append_float16(data, meter.Vhigh, 1e2f, &index);
		buffer_append_float16(data, meter.Ihigh, 2.0e3f, &index);

		modCANTransmitStandardID(modCANGetCANID(canid, CAN_PACKET_Power), data,
				(uint32_t) index);
	}
}

uint32_t modCANGetCANEXID(uint32_t destinationID, CAN_PACKET_ID packetID) {
	/*uint32_t returnCANID;
	 switch(modCANGeneralConfigHandle->CANIDStyle) {
	 default:																																					// Default to VESC style ID
	 case CANIDStyleVESC:
	 returnCANID = ((uint32_t) destinationID) | ((uint32_t)packetID << 8);
	 break;
	 case CANIDStyleFoiler:
	 returnCANID = ((uint32_t) destinationID << 8) | ((uint32_t)packetID);
	 break;
	 }*/

	return ((destinationID << 8) & 0x7F00) | (packetID & 0x00FF);
}

uint32_t modCANGetCANID(uint32_t destinationID, CAN_PACKET_ID packetID) {
	/*uint32_t returnCANID;
	 switch(modCANGeneralConfigHandle->CANIDStyle) {
	 default:																																					// Default to VESC style ID
	 case CANIDStyleVESC:
	 returnCANID = ((uint32_t) destinationID) | ((uint32_t)packetID << 8);
	 break;
	 case CANIDStyleFoiler:
	 returnCANID = ((uint32_t) destinationID << 8) | ((uint32_t)packetID);
	 break;
	 }*/

	return ((destinationID << 4) & 0x7F0) | (packetID & 0x00F);
}

void modCANTransmit(FDCAN_TxHeaderTypeDef* txmsg, uint8_t* data){

	//Check for bus errors
	FDCAN_ProtocolStatusTypeDef ProtocolStatus;
	HAL_FDCAN_GetProtocolStatus(&hfdcan2, &ProtocolStatus);

	if(ProtocolStatus.BusOff){
		CLEAR_BIT(hfdcan2.Instance->CCCR, FDCAN_CCCR_INIT);
	}

	if (HAL_FDCAN_IsRestrictedOperationMode(&hfdcan2)){
		HAL_FDCAN_ExitRestrictedOperationMode(&hfdcan2);
	}

	//Wait till there i room in the TX buffer.
	uint32_t ts = HAL_GetTick();
	while(HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan2) < 1){
		if ((ts - HAL_GetTick()) > MODCAN_TXTIMEOUT){
			return;
		}
	}

	lastAcktion = HAL_GetTick();
	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, txmsg, data);

	HAL_Delay(1);
}

void modCANTransmitStandardID(uint32_t id, uint8_t *data, uint8_t len) {
	FDCAN_TxHeaderTypeDef txmsg;
	if (len > 8)
		len = 8;

	txmsg.TxFrameType = FDCAN_DATA_FRAME;
	txmsg.IdType = FDCAN_STANDARD_ID;
	txmsg.MessageMarker = 0;
	txmsg.ErrorStateIndicator = FDCAN_ESI_PASSIVE;
	txmsg.FDFormat = FDCAN_FRAME_CLASSIC;
	txmsg.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	txmsg.BitRateSwitch = FDCAN_BRS_OFF;
	txmsg.DataLength = (uint32_t) (len << 16);
	txmsg.Identifier = id;

	modCANTransmit(&txmsg, data);

	/*
	TXQue[tx_wr].txmsg = txmsg;
	memcpy(TXQue[tx_wr].data, data, sizeof(uint8_t) * len);
	tx_wr++;
	tx_available++;
	if (tx_wr >= MODCAN_TXBUFFER_SIZE)
		tx_wr = 0;
	*/

}

void modCANTransmitExtID(uint32_t id, uint8_t *data, uint8_t len) {
	FDCAN_TxHeaderTypeDef txmsg;

	if (len > 8)
		len = 8;

	txmsg.TxFrameType = FDCAN_DATA_FRAME;
	txmsg.IdType = FDCAN_EXTENDED_ID;
	txmsg.MessageMarker = 0;
	txmsg.ErrorStateIndicator = FDCAN_ESI_PASSIVE;
	txmsg.FDFormat = FDCAN_FRAME_CLASSIC;
	txmsg.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
	txmsg.BitRateSwitch = FDCAN_BRS_OFF;
	txmsg.DataLength = (uint32_t) (len << 16);
	txmsg.Identifier = id;

	modCANTransmit(&txmsg, data);


	//TXQue[tx_wr].txmsg = txmsg;
	//memcpy(TXQue[tx_wr].data, data, sizeof(uint8_t) * len);
	//tx_wr++;
	//tx_available++;
	//if (tx_wr >= MODCAN_TXBUFFER_SIZE)
	//	tx_wr = 0;



}



void modCANSendBuffer(uint8_t controllerID, uint8_t *data, unsigned int len, bool send) {
	uint8_t send_buffer[8];

	if (len <= 6) {
		uint32_t ind = 0;
		send_buffer[ind++] = (uint8_t) canid & 0xFF;
		send_buffer[ind++] = send;
		memcpy(send_buffer + ind, data, len);
		ind += len;
		modCANTransmitExtID(modCANGetCANEXID(controllerID, CAN_PACKET_PROCESS_SHORT_BUFFER), send_buffer, (uint8_t) ind);
	} else {
		unsigned int end_a = 0;
		for (unsigned int i = 0; i < len; i += 7) {
			if (i > 255) {
				break;
			}

			end_a = i + 7;

			uint8_t send_len = 7;
			send_buffer[0] = (uint8_t) i;

			if ((i + 7) <= len) {
				memcpy(send_buffer + 1, data + i, send_len);
			} else {
				send_len = (uint8_t) (len - i);
				memcpy(send_buffer + 1, data + i, send_len);
			}

			modCANTransmitExtID(modCANGetCANEXID(controllerID, CAN_PACKET_FILL_RX_BUFFER), send_buffer, (uint8_t) (send_len + 1));
		}

		for (unsigned int i = end_a; i < len; i += 6) {
			uint8_t send_len = 6;
			send_buffer[0] = (uint8_t) (i >> 8);
			send_buffer[1] = i & 0xFF;

			if ((i + 6) <= len) {
				memcpy(send_buffer + 2, data + i, send_len);
			} else {
				send_len = (uint8_t) (len - i);
				memcpy(send_buffer + 2, data + i, send_len);
			}

			modCANTransmitExtID(modCANGetCANEXID(controllerID, CAN_PACKET_FILL_RX_BUFFER_LONG), send_buffer, (uint8_t) (send_len + 2));
		}

		uint32_t ind = 0;
		send_buffer[ind++] = (uint8_t) canid & 0xFF;
		send_buffer[ind++] = send;
		send_buffer[ind++] = (uint8_t) (len >> 8);
		send_buffer[ind++] = len & 0xFF;
		unsigned short crc = libCRCCalcCRC16(data, len);
		send_buffer[ind++] = (uint8_t) (crc >> 8);
		send_buffer[ind++] = (uint8_t) (crc & 0xFF);

		// Old ID method
		//modCANTransmitExtID(controllerID | ((uint32_t)CAN_PACKET_PROCESS_RX_BUFFER << 8), send_buffer, ind++);
		modCANTransmitExtID(modCANGetCANEXID(controllerID, CAN_PACKET_PROCESS_RX_BUFFER), send_buffer, (uint8_t) ind++);
	}
}

void modCANSendPacketWrapper(unsigned char *data, unsigned int length) {
	modCANSendBuffer(modCANRxBufferLastID, data, length, true);
}

void FDCAN2_IT0_IRQHandler(void) {

	HAL_FDCAN_IRQHandler(&hfdcan2);

}

void FDCAN2_IT1_IRQHandler(void) {

	HAL_FDCAN_IRQHandler(&hfdcan2);

}

void modCANSendSweep(float* is, float*vs, int size){

	uint8_t data[8];

	for (int i = 0; i < size; i++){
		int32_t index = 0;
		buffer_append_int8(data, i, &index);
		buffer_append_float16(data, is[i], 1e2f, &index);
		buffer_append_float16(data, vs[i], 2.0e3f, &index);

		modCANTransmitStandardID(modCANGetCANID(canid, CAN_PACKET_Sweep), data, (uint32_t) index);
	}
}



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
 

#ifndef PACKET_H_
#define PACKET_H_

#include <stdint.h>

// Settings
#define PACKET_RX_TIMEOUT		2
#define PACKET_HANDLERS			2
#define PACKET_MAX_PL_LEN		1024

// Functions
void libPacketInit(void (*s_func)(unsigned char *data, unsigned int len), void (*p_func)(unsigned char *data, unsigned int len), int handler_num);
void libPacketProcessByte(uint8_t rx_data, int handler_num);
void libPacketTimerFunc(void);
void libPacketSendPacket(unsigned char *data, unsigned int len, int handler_num);

#endif /* PACKET_H_ */

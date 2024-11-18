/*
	Copyright 2016 Benjamin Vedder	benjamin@vedder.se

	This file is part of the VESC firmware.

	The VESC firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The VESC firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef COMMANDS_H_
#define COMMANDS_H_


#include "buffer.h"
#include "terminal.h"
#include "config.h"

#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "can.h"

// Communication commands
typedef enum {
	COMM_FW_VERSION = 0,
	COMM_JUMP_TO_BOOTLOADER,
	COMM_ERASE_NEW_APP,
	COMM_WRITE_NEW_APP_DATA,
	COMM_GET_VALUES,
	COMM_SET_DUTY,
	COMM_SET_CURRENT,
	COMM_SET_CURRENT_BRAKE,
	COMM_SET_RPM,
	COMM_SET_POS,
	COMM_SET_HANDBRAKE,
	COMM_SET_DETECT,
	COMM_SET_SERVO_POS,
	COMM_SET_MCCONF,
	COMM_GET_MCCONF,
	COMM_GET_MCCONF_DEFAULT,
	COMM_SET_APPCONF,
	COMM_GET_APPCONF,
	COMM_GET_APPCONF_DEFAULT,
	COMM_SAMPLE_PRINT,
	COMM_TERMINAL_CMD,
	COMM_PRINT,
	COMM_ROTOR_POSITION,
	COMM_EXPERIMENT_SAMPLE,
	COMM_DETECT_MOTOR_PARAM,
	COMM_DETECT_MOTOR_R_L,
	COMM_DETECT_MOTOR_FLUX_LINKAGE,
	COMM_DETECT_ENCODER,
	COMM_DETECT_HALL_FOC,
	COMM_REBOOT,
	COMM_ALIVE,
	COMM_GET_DECODED_PPM,
	COMM_GET_DECODED_ADC,
	COMM_GET_DECODED_CHUK,
	COMM_FORWARD_CAN,
	COMM_SET_CHUCK_DATA,
	COMM_CUSTOM_APP_DATA,
	COMM_NRF_START_PAIRING,

	COMM_STORE_MPPT_CONF = 50,

	COMM_WRITE_CALDATA = 75,
	COMM_READ_CALDATA,

	CONM_MPPT_GET_SWEEP=100,
	CONM_MPPT_SetMode,
	CONM_MPPT_SetOutputVoltage,
	CONM_MPPT_SetOutputCurrent,
	CONM_MPPT_SetInputCurrent,
	CONM_MPPT_SetInputVoltage,
	COMM_MPPT_SCOPE_RUN,
	COMM_MPPT_SCOPE_STEP,
	COMM_MPPT_SCOPE_GET_DATA,

} COMM_PACKET_ID;

// Functions
void modCommandsInit();
void modCommandsSetSendFunction(void(*func)(unsigned char *data, unsigned int len));
void modCommandsSendPacket(unsigned char *data, unsigned int len);
void modCommandsProcessPacket(unsigned char *data, unsigned int len);
void modCommandsPrintf(const char* format, ...);
void modCommandsJumpToMainApplication(void);
void modCommandsSendSweep(void);

#endif /* COMMANDS_H_ */

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

#ifndef __MODCONFIG_H
#define __MODCONFIG_H

#include <stdint.h>
#include <stdbool.h>

#include "control.h"
#include "mppt.h"
#include "can.h"
#include "eeprom.h"

//#define hallo RX_CAN_FRAMES_SIZE

typedef struct {
	//This structure represents the data structure of the full configuration settings.
	CalibrationData_t	calData;
	ConverterSettings_t settings;
	modMPPTsettings_t mpptsettings;
	modCanSettings_t cansettings;
}modConfig_t;



typedef enum {
	EEPROM_OK,
	EEPROM_Error,
	EEPROM_Invalid_Adress,
	EEPROM_Invalid,
	EERPOM_WRITE_ERROR,
}EEPROM_resp_t;

modConfig_t* modConfigInit(void);
bool modConfigStoreAndLoadDefaultConfig(void);
bool modConfigStoreConfig(void);
bool modConfigLoadConfig(void);
bool modConfigStoreDefaultConfig(void);
void modConfigLoadDefaultConfig(void);
void modConfigLimitSettings(modConfig_t* c);

#endif

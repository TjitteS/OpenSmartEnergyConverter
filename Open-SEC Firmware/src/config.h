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

#endif

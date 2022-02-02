#ifndef __MODCONFIG_H
#define __MODCONFIG_H

#include "stdint.h"
#include "stdbool.h"
#include "modConverter.h"
#include "modMPPT.h"
#include "modCAN.h"

//#define hallo RX_CAN_FRAMES_SIZE

typedef struct {
	//This structure represents the data structure of the full configuration settings.
	CalibrationData_t	calData;
	ConverterSettings_t settings;
	modMPPTsettings_t mpptsettings;
	modCanSettings_t cansettings;
}modConfig_t;

#define __EEPROM __attribute__((__section__(".erom")))
#define EEPROM_START_ADDRESS ((uint32_t)0x0807F800)
#define EEPROM_SIZE (0x0807F800+(2<<11)-EEPROM_START_ADDRESS-4)
#define EEPROM_END_ADDRESS 0x08080000
#define CRC16 0x8005

typedef enum{
	EEPROM_Valid = 0x5555,
	EEPROM_Ereased = 0xFFFF,
}EEPROM_state;
typedef enum {
	EEPROM_OK,
	EEPROM_Error,
	EEPROM_Invalid_Adress,
	EEPROM_Invalid,
	EERPOM_WRITE_ERROR,
}EEPROM_resp_t;

const modConfig_t* modConfigInit(void);
bool modConfigStoreAndLoadDefaultConfig(void);
bool modConfigStoreConfig(void);
bool modConfigLoadConfig(void);
bool modConfigStoreDefaultConfig(void);
void modConfigLoadDefaultConfig(void);

#endif

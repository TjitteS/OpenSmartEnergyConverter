#include "config.h"
#include "main.h"

volatile __EEPROM modConfig_t storedConfig;

//Defined in harwhare definition file
extern modConfig_t defaultConvig;

modConfig_t config;

modConfig_t* modConfigInit(void) {
	bool valid = eeprom_init();

	if(valid){
		config = storedConfig;
		return &config;
	}
	else{
		if(modConfigStoreAndLoadDefaultConfig() == false){
			return &config;
		}
		else {
			return (modConfig_t*)0x0;
		}
	}
}


bool modConfigStoreConfig(void) {
	storedConfig = config;
	__ISB();
	eeprom_save();
	return false;
}

//Load config from flash
bool modConfigLoadConfig() {
	eeprom_load();
	config = storedConfig;
	return true;
}

//Set config settings
bool modConfigSetConfig(modConfig_t* conf){
	main_halt_risky();
	config = *conf;
	main_init_after_config();
	return false;
}
bool modConfigStoreAndLoadDefaultConfig(void) {
	modConfigLoadDefaultConfig();
	return modConfigStoreConfig();
};

bool modConfigStoreDefaultConfig(void) {
	//modConfigStoreAndLoadDefaultConfig();
	return true;
}


void modConfigLoadDefaultConfig() {
	memcpy(&config.cansettings, &defaultConvig.cansettings, sizeof(modCanSettings_t));
	memcpy(&config.mpptsettings, &defaultConvig.mpptsettings, sizeof(modMPPTsettings_t));
	memcpy(&config.settings, &defaultConvig.settings, sizeof(ConverterSettings_t));
}


/*
 **	Copyright 2024 Tjitte van der Ploeg, tjitte@tpee.nl
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
		modConfigLimitSettings(&config);
		return &config;
	}
	else{
		//Load default values. Also load default calibration.]
		memcpy(&config, &defaultConvig, sizeof(modConfig_t));
		modConfigLimitSettings(&config);

		//Save
		if(modConfigStoreConfig() == false){
			return &config;
		}
		else {
			//Complete failure...
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
	modConfigLimitSettings(&config);
}

void modConfigLimitSettings(modConfig_t* c){
	if(c->settings.TemperatureLimitEnd > HW_TEMP_LIM){
		c->settings.TemperatureLimitEnd = HW_TEMP_LIM;
	}
	if(c->settings.TemperatureLimitStart > c->settings.TemperatureLimitEnd){
		c->settings.TemperatureLimitStart = c->settings.TemperatureLimitEnd;
	}

	if(c->settings.HighSideCurrentLimitSoft > HW_LIMIT_HS_CURRENT_HARD * 0.65) c->settings.HighSideCurrentLimitSoft = HW_LIMIT_HS_CURRENT_HARD * 0.65;
	if(c->settings.LowSideCurrentMaxLimitSoft > HW_LIMIT_LS_CURRENT_HARD * 0.65) c->settings.LowSideCurrentMaxLimitSoft = HW_LIMIT_LS_CURRENT_HARD * 0.65;
	if(c->settings.HighSideVoltageLimitSoft > HW_LIMIT_HS_VOLTAGE_HARD * 0.95) c->settings.HighSideVoltageLimitSoft = HW_LIMIT_HS_VOLTAGE_HARD * 0.95;
	if(c->settings.LowSideVoltageLimitSoft > HW_LIMIT_LS_VOLTAGE_HARD * 0.95) c->settings.LowSideVoltageLimitSoft = HW_LIMIT_LS_VOLTAGE_HARD * 0.95;

	if(c->settings.LowSideCurrentMinLimitSoft < -500.0f)c->settings.LowSideCurrentMinLimitSoft = -500.0f;
	if(c->settings.PhaseHighSideEnableCurrent < -500.0f)c->settings.PhaseHighSideEnableCurrent = -500.0f;

	if(c->mpptsettings.PO_Stepsize         < 0)c->mpptsettings.PO_Stepsize          = 0;
	if(c->mpptsettings.PO_Timestep         < 10)c->mpptsettings.PO_Timestep         = 10;
	if(c->mpptsettings.PO_StepSizeGain     < 0)c->mpptsettings.PO_StepSizeGain      = 0;
	if(c->mpptsettings.jump_PowerThreshold < 0)c->mpptsettings.jump_PowerThreshold  = 0;
	if(c->mpptsettings.jump_Rate           < 0)c->mpptsettings.jump_Rate            = 0;


	if(c->cansettings.baudrate     > 1000)c->cansettings.baudrate     = 1000;
	if(c->cansettings.baudrate     < 10)  c->cansettings.baudrate     = 10;

	if(c->cansettings.samplepoint  > 0.9)c->cansettings.samplepoint  = 0.9;
	if(c->cansettings.samplepoint  < 0.1)c->cansettings.samplepoint  = 0.1;
	if(c->cansettings.generalCanId > 112)c->cansettings.generalCanId = 112;
	if(c->cansettings.generalCanId < 0)c->cansettings.generalCanId = 0;



}


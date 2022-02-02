#include "modConfig.h"
#include "main.h"

volatile __EEPROM EEPROM_state rom_state;
volatile __EEPROM uint16_t crc;

__EEPROM modConfig_t storedConfig;

modConfig_t config;

uint16_t eeprom_calc_crc16(void);

//Private funkiton-prototypes
EEPROM_resp_t EEPROM_write(uint64_t* dest, uint64_t*src, uint32_t length);
EEPROM_resp_t EEPROM_EndWrite();
EEPROM_resp_t EEPROM_Startwrite();


bool EepromValid(){
	uint16_t ccrc = libCRCCalcCRC16((unsigned char *)&storedConfig, sizeof(modConfig_t));
	if ((rom_state != EEPROM_Valid) || (crc != ccrc)) {
		//Current EEPROM is invalid.
		return false;
	}
	return true;
}

//Returns false/0 on sucsess.

const modConfig_t* modConfigInit(void) {
	if(EepromValid()){
		config = storedConfig;
		return &config;
	}else{
		if(modConfigStoreAndLoadDefaultConfig()==false)
			return &config;
	}
}

//Save
//Write/Load
//WriteDefault
//Write&store Default

bool modConfigStoreConfig(void) {
	mainHaltRisky();
	HAL_StatusTypeDef status = HAL_FLASH_Unlock();
	uint32_t error = 0;

	// make sure that this structure matches the datasheet of your chip
	FLASH_EraseInitTypeDef  FLASH_EraseInitStruct = {
		.TypeErase = FLASH_TYPEERASE_PAGES,
        .Banks = FLASH_BANK_2,
        .Page = 127,
        .NbPages = 1
	};

	// clear all flags before you write it to flash
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR |
				FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);

	if (status != HAL_OK)
		return status;

	// perform the erase first
	HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &error);

	//if (error)
	//	return -1;


	int size = sizeof(modConfig_t)+sizeof(uint16_t)+sizeof(uint16_t);

	uint8_t data[sizeof(modConfig_t)+sizeof(uint16_t)+sizeof(uint16_t)];

	uint32_t pos = (uint32_t)&crc - EEPROM_START_ADDRESS;
	*((uint16_t*)(data+pos)) = (uint16_t)libCRCCalcCRC16((unsigned char *)&config, sizeof(modConfig_t));
	pos = (uint32_t)&rom_state - EEPROM_START_ADDRESS;
	*((uint16_t*)(data+pos)) = EEPROM_Valid;

	pos=(uint32_t)&storedConfig - EEPROM_START_ADDRESS;
	memcpy((data+pos),&config,sizeof(modConfig_t));


	uint64_t* address = EEPROM_START_ADDRESS;
	int i =0;
	do{
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,  (uint32_t)address, ((uint64_t*)data)[i++]);
		address += 1;
		//wait for flash to be ready
		FLASH_WaitForLastOperation(10);
	}while(address < (EEPROM_START_ADDRESS+size));

	//CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
	HAL_FLASH_Lock();
	FLASH_WaitForLastOperation(100);
	HAL_FLASH_Unlock();

	mainInitAfterConfig();
	return false;
};

//Load config from flash
bool modConfigLoadConfig() {
	if(EepromValid()){
		config = storedConfig;
		return true;
	}
	return false;
};
//Set config settings
bool modConfigSetConfig(modConfig_t* conf){
	mainHaltRisky();
	config = *conf;
	mainInitAfterConfig();
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
	config.calData.InputCurrentGain           =4.255f;
	config.calData.InputCurrentOffset         =0.0f;
	config.calData.InputVoltageGain           =31.0f;
	config.calData.InputVoltageOffset         =0.0f;
	config.calData.OutputCurrentGain          =4.255f;
	config.calData.OutputCurrentOffset        =0.0f;
	config.calData.OutputVoltageGain          =31.0f;
	config.calData.OutputVoltageOffset        =0.0f;
	config.calData.Temperature_B              =4000.0f;
	config.calData.Temperature_R              =100.0e3f;
	config.calData.Temperature_Ref            =25.0f;
	config.calData.ADC_delay                  =0.1f;
	config.calData.calibrated 				  = false;
	config.settings.SwitchingFrequency        =100.0f;
	config.settings.ControllerFrequency       =35.0f;
	config.settings.DeadTime                  =20.0f;
	config.settings.L                         =32.0e-6f;
	config.settings.Cin                       =100.0e-6f;
	config.settings.Cout                      =600.0e-6f;
	config.settings.RLint                     =30e-3f;
	config.settings.Q                         =0.5f;
	config.settings.Klim                      =0.9f;
	config.settings.OutputVoltageLimitSoft    =20*4.2e3f;
	config.settings.OutputVoltageLimitHard    =90.0e3f;
	config.settings.InputVoltageLimitSoft     =90.0e3f;
	config.settings.InputVoltageLimitHard     =90.0e3f;
	config.settings.OutputCurrentLimitSoft    =10.0e3f;
	config.settings.OutputCurrentLimitHard    =10.0e3f;
	config.settings.InputCurrentLimitSoft     =10.0e3f;
	config.settings.InputCurrentLimitHard     =10.0e3f;
	config.settings.PhaseHighSideEnableCurrent=0.0f;
	config.settings.PhaseCurrentMin           =0.0f;
	config.settings.outputEnable 			  = false;
	config.settings.meterfilterCoeficient = 0.5f;

	config.mpptsettings.PO_Stepsize = 100;
	config.mpptsettings.PO_Timestep = 10;
	config.mpptsettings.PO_maxgradient = 100.0f;

	config.mpptsettings.jump_PowerThreshold = 5.0f;
	config.mpptsettings.jump_RangeMax = 25.0f;
	config.mpptsettings.jump_RangeMin = 5.0f;
	config.mpptsettings.jump_Rate = 40;

	config.cansettings.baudrate = 250;
	config.cansettings.samplepoint = 0.75f;
	config.cansettings.generalCanId = 0;

}

//PRIVATE functions
EEPROM_resp_t EEPROM_write(uint64_t* dest, uint64_t*src, uint32_t length) {
	//
	if (HAL_IS_BIT_SET(FLASH->CR, FLASH_CR_LOCK)){
		//flash is locked, return
		return EEPROM_Error;
	}

	if (length < 1) {
		return EEPROM_Error;
	}

	if (((uint32_t) dest < EEPROM_START_ADDRESS)
			|| ((uint32_t) dest > (EEPROM_END_ADDRESS))) {
		return EEPROM_Invalid_Adress;
	}

	int l = length / 2;

	if (l <= 0) l = 1;

	//wait for flash to be ready
	FLASH_WaitForLastOperation(10);

	for (int i = 0; i < l; i++) {
		//write half word
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, dest+i, src[i]);

		//wait for flash to be ready
		FLASH_WaitForLastOperation(10);

		//Verify bhaslfwords
	}


	return EEPROM_OK;
}
EEPROM_resp_t EEPROM_EndWrite() {
	//update CRC
	FLASH_WaitForLastOperation(10);

	crc = eeprom_calc_crc16();

	//write eeprom status
	FLASH_WaitForLastOperation(10);
	rom_state = EEPROM_Valid;
	FLASH_WaitForLastOperation(10);
	CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
	HAL_FLASH_Lock();

	return EEPROM_OK;
}

EEPROM_resp_t EEPROM_Startwrite(){
	HAL_FLASH_Unlock();
	FLASH_WaitForLastOperation(10);
	FLASH_PageErase(127,FLASH_BANK_2);
	FLASH_WaitForLastOperation(10);
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER); // Bug fix: bit PER has been set in Flash_PageErase(), must clear it here
	SET_BIT(FLASH->CR, FLASH_CR_PG);
	return EEPROM_OK;
}


//private functions
uint16_t eeprom_calc_crc16(void) {
	const uint8_t *data = (const uint8_t*)EEPROM_START_ADDRESS;
	uint32_t size = (uint32_t)EEPROM_SIZE;

	uint16_t out = 0;
	int bits_read = 0, bit_flag;

	while (data < EEPROM_END_ADDRESS) {
		bit_flag = out >> 15;

		/* Get next bit: */
		out <<= 1;
		out |= (*data >> (7 - bits_read)) & 1;

		/* Increment bit counter: */
		bits_read++;
		if (bits_read > 7) {
			bits_read = 0;
			data++;
			size--;
		}

		/* Cycle check: */
		if (bit_flag) out ^= CRC16;
	}
	return out;
}


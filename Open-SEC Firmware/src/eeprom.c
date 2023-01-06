/*
 * eeprom.c
 *
 *  Created on: 6 Oct 2021
 *      Author: Tjitte
 */
#include <stdint.h>
#include <string.h>

#include "stm32g4xx_hal.h"

#include "eeprom.h"
#include "crc.h"
#include "flash.h"

//These are 64 bit to make life easer when programming them.
volatile __EEPROM_FLASH uint64_t rom_state;
volatile __EEPROM_FLASH uint64_t crc;
volatile __EEPROM_FLASH uint8_t eeprommem[EEPROM_SIZE / sizeof(uint8_t)];

//Private funkiton-prototypes
//uint16_t eeprom_calc_crc16(void);
//EEPROM_resp_t eeprom_write(uint64_t* dest, uint64_t*src, uint32_t length);
//EEPROM_resp_t eeprom_endwrite();
//EEPROM_resp_t eeprom_startwrite();



bool eeprom_init() {
	__HAL_RCC_SRAM2_CLK_SLEEP_DISABLE();

	//Initialize RAM memory as FF to minimize flash writes and to set ram memory to known data.
	uint32_t* eeprom_ram = (uint32_t*) EEPROM_RAMSTART;
	for(int i = 0; i < EEPROM_SIZE/sizeof(uint32_t); i++){
		eeprom_ram[i] = 0xFFFFFFFF;
	}

	if (eeprom_valid()) {
		eeprom_load();
		return true;
	}
	return false;
}

bool eeprom_valid() {
	uint16_t ccrc = libCRCCalcCRC16(eeprommem, EEPROM_SIZE / sizeof(uint8_t));
	if ((rom_state != EEPROM_Valid) || ((uint16_t)crc != ccrc)) {
		//Current EEPROM is invalid.
		return false;
	}
	return true;
}

void eeprom_load() {
	memcpy(EEPROM_RAMSTART, (const void*) eeprommem, EEPROM_SIZE);
}

void eeprom_save() {
	__ISB();

	HAL_StatusTypeDef status = HAL_FLASH_Unlock();
	uint32_t error = 0;

	uint32_t page = (EROM_BASE - MAIN_BASE) / 0x800;
	uint32_t Bank = FLASH_BANK_1;

	if (page >= 128){
		page -= 128;
		Bank = FLASH_BANK_2;
	}

	// make sure that this structure matches the data-sheet of your chip
	FLASH_EraseInitTypeDef FLASH_EraseInitStruct = {
		.TypeErase = FLASH_TYPEERASE_PAGES,
		.Banks = Bank,
		.Page = page,
		.NbPages = 1
	};

	// clear all flags before you write it to flash
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);

	if (status != HAL_OK)
		return;

	// perform the erase first
	HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &error);

	uint64_t* address = (uint64_t*) eeprommem;
	uint64_t* data = (uint64_t*) EEPROM_RAMSTART;
	uint64_t ccrc = libCRCCalcCRC16(EEPROM_RAMSTART, EEPROM_SIZE / sizeof(uint8_t));
	uint64_t val = EEPROM_Valid;

	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)&crc, ccrc);
	HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)&rom_state, val);

	int i = 0;
	do {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)address, data[i++]);
		address += 1;

		//wait for flash to be ready
		FLASH_WaitForLastOperation(10);
	} while ((void*)address < (EEPROM_START_ADDRESS + EEPROM_SIZE));

	//CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
	HAL_FLASH_Lock();
	FLASH_WaitForLastOperation(100);
	HAL_FLASH_Unlock();

	return;
}


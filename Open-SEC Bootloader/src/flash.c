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
 

#include "flash.h"


uint16_t modFlashEraseNewAppData(uint32_t new_app_size) {
	__ISB();

	HAL_StatusTypeDef status = HAL_FLASH_Unlock();
	uint32_t error = 0;


	// clear all flags before you write it to flash
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);

	if (status != HAL_OK)
		return status;

	for (uint32_t addres = APP_BASE; addres < EROM_BASE; addres += 0x800){
		uint32_t page = (addres - MAIN_BASE) / 0x800;
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



		// perform the erase first

		HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &error);

		if (error != -1){
			return HAL_ERROR;
		}
	}

	HAL_FLASH_Lock();
	FLASH_WaitForLastOperation(100);
	HAL_FLASH_Unlock();

	return HAL_OK;
}

uint16_t modFlashEraseMainAppData(uint32_t new_app_size) {
	__ISB();

	HAL_StatusTypeDef status = HAL_FLASH_Unlock();
	uint32_t error = 0;


	// clear all flags before you write it to flash
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);

	if (status != HAL_OK)
		return status;

	for (uint32_t addres = MAIN_BASE; addres < APP_BASE; addres += 0x800){
		uint32_t page = (addres - MAIN_BASE) / 0x800;
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



		// perform the erase first

		HAL_FLASHEx_Erase(&FLASH_EraseInitStruct, &error);

		if (error != -1){
			return HAL_ERROR;
		}
	}

	HAL_FLASH_Lock();
	FLASH_WaitForLastOperation(100);
	HAL_FLASH_Unlock();

	return HAL_OK;
}


int lastOffset = -1;
uint64_t bufferbyte = 0xFFFFFFFFFFFFFFFF;
int bufferindex = 0;
uint64_t* adress = (uint64_t*)APP_BASE;


uint64_t* addr;
int written = 0;

uint16_t modFlashWriteNewAppData(uint32_t offset, uint8_t *data, uint32_t len) {
	uint16_t returnVal = HAL_OK;

	//Check if length and offset are aligned with and deviseble by blocks of 8 bytes.
	if ((offset % 8 != 0) || (len % 8 != 0)){
		return HAL_ERROR;
	}

	//Detect start of write
	if (offset == 0){
		addr = (uint64_t*)APP_BASE;
		written = 0;
	}

	//Check for sequentiality of offset
	if ((offset + APP_BASE) != addr){
		return HAL_ERROR;
	}

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);

	for (int i = 0; i < len; i += 8) {
		uint64_t dword = 0;
		for (int j = 0; j < 8; j++){
			dword += ((uint64_t)data[i+j]) << j*8;
		}
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)addr, dword) != HAL_OK){
			return HAL_ERROR;
		}
		FLASH_WaitForLastOperation(10);
		addr++;
		written += 8;
	}

	//CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
	HAL_FLASH_Lock();
	FLASH_WaitForLastOperation(100);
	HAL_FLASH_Unlock();

	return HAL_OK;
}

uint16_t modFlashCopyNewAppToMainApp(uint64_t *data, uint32_t len) {

	if (len%8 != 0){
		return HAL_ERROR;
	}

	if(len > (APP_BASE - MAIN_BASE)){
		return HAL_ERROR;
	}

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);

	uint64_t* addr = (uint64_t*)MAIN_BASE;

	for (int i = 0; i < len; i += 8){
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, (uint32_t)addr, data[i/8]) != HAL_OK){
			return HAL_ERROR;
		}
		FLASH_WaitForLastOperation(10);
		addr++;
	}

	//CLEAR_BIT(FLASH->CR, FLASH_CR_PG);
	HAL_FLASH_Lock();
	FLASH_WaitForLastOperation(100);
	HAL_FLASH_Unlock();

}

void modFlashJumpToBootloader(void) {

	__HAL_RCC_USART2_FORCE_RESET();
	HAL_Delay(5);
	__HAL_RCC_USART2_RELEASE_RESET();
	HAL_Delay(5);
	HAL_DeInit();
	HAL_RCC_DeInit();

	// Clear pending interrupts
	SCB->ICSR = SCB_ICSR_PENDSVCLR_Msk;

	// Disable all interrupts
	for(int i = 0;i < 8;i++) {
		NVIC->ICER[i] = NVIC->IABR[i];
	}

	typedef void (*pFunction)(void);
	union
	{
		uint32_t ulValue;
		pFunction pfnPointer;
	} uResetHandler;

	//----------------------------------------------------------------
	// Jump to user application
	//
	//!- calculate jump address: reset handler of new application
	uResetHandler.ulValue = *(uint32_t volatile *) (BOOT_BASE + 4);

	//!- Initialize user application's Stack Pointer
	__set_MSP(BOOT_BASE);
	SCB->VTOR = BOOT_BASE;
	SysTick->CTRL = 0;
	//!- jump to application
	uResetHandler.pfnPointer();
}

void modFlashJumpToMainApplication(void) {
	NVIC_SystemReset();
}


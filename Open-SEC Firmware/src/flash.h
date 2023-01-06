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
 

#ifndef SRC_FLASH_H_
#define SRC_FLASH_H_

#include "stm32g4xx_hal.h"
#include "stdbool.h"
#include "stdint.h"

//#define FLASH_PAGES			  100
//#define BOOTLOADER_BASE		100
//#define MAIN_APP_BASE     0
//#define NEW_APP_BASE      50
//#define NEW_APP_SECTORS   50
//#define NEW_APP_MAX_SIZE	(NEW_APP_SECTORS * (1 << 11))

#define MAIN_BASE 	0x08000000
#define APP_BASE 	0x0803d800
#define EROM_BASE   0x0807b800
#define BOOT_BASE   0x0807c000



uint16_t modFlashEraseNewAppData(uint32_t new_app_size);
uint16_t modFlashEraseMainAppData(uint32_t new_app_size);
uint16_t modFlashWriteNewAppData(uint32_t offset, uint8_t *data, uint32_t len);
uint16_t modFlashCopyNewAppToMainApp(uint64_t *data, uint32_t len);
void     modFlashJumpToBootloader(void);
void     modFlashJumpToMainApplication(void);

#endif /* SRC_FLASH_H_ */

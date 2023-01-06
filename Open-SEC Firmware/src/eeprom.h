/*
 * eeprom.h
 *
 *  Created on: 6 Oct 2021
 *      Author: Tjitte
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdbool.h>
#include "flash.h"

#define __EEPROM __attribute__((__section__(".erom_ram")))
#define __EEPROM_FLASH __attribute__((__section__(".erom_flash")))

#define EEPROM_RAMSTART 		((void*)0x2001f800)
#define EEPROM_START_ADDRESS 	((void*)EROM_BASE)
#define EEPROM_SIZE 			((1<<11)-sizeof(uint64_t)-sizeof(uint64_t))

#define EEPROM_Valid  ((uint64_t)0x00C0FEEBABE02001)


bool eeprom_init();
bool eeprom_valid();
void eeprom_load();
void eeprom_save();


#endif /* EEPROM_H_ */

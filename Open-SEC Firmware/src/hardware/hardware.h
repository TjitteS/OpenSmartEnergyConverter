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
 

#ifndef HARDWARE_HARDWARE_H_
#define HARDWARE_HARDWARE_H_

#include "stm32g4xx_hal.h"
#include "stdio.h"

#define FW_VERSION_MAJOR 1
#define FW_VERSION_MINOR 2

#if defined(HW_REBOOST)
#include "hw_reboost.h"

#elif defined(HW_SECB175)
#include "secb.h"

#elif defined(HW_SECB1754A)
#include "secb1754a.h"

#else
#error "No hardware defined."
#endif


#ifndef HW_TOPOLOGY_BOOST
#ifndef HW_TOPOLOGY_BUCK
#error "No hardware topology defined."
#endif
#endif

#define HW_TEMP_LIM 85.0f
#define HW_MIN_SETPOINT 5000.0f


extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern ADC_HandleTypeDef hadc5;

void hw_setio_output(GPIO_TypeDef * port, uint32_t pin);
void hw_setio_input(GPIO_TypeDef * port, uint32_t pin, uint32_t pull);
void hw_setio_af(GPIO_TypeDef * port, uint32_t pin, uint32_t af);
void hw_setio_analog(GPIO_TypeDef * port, uint32_t pin);

//void hw_io_init(void);
//void hw_adc_init(void);

#endif /* HARDWARE_HARDWARE_H_ */

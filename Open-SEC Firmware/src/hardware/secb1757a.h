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
 

#ifndef HARDWARE_SECB_H_
#define HARDWARE_SECB_H_
#ifdef HW_SECB175_7A

#include "stm32g4xx_hal.h"
#include "stdio.h"


#ifndef HW_NAME
#define HW_NAME "SEC-B175-7A"
#endif

#define HW_TOPOLOGY_BOOST

#define HW_HAS_IOUT_SENSOR
#define HW_HAS_UART
//#define HW_HAS_USB
#define HW_HASID
#define HSEN

#define HW_ADCREF 2900.0f

#define HW_TIMERINDEX 			HRTIM_TIMERINDEX_TIMER_B
#define HW_HRTIM_ADCTRIGGER 	HRTIM_ADCTRIGGEREVENT13_TIMERB_CMP3


//Filters
#define VOLTAGE_IN_FORGETING_FACTOR		0.0f
#define VOLTAGE_OUT_FORGETING_FACTOR	0.0f
#define CURRENT_IN_FORGETING_FACTOR     0.0f
#define CURRENT_PV_FORGETING_FACTOR		0.8f
#define TEMP_FORGETING_FACTOR			0.95f

//Limits
#define HW_LIMIT_HS_VOLTAGE_HARD 184210
#define HW_LIMIT_LS_VOLTAGE_HARD 184210
#define HW_LIMIT_HS_CURRENT_HARD 12000
#define HW_LIMIT_LS_CURRENT_HARD 12000
#define HW_MAX_CURRENT_SETTING   8000.0f

#define HW_CURRENT_LIMIT_CORRECITONFACTOR 1.17f

#define HW_SWITCHINGFREQUENCY  36
#define HW_DEADTIMERISING      111
#define HW_DEADTIMEFALLING     40
#define HW_L                   100e-6
#define HW_RLINT               20e-3

#define HW_ADC_DELAY           0.8f

#define HW_CONTROLLERFREQUENCY 25


#define HW_CLOW                220.0e-6f
#define HW_CHIGH               440.0e-6f

#define HW_Q                   0.8f
#define HW_KLIM                0.4f

//Pinout
#define LED0_Pin 			GPIO_PIN_15
#define LED0_GPIO_Port 		GPIOC
#define LED1_Pin 			GPIO_PIN_14
#define LED1_GPIO_Port 		GPIOC
#define LED2_Pin 			GPIO_PIN_13
#define LED2_GPIO_Port 		GPIOC
#define LED3_Pin 			GPIO_PIN_9
#define LED3_GPIO_Port 		GPIOB

#define ID0_Pin 			GPIO_PIN_4
#define ID0_GPIO_Port 		GPIOC
#define ID1_Pin 			GPIO_PIN_0
#define ID1_GPIO_Port		GPIOB
#define ID2_Pin 			GPIO_PIN_1
#define ID2_GPIO_Port 		GPIOB
#define ID3_Pin 			GPIO_PIN_2
#define ID3_GPIO_Port 		GPIOB

#define VLS_Pin 			GPIO_PIN_1
#define VLS_GPIO_Port 		GPIOA
#define IHS__Pin 			GPIO_PIN_2
#define IHS__GPIO_Port 		GPIOA
#define IHS_A3_Pin 			GPIO_PIN_3
#define IHS_A3_GPIO_Port 	GPIOA
#define VHS_Pin 			GPIO_PIN_7
#define VHS_GPIO_Port 		GPIOA
#define ILS__Pin 			GPIO_PIN_14
#define ILS__GPIO_Port 		GPIOB
#define ILS_B15_Pin 		GPIO_PIN_15
#define ILS_B15_GPIO_Port 	GPIOB
#define Tamb_Pin 			GPIO_PIN_8
#define Tamb_GPIO_Port 		GPIOA
#define Tsink_Pin 			GPIO_PIN_9
#define Tsink_GPIO_Port 	GPIOA
#define P2_PWM_LS_Pin 		GPIO_PIN_10
#define P2_PWM_LS_GPIO_Port GPIOA
#define P2_EN_HS_Pin 		GPIO_PIN_11
#define P2_EN_HS_GPIO_Port 	GPIOA
#define PHSEN_Pin 			GPIO_PIN_12
#define PHSEN_GPIO_Port 	GPIOA
#define PEN_Pin 			GPIO_PIN_15
#define PEN_GPIO_Port 		GPIOA
#define DREN_Pin 			GPIO_PIN_4
#define DREN_GPIO_Port 		GPIOB
#define VDRVPG_Pin 			GPIO_PIN_7
#define VDRVPG_GPIO_Port 	GPIOB
#define RST_Pin 			GPIO_PIN_10
#define RST_GPIO_Port 		GPIOG
#define BOOT0_Pin 			GPIO_PIN_8
#define BOOT0_GPIO_Port 	GPIOB


//const CalibrationData_t defaultCalibration;


void hw_io_init(void);
void hw_adc_init(void);
#endif
#endif /* HARDWARE_SECB_H_ */

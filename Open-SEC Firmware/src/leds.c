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
 
#include "leds.h"
#include "delay.h"
#include "main.h"
#include "mppt.h"

uint32_t effectLastTick = 0;
extern ConverterPhase_t phase;

bool effectBlinkState = false;
uint32_t effectBlinkLastTick;
uint32_t effectLastLimitTick;

void modEffectTask(){
	if(modDelayTick1ms(&effectBlinkLastTick,500)){
		effectBlinkState = !effectBlinkState;
	}

	if(modDelayTick1ms(&effectLastTick, 5)){

		if(phase.fault != Converter_OK){
			LED0_GPIO_Port->BRR = LED0_Pin;
			LED2_GPIO_Port->BRR = LED2_Pin;

			if(effectBlinkState){
				LED1_GPIO_Port->BSRR = LED1_Pin;
			}else{
				LED1_GPIO_Port->BRR = LED1_Pin;
			}
		}
		else {
			if(phase.mode != PhaseMode_CIV){
				LED1_GPIO_Port->BSRR = LED1_Pin;
				effectLastLimitTick = HAL_GetTick();
			}
			else{
				if(modDelayTick1ms(&effectLastLimitTick, 500)){
					LED1_GPIO_Port->BRR = LED1_Pin;
				}
			}

			switch(MpptLastAckion){
				default:
				case MpptAcktionState_Init:
					LED0_GPIO_Port->BRR = LED0_Pin;
					LED2_GPIO_Port->BRR = LED2_Pin;
					break;

				case MpptAcktionState_Up:
					LED0_GPIO_Port->BSRR = LED0_Pin;
					LED2_GPIO_Port->BRR = LED2_Pin;
					break;

				case MpptAcktionState_down:
					LED0_GPIO_Port->BRR = LED0_Pin;
					LED2_GPIO_Port->BSRR = LED2_Pin;
					break;

			}
		}
	}
}

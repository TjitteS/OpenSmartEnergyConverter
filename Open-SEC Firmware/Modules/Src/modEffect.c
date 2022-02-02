/*
 * modEffect.c
 *
 *  Created on: Mar 1, 2020
 *      Author: Tjitte van der ploeg
 */

#include "modEffect.h"
#include "modDelay.h"
#include "main.h"
#include "modMPPT.h"

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

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
 

#ifndef INC_MODMPPT_H_
#define INC_MODMPPT_H_
#include "stdbool.h"

#include "mppt.h"
#include "stdint.h"
#include "stm32g4xx_hal.h"
#include "delay.h"
#include "control.h"
#include "main.h"

#define MPPT_SWEEP_SIZE 128
float mppt_vs[MPPT_SWEEP_SIZE];
float mppt_is[MPPT_SWEEP_SIZE];


typedef enum {
	MpptState_init,
	MpptState_PO,
	MpptState_SweepStart,
	MpptState_Sweep,
	MpptState_ConstantVoltage,
}modMPPTstate_t;

typedef enum {
	MpptMode_PO,
	MpptMode_ConstantVoltage,
}modMPPTmode_t;

typedef enum{
	MpptAcktionState_Init,
	MpptAcktionState_Up,
	MpptAcktionState_down,
}modMPPTlastacktionstate_t;

typedef struct{
	float PO_Stepsize;
	uint32_t PO_Timestep;
	float PO_StepSizeGain;
	float jump_RangeMax;
	float jump_RangeMin;
	float jump_PowerThreshold;
	int jump_Rate;
}modMPPTsettings_t;

modMPPTlastacktionstate_t MpptLastAckion;

void modMPPTtask();
void modMPPTinit(modMPPTsettings_t *settings);

void modMPPTPerturbAndObserve(void);
void modMPPTSetPowerLevel(uint8_t level);
void modMpptStartSweep(float start, float end);

void modMpptSetMode(modMPPTmode_t mode);

#endif /* INC_MODMPPT_H_ */

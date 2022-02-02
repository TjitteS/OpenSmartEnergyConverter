/*
 * modMPPT.h
 *
 *  Created on: 17 Jun 2018
 *      Author: Tjitte van der Ploeg
 */

#ifndef INC_MODMPPT_H_
#define INC_MODMPPT_H_

#include "modMPPT.h"
#include "stdint.h"
#include "stm32g4xx_hal.h"
#include "modDelay.h"
#include "stdbool.h"
#include "modConverter.h"
#include "main.h"

#define MPPT_SWEEP_SIZE 32
int16_t mppt_vs[MPPT_SWEEP_SIZE];
int16_t mppt_is[MPPT_SWEEP_SIZE];


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
	float PO_maxgradient;
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

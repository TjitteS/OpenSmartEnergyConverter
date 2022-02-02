/*
 * modMPPT.h
 *
 *  Created on: 17 Jun 2018
 *      Author: Tjitte van der Ploeg
 */

#include "modMPPT.h"
#include "modCommands.h"
#include <stdlib.h>

uint32_t lastSPsw;
uint32_t lastsweep;


extern UART_HandleTypeDef huart1;

modMPPTsettings_t * modMpptsettings;
modMPPTstate_t currentmode;

float pv;
float pp;

int mpptSweepIndex = 0;
float mpptSweepStepsize = 1.0e3;
float mpptSweepSP;

int oscillationnumber = 0;
float changedStepSize = 0;
bool justjumped = 0;

//float max = 21.0f;
//float min = 10.0f;
//int oscillationthreshold = 20;
//float maxgradient = 5.0f;
//float powerthreshold = 5.0f;

void modMPPTinit(modMPPTsettings_t *settings) {
	modMpptsettings = settings;
	currentmode = MpptState_init;
	modConverterPWMOutputDisable();

	MpptLastAckion = MpptAcktionState_Init;
}


void modMPPTtask() {
	if (modDelayTick1ms(&lastsweep, modMpptsettings->PO_Timestep)) {
		switch(currentmode){
		case MpptState_init:

			if(meter.Vin > meter.Vout){
				modConverterSetInputVoltage(1000.0f*(meter.Vout*0.5f));
			}else{
				modConverterSetInputVoltage(1000.0f*roundf(meter.Vin) - modMpptsettings->PO_Stepsize);
			}
			currentmode = MpptState_PO;
			modConverterPWMOutputEnable();

			break;
		case MpptState_PO:
			modMPPTPerturbAndObserve();
			//phase.Vsp = 10000;
			break;
		case MpptState_SweepStart:
			mpptSweepIndex = 0;
			modConverterSetInputVoltage(mpptSweepSP);
			currentmode = MpptState_Sweep;
			break;

		case MpptState_Sweep:
			mppt_is[mpptSweepIndex] = (int16_t)round(1.0e3f*meter.Ipv);
			mppt_vs[mpptSweepIndex] = (int16_t)round(1.0e2f*meter.Vin);

			mpptSweepSP += mpptSweepStepsize;
			modConverterSetInputVoltage(mpptSweepSP);

			mpptSweepIndex++;

			if(mpptSweepIndex >= MPPT_SWEEP_SIZE){
				currentmode = MpptState_init;
				modCommandsSendSweep();
			}

			break;
		case MpptState_ConstantVoltage:
		default:
			break;

		}
	}
}

float randomf(float min, float max){
	float f = ((float)(rand()%(1<<12))) / ((float)(1<<12));
	return f*(max-min) + min;
}

void modMPPTPerturbAndObserve(){
	float Vsp = modConverterGetInputVoltage();

	switch(modConverterGetMode()){
	case PhaseMode_MinInputCurrent:
		Vsp -= modMpptsettings->PO_Stepsize;
		modConverterSetInputVoltage(Vsp);
		return;
	case PhaseMode_CIC:
		Vsp += modMpptsettings->PO_Stepsize;
		modConverterSetInputVoltage(Vsp);
		return;

	case PhaseMode_COC:
	case PhaseMode_COV:
	case PhaseMode_Fault:
		//No setpoint voltage setable
		return;

	case PhaseMode_CIV:
		//Contineu to PO
		break;
	}

	float p = meter.Power;
	float v = meter.Vin;
	float dv = v - pv;
	float dp = p - pp;
	float jump = 0;

	float dpdv = 0;
	if(dv != 0){
		dpdv = dp/dv;
	}
	else {
		//Stuck in some kind of limit
		Vsp -= modMpptsettings->PO_Stepsize;
		modConverterSetInputVoltage(Vsp);
	}

	//If the previous iteration was a random jump, check if the power did not decrease too much.
	if (justjumped){
		justjumped = false;

		//Jump back if the power is not enough.
		if(p <  (pp - modMpptsettings->jump_PowerThreshold)){
			Vsp = changedStepSize;
		}
	}

	//Start P&O right away.
	//If oscillationthreshold is zero, do  not ever jump.
	if ((oscillationnumber > modMpptsettings->jump_Rate) && (modMpptsettings->jump_Rate != 0)){
		changedStepSize  =  Vsp;
		//Deciding the jump points (THIS EQUATION is GENERALIZED)
		//jump = ((float)(rand()%((int)(0.7*(max-5))))) + ((max-5.0f)*0.3f);
		jump = randomf(modMpptsettings->jump_RangeMin, modMpptsettings->jump_RangeMax);

		//save the current positon, so we can jump back afterwards.
		changedStepSize  =  Vsp;

		//Vsp = jump*1000.0f;
		Vsp = jump;

		oscillationnumber = 0;
		justjumped = true;
	}
	else {
		float stepsize;
		if(modMpptsettings->PO_maxgradient > 0.0f){
			stepsize = modMpptsettings->PO_Stepsize * (1.0f+fabsf(dpdv/modMpptsettings->PO_maxgradient));
		}else{
			stepsize = modMpptsettings->PO_Stepsize;
		}

		if(dpdv > 0){
			MpptLastAckion = MpptAcktionState_Up;
			Vsp += stepsize;
		}else{
			MpptLastAckion = MpptAcktionState_down;
			Vsp -= stepsize;
		}

	}

	pv = v;
	pp=p;

	oscillationnumber ++;
	modConverterSetInputVoltage(Vsp);
}



void modMpptStartSweep(float start, float end){
	mpptSweepStepsize = (end-start)/(MPPT_SWEEP_SIZE-1);
	mpptSweepSP = start;
	currentmode = MpptState_SweepStart;
	MpptLastAckion = MpptAcktionState_Init;
}

void modMpptSetMode(modMPPTmode_t mode){
	switch(mode){
	case MpptMode_ConstantVoltage:
		currentmode = MpptState_ConstantVoltage;
		break;
	default:
	case MpptMode_PO:
		currentmode = MpptState_init;
		break;
	}
}


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
 

#include "mppt.h"
#include "commands.h"
#include <stdlib.h>

uint32_t lastSPsw;
uint32_t lastsweep;


extern UART_HandleTypeDef huart1;
extern ConverterSettings_t settings;


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

uint32_t tsweepstart;


void modMPPTtask() {
	if (modDelayTick1ms(&lastsweep, modMpptsettings->PO_Timestep)) {
		switch(currentmode){
		case MpptState_init:

			control_set_setpoint(control_get_regulated_voltage());
			currentmode = MpptState_PO;
			modConverterPWMOutputEnable();

			break;
		case MpptState_PO:
			modMPPTPerturbAndObserve();
			//phase.Vsp = 10000;
			break;
		case MpptState_SweepStart:
			mpptSweepIndex = 0;
			control_set_setpoint(mpptSweepSP);
			currentmode = MpptState_SweepStartupdelay;
			tsweepstart = HAL_GetTick();
			break;

		case MpptState_SweepStartupdelay:
			if ((HAL_GetTick() - tsweepstart) > 200){
				currentmode = MpptState_Sweep;
			}
			break;

		case MpptState_Sweep:

			if(modConverterGetMode() != PhaseMode_CIC){
				mppt_is[mpptSweepIndex] = control_get_regulated_current();
				mppt_vs[mpptSweepIndex] = control_get_regulated_voltage();

				mpptSweepSP += mpptSweepStepsize;
				control_set_setpoint(mpptSweepSP);
				mpptSweepIndex++;

				if(mpptSweepIndex >= MPPT_SWEEP_SIZE){
					currentmode = MpptState_init;
					modCommandsSendSweep();
				}
			}


			break;
		case MpptState_ConstantVoltage:
			break;

		case MpptState_Disable:
		default:
			modConverterPWMOutputDisable();
			break;



		}
	}
}

float randomf(float min, float max){
	float f = ((float)(rand()%(1<<12))) / ((float)(1<<12));
	return f*(max-min) + min;
}

void modMPPTPerturbAndObserve(){
	float Vsp = control_get_setpoint();
	float i = control_get_regulated_current();
	float v = control_get_regulated_voltage();
	float p = i*v;

	switch(modConverterGetMode()){
	case PhaseMode_MinInputCurrent:
		//Vsp -= modMpptsettings->PO_Stepsize;
		Vsp = 1e3*control_get_regulated_voltage() - (0.1*modMpptsettings->PO_Stepsize);
		control_set_setpoint(Vsp );
		pv = v;
		pp = p;
		return;
	case PhaseMode_TD:
	case PhaseMode_CIC:
		Vsp += modMpptsettings->PO_Stepsize;
		control_set_setpoint(Vsp);
		pv = v;
		pp=p;
		return;

	case PhaseMode_COC:
	case PhaseMode_COV:
	case PhaseMode_Fault:
		//No setpoint voltage setable
		pv = v;
		pp=p;
		return;

	case PhaseMode_CIV:
		//Contineu to PO
		break;
	}



	float dv = v - pv;
	float dp = p - pp;

	float dpdv = 0;
	if(dv != 0){
		dpdv = dp/dv;
	}
	else {
		//Stuck in some kind of limit
		Vsp -= modMpptsettings->PO_Stepsize;
		control_set_setpoint(Vsp);
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
	//If jump rate is zero, do  not ever jump.
	if ((oscillationnumber > modMpptsettings->jump_Rate) && (modMpptsettings->jump_Rate != 0)){

		//save the current position, so we can jump back afterwards.
		changedStepSize  =  Vsp;

		Vsp = randomf(HW_MIN_SETPOINT, settings.LowSideVoltageLimitSoft);

		oscillationnumber = 0;
		justjumped = true;
	}
	else {
		float stepsize;
		if(modMpptsettings->PO_StepSizeGain > 0.0f){
			stepsize = modMpptsettings->PO_Stepsize * (1.0f+(fabsf(dpdv)*modMpptsettings->PO_StepSizeGain));

			//Maximize step size to some reasonable number.
			if (stepsize > 5000) stepsize = 5000;


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
	control_set_setpoint(Vsp);
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
	case MpptMode_PO:
		currentmode = MpptState_init;
		break;

	default:
	case MpptMode_Disable:
		currentmode = MpptState_Disable;
	}
}


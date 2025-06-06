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
uint32_t laststep;
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
int mpptSweepSize = MPPT_SWEEP_SIZE;

int oscillationnumber = 0;
float changedStepSize = 0;
bool justjumped = 0;
uint32_t tsweepstart;

uint32_t tinit;

uint32_t loopdelay;

void modMPPTinit(modMPPTsettings_t *s) {
	modMpptsettings = s;
	modConverterPWMOutputDisable();

	if (settings.outputEnalbeOnStartup){
		currentmode = MpptState_init;
	} else{
		currentmode = MpptState_Disable;
	}

	modConverterPWMOutputDisable();

	MpptLastAckion = MpptAcktionState_Init;

	loopdelay = modMpptsettings->PO_Timestep;
}


void modMPPTtask() {

	if(modMpptsettings->Sweep_eneable){
		if(modDelayTick1ms(&lastsweep, modMpptsettings->Sweep_interval)){
			if(modMpptsettings->Sweep_direction == MPPT_SWEEP_UP)
				 modMpptStartSweep(HW_MIN_SETPOINT, settings.LowSideVoltageLimitSoft, modMpptsettings->Sweep_datapoints);
			else
				modMpptStartSweep(settings.LowSideVoltageLimitSoft, HW_MIN_SETPOINT, modMpptsettings->Sweep_datapoints);
		}
	}

	if (modDelayTick1ms(&laststep, loopdelay)) {
		switch(currentmode){
		case MpptState_init:
			tinit = HAL_GetTick();
			currentmode = MpptState_delay;
			break;

		case MpptState_delay:
			if(HAL_GetTick() - tinit > settings.startupDelay){
				control_set_setpoint(control_get_regulated_voltage());
				currentmode = MpptState_PO;
				modConverterPWMOutputEnable();
			}


		case MpptState_PO:
			modMPPTPerturbAndObserve();
			scope_trigger();

			loopdelay = modMpptsettings->PO_Timestep;

			break;
		case MpptState_SweepStart:
			mpptSweepIndex = 0;
			control_set_setpoint(mpptSweepSP);
			currentmode = MpptState_SweepStartupdelay;
			tsweepstart = HAL_GetTick();

			loopdelay = modMpptsettings->Sweep_timestep;

			break;

		case MpptState_SweepStartupdelay:
			if ((HAL_GetTick() - tsweepstart) > modMpptsettings->PO_Timestep){
				currentmode = MpptState_Sweep;
			}
			break;

		case MpptState_Sweep:

			//if(modConverterGetMode() != PhaseMode_CIC){
				mppt_is[mpptSweepIndex] = control_get_regulated_current();
				mppt_vs[mpptSweepIndex] = control_get_regulated_voltage();

				mpptSweepSP += mpptSweepStepsize;
				control_set_setpoint(mpptSweepSP);


				if(mpptSweepIndex >= mpptSweepSize){
					float vmpp = settings.LowSideVoltageLimitSoft;
					float pmax = 0.0f;
					for(int i = 0; i < mpptSweepSize; i++){
						float p = mppt_is[i]*mppt_vs[i];
						if(p > pmax){
							pmax = p;
							vmpp = mppt_vs[i]*1000.0f;
						}
					}

					control_set_setpoint(vmpp);
					currentmode = MpptState_PO;
					modCommandsSendSweep(mpptSweepSize);

					if(modMpptsettings->Sweep_publishOnCan){
						modCANSendSweep(mppt_is, mppt_vs, mpptSweepSize);

					}

				}

				mpptSweepIndex++;
			//}


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



void modMpptStartSweep(float start, float end, int size){
	//Only allow sweep when tracking.
	if(currentmode != MpptState_PO){
		return;
	}

	mpptSweepSize = size;
	if (mpptSweepSize < 4){
		mpptSweepSize = 4;
	}

	if (mpptSweepSize > MPPT_SWEEP_SIZE){
		mpptSweepSize = MPPT_SWEEP_SIZE;
	}
	mpptSweepStepsize = (end-start)/(mpptSweepSize-1);
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


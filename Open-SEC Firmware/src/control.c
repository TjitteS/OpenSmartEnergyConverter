/*
 **	Copyright 2021 Tjitte van der Ploeg	tjitte@tpee.nl
 **
 **	This file is part of the OpenBoost firmware.
 **	The OpenBoost firmware firmware is free software: you can redistribute
 **	it and/or modify it under the terms of the GNU General Public License
 **	as published by the Free Software Foundation, either version 3 of the
 **	License, or (at your option) any later version. The VESC firmware is
 **	distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 **	without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 **	PARTICULAR PURPOSE. See the GNU General Public License for more details.
 **
 **	You should have received a copy of the GNU General Public License along
 **	with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>

#include "control.h"
#include "testing.h"
#include "crc.h"
#include "string.h"


#define METER_EMA 0.95f
#define EMA(_A_, _B_, _F_)	_A_ = (_F_ * _A_) + ((1 - _F_) * _B_)
#define pi 3.14159265359f

ConverterPhase_t phase;
ConverterMueasurements_t meter;
ConverterScope_t scope;

CalibrationData_t cal;
ConverterSettings_t settings;

float Ts; //Sample period, s
float ControllerR;
extern uint32_t adc5data[6];


void control_init(const ConverterSettings_t* s, const CalibrationData_t * c) {
	//Check if settings are valid.
	if(control_check_parameters(s,c)){
		phase.fault = Converter_ConfigError;
	}

	//Even though there is a config fault, push on anyway. Do not enable output.
	settings=*s;
	cal=*c;

	phase.HSEpin  = PHSEN_Pin;
	phase.HSEport = PHSEN_GPIO_Port;
	phase.PENpin  = PEN_Pin;
	phase.PENport = PEN_GPIO_Port;
	phase.timerindex = HRTIM_TIMERINDEX_TIMER_B;
	phase.LimLEDpin = LED1_Pin;
	phase.LimLEDport= LED1_GPIO_Port;

	phase.Vsp = 10000;
	phase.Vhighlim = settings.HighSideVoltageLimitSoft;
	phase.Iindlim = settings.LowSideCurrentMaxLimitSoft;
	phase.Ihighlim = settings.HighSideCurrentLimitSoft;

	modConverterPWMOutputDisable();

	pwm_init(HW_SWITCHINGFREQUENCY, HW_CONTROLLERFREQUENCY, HW_DEADTIMERISING, HW_DEADTIMEFALLING, HW_ADC_DELAY);

	analog_init();

	Ts = pwm_GetControllerPeriod();
	ControllerR = sqrtf(HW_L/HW_CLOW)/ HW_Q  - HW_RLINT;

	HAL_Delay(100);
}

float t;

void control_controlloop(ConverterPhase_t* p){

	//LED3_GPIO_Port->BSRR = LED3_Pin;

	bool TemperatureLimited = false;
	bool OutputCurrentLimited = false;
	bool OutputVoltageLimited = false;

	//Reset the mode variable.
	p->mode = PhaseMode_CIV;



	//Ipv observer
	float dvdt = (p->Vlow - p->Vlowm1) / (float)Ts;
	float Ilowest = dvdt*HW_CLOW  + (p->Iind + p->Iindm1)/2.0f;
	p->Vlowm1 = p->Vlow;
	p->Iindm1 = p->Iind;
	EMA(p->Ilow, Ilowest, CURRENT_PV_FORGETING_FACTOR);

	//Time delay compensation
	if (p->pwm_enabled){


		p->Iind_pred  = p->Iind // Zeroth order taylor term
				+ HW_delay/HW_L*(p->Vlow - (1.0f-p->dutycycle)*p->Vhigh - HW_RLINT*p->Iind) //1st order taylor term
				+ HW_delay*HW_delay/(HW_L*HW_CLOW)*(p->Ilow - p->Iind); //second order (reduced) taylor term.

		p->Vhigh_pred = p->Vhigh;// + HW_delay/HW_CHIGH*(p->dutycycle*p->Iind  -  p->Ihigh);
		p->Vlow_pred  = p->Vlow  + HW_delay/HW_CLOW *(p->Ilow - p->Iind);
	}else{
		p->Iind_pred  = p->Iind
				+ 0.5f*HW_delay/HW_L*(p->Vlow - (1.0f-p->dutycycle)*p->Vhigh - HW_RLINT*p->Iind); //1st order taylor term
		p->Vhigh_pred = p->Vhigh;// + HW_delay/HW_CHIGH*(- p->Ihigh);
		p->Vlow_pred  = p->Vlow; // + HW_delay/HW_CLOW *(p->Ilow);
	}

	//These variables need to be set by the controller.
	float Vn = 0.0f;
	float Ilim = 0.0f;

#ifdef HW_TOPOLOGY_BOOST

	float Vnn = p->Vsp - (p->Ilow *(ControllerR+HW_RLINT));

	Ilim = p->Iindlim;


	//Temperature de-rating
	if (p->TemperatureHeatsink > settings.TemperatureLimitStart){
		t = (p->TemperatureHeatsink - settings.TemperatureLimitStart)/(settings.TemperatureLimitEnd - settings.TemperatureLimitStart);
		Ilim = p->Iindlim * (1 - t);
		if (Ilim < 0){
			Ilim = 0;
		}
		TemperatureLimited = true;
	}

	//Output voltage limit
	float Ioutlim = HW_KLIM_VOUT*HW_CHIGH*(p->Vhighlim - p->Vhigh_pred)/(Ts);// + 0.5f*p->Ihigh;

	if(Ioutlim > p->Ihighlim){
		Ioutlim =  p->Ihighlim;
		OutputCurrentLimited = true;
	}

	float Ilimmaxvout = Ioutlim/(1.0f-p->dutycycle);

	//Limit the input current to higher than 0, to prevent current under-shoot.
	//if(Ilimmaxvout < 0.0){
	//	Ilimmaxvout = 0.0f;
	//}

	if(Ilim > Ilimmaxvout){
		Ilim = Ilimmaxvout;
		OutputVoltageLimited = true;
	}

	p->Ilimvout = Ilim / 1000.0f;


	//Current limit
	//float Vnlimup = -p->Vlow - (settings.RLint * p->Iind) + ((settings.Klim * settings.L /Ts)*(settings.PhaseCurrentMin - p->Iind) );
	//float Vnlimlo = -p->Vlow - (settings.RLint * p->Iind) + ((settings.Klim * settings.L /Ts)*(Ilim - p->Iind) );
	float Vnlimup = +p->Vlow_pred - (HW_RLINT * p->Iind_pred) - (HW_KLIM*HW_L*(settings.LowSideCurrentMinLimitSoft - p->Iind_pred)/Ts)
		 + Ts/(2*HW_CLOW)*(p->Ilow - p->Iind_pred);
	float Vnlimlo = +p->Vlow_pred - (HW_RLINT * p->Iind_pred) - (HW_KLIM*HW_L*(Ilim - p->Iind_pred)/Ts )
		+ Ts/(2*HW_CLOW)*(p->Ilow - p->Iind_pred);

	//Unlimited controller
	Vn = Vnn + (p->Iind_pred*ControllerR);

	//Limit Vnn to limit phase current
	if(Vn > Vnlimup){
		Vn = Vnlimup;
		p->mode = PhaseMode_MinInputCurrent;
	}

	if(Vn < Vnlimlo) {
		Vn = Vnlimlo;
		p->mode = PhaseMode_CIC;
	}

	//Select correct operating mode

	if(p->mode == PhaseMode_CIC){
		if(TemperatureLimited){
			p->mode = PhaseMode_TD;
		}

		if(OutputVoltageLimited){
			if(OutputCurrentLimited)p->mode = PhaseMode_COC;
			else p->mode = PhaseMode_COV;
		}
	}


#elif defined(HW_TOPOLOGY_BUCK)

	float Ioutsp = 0.0f;


	static float vsp = 42000.0f;


	if((p->Vlow*1.17f) > vsp){
		EMA(vsp,(p->Vlow*1.17f),0.99f);
	}else{
		EMA(vsp,p->Vsp,0.99f);
	}

	Ioutsp = 0.5f * settings.Klim * settings.Chigh *(vsp - p->Vhigh)/(Ts) + p->Ihigh;

	//if(p->Vsp > vsp){
	//	Ioutsp = 0.7f * settings.Klim * settings.Chigh *(p->Vsp - p->Vhigh)/(Ts) + p->Ihigh;
	//}
	//else{
	//	Ioutsp = 0.7f * settings.Klim * settings.Chigh *(vsp - p->Vhigh)/(Ts) + p->Ihigh;
	//}

    float Isp = Ioutsp/(1.0f - p->dutycycle);


    Ilim = -settings.LowSideCurrentMaxLimitSoft;
    p->Ilimvout = (-0.5f*settings.Klim*settings.Clow*(settings.LowSideVoltageLimitSoft - p->Vlow)/Ts) +p->Ilow;
    bool Vhighlim = false;
    if (Ilim < p->Ilimvout){
    	Vhighlim = true;
    	//Constand low-side voltage
    	Ilim = p->Ilimvout;
    	if(Ilim > 0.0f)Ilim = 0.0f;
    }


    if(Isp > -settings.LowSideCurrentMinLimitSoft){
		Isp = -settings.LowSideCurrentMinLimitSoft;

		p->mode = PhaseMode_MinInputCurrent;
	}
    else if(Isp < Ilim){
		Isp = Ilim;
		if(Vhighlim){
			p->mode = PhaseMode_COV;
		}
		else{
			p->mode = PhaseMode_COC;
		}
	}
    Vn = p->Vlow + (settings.RLint * p->Iind) - (settings.Klim*settings.L*(Isp - p->Iind)/Ts );

    //Ilim = Isp;
#endif

	float Dn = Vn / p->Vhigh_pred;

	if (Dn > 0.999f){
		Dn = 0.999f;
	}
	if (Dn < 0.001f){
		Dn = 0.001f;
	}

	p->dutycycle = 1.0f-Dn;



	//If the output is smaller then the input voltage, make sure the duty cycle does not drop to zero.
	if((p->Vlow*1.05f) > p->Vhigh){
		//if(p->dutycycle < 0.15f)p->dutycycle = 0.15f; TODO TODO
	}


	//Control PWM enable. Startup disabled to calculate initial pwm, then depending on output current.
	//If the upper current limit is lower then the lower current limit, set the duty cycle to 0
	//This ensure full shutdown when the power level gets towards zero.
	bool disable_voutlim = false;
	if(Ilim < 0){
		p->HSEport->BRR = p->HSEpin;
		p->PENport->BRR = p->PENpin;

		disable_voutlim = true;
		p->pwm_enabled =  false;
	}else{
		if(Ilim >  HW_IOUT_EN_HYST){
			if(p->enabled){

				p->pwm_enabled =  true;
			}
		}
	}

	if(p->fault != Converter_OK){
		modConverterPWMOutputDisable();
		scope_trigger_fault();
	}else{


		//Set PWM



		//Deadtime compensation
		float dI = (p->Vlow_pred*p->dutycycle)/ (HW_SWITCHINGFREQUENCY*1e3*HW_L);
		float dtcomp = 0;

		if (p->Iind_pred +  dI/2 > 0.0f){
			dtcomp += HW_DEADTIMERISING*1e-6*HW_SWITCHINGFREQUENCY;
		}
		else{
			dtcomp -= HW_DEADTIMERISING*1e-6*HW_SWITCHINGFREQUENCY;
		}

		if (p->Iind_pred -  dI/2 > 0.0f){
			dtcomp += HW_DEADTIMEFALLING*1e-6*HW_SWITCHINGFREQUENCY;
		}
		else{
			dtcomp -= HW_DEADTIMEFALLING*1e-6*HW_SWITCHINGFREQUENCY;
		}


		pwm_setDuty(p->dutycycle + dtcomp);

		if(p->pwm_enabled){

			#ifndef SIMULATION
			p->PENport->BSRR = p->PENpin;
			#ifdef HSEN

			if(p->dutycycle < 0.01f){
				//Do not, ever put on the high side FET continuesly.
				//e.g. when the duty cycle is zero.
				p->HSEport->BRR = p->HSEpin;
			}

			#ifdef HW_TOPOLOGY_BOOST
			else if(p->Iind > settings.PhaseHighSideEnableCurrent){
				if(disable_voutlim == false){
					p->HSEport->BSRR = p->HSEpin;
				}
			}
			#elif defined HW_TOPOLOGY_BUCK
			else if(disable_voutlim == false){
				p->HSEport->BSRR = p->HSEpin;
			}
			#endif
			else{
				p->HSEport->BRR = p->HSEpin;
			}

			#else
					p->HSEport->BRR = p->HSEpin;
			#endif
			#endif
		}else{
			p->HSEport->BRR = p->HSEpin;
			p->PENport->BRR = p->PENpin;
		}
	}

	//LED3_GPIO_Port->BRR = LED3_Pin;


	//Do Lower priority duties now.

	//Calculate converter input and output currents
	//p->Power = (p->Ihigh*p->Vhigh)*1.0e-6f;
	p->Power = (p->Ilow*p->Vlow)*1.0e-6f;

	if(p->Power){
		//p->eff = (p->Power)/(p->Ilow*p->Vlow*1.0e-6f);
		p->eff = ((p->Ihigh*p->Vhigh)*1.0e-6f)/(p->Power);
	}else{
		p->eff = 0;
	}

	EMA(meter.Iind, p->Iind*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Ihigh, p->Ihigh*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Ilow, p->Ilow*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Vlow, p->Vlow*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Vhigh, p->Vhigh*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Eff, p->eff,settings.meterfilterCoeficient);
	EMA(meter.TemperatureAmbient, p->TemperatureAmbient,settings.meterfilterCoeficient);
	EMA(meter.TemperatureHeatsink, p->TemperatureHeatsink,settings.meterfilterCoeficient);
	meter.Power = meter.Vlow*meter.Ilow;


	if(scope.running){
		if((scope.dividerindex++ >= scope.divider) || (scope.divider <= 1)){
			scope.dividerindex = 0;

			for(int ch = 0; ch < CONVERTER_SCOPE_CHANNELS; ch++){
				float tempval = 0.0f;

				switch (scope.channel[ch].source){
				case SourceIndex_Iind:
					tempval = p->Iind / 1.0e3f;
					break;
				case SourceIndex_Ihigh:
					tempval = p->Ihigh / 1.0e3f;
					break;
				case SourceIndex_Vlow:
					tempval = p->Vlow / 1.0e3f;
					break;
				case SourceIndex_Vhigh:
					tempval = p->Vhigh / 1.0e3f;
					break;
				case SourceIndex_Ilow:
					tempval = p->Ilow / 1.0e3f;
					break;
				case SourceIndex_Power:
					tempval = p->Power;
					break;
				case SourceIndex_Eff:
					tempval = p->eff;
					break;
				case SourceIndex_Iind_Filtered:
					tempval = meter.Iind;
					break;
				case SourceIndex_Ihigh_Filtered:
					tempval = meter.Ihigh;
					break;
				case SourceIndex_Vlow_Filtered:
					tempval = meter.Vlow;
					break;
				case SourceIndex_Vhigh_Filtered:
					tempval = meter.Vhigh;
					break;
				case SourceIndex_Ilow_Filtered:
					tempval = meter.Ilow;
					break;
				case SourceIndex_Power_Filtered:
					tempval = meter.Power;
					break;
				case SourceIndex_Eff_Filtered:
					tempval = meter.Eff;
					break;
				}

				scope.channel[ch].samples[scope.writeindex] = tempval;
			}
			scope.writeindex++;

			if (scope.writeindex >= scope.samples){
				scope.writeindex = 0;
			}

			if(scope.trigered){
				int stopindex = scope.triggerindex - scope.pretrigger;
				if (stopindex < 0){
					stopindex = scope.samples + stopindex;
				}

				if(scope.writeindex == stopindex){
					scope.running = false;
				}
			}
		}
	}

#ifdef SIMULATION
	modTestingSimstep(&simstate, Ts/1,&phase);
#endif
}

bool control_check_parameters(ConverterSettings_t* s, CalibrationData_t * c){
	bool error = false;

	//Check calibration CRC
	uint16_t crc = libCRCCalcCRC16((unsigned char *)c, sizeof(CalibrationData_t) - sizeof(uint16_t) - 2);
	if (crc != c->calcrc){
		error = true;
	}

	//Check Firmware hwname against stored HW name
	#ifndef SIMULATION // In simulation mode, this does not matter.
	if (strcmp(HW_NAME, c->HardwareName) != 0){
		error = true;
	}
	#endif

	//Check firmware verison with calibration compatibility


	//Check UUID with calibraiton id, t check if calibration is done for this hardware

	//float fres = sqrtf(1/(2*pi*pi*HW_L*c->Clow))*1.0e-3f;
	//if(c->ControllerFrequency > 45.0f)error = true;
	//if(c->ControllerFrequency < (8*fres))error = true;
	//if(c->SwitchingFrequency  < (5*fres))error = true;
	//if(c->calibrated == false) error = true;
	//if(c->Q > 2.0f)error = true;

	//Check deadtime values
	//if(c->DeadTimeRising  > 188.0)error = true;
	//if(c->DeadTimeRising  < 5)error = true;
	//if(c->DeadtimeFalling > 188.0)error = true;
	//if(c->DeadtimeFalling < 5)error = true;

	return error;
}

void control_disable(void){
	phase.HSEport->BRR = phase.HSEpin;
	phase.PENport->BRR = phase.PENpin;
	phase.pwm_enabled =  false;
	phase.dutycycle = 0;
	phase.enabled = false;
	pwm_disable();
}


inline void modConverterPWMOutputDisable(){
#ifndef SIMULATION
	phase.PENport->BRR = phase.PENpin;
	phase.HSEport->BRR = phase.HSEpin;
	DREN_GPIO_Port->BRR = DREN_Pin;
#endif
	phase.enabled = false;
	phase.pwm_enabled =  false;
}

inline void modConverterPWMOutputEnable() {
	//Make sure MPPT is not enabled when a fault is active.
	if(phase.fault == Converter_OK){
		if(settings.outputEnable){
	#ifndef SIMULATION
			//phase.PENport->BSRR = phase.PENpin;
			DREN_GPIO_Port->BSRR = DREN_Pin;
	#endif
			phase.enabled = true;
			//phase.pwm_enabled =  true;
		}else{
			modConverterPWMOutputDisable();
		}
	}
}

float convertTemperature(uint32_t data){
	float vt = ((float)data) * HW_ADCREF * 0.001f / ((float)(1<<16));
	float i = ((3.24f-vt) / cal.Temperature_R);
	float r = vt / i;


	float inverseKelvin = (1.0f / (cal.Temperature_Ref+273.16f)) + (logf(r / cal.Temperature_R))/cal.Temperature_B;

	return (1.0f / inverseKelvin) - 273.16f;
	return vt;
}


float noise(float amp){
	return (((float)rand()/(float)__RAND_MAX)-0.5f)*amp;
	//return 0.0f;
}


void control_convert_vls(uint32_t raw){
	float V = 0;

	#ifdef SIMULATION
	V = 1.0e3f*(simstate.Vlow + noise(0.05f));
	#else
	V = ((((float)raw) * cal.InputVoltageGain * HW_ADCREF) / (float)0x1000) + cal.InputVoltageOffset;
	#endif

	if(fabsf(V) > HW_LIMIT_LS_VOLTAGE_HARD){
		phase.fault = Converter_InputOverVolt;
	}
	phase.Vlowm1 = phase.Vlow;
	EMA(phase.Vlow, V , VOLTAGE_IN_FORGETING_FACTOR);

	//modConverterIpvObserver(phase);
}

void control_convert_vhs(uint32_t raw){
	float V = 0.0;
	#ifdef SIMULATION
	V = 1.0e3f*(simstate.Vhigh + noise(0.05f));
	#else
	V = ((((float)raw) * cal.OutputVoltageGain * HW_ADCREF) / (float)0x1000) + cal.OutputVoltageOffset;
	#endif

	if(fabsf(V) > HW_LIMIT_HS_VOLTAGE_HARD){
		phase.fault = Converter_OutputOverVolt;
	}
	EMA(phase.Vhigh, V , VOLTAGE_OUT_FORGETING_FACTOR);

}

void control_convert_iind(uint32_t raw){
	float I = 0.0;
	#ifdef SIMULATION
	I = 1.0e3f*(simstate.Iind + noise(0.05f));
	#else
	I = ((((float)raw-(float)0x800) * cal.InputCurrentGain *2.0f* HW_ADCREF) / (float)0x1000) + cal.InputCurrentOffset;
	#endif

	if(I > HW_LIMIT_LS_CURRENT_HARD){
		phase.fault = Converter_InputOverCurrent;
	}else if(I < -HW_LIMIT_LS_CURRENT_HARD){
		phase.fault = Converter_InputUnderCurrent;
	}


	EMA(phase.Iind,I, CURRENT_IN_FORGETING_FACTOR);
}

void control_convert_ihs(uint32_t raw){
	float I = 0.0f;
	#ifdef SIMULATION
	I = 1.0e3f*(simstate.Ihigh + noise(0.05f));
	#else
	I = ( ((float)raw-(float)0x800) * cal.OutputCurrentGain *2.0f* (HW_ADCREF / (float)0x1000)) + cal.OutputCurrentOffset;
	#endif
	if(I > HW_LIMIT_HS_CURRENT_HARD){
		phase.fault = Converter_OutputOverCurrent;
	}
	EMA(phase.Ihigh,I, CURRENT_IN_FORGETING_FACTOR);
}

void convertAdc5(uint32_t* data){
	EMA(phase.TemperatureMCU, __LL_ADC_CALC_TEMPERATURE((uint32_t)HW_ADCREF,data[0],LL_ADC_RESOLUTION_12B), TEMP_FORGETING_FACTOR);
	EMA(phase.TemperatureHeatsink, convertTemperature(data[2]), TEMP_FORGETING_FACTOR);
	EMA(phase.TemperatureAmbient, convertTemperature(data[1]), TEMP_FORGETING_FACTOR);
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc){
	if(hadc->Instance == ADC5){
		convertAdc5(&adc5data[3]);
	}
}
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc){
	if(hadc->Instance == ADC5){
		convertAdc5(&adc5data[0]);
	}
}


PhaseMode_t modConverterGetMode(void){
	return phase.mode;
}

PhaseFault_t modConverterGetFault(void){
	return phase.fault;
}

void control_set_setpoint(float v){
	if (!isnan(v)){
#if defined HW_TOPOLOGY_BOOST
		if(v > settings.LowSideVoltageLimitSoft)v = settings.LowSideVoltageLimitSoft;

#elif defined HW_TOPOLOGY_BUCK
		if(v > settings.HighSideVoltageLimitSoft)v = settings.HighSideVoltageLimitSoft;
#endif
		if(v < HW_MIN_SETPOINT)v = HW_MIN_SETPOINT;
		phase.Vsp = v;
	}
}

float control_get_setpoint(){
	return phase.Vsp;
}

float control_get_regulated_voltage(){
#if defined(HW_TOPOLOGY_BOOST)
	return meter.Vlow;
#elif defined(HW_TOPOLOGY_BUCK)
	return meter.Vhigh;
#endif
}

float control_get_regulated_current(){
#if defined(HW_TOPOLOGY_BOOST)
	return meter.Ilow;
#elif defined(HW_TOPOLOGY_BUCK)
	return -meter.Ihigh;
#endif
}
/*
void modConverterSetInputVoltage_dep(float v){

}

void modConverterSetOutputCurrent_dep(float c){
	if(c > settings.HighSideCurrentLimitSoft)c = settings.HighSideCurrentLimitSoft;
	if(c < 1000.0f)c = 1000.0f;
	phase.Ihighlim;
}

void modConverterSetInputCurrent_dep(float c){
	phase.Iindlim = c;
}
*/


void control_set_vhs_limit(float v){
	//TODO make this dependent work for a buck converter
	if(v > settings.HighSideVoltageLimitSoft)v = settings.HighSideVoltageLimitSoft;
	if(v < 5000.0f)v = 5000.0f;
	phase.Vhighlim = v;
}

void scope_trigger_fault(){
	if(scope.faulttrigger == true){
		if(scope.running && (scope.trigered == false)){
			if(scope.writeindex > scope.pretrigger){
				scope.trigered = true;
				scope.triggerindex = scope.writeindex;
			}
		}
	}
}

void scope_trigger(){
	if(scope.faulttrigger == false){
		if(scope.running && (scope.trigered == false)){
			if(scope.writeindex > scope.pretrigger){
				scope.trigered = true;
				scope.triggerindex = scope.writeindex;
			}
		}
	}
}

void scope_start(bool OnFault){
	scope.faulttrigger = OnFault;
	scope.running = false;
	int div = 1;
	if (scope.divider > 1){
		div = scope.divider;
	}
	scope.samplerate = 1/(Ts*div);
	scope.writeindex = 0;
	scope.trigered = false;
	scope.triggerindex = 0;

	scope.running = true;


}

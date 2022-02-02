#include "modConverter.h"
#include "Defs.h"
#include "stdbool.h"
#include "main.h"
#include "math.h"
#include "hrtim.h"
#include "modTesting.h"

#define METER_EMA 0.95f
#define EMA(_A_, _B_, _F_)	_A_ = (_F_ * _A_) + ((1 - _F_) * _B_)
#define pi 3.14159265359f

ConverterPhase_t phase;
ConverterMueasurements_t meter;
CalibrationData_t cal;
ConverterSettings_t settings;

extern HRTIM_HandleTypeDef hhrtim1;

//Internal claculated floats
float Ts = 0; //Sample period
float ControllerR;
float timebase;

extern uint32_t adc5data[6];

PhaseMode_t modConverterGetMode(void){
	return phase.mode;
}

PhaseFault_t modConverterGetFault(void){
	return phase.fault;
}


void modConverterSetInputCurrent(float c){
	if(c > settings.InputCurrentLimitSoft)c = settings.InputCurrentLimitSoft;
	if(c < 1000.0f)c = 1000.0f;
	phase.Iinlim;
}
void modConverterSetInputVoltage(float v){
	if (!isnan(v)){
		if(v > settings.InputVoltageLimitSoft)v = settings.InputVoltageLimitSoft;
		if(v < 2000.0f)v = 2000.0f;
		phase.Vsp = v;
	}
}
void modConverterSetOutputCurrent(float c){
	if(c > settings.OutputCurrentLimitSoft)c = settings.OutputCurrentLimitSoft;
	if(c < 1000.0f)c = 1000.0f;
	phase.Ioutlim;
}
void modConverterSetOutputVoltage(float v){
	if(v > settings.OutputVoltageLimitSoft)v = settings.OutputVoltageLimitSoft;
	if(v < 5000.0f)v = 5000.0f;
	phase.Voutlim = v;
}
float modConverterGetInputCurrent(){
	return phase.Iinlim;
}
float modConverterGetInputVoltage(){
	return phase.Vsp;
}
float modConverterGetOutputCurrent(){
	return phase.Ioutlim;
}
float modConverterGetOutputVoltage(){
	return phase.Voutlim;
}


bool modConverterCheckParametersValid(const ConverterSettings_t* s, const CalibrationData_t * c){
	bool error = false;
	float fres = sqrtf(1/(2*pi*pi*s->L*s->Cin))*1.0e-3f;
	if(s->ControllerFrequency > 45.0f)error = true;
	if(s->ControllerFrequency < (8*fres))error = true;
	if(s->SwitchingFrequency  < (5*fres))error = true;
	if(c->calibrated == false) error = true;
	if(s->Q > 2.0f)error = true;

	return error;
}

void modConverterInit(const ConverterSettings_t* s, const CalibrationData_t * c) {
	//Check if settings are valid.
	if(modConverterCheckParametersValid(s,c)){
		phase.fault = Converter_ConfigError;
	}

	//Even though there is a config fault, push on anyway. Do not anable output.
	settings=*s;
	cal=*c;

	uint32_t TIMER_MUL = 0;
	float mul;

	for (int i = 0; i < 8; i++){
		mul = roundf(powf(2.0f,5.0f-(float)i));
		timebase = 1000.0f / (170.0e6f * mul);
		float fmin = 1 / (0xFFFF*timebase);
		if (fmin < settings.SwitchingFrequency){
			TIMER_MUL = (uint32_t)i;
			break;
		}
	}
	timebase = 1000.0f / (170.0e6f * mul);
	uint32_t period = (uint32_t)roundf(1.0f / (settings.SwitchingFrequency * timebase));

	uint32_t dif = (uint32_t)ceilf(settings.SwitchingFrequency / settings.ControllerFrequency)-1;
	if (dif > 0xF){
		dif = 0xF;
		//TODO ERROR! Controller time might not be valid now.
	}

	//Calculate Controller sample period.
	Ts = 1e-3f * ((float)dif+1.0f) /settings.SwitchingFrequency;
	ControllerR = sqrtf(settings.L/settings.Cin)/ settings.Q-settings.RLint;

	//Slomo mode if in simulaiton
#ifdef SIMULATIONMODE
	//Ts *= 1e-2;
#endif

	//Make sure this is not called in main, as TIMER_MUL still needs to be calculated.
	MX_HRTIM1_Init(TIMER_MUL);

	// Init HighSideEnable pins
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HRTIM1->sMasterRegs.MPER = period;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].PERxR = period;

	uint32_t dt = (uint32_t)round((settings.DeadTime * (0.170 * 8)));
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].DTxR |=  (dt) | (dt<<16) ;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP2xR = 64;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP3xR = 32;


	HAL_HRTIM_ADCPostScalerConfig(&hhrtim1, HRTIM_ADCTRIGGER_1, dif);
	//HAL_HRTIM_ADCPostScalerConfig(&hhrtim1, HRTIM_ADCTRIGGER_2, CONTROL_LOOP_DIV);
	//HAL_HRTIM_ADCPostScalerConfig(&hhrtim1, HRTIM_ADCTRIGGER_6, CONTROL_LOOP_DIV);

	phase.HSEpin  = PHSEN_Pin;
	phase.HSEport = PHSEN_GPIO_Port;
	phase.PENpin  = PEN_Pin;
	phase.PENport = PEN_GPIO_Port;
	phase.timerindex = HRTIM_TIMERINDEX_TIMER_B;

	phase.LimLEDpin = LED1_Pin;
	phase.LimLEDport= LED1_GPIO_Port;


	phase.Vsp = 10000;
	phase.Voutlim = settings.OutputVoltageLimitSoft;
	phase.Iinlim = settings.InputCurrentLimitSoft;
	phase.Ioutlim = settings.OutputCurrentLimitSoft;
	modConverterPWMOutputDisable();

	HAL_HRTIM_WaveformCounterStart_IT(&hhrtim1, HRTIM_TIMERID_MASTER);
	HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_TIMERID_TIMER_B);


	if(s->outputEnable){
#ifndef SIMULATIONMODE
		HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TB1|HRTIM_OUTPUT_TB2);
#endif
	}



}

void modConverterController(ConverterPhase_t* p){

	p->mode = PhaseMode_CIV;

	//Calculate converter input and output currents
	p->Power = (p->Iout*p->Vout)*1.0e-6f;

	//Ipv observer
	float dvdt = (p->Vin - p->Vinm1) / (float)Ts;
	p->Vinm1 = p->Vin;
	float Ipvest = settings.Cin*dvdt + p->Iin;
	EMA(p->Ipv, Ipvest, CURRENT_PV_FORGETING_FACTOR);

	if(p->Power){
		p->eff = (p->Power)/(p->Ipv*p->Vin*1.0e-6f);
	}else{
		p->eff = 0;
	}

	float Vnn = p->Vsp - (p->Ipv *(ControllerR+settings.RLint));

	float Ilim = p->Iinlim;

	//Output voltage limit
	float Ioutlim = 0.8f * settings.Klim * settings.Cout*(p->Voutlim - p->Vout)/(Ts) + p->Iout;

	bool ioutlimited = false;
	if(Ioutlim > p->Ioutlim){
		Ioutlim =  p->Ioutlim;
		ioutlimited=true;
	}

	float Ilimmaxvout = Ioutlim/(1.0f-p->dutycycle);

	//Limit the input current to hifgher than 0, to prefent current undershoot.

	if(Ilimmaxvout < 0.0){
		Ilimmaxvout = 0.0f;
	}

	if(Ilim > Ilimmaxvout){
		Ilim = Ilimmaxvout;
		if(ioutlimited){
			p->mode = PhaseMode_COC;
		}else{
			p->mode = PhaseMode_COV;
		}
	}

	p->Ilimvout = Ilim / 1000.0f;


	//Current limit
	//float Vnlimup = -p->Vin - (settings.RLint * p->Iin) + ((settings.Klim * settings.L /Ts)*(settings.PhaseCurrentMin - p->Iin) );
	//float Vnlimlo = -p->Vin - (settings.RLint * p->Iin) + ((settings.Klim * settings.L /Ts)*(Ilim - p->Iin) );
	float Vnlimup = +p->Vin + (settings.RLint * p->Iin) - (settings.Klim*settings.L*(settings.PhaseCurrentMin - p->Iin)/Ts);
	float Vnlimlo = +p->Vin + (settings.RLint * p->Iin) - (settings.Klim*settings.L*(Ilim - p->Iin)/Ts );


	float Vn = Vnn + (p->Iin*ControllerR);

	//Limit Vnn to limit phase current
	if(Vn > Vnlimup){
		Vn = Vnlimup;
		p->mode = PhaseMode_MinInputCurrent;
	}

	if(Vn < Vnlimlo){
		Vn = Vnlimlo;
		if(p->mode == PhaseMode_CIV)
			p->mode = PhaseMode_CIC;
	}else{
		if(p->mode != PhaseMode_MinInputCurrent)
			p->mode = PhaseMode_CIV;
	}

	float Dn = Vn / p->Vout;

	if (Dn > 1.0f){
		Dn = 1.0f;
	}
	if (Dn < 0.01f){
		Dn = 0.01f;
	}

	p->dutycycle = 1.0f-Dn;


	//If the output is smaller then the input voltage, make sure the duty cycle does not drop to zero.
	if((p->Vin*1.10f) > p->Vout){
		//if(p->dutycycle < 0.15f)p->dutycycle = 0.15f; TODO TODO
	}

	//If the upper current limit is lower then the lower current limit, set the duty cyle to 0
	//This ensure full shutdown when the power level gets towords zero.
	bool disable_voutlim = false;
	if(Ilim < settings.PhaseCurrentMin){
		p->HSEport->BRR = p->HSEpin;
		p->PENport->BRR = p->PENpin;
		disable_voutlim = true;
		//modConverterPWMOutputDisable();
	}else{
		if(p->enabled){
			p->PENport->BSRR = p->PENpin;
		}

		//modConverterPWMOutputEnable();
	}

	if(p->fault != Converter_OK){
		modConverterPWMOutputDisable();
	}else{
		//Set PWM
#ifndef SIMULATIONMODE
#ifdef HSEN
		if(p->dutycycle < 0.01f){
			//Do not, ever put on the high side FET continuesly.
			//e.g. when the duty cycle is zero.
			p->HSEport->BRR = p->HSEpin;
		}
		else if(p->Iin > settings.PhaseHighSideEnableCurrent){
			if(disable_voutlim == false){
				p->HSEport->BSRR = p->HSEpin;
			}
		}else{
			p->HSEport->BRR = p->HSEpin;
		}

#else
		p->HSEport->BRR = p->HSEpin;
#endif
#endif

		uint32_t cmpval1;
		uint32_t per = HRTIM1->sTimerxRegs[1].PERxR;
		cmpval1 = (uint32_t)roundf((float)per * p->dutycycle);
		uint32_t adccmp = ((cmpval1 / 2) + (int32_t)roundf((((cal.ADC_delay) / 1000.0f) / timebase))) % per;

		//Make sure the AC valeus operate within their limits.
		if (cmpval1 < 64)
			cmpval1 = 64;
		if (cmpval1 > (per-64))
			cmpval1 = (per-64);

		if (adccmp < 64)
			adccmp = 64;
		if (adccmp > (per-64))
			adccmp = (per-64);


		HRTIM1->sTimerxRegs[p->timerindex].CMP2xR = cmpval1;
		HRTIM1->sTimerxRegs[p->timerindex].CMP3xR = adccmp;
	}

	//Do Lower priorety duties now.
	EMA(meter.Iin, p->Iin*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Iout, p->Iout*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Ipv, p->Ipv*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Vin, p->Vin*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Vout, p->Vout*0.001f,settings.meterfilterCoeficient);
	EMA(meter.Eff, p->eff,settings.meterfilterCoeficient);
	EMA(meter.TemperatureAmbient, p->TemperatureAmbient,settings.meterfilterCoeficient);
	EMA(meter.TemperatureHeatsink, p->TemperatureHeatsink,settings.meterfilterCoeficient);
	meter.Power = meter.Vout*meter.Iout;


#ifdef SIMULATIONMODE
	modTestingSimstep(&simstate, Ts,&phase);
#endif
}

void modConverterDisable(void){
	phase.HSEport->BRR = phase.HSEpin;
	phase.PENport->BRR = phase.PENpin;
	phase.dutycycle = 0;
	phase.enabled = false;
	HAL_HRTIM_WaveformCounterStop_IT(&hhrtim1, HRTIM_TIMERID_MASTER);
	HAL_HRTIM_WaveformCountStop(&hhrtim1, HRTIM_TIMERID_TIMER_B);
	HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TB1|HRTIM_OUTPUT_TB2);
}

inline void modConverterPWMOutputDisable(){
#ifndef SIMULATIONMODE
	phase.PENport->BRR = phase.PENpin;
	phase.HSEport->BRR = phase.HSEpin;
	DREN_GPIO_Port->BRR = DREN_Pin;
#endif
	phase.enabled = false;
}

inline void modConverterPWMOutputEnable() {
	if(settings.outputEnable){
#ifndef SIMULATIONMODE
		phase.PENport->BSRR = phase.PENpin;
		DREN_GPIO_Port->BSRR = DREN_Pin;
#endif
		phase.enabled = true;

	}else{
		modConverterPWMOutputDisable();
	}
}

float convertTemperature(uint32_t data){
	float vt = ((float)data) * ADC_VREF * 0.001f / ((float)(1<<16));
	float i = ((3.24f-vt) / cal.Temperature_R);
	float r = vt / i;


	float inverseKelvin = (1.0f / (cal.Temperature_Ref+273.16f)) + (logf(r / cal.Temperature_R))/cal.Temperature_B;

	return (1.0f / inverseKelvin) - 273.16f;
	return vt;
}


float noise(float amp){
	return ((float)(rand()/(float)__RAND_MAX)-0.5f)*amp;
	//return 0.0f;
}

void ConvertInputVoltage(ConverterPhase_t* phase, uint32_t raw){
	float V = 0;

	#ifdef SIMULATIONMODE
	V = 1.0e3f*(simstate.Vin + noise(0.05f));
	#else
	V = ((((float)raw) * cal.InputVoltageGain * ADC_VREF) / (float)0x1000) + cal.InputVoltageOffset;
	#endif

	if(V > settings.InputVoltageLimitHard){
		phase->fault = Converter_InputOverVolt;
	}
	phase->Vinm1 = phase->Vin;
	EMA(phase->Vin, V , VOLTAGE_IN_FORGETING_FACTOR);

	//modConverterIpvObserver(phase);
}

void ConvertOutputVoltage(ConverterPhase_t* phase, uint32_t raw){
	float V = 0.0;
	#ifdef SIMULATIONMODE
	V = 1.0e3f*(simstate.Vout + noise(0.05f));
	#else
	V = ((((float)raw) * cal.OutputVoltageGain * ADC_VREF) / (float)0x1000) + cal.OutputVoltageOffset;
	#endif

	if(V > settings.OutputVoltageLimitHard){
		phase->fault = Converter_OutputOverVolt;
	}
	EMA(phase->Vout, V , VOLTAGE_OUT_FORGETING_FACTOR);

}

void ConvertInputCurrent(ConverterPhase_t* phase, uint32_t raw){
	float I = 0.0;
	#ifdef SIMULATIONMODE
	I = 1.0e3f*(simstate.Il + noise(0.05f));
	#else
	I = ((((float)raw-(float)0x800) * cal.InputCurrentGain *2.0f* ADC_VREF) / (float)0x1000) + cal.InputCurrentOffset;
	#endif

	if(I > settings.InputCurrentLimitHard){
		phase->fault = Converter_InputOverCurrent;
	}else if(I < -settings.InputCurrentLimitHard){
		phase->fault = Converter_InputUnderCurrent;
	}


	EMA(phase->Iin,I, CURRENT_IN_FORGETING_FACTOR);
}

void ConvertOutputCurrent(ConverterPhase_t* phase, uint32_t raw){
	float I = 0.0f;
	#ifdef SIMULATIONMODE
	I = 1.0e3f*(simstate.Iout + noise(0.05f));
	#else
	I = ((((float)raw-(float)0x800) * cal.OutputCurrentGain *2.0f* ADC_VREF) / 0x1000) + cal.OutputCurrentOffset;
	#endif
	if(I > settings.InputCurrentLimitHard){
		phase->fault = Converter_OutputOverCurrent;
	}
	EMA(phase->Iout,I, CURRENT_IN_FORGETING_FACTOR);
}

void convertAdc5(uint32_t* data){
	phase.TemperatureMCU = __LL_ADC_CALC_TEMPERATURE((uint32_t)ADC_VREF,data[0],LL_ADC_RESOLUTION_12B);
	phase.TemperatureHeatsink = convertTemperature(data[1]);
	phase.TemperatureAmbient = convertTemperature(data[2]);
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



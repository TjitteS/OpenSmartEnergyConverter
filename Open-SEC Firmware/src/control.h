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

#ifndef INC_MODCOVERTER_H_
#define INC_MODCOVERTER_H_

#include "hardware.h"
#include "pwm.h"
#include "analog.h"
#include "delay.h"
#include "stdbool.h"
#include "main.h"
#include "math.h"

#define CONVERTER_SCOPE_CHANNELS	2
#define CONVERTER_SCOPE_SAMPLESIZE	4096

typedef enum PhaseMode_t{
	PhaseMode_CIV,
	PhaseMode_CIC,
	PhaseMode_MinInputCurrent,
	PhaseMode_COV,
	PhaseMode_COC,
	PhaseMode_TD, //Temperature De-rating
	PhaseMode_Fault,
}PhaseMode_t;

typedef enum PhaseFault_t{
	Converter_OK,
	Converter_ConfigError,
	Converter_InputOverVolt,
	Converter_OutputOverVolt,
	Converter_OutputOverCurrent,
	Converter_InputOverCurrent,
	Converter_InputUnderCurrent,
	Converter_PhaseOverCurrent,
	Converter_Fault,
}PhaseFault_t;

typedef struct ConverterPhase_t{

	//Measurements
	float Iind;
	float Vhigh;
	float Ihigh;
	float Vlow;
	float Ilow;
	float TemperatureAmbient;
	float TemperatureHeatsink;
	float TemperatureMCU;

	//Inferred measurements
	float Power;
	float eff;

	//Controller state information
	PhaseMode_t mode;
	PhaseFault_t fault;

	//states
	float Vlowm1;

	//Inputs
	float Vsp;

	float Vhighlim;
	float Iindlim;
	float Ihighlim;

	bool enabled;

	//Output
	float dutycycle;

	//debug
	float Ilimvout;

	//IO
	GPIO_TypeDef* HSEport;
	uint32_t HSEpin;
	GPIO_TypeDef* PENport;
	uint32_t PENpin;
	GPIO_TypeDef* TrackUPLEDport;
	uint32_t TrackUPLEDpin;
	GPIO_TypeDef* TrackDOWNLEDport;
	uint32_t TrackDOWNLEDpin;
	GPIO_TypeDef* LimLEDport;
	uint32_t LimLEDpin;

	int timerindex;
}ConverterPhase_t;

typedef struct IpvObserverState_{
	float Vm1;
	float Ipv;
}IpvObserverState_t;

typedef struct CalibrationData_t{
	//Information about the hardware
	char HardwareName[32];
	char HardwhareVersionString[8];
	char SerialString[8];
	bool calibrated;

	float InputCurrentGain;// A/V
	float InputCurrentOffset;//mA
	float InputVoltageGain;// V/V
	float InputVoltageOffset;//mV
	float OutputCurrentGain;//  A/V
	float OutputCurrentOffset;//mA
	float OutputVoltageGain;// V/V
	float OutputVoltageOffset;//mV
	float Temperature_B;//4000.0f
	float Temperature_R;//100000.0f
	float Temperature_Ref;//25.0f

	//Moved hardware related variables that do not change form unit to unit to harwhare.h
	//float ADC_delay;//0.1us
	//float SwitchingFrequency;
	//float ControllerFrequency;
	//float DeadTimeRising;
	//float DeadtimeFalling;
	//float L;
	//float Clow;
	//float Chigh;
	//float RLint;
	//float Q; //1
	//float Klim;
	uint32_t reserved[32];
	uint16_t calcrc; //Checksum

}CalibrationData_t;

typedef struct ConverterSettings_t {
	float meterfilterCoeficient;
	float HighSideVoltageLimitSoft;		// mV
	float LowSideVoltageLimitSoft;		// mV
	float HighSideCurrentLimitSoft;		// mA
	float LowSideCurrentMaxLimitSoft;  	//Minimum input current
	float LowSideCurrentMinLimitSoft;	//Minimum input current
	float PhaseHighSideEnableCurrent;	// mA

	float TemperatureLimitStart;
	float TemperatureLimitEnd;

	bool outputEnable;

	bool outputEnalbeOnStartup;
	uint32_t startupDelay;

}ConverterSettings_t;


typedef struct{
	//Measurements
	float Iind;
	float Ihigh;
	float Vlow;
	float Vhigh;
	float TemperatureHeatsink;
	float TemperatureAmbient;

	//Invered measurements
	float Ilow;
	float Power;
	float Eff;
}ConverterMueasurements_t;


typedef enum{
	SourceIndex_Iind,
	SourceIndex_Ihigh,
	SourceIndex_Vlow,
	SourceIndex_Vhigh,
	SourceIndex_Ilow,
	SourceIndex_Power,
	SourceIndex_Eff,
	SourceIndex_Iind_Filtered,
	SourceIndex_Ihigh_Filtered,
	SourceIndex_Vlow_Filtered,
	SourceIndex_Vhigh_Filtered,
	SourceIndex_Ilow_Filtered,
	SourceIndex_Power_Filtered,
	SourceIndex_Eff_Filtered,

}ConverterScopeSourceIndex_t;


typedef struct{
	ConverterScopeSourceIndex_t source;
	float samples[CONVERTER_SCOPE_SAMPLESIZE];
}ConverterScopeChannel;

typedef struct{
	int samples;
	int divider;
	int dividerindex;
	int pretrigger;
	int writeindex;
	int triggerindex;
	bool running;
	bool trigered;
	ConverterScopeChannel channel[CONVERTER_SCOPE_CHANNELS];
	float samplerate;
	bool faulttrigger;
}ConverterScope_t;

extern ConverterScope_t scope;
extern ConverterMueasurements_t meter;
ConverterPhase_t phase;

bool control_check_parameters(ConverterSettings_t* s, CalibrationData_t * c);
void control_init(const ConverterSettings_t* s, const CalibrationData_t * c);
void control_disable(void);
void control_controlloop(ConverterPhase_t* phase);

//void modConverterIpvObserverParallel(ConverterPhase_t* p1,ConverterPhase_t* p2);
//void modConverterIpvObserver(ConverterPhase_t* p);

void control_convert_vls(uint32_t raw);
void control_convert_vhs(uint32_t raw);
void control_convert_iind(uint32_t raw);
void control_convert_ihs(uint32_t raw);

//PhaseFault_t modConverterGetFault(void);
//PhaseMode_t modConverterGetMode(void);

//void modConverterSetInputCurrent(float c);
//void modConverterSetInputVoltage(float v);
//void modConverterSetOutputCurrent(float c);
void control_set_vhs_limit(float v);
//float modConverterGetInputCurrent();
//float modConverterGetInputVoltage();
//float modConverterGetOutputCurrent();
//float modConverterGetOutputVoltage();

void modConverterPWMOutputDisable(void);
void modConverterPWMOutputEnable(void);

PhaseMode_t modConverterGetMode(void);

void control_set_setpoint(float v);
float control_get_setpoint(void);

float control_get_regulated_voltage();
float control_get_regulated_current();

void scope_trigger_fault();
void scope_trigger();
void scope_start(bool OnFault);
#endif

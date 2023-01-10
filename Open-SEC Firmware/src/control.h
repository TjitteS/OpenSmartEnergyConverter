
#ifndef INC_MODCOVERTER_H_
#define INC_MODCOVERTER_H_

#include "hardware.h"
#include "pwm.h"
#include "analog.h"
#include "delay.h"
#include "stdbool.h"
#include "main.h"
#include "math.h"

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
	float ADC_delay;//0.1us
	float SwitchingFrequency;
	float ControllerFrequency;
	float DeadTimeRising;
	float DeadtimeFalling;
	float L;
	float Clow;
	float Chigh;
	float RLint;
	float Q; //1
	float Klim;
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


void control_set_setpoint(float v);
float control_get_setpoint(void);

float control_get_regulated_voltage();
float control_get_regulated_current();

#endif

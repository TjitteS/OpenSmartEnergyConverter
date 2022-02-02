
#ifndef INC_MODCOVERTER_H_
#define INC_MODCOVERTER_H_

#include "modDelay.h"
#include "stdbool.h"
#include "Defs.h"

typedef enum PhaseMode_t{
	PhaseMode_CIV,
	PhaseMode_CIC,
	PhaseMode_MinInputCurrent,
	PhaseMode_COV,
	PhaseMode_COC,
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
	float Iin;
	float Iout;
	float Vin;
	float Vout;
	float TemperatureAmbient;
	float TemperatureHeatsink;
	float TemperatureMCU;

	//Invered measurements
	float Ipv;
	float Power;
	float eff;

	//Controller state information
	PhaseMode_t mode;
	PhaseFault_t fault;


	//states
	float Vinm1;

	//Inputs
	float Vsp;

	float Voutlim;
	float Iinlim;
	float Ioutlim;

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
	bool calibrated;

}CalibrationData_t;
typedef struct ConverterSettings_t {
	float SwitchingFrequency;
	float ControllerFrequency;
	float DeadTime;
	float L;
	float Cin;
	float Cout;
	float RLint;

	//Controller
	float Q; //0.5
	float Klim;

	float meterfilterCoeficient;

	//Limits
	float OutputVoltageLimitSoft;		//147000.0f	// mV
	float OutputVoltageLimitHard;		//150000.0f	// mV
	float InputVoltageLimitSoft;		//90000.0f	// mV
	float InputVoltageLimitHard;		//100000.0f	// mV
	float OutputCurrentLimitSoft;		//10000.0f	// mA
	float OutputCurrentLimitHard;		//20000.0f	// mA
	float InputCurrentLimitSoft;		//8000.0f	// mA
	float InputCurrentLimitHard;		//9000.0f	// mA
	float PhaseHighSideEnableCurrent;	//300.0f	// mA
	float PhaseCurrentMin;				//500.0f	// mA

	bool outputEnable;
}ConverterSettings_t;


typedef struct{
	//Measurements
	float Iin;
	float Iout;
	float Vin;
	float Vout;
	float TemperatureHeatsink;
	float TemperatureAmbient;

	//Invered measurements
	float Ipv;
	float Power;
	float Eff;
}ConverterMueasurements_t;

extern ConverterMueasurements_t meter;

bool modConverterCheckParametersValid(const ConverterSettings_t* s, const CalibrationData_t * c);

void modConverterInit(const ConverterSettings_t* s, const CalibrationData_t * c);

//Disable converter safly.
void modConverterDisable(void);

void modConverterPWMOutputDisable(void);
void modConverterPWMOutputEnable(void);
void modConverterController(ConverterPhase_t* phase);
void modConverterIpvObserverParallel(ConverterPhase_t* p1,ConverterPhase_t* p2);
void modConverterIpvObserver(ConverterPhase_t* p);

void ConvertInputVoltage(ConverterPhase_t* phase, uint32_t raw);
void ConvertOutputVoltage(ConverterPhase_t* phase, uint32_t raw);
void ConvertInputCurrent(ConverterPhase_t* phase, uint32_t raw);
void ConvertOutputCurrent(ConverterPhase_t* phase, uint32_t raw);
PhaseFault_t modConverterGetFault(void);
PhaseMode_t modConverterGetMode(void);

void modConverterSetInputCurrent(float c);
void modConverterSetInputVoltage(float v);
void modConverterSetOutputCurrent(float c);
void modConverterSetOutputVoltage(float v);
float modConverterGetInputCurrent();
float modConverterGetInputVoltage();
float modConverterGetOutputCurrent();
float modConverterGetOutputVoltage();
#endif

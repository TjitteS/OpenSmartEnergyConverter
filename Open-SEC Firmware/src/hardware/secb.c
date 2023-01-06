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
 
#include "secb.h"
#include "stm32g4xx_hal.h"
#include "stdbool.h"
#include "config.h"

OPAMP_HandleTypeDef hopamp2;
OPAMP_HandleTypeDef hopamp3;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern ADC_HandleTypeDef hadc3;
extern ADC_HandleTypeDef hadc4;
extern ADC_HandleTypeDef hadc5;

#ifdef HW_SECB175

modConfig_t defaultConvig = {
		{//CalibrationData_t	calData;
				"SEC-B175-7A",
				"V221201",
				"00000000",
				false,
				1.0,//float InputCurrentGain;// A/V
				0,	//float InputCurrentOffset;//mA
				1,	//float InputVoltageGain;// V/V
				0,	//float InputVoltageOffset;//mV
				1,	//float OutputCurrentGain;//  A/V
				0,	//float OutputCurrentOffset;//mA
				1,	//float OutputVoltageGain;// V/V
				0,	//float OutputVoltageOffset;//mV
				4000.0f,	//float Temperature_B;//4000.0f
				100.0f,	//float Temperature_R;//100000.0f
				25.0f,	//float Temperature_Ref;//25.0f
				1.2f,	//float ADC_delay;//0.1us
				100,	//float SwitchingFrequency;
				35,	//float ControllerFrequency;
				30,	//float DeadTimeRising;
				30,	//float DedtimeFalling;
				50,	//float L;
				220,	//float Clow;
				440,	//float Chigh;
				30,	//float RLint;
				1,	//float Q; //1
				0.4,	//float Klim;
				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},	//uint32_t reserved[32];
				0x5c3d,	//uint16_t calcrc; //Checksum
		},{//ConverterSettings_t settings;
				0.999f, //float meterfilterCoeficient;
				175.0e3f, //float HighSideVoltageLimitSoft;		// mV
				175.0e3f,//float LowSideVoltageLimitSoft;		// mV
				7.0e3f, //float HighSideCurrentLimitSoft;		// mA
				7.0e3f, //float LowSideCurrentMaxLimitSoft;  	//Minimum input current
				10.0f, //float LowSideCurrentMinLimitSoft;	//Minimum input current
				10.0f,//float PhaseHighSideEnableCurrent;	// mA
				false,//bool outputEnable;
		},{//modMPPTsettings_t mpptsettings;
				100.0f, //float PO_Stepsize; mV
				50, //uint32_t PO_Timestep; ms
				0, //float PO_maxgradient;
				0, //float jump_RangeMax;
				0, //float jump_RangeMin;
				0, //float jump_PowerThreshold;
				0, //int jump_Rate;
		},{//modCanSettings_t cansettings;
				250,//int baudrate; kbps
				0.75,//float samplepoint;
				56,//uint16_t generalCanId;
		},
};


void hw_io_init(){

	//PWM
	hw_setio_af(GPIOA, P2_PWM_LS_Pin, GPIO_AF13_HRTIM1);
	hw_setio_af(GPIOA, P2_EN_HS_Pin,  GPIO_AF13_HRTIM1);

	//UART
	hw_setio_af(GPIOC, GPIO_PIN_10, GPIO_AF7_USART3);
	hw_setio_af(GPIOC, GPIO_PIN_11, GPIO_AF7_USART3);

	HAL_GPIO_WritePin(GPIOC, LED2_Pin|LED1_Pin|LED0_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, PHSEN_Pin|PEN_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, DREN_Pin|LED3_Pin, GPIO_PIN_RESET);

	hw_setio_output(LED0_GPIO_Port, LED0_Pin);
	hw_setio_output(LED1_GPIO_Port, LED1_Pin);
	hw_setio_output(LED2_GPIO_Port, LED2_Pin);
	hw_setio_output(LED3_GPIO_Port, LED3_Pin);
	hw_setio_output(PHSEN_GPIO_Port, PHSEN_Pin);
	hw_setio_output(PEN_GPIO_Port,PEN_Pin);
	hw_setio_output(DREN_GPIO_Port, DREN_Pin);

	hw_setio_input(ID0_GPIO_Port, ID0_Pin, GPIO_PULLUP);
	hw_setio_input(ID1_GPIO_Port, ID1_Pin, GPIO_PULLUP);
	hw_setio_input(ID2_GPIO_Port, ID2_Pin, GPIO_PULLUP);
	hw_setio_input(ID3_GPIO_Port, ID3_Pin, GPIO_PULLUP);

	hw_setio_input(VDRVPG_GPIO_Port, VDRVPG_Pin, GPIO_PULLUP);
	hw_setio_input(BOOT0_GPIO_Port, BOOT0_Pin, GPIO_PULLUP);
	hw_setio_input(RST_GPIO_Port, RST_Pin, GPIO_NOPULL);

	hw_setio_analog(IHS__GPIO_Port, IHS__Pin);
	hw_setio_analog(IHS_A3_GPIO_Port, IHS_A3_Pin);

	hw_setio_analog(ILS__GPIO_Port, ILS__Pin);
	hw_setio_analog(ILS_B15_GPIO_Port, ILS_B15_Pin);

	hw_setio_analog(Tamb_GPIO_Port, Tamb_Pin);
	hw_setio_analog(Tsink_GPIO_Port, Tsink_Pin);

	hw_setio_analog(VHS_GPIO_Port, VHS_Pin);
	hw_setio_analog(VLS_GPIO_Port, VLS_Pin);
}

void hw_adc_init(){
	//Init opamps
	hopamp3.Instance = OPAMP3;
	hopamp3.Init.PowerMode = OPAMP_POWERMODE_NORMALSPEED;
	hopamp3.Init.Mode = OPAMP_FOLLOWER_MODE;
	hopamp3.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO2;
	hopamp3.Init.InternalOutput = ENABLE;
	hopamp3.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
	hopamp3.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
	HAL_OPAMP_Init(&hopamp3);

	hopamp2.Instance = OPAMP2;
	hopamp2.Init.PowerMode = OPAMP_POWERMODE_NORMALSPEED;
	hopamp2.Init.Mode = OPAMP_FOLLOWER_MODE;
	hopamp2.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO0;
	hopamp2.Init.InternalOutput = ENABLE;
	hopamp2.Init.TimerControlledMuxmode = OPAMP_TIMERCONTROLLEDMUXMODE_DISABLE;
	hopamp2.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
	HAL_OPAMP_Init(&hopamp2);


	HAL_OPAMP_Start(&hopamp2);
	HAL_OPAMP_Start(&hopamp3);
	HAL_OPAMPEx_SelfCalibrateAll(&hopamp2, &hopamp3, &hopamp3, &hopamp3, &hopamp3, &hopamp3);

	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Offset = 0;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
	//sConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;

	//sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Channel = ADC_CHANNEL_4;

	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	//sConfig.Channel = ADC_CHANNEL_4;
	sConfig.Channel = ADC_CHANNEL_5;

	HAL_ADC_ConfigChannel(&hadc4, &sConfig);

	sConfig.Channel = ADC_CHANNEL_VOPAMP2;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);

	sConfig.Channel = ADC_CHANNEL_VOPAMP3_ADC3;
	HAL_ADC_ConfigChannel(&hadc3, &sConfig);

	sConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC5;
	sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	HAL_ADC_ConfigChannel(&hadc5, &sConfig);

	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	HAL_ADC_ConfigChannel(&hadc5, &sConfig);

	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	HAL_ADC_ConfigChannel(&hadc5, &sConfig);
}
#endif

/*
 **	Copyright 2024 Tjitte van der Ploeg, tjitte@tpee.nl
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

#ifdef HW_SECB175_7A

#include <secb1757a.h>
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


modConfig_t defaultConvig = {
		.calData = (CalibrationData_t){//CalibrationData_t	calData;
				.HardwareName 					=  "SEC-B175-7A",
				.HardwhareVersionString 		=  "002405",
				.SerialString 					=  "000000",
				.calibrated 					=  false,
				.InputCurrentGain 				=  4.67f,	// A/V
				.InputCurrentOffset 			=  0.00f,	// mA
				.InputVoltageGain 				=  78.90f,	// V/V
				.InputVoltageOffset 			=  106.20f,	// mV
				.OutputCurrentGain 				= -4.82f,	// A/V
				.OutputCurrentOffset 			=  0.00f,	// mA
				.OutputVoltageGain 				=  78.90f,	// V/V
				.OutputVoltageOffset 			=  106.15f,	// mV
				.Temperature_B 					=  4480.00f,// 4000.0f
				.Temperature_R 					=  100.00f, // 100000.0f
				.Temperature_Ref 				=  25.00f,	// 25.0f
				.reserved 						=  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
				.calcrc 						=  0x32ed,	// Checksum
			},

		.settings = (ConverterSettings_t){//ConverterSettings_t settings;
				.meterfilterCoeficient 			=  0.93f,	// %
				.HighSideVoltageLimitSoft    	=  147.00e3f,	// mV
				.LowSideVoltageLimitSoft 		=  90.00e3f,	// mV
				.HighSideCurrentLimitSoft 		=  8.00e3f,	// mA
				.LowSideCurrentMaxLimitSoft 	=  8.00e3f, // mA
				.LowSideCurrentMinLimitSoft 	= -0.30e3f,	// mA
				.PhaseHighSideEnableCurrent 	= -0.50e3f, // mA
				.TemperatureLimitStart 			=  80.00f, 	// C
				.TemperatureLimitEnd 			=  85.00f,	// C
				.outputEnable 					=  false,	// Bool
				.outputEnalbeOnStartup			=  true,	// Bool
				.startupDelay                 	=  0,		// ms
			},
		.mpptsettings = (modMPPTsettings_t){
				.PO_Stepsize					=  100.0f,	// mV
				.PO_Timestep					=  5,		// ms
				.PO_StepSizeGain 				=  1.0f,	// float P&O Step Size Gain;
				.jump_PowerThreshold 			=  50, 		//float jump_PowerThreshold;
				.jump_Rate 						=  0, 		//int jump_Rate;

				.Sweep_eneable 					= false,
				.Sweep_interval 				= 5000,
				.Sweep_timestep 				= 1,
				.Sweep_direction 				= false,
				.Sweep_datapoints 				= 16,
				.Sweep_publishOnCan 			= false,
			},
		.cansettings = (modCanSettings_t){
				.baudrate 						=  250,  	// kbps
				.samplepoint					=  0.75f,	// %
				.generalCanId					=  32,
			},
};


void hw_io_init(){

	//PWM
	hw_setio_af(GPIOA, P2_PWM_LS_Pin, GPIO_AF13_HRTIM1);
	hw_setio_af(GPIOA, P2_EN_HS_Pin,  GPIO_AF13_HRTIM1);

	//CAN
	hw_setio_af(GPIOB,  GPIO_PIN_5,  GPIO_AF9_FDCAN2);
	hw_setio_af(GPIOB, GPIO_PIN_6, GPIO_AF9_FDCAN2);


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
	//sConfig.SamplingTime = ADC_SAMPLETIME_47CYCLES_5;
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

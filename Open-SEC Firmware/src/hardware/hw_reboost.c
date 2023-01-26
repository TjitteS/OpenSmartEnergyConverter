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
 
#include "hardware.h"
#include "hw_reboost.h"
#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "config.h"
#include "stdbool.h"

#ifdef HW_REBOOST

OPAMP_HandleTypeDef hopamp2;
OPAMP_HandleTypeDef hopamp3;

modConfig_t defaultConvig = {
		{//CalibrationData_t	calData;
				"SEC-B80-8A",
				"V0.2.1",
				"0000000",
				false,
				-4.3f,//float InputCurrentGain;// A/V
				10.0f,	//float InputCurrentOffset;//mA

				30.9f,	//float InputVoltageGain;// V/V
				100.0f,//float InputVoltageOffset;//mV

				4.3f,	//float OutputCurrentGain;//  A/V
				-10.0f,	//float OutputCurrentOffset;//mA

				30.9f,	//float OutputVoltageGain;// V/V
				100.0f,	//float OutputVoltageOffset;//mV

				4100.0f,	//float Temperature_B;//4000.0f
				100.0f,	//float Temperature_R;//100000.0f
				25.0f,	//float Temperature_Ref;//25.0f

				{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},	//uint32_t reserved[32];
				0xaf83,	//uint16_t calcrc; //Checksum
		},{//ConverterSettings_t settings;
				0.99f, //float meterfilterCoeficient;
				80.0e3f, //float HighSideVoltageLimitSoft;		// mV
				80.0e3f,//float LowSideVoltageLimitSoft;		// mV
				8.0e3f, //float HighSideCurrentLimitSoft;		// mA
				8.0e3f, //float LowSideCurrentMaxLimitSoft;  	//Minimum input current
				10.0f, //float LowSideCurrentMinLimitSoft;	//Minimum input current
				10.0f,//float PhaseHighSideEnableCurrent;	// mA
				80.0f, 		// float TemeratureLimitStart,	 	Degrees Celcius
				85.0f,		// float TemeratureLimitEnd, 		Degrees Celcius
				false,//bool outputEnable;
		},{//modMPPTsettings_t mpptsettings;
				100.0f, //float PO_Stepsize; mV
				30, //uint32_t PO_Timestep; ms
				2.0, //float P&O Step Size Gain;
				5, //float jump_PowerThreshold;
				0, //int jump_Rate;
		},{//modCanSettings_t cansettings;
				250,//int baudrate; kbps
				0.75,//float samplepoint;
				32,//uint16_t generalCanId;
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
	sConfig.SingleDiff = ADC_DIFFERENTIAL_ENDED;

	sConfig.Channel = ADC_CHANNEL_3;


	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	sConfig.Channel = ADC_CHANNEL_4;
	//sConfig.Channel = ADC_CHANNEL_5;

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


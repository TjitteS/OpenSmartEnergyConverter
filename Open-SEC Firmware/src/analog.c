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
#include "analog.h"
#include "dma.h"
#include "control.h"

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
ADC_HandleTypeDef hadc4;
ADC_HandleTypeDef hadc5;

extern ConverterPhase_t phase;
extern CalibrationData_t cal;


void analog_init() {

	//MSP init

	__HAL_RCC_ADC12_CLK_ENABLE();
	__HAL_RCC_ADC345_CLK_ENABLE();

	HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
	HAL_NVIC_SetPriority(ADC3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC3_IRQn);
	HAL_NVIC_SetPriority(ADC4_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(ADC4_IRQn);
	HAL_NVIC_SetPriority(ADC5_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(ADC5_IRQn);

	//Initialize ADC's
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.GainCompensation = 0;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_HRTIM_TRG1;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.OversamplingMode = DISABLE;

	hadc2.Instance = ADC2;
	hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;
	hadc2.Init.Resolution = ADC_RESOLUTION_12B;
	hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc2.Init.GainCompensation = 0;
	hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc2.Init.LowPowerAutoWait = DISABLE;
	hadc2.Init.ContinuousConvMode = DISABLE;
	hadc2.Init.NbrOfConversion = 1;
	hadc2.Init.DiscontinuousConvMode = DISABLE;
	hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIG_HRTIM_TRG1;
	hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc2.Init.DMAContinuousRequests = DISABLE;
	hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc2.Init.OversamplingMode = DISABLE;

	hadc3.Instance = ADC3;
	hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;
	hadc3.Init.Resolution = ADC_RESOLUTION_12B;
	hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc3.Init.GainCompensation = 0;
	hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc3.Init.LowPowerAutoWait = DISABLE;
	hadc3.Init.ContinuousConvMode = DISABLE;
	hadc3.Init.NbrOfConversion = 1;
	hadc3.Init.DiscontinuousConvMode = DISABLE;
	hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIG_HRTIM_TRG1;
	hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc3.Init.DMAContinuousRequests = DISABLE;
	hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc3.Init.OversamplingMode = DISABLE;

	hadc4.Instance = ADC4;
	hadc4.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;
	hadc4.Init.Resolution = ADC_RESOLUTION_12B;
	hadc4.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc4.Init.GainCompensation = 0;
	hadc4.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc4.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc4.Init.LowPowerAutoWait = DISABLE;
	hadc4.Init.ContinuousConvMode = DISABLE;
	hadc4.Init.NbrOfConversion = 1;
	hadc4.Init.DiscontinuousConvMode = DISABLE;
	hadc4.Init.ExternalTrigConv = ADC_EXTERNALTRIG_HRTIM_TRG1;
	hadc4.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc4.Init.DMAContinuousRequests = DISABLE;
	hadc4.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	hadc4.Init.OversamplingMode = DISABLE;

	hadc5.Instance = ADC5;
	hadc5.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV16;
	hadc5.Init.Resolution = ADC_RESOLUTION_12B;
	hadc5.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc5.Init.GainCompensation = 0;
	hadc5.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc5.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	hadc5.Init.LowPowerAutoWait = DISABLE;
	hadc5.Init.ContinuousConvMode = ENABLE;
	hadc5.Init.NbrOfConversion = 3;
	hadc5.Init.DiscontinuousConvMode = DISABLE;
	hadc5.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc5.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc5.Init.DMAContinuousRequests = ENABLE;
	hadc5.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
	hadc5.Init.OversamplingMode = ENABLE;
	hadc5.Init.Oversampling.Ratio = ADC_OVERSAMPLING_RATIO_256;
	hadc5.Init.Oversampling.RightBitShift = ADC_RIGHTBITSHIFT_4;
	hadc5.Init.Oversampling.TriggeredMode = ADC_TRIGGEREDMODE_SINGLE_TRIGGER;
	hadc5.Init.Oversampling.OversamplingStopReset =
			ADC_REGOVERSAMPLING_RESUMED_MODE;

	hdma_adc5.Instance = DMA1_Channel1;
	hdma_adc5.Init.Request = DMA_REQUEST_ADC5;
	hdma_adc5.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc5.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc5.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc5.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc5.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
	hdma_adc5.Init.Mode = DMA_CIRCULAR;
	hdma_adc5.Init.Priority = DMA_PRIORITY_MEDIUM;

	ADC_MultiModeTypeDef multimode = { 0 };
	multimode.Mode = ADC_MODE_INDEPENDENT;

	HAL_DMA_Init(&hdma_adc5);
	__HAL_LINKDMA(&hadc5, DMA_Handle, hdma_adc5);

	HAL_ADC_Init(&hadc1);
	HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);
	HAL_ADC_Init(&hadc2);
	HAL_ADC_Init(&hadc3);
	HAL_ADCEx_MultiModeConfigChannel(&hadc3, &multimode);
	HAL_ADC_Init(&hadc4);
	HAL_ADC_Init(&hadc5);

	//Initialize channels

	hw_adc_init();

	if(cal.calibrated == false){
		HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED);
		HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
		HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
		HAL_ADCEx_Calibration_Start(&hadc4, ADC_SINGLE_ENDED);
		HAL_ADCEx_Calibration_Start(&hadc5, ADC_SINGLE_ENDED);

		HAL_ADCEx_Calibration_Start(&hadc1, ADC_DIFFERENTIAL_ENDED);
		HAL_ADCEx_Calibration_Start(&hadc2, ADC_DIFFERENTIAL_ENDED);
		HAL_ADCEx_Calibration_Start(&hadc3, ADC_DIFFERENTIAL_ENDED);
		HAL_ADCEx_Calibration_Start(&hadc4, ADC_DIFFERENTIAL_ENDED);
		HAL_ADCEx_Calibration_Start(&hadc5, ADC_DIFFERENTIAL_ENDED);
	}


	HAL_ADC_Start_IT(&hadc2);
	HAL_ADC_Start_IT(&hadc1);

	HAL_ADC_Start_IT(&hadc3);
	HAL_ADC_Start_IT(&hadc4);

	HAL_ADC_Start_DMA(&hadc5, adc5data, 6);

	//HAL_ADCEx_Calibration_Start(&hadc3, ADC_SINGLE_ENDED);
	//HAL_ADCEx_Calibration_Start(&hadc4, ADC_DIFFERENTIAL_ENDED);
	//HAL_ADCEx_Calibration_Start(&hadc5, ADC_SINGLE_ENDED);
	//HAL_ADCEx_Calibration_Start(&hadc1, ADC_DIFFERENTIAL_ENDED);
	//HAL_ADCEx_Calibration_Start(&hadc2, ADC_SINGLE_ENDED);
}

void ADC1_2_IRQHandler(void) {
	//LED3_GPIO_Port->BSRR = LED3_Pin;
	//LED3_GPIO_Port->BRR = LED3_Pin;

	uint32_t tmp_isr = ADC1->ISR;
	uint32_t tmp2_isr = ADC2->ISR;

	if (tmp2_isr & ADC_ISR_EOC) {
		//ADC 2 Data ready

		uint32_t raw = ADC2->DR;
		ADC2->ISR = ADC_ISR_EOC;
		control_convert_vhs(raw);
	}
	if (tmp_isr & ADC_ISR_EOC) {
		uint32_t raw = ADC1->DR;
		ADC1->ISR = ADC_ISR_EOC;
		control_convert_ihs(raw);
	}

}

void ADC3_IRQHandler(void) {
	//LED3_GPIO_Port->BSRR = LED3_Pin;
	//LED3_GPIO_Port->BRR = LED3_Pin;

	uint32_t tmp_isr = ADC3->ISR;
	//uint32_t tmp_ier = ADC3->IER;

	if (tmp_isr & ADC_ISR_EOC) {

		uint32_t raw = ADC3->DR;
		ADC3->ISR = ADC_ISR_EOC;
		control_convert_vls(raw);
	}
}

void ADC4_IRQHandler(void) {
	//LED3_GPIO_Port->BSRR = LED3_Pin;
	//LED3_GPIO_Port->BRR = LED3_Pin;

	uint32_t tmp_isr = ADC4->ISR;
	//uint32_t tmp_ier = ADC4->IER;

	if (tmp_isr & ADC_ISR_EOC) {
		uint32_t raw = ADC4->DR;
		ADC4->ISR = ADC_ISR_EOC;

		control_convert_iind(raw);
		control_controlloop(&phase);
	}

}

void ADC5_IRQHandler(void) {
	//LED3_GPIO_Port->BSRR = LED3_Pin;
	//LED3_GPIO_Port->BRR = LED3_Pin;

	HAL_ADC_IRQHandler(&hadc5);
}


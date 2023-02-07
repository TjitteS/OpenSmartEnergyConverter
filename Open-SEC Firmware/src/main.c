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
 


#include "main.h"
#include <stdbool.h>
#include "stm32g4xx_hal.h"
#include "hardware.h"
#include "can.h"
#include "mppt.h"
#include "control.h"
#include "terminal.h"
#include "leds.h"
#include "config.h"
#include "uart.h"
#include "testing.h"
#include "dma.h"

void system_clock_config(void);

extern ConverterPhase_t phase;
modConfig_t *acktiveConfig;

//(Re)Initialize all parts that require initialization based on configuration memory.
void main_init_after_config() {
	modCANinit(&acktiveConfig->cansettings);
	control_init(&acktiveConfig->settings, &acktiveConfig->calData);
	modMPPTinit(&acktiveConfig->mpptsettings);
}

int main(void) {
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	DBGMCU->APB2FZ |= DBGMCU_APB2FZ_DBG_HRTIM1_STOP;
	HRTIM1->sCommonRegs.OENR = 0;

	HAL_Init();

	system_clock_config();
	dma_init();


	hw_io_init();

	acktiveConfig = modConfigInit();

	modUARTInit();
	modTestingInit();
	main_init_after_config();


	while (1) {
		modEffectTask();
		modMPPTtask();
		modCANtask();

#ifdef HW_HAS_UART
		modUARTTask();
#endif

		modTestingTask();
	}
}

void system_clock_config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
	RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

	// configure the main internal regulator output voltage
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
	RCC_OscInitStruct.PLL.PLLN = 85;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
		Error_Handler();
	}
	/** Initializes the peripherals clocks
	 */
	PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3
			| RCC_PERIPHCLK_ADC12 | RCC_PERIPHCLK_ADC345 | RCC_PERIPHCLK_FDCAN;
	PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
	PeriphClkInit.FdcanClockSelection = RCC_FDCANCLKSOURCE_PCLK1;
	PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
	PeriphClkInit.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
		Error_Handler();
	}
}

void HAL_MspInit(void) {
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	// System interrupt init
	// SVCall_IRQn interrupt configuration
	HAL_NVIC_SetPriority(SVCall_IRQn, 2, 0);
	// DebugMonitor_IRQn interrupt configuration
	HAL_NVIC_SetPriority(DebugMonitor_IRQn, 2, 0);
	// PendSV_IRQn interrupt configuration
	HAL_NVIC_SetPriority(PendSV_IRQn, 2, 0);

	// Configure the internal voltage reference buffer voltage scale
	HAL_SYSCFG_VREFBUF_VoltageScalingConfig(SYSCFG_VREFBUF_VOLTAGE_SCALE2);

	// Enable the Internal Voltage Reference buffer
	HAL_SYSCFG_EnableVREFBUF();

	// Configure the internal voltage reference buffer high impedance mode
	HAL_SYSCFG_VREFBUF_HighImpedanceConfig(
	SYSCFG_VREFBUF_HIGH_IMPEDANCE_DISABLE);

	// Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
	HAL_PWREx_DisableUCPDDeadBattery();
}

bool main_halt_risky() {
	control_disable();
	HAL_Delay(100);
	//OK
	return 0;
}

void Error_Handler(void) {
	while (1);
}

void NMI_Handler(void) {

}

void HardFault_Handler(void) {
	while (1) {

	}
}

void MemManage_Handler(void) {

	while (1) {

	}
}

void BusFault_Handler(void) {

	while (1) {

	}
}

void UsageFault_Handler(void) {

	while (1) {

	}
}

void SVC_Handler(void) {

}

void DebugMon_Handler(void) {

}

void PendSV_Handler(void) {

}

void SysTick_Handler(void) {

	HAL_IncTick();

}


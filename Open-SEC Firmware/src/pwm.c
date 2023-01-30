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
 
#include "pwm.h"
#include "math.h"
#include "main.h"

float ControllerPeriod; //
float timebase; // Milliseconds per count
float triggerdelay;

HRTIM_HandleTypeDef hhrtim1;

void pwm_init(float switchingFrequency, float controllerFrequency, float tdr, float tdf, float adc_td) {
	triggerdelay = adc_td;
	uint32_t TIMER_MUL = 0;
	float mul;

	for (int i = 0; i < 8; i++) {
		mul = roundf(powf(2.0f, 5.0f - (float) i));
		timebase = 1000.0f / (170.0e6f * mul);
		float fmin = 1 / (0xFFFF * timebase);
		if (fmin < switchingFrequency) {
			TIMER_MUL = (uint32_t) i;
			break;
		}
	}
	timebase = 1000.0f / (170.0e6f * mul); // Milliseconds per count
	uint32_t period = (uint32_t) roundf(1.0f / (switchingFrequency * timebase));

	uint32_t dif = (uint32_t) ceilf(switchingFrequency / controllerFrequency)
			- 1;
	if (dif > 0xF) {
		dif = 0xF;
		//Error_Handler();
	}

	//Calculate Controller sample period.
	ControllerPeriod = 1e-3f * ((float) dif + 1.0f) / switchingFrequency;

	HRTIM_ADCTriggerCfgTypeDef pADCTriggerCfg = { 0 };
	HRTIM_TimeBaseCfgTypeDef pTimeBaseCfg = { 0 };
	HRTIM_TimerCfgTypeDef pTimerCfg = { 0 };
	HRTIM_TimerCtlTypeDef pTimerCtl = { 0 };
	HRTIM_CompareCfgTypeDef pCompareCfg = { 0 };
	HRTIM_DeadTimeCfgTypeDef pDeadTimeCfg = { 0 };
	HRTIM_OutputCfgTypeDef pOutputCfg = { 0 };


	__HAL_RCC_HRTIM1_CLK_ENABLE();

    /* HRTIM1 interrupt Init */
    HAL_NVIC_SetPriority(HRTIM1_Master_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(HRTIM1_Master_IRQn);


	hhrtim1.Instance = HRTIM1;
	hhrtim1.Init.HRTIMInterruptResquests = HRTIM_IT_NONE;
	hhrtim1.Init.SyncOptions = HRTIM_SYNCOPTION_NONE;
	if (HAL_HRTIM_Init(&hhrtim1) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_HRTIM_DLLCalibrationStart(&hhrtim1, HRTIM_CALIBRATIONRATE_3)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_HRTIM_PollForDLLCalibration(&hhrtim1, 100) != HAL_OK) {
		Error_Handler();
	}

	pADCTriggerCfg.UpdateSource = HRTIM_ADCTRIGGERUPDATE_MASTER;
	pADCTriggerCfg.Trigger = HW_HRTIM_ADCTRIGGER;
	if (HAL_HRTIM_ADCTriggerConfig(&hhrtim1, HRTIM_ADCTRIGGER_1,
			&pADCTriggerCfg) != HAL_OK) {
		Error_Handler();
	}

	if (HAL_HRTIM_ADCPostScalerConfig(&hhrtim1, HRTIM_ADCTRIGGER_1, dif)
			!= HAL_OK) {
		Error_Handler();
	}
	pTimeBaseCfg.Period = period;
	pTimeBaseCfg.RepetitionCounter = 100;
	pTimeBaseCfg.PrescalerRatio = TIMER_MUL;
	pTimeBaseCfg.Mode = HRTIM_MODE_CONTINUOUS;
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HRTIM_TIMERINDEX_MASTER,
			&pTimeBaseCfg) != HAL_OK) {
		Error_Handler();
	}
	pTimerCfg.InterruptRequests = HRTIM_MASTER_IT_MREP;
	pTimerCfg.DMARequests = HRTIM_MASTER_DMA_NONE;
	pTimerCfg.DMASrcAddress = 0x0000;
	pTimerCfg.DMADstAddress = 0x0000;
	pTimerCfg.DMASize = 0x1;
	pTimerCfg.HalfModeEnable = HRTIM_HALFMODE_DISABLED;
	pTimerCfg.InterleavedMode = HRTIM_INTERLEAVED_MODE_DISABLED;
	pTimerCfg.StartOnSync = HRTIM_SYNCSTART_DISABLED;
	pTimerCfg.ResetOnSync = HRTIM_SYNCRESET_DISABLED;
	pTimerCfg.DACSynchro = HRTIM_DACSYNC_NONE;
	pTimerCfg.PreloadEnable = HRTIM_PRELOAD_ENABLED;
	pTimerCfg.UpdateGating = HRTIM_UPDATEGATING_INDEPENDENT;
	pTimerCfg.BurstMode = HRTIM_TIMERBURSTMODE_MAINTAINCLOCK;
	pTimerCfg.RepetitionUpdate = HRTIM_UPDATEONREPETITION_ENABLED;
	pTimerCfg.ReSyncUpdate = HRTIM_TIMERESYNC_UPDATE_UNCONDITIONAL;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HRTIM_TIMERINDEX_MASTER,
			&pTimerCfg) != HAL_OK) {
		Error_Handler();
	}

	pTimeBaseCfg.Period = period;
	pTimeBaseCfg.RepetitionCounter = 0x00;
	if (HAL_HRTIM_TimeBaseConfig(&hhrtim1, HW_TIMERINDEX,
			&pTimeBaseCfg) != HAL_OK) {
		Error_Handler();
	}
	pTimerCtl.UpDownMode = HRTIM_TIMERUPDOWNMODE_UP;
	pTimerCtl.TrigHalf = HRTIM_TIMERTRIGHALF_DISABLED;
	pTimerCtl.GreaterCMP3 = HRTIM_TIMERGTCMP3_EQUAL;
	pTimerCtl.DualChannelDacEnable = HRTIM_TIMER_DCDE_DISABLED;
	if (HAL_HRTIM_WaveformTimerControl(&hhrtim1, HW_TIMERINDEX,
			&pTimerCtl) != HAL_OK) {
		Error_Handler();
	}
	pTimerCfg.InterruptRequests = HRTIM_TIM_IT_NONE;
	pTimerCfg.DMARequests = HRTIM_TIM_DMA_NONE;
	pTimerCfg.PushPull = HRTIM_TIMPUSHPULLMODE_DISABLED;
	pTimerCfg.FaultEnable = HRTIM_TIMFAULTENABLE_NONE;
	pTimerCfg.FaultLock = HRTIM_TIMFAULTLOCK_READWRITE;
	pTimerCfg.DeadTimeInsertion = HRTIM_TIMDEADTIMEINSERTION_ENABLED;
	pTimerCfg.DelayedProtectionMode =
			HRTIM_TIMER_A_B_C_DELAYEDPROTECTION_DISABLED;
	pTimerCfg.UpdateTrigger = HRTIM_TIMUPDATETRIGGER_TIMER_B;
	pTimerCfg.ResetTrigger = HRTIM_TIMRESETTRIGGER_MASTER_PER;
	pTimerCfg.ResetUpdate = HRTIM_TIMUPDATEONRESET_ENABLED;
	if (HAL_HRTIM_WaveformTimerConfig(&hhrtim1, HW_TIMERINDEX,
			&pTimerCfg) != HAL_OK) {
		Error_Handler();
	}

	pCompareCfg.AutoDelayedMode = HRTIM_AUTODELAYEDMODE_REGULAR;
	pCompareCfg.AutoDelayedTimeout = 0x0000;

	pCompareCfg.CompareValue = 64;
	HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HW_TIMERINDEX, HRTIM_COMPAREUNIT_2, &pCompareCfg);

	pCompareCfg.CompareValue = 32;
	HAL_HRTIM_WaveformCompareConfig(&hhrtim1, HW_TIMERINDEX,HRTIM_COMPAREUNIT_3, &pCompareCfg);


	pDeadTimeCfg.Prescaler = HRTIM_TIMDEADTIME_PRESCALERRATIO_MUL8;
	pDeadTimeCfg.RisingValue = 10;
	pDeadTimeCfg.RisingSign = HRTIM_TIMDEADTIME_RISINGSIGN_POSITIVE;
	pDeadTimeCfg.RisingLock = HRTIM_TIMDEADTIME_RISINGLOCK_WRITE;
	pDeadTimeCfg.RisingSignLock = HRTIM_TIMDEADTIME_RISINGSIGNLOCK_WRITE;
	pDeadTimeCfg.FallingValue = 10;
	pDeadTimeCfg.FallingSign = HRTIM_TIMDEADTIME_FALLINGSIGN_POSITIVE;
	pDeadTimeCfg.FallingLock = HRTIM_TIMDEADTIME_FALLINGLOCK_WRITE;
	pDeadTimeCfg.FallingSignLock = HRTIM_TIMDEADTIME_FALLINGSIGNLOCK_WRITE;
	if (HAL_HRTIM_DeadTimeConfig(&hhrtim1, HW_TIMERINDEX,
			&pDeadTimeCfg) != HAL_OK) {
		Error_Handler();
	}

	pOutputCfg.Polarity = HRTIM_OUTPUTPOLARITY_HIGH;
	pOutputCfg.SetSource = HRTIM_OUTPUTSET_TIMPER;
	pOutputCfg.ResetSource = HRTIM_OUTPUTRESET_TIMCMP2;
	pOutputCfg.IdleMode = HRTIM_OUTPUTIDLEMODE_NONE;
	pOutputCfg.IdleLevel = HRTIM_OUTPUTIDLELEVEL_INACTIVE;
	pOutputCfg.FaultLevel = HRTIM_OUTPUTFAULTLEVEL_NONE;
	pOutputCfg.ChopperModeEnable = HRTIM_OUTPUTCHOPPERMODE_DISABLED;
	pOutputCfg.BurstModeEntryDelayed = HRTIM_OUTPUTBURSTMODEENTRY_REGULAR;
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HW_TIMERINDEX,
			HRTIM_OUTPUT_TB1, &pOutputCfg) != HAL_OK) {
		Error_Handler();
	}
	pOutputCfg.SetSource = HRTIM_OUTPUTSET_NONE;
	pOutputCfg.ResetSource = HRTIM_OUTPUTRESET_NONE;
	if (HAL_HRTIM_WaveformOutputConfig(&hhrtim1, HW_TIMERINDEX,
			HRTIM_OUTPUT_TB2, &pOutputCfg) != HAL_OK) {
		Error_Handler();
	}

	uint32_t dtri = (uint32_t) round((tdr * (0.170 * 8)));
	uint32_t dtfi = (uint32_t) round((tdf * (0.170 * 8)));

	if(dtri > 0xff){
		dtri = 0xff;
	}
	if(dtfi > 0xff){
		dtfi = 0xff;
	}

	HRTIM1->sTimerxRegs[HW_TIMERINDEX].DTxR |= (dtri & 0xff) | ((dtfi&0xff) << 16);

	HAL_HRTIM_WaveformCounterStart_IT(&hhrtim1, HRTIM_TIMERID_MASTER);
	HAL_HRTIM_WaveformCountStart(&hhrtim1, HRTIM_TIMERID_TIMER_B);
	HAL_HRTIM_WaveformOutputStart(&hhrtim1, HRTIM_OUTPUT_TB1|HRTIM_OUTPUT_TB2);
}

void pwm_setDuty(float duty) {
	uint32_t cmpval1;
	uint32_t per = HRTIM1->sTimerxRegs[1].PERxR;
	cmpval1 = (uint32_t) roundf((float) per * duty);
	uint32_t adccmp = ((cmpval1 / 2) + (int32_t) roundf((((triggerdelay) / 1000.0f) / timebase))) % per;

	//Make sure the AC values operate within their limits.
	if (cmpval1 < 64)
		cmpval1 = 64;
	if (cmpval1 > (per - 64))
		cmpval1 = (per - 64);

	if (adccmp < 64)
		adccmp = 64;
	if (adccmp > (per - 64))
		adccmp = (per - 64);

	HRTIM1->sTimerxRegs[HW_TIMERINDEX].CMP2xR = cmpval1;
	HRTIM1->sTimerxRegs[HW_TIMERINDEX].CMP3xR = adccmp;
}

inline void pwm_disable() {
	HAL_HRTIM_WaveformCounterStop_IT(&hhrtim1, HRTIM_TIMERID_MASTER);
	HAL_HRTIM_WaveformCountStop(&hhrtim1, HRTIM_TIMERID_TIMER_B);
	HAL_HRTIM_WaveformOutputStop(&hhrtim1, HRTIM_OUTPUT_TB1 | HRTIM_OUTPUT_TB2);

}

float pwm_GetControllerPeriod() {
	return ControllerPeriod;
}

void HRTIM1_Master_IRQHandler(void){
  HAL_HRTIM_IRQHandler(&hhrtim1,HRTIM_TIMERINDEX_MASTER);
}

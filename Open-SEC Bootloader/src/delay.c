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
 
#include "delay.h"

static uint32_t hmsCnt = 0;

void modDelayInit(void) {
	SystemCoreClockUpdate();
	
	//SystemCoreClock / 1000
	if(SysTick_Config(SystemCoreClock / 1000)){
		while(1); //Error setting SysTick.
	}
}

uint8_t modDelayTick1ms(uint32_t *last, uint32_t ticks) {
	if((uint32_t)(HAL_GetTick() - *last) >= ticks)
	{
		*last = HAL_GetTick();
		return true;
	}

	return false;
}

uint8_t modDelayTick100ms(uint32_t *last, uint32_t ticks) {
	static uint32_t msTicks = 0;

	if(modDelayTick1ms(&msTicks,99))
		hmsCnt++;

	if((uint32_t)(hmsCnt - *last) >= ticks)
	{
		*last = hmsCnt;
		return true;
	}

	return false;
}

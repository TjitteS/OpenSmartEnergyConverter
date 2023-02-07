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

void hw_setio_output(GPIO_TypeDef * port, uint32_t pin){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void hw_setio_input(GPIO_TypeDef * port, uint32_t pin, uint32_t pull){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = pull;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void hw_setio_af(GPIO_TypeDef * port, uint32_t pin, uint32_t af){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = af;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void hw_setio_analog(GPIO_TypeDef * port, uint32_t pin){
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = pin;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(port, &GPIO_InitStruct);
}

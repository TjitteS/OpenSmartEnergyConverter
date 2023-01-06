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
 
#ifndef SRC_PWM_H_
#define SRC_PWM_H_

#include "hardware.h"

void pwm_init(float switchingFrequency, float controllerFrequency, float dtr, float dtf, float adc_td);
void pwm_setDuty(float duty);
void pwm_disable();
float pwm_GetControllerPeriod(void);

#endif /* SRC_PWM_H_ */

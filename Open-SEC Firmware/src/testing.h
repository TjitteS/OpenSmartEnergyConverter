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
 
#ifndef INC_MODTESTING_H_
#define INC_MODTESTING_H_

#include "control.h"

#define NSTATES 3
#define INPUTS 2

//float state[NSTATES];

typedef struct{
	float Vlow;
	float Vhigh;
	float Iind;
	float Ihigh;
}modTestingSimState_t;

modTestingSimState_t simstate;

typedef enum tests{
	test_nominal,
	test_load_disconect,
	test_lastest,
}modTestS_t;

typedef struct{
	float C1;
	float C2;
	float Isc;
	float Voc;
}modTestingSolarCell_t;

void modTestingInit(void);
void modTestingTask(void);

void modTestingModel(float* x, float* dx, float*u);

void modTestingSimstep(modTestingSimState_t *state, float dt, ConverterPhase_t* phase);

#endif /* INC_MODTESTING_H_ */

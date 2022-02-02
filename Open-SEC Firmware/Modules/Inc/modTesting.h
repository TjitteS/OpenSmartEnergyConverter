/*
 * modTesting.h
 *
 *  Created on: Jan 16, 2021
 *      Author: Tjitte
 */

#ifndef INC_MODTESTING_H_
#define INC_MODTESTING_H_

#include "modConverter.h"

#define NSTATES 3
#define INPUTS 2

float state[NSTATES];

typedef struct{
	float Vin;
	float Vout;
	float Il;
	float Iout;
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

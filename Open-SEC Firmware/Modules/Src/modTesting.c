/*
 * modTesting.c
 *
 *  Created on: Jan 16, 2021
 *      Author: Tjitte
 */
#include "modTesting.h"
#include "modDelay.h"
#include  "modConverter.h"


//#define SIM_BATTERY_DISCONECT
//#define SIM_NOMINAL
#define SIM_MULTPP
//#define SIM_FWDMODE

#ifdef SIM_FWDMODE
#define Vbat 15.0f
#else
#define Vbat 50.0f
#endif

#define IVC_SIZE 32
float IVc1[IVC_SIZE][2] = {{0.00000, 5.99999},{0.77419,  5.99999},{1.54839,  5.99997},{2.32258,  5.99994},{3.09677,  5.99988},{3.87097,  5.99975},{4.64516,  5.99946},{5.41935,  5.99883},{6.19355,  5.99743},{6.96774,  5.99434},{7.74194,  5.98746},{8.51613,  5.97224},{9.29032,  5.93908},{10.0645,  5.86951},{10.8387,  5.73388},{11.6129,  5.49793},{12.3871,  5.14028},{13.1613,  4.66412},{13.9355,  4.09187},{14.7097,  3.46056},{15.4839,  3.00222},{16.2581,  2.99999},{17.0323,  2.99993},{17.8065,  2.99929},{18.5806,  2.99263},{19.3548,  2.93376},{20.129 ,  2.69399},{20.9032,  2.27170},{21.6774,  1.74942},{22.4516,  1.16974},{23.2258,  0.55207},{24.0000,  -0.0932}};
float IVc2[IVC_SIZE][2] = {{0.0000,5.9999},  {0.77419,5.9999},{1.54839,5.9999},{2.32258,5.9999},{3.09677,5.9998},{3.87097,5.9997},{4.64516,5.9994},{5.41935,5.9989},{6.19355,5.9975},{6.96774,5.9946},{7.74194,5.9882},{8.51613,5.9738},{9.29032,5.9426},{10.0645,5.8769},{10.8387,5.7480},{11.6129,5.5221},{12.3871,5.1779},{13.1613,4.7207},{13.9355,4.1956},{14.7097,4.0000},{15.4839,3.9999},{16.2581,3.9998},{17.0323,3.9982},{17.8065,3.9822},{18.5806,3.8732},{19.3548,3.5770},{20.1290,3.1441},{20.9032,2.6318},{21.6774,2.0675},{22.4516,1.4655},{23.2258,0.8347},{24.0000,-0.01811}};

float mpp;
float lossedEnergy = 0;
float runtime= 0;
float lossedPowerAverage=0;

float * cvic;
modTestS_t test;

uint32_t lastTestTick = 0;
modTestingSolarCell_t cell;
uint32_t timestep = 0;

extern ConverterSettings_t settings;

bool curvestate;

void modTestingPVInit(modTestingSolarCell_t* cell, float Vmp, float Voc, float Imp, float Isc, float A){
	Imp = Imp * A / 1000.0f;
	Isc = Isc * A / 1000.0f;

	cell->Isc = Isc;
	cell->Voc = Voc;
	cell->C2 = (Vmp-Voc)/logf(1- (Imp/Isc));
	cell->C1 = (Isc-Imp)/expf((Vmp-Voc)/cell->C2);
}

float getMpp(float* IVc){
	float mpp = 0.0f;
	for (int i = 0; i < IVC_SIZE; i++){
		float p = IVc[i*2]*IVc[i*2+1];
		if(p > mpp)mpp=p;
	}
	return mpp;
}

void modTestingInit(){
	modTestingPVInit(&cell, 40*0.649f, 40*0.742f, 1.3f, 1.5f, 1000);
	simstate.Vin=30.0f;
	simstate.Vout=Vbat;
	simstate.Il = 0;
	simstate.Iout = 0;
	cvic = IVc2;
	mpp = getMpp(IVc2);

}



void modTestingTask(){
	if(modDelayTick1ms(&lastTestTick, 1000)){
		curvestate = !curvestate;
		if(curvestate){
			mpp = getMpp(IVc2);

			cvic = IVc2;
		}else{
			mpp = getMpp(IVc1);
			cvic = IVc1;
		}
	}
}

float Ipvmodel(modTestingSolarCell_t *cell, float V){
	if(V <= 0.0f){
		return cell->Isc;
	}
	if (V >= cell->Voc){
		return 0;
	}
	return (cell->Isc - cell->C1*expf(-cell->Voc/cell->C2)*(expf(V/cell->C2)-1));
}

float IpvPanel(float* IVc, float V){
	//V /= 1000.0f;
	int i;
	for (i = 0; i < IVC_SIZE; i++){
		float v = IVc[i*2];
		if (v > V){
			break;
		}
	}
	if(i == 0){
		return IVc[1];
	}

	float i0 = IVc[(i*2)+1];
	float v0 = IVc[(i*2)];
	float i1 = IVc[(i*2)-1];
	float v1 = IVc[(i*2)-2];

	return (V-v0)/(v1-v0) * (i1-i0) + i0;
}

void modTestingSimstep(modTestingSimState_t *state, float dt, ConverterPhase_t* phase){
	float Iin = 0.0;

#ifdef SIM_NOMINAL
	Iin = Ipvmodel(&cell,state->Vin);
	state->Iout= (state->Vout-Vbat) / 0.1f;
#elif defined (SIM_BATTERY_DISCONECT)
	Iin = Ipvmodel(&cell,state->Vin);
	state->Iout = 0;
#elif defined (SIM_MULTPP)
	Iin = IpvPanel(cvic ,state->Vin);
	state->Iout= (state->Vout-Vbat) / 0.1f;
#elif defined SIM_FWDMODE
	Iin = Ipvmodel(&cell,state->Vin);
	state->Iout= (state->Vout-Vbat) / 0.1f;
#endif

	float Dn = 1.0f-phase->dutycycle;

	if(Dn > 1.0f)Dn = 1.0f;
	if(Dn < 0.0f)Dn = 0.0f;


	float dVindt 	= 0;
	float dIldt 	= 0;
	float dVoutdt 	= 0;

	if(phase->enabled){
		dVindt 	= (Iin - state->Il)/settings.Cin*dt;
		dIldt 	= (state->Vin -(Dn*state->Vout))/settings.L*dt;
		dVoutdt = ((state->Il*Dn)-state->Iout-(0.3f/state->Vout))/settings.Cout*dt;
	}else{

		dVindt 	= (Iin - state->Il)/settings.Cin*dt;
		dIldt 	= -state->Il;
		dVoutdt = (-state->Iout-(0.3f/state->Vout))/settings.Cout*dt;

		if(state->Vout < (state->Vin + 2.5f)){
			dVoutdt = ((state->Il*Dn)-state->Iout-(0.3f/state->Vout))/settings.Cout*dt;
			dIldt 	= (state->Vin -state->Vout -2.5f - (state->Il*2.0))/settings.L*dt; // Added damping turn to increase stability
		}

	}

	state->Il 	+= dIldt;
	state->Vin	+= dVindt;
	state->Vout	+= dVoutdt;

	lossedEnergy += dt*((state->Vin*Iin)-mpp);
	runtime += dt;
	lossedPowerAverage = lossedEnergy/runtime;
	timestep++;


}

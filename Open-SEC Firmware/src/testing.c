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
 
#include "testing.h"
#include "delay.h"
#include "control.h"


//#define SIM_BATTERY_DISCONECT
#define SIM_NOMINAL
//#define SIM_MULTPP
//#define SIM_FWDMODE

#ifdef SIM_FWDMODE
#define Vbat 15.0f
#else

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
extern CalibrationData_t cal;

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

#if defined(HW_TOPOLOGY_BOOST)
#define Vbat 42.0f
	modTestingPVInit(&cell, 40*0.649f, 40*0.742f, 1.3f, 1.5f, 1000);
	simstate.Vlow=30.0f;
	simstate.Vhigh=Vbat;

#elif defined(HW_TOPOLOGY_BUCK)
#define Vbat 20.0f
	modTestingPVInit(&cell, 70*0.649f, 70*0.742f, 1.3f, 1.5f, 1000);
	simstate.Vlow=Vbat;
	simstate.Vhigh=60.0f;
#endif

	simstate.Iind = 0;
	simstate.Ihigh = 0;
	cvic = (float*)IVc2;
	mpp = getMpp( (float*)IVc2 );
}



void modTestingTask(){
	if(modDelayTick1ms(&lastTestTick, 1000)){
		curvestate = !curvestate;
		if(curvestate){
			mpp = getMpp((float*)IVc2);

			cvic = (float*)IVc2;
		}else{
			mpp = getMpp((float*)IVc1);
			cvic = (float*)IVc1;
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
	float Ilow = 0.0;
#ifdef HW_TOPOLOGY_BOOST
#ifdef SIM_NOMINAL
	Ilow = Ipvmodel(&cell,state->Vlow);
	state->Ihigh= (state->Vhigh-Vbat) / 0.1f;
#elif defined (SIM_BATTERY_DISCONECT)
	Iin = Ipvmodel(&cell,state->Vin);
	state->Iout = 0;
#elif defined (SIM_MULTPP)
	Ilow = IpvPanel(cvic ,state->Vlow);
	state->Ihigh= (state->Vhigh-Vbat) / 0.1f;
#elif defined SIM_FWDMODE
	Iin = Ipvmodel(&cell,state->Vin);
	state->Iout= (state->Vout-Vbat) / 0.1f;
#endif

#elif defined HW_TOPOLOGY_BUCK
#ifdef SIM_NOMINAL
	Ilow = ((Vbat - state->Vlow) / 0.100f) - (state->Vlow / 1.0e4f);
	state->Ihigh = -Ipvmodel(&cell,state->Vhigh);
#elif defined SIM_BATTERY_DISCONECT
	Ilow = -state->Vlow / 1.0e4f;
	state->Ihigh = (state->Vhigh - 70.0f) / 8.0f;
#endif
#endif

	float Dn = 1.0f-phase->dutycycle;

	if(Dn > 1.0f)Dn = 1.0f;
	if(Dn < 0.0f)Dn = 0.0f;


	float dVlowdt 	= 0;
	float dIinddt 	= 0;
	float dVhighdt 	= 0;

	if(phase->enabled){
		dVlowdt 	= (Ilow - state->Iind)/cal.Clow*dt;
		dIinddt 	= (state->Vlow -(Dn*state->Vhigh))/cal.L*dt;
		dVhighdt = ((state->Iind*Dn)-state->Ihigh-(0.3f/state->Vhigh))/cal.Chigh*dt;
	}else{

		dVlowdt 	= (Ilow - state->Iind)/cal.Clow*dt;
		dIinddt 	= -state->Iind;
		dVhighdt = (-state->Ihigh-(0.3f/state->Vhigh))/cal.Chigh*dt;

		if(state->Vhigh < (state->Vlow + 2.5f)){
			dVhighdt = ((state->Iind*Dn)-state->Ihigh-(0.3f/state->Vhigh))/cal.Chigh*dt;
			dIinddt 	= (state->Vlow -state->Vhigh -2.5f - (state->Iind*2.0f))/cal.L*dt; // Added damping turn to increase stability
		}

	}

	state->Iind 	+= dIinddt;
	state->Vlow	+= dVlowdt;
	state->Vhigh	+= dVhighdt;

	lossedEnergy += dt*((state->Vlow*Ilow)-mpp);
	runtime += dt;
	lossedPowerAverage = lossedEnergy/runtime;
	timestep++;
}

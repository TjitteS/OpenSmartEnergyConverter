#include "modCommands.h"
#include "main.h"
#include "stm32g4xx_hal.h"
#include "libBuffer.h"
#include "modconfig.h"
#include "modMPPT.h"

extern modConfig_t* acktiveConfig;

// Private variables
#define PACKET_MAX_PL_LEN 256
static uint8_t modCommandsSendBuffer[PACKET_MAX_PL_LEN];
static void(*modCommandsSendFunction)(unsigned char *data, unsigned int len) = 0;
bool jumpBootloaderTrue;


void modCommandsInit() {
	//modCommandsGeneralConfig = configPointer;
	//modCommandsGeneralState  = generalState;
	jumpBootloaderTrue = false;
}

void modCommandsSetSendFunction(void(*func)(unsigned char *data, unsigned int len)) {
	modCommandsSendFunction = func;
}

void modCommandsSendPacket(unsigned char *data, unsigned int len) {
	if (modCommandsSendFunction) {
		modCommandsSendFunction(data, len);
	}
}

void modCommandsProcessPacket(unsigned char *data, unsigned int len) {
	if (!len) {
		return;
	}

	COMM_PACKET_ID packet_id;
	int32_t ind = 0;
	uint16_t flash_res;
	uint32_t new_app_offset;
	uint32_t delayTick;
	uint8_t cellPointer;

	float start;
	float end;
	float tempfloat;


	packet_id = (COMM_PACKET_ID) data[0];
	data++;
	len--;

	switch (packet_id) {
		case COMM_FW_VERSION:
			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_FW_VERSION;
			modCommandsSendBuffer[ind++] = FW_VERSION_MAJOR;
			modCommandsSendBuffer[ind++] = FW_VERSION_MINOR;
			strcpy((char*)(modCommandsSendBuffer + ind), HW_NAME);
			ind += strlen(HW_NAME) + 1;
			memcpy(modCommandsSendBuffer + ind, UID_BASE, 12);
			ind += 12;

			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		case COMM_JUMP_TO_BOOTLOADER:
			jumpBootloaderTrue = true;
			delayTick = HAL_GetTick();
			break;
		case COMM_ERASE_NEW_APP:
			break;
		case COMM_WRITE_NEW_APP_DATA:
			break;
		case COMM_GET_VALUES:
			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_GET_VALUES;
			buffer_append_float16(modCommandsSendBuffer, meter.Iin             	    ,3.0e3f, &ind);
			buffer_append_float16(modCommandsSendBuffer, meter.Iout            	    ,3.0e3f, &ind);
			buffer_append_float16(modCommandsSendBuffer, meter.Ipv                  ,3.0e3f, &ind);
			buffer_append_float16(modCommandsSendBuffer, meter.Vin              	,0.3e3f, &ind);
			buffer_append_float16(modCommandsSendBuffer, meter.Vout                 ,0.3e3f, &ind);
			buffer_append_float16(modCommandsSendBuffer, meter.TemperatureAmbient 	,0.1e3f, &ind);
			buffer_append_float16(modCommandsSendBuffer, meter.TemperatureHeatsink	,0.1e3f, &ind);
			buffer_append_float16(modCommandsSendBuffer, meter.Eff	,3.0e4f, &ind);
			buffer_append_uint8(modCommandsSendBuffer, modConverterGetMode(),  &ind);
			buffer_append_uint8(modCommandsSendBuffer, modConverterGetFault(),  &ind);

			modCommandsSendPacket(modCommandsSendBuffer, ind);

			break;
		
		case COMM_SET_MCCONF:
			ind = 0;

			//Before writing the data, disable the outptut.

			mainHaltRisky();
			acktiveConfig->calData.InputCurrentGain = buffer_get_float32_auto(data,&ind);
			acktiveConfig->calData.InputCurrentOffset = buffer_get_float32_auto(data,&ind);

			acktiveConfig->calData.OutputCurrentGain = buffer_get_float32_auto(data,&ind);
			acktiveConfig->calData.OutputCurrentOffset = buffer_get_float32_auto(data,&ind);

			acktiveConfig->calData.InputVoltageGain = buffer_get_float32_auto(data,&ind);
			acktiveConfig->calData.InputVoltageOffset = buffer_get_float32_auto(data,&ind);

			acktiveConfig->calData.OutputVoltageGain = buffer_get_float32_auto(data,&ind);
			acktiveConfig->calData.OutputVoltageOffset = buffer_get_float32_auto(data,&ind);

			//Temperature
			acktiveConfig->calData.Temperature_B = buffer_get_float16(data,1.0f,&ind);
			acktiveConfig->calData.Temperature_R = buffer_get_uint16(data,&ind);
			acktiveConfig->calData.Temperature_Ref = buffer_get_float16(data,200.0f,&ind);

			//Cal general
			acktiveConfig->calData.ADC_delay = buffer_get_float16(data,2.0e3f,&ind);

			acktiveConfig->calData.calibrated = buffer_get_int8(data,&ind);

			acktiveConfig->settings.SwitchingFrequency        = (float) buffer_get_uint16(data,&ind);
			acktiveConfig->settings.ControllerFrequency       = (float )buffer_get_uint8(data,&ind);
			acktiveConfig->settings.DeadTime                  = buffer_get_float16(data, 1.0e2f,&ind);
			acktiveConfig->settings.L                         = 1.0e-6f*(float )buffer_get_uint16(data,&ind);
			acktiveConfig->settings.Cin                       = 1.0e-6f*(float )buffer_get_uint16(data,&ind);
			acktiveConfig->settings.Cout                      = 1.0e-6f*(float )buffer_get_uint16(data,&ind);
			acktiveConfig->settings.RLint                     = 1.0e-3f*buffer_get_float16(data, 1.0e1f,&ind);
			acktiveConfig->settings.Q                         = buffer_get_float16(data, 1.0e4f,&ind);
			acktiveConfig->settings.Klim                      = buffer_get_float16(data, 1.0e4f,&ind);
			acktiveConfig->settings.OutputVoltageLimitSoft    = 1.0e3f*buffer_get_float16(data, 1.0e2f,&ind);
			acktiveConfig->settings.OutputVoltageLimitHard    = 1.0e3f*buffer_get_float16(data, 1.0e2f,&ind);
			acktiveConfig->settings.InputVoltageLimitSoft     = 1.0e3f*buffer_get_float16(data, 1.0e2f,&ind);
			acktiveConfig->settings.InputVoltageLimitHard     = 1.0e3f*buffer_get_float16(data, 1.0e2f,&ind);
			acktiveConfig->settings.OutputCurrentLimitSoft    = 1.0e3f*buffer_get_float16(data, 1.0e3f,&ind);
			acktiveConfig->settings.OutputCurrentLimitHard    = 1.0e3f*buffer_get_float16(data, 1.0e3f,&ind);
			acktiveConfig->settings.InputCurrentLimitSoft     = 1.0e3f*buffer_get_float16(data, 1.0e3f,&ind);
			acktiveConfig->settings.InputCurrentLimitHard     = 1.0e3f*buffer_get_float16(data, 1.0e3f,&ind);
			acktiveConfig->settings.PhaseHighSideEnableCurrent= 1.0e3f*buffer_get_float16(data, 1.0e3f,&ind);
			acktiveConfig->settings.PhaseCurrentMin           = 1.0e3f*buffer_get_float16(data, 1.0e3f,&ind);
			acktiveConfig->settings.outputEnable = buffer_get_int8(data,&ind);
			acktiveConfig->settings.meterfilterCoeficient = buffer_get_float16(data, 1.0e4f,&ind);

			acktiveConfig->mpptsettings.PO_Stepsize = 1.0e3f*buffer_get_float16(data, 1e3, &ind);
			acktiveConfig->mpptsettings.PO_Timestep = (uint32_t)buffer_get_uint16(data, &ind);
			acktiveConfig->mpptsettings.PO_maxgradient = buffer_get_float16(data, 1e2, &ind);

			acktiveConfig->mpptsettings.jump_PowerThreshold = buffer_get_float16(data, 1e3, &ind);
			acktiveConfig->mpptsettings.jump_RangeMax = 1.0e3f*buffer_get_float16(data, 1e2, &ind);;
			acktiveConfig->mpptsettings.jump_RangeMin = 1.0e3f*buffer_get_float16(data, 1e2, &ind);
			acktiveConfig->mpptsettings.jump_Rate = buffer_get_uint16(data, &ind);

			acktiveConfig->cansettings.baudrate			= buffer_get_uint16(data, &ind);
			acktiveConfig->cansettings.samplepoint		= buffer_get_float16(data, 1e2, &ind)/100.0f;
			acktiveConfig->cansettings.generalCanId		= buffer_get_uint16(data, &ind);

			ind = 0;
			modCommandsSendBuffer[ind++] = packet_id;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
			mainInitAfterConfig();

			break;
		case COMM_GET_MCCONF:
		case COMM_GET_MCCONF_DEFAULT:
			if(packet_id == COMM_GET_MCCONF_DEFAULT){
				//modConfigLoadDefaultConfig(&modCommandsConfigStorage);
				//modCommandsToBeSendConfig = &modCommandsConfigStorage;
				modConfigLoadDefaultConfig();
			}else{
				//modCommandsToBeSendConfig = modCommandsGeneralConfig;
			}
		
			ind = 0;
			modCommandsSendBuffer[ind++] = packet_id;
		  
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->calData.InputCurrentGain,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->calData.InputCurrentOffset,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->calData.OutputCurrentGain,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->calData.OutputCurrentOffset,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->calData.InputVoltageGain,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->calData.InputVoltageOffset,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->calData.OutputVoltageGain,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->calData.OutputVoltageOffset,&ind);

			//Temperature
			buffer_append_float16(modCommandsSendBuffer,acktiveConfig->calData.Temperature_B,1.0f,&ind);
			buffer_append_uint16(modCommandsSendBuffer,acktiveConfig->calData.Temperature_R,&ind);
			buffer_append_float16(modCommandsSendBuffer,acktiveConfig->calData.Temperature_Ref,200.0f,&ind);

			//Cal general
			buffer_append_float16(modCommandsSendBuffer,acktiveConfig->calData.ADC_delay,2.0e3f,&ind);
			buffer_append_int8(modCommandsSendBuffer,acktiveConfig->calData.calibrated,&ind);

			//settings
			buffer_append_uint16	(modCommandsSendBuffer,acktiveConfig->settings.SwitchingFrequency        ,&ind);
			buffer_append_uint8		(modCommandsSendBuffer,acktiveConfig->settings.ControllerFrequency       ,&ind);
			buffer_append_float16	(modCommandsSendBuffer,acktiveConfig->settings.DeadTime 				 ,1.0e2f,&ind);
			buffer_append_uint16	(modCommandsSendBuffer,1.0e6f*acktiveConfig->settings.L                         ,&ind);
			buffer_append_uint16	(modCommandsSendBuffer,1.0e6f*acktiveConfig->settings.Cin                       ,&ind);
			buffer_append_uint16	(modCommandsSendBuffer,1.0e6f*acktiveConfig->settings.Cout                      ,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e3f*acktiveConfig->settings.RLint                     ,1.0e1f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,acktiveConfig->settings.Q                         ,1.0e4f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,acktiveConfig->settings.Klim                      ,1.0e4f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.OutputVoltageLimitSoft    ,1.0e2f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.OutputVoltageLimitHard    ,1.0e2f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.InputVoltageLimitSoft     ,1.0e2f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.InputVoltageLimitHard     ,1.0e2f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.OutputCurrentLimitSoft    ,1.0e3f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.OutputCurrentLimitHard    ,1.0e3f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.InputCurrentLimitSoft     ,1.0e3f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.InputCurrentLimitHard     ,1.0e3f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.PhaseHighSideEnableCurrent,1.0e3f,&ind);
			buffer_append_float16	(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.PhaseCurrentMin           ,1.0e3f,&ind);
			buffer_append_int8		(modCommandsSendBuffer,acktiveConfig->settings.outputEnable          	 ,&ind);
			buffer_append_float16	(modCommandsSendBuffer,acktiveConfig->settings.meterfilterCoeficient           ,1.0e4f,&ind);

			buffer_append_float16	(modCommandsSendBuffer, 1.0e-3f*acktiveConfig->mpptsettings.PO_Stepsize, 1.0e3f,&ind);
			buffer_append_uint16	(modCommandsSendBuffer, acktiveConfig->mpptsettings.PO_Timestep, &ind);
			buffer_append_float16	(modCommandsSendBuffer, acktiveConfig->mpptsettings.PO_maxgradient, 1.0e2f,&ind);

			buffer_append_float16	(modCommandsSendBuffer, acktiveConfig->mpptsettings.jump_PowerThreshold, 1.0e3f,&ind);
			buffer_append_float16	(modCommandsSendBuffer, 1.0e-3f*acktiveConfig->mpptsettings.jump_RangeMax, 1.0e2f,&ind);
			buffer_append_float16	(modCommandsSendBuffer, 1.0e-3f*acktiveConfig->mpptsettings.jump_RangeMin, 1.0e2f,&ind);
			buffer_append_uint16	(modCommandsSendBuffer, acktiveConfig->mpptsettings.jump_Rate,&ind);


			buffer_append_uint16(modCommandsSendBuffer,  acktiveConfig->cansettings.baudrate,&ind);
			buffer_append_float16(modCommandsSendBuffer, acktiveConfig->cansettings.samplepoint*100.0f, 1e2, &ind);
			buffer_append_uint16(modCommandsSendBuffer,  acktiveConfig->cansettings.generalCanId,&ind);

			modCommandsSendPacket(modCommandsSendBuffer, ind);
		
			break;
		case COMM_TERMINAL_CMD:
		  data[len] = '\0';
		  terminal_process_string((char*)data);
			break;
		case COMM_REBOOT:
			modCommandsJumpToMainApplication();
			break;
		case COMM_ALIVE:
			break;
		case COMM_FORWARD_CAN:
			modCANSendBuffer(data[0], data + 1, len - 1, false);
			break;
		case COMM_STORE_MPPT_CONF:
			modConfigStoreConfig();
		
			ind = 0;
			modCommandsSendBuffer[ind++] = packet_id;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		case CONM_MPPT_GET_SWEEP:
			start = 1.0e3f*buffer_get_float16(data, 1.0e2f,&ind);
			end   = 1.0e3f*buffer_get_float16(data, 1.0e2f,&ind);
			modMpptStartSweep(start,end);
			break;

		case CONM_MPPT_SetMode:
			modMpptSetMode((modMPPTmode_t) buffer_get_uint8(data, &ind));
			break;

		case CONM_MPPT_SetOutputVoltage:
			tempfloat = 1000.0*buffer_get_float16(data, 1.0e2f,&ind);
			modConverterSetOutputVoltage(tempfloat);

			break;
		case CONM_MPPT_SetOutputCurrent:
			tempfloat = 1000.0*buffer_get_float16(data, 1.0e3f,&ind);
			modConverterSetOutputCurrent(tempfloat);

			break;
		case CONM_MPPT_SetInputCurrent:
			tempfloat = 1000.0*buffer_get_float16(data, 1.0e3f,&ind);
			modConverterSetInputCurrent(tempfloat);

			break;
		case CONM_MPPT_SetInputVoltage:
			tempfloat = 1000.0*buffer_get_float16(data, 1.0e2f,&ind);
			modConverterSetInputVoltage(tempfloat);
			break;
		default:
			break;
	}
	
	//if(modDelayTick1ms(&delayTick,1000) && jumpBootloaderTrue)
		//modFlashJumpToBootloader();
}

void modCommandsPrintf(const char* format, ...) {
	va_list arg;
	va_start (arg, format);
	int len;
	static char print_buffer[255];

	print_buffer[0] = COMM_PRINT;
	len = vsnprintf(print_buffer+1, 254, format, arg);
	va_end (arg);

	if(len > 0) {
		modCommandsSendPacket((unsigned char*)print_buffer, (len<254)? len+1: 255);
	}
}


void modCommandsJumpToMainApplication(void) {
	NVIC_SystemReset();
}

void modCommandsSendSweep(){
	int32_t ind = 0;
	buffer_append_uint8(modCommandsSendBuffer, CONM_MPPT_GET_SWEEP,&ind);
	buffer_append_uint8(modCommandsSendBuffer, (uint8_t) MPPT_SWEEP_SIZE,&ind);
	for(int i =0; i < MPPT_SWEEP_SIZE; i++){
		buffer_append_float16(modCommandsSendBuffer, 1.0e-2f*mppt_vs[i], 1.0e2f,&ind);
		buffer_append_float16(modCommandsSendBuffer, 1.0e-3f*mppt_is[i], 1.0e3f,&ind);
	}
	modCommandsSendPacket(modCommandsSendBuffer, ind);
}

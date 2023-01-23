#include "commands.h"
#include "main.h"
#include "stm32g4xx_hal.h"
#include "buffer.h"
#include "config.h"
#include "mppt.h"
#include "control.h"
#include "flash.h"

extern modConfig_t* acktiveConfig;

static uint8_t modCommandsSendBuffer[PACKET_MAX_PL_LEN];
static void(*modCommandsSendFunction)(unsigned char *data, unsigned int len) = 0;

bool jumpBootloaderTrue;
uint32_t delaytick;


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
COMM_PACKET_ID last_packet_id;
void modCommandsProcessPacket(unsigned char *data, unsigned int len) {

	if (!len) {
		return;
	}

	COMM_PACKET_ID packet_id;
	int32_t ind = 0;
	float start;
	float end;


	uint16_t flash_res;
	uint32_t new_app_offset;

	uint16_t calcrc;

	packet_id = (COMM_PACKET_ID) data[0];
	last_packet_id =packet_id;
	data++;
	len--;

	float tempfloat;
	switch (packet_id) {
		case COMM_FW_VERSION:
			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_FW_VERSION;
			modCommandsSendBuffer[ind++] = FW_VERSION_MAJOR;
			modCommandsSendBuffer[ind++] = FW_VERSION_MINOR;
			strcpy((char*)(modCommandsSendBuffer + ind), acktiveConfig->calData.HardwareName);
			ind += strlen(acktiveConfig->calData.HardwareName) + 1;
			memcpy(modCommandsSendBuffer + ind, (void*) UID_BASE, 12);
			ind += 12;

			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		case COMM_JUMP_TO_BOOTLOADER:
			main_halt_risky();

			//jumpBootloaderTrue = true;
			//delaytick = HAL_GetTick();

			modFlashJumpToBootloader();
			break;
		case COMM_ERASE_NEW_APP:
			main_halt_risky();
			ind = 0;
			flash_res = modFlashEraseNewAppData(buffer_get_uint32(data, &ind));

			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_ERASE_NEW_APP;
			modCommandsSendBuffer[ind++] = flash_res == HAL_OK ? true : false;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		case COMM_WRITE_NEW_APP_DATA:
			ind = 0;
			new_app_offset = buffer_get_uint32(data, &ind);
			flash_res = modFlashWriteNewAppData(new_app_offset, data + ind, len - ind);

			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_WRITE_NEW_APP_DATA;
			modCommandsSendBuffer[ind++] = flash_res == HAL_OK ? 1 : 0;
			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		case COMM_GET_VALUES:
			ind = 0;
			modCommandsSendBuffer[ind++] = COMM_GET_VALUES;
			buffer_append_float32_auto(modCommandsSendBuffer, meter.Iind , &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, meter.Ihigh, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, meter.Ilow, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, meter.Vlow, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, meter.Vhigh, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, meter.TemperatureHeatsink, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, meter.TemperatureAmbient, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, meter.Eff, &ind);
			buffer_append_uint8(modCommandsSendBuffer, phase.mode,  &ind);
			buffer_append_uint8(modCommandsSendBuffer, phase.fault,  &ind);
			modCommandsSendPacket(modCommandsSendBuffer, ind);
			break;
		
		case COMM_SET_MCCONF:
			ind = 0;
			//Before writing the data, disable the outptut.
			main_halt_risky();
			acktiveConfig->settings.meterfilterCoeficient = buffer_get_float32_auto(data,&ind);
			acktiveConfig->settings.HighSideVoltageLimitSoft    = 1.0e3f * buffer_get_float32_auto(data,&ind);
			acktiveConfig->settings.LowSideVoltageLimitSoft     = 1.0e3f * buffer_get_float32_auto(data,&ind);
			acktiveConfig->settings.HighSideCurrentLimitSoft    = 1.0e3f * buffer_get_float32_auto(data,&ind);
			acktiveConfig->settings.LowSideCurrentMaxLimitSoft  = 1.0e3f * buffer_get_float32_auto(data,&ind);
			acktiveConfig->settings.PhaseHighSideEnableCurrent  = 1.0e3f * buffer_get_float32_auto(data,&ind);
			acktiveConfig->settings.LowSideCurrentMinLimitSoft  = 1.0e3f * buffer_get_float32_auto(data,&ind);
			acktiveConfig->settings.TemperatureLimitStart		= buffer_get_float32_auto(data,&ind);
			acktiveConfig->settings.TemperatureLimitEnd			= buffer_get_float32_auto(data,&ind);
			acktiveConfig->settings.outputEnable = buffer_get_int8(data,&ind);

			acktiveConfig->mpptsettings.PO_Stepsize = buffer_get_float32_auto(data,&ind);
			acktiveConfig->mpptsettings.PO_Timestep = (uint32_t)buffer_get_uint16(data, &ind);

			acktiveConfig->mpptsettings.PO_StepSizeGain = buffer_get_float32_auto(data,&ind);
			acktiveConfig->mpptsettings.jump_PowerThreshold = buffer_get_float32_auto(data,&ind);

			acktiveConfig->mpptsettings.jump_Rate 		= buffer_get_uint16(data, &ind);
			acktiveConfig->cansettings.baudrate			= buffer_get_uint16(data, &ind);
			acktiveConfig->cansettings.samplepoint		= buffer_get_float32_auto(data,&ind)/100.0;
			acktiveConfig->cansettings.generalCanId		= buffer_get_uint16(data, &ind);

			modConfigLimitSettings(acktiveConfig);
			ind = 0;
			modCommandsSendBuffer[ind++] = packet_id;
			modCommandsSendPacket(modCommandsSendBuffer, ind);


			main_init_after_config();

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

			//settings
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->settings.meterfilterCoeficient           ,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.HighSideVoltageLimitSoft    ,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.LowSideVoltageLimitSoft     ,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.HighSideCurrentLimitSoft    ,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.LowSideCurrentMaxLimitSoft  ,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.PhaseHighSideEnableCurrent ,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,1.0e-3f*acktiveConfig->settings.LowSideCurrentMinLimitSoft ,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->settings.TemperatureLimitStart ,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer,acktiveConfig->settings.TemperatureLimitEnd ,&ind);
			buffer_append_int8		(modCommandsSendBuffer,acktiveConfig->settings.outputEnable          	 ,&ind);



			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->mpptsettings.PO_Stepsize,&ind);
			buffer_append_uint16	(modCommandsSendBuffer, acktiveConfig->mpptsettings.PO_Timestep, &ind);

			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->mpptsettings.PO_StepSizeGain, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->mpptsettings.jump_PowerThreshold,&ind);

			buffer_append_uint16(modCommandsSendBuffer, acktiveConfig->mpptsettings.jump_Rate,&ind);
			buffer_append_uint16(modCommandsSendBuffer,  acktiveConfig->cansettings.baudrate,&ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->cansettings.samplepoint*100.0f, &ind);
			buffer_append_uint16(modCommandsSendBuffer,  acktiveConfig->cansettings.generalCanId,&ind);

			modCommandsSendPacket(modCommandsSendBuffer, ind);
			if(packet_id == COMM_GET_MCCONF_DEFAULT){
				main_init_after_config();
			}
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

		case COMM_WRITE_CALDATA:
			main_halt_risky();
			//buffer_append_str(modCommandsSendBuffer, acktiveConfig->calData.HardwareName, 32
			//buffer_append_str(modCommandsSendBuffer, acktiveConfig->calData.HardwhareVersionString, 8
			//buffer_append_str(modCommandsSendBuffer, acktiveConfig->calData.SerialString, 8
			ind = 0;
			memcpy(acktiveConfig->calData.HardwareName, data + ind, 32);
			ind += 32;
			memcpy(acktiveConfig->calData.HardwhareVersionString, data + ind, 8);
			ind += 8;
			memcpy(acktiveConfig->calData.SerialString, data + ind, 8);
			ind += 8;
			acktiveConfig->calData.InputCurrentGain     = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.InputCurrentOffset   = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.InputVoltageGain     = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.InputVoltageOffset   = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.OutputCurrentGain    = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.OutputCurrentOffset  = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.OutputVoltageGain    = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.OutputVoltageOffset  = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.Temperature_B        = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.Temperature_R        = buffer_get_float32_auto(data, &ind);
			acktiveConfig->calData.Temperature_Ref      = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.ADC_delay            = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.SwitchingFrequency   = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.ControllerFrequency  = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.DeadTimeRising       = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.DeadtimeFalling      = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.L                    = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.Clow                 = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.Chigh                = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.RLint                = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.Q                    = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.Klim                 = buffer_get_float32_auto(data, &ind);
			//acktiveConfig->calData.calibrated           = buffer_get_int8(data, &ind);
			//acktiveConfig->calData.calcrc =  libCRCCalcCRC16((unsigned char *) &(acktiveConfig->calData), sizeof(CalibrationData_t) - sizeof(uint16_t) - 2);
			break;

		case COMM_READ_CALDATA:
			ind = 0;
			modCommandsSendBuffer[ind++] = packet_id;

			//Information about the hardware
			buffer_append_str(modCommandsSendBuffer, acktiveConfig->calData.HardwareName, 32, &ind); //char HardwareName[32];
			buffer_append_str(modCommandsSendBuffer, acktiveConfig->calData.HardwhareVersionString, 8, &ind); //char HardwhareVersionString[8];
			buffer_append_str(modCommandsSendBuffer, acktiveConfig->calData.SerialString, 8, &ind); //char SerialString[8];
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.InputCurrentGain, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.InputCurrentOffset, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.InputVoltageGain, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.InputVoltageOffset, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.OutputCurrentGain, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.OutputCurrentOffset, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.OutputVoltageGain, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.OutputVoltageOffset, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.Temperature_B, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.Temperature_R, &ind);
			buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.Temperature_Ref, &ind);
			//buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.ADC_delay, &ind);
			//buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.SwitchingFrequency, &ind);
			//buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.ControllerFrequency, &ind);
			//buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.DeadTimeRising, &ind);
			//buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.DeadtimeFalling, &ind);
			///buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.L, &ind);
			//buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.Clow, &ind);
			//buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.Chigh, &ind);
			//buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.RLint, &ind);
			//buffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.Q, &ind);
			//b/uffer_append_float32_auto(modCommandsSendBuffer, acktiveConfig->calData.Klim, &ind);
			//buffer_append_int8(modCommandsSendBuffer, acktiveConfig->calData.calibrated, &ind);

			modCommandsSendPacket(modCommandsSendBuffer, ind);
			//uint32_t reserved[32];
			//uint16_t calcrc; //Checksum
			break;


		case CONM_MPPT_GET_SWEEP:
			start = 1.0e3f*buffer_get_float16(data, 1.0e2f,&ind);
			end   = 1.0e3f*buffer_get_float16(data, 1.0e2f,&ind);
			modMpptStartSweep(start,end);
			break;

		case CONM_MPPT_SetInputVoltage:
			tempfloat = 1000.0*buffer_get_float16(data, 1.0e2f,&ind);
			modMpptSetMode(MpptMode_ConstantVoltage);
			control_set_setpoint(tempfloat);

			/* TODO write interface commands.
		case CONM_MPPT_SetMode:
			modMpptSetMode((modMPPTmode_t) buffer_get_uint8(data, &ind));
			break;

		case CONM_MPPT_SetOutputVoltage:
			tempfloat = 1000.0*buffer_get_float16(data, 1.0e2f,&ind);
			control_set_vhs_limit(tempfloat);

			break;
		case CONM_MPPT_SetOutputCurrent:
			tempfloat = 1000.0*buffer_get_float16(data, 1.0e3f,&ind);
			//modConverterSetOutputCurrent(tempfloat);
			//TODO
			break;
		case CONM_MPPT_SetInputCurrent:
			tempfloat = 1000.0*buffer_get_float16(data, 1.0e3f,&ind);
			//modConverterSetInputCurrent(tempfloat);
			//TODO

			break;
		case CONM_MPPT_SetInputVoltage:
			//tempfloat = 1000.0*buffer_get_float16(data, 1.0e2f,&ind);
			//modConverterSetInputVoltage_dep(tempfloat);
			break;
			*/
		default:
			break;
	}
	
	if(modDelayTick1ms(&delaytick,1000) && jumpBootloaderTrue)
		modFlashJumpToBootloader();
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
		buffer_append_float16(modCommandsSendBuffer, mppt_vs[i], 1.0e2f,&ind);
		buffer_append_float16(modCommandsSendBuffer, mppt_is[i], 1.0e3f,&ind);
	}
	modCommandsSendPacket(modCommandsSendBuffer, ind);
}

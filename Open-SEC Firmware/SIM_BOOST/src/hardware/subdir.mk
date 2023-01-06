################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hardware/hardware.c \
../src/hardware/hw_reboost.c \
../src/hardware/secb.c 

OBJS += \
./src/hardware/hardware.o \
./src/hardware/hw_reboost.o \
./src/hardware/secb.o 

C_DEPS += \
./src/hardware/hardware.d \
./src/hardware/hw_reboost.d \
./src/hardware/secb.d 


# Each subdirectory must supply rules for building sources it contributes
src/hardware/%.o: ../src/hardware/%.c src/hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSIMULATION -DHW_TOPOLOGY_BOOST -DHW_REBOOST -DSTM32G474xx -DUSE_HAL_DRIVER -c -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/STM32G4xx_HAL_Driver/Inc" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/CMSIS/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/CMSIS/Device/ST/STM32G4xx/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/src" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/src/hardware" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-hardware

clean-src-2f-hardware:
	-$(RM) ./src/hardware/hardware.d ./src/hardware/hardware.o ./src/hardware/hw_reboost.d ./src/hardware/hw_reboost.o ./src/hardware/secb.d ./src/hardware/secb.o

.PHONY: clean-src-2f-hardware


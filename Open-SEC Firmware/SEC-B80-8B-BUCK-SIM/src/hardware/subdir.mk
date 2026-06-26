################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/hardware/hardware.c \
../src/hardware/hw_reboost.c \
../src/hardware/secb17510a.c \
../src/hardware/secb1754a.c \
../src/hardware/secb1754b.c \
../src/hardware/secb1757a.c \
../src/hardware/secb808b.c 

OBJS += \
./src/hardware/hardware.o \
./src/hardware/hw_reboost.o \
./src/hardware/secb17510a.o \
./src/hardware/secb1754a.o \
./src/hardware/secb1754b.o \
./src/hardware/secb1757a.o \
./src/hardware/secb808b.o 

C_DEPS += \
./src/hardware/hardware.d \
./src/hardware/hw_reboost.d \
./src/hardware/secb17510a.d \
./src/hardware/secb1754a.d \
./src/hardware/secb1754b.d \
./src/hardware/secb1757a.d \
./src/hardware/secb808b.d 


# Each subdirectory must supply rules for building sources it contributes
src/hardware/%.o src/hardware/%.su src/hardware/%.cyclo: ../src/hardware/%.c src/hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -DSIMULATION -DHW_TOPOLOGY_BUCK -DHW_SECB808B -DUSE_HAL_DRIVER -UHW_TOPOLOGY_BOOST -c -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/STM32G4xx_HAL_Driver/Inc" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/CMSIS/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/CMSIS/Device/ST/STM32G4xx/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/src" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/src/hardware" -Ofast -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-src-2f-hardware

clean-src-2f-hardware:
	-$(RM) ./src/hardware/hardware.cyclo ./src/hardware/hardware.d ./src/hardware/hardware.o ./src/hardware/hardware.su ./src/hardware/hw_reboost.cyclo ./src/hardware/hw_reboost.d ./src/hardware/hw_reboost.o ./src/hardware/hw_reboost.su ./src/hardware/secb17510a.cyclo ./src/hardware/secb17510a.d ./src/hardware/secb17510a.o ./src/hardware/secb17510a.su ./src/hardware/secb1754a.cyclo ./src/hardware/secb1754a.d ./src/hardware/secb1754a.o ./src/hardware/secb1754a.su ./src/hardware/secb1754b.cyclo ./src/hardware/secb1754b.d ./src/hardware/secb1754b.o ./src/hardware/secb1754b.su ./src/hardware/secb1757a.cyclo ./src/hardware/secb1757a.d ./src/hardware/secb1757a.o ./src/hardware/secb1757a.su ./src/hardware/secb808b.cyclo ./src/hardware/secb808b.d ./src/hardware/secb808b.o ./src/hardware/secb808b.su

.PHONY: clean-src-2f-hardware


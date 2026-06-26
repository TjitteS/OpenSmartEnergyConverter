################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../src/startup_stm32g474ceux.s 

C_SRCS += \
../src/analog.c \
../src/buffer.c \
../src/can.c \
../src/commands.c \
../src/config.c \
../src/control.c \
../src/crc.c \
../src/delay.c \
../src/dma.c \
../src/eeprom.c \
../src/flash.c \
../src/leds.c \
../src/main.c \
../src/mppt.c \
../src/packet.c \
../src/pwm.c \
../src/syscalls.c \
../src/sysmem.c \
../src/system_stm32g4xx.c \
../src/terminal.c \
../src/testing.c \
../src/uart.c 

OBJS += \
./src/analog.o \
./src/buffer.o \
./src/can.o \
./src/commands.o \
./src/config.o \
./src/control.o \
./src/crc.o \
./src/delay.o \
./src/dma.o \
./src/eeprom.o \
./src/flash.o \
./src/leds.o \
./src/main.o \
./src/mppt.o \
./src/packet.o \
./src/pwm.o \
./src/startup_stm32g474ceux.o \
./src/syscalls.o \
./src/sysmem.o \
./src/system_stm32g4xx.o \
./src/terminal.o \
./src/testing.o \
./src/uart.o 

S_DEPS += \
./src/startup_stm32g474ceux.d 

C_DEPS += \
./src/analog.d \
./src/buffer.d \
./src/can.d \
./src/commands.d \
./src/config.d \
./src/control.d \
./src/crc.d \
./src/delay.d \
./src/dma.d \
./src/eeprom.d \
./src/flash.d \
./src/leds.d \
./src/main.d \
./src/mppt.d \
./src/packet.d \
./src/pwm.d \
./src/syscalls.d \
./src/sysmem.d \
./src/system_stm32g4xx.d \
./src/terminal.d \
./src/testing.d \
./src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o src/%.su src/%.cyclo: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -DHW_TOPOLOGY_BUCK -DHW_SECB175_7A -DUSE_HAL_DRIVER -c -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/STM32G4xx_HAL_Driver/Inc" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/CMSIS/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/CMSIS/Device/ST/STM32G4xx/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/src" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/src/hardware" -Ofast -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/%.o: ../src/%.s src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DSTM32G474xx -DHW_TOPOLOGY_BUCK -c -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/STM32G4xx_HAL_Driver/Inc" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/CMSIS/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/CMSIS/Device/ST/STM32G4xx/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/src" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/src/hardware" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-src

clean-src:
	-$(RM) ./src/analog.cyclo ./src/analog.d ./src/analog.o ./src/analog.su ./src/buffer.cyclo ./src/buffer.d ./src/buffer.o ./src/buffer.su ./src/can.cyclo ./src/can.d ./src/can.o ./src/can.su ./src/commands.cyclo ./src/commands.d ./src/commands.o ./src/commands.su ./src/config.cyclo ./src/config.d ./src/config.o ./src/config.su ./src/control.cyclo ./src/control.d ./src/control.o ./src/control.su ./src/crc.cyclo ./src/crc.d ./src/crc.o ./src/crc.su ./src/delay.cyclo ./src/delay.d ./src/delay.o ./src/delay.su ./src/dma.cyclo ./src/dma.d ./src/dma.o ./src/dma.su ./src/eeprom.cyclo ./src/eeprom.d ./src/eeprom.o ./src/eeprom.su ./src/flash.cyclo ./src/flash.d ./src/flash.o ./src/flash.su ./src/leds.cyclo ./src/leds.d ./src/leds.o ./src/leds.su ./src/main.cyclo ./src/main.d ./src/main.o ./src/main.su ./src/mppt.cyclo ./src/mppt.d ./src/mppt.o ./src/mppt.su ./src/packet.cyclo ./src/packet.d ./src/packet.o ./src/packet.su ./src/pwm.cyclo ./src/pwm.d ./src/pwm.o ./src/pwm.su ./src/startup_stm32g474ceux.d ./src/startup_stm32g474ceux.o ./src/syscalls.cyclo ./src/syscalls.d ./src/syscalls.o ./src/syscalls.su ./src/sysmem.cyclo ./src/sysmem.d ./src/sysmem.o ./src/sysmem.su ./src/system_stm32g4xx.cyclo ./src/system_stm32g4xx.d ./src/system_stm32g4xx.o ./src/system_stm32g4xx.su ./src/terminal.cyclo ./src/terminal.d ./src/terminal.o ./src/terminal.su ./src/testing.cyclo ./src/testing.d ./src/testing.o ./src/testing.su ./src/uart.cyclo ./src/uart.d ./src/uart.o ./src/uart.su

.PHONY: clean-src


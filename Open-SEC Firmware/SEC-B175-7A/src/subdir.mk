################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
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
src/%.o: ../src/%.c src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -DHW_SECB175 -DUSE_HAL_DRIVER -c -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/STM32G4xx_HAL_Driver/Inc" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/CMSIS/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/CMSIS/Device/ST/STM32G4xx/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/src" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/src/hardware" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
src/%.o: ../src/%.s src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DSTM32G474xx -DHW_SECB175 -c -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/STM32G4xx_HAL_Driver/Inc" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/CMSIS/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/CMSIS/Device/ST/STM32G4xx/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/src" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Projects/TP2102 OpenBoost/Firmware/OpenBoost/src/hardware" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-src

clean-src:
	-$(RM) ./src/analog.d ./src/analog.o ./src/buffer.d ./src/buffer.o ./src/can.d ./src/can.o ./src/commands.d ./src/commands.o ./src/config.d ./src/config.o ./src/control.d ./src/control.o ./src/crc.d ./src/crc.o ./src/delay.d ./src/delay.o ./src/dma.d ./src/dma.o ./src/eeprom.d ./src/eeprom.o ./src/flash.d ./src/flash.o ./src/leds.d ./src/leds.o ./src/main.d ./src/main.o ./src/mppt.d ./src/mppt.o ./src/packet.d ./src/packet.o ./src/pwm.d ./src/pwm.o ./src/startup_stm32g474ceux.d ./src/startup_stm32g474ceux.o ./src/syscalls.d ./src/syscalls.o ./src/sysmem.d ./src/sysmem.o ./src/system_stm32g4xx.d ./src/system_stm32g4xx.o ./src/terminal.d ./src/terminal.o ./src/testing.d ./src/testing.o ./src/uart.d ./src/uart.o

.PHONY: clean-src


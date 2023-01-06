################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/hardware/subdir.mk
-include src/subdir.mk
-include Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := OpenSEC_SEC-B175-7A_default
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
OpenSEC_SEC-B175-7A_default.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
OpenSEC_SEC-B175-7A_default.list \

OBJCOPY_BIN += \
OpenSEC_SEC-B175-7A_default.bin \


# All Target
all: main-build

# Main-build Target
main-build: OpenSEC_SEC-B175-7A_default.elf secondary-outputs

# Tool invocations
OpenSEC_SEC-B175-7A_default.elf: $(OBJS) $(USER_OBJS) C:\Users\Tjitte\ van\ der\ Ploeg\OneDrive\ -\ TPEE\Documents\TPEE\Projects\TP2102\ OpenBoost\Firmware\OpenBoost\STM32G474CEUX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "OpenSEC_SEC-B175-7A_default.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Tjitte van der Ploeg\OneDrive - TPEE\Documents\TPEE\Projects\TP2102 OpenBoost\Firmware\OpenBoost\STM32G474CEUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="OpenSEC_SEC-B175-7A_default.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

OpenSEC_SEC-B175-7A_default.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "OpenSEC_SEC-B175-7A_default.list"
	@echo 'Finished building: $@'
	@echo ' '

OpenSEC_SEC-B175-7A_default.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "OpenSEC_SEC-B175-7A_default.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) OpenSEC_SEC-B175-7A_default.bin OpenSEC_SEC-B175-7A_default.elf OpenSEC_SEC-B175-7A_default.list default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets

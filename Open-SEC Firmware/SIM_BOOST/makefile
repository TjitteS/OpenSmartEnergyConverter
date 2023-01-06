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


BUILD_ARTIFACT_NAME := OpenSEC-SIM_BOOST
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
OpenSEC-SIM_BOOST.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
OpenSEC-SIM_BOOST.list \

OBJCOPY_BIN += \
OpenSEC-SIM_BOOST.bin \


# All Target
all: main-build

# Main-build Target
main-build: OpenSEC-SIM_BOOST.elf secondary-outputs

# Tool invocations
OpenSEC-SIM_BOOST.elf: $(OBJS) $(USER_OBJS) C:\Users\Tjitte\ van\ der\ Ploeg\OneDrive\ -\ TPEE\Documents\TPEE\Projects\TP2102\ OpenBoost\Firmware\OpenBoost\STM32G474CEUX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "OpenSEC-SIM_BOOST.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\Tjitte van der Ploeg\OneDrive - TPEE\Documents\TPEE\Projects\TP2102 OpenBoost\Firmware\OpenBoost\STM32G474CEUX_FLASH.ld" --specs=nosys.specs -Wl,-Map="OpenSEC-SIM_BOOST.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

OpenSEC-SIM_BOOST.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "OpenSEC-SIM_BOOST.list"
	@echo 'Finished building: $@'
	@echo ' '

OpenSEC-SIM_BOOST.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "OpenSEC-SIM_BOOST.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) OpenSEC-SIM_BOOST.bin OpenSEC-SIM_BOOST.elf OpenSEC-SIM_BOOST.list default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets

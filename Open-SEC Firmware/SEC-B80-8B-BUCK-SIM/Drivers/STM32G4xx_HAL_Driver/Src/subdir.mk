################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_comp.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cordic.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fmac.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_hrtim.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_irda.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_lptim.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nand.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nor.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_qspi.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rng.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sram.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart_ex.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_comp.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_cordic.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crs.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dac.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dma.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_exti.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmac.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_gpio.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_hrtim.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_i2c.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lptim.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lpuart.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_opamp.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rcc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rng.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rtc.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_spi.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_tim.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_ucpd.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usart.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.c \
../Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_utils.c 

OBJS += \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_comp.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cordic.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fmac.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_hrtim.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_irda.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_lptim.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nand.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nor.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_qspi.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rng.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sram.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart_ex.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_comp.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_cordic.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crs.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dac.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dma.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_exti.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmac.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_gpio.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_hrtim.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_i2c.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lptim.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lpuart.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_opamp.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rcc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rng.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rtc.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_spi.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_tim.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_ucpd.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usart.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.o \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_utils.o 

C_DEPS += \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_comp.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cordic.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fmac.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_hrtim.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_irda.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_lptim.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nand.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nor.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_qspi.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rng.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sram.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart_ex.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_comp.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_cordic.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crs.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dac.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dma.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_exti.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmac.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_gpio.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_hrtim.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_i2c.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lptim.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lpuart.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_opamp.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rcc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rng.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rtc.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_spi.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_tim.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_ucpd.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usart.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.d \
./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32G4xx_HAL_Driver/Src/%.o Drivers/STM32G4xx_HAL_Driver/Src/%.su Drivers/STM32G4xx_HAL_Driver/Src/%.cyclo: ../Drivers/STM32G4xx_HAL_Driver/Src/%.c Drivers/STM32G4xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G474xx -DSIMULATION -DHW_TOPOLOGY_BUCK -DHW_SECB808B -DUSE_HAL_DRIVER -UHW_TOPOLOGY_BOOST -c -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/STM32G4xx_HAL_Driver/Inc" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/CMSIS/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/CMSIS/Device/ST/STM32G4xx/Include" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/Drivers/STM32G4xx_HAL_Driver/Inc/Legacy" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/src" -I"C:/Users/Tjitte van der Ploeg/OneDrive - TPEE/Documents/TPEE/Tools/OpenSmartEnergyConverter/Open-SEC Firmware/src/hardware" -Ofast -ffunction-sections -fdata-sections -Wall -fcommon -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32G4xx_HAL_Driver-2f-Src

clean-Drivers-2f-STM32G4xx_HAL_Driver-2f-Src:
	-$(RM) ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_adc_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_comp.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_comp.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_comp.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_comp.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cordic.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cordic.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cordic.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cordic.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cortex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_crc_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_cryp_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dac_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_dma_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_exti.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fdcan.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_flash_ramfunc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fmac.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fmac.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fmac.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_fmac.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_gpio.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_hrtim.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_hrtim.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_hrtim.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_hrtim.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2c_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_i2s.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_irda.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_irda.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_irda.o
	-$(RM) ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_irda.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_iwdg.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_lptim.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_lptim.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_lptim.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_lptim.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nand.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nand.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nand.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nand.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nor.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nor.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nor.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_nor.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_opamp_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pcd_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_pwr_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_qspi.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_qspi.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_qspi.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_qspi.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rcc_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rng.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rng.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rng.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rng.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_rtc_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sai_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smartcard_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_smbus_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_spi_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sram.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sram.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sram.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_sram.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.cyclo
	-$(RM) ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_tim_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_uart_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart_ex.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart_ex.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart_ex.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_usart_ex.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_hal_wwdg.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_adc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_comp.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_comp.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_comp.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_comp.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_cordic.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_cordic.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_cordic.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_cordic.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crc.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crs.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crs.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crs.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_crs.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dac.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dac.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dac.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dac.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dma.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dma.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dma.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_dma.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_exti.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_exti.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_exti.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_exti.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmac.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmac.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmac.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmac.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmc.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_fmc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_gpio.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_gpio.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_gpio.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_gpio.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_hrtim.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_hrtim.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_hrtim.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_hrtim.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_i2c.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_i2c.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_i2c.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_i2c.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lptim.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lptim.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lptim.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lptim.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lpuart.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lpuart.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lpuart.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_lpuart.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_opamp.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_opamp.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_opamp.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_opamp.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_pwr.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rcc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rcc.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rcc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rcc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rng.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rng.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rng.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rng.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rtc.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rtc.d
	-$(RM) ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rtc.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_rtc.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_spi.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_spi.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_spi.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_spi.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_tim.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_tim.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_tim.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_tim.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_ucpd.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_ucpd.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_ucpd.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_ucpd.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usart.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usart.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usart.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usart.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_usb.su ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_utils.cyclo ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_utils.d ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_utils.o ./Drivers/STM32G4xx_HAL_Driver/Src/stm32g4xx_ll_utils.su

.PHONY: clean-Drivers-2f-STM32G4xx_HAL_Driver-2f-Src


################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/gpio.c \
../Src/main.c \
../Src/spi.c \
../Src/stm32f1xx_hal_msp.c \
../Src/stm32f1xx_it.c \
../Src/syscalls.c \
../Src/system_stm32f1xx.c \
../Src/tim.c \
../Src/userMain.c 

OBJS += \
./Src/gpio.o \
./Src/main.o \
./Src/spi.o \
./Src/stm32f1xx_hal_msp.o \
./Src/stm32f1xx_it.o \
./Src/syscalls.o \
./Src/system_stm32f1xx.o \
./Src/tim.o \
./Src/userMain.o 

C_DEPS += \
./Src/gpio.d \
./Src/main.d \
./Src/spi.d \
./Src/stm32f1xx_hal_msp.d \
./Src/stm32f1xx_it.d \
./Src/syscalls.d \
./Src/system_stm32f1xx.d \
./Src/tim.d \
./Src/userMain.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o: ../Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DUSE_HAL_DRIVER -DSTM32F103xB -I"A:/STM32Projects/13_LCDHygroTemp/Inc" -I"A:/STM32Projects/13_LCDHygroTemp/Drivers/STM32F1xx_HAL_Driver/Inc" -I"A:/STM32Projects/13_LCDHygroTemp/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"A:/STM32Projects/13_LCDHygroTemp/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"A:/STM32Projects/13_LCDHygroTemp/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



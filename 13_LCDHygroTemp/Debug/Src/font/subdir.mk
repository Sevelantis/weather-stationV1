################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/font/font.c 

OBJS += \
./Src/font/font.o 

C_DEPS += \
./Src/font/font.d 


# Each subdirectory must supply rules for building sources it contributes
Src/font/%.o: ../Src/font/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -DUSE_HAL_DRIVER -DSTM32F103xB -I"A:/STM32Projects/13_LCDHygroTemp/Inc" -I"A:/STM32Projects/13_LCDHygroTemp/Drivers/STM32F1xx_HAL_Driver/Inc" -I"A:/STM32Projects/13_LCDHygroTemp/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"A:/STM32Projects/13_LCDHygroTemp/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"A:/STM32Projects/13_LCDHygroTemp/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



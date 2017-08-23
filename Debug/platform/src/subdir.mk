################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/src/stm32f303xc_it.c \
../platform/src/system_stm32f303cx.c 

OBJS += \
./platform/src/stm32f303xc_it.o \
./platform/src/system_stm32f303cx.o 

C_DEPS += \
./platform/src/stm32f303xc_it.d \
./platform/src/system_stm32f303cx.d 


# Each subdirectory must supply rules for building sources it contributes
platform/src/%.o: ../platform/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DSTM32F303xC -DHSE_VALUE=8000000 -I"/Users/technix/Developer/Eclipse/default/LCDClock/include" -I"/Users/technix/Developer/Eclipse/default/LCDClock/platform/include" -I"/Users/technix/Developer/Eclipse/default/LCDClock/cmsis/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



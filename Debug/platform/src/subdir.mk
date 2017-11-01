################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../platform/src/stm32f1xx_it.c \
../platform/src/system_stm32f1xx.c 

OBJS += \
./platform/src/stm32f1xx_it.o \
./platform/src/system_stm32f1xx.o 

C_DEPS += \
./platform/src/stm32f1xx_it.d \
./platform/src/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
platform/src/%.o: ../platform/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fsingle-precision-constant  -g3 -DUSB_PMASIZE=512 -DSTM32F103xB -DHSE_VALUE=8000000 -I"/Users/technix/Developer/Eclipse/default/LCDClock/include" -I"/Users/technix/Developer/Eclipse/default/LCDClock/contrib/libusb_stm32/include" -I"/Users/technix/Developer/Eclipse/default/LCDClock/system/include" -I"/Users/technix/Developer/Eclipse/default/LCDClock/platform/include" -I"/Users/technix/Developer/Eclipse/default/LCDClock/cmsis/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



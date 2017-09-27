################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/board.c \
../src/lcd.c \
../src/main.c \
../src/pwm.c 

OBJS += \
./src/board.o \
./src/lcd.o \
./src/main.o \
./src/pwm.o 

C_DEPS += \
./src/board.d \
./src/lcd.d \
./src/main.d \
./src/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fsingle-precision-constant  -g3 -DSTM32F303xC -DHSE_VALUE=8000000 -I"/Users/technix/Developer/Eclipse/default/LCDClock/include" -I"/Users/technix/Developer/Eclipse/default/LCDClock/system/include" -I"/Users/technix/Developer/Eclipse/default/LCDClock/platform/include" -I"/Users/technix/Developer/Eclipse/default/LCDClock/cmsis/include" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



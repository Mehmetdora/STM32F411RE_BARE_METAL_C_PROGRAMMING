################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Modules/ADCDriver.c \
../Drivers/Modules/GPIODriver.c \
../Drivers/Modules/PWMDriver.c 

OBJS += \
./Drivers/Modules/ADCDriver.o \
./Drivers/Modules/GPIODriver.o \
./Drivers/Modules/PWMDriver.o 

C_DEPS += \
./Drivers/Modules/ADCDriver.d \
./Drivers/Modules/GPIODriver.d \
./Drivers/Modules/PWMDriver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Modules/%.o Drivers/Modules/%.su Drivers/Modules/%.cyclo: ../Drivers/Modules/%.c Drivers/Modules/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/Modules -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Modules

clean-Drivers-2f-Modules:
	-$(RM) ./Drivers/Modules/ADCDriver.cyclo ./Drivers/Modules/ADCDriver.d ./Drivers/Modules/ADCDriver.o ./Drivers/Modules/ADCDriver.su ./Drivers/Modules/GPIODriver.cyclo ./Drivers/Modules/GPIODriver.d ./Drivers/Modules/GPIODriver.o ./Drivers/Modules/GPIODriver.su ./Drivers/Modules/PWMDriver.cyclo ./Drivers/Modules/PWMDriver.d ./Drivers/Modules/PWMDriver.o ./Drivers/Modules/PWMDriver.su

.PHONY: clean-Drivers-2f-Modules


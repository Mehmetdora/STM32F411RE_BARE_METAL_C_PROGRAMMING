################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Modules/ADCDriver.c \
../Drivers/Modules/HELPER.c \
../Drivers/Modules/UARTDriver.c 

OBJS += \
./Drivers/Modules/ADCDriver.o \
./Drivers/Modules/HELPER.o \
./Drivers/Modules/UARTDriver.o 

C_DEPS += \
./Drivers/Modules/ADCDriver.d \
./Drivers/Modules/HELPER.d \
./Drivers/Modules/UARTDriver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Modules/%.o Drivers/Modules/%.su Drivers/Modules/%.cyclo: ../Drivers/Modules/%.c Drivers/Modules/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Drivers/Modules -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Modules

clean-Drivers-2f-Modules:
	-$(RM) ./Drivers/Modules/ADCDriver.cyclo ./Drivers/Modules/ADCDriver.d ./Drivers/Modules/ADCDriver.o ./Drivers/Modules/ADCDriver.su ./Drivers/Modules/HELPER.cyclo ./Drivers/Modules/HELPER.d ./Drivers/Modules/HELPER.o ./Drivers/Modules/HELPER.su ./Drivers/Modules/UARTDriver.cyclo ./Drivers/Modules/UARTDriver.d ./Drivers/Modules/UARTDriver.o ./Drivers/Modules/UARTDriver.su

.PHONY: clean-Drivers-2f-Modules


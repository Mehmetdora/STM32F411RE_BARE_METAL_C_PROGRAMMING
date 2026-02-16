################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Modules/LedDriver.c \
../Drivers/Modules/SysTickDriver.c 

OBJS += \
./Drivers/Modules/LedDriver.o \
./Drivers/Modules/SysTickDriver.o 

C_DEPS += \
./Drivers/Modules/LedDriver.d \
./Drivers/Modules/SysTickDriver.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Modules/%.o Drivers/Modules/%.su Drivers/Modules/%.cyclo: ../Drivers/Modules/%.c Drivers/Modules/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/Modules -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Modules

clean-Drivers-2f-Modules:
	-$(RM) ./Drivers/Modules/LedDriver.cyclo ./Drivers/Modules/LedDriver.d ./Drivers/Modules/LedDriver.o ./Drivers/Modules/LedDriver.su ./Drivers/Modules/SysTickDriver.cyclo ./Drivers/Modules/SysTickDriver.d ./Drivers/Modules/SysTickDriver.o ./Drivers/Modules/SysTickDriver.su

.PHONY: clean-Drivers-2f-Modules


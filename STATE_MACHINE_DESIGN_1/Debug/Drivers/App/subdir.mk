################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/App/LED_APP.c 

OBJS += \
./Drivers/App/LED_APP.o 

C_DEPS += \
./Drivers/App/LED_APP.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/App/%.o Drivers/App/%.su Drivers/App/%.cyclo: ../Drivers/App/%.c Drivers/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/App -I../Drivers/Modules -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-App

clean-Drivers-2f-App:
	-$(RM) ./Drivers/App/LED_APP.cyclo ./Drivers/App/LED_APP.d ./Drivers/App/LED_APP.o ./Drivers/App/LED_APP.su

.PHONY: clean-Drivers-2f-App


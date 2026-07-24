################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SSD1306_Driver/SSD1306_CUSTOM_Driver.c \
../Drivers/SSD1306_Driver/ssd1306.c \
../Drivers/SSD1306_Driver/ssd1306_fonts.c \
../Drivers/SSD1306_Driver/ssd1306_tests.c 

OBJS += \
./Drivers/SSD1306_Driver/SSD1306_CUSTOM_Driver.o \
./Drivers/SSD1306_Driver/ssd1306.o \
./Drivers/SSD1306_Driver/ssd1306_fonts.o \
./Drivers/SSD1306_Driver/ssd1306_tests.o 

C_DEPS += \
./Drivers/SSD1306_Driver/SSD1306_CUSTOM_Driver.d \
./Drivers/SSD1306_Driver/ssd1306.d \
./Drivers/SSD1306_Driver/ssd1306_fonts.d \
./Drivers/SSD1306_Driver/ssd1306_tests.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SSD1306_Driver/%.o Drivers/SSD1306_Driver/%.su Drivers/SSD1306_Driver/%.cyclo: ../Drivers/SSD1306_Driver/%.c Drivers/SSD1306_Driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"/Users/mehmet_dora/Desktop/STM32/SSD1306_OLED_DRIVER_1/Drivers/SSD1306_Driver" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SSD1306_Driver

clean-Drivers-2f-SSD1306_Driver:
	-$(RM) ./Drivers/SSD1306_Driver/SSD1306_CUSTOM_Driver.cyclo ./Drivers/SSD1306_Driver/SSD1306_CUSTOM_Driver.d ./Drivers/SSD1306_Driver/SSD1306_CUSTOM_Driver.o ./Drivers/SSD1306_Driver/SSD1306_CUSTOM_Driver.su ./Drivers/SSD1306_Driver/ssd1306.cyclo ./Drivers/SSD1306_Driver/ssd1306.d ./Drivers/SSD1306_Driver/ssd1306.o ./Drivers/SSD1306_Driver/ssd1306.su ./Drivers/SSD1306_Driver/ssd1306_fonts.cyclo ./Drivers/SSD1306_Driver/ssd1306_fonts.d ./Drivers/SSD1306_Driver/ssd1306_fonts.o ./Drivers/SSD1306_Driver/ssd1306_fonts.su ./Drivers/SSD1306_Driver/ssd1306_tests.cyclo ./Drivers/SSD1306_Driver/ssd1306_tests.d ./Drivers/SSD1306_Driver/ssd1306_tests.o ./Drivers/SSD1306_Driver/ssd1306_tests.su

.PHONY: clean-Drivers-2f-SSD1306_Driver


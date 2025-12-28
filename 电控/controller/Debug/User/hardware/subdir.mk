################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/hardware/NRF24L01.c \
../User/hardware/axis.c \
../User/hardware/button.c \
../User/hardware/font.c \
../User/hardware/oled.c 

OBJS += \
./User/hardware/NRF24L01.o \
./User/hardware/axis.o \
./User/hardware/button.o \
./User/hardware/font.o \
./User/hardware/oled.o 

C_DEPS += \
./User/hardware/NRF24L01.d \
./User/hardware/axis.d \
./User/hardware/button.d \
./User/hardware/font.d \
./User/hardware/oled.d 


# Each subdirectory must supply rules for building sources it contributes
User/hardware/%.o User/hardware/%.su User/hardware/%.cyclo: ../User/hardware/%.c User/hardware/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/a/Desktop/.cqu/bot/code/controller/User/hardware" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-hardware

clean-User-2f-hardware:
	-$(RM) ./User/hardware/NRF24L01.cyclo ./User/hardware/NRF24L01.d ./User/hardware/NRF24L01.o ./User/hardware/NRF24L01.su ./User/hardware/axis.cyclo ./User/hardware/axis.d ./User/hardware/axis.o ./User/hardware/axis.su ./User/hardware/button.cyclo ./User/hardware/button.d ./User/hardware/button.o ./User/hardware/button.su ./User/hardware/font.cyclo ./User/hardware/font.d ./User/hardware/font.o ./User/hardware/font.su ./User/hardware/oled.cyclo ./User/hardware/oled.d ./User/hardware/oled.o ./User/hardware/oled.su

.PHONY: clean-User-2f-hardware


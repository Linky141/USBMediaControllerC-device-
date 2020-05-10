################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SRC/SOURCE/DEV_COMMAND.c \
../SRC/SOURCE/LCD_EXPLOITING.c \
../SRC/SOURCE/LEDcontrol.c \
../SRC/SOURCE/PWM.c \
../SRC/SOURCE/SettingsMenu.c \
../SRC/SOURCE/UART_COMMUNICATION.c \
../SRC/SOURCE/main.c 

OBJS += \
./SRC/SOURCE/DEV_COMMAND.o \
./SRC/SOURCE/LCD_EXPLOITING.o \
./SRC/SOURCE/LEDcontrol.o \
./SRC/SOURCE/PWM.o \
./SRC/SOURCE/SettingsMenu.o \
./SRC/SOURCE/UART_COMMUNICATION.o \
./SRC/SOURCE/main.o 

C_DEPS += \
./SRC/SOURCE/DEV_COMMAND.d \
./SRC/SOURCE/LCD_EXPLOITING.d \
./SRC/SOURCE/LEDcontrol.d \
./SRC/SOURCE/PWM.d \
./SRC/SOURCE/SettingsMenu.d \
./SRC/SOURCE/UART_COMMUNICATION.d \
./SRC/SOURCE/main.d 


# Each subdirectory must supply rules for building sources it contributes
SRC/SOURCE/%.o: ../SRC/SOURCE/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega32 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



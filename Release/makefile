################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include SRC/SOURCE/subdir.mk
-include Libraries/Source/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
USB_MEDIA_CONTROLLER.lss \

FLASH_IMAGE += \
USB_MEDIA_CONTROLLER.hex \

EEPROM_IMAGE += \
USB_MEDIA_CONTROLLER.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: USB_MEDIA_CONTROLLER.elf secondary-outputs

# Tool invocations
USB_MEDIA_CONTROLLER.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,USB_MEDIA_CONTROLLER.map -mmcu=atmega32 -o "USB_MEDIA_CONTROLLER.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

USB_MEDIA_CONTROLLER.lss: USB_MEDIA_CONTROLLER.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S USB_MEDIA_CONTROLLER.elf  >"USB_MEDIA_CONTROLLER.lss"
	@echo 'Finished building: $@'
	@echo ' '

USB_MEDIA_CONTROLLER.hex: USB_MEDIA_CONTROLLER.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex USB_MEDIA_CONTROLLER.elf  "USB_MEDIA_CONTROLLER.hex"
	@echo 'Finished building: $@'
	@echo ' '

USB_MEDIA_CONTROLLER.eep: USB_MEDIA_CONTROLLER.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex USB_MEDIA_CONTROLLER.elf  "USB_MEDIA_CONTROLLER.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: USB_MEDIA_CONTROLLER.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 USB_MEDIA_CONTROLLER.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) USB_MEDIA_CONTROLLER.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents

-include ../makefile.targets

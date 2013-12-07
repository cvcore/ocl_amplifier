################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
OCL_Amplifier.lss \

FLASH_IMAGE += \
OCL_Amplifier.hex \

EEPROM_IMAGE += \
OCL_Amplifier.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: OCL_Amplifier.elf secondary-outputs

# Tool invocations
OCL_Amplifier.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,OCL_Amplifier.map -mmcu=atmega8 -o "OCL_Amplifier.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

OCL_Amplifier.lss: OCL_Amplifier.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S OCL_Amplifier.elf  >"OCL_Amplifier.lss"
	@echo 'Finished building: $@'
	@echo ' '

OCL_Amplifier.hex: OCL_Amplifier.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex OCL_Amplifier.elf  "OCL_Amplifier.hex"
	@echo 'Finished building: $@'
	@echo ' '

OCL_Amplifier.eep: OCL_Amplifier.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex OCL_Amplifier.elf  "OCL_Amplifier.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: OCL_Amplifier.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega8 OCL_Amplifier.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(EEPROM_IMAGE)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) OCL_Amplifier.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

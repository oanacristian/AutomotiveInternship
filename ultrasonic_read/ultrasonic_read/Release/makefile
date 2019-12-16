################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 7
first : all
-include ../makefile.init

# This file contains definitions of environment variables used in the makefiles and .args files if exist.
-include makefile.local

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include Project_Settings/Linker_Files/subdir.mk
-include Project_Settings/Startup_Code/subdir.mk
-include include/subdir.mk
-include src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
button.elf \

EXECUTABLES_QUOTED += \
"button.elf" \

EXECUTABLES_OS_FORMAT += \
button.elf \

SECONDARY_SIZE += \
button.siz \
button.siz \

SECONDARY_SIZE_QUOTED += \
"button.siz" \
"button.siz" \

SECONDARY_SIZE_OS_FORMAT += \
button.siz \
button.siz \

SECONDARY_SIZE += \
button.siz \
button.siz \

SECONDARY_SIZE_QUOTED += \
"button.siz" \
"button.siz" \

SECONDARY_SIZE_OS_FORMAT += \
button.siz \
button.siz \


# All Target
all: warning-messages explicit-dependencies button.elf secondary-outputs

# Tool invocations
button.elf: $(OBJS) C:/Users/egon.osvath/workspaceS32DS.ARM.2018.R1/ultrasonic_read/ultrasonic_read/Project_Settings/Linker_Files/S32K1xx_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #8 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "button.elf" "@button.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

button.siz: $(EXECUTABLES)
	@echo 'Executing target #9 $@'
	@echo 'Invoking: Standard S32DS Print Size'
	arm-none-eabi-size --format=berkeley $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*.d ./*/*/*.d  ./*/*.o ./*/*/*.o  $(SECONDARY_SIZE_QUOTED) $(EXECUTABLES_QUOTED)
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE) $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 


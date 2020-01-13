################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 11
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
cruise_control_tasks.elf \

EXECUTABLES_QUOTED += \
"cruise_control_tasks.elf" \

EXECUTABLES_OS_FORMAT += \
cruise_control_tasks.elf \

SECONDARY_SIZE += \
cruise_control_tasks.siz \
cruise_control_tasks.siz \

SECONDARY_SIZE_QUOTED += \
"cruise_control_tasks.siz" \
"cruise_control_tasks.siz" \

SECONDARY_SIZE_OS_FORMAT += \
cruise_control_tasks.siz \
cruise_control_tasks.siz \

SECONDARY_SIZE += \
cruise_control_tasks.siz \
cruise_control_tasks.siz \

SECONDARY_SIZE_QUOTED += \
"cruise_control_tasks.siz" \
"cruise_control_tasks.siz" \

SECONDARY_SIZE_OS_FORMAT += \
cruise_control_tasks.siz \
cruise_control_tasks.siz \


# All Target
all: warning-messages explicit-dependencies cruise_control_tasks.elf secondary-outputs

# Tool invocations
cruise_control_tasks.elf: $(OBJS) C:/Users/egon.osvath/Desktop/AutomotiveInternship/master/cruise_control_sensor_read/Project_Settings/Linker_Files/S32K1xx_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #12 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "cruise_control_tasks.elf" "@cruise_control_tasks.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

cruise_control_tasks.siz: $(EXECUTABLES)
	@echo 'Executing target #13 $@'
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


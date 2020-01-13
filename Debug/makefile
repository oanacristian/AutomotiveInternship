################################################################################
# Automatically-generated file. Do not edit!
################################################################################

#Progress monitor hint: 5
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
hello_clocks.elf \

EXECUTABLES_QUOTED += \
"hello_clocks.elf" \

EXECUTABLES_OS_FORMAT += \
hello_clocks.elf \

SECONDARY_SIZE += \
hello_clocks.siz \

SECONDARY_SIZE_QUOTED += \
"hello_clocks.siz" \

SECONDARY_SIZE_OS_FORMAT += \
hello_clocks.siz \


# All Target
all: warning-messages explicit-dependencies hello_clocks.elf secondary-outputs

# Tool invocations
hello_clocks.elf: $(OBJS) C:/SW/S32K14x/S32DSv2.0/hello_clocks/Project_Settings/Linker_Files/S32K1xx_flash.ld $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Executing target #6 $@'
	@echo 'Invoking: Standard S32DS C Linker'
	arm-none-eabi-gcc -o "hello_clocks.elf" "@hello_clocks.args"  $(USER_OBJS_QUOTED) $(LIBS_QUOTED)
	@echo 'Finished building target: $@'
	@echo ' '

hello_clocks.siz: $(EXECUTABLES)
	@echo 'Executing target #7 $@'
	@echo 'Invoking: Standard S32DS Print Size'
	arm-none-eabi-size --format=berkeley $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) ./*/*.d ./*/*/*.d  ./*/*.o ./*/*/*.o  $(SECONDARY_SIZE_QUOTED) $(EXECUTABLES_QUOTED)
	-@echo ' '

secondary-outputs: $(SECONDARY_SIZE)

.PHONY: all clean dependents explicit-dependencies warning-messages
.SECONDARY:

-include ../makefile.targets

explicit-dependencies: 

warning-messages: 


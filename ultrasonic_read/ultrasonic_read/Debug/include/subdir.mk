################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../include/clocks_and_modes.c" \

C_SRCS += \
../include/clocks_and_modes.c \

OBJS_OS_FORMAT += \
./include/clocks_and_modes.o \

C_DEPS_QUOTED += \
"./include/clocks_and_modes.d" \

OBJS += \
./include/clocks_and_modes.o \

OBJS_QUOTED += \
"./include/clocks_and_modes.o" \

C_DEPS += \
./include/clocks_and_modes.d \


# Each subdirectory must supply rules for building sources it contributes
include/clocks_and_modes.o: ../include/clocks_and_modes.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/clocks_and_modes.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/clocks_and_modes.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



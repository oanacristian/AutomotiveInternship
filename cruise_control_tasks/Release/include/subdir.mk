################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../include/FTM.c" \
"../include/HC_SR04.c" \
"../include/LPUART.c" \
"../include/clocks_and_modes.c" \
"../include/pwm.c" \
"../include/tasks.c" \

C_SRCS += \
../include/FTM.c \
../include/HC_SR04.c \
../include/LPUART.c \
../include/clocks_and_modes.c \
../include/pwm.c \
../include/tasks.c \

OBJS_OS_FORMAT += \
./include/FTM.o \
./include/HC_SR04.o \
./include/LPUART.o \
./include/clocks_and_modes.o \
./include/pwm.o \
./include/tasks.o \

C_DEPS_QUOTED += \
"./include/FTM.d" \
"./include/HC_SR04.d" \
"./include/LPUART.d" \
"./include/clocks_and_modes.d" \
"./include/pwm.d" \
"./include/tasks.d" \

OBJS += \
./include/FTM.o \
./include/HC_SR04.o \
./include/LPUART.o \
./include/clocks_and_modes.o \
./include/pwm.o \
./include/tasks.o \

OBJS_QUOTED += \
"./include/FTM.o" \
"./include/HC_SR04.o" \
"./include/LPUART.o" \
"./include/clocks_and_modes.o" \
"./include/pwm.o" \
"./include/tasks.o" \

C_DEPS += \
./include/FTM.d \
./include/HC_SR04.d \
./include/LPUART.d \
./include/clocks_and_modes.d \
./include/pwm.d \
./include/tasks.d \


# Each subdirectory must supply rules for building sources it contributes
include/FTM.o: ../include/FTM.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/FTM.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/FTM.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/HC_SR04.o: ../include/HC_SR04.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/HC_SR04.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/HC_SR04.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/LPUART.o: ../include/LPUART.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LPUART.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/LPUART.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/clocks_and_modes.o: ../include/clocks_and_modes.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/clocks_and_modes.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/clocks_and_modes.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/pwm.o: ../include/pwm.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/pwm.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/pwm.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/tasks.o: ../include/tasks.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/tasks.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/tasks.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



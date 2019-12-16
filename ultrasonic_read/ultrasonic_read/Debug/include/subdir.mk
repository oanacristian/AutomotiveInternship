################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../include/LPSPI.c" \
"../include/LPUART.c" \
"../include/clocks_and_modes.c" \

C_SRCS += \
../include/LPSPI.c \
../include/LPUART.c \
../include/clocks_and_modes.c \

OBJS_OS_FORMAT += \
./include/LPSPI.o \
./include/LPUART.o \
./include/clocks_and_modes.o \

C_DEPS_QUOTED += \
"./include/LPSPI.d" \
"./include/LPUART.d" \
"./include/clocks_and_modes.d" \

OBJS += \
./include/LPSPI.o \
./include/LPUART.o \
./include/clocks_and_modes.o \

OBJS_QUOTED += \
"./include/LPSPI.o" \
"./include/LPUART.o" \
"./include/clocks_and_modes.o" \

C_DEPS += \
./include/LPSPI.d \
./include/LPUART.d \
./include/clocks_and_modes.d \


# Each subdirectory must supply rules for building sources it contributes
include/LPSPI.o: ../include/LPSPI.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LPSPI.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/LPSPI.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/LPUART.o: ../include/LPUART.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/LPUART.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/LPUART.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

include/clocks_and_modes.o: ../include/clocks_and_modes.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@include/clocks_and_modes.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "include/clocks_and_modes.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



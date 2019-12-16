################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/ultrasonic_read.c" \

C_SRCS += \
../src/ultrasonic_read.c \

OBJS_OS_FORMAT += \
./src/ultrasonic_read.o \

C_DEPS_QUOTED += \
"./src/ultrasonic_read.d" \

OBJS += \
./src/ultrasonic_read.o \

OBJS_QUOTED += \
"./src/ultrasonic_read.o" \

C_DEPS += \
./src/ultrasonic_read.d \


# Each subdirectory must supply rules for building sources it contributes
src/ultrasonic_read.o: ../src/ultrasonic_read.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/ultrasonic_read.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "src/ultrasonic_read.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



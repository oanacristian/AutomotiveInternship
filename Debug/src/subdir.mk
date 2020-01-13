################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../src/clocks_and_modes.c" \
"../src/hello_clocks.c" \

C_SRCS += \
../src/clocks_and_modes.c \
../src/hello_clocks.c \

OBJS_OS_FORMAT += \
./src/clocks_and_modes.o \
./src/hello_clocks.o \

C_DEPS_QUOTED += \
"./src/clocks_and_modes.d" \
"./src/hello_clocks.d" \

OBJS += \
./src/clocks_and_modes.o \
./src/hello_clocks.o \

OBJS_QUOTED += \
"./src/clocks_and_modes.o" \
"./src/hello_clocks.o" \

C_DEPS += \
./src/clocks_and_modes.d \
./src/hello_clocks.d \


# Each subdirectory must supply rules for building sources it contributes
src/clocks_and_modes.o: ../src/clocks_and_modes.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/clocks_and_modes.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/clocks_and_modes.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/hello_clocks.o: ../src/hello_clocks.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: Standard S32DS C Compiler'
	arm-none-eabi-gcc "@src/hello_clocks.args" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "src/hello_clocks.o" "$<"
	@echo 'Finished building: $<'
	@echo ' '



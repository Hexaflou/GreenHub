################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Component.c \
../ComponentInterface.c \
../Test.c \
../Utility.c \
../main.c 

OBJS += \
./Component.o \
./ComponentInterface.o \
./Test.o \
./Utility.o \
./main.o 

C_DEPS += \
./Component.d \
./ComponentInterface.d \
./Test.d \
./Utility.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



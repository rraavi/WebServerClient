################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/TCP_IP_Client/Client.c 

OBJS += \
./src/TCP_IP_Client/Client.o 

C_DEPS += \
./src/TCP_IP_Client/Client.d 


# Each subdirectory must supply rules for building sources it contributes
src/TCP_IP_Client/%.o: ../src/TCP_IP_Client/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



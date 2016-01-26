################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Client.c \
../src/Web_Server.c \
../src/udp_client.c \
../src/udp_server.c \
../src/udpserver.c 

OBJS += \
./src/Client.o \
./src/Web_Server.o \
./src/udp_client.o \
./src/udp_server.o \
./src/udpserver.o 

C_DEPS += \
./src/Client.d \
./src/Web_Server.d \
./src/udp_client.d \
./src/udp_server.d \
./src/udpserver.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



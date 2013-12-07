################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../backup-Nov29-2157/AD5291.c \
../backup-Nov29-2157/AD5291_Parallel.c \
../backup-Nov29-2157/AD637.c \
../backup-Nov29-2157/LCD1602.c \
../backup-Nov29-2157/keyboard.c \
../backup-Nov29-2157/main.c \
../backup-Nov29-2157/myutil.c 

OBJS += \
./backup-Nov29-2157/AD5291.o \
./backup-Nov29-2157/AD5291_Parallel.o \
./backup-Nov29-2157/AD637.o \
./backup-Nov29-2157/LCD1602.o \
./backup-Nov29-2157/keyboard.o \
./backup-Nov29-2157/main.o \
./backup-Nov29-2157/myutil.o 

C_DEPS += \
./backup-Nov29-2157/AD5291.d \
./backup-Nov29-2157/AD5291_Parallel.d \
./backup-Nov29-2157/AD637.d \
./backup-Nov29-2157/LCD1602.d \
./backup-Nov29-2157/keyboard.d \
./backup-Nov29-2157/main.d \
./backup-Nov29-2157/myutil.d 


# Each subdirectory must supply rules for building sources it contributes
backup-Nov29-2157/%.o: ../backup-Nov29-2157/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -O1 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega8 -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



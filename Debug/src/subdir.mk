################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/SleepyZzz.c \
../src/adc.c \
../src/cr_startup_lpc8xx.c \
../src/crp.c \
../src/helper.c \
../src/i2c.c \
../src/lcd.c \
../src/mtb.c \
../src/print.c \
../src/switch_matrix.c \
../src/sysinit.c \
../src/uart.c \
../src/wifi.c 

OBJS += \
./src/SleepyZzz.o \
./src/adc.o \
./src/cr_startup_lpc8xx.o \
./src/crp.o \
./src/helper.o \
./src/i2c.o \
./src/lcd.o \
./src/mtb.o \
./src/print.o \
./src/switch_matrix.o \
./src/sysinit.o \
./src/uart.o \
./src/wifi.o 

C_DEPS += \
./src/SleepyZzz.d \
./src/adc.d \
./src/cr_startup_lpc8xx.d \
./src/crp.d \
./src/helper.d \
./src/i2c.d \
./src/lcd.d \
./src/mtb.d \
./src/print.d \
./src/switch_matrix.d \
./src/sysinit.d \
./src/uart.d \
./src/wifi.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M0PLUS -D__MTB_BUFFER_SIZE=256 -D__USE_LPCOPEN -D__LPC8XX__ -D__REDLIB__ -I"/home/donald/Documents/MCUXpresso_10.0.0_344/workspace/SleepyZzz/inc" -I"/home/donald/Documents/MCUXpresso_10.0.0_344/workspace/lpc_board_lpc812max/inc" -I"/home/donald/Documents/MCUXpresso_10.0.0_344/workspace/lpc_chip_8xx/inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '



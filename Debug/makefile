################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: SleepyZzz.axf

# Tool invocations
SleepyZzz.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"/Users/adamseeto/Desktop/stuff/school/FYDP/code/lpc_board_lpc812max/Debug" -L"/Users/adamseeto/Desktop/stuff/school/FYDP/code/lpc_chip_8xx/Debug" -Xlinker -Map="SleepyZzz.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m0 -mthumb -T "SleepyZzz_Debug.ld" -o "SleepyZzz.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) SleepyZzz.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "SleepyZzz.axf"; # arm-none-eabi-objcopy -v -O binary "SleepyZzz.axf" "SleepyZzz.bin" ; # checksum -p LPC812 -d "SleepyZzz.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

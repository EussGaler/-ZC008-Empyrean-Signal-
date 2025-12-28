################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/unniversity/EIDC/ZC008/ZC008_v0.6.0" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.6.0/Debug" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.6.0/BSP/uart" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.6.0/BSP/delay" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.6.0/BSP" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.6.0/User" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/DSP/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -gdwarf-3 -MMD -MP -MF"User/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-523885807: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.22.0/sysconfig_cli.bat" --script "D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/empty.syscfg" -o "." -s "C:/ti/mspm0_sdk_2_03_00_07/.metadata/product.json" -d "MSPM0G350X" -p "LQFP-48(PT)" -r "Default" --context "system" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-523885807 ../empty.syscfg
device.opt: build-523885807
device.cmd.genlibs: build-523885807
ti_msp_dl_config.c: build-523885807
ti_msp_dl_config.h: build-523885807
Event.dot: build-523885807

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/Debug" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/BSP" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/User" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/DSP/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: C:/ti/mspm0_sdk_2_03_00_07/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/Debug" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/BSP" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/User" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/DSP/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs2020/ccs/tools/compiler/ti-cgt-armllvm_4.0.3.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/Debug" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/BSP" -I"D:/unniversity/EIDC/ZC008/ZC008_v0.2.0/User" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/DSP/Include" -I"C:/ti/mspm0_sdk_2_03_00_07/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '



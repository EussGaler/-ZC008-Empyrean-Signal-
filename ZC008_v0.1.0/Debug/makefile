################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs2001/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS

GEN_OPTS__FLAG := @"./device.opt" 
GEN_CMDS__FLAG := -Wl,-l"./device_linker.cmd" 

ORDERED_OBJS += \
"./ti_msp_dl_config.o" \
"./startup_mspm0g350x_ticlang.o" \
"./main.o" \
"./BSP/adc.o" \
"./BSP/delay.o" \
"./BSP/key.o" \
"./BSP/lcd.o" \
"./BSP/lcd_init.o" \
$(GEN_CMDS__FLAG) \
-Wl,-l"C:/ti/mspm0_sdk_2_03_00_07/source/third_party/CMSIS/DSP/lib/ticlang/m0p/arm_cortexM0l_math.a" \
-Wl,-ldevice.cmd.genlibs \
-Wl,-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include BSP/subdir_vars.mk
-include subdir_rules.mk
-include BSP/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
ZC008_v0.1.0.out 

EXE_OUTPUTS__QUOTED += \
"ZC008_v0.1.0.out" 

BIN_OUTPUTS += \
ZC008_v0.1.0.hex 

BIN_OUTPUTS__QUOTED += \
"ZC008_v0.1.0.hex" 


# All Target
all: $(OBJS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "ZC008_v0.1.0.out" secondary-outputs

# Tool invocations
ZC008_v0.1.0.out: $(OBJS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs2001/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmclang.exe" @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O0 -gdwarf-3 -Wl,-m"ZC008_v0.1.0.map" -Wl,-i"C:/ti/mspm0_sdk_2_03_00_07/source" -Wl,-i"D:/unniversity/EIDC/ZC008/ZC008_v0.1.0" -Wl,-i"D:/unniversity/EIDC/ZC008/ZC008_v0.1.0/Debug/syscfg" -Wl,-i"C:/ti/ccs2001/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/lib" -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="ZC008_v0.1.0_linkInfo.xml" -Wl,--rom_model -o "ZC008_v0.1.0.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

ZC008_v0.1.0.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: Arm Hex Utility'
	"C:/ti/ccs2001/ccs/tools/compiler/ti-cgt-armllvm_4.0.1.LTS/bin/tiarmhex.exe" --memwidth=16 --romwidth=16 --diag_wrap=off --intel -o "ZC008_v0.1.0.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(BIN_OUTPUTS__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "ti_msp_dl_config.o" "startup_mspm0g350x_ticlang.o" "main.o" "BSP\adc.o" "BSP\delay.o" "BSP\key.o" "BSP\lcd.o" "BSP\lcd_init.o" 
	-$(RM) "ti_msp_dl_config.d" "startup_mspm0g350x_ticlang.d" "main.d" "BSP\adc.d" "BSP\delay.d" "BSP\key.d" "BSP\lcd.d" "BSP\lcd_init.d" 
	-@echo 'Finished clean'
	-@echo ' '

secondary-outputs: $(BIN_OUTPUTS)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets


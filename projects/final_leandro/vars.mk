# ##############################################################################
# Sample Makefile for AVR/STM32 HAL Library applications in C and C++
# Leandro Schwarz
# ##############################################################################

# ==============================================================================
# Environment variables
# ==============================================================================

# ------------------------------------------------------------------------------
# Received through application -------------------------------------------------

BUILD_DIR								:= build
BUILD_NAME								:= application
COMPILE_C_AS_CPP						:= 0
COMPILER_DEBUG							:= 0
COMPILER_OPT							:= -g -Og
# COMPILER_OPT							:= -Os
COMPILER_PATH							:=
COMPILER_STD_C							:= c11
COMPILER_STD_CPP						:= c++14
FUNSAPE_PATH							:=
EXCLUDE_PATH							:= "_hide"
LIBRARY_PATH							:=
MCU_DEVICE_CORE_FLOAT_ABI				:=
MCU_DEVICE_CORE_FPU						:=
MCU_DEVICE_NAME							:= atmega328p
# MCU_DEVICE_NAME							:= STM32F103C8Tx
PROGRAMMER_CONFIG_FILE					:= /home/schwarz/app/vscode/toolchains/avrdude.conf
PROGRAMMER_TYPE							:= arduino
# PROGRAMMER_TYPE							:= ST_LINK_JTAG
# PROGRAMMER_TYPE							:= ST_LINK_SWD
USER_DEFINES_ASM						:=
USER_DEFINES_C							:= USE_HAL_DRIVER
USER_DEFINES_CPP						:= USE_HAL_DRIVER
USER_FLAGS_ASM							:=
USER_FLAGS_C							:= -Wno-switch
USER_FLAGS_CPP							:= -Wno-switch
USER_LIBS								:=

#
# Wizard autogenerated makefile.
# DO NOT EDIT, use the kernel_user.mk file instead.
#

# Constants automatically defined by the selected modules
kernel_DEBUG = 1

# Our target application
TRG += kernel

kernel_PREFIX = "avr-"

kernel_SUFFIX = ""

kernel_SRC_PATH = boards/triface/templates/kernel

kernel_HW_PATH = boards/triface

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_USER_CSRC INSTEAD!
kernel_WIZARD_CSRC = \
	bertos/kern/kfile.c \
	bertos/kern/signal.c \
	bertos/cpu/avr/drv/timer_avr.c \
	bertos/kern/sem.c \
	bertos/mware/formatwr.c \
	bertos/struct/heap.c \
	bertos/kern/monitor.c \
	bertos/drv/timer.c \
	bertos/mware/event.c \
	bertos/kern/proc.c \
	bertos/cpu/avr/drv/ser_avr.c \
	bertos/drv/ser.c \
	bertos/mware/hex.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_USER_PCSRC INSTEAD!
kernel_WIZARD_PCSRC = \
	bertos/mware/formatwr.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_USER_CPPASRC INSTEAD!
kernel_WIZARD_CPPASRC = \
	bertos/cpu/avr/hw/switch_ctx_avr.S \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_USER_CXXSRC INSTEAD!
kernel_WIZARD_CXXSRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_USER_ASRC INSTEAD!
kernel_WIZARD_ASRC = \
	 \
	#

kernel_CPPFLAGS = -D'CPU_FREQ=(14745600UL)' -D'ARCH=(ARCH_DEFAULT)' -D'WIZ_AUTOGEN' -I$(kernel_HW_PATH) -I$(kernel_SRC_PATH) $(kernel_CPU_CPPFLAGS) $(kernel_USER_CPPFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_LDFLAGS = $(kernel_CPU_LDFLAGS) $(kernel_WIZARD_LDFLAGS) $(kernel_USER_LDFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_CPPAFLAGS = $(kernel_CPU_CPPAFLAGS) $(kernel_WIZARD_CPPAFLAGS) $(kernel_USER_CPPAFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_CSRC = $(kernel_CPU_CSRC) $(kernel_WIZARD_CSRC) $(kernel_USER_CSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_PCSRC = $(kernel_CPU_PCSRC) $(kernel_WIZARD_PCSRC) $(kernel_USER_PCSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_CPPASRC = $(kernel_CPU_CPPASRC) $(kernel_WIZARD_CPPASRC) $(kernel_USER_CPPASRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_CXXSRC = $(kernel_CPU_CXXSRC) $(kernel_WIZARD_CXXSRC) $(kernel_USER_CXXSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_ASRC = $(kernel_CPU_ASRC) $(kernel_WIZARD_ASRC) $(kernel_USER_ASRC)

# CPU specific flags and options, defined in the CPU definition files.
# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_MCU = atmega1281
kernel_CPU_CPPFLAGS = -Os -Ibertos/cpu/avr/
kernel_PROGRAMMER_CPU = atmega1281
kernel_STOPFLASH_SCRIPT = bertos/prg_scripts/avr/stopflash.sh
kernel_STOPDEBUG_SCRIPT = bertos/prg_scripts/none.sh
kernel_DEBUG_SCRIPT = bertos/prg_scripts/nodebug.sh
kernel_FLASH_SCRIPT = bertos/prg_scripts/avr/flash.sh

include $(kernel_SRC_PATH)/kernel_user.mk

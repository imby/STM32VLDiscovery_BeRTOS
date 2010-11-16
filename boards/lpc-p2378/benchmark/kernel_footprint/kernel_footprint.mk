#
# Wizard autogenerated makefile.
# DO NOT EDIT, use the kernel_footprint_user.mk file instead.
#

# Constants automatically defined by the selected modules


# Our target application
TRG += kernel_footprint

kernel_footprint_PREFIX = "arm-none-eabi-"

kernel_footprint_SUFFIX = ""

kernel_footprint_SRC_PATH = boards/ek-lpc-p2378/benchmark/kernel_footprint

kernel_footprint_HW_PATH = boards/ek-lpc-p2378

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_footprint_USER_CSRC INSTEAD!
kernel_footprint_WIZARD_CSRC = \
	bertos/kern/signal.c \
	bertos/cpu/arm/drv/timer_lpc2.c \
	bertos/kern/sem.c \
	bertos/drv/timer.c \
	bertos/mware/formatwr.c \
	bertos/cpu/arm/drv/timer_arm.c \
	bertos/io/kfile.c \
	bertos/mware/event.c \
	bertos/kern/proc.c \
	bertos/benchmark/kernel_footprint.c \
	bertos/mware/hex.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_footprint_USER_PCSRC INSTEAD!
kernel_footprint_WIZARD_PCSRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_footprint_USER_CPPASRC INSTEAD!
kernel_footprint_WIZARD_CPPASRC = \
	bertos/cpu/arm/hw/switch_ctx_arm.S \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_footprint_USER_CXXSRC INSTEAD!
kernel_footprint_WIZARD_CXXSRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE kernel_footprint_USER_ASRC INSTEAD!
kernel_footprint_WIZARD_ASRC = \
	 \
	#

kernel_footprint_CPPFLAGS = -D'CPU_FREQ=(72000000UL)' -D'ARCH=(ARCH_DEFAULT)' -D'WIZ_AUTOGEN' -I$(kernel_footprint_HW_PATH) -I$(kernel_footprint_SRC_PATH) $(kernel_footprint_CPU_CPPFLAGS) $(kernel_footprint_USER_CPPFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_footprint_LDFLAGS = $(kernel_footprint_CPU_LDFLAGS) $(kernel_footprint_WIZARD_LDFLAGS) $(kernel_footprint_USER_LDFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_footprint_CPPAFLAGS = $(kernel_footprint_CPU_CPPAFLAGS) $(kernel_footprint_WIZARD_CPPAFLAGS) $(kernel_footprint_USER_CPPAFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_footprint_CSRC = $(kernel_footprint_CPU_CSRC) $(kernel_footprint_WIZARD_CSRC) $(kernel_footprint_USER_CSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_footprint_PCSRC = $(kernel_footprint_CPU_PCSRC) $(kernel_footprint_WIZARD_PCSRC) $(kernel_footprint_USER_PCSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_footprint_CPPASRC = $(kernel_footprint_CPU_CPPASRC) $(kernel_footprint_WIZARD_CPPASRC) $(kernel_footprint_USER_CPPASRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_footprint_CXXSRC = $(kernel_footprint_CPU_CXXSRC) $(kernel_footprint_WIZARD_CXXSRC) $(kernel_footprint_USER_CXXSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_footprint_ASRC = $(kernel_footprint_CPU_ASRC) $(kernel_footprint_WIZARD_ASRC) $(kernel_footprint_USER_ASRC)

# CPU specific flags and options, defined in the CPU definition files.
# Automatically generated by the wizard. PLEASE DO NOT EDIT!
kernel_footprint_CPU_CPPASRC = bertos/cpu/arm/hw/crt_arm7tdmi.S bertos/cpu/arm/hw/vectors_lpc2.S
kernel_footprint_CPU_CPPAFLAGS = -g -gdwarf-2
kernel_footprint_CPU_CPPFLAGS = -O0 -g3 -gdwarf-2 -fverbose-asm -Ibertos/cpu/arm/ -D__ARM_LPC2378__
kernel_footprint_CPU_CSRC = bertos/cpu/arm/hw/init_lpc2.c bertos/cpu/arm/drv/vic_lpc2.c
kernel_footprint_PROGRAMMER_CPU = lpc2378
kernel_footprint_CPU_LDFLAGS = -nostartfiles -Wl,--no-warn-mismatch -T bertos/cpu/arm/scripts/lpc2378.ld
kernel_footprint_STOPFLASH_SCRIPT = bertos/prg_scripts/arm/stopopenocd.sh
kernel_footprint_CPU = arm7tdmi
kernel_footprint_STOPDEBUG_SCRIPT = bertos/prg_scripts/arm/stopopenocd.sh
kernel_footprint_DEBUG_SCRIPT = bertos/prg_scripts/arm/debug.sh
kernel_footprint_FLASH_SCRIPT = bertos/prg_scripts/arm/flash.sh

include $(kernel_footprint_SRC_PATH)/kernel_footprint_user.mk

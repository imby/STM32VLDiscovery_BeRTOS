#
# Wizard autogenerated makefile.
# DO NOT EDIT, use the display_user.mk file instead.
#

# Constants automatically defined by the selected modules
display_DEBUG = 1

# Our target application
TRG += display

display_PREFIX = "arm-none-eabi-"

display_SUFFIX = ""

display_SRC_PATH = boards/sam3n-ek/examples/display

display_HW_PATH = boards/sam3n-ek

# Files automatically generated by the wizard. DO NOT EDIT, USE display_USER_CSRC INSTEAD!
display_WIZARD_CSRC = \
	bertos/struct/heap.c \
	bertos/cpu/cortex-m3/hw/switch_ctx_cm3.c \
	bertos/mware/event.c \
	bertos/gfx/bitmap.c \
	bertos/gfx/text.c \
	bertos/drv/lcd_ili9225.c \
	bertos/cpu/cortex-m3/drv/timer_cm3.c \
	bertos/drv/timer.c \
	bertos/io/kfile.c \
	bertos/mware/formatwr.c \
	bertos/mware/sprintf.c \
	bertos/gfx/line.c \
	bertos/gfx/text_format.c \
	bertos/drv/kbd.c \
	bertos/fonts/luBS14.c \
	bertos/kern/signal.c \
	bertos/kern/proc.c \
	bertos/mware/hex.c \
	bertos/gui/menu.c \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE display_USER_PCSRC INSTEAD!
display_WIZARD_PCSRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE display_USER_CPPASRC INSTEAD!
display_WIZARD_CPPASRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE display_USER_CXXSRC INSTEAD!
display_WIZARD_CXXSRC = \
	 \
	#

# Files automatically generated by the wizard. DO NOT EDIT, USE display_USER_ASRC INSTEAD!
display_WIZARD_ASRC = \
	 \
	#

display_CPPFLAGS = -D'CPU_FREQ=(48000000UL)' -D'ARCH=(ARCH_DEFAULT)' -D'WIZ_AUTOGEN' -I$(display_HW_PATH) -I$(display_SRC_PATH) $(display_CPU_CPPFLAGS) $(display_USER_CPPFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
display_LDFLAGS = $(display_CPU_LDFLAGS) $(display_WIZARD_LDFLAGS) $(display_USER_LDFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
display_CPPAFLAGS = $(display_CPU_CPPAFLAGS) $(display_WIZARD_CPPAFLAGS) $(display_USER_CPPAFLAGS)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
display_CSRC = $(display_CPU_CSRC) $(display_WIZARD_CSRC) $(display_USER_CSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
display_PCSRC = $(display_CPU_PCSRC) $(display_WIZARD_PCSRC) $(display_USER_PCSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
display_CPPASRC = $(display_CPU_CPPASRC) $(display_WIZARD_CPPASRC) $(display_USER_CPPASRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
display_CXXSRC = $(display_CPU_CXXSRC) $(display_WIZARD_CXXSRC) $(display_USER_CXXSRC)

# Automatically generated by the wizard. PLEASE DO NOT EDIT!
display_ASRC = $(display_CPU_ASRC) $(display_WIZARD_ASRC) $(display_USER_ASRC)

# CPU specific flags and options, defined in the CPU definition files.
# Automatically generated by the wizard. PLEASE DO NOT EDIT!
display_CPU_CPPASRC = bertos/cpu/cortex-m3/hw/crt_cm3.S bertos/cpu/cortex-m3/hw/vectors_cm3.S 
display_CPU_CPPAFLAGS = -g -gdwarf-2 -mthumb -mno-thumb-interwork
display_CPU_CPPFLAGS = -O0 -g3 -gdwarf-2 -mthumb -mno-thumb-interwork -fno-strict-aliasing -fwrapv -fverbose-asm -Ibertos/cpu/cortex-m3/ -D__ARM_SAM3N4__
display_CPU_CSRC = bertos/cpu/cortex-m3/hw/init_cm3.c bertos/cpu/cortex-m3/drv/irq_cm3.c bertos/cpu/cortex-m3/drv/clock_sam3.c 
display_PROGRAMMER_CPU = sam3
display_CPU_LDFLAGS = -mthumb -mno-thumb-interwork -nostartfiles -Wl,--no-warn-mismatch -T bertos/cpu/cortex-m3/scripts/sam3n4_rom.ld
display_STOPFLASH_SCRIPT = bertos/prg_scripts/arm/stopopenocd.sh
display_CPU = cortex-m3
display_STOPDEBUG_SCRIPT = bertos/prg_scripts/arm/stopopenocd.sh
display_DEBUG_SCRIPT = bertos/prg_scripts/arm/debug.sh
display_FLASH_SCRIPT = bertos/prg_scripts/arm/flash-sam3.sh

include $(display_SRC_PATH)/display_user.mk

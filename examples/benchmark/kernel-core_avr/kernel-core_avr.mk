#
# Copyright 2009 Develer S.r.l. (http://www.develer.com/)
# All rights reserved.
#
# Makefile template for BeRTOS wizard.
#
# Author: Lorenzo Berni <duplo@develer.com>
#
#

# Programmer interface configuration, see http://dev.bertos.org/wiki/ProgrammerInterface for help
kernel-core_avr_PROGRAMMER_TYPE = none
kernel-core_avr_PROGRAMMER_PORT = none

# Files included by the user.
kernel-core_avr_USER_CSRC = \
	examples/benchmark/kernel-core_avr/main.c \
	#

# Files included by the user.
kernel-core_avr_USER_PCSRC = \
	#

# Files included by the user.
kernel-core_avr_USER_CPPASRC = \
	#

# Files included by the user.
kernel-core_avr_USER_CXXSRC = \
	#

# Files included by the user.
kernel-core_avr_USER_ASRC = \
	#

# Flags included by the user.
kernel-core_avr_USER_LDFLAGS = \
	#

# Flags included by the user.
kernel-core_avr_USER_CPPAFLAGS = -O2 \
	#

# Flags included by the user.
kernel-core_avr_USER_CPPFLAGS = -O2 \
	#

# Include the mk file generated by the wizard
include examples/benchmark/kernel-core_avr/kernel-core_avr_wiz.mk

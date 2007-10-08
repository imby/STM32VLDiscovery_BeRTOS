/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2004, 2005 Develer S.r.l. (http://www.develer.com/)
 * This file is part of DevLib - See README.devlib for information.
 * -->
 *
 * \brief OS-specific definitions
 *
 * \version $Id$
 *
 * \author Bernardo Innocenti <bernie@develer.com>
 */

/*#*
 *#* $Log$
 *#* Revision 1.11  2007/09/29 16:19:47  bernie
 *#* Changes to compile with sparse.
 *#*
 *#* Revision 1.10  2006/07/19 12:56:25  bernie
 *#* Convert to new Doxygen style.
 *#*
 *#* Revision 1.9  2006/03/22 13:34:34  bernie
 *#* MSVC support.
 *#*
 *#* Revision 1.8  2006/02/23 09:09:28  bernie
 *#* Remove Linux specific hack.
 *#*
 *#* Revision 1.7  2006/02/20 01:46:59  bernie
 *#* Port to MacOSX.
 *#*
 *#* Revision 1.6  2006/02/15 09:12:33  bernie
 *#* Don't mask useful user signals on UNIX.
 *#*
 *#* Revision 1.5  2005/11/27 23:32:42  bernie
 *#* Add CPU fallback for OS_ID.
 *#*
 *#* Revision 1.4  2005/11/27 03:07:13  bernie
 *#* IRQ_SAVE_DISABLE(): Really block signals.
 *#*
 *#* Revision 1.3  2005/11/27 03:02:40  bernie
 *#* Add POSIX emulation for IRQ_* macros; Add Qt support.
 *#*
 *#* Revision 1.2  2005/11/04 16:20:01  bernie
 *#* Fix reference to README.devlib in header.
 *#*
 *#* Revision 1.1  2005/04/11 19:04:13  bernie
 *#* Move top-level headers to cfg/ subdir.
 *#*
 *#* Revision 1.1  2004/12/31 17:40:24  bernie
 *#* Add OS detection code.
 *#*
 *#*/
#ifndef DEVLIB_OS_H
#define DEVLIB_OS_H

/** Macro to include OS-specific versions of the headers. */
#define OS_HEADER(module)  PP_STRINGIZE(PP_CAT3(module, _, OS_ID).h)
#define OS_CSOURCE(module) PP_STRINGIZE(PP_CAT3(module, _, OS_ID).c)

/*
 * OS autodetection (Some systems trigger multiple OS definitions)
 */
#ifdef _WIN32
	#define OS_WIN32  1
	#define OS_ID     win32

	// FIXME: Maybe disable Win32 exceptions?
	typedef int cpuflags_t;
	#define IRQ_DISABLE                /* FIXME */
	#define IRQ_ENABLE                 /* FIXME */
	#define IRQ_SAVE_DISABLE(old_sigs) /* FIXME */
	#define IRQ_RESTORE(old_sigs)      /* FIXME */

#else
	#define OS_WIN32  0
#endif

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
	#define OS_UNIX   1
	#define OS_POSIX  1  /* Not strictly UNIX, but no way to autodetect it. */
	#define OS_ID     posix

	/*
	 * The POSIX moral equivalent of disabling IRQs is disabling signals.
	 */
	#include <signal.h>
	typedef sigset_t cpuflags_t;

	#define SET_ALL_SIGNALS(sigs) \
	do { \
		sigfillset(&sigs); \
		sigdelset(&sigs, SIGINT); \
		sigdelset(&sigs, SIGSTOP); \
		sigdelset(&sigs, SIGCONT); \
	} while(0)

	#define IRQ_DISABLE \
	do { \
		sigset_t sigs; \
		SET_ALL_SIGNALS(sigs); \
		sigprocmask(SIG_BLOCK, &sigs, NULL); \
	} while (0)

	#define IRQ_ENABLE \
	do { \
		sigset_t sigs; \
		SET_ALL_SIGNALS(sigs); \
		sigprocmask(SIG_UNBLOCK, &sigs, NULL); \
	} while (0)

	#define IRQ_SAVE_DISABLE(old_sigs) \
	do { \
		sigset_t sigs; \
		SET_ALL_SIGNALS(sigs); \
		sigprocmask(SIG_BLOCK, &sigs, &old_sigs); \
	} while (0)

	#define IRQ_RESTORE(old_sigs) \
	do { \
		sigprocmask(SIG_SETMASK, &old_sigs, NULL); \
	} while (0)
#else
	#define OS_UNIX   0
	#define OS_POSIX  0
#endif

#ifdef __linux__
	#define OS_LINUX  1
#else
	#define OS_LINUX  0
#endif

#if defined(__APPLE__) && defined(__MACH__)
	#define OS_DARWIN 1
#else
	#define OS_DARWIN 0
#endif

/*
 * We want Qt and other frameworks to look like OSes because you would
 * tipically want their portable abstractions if you're using one of these.
 */
#if defined(_QT)
	#define OS_QT 1
	#undef  OS_ID
	#define OS_ID qt
#else
	#define OS_QT 0
#endif

/*
 * Summarize hosted environments as OS_HOSTED and embedded
 * environment with OS_EMBEDDED.
 */
#if OS_WIN32 || OS_UNIX || OS_DARWIN || OS_QT
	#define OS_HOSTED   1
	#define OS_EMBEDDED 0
#else
	#define OS_HOSTED   0
	#define OS_EMBEDDED 1

	/* Embedded environments fall back to CPU-specific code. */
	#define OS_ID       CPU_ID
#endif

/* Self-check for the detection */
#if !defined(OS_ID)
	#error OS_ID not defined
#endif
#if OS_HOSTED && OS_EMBEDDED
	#error Both hosted and embedded OS environment
#endif
#if !OS_HOSTED && !OS_EMBEDDED
	#error Neither hosted nor embedded OS environment
#endif

#endif /* DEVLIB_OS_H */

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
 * Copyright 2004 Develer S.r.l. (http://www.develer.com/)
 * Copyright 1999, 2000, 2001 Bernie Innocenti <bernie@codewiz.org>
 *
 * -->
 *
 * \brief IPC signals implementation.
 *
 * Signals are a low-level IPC primitive.  A process receives a signal
 * when some external event has happened.  Like interrupt requests,
 * signals do not carry any additional information.  If processing a
 * specific event requires additional data, the process must obtain it
 * through some other mechanism.
 *
 * Despite the name, one shouldn't confuse these signals with POSIX
 * signals.  POSIX signals are usually executed synchronously, like
 * software interrupts.
 *
 * Signals are very low overhead.  Using them exclusively to wait
 * for multiple asynchronous events results in very simple dispatch
 * logic with low processor and resource usage.
 *
 * The "event" module is a higher-level interface that can optionally
 * deliver signals to processes.  Messages provide even higher-level
 * IPC services built on signals.  Semaphore arbitration is also
 * implemented using signals.
 *
 * In this implementation, each process has a limited set of signal
 * bits (usually 32) and can wait for multiple signals at the same
 * time using sig_wait().  Signals can also be polled using sig_check(),
 * but a process spinning on its signals usually defeats their purpose
 * of providing a multitasking-friendly infrastructure for event-driven
 * applications.
 *
 * Signals are like flags: they are either active or inactive.  After an
 * external event has delivered a particular signal, it remains raised until
 * the process acknowledges it using either sig_wait() or sig_check().
 * Counting signals is not a reliable way to count how many times a
 * particular event has occurred, because the same signal may be
 * delivered twice before the process can notice.
 *
 * Any execution context, including an interrupt handler, can deliver
 * a signal to a process using sig_signal().  Multiple independent signals
 * may be delivered at once with a single invocation of sig_signal(),
 * although this is rarely useful.
 *
 * \section signal_allocation Signal Allocation
 *
 * There's no hardcoded mapping of specific events to signal bits.
 * The meaning of a particular signal bit is defined by an agreement
 * between the delivering entity and the receiving process.
 * For instance, a terminal driver may be designed to deliver
 * a signal bit called SIG_INT when it reads the CTRL-C sequence
 * from the keyboard, and a process may react to it by quitting.
 *
 * \section sig_single SIG_SINGLE
 *
 * The SIG_SINGLE bit is reserved as a convenient shortcut in those
 * simple scenarios where a process needs to wait on just one event
 * synchronously.  By using SIG_SINGLE, there's no need to allocate
 * a specific signal from the free pool.  The constraints for safely
 * accessing SIG_SINGLE are:
 *  - The process MUST sig_wait() exclusively on SIG_SINGLE
 *  - SIG_SIGNAL MUST NOT be left pending after use (sig_wait() will reset
 *	  it automatically)
 *  - Do not sleep between starting the asynchronous task that will fire
 *    SIG_SINGLE, and the call to  sig_wait().
 *  - Do not call system functions that may implicitly sleep, such as
 *    timer_delayTickes().
 *
 * \version $Id$
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 */

#include "signal.h"

#include <cfg/debug.h>
#include <drv/timer.h>
#include <kern/proc.h>
#include <kern/proc_p.h>
 

#if CONFIG_KERN_SIGNALS

/**
 * Check if any of the signals in \a sigs has occurred and clear them.
 * \return the signals that have occurred.
 */
sigmask_t sig_check(sigmask_t sigs)
{
	sigmask_t result;
	cpuflags_t flags;

	IRQ_SAVE_DISABLE(flags);
	result = CurrentProcess->sig_recv & sigs;
	CurrentProcess->sig_recv &= ~sigs;
	IRQ_RESTORE(flags);

	return result;
}


/**
 * Sleep until any of the signals in \a sigs occurs.
 * \return the signal(s) that have awoken the process.
 */
sigmask_t sig_wait(sigmask_t sigs)
{
	sigmask_t result;
	cpuflags_t flags;

	IRQ_SAVE_DISABLE(flags);

	/* Loop until we get at least one of the signals */
	while (!(result = CurrentProcess->sig_recv & sigs))
	{
		/* go to sleep and proc_schedule() another process */
		CurrentProcess->sig_wait = sigs;
		proc_schedule();

		/* When we come back here, a signal must be arrived */
		ASSERT(!CurrentProcess->sig_wait);
		ASSERT(CurrentProcess->sig_recv);
	}

	/* Signals found: clear them and return */
	CurrentProcess->sig_recv &= ~sigs;

	IRQ_RESTORE(flags);
	return result;
}

/**
 * Sleep until any of the signals in \a sigs or \a timeout ticks elapse.
 * If the timeout elapse a SIG_TIMEOUT is added to the received signal(s).
 * \return the signal(s) that have awoken the process.
 * \note Caller must check return value to check which signal awoke the process.
 */
sigmask_t sig_waitTimeout(sigmask_t sigs, ticks_t timeout)
{
	Timer t;
	sigmask_t res;
	cpuflags_t flags;

	ASSERT(!sig_check(SIG_TIMEOUT));
	ASSERT(!(sigs & SIG_TIMEOUT));
	/* IRQ are needed to run timer */
	ASSERT(IRQ_ENABLED());

	timer_set_event_signal(&t, proc_current(), SIG_TIMEOUT);
	timer_setDelay(&t, timeout);
	timer_add(&t);
	res = sig_wait(SIG_TIMEOUT | sigs);

	IRQ_SAVE_DISABLE(flags);
	/* Remove timer if sigs occur before timer signal */
	if (!(res & SIG_TIMEOUT) && !sig_check(SIG_TIMEOUT))
		timer_abort(&t);
	IRQ_RESTORE(flags);
	return res;
}


/**
 * Send the signals \a sigs to the process \a proc.
 * The process will be awoken if it was waiting for any of them.
 *
 * \note This call is interrupt safe.
 */
void sig_signal(Process *proc, sigmask_t sigs)
{
	cpuflags_t flags;
	IRQ_SAVE_DISABLE(flags);

	/* Set the signals */
	proc->sig_recv |= sigs;

	/* Check if process needs to be awoken */
	if (proc->sig_recv & proc->sig_wait)
	{
		/* Wake up process and enqueue in ready list */
		proc->sig_wait = 0;
		SCHED_ENQUEUE(proc);
	}

	IRQ_RESTORE(flags);
}

#endif /* CONFIG_KERN_SIGNALS */


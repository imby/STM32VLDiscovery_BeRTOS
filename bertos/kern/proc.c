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
 * Copyright 2001, 2004 Develer S.r.l. (http://www.develer.com/)
 * Copyright 1999, 2000, 2001, 2008 Bernie Innocenti <bernie@codewiz.org>
 * -->
 *
 * \brief Simple cooperative multitasking scheduler.
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 * \author Stefano Fedrigo <aleph@develer.com>
 */

#include "proc_p.h"
#include "proc.h"

#include "cfg/cfg_proc.h"
#define LOG_LEVEL KERN_LOG_LEVEL
#define LOG_FORMAT KERN_LOG_FORMAT
#include <cfg/log.h>

#include "cfg/cfg_monitor.h"
#include <cfg/macros.h>    // ROUND_UP2
#include <cfg/module.h>
#include <cfg/depend.h>    // CONFIG_DEPEND()

#include <cpu/irq.h>
#include <cpu/types.h>
#include <cpu/attr.h>
#include <cpu/frame.h>

#if CONFIG_KERN_HEAP
	#include <struct/heap.h>
#endif

#include <string.h>           /* memset() */

#define PROC_SIZE_WORDS (ROUND_UP2(sizeof(Process), sizeof(cpu_stack_t)) / sizeof(cpu_stack_t))

/*
 * The scheduer tracks ready processes by enqueuing them in the
 * ready list.
 *
 * \note Access to the list must occur while interrupts are disabled.
 */
REGISTER List proc_ready_list;

/*
 * Holds a pointer to the TCB of the currently running process.
 *
 * \note User applications should use proc_current() to retrieve this value.
 */
REGISTER Process *current_process;

/** The main process (the one that executes main()). */
static struct Process main_process;

#if CONFIG_KERN_HEAP

/**
 * Local heap dedicated to allocate the memory used by the processes.
 */
static HEAP_DEFINE_BUF(heap_buf, KERN_MINSTACKSIZE * 128);
static Heap proc_heap;

/*
 * Keep track of zombie processes (processes that are exiting and need to
 * release some resources).
 *
 * \note Access to the list must occur while kernel preemption is disabled.
 */
static List zombie_list;

#endif /* CONFIG_KERN_HEAP */

static void proc_initStruct(Process *proc)
{
	/* Avoid warning for unused argument. */
	(void)proc;

#if CONFIG_KERN_SIGNALS
	proc->sig_recv = 0;
	proc->sig_wait = 0;
#endif

#if CONFIG_KERN_HEAP
	proc->flags = 0;
#endif

#if CONFIG_KERN_PRI
	proc->link.pri = 0;
#endif
}

MOD_DEFINE(proc);

void proc_init(void)
{
	LIST_INIT(&proc_ready_list);

#if CONFIG_KERN_HEAP
	LIST_INIT(&zombie_list);
	heap_init(&proc_heap, heap_buf, sizeof(heap_buf));
#endif
	/*
	 * We "promote" the current context into a real process. The only thing we have
	 * to do is create a PCB and make it current. We don't need to setup the stack
	 * pointer because it will be written the first time we switch to another process.
	 */
	proc_initStruct(&main_process);
	current_process = &main_process;

#if CONFIG_KERN_MONITOR
	monitor_init();
	monitor_add(current_process, "main");
#endif

#if CONFIG_KERN_PREEMPT
	preempt_init();
#endif

	MOD_INIT(proc);
}


#if CONFIG_KERN_HEAP

/**
 * Free all the resources of all zombie processes previously added to the zombie
 * list.
 */
static void proc_freeZombies(void)
{
	Process *proc;

	while (1)
	{
		PROC_ATOMIC(proc = (Process *)list_remHead(&zombie_list));
		if (proc == NULL)
			return;

		if (proc->flags & PF_FREESTACK)
		{
			PROC_ATOMIC(heap_freemem(&proc_heap, proc->stack_base,
				proc->stack_size + PROC_SIZE_WORDS * sizeof(cpu_stack_t)));
		}
	}
}

/**
 * Enqueue a process in the zombie list.
 */
static void proc_addZombie(Process *proc)
{
	Node *node;
#if CONFIG_KERN_PREEMPT
	ASSERT(!proc_preemptAllowed());
#endif

#if CONFIG_KERN_PRI
	node = &(proc)->link.link;
#else
	node = &(proc)->link;
#endif
	LIST_ASSERT_VALID(&zombie_list);
	ADDTAIL(&zombie_list, node);
}

#endif /* CONFIG_KERN_HEAP */

/**
 * Create a new process, starting at the provided entry point.
 *
 *
 * \note The function
 * \code
 * proc_new(entry, data, stacksize, stack)
 * \endcode
 * is a more convenient way to create a process, as you don't have to specify
 * the name.
 *
 * \return Process structure of new created process
 *         if successful, NULL otherwise.
 */
struct Process *proc_new_with_name(UNUSED_ARG(const char *, name), void (*entry)(void), iptr_t data, size_t stack_size, cpu_stack_t *stack_base)
{
	Process *proc;
	LOG_INFO("name=%s", name);
#if CONFIG_KERN_HEAP
	bool free_stack = false;

	/*
	 * Free up resources of a zombie process.
	 *
	 * We're implementing a kind of lazy garbage collector here for
	 * efficiency reasons: we can avoid to introduce overhead into another
	 * kernel task dedicated to free up resources (e.g., idle) and we're
	 * not introducing any overhead into the scheduler after a context
	 * switch (that would be *very* bad, because the scheduler runs with
	 * IRQ disabled).
	 *
	 * In this way we are able to release the memory of the zombie tasks
	 * without disabling IRQs and without introducing any significant
	 * overhead in any other kernel task.
	 */
	proc_freeZombies();

	/* Did the caller provide a stack for us? */
	if (!stack_base)
	{
		/* Did the caller specify the desired stack size? */
		if (!stack_size)
			stack_size = KERN_MINSTACKSIZE;

		/* Allocate stack dinamically */
		PROC_ATOMIC(stack_base =
			(cpu_stack_t *)heap_allocmem(&proc_heap, stack_size));
		if (stack_base == NULL)
			return NULL;

		free_stack = true;
	}

#else // CONFIG_KERN_HEAP

	/* Stack must have been provided by the user */
	ASSERT_VALID_PTR(stack_base);
	ASSERT(stack_size);

#endif // CONFIG_KERN_HEAP

#if CONFIG_KERN_MONITOR
	/*
	 * Fill-in the stack with a special marker to help debugging.
	 * On 64bit platforms, CONFIG_KERN_STACKFILLCODE is larger
	 * than an int, so the (int) cast is required to silence the
	 * warning for truncating its size.
	 */
	memset(stack_base, (int)CONFIG_KERN_STACKFILLCODE, stack_size);
#endif

	/* Initialize the process control block */
	if (CPU_STACK_GROWS_UPWARD)
	{
		proc = (Process *)stack_base;
		proc->stack = stack_base + PROC_SIZE_WORDS;
		// On some architecture stack should be aligned, so we do it.
		proc->stack = (cpu_stack_t *)((uintptr_t)proc->stack + (sizeof(cpu_aligned_stack_t) - ((uintptr_t)proc->stack % sizeof(cpu_aligned_stack_t))));
		if (CPU_SP_ON_EMPTY_SLOT)
			proc->stack++;
	}
	else
	{
		proc = (Process *)(stack_base + stack_size / sizeof(cpu_stack_t) - PROC_SIZE_WORDS);
		// On some architecture stack should be aligned, so we do it.
		proc->stack = (cpu_stack_t *)((uintptr_t)proc - ((uintptr_t)proc % sizeof(cpu_aligned_stack_t)));
		if (CPU_SP_ON_EMPTY_SLOT)
			proc->stack--;
	}
	/* Ensure stack is aligned */
	ASSERT((uintptr_t)proc->stack % sizeof(cpu_aligned_stack_t) == 0);

	stack_size -= PROC_SIZE_WORDS * sizeof(cpu_stack_t);
	proc_initStruct(proc);
	proc->user_data = data;

#if CONFIG_KERN_HEAP | CONFIG_KERN_MONITOR
	proc->stack_base = stack_base;
	proc->stack_size = stack_size;
	#if CONFIG_KERN_HEAP
	if (free_stack)
		proc->flags |= PF_FREESTACK;
	#endif
#endif
	proc->user_entry = entry;
	CPU_CREATE_NEW_STACK(proc->stack);

#if CONFIG_KERN_MONITOR
	monitor_add(proc, name);
#endif

	/* Add to ready list */
	ATOMIC(SCHED_ENQUEUE(proc));

	return proc;
}

/**
 * Return the name of the specified process.
 *
 * NULL is a legal argument and will return the name "<NULL>".
 */
const char *proc_name(struct Process *proc)
{
#if CONFIG_KERN_MONITOR
	return proc ? proc->monitor.name : "<NULL>";
#else
	(void)proc;
	return "---";
#endif
}

/// Return the name of the currently running process
const char *proc_currentName(void)
{
	return proc_name(proc_current());
}

/// Rename a process
void proc_rename(struct Process *proc, const char *name)
{
#if CONFIG_KERN_MONITOR
	monitor_rename(proc, name);
#else
	(void)proc; (void)name;
#endif
}


#if CONFIG_KERN_PRI
/**
 * Change the scheduling priority of a process.
 *
 * Process piorities are signed ints, whereas a larger integer value means
 * higher scheduling priority.  The default priority for new processes is 0.
 * The idle process runs with the lowest possible priority: INT_MIN.
 *
 * A process with a higher priority always preempts lower priority processes.
 * Processes of equal priority share the CPU time according to a simple
 * round-robin policy.
 *
 * As a general rule to maximize responsiveness, compute-bound processes
 * should be assigned negative priorities and tight, interactive processes
 * should be assigned positive priorities.
 *
 * To avoid interfering with system background activities such as input
 * processing, application processes should remain within the range -10
 * and +10.
 */
void proc_setPri(struct Process *proc, int pri)
{
	if (proc->link.pri == pri)
		return;

	proc->link.pri = pri;

	if (proc != current_process)
		ATOMIC(sched_reenqueue(proc));
}
#endif // CONFIG_KERN_PRI

INLINE void proc_run(void)
{
	void (*entry)(void) = current_process->user_entry;

	LOG_INFO("New process starting at %p", entry);
	entry();
}

/**
 * Entry point for all the processes.
 */
void proc_entry(void)
{
	/*
	 * Return from a context switch assumes interrupts are disabled, so
	 * we need to explicitly re-enable them as soon as possible.
	 */
	IRQ_ENABLE;
	/* Call the actual process's entry point */
	proc_run();
	proc_exit();
}

/**
 * Terminate the current process
 */
void proc_exit(void)
{
	LOG_INFO("%p:%s", current_process, proc_currentName());

#if CONFIG_KERN_MONITOR
	monitor_remove(current_process);
#endif

	proc_forbid();
#if CONFIG_KERN_HEAP
	/*
	 * Set the task as zombie, its resources will be freed in proc_new() in
	 * a lazy way, when another process will be created.
	 */
	proc_addZombie(current_process);
#endif
	current_process = NULL;
	proc_permit();

	proc_switch();

	/* never reached */
	ASSERT(0);
}


/**
 * Get the pointer to the user data of the current process
 */
iptr_t proc_currentUserData(void)
{
	return current_process->user_data;
}

/*!
 * \file
 * <!--
 * Copyright 2003, 2004, 2005 Develer S.r.l. (http://www.develer.com/)
 * Copyright 1999, 2001, 2003 Bernardo Innocenti <bernie@develer.com>
 * This file is part of DevLib - See README.devlib for information.
 * -->
 *
 * \brief Events handling
 *
 * This module implements a common system for executing
 * a user defined action calling a hook function.
 *
 * \version $Id$
 *
 * \author Bernardo Innocenti <bernie@develer.com>
 */

/*#*
 *#* $Log$
 *#* Revision 1.3  2006/01/16 03:27:49  bernie
 *#* Rename sig_t to sigbit_t to avoid clash with POSIX.
 *#*
 *#* Revision 1.2  2005/11/27 03:02:55  bernie
 *#* Convert to appconfig.h.
 *#*
 *#* Revision 1.1  2005/11/27 01:39:48  bernie
 *#* Move event.[ch] from kern/ to mware/.
 *#*
 *#* Revision 1.11  2005/04/11 19:10:28  bernie
 *#* Include top-level headers from cfg/ subdir.
 *#*
 *#* Revision 1.10  2005/01/24 04:22:02  bernie
 *#* Update copyright information.
 *#*
 *#* Revision 1.9  2005/01/24 04:19:55  bernie
 *#* Remove obsolete names.
 *#*
 *#* Revision 1.8  2005/01/24 04:19:05  bernie
 *#* Function pointer based event dispatching.
 *#*
 *#* Revision 1.7  2004/08/25 14:12:09  rasky
 *#* Aggiornato il comment block dei log RCS
 *#*
 *#* Revision 1.6  2004/08/14 19:37:57  rasky
 *#* Merge da SC: macros.h, pool.h, BIT_CHANGE, nome dei processi, etc.
 *#*
 *#* Revision 1.5  2004/07/30 14:30:27  rasky
 *#* Resa la sig_signal interrupt safe (con il nuovo scheduler IRQ-safe)
 *#* Rimossa event_doIntr (ora inutile) e semplificata la logica delle macro con funzioni inline
 *#*
 *#* Revision 1.4  2004/06/07 15:58:00  aleph
 *#* Add function prototypes
 *#*
 *#* Revision 1.3  2004/06/06 18:25:44  bernie
 *#* Rename event macros to look like regular functions.
 *#*
 *#* Revision 1.2  2004/06/03 11:27:09  bernie
 *#* Add dual-license information.
 *#*
 *#* Revision 1.1  2004/05/23 17:27:00  bernie
 *#* Import kern/ subdirectory.
 *#*
 *#*/
#ifndef KERN_EVENT_H
#define KERN_EVENT_H

#include <appconfig.h>

#if CONFIG_KERNEL
	#include <cfg/config_kern.h>
	#if defined(CONFIG_KERN_SIGNALS) && CONFIG_KERN_SIGNALS
		#include "signal.h"
	#endif
#endif


//! User defined callback type
typedef void (*Hook)(void *);

typedef struct Event
{
	void (*action)(struct Event *);
	union
	{
#if defined(CONFIG_KERN_SIGNALS) && CONFIG_KERN_SIGNALS
		struct
		{
			struct Process *sig_proc;  /* Process to be signalled */
			sigbit_t        sig_bit;   /* Signal to send */
		} Sig;
#endif
		struct
		{
			Hook  func;         /* Pointer to softint hook */
			void *user_data;    /* Data to be passed back to user hook */
		} Int;
	} Ev;
} Event;

void event_hook_ignore(Event *event);
void event_hook_signal(Event *event);
void event_hook_softint(Event *event);

/*! Initialize the event \a e as a no-op */
#define event_initNone(e) \
	((e)->action = event_hook_ignore)

/*! Same as event_initNone(), but returns the initialized event */
INLINE Event event_createNone(void);
INLINE Event event_createNone(void)
{
	Event e;
	e.action = event_hook_ignore;
	return e;
}

/*! Initialize the event \a e with a software interrupt (call function \a f, with parameter \a u) */
#define event_initSoftInt(e,f,u) \
	((e)->action = event_hook_softint,(e)->Ev.Int.func = (f), (e)->Ev.Int.user_data = (u))

/*! Same as event_initSoftInt(), but returns the initialized event */
INLINE Event event_createSoftInt(Hook func, void *user_data)
{
	Event e;
	e.action = event_hook_softint;
	e.Ev.Int.func = func;
	e.Ev.Int.user_data = user_data;
	return e;
}


#if defined(CONFIG_KERN_SIGNALS) && CONFIG_KERN_SIGNALS

/*! Initialize the event \a e with a signal (send signal \a s to process \a p) */
#define event_initSignal(e,p,s) \
	((e)->action = event_hook_signal,(e)->Ev.Sig.sig_proc = (p), (e)->Ev.Sig.sig_bit = (s))

/*! Same as event_initSignal(), but returns the initialized event */
INLINE Event event_createSignal(struct Process *proc, sigbit_t bit)
{
	Event e;
	e.action = event_hook_signal;
	e.Ev.Sig.sig_proc = proc;
	e.Ev.Sig.sig_bit = bit;
	return e;
}

#endif

/*! Trigger an event */
INLINE void event_do(struct Event *e)
{
	e->action(e);
}

#endif /* KERN_EVENT_H */

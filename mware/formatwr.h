/*!
 * \file
 * <!--
 * Copyright 2003, 2004 Develer S.r.l. (http://www.develer.com/)
 * This file is part of DevLib - See devlib/README for information.
 * -->
 *
 * \version $Id$
 *
 * \brief Basic "printf", "sprintf" and "fprintf" formatter.
 */

/*
 * $Log$
 * Revision 1.3  2004/07/29 22:57:09  bernie
 * Add values for new-style CONFIG_PRINTF option.
 *
 * Revision 1.2  2004/06/03 11:27:09  bernie
 * Add dual-license information.
 *
 * Revision 1.1  2004/05/23 15:43:16  bernie
 * Import mware modules.
 *
 */
#ifndef MWARE_FORMATWR_H
#define MWARE_FORMATWR_H

#include <stdarg.h>

/*!
 * \name _formatted_write() configuration
 * \{
 */
#define PRINTF_DISABLED    0
#define PRINTF_NOMODIFIERS 1
#define PRINTF_REDUCED     2
#define PRINTF_NOFLOAT     3
#define PRINTF_FULL        4
/* \} */


int _formatted_write(
	const char *format,
	void put_char_func(char c, void *user_data),
	void *user_data,
	va_list ap);

#ifdef __AVR__
	#include <avr/pgmspace.h>
	int _formatted_write_P(
		const char * PROGMEM format,
		void put_char_func(char c, void *user_data),
		void *user_data,
		va_list ap);
#endif /* __AVR__ */

#endif /* MWARE_FORMATWR_H */


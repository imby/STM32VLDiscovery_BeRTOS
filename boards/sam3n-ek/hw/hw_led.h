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
 * Copyright 2010 Develer S.r.l. (http://www.develer.com/)
 * All Rights Reserved.
 * -->
 *
 * \brief Led on/off macros.
 *
 * \author Luca Ottaviano <lottaviano@develer.com>
 */

#ifndef HW_LED_H
#define HW_LED_H

#include <cfg/macros.h>

// Leds
#define LED_BLUE_PIN       BV(23)  // Port A
#define LED_AMBER_PIN      BV(25)  // Port A
#define LED_GREEN_PIN      BV(14)  // Port B

#define LED_BLUE_ON(led)    (PIOA_CODR = LED_BLUE_PIN)
#define LED_BLUE_OFF(led)   (PIOA_SODR = LED_BLUE_PIN)
#define LED_AMBER_ON(led)   (PIOA_CODR = LED_AMBER_PIN)
#define LED_AMBER_OFF(led)  (PIOA_SODR = LED_AMBER_PIN)
#define LED_GREEN_ON(led)   (PIOB_CODR = LED_GREEN_PIN)
#define LED_GREEN_OFF(led)  (PIOB_SODR = LED_GREEN_PIN)

#define LED_INIT() \
	do { \
		PIOA_SODR = LED_BLUE_PIN | LED_AMBER_PIN; \
		PIOA_OER = LED_BLUE_PIN | LED_AMBER_PIN; \
		PIOA_PER = LED_BLUE_PIN | LED_AMBER_PIN; \
		PIOB_SODR = LED_GREEN_PIN; \
		PIOB_OER = LED_GREEN_PIN; \
		PIOB_PER = LED_GREEN_PIN; \
	} while(0)

#endif /* HW_LED_H */

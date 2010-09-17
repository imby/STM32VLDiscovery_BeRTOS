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
 *
 * -->
 *
 * \brief AT91SAM3 debug support (implementation).
 *
 * \author Stefano Fedrigo <aleph@develer.com>
 */

#include <cfg/cfg_debug.h>
#include <cfg/macros.h> /* for BV() */

#include <io/sam3_gpio.h>
#include <io/sam3_pmc.h>
#include <io/sam3_uart.h>


#if CONFIG_KDEBUG_PORT == 0
	#define UART_BASE       UART0_BASE
	#define UART_INT        INT_UART0
	#define UART_GPIO_BASE  GPIO_PORTA_BASE
	#ifdef CPU_CM3_AT91SAM3U
		#define UART_PINS       (BV(12) | BV(11))
	#else
		#define UART_PINS       (BV(10) | BV(9))
	#endif
#elif (CONFIG_KDEBUG_PORT == 1) && !defined(CPU_CM3_AT91SAM3U)
	#define UART_BASE       UART1_BASE
	#define UART_INT        INT_UART1
	#define UART_GPIO_BASE  GPIO_PORTB_BASE
	#define UART_PINS       (BV(3) | BV(2))
#else
	#error "UART port not supported in this board"
#endif

// TODO: refactor serial simple functions and use them, see lm3s kdebug
#define KDBG_WAIT_READY()     while (!(HWREG(UART_BASE + UART_SR) & UART_SR_TXRDY)) {}
#define KDBG_WAIT_TXDONE()    while (!(HWREG(UART_BASE + UART_SR) & UART_SR_TXEMPTY)) {}

#define KDBG_WRITE_CHAR(c)    do { HWREG(UART_BASE + UART_THR) = (c); } while(0)

/* Debug unit is used only for debug purposes so does not generate interrupts. */
#define KDBG_MASK_IRQ(old)    do { (void)old; } while(0)

/* Debug unit is used only for debug purposes so does not generate interrupts. */
#define KDBG_RESTORE_IRQ(old) do { (void)old; } while(0)

typedef uint32_t kdbg_irqsave_t;


INLINE void kdbg_hw_init(void)
{
	/* Disable PIO mode and set appropriate UART pins peripheral mode */
	HWREG(UART_GPIO_BASE + GPIO_PDR) = UART_PINS;
	HWREG(UART_GPIO_BASE + GPIO_ABCDSR1) &= ~UART_PINS;
	HWREG(UART_GPIO_BASE + GPIO_ABCDSR2) &= ~UART_PINS;

	/* Enable the peripheral clock */
	PMC_PCER_R = UART_INT;

	/* Reset and disable receiver & transmitter */
	HWREG(UART_BASE + UART_CR) = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

	/* Set mode: normal, no parity */
	HWREG(UART_BASE + UART_MR) = UART_MR_PAR_NO;

	/* Set baud rate */
	HWREG(UART_BASE + UART_BRGR) = CPU_FREQ / CONFIG_KDEBUG_BAUDRATE / 16;

	/* Enable receiver & transmitter */
	HWREG(UART_BASE + UART_CR) = UART_CR_RXEN | UART_CR_TXEN;
}

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
 * Copyright 2011 Develer S.r.l. (http://www.develer.com/)
 *
 * -->
 *
 * \brief DAC hardware-specific implementation
 *
 * \author Daniele Basile <asterix@develer.com>
 */


#include "cfg/cfg_dac.h"

#include <cfg/macros.h>
#include <cfg/compiler.h>

// Define log settings for cfg/log.h.
#define LOG_LEVEL         DAC_LOG_LEVEL
#define LOG_FORMAT        DAC_LOG_FORMAT
#include <cfg/log.h>

#include <drv/dac.h>

#include <drv/irq_cm3.h>

#include <io/cm3.h>

#include <string.h>



static int sam3x_dac_write(DacContext *ctx, unsigned channel, uint16_t sample)
{
	ASSERT(channel <= 1);

	DACC_MR |= (channel << DACC_USER_SEL_SHIFT) & DACC_USER_SEL_MASK;
	DACC_CHER |= BV(channel);

	DACC_CDR = sample ;

	return 0;
}

static void sam3x_dac_setCh(struct DacContext *ctx, uint32_t mask)
{
}

static void sam3x_dac_setSampleRate(struct DacContext *ctx, uint32_t rate)
{
}

static void sam3x_dac_conversion(struct DacContext *ctx, void *buf, size_t len)
{
	DACC_TPR = (uint32_t)buf ;
	DACC_TCR = len;
	DACC_PTCR |= BV(DACC_PTCR_TXTEN);
}

static bool sam3x_dac_isFinished(struct DacContext *ctx)
{
}

static void sam3x_dac_start(struct DacContext *ctx, void *buf, size_t len, size_t slicelen)
{
}

static void sam3x_dac_stop(struct DacContext *ctx)
{
}


void dac_init(struct DacContext *ctx)
{
	ctx->write = sam3x_dac_write;
	ctx->setCh = sam3x_dac_setCh;
	ctx->setSampleRate = sam3x_dac_setSampleRate;
	ctx->conversion = sam3x_dac_conversion;
	ctx->isFinished = sam3x_dac_isFinished;
	ctx->start = sam3x_dac_start;
	ctx->stop = sam3x_dac_stop;


	/* Clock ADC peripheral */
	pmc_periphEnable(DACC_ID);

	DACC_CR |= BV(DACC_SWRST);
	DACC_MR = 0;

	/* Refresh period */
	DACC_MR |= (16 << DACC_REFRESH_SHIFT) & DACC_REFRESH_MASK;
	/* Start up */
	DACC_MR |= (DACC_MR_STARTUP_0 << DACC_STARTUP_SHIFT) & DACC_STARTUP_MASK;

}

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
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 *
 * -->
 *
 * \brief LM75 sensor temperature family.
 *
 * \author Daniele Basile <asterix@develer.com>
 *
 * $WIZ$ module_name = "lm75"
 * $WIZ$ module_depends = "timer", "i2c"
 * $WIZ$ module_configuration = "bertos/cfg/cfg_lm75.h"
 * $WIZ$ module_hw = "bertos/hw/hw_lm75.h"
 */

#ifndef DRV_LM75_H
#define DRV_LM75_H

#include <cpu/types.h>

#include <drv/ntc.h> // Macro and data type to manage celsius degree

#define LM75_ADDRESS_BYTE    0x91
#define LM75_PAD_BYTE        0x0

typedef uint8_t addr_t;

deg_t lm75_read(addr_t sens_addr);

void lm75_init(void);

#endif /* DRV_LM75_H */

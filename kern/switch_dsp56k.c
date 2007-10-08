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
 *
 * -->
 *
 * \version $Id$
 *
 * \author Giovanni Bajo <rasky@develer.com>
 *
 * \brief DSP5680x task switching support
 */

/*#*
 *#* $Log$
 *#* Revision 1.7  2006/07/19 12:56:27  bernie
 *#* Convert to new Doxygen style.
 *#*
 *#* Revision 1.6  2006/02/24 01:17:05  bernie
 *#* Update for new emulator.
 *#*
 *#* Revision 1.5  2005/11/04 16:20:02  bernie
 *#* Fix reference to README.devlib in header.
 *#*
 *#* Revision 1.4  2004/08/25 14:12:09  rasky
 *#* Aggiornato il comment block dei log RCS
 *#*
 *#* Revision 1.3  2004/07/30 14:24:16  rasky
 *#* Task switching con salvataggio perfetto stato di interrupt (SR)
 *#* Kernel monitor per dump informazioni su stack dei processi
 *#*
 *#* Revision 1.2  2004/06/03 11:27:09  bernie
 *#* Add dual-license information.
 *#*
 *#* Revision 1.1  2004/05/23 17:27:00  bernie
 *#* Import kern/ subdirectory.
 *#*
 *#*/

void asm_switch_context(void ** new_sp/*R2*/, void ** save_sp/*R3*/);
asm void asm_switch_context(void ** new_sp, void ** save_sp)
{
	lea   (SP)+

	; From the manual:
	; The compiler uses page 0 address locations X: 0x0030 - 0x003F as register
	; variables. Frequently accessed local variables are assigned to the page 0
	; registers instead of to stack locations so that load and store instructions
	; are shortened. Addresses X: 0x0030 - 0x0037 (page 0 registers MR0-MR7) are
	; volatile registers and can be overwritten. The remaining registers (page 0
	; registers MR8-MR15) are treated as non-volatile and, if used by a routine,
	; must be saved on entry and restored on exit.
	;
	; So, register 0x30-0x37 are caller-save, while 0x38-0x3F are callee-save.
	move  x:<$38,y1
	move  y1,x:(SP)+
	move  x:<$39,y1
	move  y1,x:(SP)+
	move  x:<$3A,y1
	move  y1,x:(SP)+
	move  x:<$3B,y1
	move  y1,x:(SP)+
	move  x:<$3C,y1
	move  y1,x:(SP)+
	move  x:<$3D,y1
	move  y1,x:(SP)+
	move  x:<$3E,y1
	move  y1,x:(SP)+
	move  x:<$3F,y1
	move  y1,x:(SP)

	;
	; Switch stacks
	nop
	move SP, x:(R3)
	nop
	move x:(R2), SP
	nop

	;
	; restore all saved registers
	;
	pop   y1
	move  y1,x:<$3F
	pop   y1
	move  y1,x:<$3E
	pop   y1
	move  y1,x:<$3D
	pop   y1
	move  y1,x:<$3C
	pop   y1
	move  y1,x:<$3B
	pop   y1
	move  y1,x:<$3A
	pop   y1
	move  y1,x:<$39
	pop   y1
	move  y1,x:<$38

	; SR is already pushed on the stack (normal call context). Use RTI to restore
	; it, so that interrupt status is preserved across the tasks.
	rti
}

int asm_switch_version(void);
int asm_switch_version(void)
{
	return 1;
}

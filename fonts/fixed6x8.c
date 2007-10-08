/**
 * \file
 * <!--
 * Copyright 2003, 2004, 2005 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2001 Bernardo Innocenti <bernie@codewiz.org>
 * This file is part of DevLib - See README.devlib for information.
 * -->
 *
 * \version $Id$
 *
 * \author Bernardo Innocenti <bernie@develer.com>
 *
 * \brief Font 8x6 IBM-PC 8bit
 */

/*#*
 *#* $Log$
 *#* Revision 1.2  2006/07/19 12:56:26  bernie
 *#* Convert to new Doxygen style.
 *#*
 *#* Revision 1.1  2006/05/27 22:30:22  bernie
 *#* Move 6x8 font to fonts/.
 *#*
 *#* Revision 1.3  2006/02/10 12:29:05  bernie
 *#* Add multiple font support in bitmaps.
 *#*
 *#* Revision 1.2  2005/11/04 18:17:45  bernie
 *#* Fix header guards and includes for new location of gfx module.
 *#*
 *#* Revision 1.1  2005/11/04 18:11:35  bernie
 *#* Move graphics stuff from mware/ to gfx/.
 *#*
 *#* Revision 1.6  2005/11/04 16:20:02  bernie
 *#* Fix reference to README.devlib in header.
 *#*
 *#* Revision 1.5  2005/01/08 09:20:38  bernie
 *#* Add missing type in declaration.
 *#*
 *#* Revision 1.4  2004/12/31 16:42:55  bernie
 *#* Sanitize for non-Harvard processors.
 *#*
 *#* Revision 1.3  2004/08/25 14:12:09  rasky
 *#* Aggiornato il comment block dei log RCS
 *#*
 *#* Revision 1.2  2004/06/03 11:27:09  bernie
 *#* Add dual-license information.
 *#*
 *#* Revision 1.1  2004/05/23 15:43:16  bernie
 *#* Import mware modules.
 *#*/

#include "font.h"

static const PROGMEM uint8_t default_font_glyphs[256 * 6] =
{
/* 0x00 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*   */
	0x3E, 0x45, 0x51, 0x45, 0x3E, 0x00, /*   */
	0x3E, 0x7B, 0x6F, 0x7B, 0x3E, 0x00, /*   */
	0x1C, 0x3E, 0x7C, 0x3E, 0x1C, 0x00, /*   */
	0x08, 0x1C, 0x3E, 0x1C, 0x08, 0x00, /*   */
	0x18, 0x5A, 0x67, 0x5A, 0x18, 0x00, /*   */
	0x0C, 0x5E, 0x6F, 0x5E, 0x0C, 0x00, /*   */
	0x08, 0x1C, 0x3E, 0x1C, 0x08, 0x00, /*   */
/* 0x08 */
	0x77, 0x63, 0x41, 0x63, 0x77, 0x00, /*   */
	0x18, 0x3C, 0x66, 0x3C, 0x18, 0x00, /*   */
	0x77, 0x63, 0x41, 0x63, 0x77, 0x00, /*   */
	0x20, 0x50, 0x5A, 0x56, 0x2E, 0x00, /*   */
	0x06, 0x29, 0x79, 0x29, 0x06, 0x00, /*   */
	0x60, 0x60, 0x7F, 0x05, 0x07, 0x00, /*   */
	0x18, 0x1F, 0x01, 0x61, 0x7F, 0x00, /*   */
	0x15, 0x0E, 0x1B, 0x0E, 0x15, 0x00, /*   */
/* 0x10 */
	0x00, 0x08, 0x1C, 0x3E, 0x08, 0x08, /*   */
	0x7F, 0x3E, 0x1C, 0x08, 0x00, 0x00, /*   */
	0x14, 0x36, 0x7F, 0x36, 0x14, 0x00, /*   */
	0x00, 0x5F, 0x00, 0x5F, 0x00, 0x00, /*   */
	0x02, 0x05, 0x7F, 0x01, 0x7F, 0x00, /*   */
	0x20, 0x4A, 0x55, 0x29, 0x02, 0x00, /*   */
	0x60, 0x60, 0x60, 0x60, 0x60, 0x00, /*   */
	0x54, 0x76, 0x7F, 0x76, 0x54, 0x00, /*   */
/* 0x18 */
	0x08, 0x0C, 0x7E, 0x0C, 0x08, 0x00, /*   */
	0x10, 0x30, 0x7E, 0x30, 0x10, 0x00, /*   */
	0x08, 0x08, 0x3E, 0x1C, 0x08, 0x00, /*   */
	0x08, 0x1C, 0x3E, 0x08, 0x08, 0x00, /*   */
	0x1C, 0x10, 0x10, 0x10, 0x10, 0x00, /*   */
	0x08, 0x1C, 0x08, 0x1C, 0x08, 0x00, /*   */
	0x18, 0x1C, 0x1E, 0x1C, 0x18, 0x00, /*   */
	0x0C, 0x1C, 0x3C, 0x1C, 0x0C, 0x00, /*   */
/* 0x20 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*   */
	0x00, 0x00, 0x4F, 0x00, 0x00, 0x00, /* ! */
	0x00, 0x07, 0x00, 0x07, 0x00, 0x00, /* " */
	0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00, /* # */
	0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00, /* $ */
	0x63, 0x13, 0x08, 0x64, 0x63, 0x00, /* % */
	0x36, 0x49, 0x55, 0x22, 0x50, 0x00, /* & */
	0x00, 0x05, 0x03, 0x00, 0x00, 0x00, /* ' */
/* 0x28 */
	0x00, 0x1C, 0x22, 0x41, 0x00, 0x00, /* ( */
	0x00, 0x00, 0x41, 0x22, 0x1C, 0x00, /* ) */
	0x14, 0x08, 0x3E, 0x08, 0x14, 0x00, /* * */
	0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, /* + */
	0x00, 0x50, 0x30, 0x00, 0x00, 0x00, /* , */
	0x08, 0x08, 0x08, 0x08, 0x08, 0x00, /* - */
	0x00, 0x60, 0x60, 0x00, 0x00, 0x00, /* . */
	0x20, 0x10, 0x08, 0x04, 0x02, 0x00, /* / */
/* 0x30 */
	0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00, /* 0 */
	0x00, 0x42, 0x7F, 0x40, 0x00, 0x00, /* 1 */
	0x62, 0x51, 0x49, 0x49, 0x46, 0x00, /* 2 */
	0x21, 0x41, 0x45, 0x4B, 0x31, 0x00, /* 3 */
	0x18, 0x14, 0x12, 0x7F, 0x10, 0x00, /* 4 */
	0x27, 0x45, 0x45, 0x45, 0x39, 0x00, /* 5 */
	0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00, /* 6 */
	0x01, 0x71, 0x09, 0x05, 0x03, 0x00, /* 7 */
/* 0x38 */
	0x36, 0x49, 0x49, 0x49, 0x36, 0x00, /* 8 */
	0x06, 0x49, 0x49, 0x29, 0x1E, 0x00, /* 9 */
	0x00, 0x36, 0x36, 0x00, 0x00, 0x00, /* : */
	0x00, 0x56, 0x36, 0x00, 0x00, 0x00, /* ; */
	0x08, 0x14, 0x22, 0x41, 0x00, 0x00, /* < */
	0x14, 0x14, 0x14, 0x14, 0x14, 0x00, /* = */
	0x00, 0x41, 0x22, 0x14, 0x08, 0x00, /* > */
	0x02, 0x01, 0x51, 0x09, 0x06, 0x00, /* ? */
/* 0x40 */
	0x32, 0x49, 0x79, 0x41, 0x3E, 0x00, /* @ */
	0x7E, 0x09, 0x09, 0x09, 0x7E, 0x00, /* A */
	0x7F, 0x49, 0x49, 0x49, 0x36, 0x00, /* B */
	0x3E, 0x41, 0x41, 0x41, 0x22, 0x00, /* C */
	0x7F, 0x41, 0x41, 0x22, 0x1C, 0x00, /* D */
	0x7F, 0x49, 0x49, 0x49, 0x41, 0x00, /* E */
	0x7F, 0x09, 0x09, 0x09, 0x01, 0x00, /* F */
	0x3E, 0x41, 0x49, 0x49, 0x7A, 0x00, /* G */
/* 0x48 */
	0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00, /* H */
	0x00, 0x41, 0x7F, 0x41, 0x00, 0x00, /* I */
	0x20, 0x40, 0x41, 0x3F, 0x01, 0x00, /* J */
	0x7F, 0x08, 0x14, 0x22, 0x41, 0x00, /* K */
	0x7F, 0x40, 0x40, 0x40, 0x40, 0x00, /* L */
	0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00, /* M */
	0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00, /* N */
	0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00, /* O */
/* 0x50 */
	0x7F, 0x09, 0x09, 0x09, 0x06, 0x00, /* P */
	0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00, /* Q */
	0x7F, 0x09, 0x19, 0x29, 0x46, 0x00, /* R */
	0x26, 0x49, 0x49, 0x49, 0x32, 0x00, /* S */
	0x01, 0x01, 0x7F, 0x01, 0x01, 0x00, /* T */
	0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00, /* U */
	0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00, /* V */
	0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00, /* W */
/* 0x58 */
	0x63, 0x14, 0x08, 0x14, 0x63, 0x00, /* X */
	0x07, 0x08, 0x70, 0x08, 0x07, 0x00, /* Y */
	0x61, 0x51, 0x49, 0x45, 0x43, 0x00, /* Z */
	0x00, 0x7F, 0x41, 0x41, 0x00, 0x00, /* [ */
	0x02, 0x04, 0x08, 0x10, 0x20, 0x00, /* \ */
	0x00, 0x41, 0x41, 0x7F, 0x00, 0x00, /* ] */
	0x04, 0x02, 0x01, 0x02, 0x04, 0x00, /* ^ */
	0x40, 0x40, 0x40, 0x40, 0x40, 0x00, /* _ */
/* 0x60 */
	0x00, 0x01, 0x02, 0x04, 0x00, 0x00, /* ` */
	0x00, 0x74, 0x54, 0x54, 0x78, 0x00, /* a */
	0x7F, 0x44, 0x44, 0x44, 0x38, 0x00, /* b */
	0x38, 0x44, 0x44, 0x44, 0x28, 0x00, /* c */
	0x38, 0x44, 0x44, 0x44, 0x7F, 0x00, /* d */
	0x38, 0x54, 0x54, 0x54, 0x18, 0x00, /* e */
	0x08, 0x7E, 0x09, 0x01, 0x02, 0x00, /* f */
	0x08, 0x54, 0x54, 0x54, 0x3C, 0x00, /* g */
/* 0x68 */
	0x7F, 0x04, 0x04, 0x04, 0x78, 0x00, /* h */
	0x00, 0x44, 0x7D, 0x40, 0x00, 0x00, /* i */
	0x20, 0x40, 0x44, 0x3D, 0x00, 0x00, /* j */
	0x7F, 0x10, 0x28, 0x44, 0x00, 0x00, /* k */
	0x00, 0x41, 0x7F, 0x40, 0x00, 0x00, /* l */
	0x7C, 0x04, 0x18, 0x04, 0x78, 0x00, /* m */
	0x7C, 0x08, 0x04, 0x04, 0x78, 0x00, /* n */
	0x38, 0x44, 0x44, 0x44, 0x38, 0x00, /* o */
/* 0x70 */
	0x7C, 0x14, 0x14, 0x14, 0x08, 0x00, /* p */
	0x08, 0x14, 0x14, 0x14, 0x7C, 0x00, /* q */
	0x7C, 0x08, 0x04, 0x04, 0x08, 0x00, /* r */
	0x48, 0x54, 0x54, 0x54, 0x24, 0x00, /* s */
	0x04, 0x3F, 0x44, 0x40, 0x20, 0x00, /* t */
	0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00, /* u */
	0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00, /* v */
	0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00, /* w */
/* 0x78 */
	0x44, 0x28, 0x10, 0x28, 0x44, 0x00, /* x */
	0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00, /* y */
	0x44, 0x64, 0x54, 0x4C, 0x44, 0x00, /* z */
	0x00, 0x08, 0x36, 0x41, 0x00, 0x00, /* { */
	0x00, 0x00, 0x77, 0x00, 0x00, 0x00, /* | */
	0x00, 0x41, 0x36, 0x08, 0x00, 0x00, /* } */
	0x02, 0x01, 0x02, 0x01, 0x00, 0x00, /* ~ */
	0x70, 0x48, 0x44, 0x48, 0x70, 0x00, /*  */
/* 0x80 */
	0x38, 0xC4, 0xC4, 0x44, 0x28, 0x00, /* � */
	0x3A, 0x40, 0x40, 0x20, 0x7A, 0x00, /* � */
	0x38, 0x54, 0x54, 0x55, 0x19, 0x00, /* � */
	0x22, 0x55, 0x55, 0x55, 0x78, 0x00, /* � */
	0x20, 0x55, 0x54, 0x54, 0x79, 0x00, /* � */
	0x21, 0x75, 0x55, 0x54, 0x78, 0x00, /* � */
	0x20, 0x74, 0x57, 0x54, 0x78, 0x00, /* � */
	0x08, 0x54, 0x54, 0x74, 0x14, 0x00, /* � */
/* 0x88 */
	0x3A, 0x55, 0x55, 0x55, 0x1A, 0x00, /* � */
	0x39, 0x54, 0x54, 0x55, 0x18, 0x00, /* � */
	0x39, 0x55, 0x55, 0x54, 0x18, 0x00, /* � */
	0x00, 0x45, 0x7C, 0x41, 0x00, 0x00, /* � */
	0x02, 0x45, 0x7D, 0x42, 0x00, 0x00, /* � */
	0x01, 0x45, 0x7D, 0x40, 0x00, 0x00, /* � */
	0x79, 0x14, 0x12, 0x14, 0x79, 0x00, /* � */
	0x70, 0x2B, 0x2B, 0x2B, 0x70, 0x00, /* � */
/* 0x90 */
	0x7C, 0x54, 0x55, 0x55, 0x45, 0x00, /* � */
	0x20, 0x54, 0x38, 0x54, 0x48, 0x00, /* � */
	0x7E, 0x09, 0x7F, 0x49, 0x49, 0x00, /* � */
	0x32, 0x49, 0x49, 0x49, 0x32, 0x00, /* � */
	0x32, 0x48, 0x48, 0x48, 0x32, 0x00, /* � */
	0x32, 0x4A, 0x4A, 0x48, 0x30, 0x00, /* � */
	0x3A, 0x41, 0x41, 0x21, 0x7A, 0x00, /* � */
	0x3A, 0x42, 0x42, 0x20, 0x78, 0x00, /* � */
/* 0x98 */
	0x0D, 0x50, 0x50, 0x50, 0x3D, 0x00, /* � */
	0x19, 0x24, 0x42, 0x24, 0x19, 0x00, /* � */
	0x3D, 0x40, 0x40, 0x40, 0x3D, 0x00, /* � */
	0x18, 0x24, 0x7E, 0x24, 0x24, 0x00, /* � */
	0x28, 0x5E, 0x29, 0x42, 0x20, 0x00, /* � */
	0x09, 0x2A, 0x7C, 0x2A, 0x09, 0x00, /* � */
	0x7F, 0x05, 0x15, 0x3D, 0x52, 0x00, /* � */
	0x20, 0x48, 0x3E, 0x09, 0x02, 0x00, /* � */
/* 0xa0 */
	0x20, 0x74, 0x55, 0x55, 0x79, 0x00, /* � */
	0x01, 0x45, 0x7D, 0x40, 0x00, 0x00, /* � */
	0x30, 0x48, 0x4A, 0x4A, 0x32, 0x00, /* � */
	0x38, 0x40, 0x42, 0x22, 0x7A, 0x00, /* � */
	0x7A, 0x12, 0x0A, 0x0A, 0x72, 0x00, /* � */
	0x7D, 0x09, 0x11, 0x21, 0x7D, 0x00, /* � */
	0x02, 0x15, 0x15, 0x12, 0x04, 0x00, /* � */
	0x02, 0x15, 0x15, 0x15, 0x02, 0x00, /* � */
/* 0xa8 */
	0x30, 0x48, 0x45, 0x40, 0x20, 0x00, /* � */
	0x00, 0x38, 0x08, 0x08, 0x08, 0x00, /* � */
	0x00, 0x08, 0x08, 0x08, 0x38, 0x00, /* � */
	0x0B, 0x04, 0x6A, 0x55, 0x48, 0x00, /* � */
	0x0B, 0x24, 0x32, 0x79, 0x20, 0x00, /* � */
	0x00, 0x00, 0x79, 0x00, 0x00, 0x00, /* � */
	0x08, 0x14, 0x2A, 0x14, 0x22, 0x00, /* � */
	0x22, 0x14, 0x2A, 0x14, 0x08, 0x00, /* � */
/* 0xb0 */
	0x2A, 0x55, 0x00, 0x2A, 0x55, 0x00, /* � */
	0x2A, 0x55, 0x2A, 0x55, 0x2A, 0x55, /* � */
	0x55, 0x2A, 0x7F, 0x55, 0x2A, 0x7F, /* � */
	0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, /* � */
	0x08, 0x08, 0xFF, 0x00, 0x00, 0x00, /* � */
	0x14, 0x14, 0xFF, 0x00, 0x00, 0x00, /* � */
	0x08, 0xFF, 0x00, 0xFF, 0x00, 0x00, /* � */
	0x08, 0xF8, 0x08, 0xF8, 0x00, 0x00, /* � */
/* 0xb8 */
	0x14, 0x14, 0xFC, 0x00, 0x00, 0x00, /* � */
	0x14, 0xF7, 0x00, 0xFF, 0x00, 0x00, /* � */
	0x00, 0xFF, 0x00, 0xFF, 0x00, 0x00, /* � */
	0x14, 0xF4, 0x04, 0xFC, 0x00, 0x00, /* � */
	0x14, 0x17, 0x10, 0x1F, 0x00, 0x00, /* � */
	0x08, 0x0F, 0x08, 0x0F, 0x00, 0x00, /* � */
	0x14, 0x14, 0x1F, 0x00, 0x00, 0x00, /* � */
	0x08, 0x08, 0xF8, 0x00, 0x00, 0x00, /* � */
/* 0xc0 */
	0x00, 0x00, 0x0F, 0x08, 0x08, 0x08, /* � */
	0x08, 0x08, 0x0F, 0x08, 0x08, 0x08, /* � */
	0x08, 0x08, 0xF8, 0x08, 0x08, 0x08, /* � */
	0x00, 0x00, 0xFF, 0x08, 0x08, 0x08, /* � */
	0x08, 0x08, 0x08, 0x08, 0x08, 0x08, /* � */
	0x08, 0x08, 0xFF, 0x08, 0x08, 0x08, /* � */
	0x00, 0x00, 0xFF, 0x14, 0x14, 0x14, /* � */
	0x00, 0xFF, 0x00, 0xFF, 0x08, 0x08, /* � */
/* 0xc8 */
	0x00, 0x1F, 0x10, 0x17, 0x14, 0x14, /* � */
	0x00, 0xFC, 0x04, 0xF4, 0x14, 0x14, /* � */
	0x28, 0x2F, 0x20, 0x2F, 0x28, 0x28, /* � */
	0x14, 0xF4, 0x04, 0xF4, 0x14, 0x14, /* � */
	0x00, 0xFF, 0x00, 0xF7, 0x14, 0x14, /* � */
	0x14, 0x14, 0x14, 0x14, 0x14, 0x14, /* � */
	0x14, 0xF7, 0x00, 0xF7, 0x14, 0x14, /* � */
	0x14, 0x14, 0xF7, 0x14, 0x14, 0x14, /* � */
/* 0xd0 */
	0x08, 0x0F, 0x08, 0x0F, 0x08, 0x08, /* � */
	0x14, 0x14, 0xF4, 0x14, 0x14, 0x14, /* � */
	0x08, 0x08, 0xF8, 0x08, 0x08, 0x08, /* � */
	0x08, 0x0F, 0x08, 0x0F, 0x08, 0x08, /* � */
	0x00, 0x00, 0x1F, 0x14, 0x14, 0x14, /* � */
	0x00, 0x00, 0xFC, 0x14, 0x14, 0x14, /* � */
	0x00, 0xF8, 0x08, 0xF8, 0x08, 0x08, /* � */
	0x08, 0xFF, 0x08, 0xFF, 0x08, 0x08, /* � */
/* 0xd8 */
	0x14, 0x14, 0xFF, 0x14, 0x14, 0x14, /* � */
	0x08, 0x08, 0x0F, 0x00, 0x00, 0x00, /* � */
	0x00, 0x00, 0xF8, 0x08, 0x08, 0x08, /* � */
	0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x00, /* � */
	0x70, 0x70, 0x70, 0x70, 0x70, 0x00, /* � */
	0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, /* � */
	0x00, 0x00, 0x00, 0x7F, 0x7F, 0x00, /* � */
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x00, /* � */
/* 0xe0 */
	0x30, 0x48, 0x48, 0x30, 0x48, 0x00, /* � */
	0x7E, 0x11, 0x25, 0x25, 0x1A, 0x00, /* � */
	0x7E, 0x02, 0x02, 0x02, 0x06, 0x00, /* � */
	0x04, 0x7C, 0x04, 0x7C, 0x04, 0x00, /* � */
	0x41, 0x63, 0x55, 0x49, 0x63, 0x00, /* � */
	0x3C, 0x42, 0x4A, 0x4A, 0x31, 0x00, /* � */
	0x40, 0x7C, 0x20, 0x20, 0x1C, 0x00, /* � */
	0x08, 0x04, 0x7C, 0x08, 0x04, 0x00, /* � */
/* 0xe8 */
	0x49, 0x55, 0x77, 0x55, 0x49, 0x00, /* � */
	0x1C, 0x2A, 0x49, 0x2A, 0x1C, 0x00, /* � */
	0x4C, 0x72, 0x02, 0x72, 0x4C, 0x00, /* � */
	0x30, 0x4A, 0x45, 0x49, 0x31, 0x00, /* � */
	0x18, 0x24, 0x18, 0x24, 0x18, 0x00, /* � */
	0x5C, 0x72, 0x2A, 0x27, 0x1D, 0x00, /* � */
	0x1C, 0x2A, 0x49, 0x49, 0x00, 0x00, /* � */
	0x7E, 0x01, 0x01, 0x01, 0x7E, 0x00, /* � */
/* 0xf0 */
	0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 0x00, /* � */
	0x24, 0x24, 0x2E, 0x24, 0x24, 0x00, /* � */
	0x40, 0x51, 0x4A, 0x44, 0x00, 0x00, /* � */
	0x44, 0x4A, 0x51, 0x40, 0x00, 0x00, /* � */
	0x00, 0x00, 0xFE, 0x01, 0x02, 0x00, /* � */
	0x20, 0x40, 0x3F, 0x00, 0x00, 0x00, /* � */
	0x08, 0x08, 0x2A, 0x08, 0x08, 0x00, /* � */
	0x24, 0x12, 0x24, 0x12, 0x00, 0x00, /* � */
/* 0xf8 */
	0x06, 0x09, 0x09, 0x09, 0x06, 0x00, /* � */
	0x00, 0x18, 0x18, 0x00, 0x00, 0x00, /* � */
	0x00, 0x10, 0x10, 0x00, 0x00, 0x00, /* � */
	0x10, 0x30, 0x7F, 0x01, 0x01, 0x00, /* � */
	0x01, 0x0E, 0x01, 0x01, 0x0E, 0x00, /* � */
	0x0A, 0x09, 0x0D, 0x0A, 0x00, 0x00, /* � */
	0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x00, /* � */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00  /* � */
};

struct Font default_font =
{
	/* .glyph = */  default_font_glyphs,
	/* .width = */  6,
	/* .height = */ 8
};


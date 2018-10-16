/*******************************************************************************
 *
 * Copyright (C) 2004-2014 Emulex. All rights reserved.
 * EMULEX is a trademark of Emulex.
 * www.emulex.com
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published by the
 * Free Software Foundation.
 * This program is distributed in the hope that it will be useful. ALL EXPRESS
 * OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED
 * WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 * NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS
 * ARE HELD TO BE LEGALLY INVALID. See the GNU General Public License for more
 * details, a copy of which can be found in the file COPYING included
 * with this package.
 *
 ********************************************************************************/

/// Tile snoop register offsets

#define TSCTL           0x0
#define TSSTS           0x4
#define INTRCLMNLO      0x8
#define INTRCLMNHI      0xC
#define INTRLINELO      0x10
#define INTRLINEHI      0x14
#define TILECNT         0x18
#define INTRCNT         0x1c
#define STARTOFFSET     0x20
#define TEXTLIMIT       0x24

/// Bit definitions

////// TSCTL Register /////
#define tse_en          (1<<0)
#define mgamode         (1<<1)
#define tse_bpp             2
#define tse_lpt             4
#define tse_cpt             6
#define tse_irq_en      (1<<8)
#define tse_fiq_en      (1<<9)
#define initscreen0     (1<<13)
#define initscreen1     (1<<14)
#define ownscreen       (1<<15)
#define tse_offset          16

///// TSSTS Registers //////
#define ts_intr0 	(1<<0)
#define ts_intr1 	(1<<1)
#define ascii_mod	(1<<2)
#define attr_mod	(1<<3)
#define font_mod	(1<<4)
#define mgamode_sts	(1<<8)
#define scroff	  	(1<<9)

// FBOFFSET Defines
#define FBOFFSET_640    (0x280) 
#define FBOFFSET_800    (0x320)
#define FBOFFSET_1024   (0x400)
#define FBOFFSET_1280   (0x500)
#define FBOFFSET_1600   (0x640)
#define FBOFFSET_1920   (0x780)

#define TSE_1BPP        (0x0) 
#define TSE_2BPP        (0x1)
#define TSE_4BPP        (0x2)
#define TSE_8BPP        (0x3)

#define TSE_16LPT      (0x0) 
#define TSE_32LPT      (0x1)
#define TSE_64LPT      (0x2)
#define TSE_128LPT     (0x3)

#define TSE_16CPT      (0x0)
#define TSE_32CPT      (0x1)
#define TSE_64CPT      (0x2)
#define TSE_128CPT     (0x3)

typedef struct TSE_REGS 
	{
	 volatile unsigned int  tsctl ;
	 volatile unsigned int  tssts ;
	 volatile unsigned int  tileintrclmn_lo;
	 volatile unsigned int  tileintrclmn_hi;
	 volatile unsigned int  tileintrline_lo;
	 volatile unsigned int  tileintrline_hi;
	 volatile unsigned int  tilecnt;
	 volatile unsigned int  startoffset;
	 volatile unsigned int  ts_intrcnt;
	 volatile unsigned int  textlimit;
	 } TSE_REGS;

#define FGB_TS_REGS             ((volatile TILE_SNOOP_REGS *)(FGBBASE+0x400))
#define FGB_TSE_REGS            ((volatile TSE_REGS *)(FGBBASE+0x400))
#define FGB_TS_BASE             (FGBBASE+0x400)
#define FGB_TSE_SCRN             (FGBBASE+0x600)

extern void tse_enable (void);
extern void tse_set_mgamode (void);
extern void tse_initscreen0 (void);
extern void tse_initscreen1 (void);
extern void tse_set_fboffset (tU16); 
extern void tse_set_bpp (tU8 );
extern void tse_set_lpt (tU8 );
extern void tse_set_cpt (tU8 );
extern void tse_set_ownscrn_zero (void);
extern void tse_set_ownscrn_one (void);
extern void tse_flip_ownscrn (void);
extern void tse_set_irq_handler (tU32); 
extern void tse_set_irq_handler (tU32); 
extern void tse_program_upperlimit (tU32);
extern tU8 tse_compare_snoopram (tU32, tU32, tU8);
extern tU8 tse_compare_tilecnt (tU16);
extern tU8 tse_compare_snoopmap (tU32, tU32, tU32, tU32);
extern void init_g200_ahb2pci();
extern void Ahb2pciEnable();

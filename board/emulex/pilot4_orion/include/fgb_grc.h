/*******************************************************************************
 *
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

#define GRC_PLTRAM_DATA_OFF     0x0000
#define GRC_CURSORCOL_DATA_OFF  0x0400
#define GRC_CURSORCTL_DATA_OFF  0x0440
#define GRC_CRTC_DATA_OFF       0x0480
#define GRC_CRTCEXT_DATA_OFF    0x04C0
#define GRC_SEQ_DATA_OFF        0x04C8
#define GRC_GCTL_DATA_OFF       0x04D0
#define GRC_ATTR_DATA_OFF       0x04E0
#define GRC_CTL_OFF             0x0500

//////////// GRC CONTROL OFFSETS ////////////////
//typedef struct GRC_CTL_IRQEN 
//	{
//	 volatile unsigned int palette_en	:1;  /* [0 : 0] */
//	 volatile unsigned int attr_en	        :1;  /* [1 : 1] */
//	 volatile unsigned int seq_en	        :1;  /* [2 : 2] */
//	 volatile unsigned int gctl_en		:1; /*  [3 : 3] */
//	 volatile unsigned int crtc_en		:1; /*  [4 : 4] */
//	 volatile unsigned int crtcext_en	:1; /*  [5 : 5] */
//	 volatile unsigned int pltram_en	:1; /*  [6 : 6] */
//	 volatile unsigned int xcurcol_en	:1; /*  [7 : 7] */
//	 volatile unsigned int xcurctl_en	:1; /*  [8 : 8] */
//	 volatile unsigned int curpos_en	:1; /*  [9 : 9] */
//	 volatile unsigned int rsvd		:1; /*  [10:31] */
//	 } GRC_CTL_IRQEN;
//
//typedef struct GRC_CTL_FIQEN 
//	{	 
//         volatile unsigned int palette_en	:1;  /* [0 : 0] */
//	 volatile unsigned int attr_en	        :1;  /* [1 : 1] */
//	 volatile unsigned int seq_en	        :1;  /* [2 : 2] */
//	 volatile unsigned int gctl_en		:1; /*  [3 : 3] */
//	 volatile unsigned int crtc_en		:1; /*  [4 : 4] */
//	 volatile unsigned int crtcext_en	:1; /*  [5 : 5] */
//	 volatile unsigned int pltram_en	:1; /*  [6 : 6] */
//	 volatile unsigned int xcurcol_en	:1; /*  [7 : 7] */
//	 volatile unsigned int xcurctl_en	:1; /*  [8 : 8] */
//	 volatile unsigned int curpos_en	:1; /*  [9 : 9] */
//	 volatile unsigned int rsvd		:1; /*  [10:31] */
//	 } GRC_CTL_FIQEN;
//
//typedef struct GRC_CTL_INTSTS 
//	{
//         volatile unsigned int palette_sts	:1;  /* [0 : 0] */
//	 volatile unsigned int attr_sts	        :1;  /* [1 : 1] */
//	 volatile unsigned int seq_sts	        :1;  /* [2 : 2] */
//	 volatile unsigned int gctl_sts		:1; /*  [3 : 3] */
//	 volatile unsigned int crtc_sts		:1; /*  [4 : 4] */
//	 volatile unsigned int crtcext_sts	:1; /*  [5 : 5] */
//	 volatile unsigned int pltram_sts	:1; /*  [6 : 6] */
//	 volatile unsigned int xcurcol_sts	:1; /*  [7 : 7] */
//	 volatile unsigned int xcurctl_sts	:1; /*  [8 : 8] */
//	 volatile unsigned int curpos_sts	:1; /*  [9 : 9] */
//	 volatile unsigned int rsvd		:1; /*  [10:31] */
//	 } GRC_CTL_INTSTS;
//
//
//typedef union U_GRC_CTL_IRQEN { GRC_CTL_IRQEN s; unsigned int val; } u_grc_ctl_irqen ;
//typedef union U_GRC_CTL_FIQEN { GRC_CTL_FIQEN s; unsigned int val; } u_grc_ctl_fiqen ;
//typedef union U_GRC_CTL_INTSTS { GRC_CTL_INTSTS s; unsigned int val; } u_grc_ctl_intsts ;
//
//typedef struct GRC_CTL_REGS 
//	{
//	 volatile u_grc_ctl_irqen      irqen ;
//	 volatile u_grc_ctl_fiqen      fiqen ;
//	 volatile u_grc_ctl_intsts     intsts;
//	 volatile unsigned int         count;
//	 } GRC_CTL_REGS;


typedef struct GRCE_CTL_REGS 
	{
	 tVU32 irqen;
	 tVU32 fiqen;
	 tVU32 intsts;
	 tVU32 count;
	 } GRCE_CTL_REGS;


// Register Bit Definitions
// GRC IRQ EN CTL
#define palette_irq_en          (1<<0)
#define attr_irq_en             (1<<1)
#define seq_irq_en              (1<<2)
#define gctl_irq_en             (1<<3)
#define crtc_irq_en             (1<<4)
#define crtcext_irq_en          (1<<5)
#define pltram_irq_en           (1<<6)
#define xcurcol_irq_en          (1<<7)
#define xcurctl_irq_en          (1<<8)
#define curpos_irq_en           (1<<9)

// GRC FIQ EN CTL
#define palette_fiq_en          (1<<0)
#define attr_fiq_en             (1<<1)
#define seq_fiq_en              (1<<2)
#define gctl_fiq_en             (1<<3)
#define crtc_fiq_en             (1<<4)
#define crtcext_fiq_en          (1<<5)
#define pltram_fiq_en           (1<<6)
#define xcurcol_fiq_en          (1<<7)
#define xcurctl_fiq_en          (1<<8)
#define curpos_fiq_en           (1<<9)

// GRC STS
#define palette_sts          (1<<0)
#define attr_sts             (1<<1)
#define seq_sts              (1<<2)
#define gctl_sts             (1<<3)
#define crtc_sts             (1<<4)
#define crtcext_sts          (1<<5)
#define pltram_sts           (1<<6)
#define xcurcol_sts          (1<<7)
#define xcurctl_sts          (1<<8)
#define curpos_sts           (1<<9)

// FGB BASE Registers
#define  FGB_GRC                (FGBBASE+0x800)
#define  FGB_GRCCTL_REGS        ((volatile GRCE_CTL_REGS *)(FGB_GRC+ GRC_CTL_OFF))
#define  FGB_GRCATTR_REGS       (FGB_GRC + GRC_ATTR_DATA_OFF)
#define  FGB_GRCSEQ_REGS        (FGB_GRC + GRC_SEQ_DATA_OFF)
#define  FGB_GRCGCTL_REGS       (FGB_GRC + GRC_GCTL_DATA_OFF)
#define  FGB_GRCCRTC_REGS       (FGB_GRC + GRC_CRTC_DATA_OFF)
#define  FGB_GRCCRTCEXT_REGS    (FGB_GRC + GRC_CRTCEXT_DATA_OFF)
#define  FGB_GRCCURSORCOL_REGS  (FGB_GRC + GRC_CURSORCOL_DATA_OFF)
#define  FGB_GRCCURSORCTL_REGS  (FGB_GRC + GRC_CURSORCTL_DATA_OFF)
#define  FGB_GRCPLTRAM_REGS     (FGB_GRC + GRC_PLTRAM_DATA_OFF)

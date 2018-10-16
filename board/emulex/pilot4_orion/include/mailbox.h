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

#define MBDAT0		(KCS5BASE+0x20) 
#define MBDAT1		(KCS5BASE+0x21)
#define MBDAT2		(KCS5BASE+0x22)
#define MBDAT3		(KCS5BASE+0x23)
#define MBDAT4		(KCS5BASE+0x24)
#define MBDAT5		(KCS5BASE+0x25)
#define MBDAT6		(KCS5BASE+0x26)
#define MBDAT7		(KCS5BASE+0x27)
#define MBDAT8		(KCS5BASE+0x28)
#define MBDAT9		(KCS5BASE+0x29)
#define MBDAT10		(KCS5BASE+0x2A)
#define MBDAT11		(KCS5BASE+0x2B)
#define MBDAT12		(KCS5BASE+0x2C)
#define MBDAT13		(KCS5BASE+0x2D)
#define MBDAT14		(KCS5BASE+0x2E)
#define MBDAT15		(KCS5BASE+0x2F)
#define MBST0		(KCS5BASE+0x30)
#define MBST1		(KCS5BASE+0x31)
#define MBBINTREG	(KCS5BASE+0x32)
#define MBHINTREG	(KCS5BASE+0x33)
#define MBHIE0REG	(KCS5BASE+0x34)
#define MBHIE1REG	(KCS5BASE+0x35)
#define MBBIE0REG	(KCS5BASE+0x36)
#define MBBIE1REG	(KCS5BASE+0x37)
#define KCSISR		(KCS5BASE+0x38)
#define KCSIER		(KCS5BASE+0x39)
#define HISR		(KCS5BASE+0x3A)

// Misc defines
#define MBTXPTR		0
#define MBCTXPTR	4
#define MBTXSIZE	8
#define MBCTXSIZE	0xC

#define MBRXPTR		0x10
#define MBCRXPTR	0x14
#define MBRXSIZE	0x18
#define MBCRXSIZE	0x1C

// Mailbox Status Reg0
#define MBINT0			(1<<0) 
#define MBINT1			(1<<1)
#define MBINT2			(1<<2)
#define MBINT3			(1<<3)
#define MBINT4			(1<<4)
#define MBINT5			(1<<5)
#define MBINT6			(1<<6)
#define MBINT7			(1<<7)

// Mailbox Status Reg1
#define MBINT8			(1<<0) 
#define MBINT9			(1<<1)
#define MBINT10			(1<<2)
#define MBINT11			(1<<3)
#define MBINT12			(1<<4)
#define MBINT13			(1<<5)
#define MBINT14			(1<<6)
#define MBINT15			(1<<7)

// MBBINT
#define MBHINT			(1<<0)
#define MBBMK			(1<<1)
#define MBBMCINTSTS		(1<<7)

// MBHINT
#define MBBINT			(1<<0)
#define MBHMK			(1<<1)
#define MBHOSTINTSTS		(1<<7)

// Mailbox host interrupt enable reg0
#define MBHIE0			(1<<0) 
#define MBHIE1			(1<<1)
#define MBHIE2			(1<<2)
#define MBHIE3			(1<<3)
#define MBHIE4			(1<<4)
#define MBHIE5			(1<<5)
#define MBHIE6			(1<<6)
#define MBHIE7			(1<<7)

// Mailbox host interrupt enable reg1
#define MBHIE8			(1<<0) 
#define MBHIE9			(1<<1)
#define MBHIE10			(1<<2)
#define MBHIE11			(1<<3)
#define MBHIE12			(1<<4)
#define MBHIE13			(1<<5)
#define MBHIE14			(1<<6)
#define MBHIE15			(1<<7)

// Mailbox bmc interrupt enable reg0
#define MBBIE0			(1<<0)   
#define MBBIE1			(1<<1)
#define MBBIE2			(1<<2)
#define MBBIE3			(1<<3)
#define MBBIE4			(1<<4)
#define MBBIE5			(1<<5)
#define MBBIE6			(1<<6)
#define MBBIE7			(1<<7)

// Mailbox bmc interrupt enable reg1
#define MBBIE8			(1<<0) 
#define MBBIE9			(1<<1)
#define MBBIE10			(1<<2)
#define MBBIE11			(1<<3)
#define MBBIE12			(1<<4)
#define MBBIE13			(1<<5)
#define MBBIE14			(1<<6)
#define MBBIE15			(1<<7)

//KCS combined interrupt status reg
#define KCS1INTSTS		(1<<0) 
#define KCS2INTSTS		(1<<1)
#define KCS3INTSTS		(1<<2)
#define KCS4INTSTS		(1<<3)
#define KCS5INTSTS		(1<<4)
#define KCS5BTINTSTS		(1<<5)
#define KCS5SMICINTSTS		(1<<6)
#define KCS5MBINTSTS		(1<<7)

//KCS combined interrupt enable reg
#define KCS1INTEN		(1<<0) 
#define KCS2INTEN		(1<<1)
#define KCS3INTEN		(1<<2)
#define KCS4INTEN		(1<<3)
#define KCS5INTEN		(1<<4)
#define KCS5BTINTEN		(1<<5)
#define KCS5SMICINTEN		(1<<6)
#define KCS5MBINTEN		(1<<7)

// HISR bit definitions
#define KCS5HOSTINTSTS	        (1<<0)
#define BTHOSTINTSTS	        (1<<1)
#define SMISHOSTINTSTS	        (1<<2)
#define MAILBOXHOSTINTSTS	(1<<3)


typedef struct PII_MAILBOX_REGS 
{
  volatile unsigned char  mbdat0;
  volatile unsigned char  mbdat1;
  volatile unsigned char  mbdat2;
  volatile unsigned char  mbdat3;
  volatile unsigned char  mbdat4;
  volatile unsigned char  mbdat5;
  volatile unsigned char  mbdat6;
  volatile unsigned char  mbdat7;
  volatile unsigned char  mbdat8;
  volatile unsigned char  mbdat9;
  volatile unsigned char  mbdat10;
  volatile unsigned char  mbdat11;
  volatile unsigned char  mbdat12;
  volatile unsigned char  mbdat13;
  volatile unsigned char  mbdat14;
  volatile unsigned char  mbdat15;
  volatile unsigned char  mbst0;
  volatile unsigned char  mbst1;
  volatile unsigned char  mbbintreg;
  volatile unsigned char  mbhintreg;
  volatile unsigned char  mbhie0reg;
  volatile unsigned char  mbhie1reg;
  volatile unsigned char  mbbie0reg;
  volatile unsigned char  mbbie1reg;
  volatile unsigned char  kcsisr;
  volatile unsigned char  kcsier;
  volatile unsigned char hisr;
} PII_MAILBOX_REGS;

#define  MAILBOX_REGS	((volatile PII_MAILBOX_REGS *)(KCS5BASE+0x20))

extern tU8 IDREADWRITEC (tU16, tU16, tU8, tU8, tU8);
extern tU8 MBREADWRITEC (tU16, tU8, tU8);
extern void CLEARMAILBOXINTERRUPTS (void);
extern void MBWriteINTRCMDC (tU16, tU8, tU8);
extern tU32 MBREADWRITE32C (tU32, tU8, tU32);

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

//////////////////////////////////////////////////
/// Filename: swc.h
/// Section: Super IO SWC
/// Author: Saurabh Kasat
/// Description: Include file for SuperIO SWC module
//////////////////////////////////////////////////

#include "buffer.h"

#define WKSTS0		0
#define WKSTS1		1
#define WKEN0		2
#define WKEN1		3
#define WKCFG0		4
#define WKCFG1		5
#define WKSMIEN0	6
#define WKIRQEN0	8
#define SWCTL		9
#define SWCSTAT0	(0X0A) 
#define SWCSTAT1	(0X0B)
#define OCTL0S31	(0X0E)
#define OCTL1S31	(0X0F)
#define OCTL0S45	(0X10)
#define OCTL1S45	(0X11)
#define PBOUT0S31	(0X12)
#define PBOUT1S31	(0X13)
#define PBOUT0S45	(0X14)
#define PBOUT1S45	(0X15)
#define PS2CTL		(0X16)
#define PS2KEY0		(0X18)
#define PS2KEY1		(0X19)
#define PS2KEY2 	(0X1A)
#define PS2KEY3		(0X1B)
#define PS2KEY4		(0X1C)
#define PS2KEY5		(0X1D)
#define PS2KEY6		(0X1E)
#define PS2KEY7		(0X1F)
#define S0SLPTYP	(0X20)
#define S1SLPTYP	(0X21)
#define S2SLPTYP	(0X22)
#define S3SLPTYP	(0X23)
#define S4SLPTYP	(0X24)
#define S5SLPTYP	(0X25)
#define SLPSTCFG	(0X26)

#define PASSTHRU_CNTL   (0x27)
#define PASSTHRU_STAT   (0x28)
#define PM1B_STS_LOW    (0x29)
#define PM1B_STS_HIGH   (0x2a)
#define PM1B_EN_LOW     (0x2b)
#define PM1B_EN_HIGH    (0x2c)
#define PM1B_CNT_LOW    (0x2d)
#define PM1B_CNT_HIGH   (0x2e)
#define GPE1_0_STS      (0x2f)
#define GPE1_1_STS      (0x30)
#define GPE1_0_EN       (0x31)
#define GPE1_1_EN       (0x32)
#define PASSTHRU_DBC    (0x33)
#define PASSTHRU1_CFG   (0x34)
#define PASSTHRU2_CFG   (0x35)
#define PASSTHRU3_CFG   (0x36)
#define PWBTIN_STS      (0x37)
#define RI_STS          (0x38)
#define SMI_SEC_CONT    (0x39)
#define SMI_MIN_CONT    (0x3A)
#define SMI_SEC_CNTR    (0x3B)
#define SMI_MIN_CNTR    (0x3C)
#define SMI_CNTL        (0x3D)
#define SMI_STS         (0x3E)


#define GPREG0		(0X40)
#define GPREG1		(0X41)
#define GPREG2		(0X42)
#define GPREG3		(0X43)
#define GPREG4		(0X44)
#define GPREG5		(0X45)
#define GPREG6		(0X46)
#define GPREG7		(0X47)
#define GPREG8		(0X48)
#define GPREG9		(0X49)
#define GPREGA		(0X4A)
#define GPREGB		(0X4B)
#define GPREGC		(0X4C)
#define GPREGD		(0X4D)
#define GPREGE		(0X4E)
#define GPREGF		(0X4F)
#define GPREG10		(0X50) 
#define GPREG11		(0X51)
#define GPREG12		(0X52)
#define GPREG13		(0X53)
#define GPREG14		(0X54)
#define GPREG15		(0X55)
#define GPREG16		(0X56)
#define GPREG17		(0X57)
#define GPREG18		(0X58)
#define GPREG19		(0X59)
#define GPREG1A		(0X5A)
#define GPREG1B		(0X5B)
#define GPREG1C		(0X5C)
#define GPREG1D		(0X5D)
#define GPREG1E		(0X5E)
#define GPREG1F		(0X5F)

#define IDCR		(0X60)
#define IDSR		(0X61)


// WKSTS0 register
#define SOFTIEV		(1<<6)
#define MOUSEEV		(1<<3)
#define KBDEV		(1<<2)
#define RI2EV		(1<<1)
#define RI1EV		(1<<0)

//	WKSTS1 Register Bit Definitions
#define GPIE0		(1<<0) 
#define GPIE1		(1<<1)
#define GPIE2		(1<<2)
#define GPIE3		(1<<3)
#define GPIE4		(1<<4)
#define GPIE5		(1<<5)
#define GPIE6		(1<<6)
#define GPIE7		(1<<7)

//  WaKEUPCFG0 FIELD DEFINITIONS

#define PWRGDINTEN	(1<<0)  
#define SOFTOFFONCTL	(1<<1)
#define SOFTONONCTL	(1<<2)
#define ONCTLEN		(1<<3)
#define ONCTLGPOEN	(1<<4)
#define ONCTLOEN	(1<<5)
#define ONCTLGPO	(1<<6)

//      SWCTL Register
#define SLPTYPINTEN	(1<<6)
#define SLPSTINTEN	(1<<5)

#define MOUSEEVENT	(1<<3) 
#define KBDEVENT	(1<<2)
#define SLPS5   	(1<<1)
#define SLPS3   	(1<<0)

//  SWCSTAT0 FIELD DEFINITIONS
#define PWRUPFAILSTS	(1<<0) 
#define PWRBTNOVRDSTS	(1<<1)
#define PWRGDINTSTS	(1<<2)
#define PWDGDSTS	(1<<3)
#define ONCTLSTS	(1<<4)
#define WKUPPWRFAIL	(1<<5)
#define LASTONCTL	(1<<6)

//   SWCTL Register
#define SLPTYPINTSTS	(1<<3)
#define SLPSTINTSTS	(1<<4)
#define SLPS5   	(1<<1)
#define SLPS3   	(1<<0)

//	PS2CTL REGISTER FIELD DEFINITIONS
#define DISKBDWKUP	(0<<0) 
#define KBDSPLKEWSQ	(1<<0)
#define KBDPASSWD1	(0X8)
#define KBDPASSWD2	(0X9)
#define KBDPASSWD3	(0XA)
#define KBDPASSWD4	(0XB)
#define KBDPASSWD5	(0xC)
#define KBDPASSWD6	(0XD)
#define KBDPASSWD7	(0XE)
#define KBDPASSWD8	(0XF)
#define DISMSEWKUP	(0<<4)
#define WKUPMSEBUTTON	(1<<4)
#define WKUPLEFTBUTTON	(2<<4)
#define WKUPLEFTDBLCLK	(3<<4)
#define WKUPRTBUTTON	(4<<4)
#define WKUPRTDBLCLK	(5<<4)
#define WKUPANYBUTTON	(6<<4)
#define WKUPANYDBLCLK	(7<<4)
#define DISPARCHK	(1<<7)

#define SWCINTCOUNT     (SWCWorkBase) 
#define SWC_REGS        (SWCBASE)
#define VDDIRQINTFLAG   (SWCWorkBase + 0x4)
#define PTHRU1INTFLAG   (SWCWorkBase + 0x8)
#define PTHRU2INTFLAG   (SWCWorkBase + 0xc)
#define PTHRU3INTFLAG   (SWCWorkBase + 0x10)
#define PWBTININTFLAG   (SWCWorkBase + 0x14)
#define RI1FALLINGINTFLAG (SWCWorkBase + 0x18)
#define RI1RISINGINTFLAG (SWCWorkBase + 0x1C)
#define RI2FALLINGINTFLAG (SWCWorkBase + 0x20)
#define RI2RISINGINTFLAG (SWCWorkBase + 0x24)
#define SLPSINTFLAG     (SWCWorkBase +0x28)
#define SLPSVAL         (SWCWorkBase+0x2c)
#define PM1BIRQINTFLAG  (SWCWorkBase+0x30)

#define SWC_TEST_CFG    (0x401008fd)


extern void swcgpioconfigure (void);
extern void swctimerfunc (tU32, tU8, tU8);
extern void swcinittimer1 (void);
extern void swc_set_irq_handler (tU32);
extern void swchandler (void);
extern void pwbtinhandler (void);
extern void slptypintrhandler(void);
extern void slpstintrhandler(void);
extern void passthruhandler(void);
extern void rihandler(void);
extern void vddintrhandler (void);

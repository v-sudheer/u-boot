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

#include "buffer.h"

#define BMCWDTBASE		0x40410000
#define SIOWDTBASE		0x40427000

#define BMCWDCTL		BMCWDTBASE
#define BMCWDCNT0		BMCWDTBASE+0x10
#define BMCWDCNT1		BMCWDTBASE+0x14

#define SIOWDCTL		SIOWDTBASE
#define SIOWDCNT		SIOWDTBASE+4

#define WDCTLLPCOff		(0<<8)
#define WDCNTLPCOff		(4<<8)

#define BMCWDCNT_RUN		(1<<0) 
#define BMCWD_FIRED		(1<<1)
#define BMCWD_PWROFF		(1<<2)
#define BMCWD_DISABLE		(1<<3)
#define BMCWD_WDO_DELAY_EN	(1<<6)
#define BMCWD_TRRIGER		(1<<7)

#define WDINTRFLAG		BMCWDTWorkBase
#define SIOWDTINTRFLAG		BMCWDTWorkBase+0x4
#define WDOPULSEFLAG		BMCWDTWorkBase+0x8
#define BMCWDOPULSEFLAG		BMCWDTWorkBase+0xC

#define WDT		        WatchdogTimerMSFT
#define WDTCTL0                 WDT+0x00
#define WDTCTL1                 WDT+0x01
#define WDTCTL2                 WDT+0x02
#define WDTCTL3                 WDT+0x03
#define WDTCNT0                 WDT+0x04
#define WDTCNT1                 WDT+0x05
#define WDTCNT2                 WDT+0x06
#define WDTCNT3                 WDT+0x07

extern void sio_wdt_run (void);
extern void sio_load_wdt_count (tU16);
extern void sio_wdt_trigger (void); 
extern void sio_wdt_set_irq_handler  (tU32);
extern void sio_wdt_handler(void);


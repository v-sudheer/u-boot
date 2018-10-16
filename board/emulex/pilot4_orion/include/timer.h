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

//#include "buffer.h"

#define Timer1base		Timer1Base
#define Timer2base		Timer1Base+0x14
#define Timer3base		Timer1base+0x28

#define Timer4Base              Timer1Base+0x800

#define Timer4base		Timer4Base
#define Timer5base		Timer4Base+0x14
#define Timer6base		Timer4base+0x28

#define TimerLoadCount		(0x00) 	
#define TimerCurrentValue	(0x04)	
#define TimerValue 		(0x04)	
 
#define TimerControl		(0x08)	
#define TimerEOI 		(0x0C)	
#define TimerIntStatus		(0x10)	
#define TimersIntStatus		(0xa0)	
#define TimersEOI		(0xa4)	
#define TimersRawIntStatus	(0xa8)	
 
#define TIMERSCOMPVERSION	(0xac)	

//Timer Control Register
 
#define TimerEnable		(1<<0) 	
#define TimerModeSelect 	(1<<1)	
#define TimerFreeRunning	(0<<1)
#define PeriodicMode	 	(1<<1)
#define FreerunninMode		(0<<1)
#define TimerInterruptMask 	(1<<2)	
#define TimerIntEnable		(0<<2)

#define Timer1IntCount          Timer1WorkBase
#define Timer2IntCount          Timer2WorkBase
#define Timer3IntCount          Timer3WorkBase
#define Timer4IntCount          Timer4WorkBase
#define Timer5IntCount          Timer5WorkBase
#define Timer6IntCount          Timer6WorkBase

extern void Timer1IntHandler(void);
extern void timer_setup(tU32, tU32, tU32);
extern void enable_timer(tU32);
extern void Timer1_Handler(void);
extern void Timer2_Handler(void);
extern void Timer3_Handler(void);
extern void Timer4_Handler(void);
extern void Timer5_Handler(void);
extern void Timer6_Handler(void);
extern void combo12handler(void);

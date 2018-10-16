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

#include <configs/pilot4_orion.h>

#define SE_REGISTER_BASE                0x40000000
#define SE_TIMER_BASE                   (SE_REGISTER_BASE + 0x0411000)
#define SE_TIMER_VA_BASE                (SE_TIMER_BASE)
#define CLOCK_TICK_RATE 25000000

#define PRINTCH(x) very_early_putc(x)
#define u32 unsigned int
#define wfe()	__asm__ __volatile__ ("wfe" : : : "memory")
void pilot_timer3_interrupt(void)
{
    volatile u32 sysrst_status;
    volatile u32 *sysrst_status_ptr;
    volatile unsigned long dummy_read;
    dummy_read = *((volatile unsigned long*) (SE_TIMER_VA_BASE+0x0c + 0x28));
    sysrst_status = (volatile u32)(0x401008b0);
    sysrst_status_ptr = (void*)sysrst_status;

#ifndef CONFIG_USE_SSP_RESET
    // Ping WDT2
    *(volatile unsigned int*)(0x4010089C) = 0x1DC4FFFF;
    *(volatile unsigned int*)(0x40100890) |= 0x800000;
#endif

    if(*(volatile u32*)sysrst_status_ptr & (1 << 10)) {
	PRINTCH('B');
	PRINTCH('M');
	PRINTCH('C');
	PRINTCH('\n');
	gic_mask_irq(91);
    gic_mask_irq(86);
#ifndef CONFIG_USE_SSP_RESET
    *(volatile unsigned int*)(0x401008b0) |= (1 << 10);
    *(volatile unsigned int*)(0x40100858) = 0;
    *(volatile unsigned int*)(0x40100854) = 3;
    *(volatile unsigned int*)(0x40100850) |= 1;
#endif
	while(1)
		wfe();
    }
}

static void set_periodic_timer3(void)
{
    volatile unsigned long dummy_read;
    unsigned long ctrl;
    /* Disable timer and interrups */
    *((volatile unsigned long *)(SE_TIMER_VA_BASE+0x08 + 0x28)) = 0x0;
    dummy_read = *((volatile unsigned long*) (SE_TIMER_VA_BASE+0x0c + 0x28));
    /* Load counter values 100ms*/
    *((volatile unsigned long *)(SE_TIMER_VA_BASE + 0x28)) = ((CLOCK_TICK_RATE + 10/2) / 10);
    ctrl = 0x3;
    *((volatile unsigned long *)(SE_TIMER_VA_BASE+0x08 + 0x28)) = ctrl;
}

void stop_pilot_timer3(void)
{
	*((volatile unsigned long *)(SE_TIMER_VA_BASE+0x08 + 0x28)) = 0x0;
}

void start_pilot_timer3(void)
{
	set_periodic_timer3();
	gic_unmask_irq(86);
}

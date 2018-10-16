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


# include "defs.h"
# include "timer.h"


extern void m3_uart_puts(char* buf);

void m3_timer_init ( void )
{
	u32 val = 0;

	// Set the SysTick interrupt to the default priority
	val = *(u32 *)(NVIC_SYSH12_15_PRIORITY);
	val &= ~NVIC_SYSH_PRIORITY_PR15_MASK;
	val |= (NVIC_SYSH_PRIORITY_DEFAULT << NVIC_SYSH_PRIORITY_PR15_SHIFT);
	*(u32 *)(NVIC_SYSH12_15_PRIORITY) = val;

    // Configure SysTick to interrupt at the requested rate
	*(u32 *)(NVIC_SYSTICK_RELOAD) = SYSTICK_RELOAD;

    // Use the processor's clock
	val = *(u32 *)(NVIC_SYSTICK_CTRL);
	val |= NVIC_SYSTICK_CTRL_CLKSOURCE;
	*(u32 *)(NVIC_SYSTICK_CTRL) = val;

    // Enable SysTick interrupts
	val = (NVIC_SYSTICK_CTRL_CLKSOURCE|NVIC_SYSTICK_CTRL_TICKINT|NVIC_SYSTICK_CTRL_ENABLE);
    *(u32 *)(NVIC_SYSTICK_CTRL) = val;
}

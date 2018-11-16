/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 
 * ryan_chen@aspeedtech.com
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>

#include <asm/proc-armv/ptrace.h>

void do_irq (struct pt_regs *pt_regs)
{
//	u32 isr0 = readl(0x1E6C0080);
	puts("do_irq\n");
}

int arch_interrupt_init(void) {
	puts("arch_interrupt_init\n");
	//ast2500 for initial eSPI interrupt 

}
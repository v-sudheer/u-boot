/*
 * Copyright (C) 2012 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2011-2012 Xilinx, Inc. All rights reserved.
 *
 * (C) Copyright 2008
 * Guennadi Liakhovetki, DENX Software Engineering, <lg@denx.de>
 *
 * (C) Copyright 2004
 * Philippe Robin, ARM Ltd. <philippe.robin@arm.com>
 *
 * (C) Copyright 2002-2004
 * Gary Jennejohn, DENX Software Engineering, <gj@denx.de>
 *
 * (C) Copyright 2003
 * Texas Instruments <www.ti.com>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Alex Zuepke <azu@sysgo.de>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * Copyright (c) 2010-2015, Emulex Corporation.
 * Modifications made by Emulex Corporation under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

#include <common.h>
#include <div64.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

struct scu_timer {
	u32 load; /* Timer Load Register */
	u32 counter; /* Timer Counter Register */
	u32 control; /* Timer Control Register */
};

static struct scu_timer *timer_base =
			      (struct scu_timer *) CONFIG_SCUTIMER_BASEADDR;

#define SCUTIMER_CONTROL_PRESCALER_MASK	0x0000FF00 /* Prescaler */
#define SCUTIMER_CONTROL_PRESCALER_SHIFT	8
#define SCUTIMER_CONTROL_AUTO_RELOAD_MASK	0x00000002 /* Auto-reload */
#define SCUTIMER_CONTROL_ENABLE_MASK		0x00000001 /* Timer enable */

#define TIMER_LOAD_VAL 0xFFFFFFFF
#define TIMER_PRESCALE 255
#define TIMER_TICK_HZ  (CONFIG_CPU_FREQ_HZ / 2 / (TIMER_PRESCALE + 1))
#define NSEC_PER_SEC	1000000000
#define ARM_CLOCK_MASK 0xFFFFFFFF
/*This is used to convert nanoseconds to Clocks ticks
  * This is taken from Linux
  */
static void calc_mult_shift(u32 *mult, u32 *shift, u32 from, u32 to, u32 maxsec)
{
	u64 tmp;
	u32 sft, sftacc= 32;

	/*
	 * Calculate the shift factor which is limiting the conversion
	 * range:
	 */
	tmp = ((u64)maxsec * from) >> 32;
	while (tmp) {
		tmp >>=1;
		sftacc--;
	}

	/*
	 * Find the conversion shift/mult pair which has the best
	 * accuracy and fits the maxsec conversion range:
	 */
	for (sft = 32; sft > 0; sft--) {
		tmp = (u64) to << sft;
		tmp += from / 2;
		tmp = tmp/from;
		if ((tmp >> sftacc) == 0)
			break;
	}
	*mult = tmp;
	*shift = sft;
}

int timer_init(void)
{
	const u32 emask = SCUTIMER_CONTROL_AUTO_RELOAD_MASK |
			(TIMER_PRESCALE << SCUTIMER_CONTROL_PRESCALER_SHIFT) |
			SCUTIMER_CONTROL_ENABLE_MASK;
	u32 maxsec = 0xFFFFFFFF;

	/* Load the timer counter register */
	writel(0, &timer_base->control);
	writel(TIMER_LOAD_VAL, &timer_base->load);

	/*
	 * Start the A9Timer device
	 * Enable Auto reload mode, Clear prescaler control bits
	 * Set prescaler value, Enable the decrementer
	 */
	clrsetbits_le32(&timer_base->control, SCUTIMER_CONTROL_PRESCALER_MASK,
								emask);
	maxsec = maxsec/TIMER_TICK_HZ;
	if(!maxsec)
		maxsec = 1;
	calc_mult_shift(&gd->mult, &gd->shift, NSEC_PER_SEC, TIMER_TICK_HZ, maxsec);
	/* Reset time */
	gd->lastinc = 0xFFFFFFFF;
	gd->prev_ctr_value = readl(&timer_base->counter);
	gd->tbl = 0;
	return 0;
}
/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */

ulong get_timer_masked(void)
{
	ulong now;
	ulong diff;
	unsigned long curr_ctr_value = 0;
	static unsigned long remainder = 0;
	if(remainder > (TIMER_TICK_HZ / CONFIG_SYS_HZ))
		printf("WARN:>rem now %u curr_ctr_value 0x%x prev_ctr_value 0x%x diff %u mod %u remainder %u",
			now, curr_ctr_value, gd->prev_ctr_value, diff, (diff % (TIMER_TICK_HZ / CONFIG_SYS_HZ)), remainder);
	curr_ctr_value = readl(&timer_base->counter);
	diff = (gd->prev_ctr_value - curr_ctr_value) & ARM_CLOCK_MASK;
	diff = diff + remainder;
	now =  (diff)/ (TIMER_TICK_HZ / CONFIG_SYS_HZ);
	if(now) {
		/*
		 * Take care of the leftover ticks this will keep accumulating over a period
		 * Ex:->if prev_ctr_value is 0x70e96062 curr_ctr_value 0x70c69480 now will be 9 ms
		 * but in reality it is 9,121,672ns so need to carry over 121,672ns or 30,418 ticks if
		 * timer is running at 250MHZ
		 * Now timer will be more predictable even for longer sleeps and for varied frequencies.
		 */
		remainder = diff % (TIMER_TICK_HZ / CONFIG_SYS_HZ);
		gd->tbl += now;
		gd->lastinc = gd->lastinc - now;
		gd->prev_ctr_value = curr_ctr_value;
	}
	return gd->tbl;
}

void __udelay(unsigned long usec)
{
	unsigned long long clc;
	unsigned long tmp = 0;
	unsigned long tmp2 = 0;
	unsigned long ticks = 0;

	tmp  = readl(&timer_base->counter);
	clc = (unsigned long long) usec * 1000;
	//Given nanoseconds convert to required clock ticks
	clc = ((unsigned long long) clc * gd->mult) >> gd->shift;
	ticks = (unsigned long)clc;
	tmp2  = (unsigned long)readl(&timer_base->counter);
	while (((tmp - tmp2) & ARM_CLOCK_MASK) < ticks) { /* Loop till event */
		tmp2  = (unsigned long)readl(&timer_base->counter);
	}
}

/* Timer without interrupts */
//Application needs to take care of rollovers.
ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

/*
 * This function is derived from PowerPC code (read timebase as long long).
 * On ARM it just returns the timer value.
 */
unsigned long long get_ticks(void)
{
	return get_timer(0);
}

/*
 * This function is derived from PowerPC code (timebase clock frequency).
 * On ARM it returns the number of timer ticks per second.
 */
ulong get_tbclk(void)
{
	return CONFIG_SYS_HZ;
}

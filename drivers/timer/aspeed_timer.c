// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2016 Google Inc.
 * Copyright (C) ASPEED Technology Inc.
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <timer.h>
#include <asm/io.h>

#define TIMER_RELOAD_VAL 0xffffffff
#define ASPEED_TIMER_CLOCK (1 * 1000 * 1000) //1Mhz
#define CLK_PER_HZ (ASPEED_TIMER_CLOCK / CONFIG_SYS_HZ)

#ifdef CONFIG_TIMER
/* Each timer has 4 control bits in ctrl1 register.
 * Timer1 uses bits 0:3, Timer2 uses bits 4:7 and so on,
 * such that timer X uses bits (4 * X - 4):(4 * X - 1)
 * If the timer does not support PWM, bit 4 is reserved.
 */
#define AST_TMC_EN (1 << 0)
#define AST_TMC_1MHZ (1 << 1)
#define AST_TMC_OVFINTR (1 << 2)
#define AST_TMC_PWM (1 << 3)

/* Timers are counted from 1 in the datasheet. */
#define AST_TMC_CTRL1_SHIFT(n) (4 * ((n)-1))

/*
 * All timers share control registers, which makes it harder to make them
 * separate devices. Since only one timer is needed at the moment, making
 * it this just one device.
 */

struct ast_timer_counter
{
	u32 status;
	u32 reload_val;
	u32 match1;
	u32 match2;
};

struct ast_timer
{
	struct ast_timer_counter timers1[3];
	u32 ctrl1;
	u32 ctrl2;
#ifdef CONFIG_MACH_ASPEED_G5
	u32 ctrl3;
	u32 ctrl1_clr;
#else
	u32 reserved[2];
#endif
	struct ast_timer_counter timers2[5];
};

#define AST_TICK_TIMER 1

struct ast_timer_priv
{
	struct ast_timer *regs;
	struct ast_timer_counter *tmc;
};

static struct ast_timer_counter *ast_get_timer_counter(struct ast_timer *timer,
						       int n)
{
	if (n > 3)
		return &timer->timers2[n - 4];
	else
		return &timer->timers1[n - 1];
}

static int ast_timer_probe(struct udevice *dev)
{
	struct ast_timer_priv *priv = dev_get_priv(dev);
	struct timer_dev_priv *uc_priv = dev_get_uclass_priv(dev);

	writel(TIMER_RELOAD_VAL, &priv->tmc->reload_val);

	/*
         * Stop the timer. This will also load reload_val into
         * the status register.
         */
	clrbits_le32(&priv->regs->ctrl1,
		     AST_TMC_EN << AST_TMC_CTRL1_SHIFT(AST_TICK_TIMER));
	/* Start the timer from the fixed 1MHz clock. */
	setbits_le32(&priv->regs->ctrl1,
		     (AST_TMC_EN | AST_TMC_1MHZ) << AST_TMC_CTRL1_SHIFT(AST_TICK_TIMER));

	uc_priv->clock_rate = ASPEED_TIMER_CLOCK;

	return 0;
}

static int ast_timer_get_count(struct udevice *dev, u64 *count)
{
	struct ast_timer_priv *priv = dev_get_priv(dev);

	*count = TIMER_RELOAD_VAL - readl(&priv->tmc->status);

	return 0;
}

static int ast_timer_ofdata_to_platdata(struct udevice *dev)
{
	struct ast_timer_priv *priv = dev_get_priv(dev);

	//        priv->regs = devfdt_get_addr_ptr(dev);
	priv->regs = map_physmem(dev_get_addr(dev),
				 sizeof(struct ast_timer), MAP_NOCACHE);
	if (IS_ERR(priv->regs))
		return PTR_ERR(priv->regs);

	priv->tmc = ast_get_timer_counter(priv->regs, AST_TICK_TIMER);

	return 0;
}

static const struct timer_ops ast_timer_ops = {
    .get_count = ast_timer_get_count,
};

static const struct udevice_id ast_timer_ids[] = {
    {.compatible = "aspeed,ast2500-timer"},
    {.compatible = "aspeed,ast2400-timer"},
    {}};

U_BOOT_DRIVER(ast_timer) = {
    .name = "ast_timer",
    .id = UCLASS_TIMER,
    .of_match = ast_timer_ids,
    .probe = ast_timer_probe,
    .priv_auto_alloc_size = sizeof(struct ast_timer_priv),
    .ofdata_to_platdata = ast_timer_ofdata_to_platdata,
    .ops = &ast_timer_ops,
    .flags = DM_FLAG_PRE_RELOC,
};

#else
#define AST_TIMER_BASE 0x1E782000 /* TIMER #0~7*/

/* macro to read the 32 bit timer */
#define READ_CLK (*(volatile ulong *)(AST_TIMER_BASE + 0))
#define READ_TIMER (READ_CLK / CLK_PER_HZ)

static ulong timestamp;
static ulong lastdec;

int timer_init(void)
{
	*(volatile ulong *)(AST_TIMER_BASE + 4) = TIMER_RELOAD_VAL;
	*(volatile ulong *)(AST_TIMER_BASE + 0x30) = 0x3; /* enable timer1 */

	/* init the timestamp and lastdec value */
	reset_timer_masked();

	return 0;
}

/*
 * timer without interrupts
 */

void reset_timer(void)
{
	reset_timer_masked();
}

ulong get_timer(ulong base)
{
	return get_timer_masked() - base;
}

void set_timer(ulong t)
{
	timestamp = t;
}

/* delay x useconds AND perserve advance timstamp value */
void __udelay(unsigned long usec)
{
	ulong last = READ_CLK;
	ulong clks;
	ulong elapsed = 0;

	/* translate usec to clocks */
	clks = (usec / 1000) * CLK_PER_HZ;
	clks += (usec % 1000) * CLK_PER_HZ / 1000;

	while (clks > elapsed)
	{
		ulong now = READ_CLK;
		if (now <= last)
		{
			elapsed += last - now;
		}
		else
		{
			elapsed += TIMER_RELOAD_VAL - (now - last);
		}
		last = now;
	}
}

void reset_timer_masked(void)
{
	/* reset time */
	lastdec = READ_TIMER; /* capure current decrementer value time */
	timestamp = 0;	/* start "advancing" time stamp from 0 */
}

ulong get_timer_masked(void)
{
	ulong now = READ_TIMER; /* current tick value */

	if (lastdec >= now)
	{       /* normal mode (non roll) */
		/* move stamp fordward with absolute diff ticks */
		timestamp += lastdec - now;
	}
	else
	{ /* we have overflow of the count down timer */

		/* nts = ts + ld + (TLV - now)
		 * ts=old stamp, ld=time that passed before passing through -1
		 * (TLV-now) amount of time after passing though -1
		 * nts = new "advancing time stamp"... it could also roll and
		 * cause problems.
		 */
		timestamp += lastdec + (TIMER_RELOAD_VAL / CLK_PER_HZ) - now;
	}
	lastdec = now;

	return timestamp;
}

/* waits specified delay value and resets timestamp */
void udelay_masked(unsigned long usec)
{
	__udelay(usec);
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
#endif

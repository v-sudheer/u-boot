/*
 * ast_wdt.c - WDT driver for the Aspeed SoC
 *
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <common.h>
#include <asm/io.h>
#include <asm/arch/platform.h>

void wdt_stop(void)
{
	__raw_writel(0, AST_WDT_BASE+0x0c);
}

void wdt_start(unsigned int reload)
{
	wdt_stop();

	/* set the reload value */
	__raw_writel(reload, AST_WDT_BASE + 0x04);
	/* magic word to reload */
	__raw_writel(0x4755, AST_WDT_BASE + 0x08);
	/* start the watchdog with 1M clk src and reset whole chip */
#ifdef CONFIG_AST_WATCHDOG_ENABLE	
	__raw_writel(0x3, AST_WDT_BASE + 0x0c);
	printf("Watchdog: %d(s)\n", reload/(1*1000*1000));
#endif	
}

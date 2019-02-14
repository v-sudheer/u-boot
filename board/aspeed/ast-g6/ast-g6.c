/*
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */


#include <common.h>
#include <netdev.h>
#include <asm/arch/ast-scu.h>
#include <asm/arch/ast-sdmc.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

int misc_init_r (void)
{

#ifdef CONFIG_AST_WATCHDOG
	wdt_start(CONFIG_AST_WATCHDOG_TIMEOUT);
#endif

	int update = 0, i;
	uchar ethaddr[6];

	for (i = 0; i < ASPEED_MAC_COUNT; i++) {
		if (!eth_getenv_enetaddr_by_index("eth", i, ethaddr)) {
			net_random_ethaddr(ethaddr);
			if (eth_setenv_enetaddr_by_index("eth", i, ethaddr)) {
				printf("Failed to set random ethernet address\n");
			} else {
				printf("Setting random ethernet address %pM.\n",
					   (uchar *)&ethaddr);
			}
			update++;
		}
	}

#ifdef CONFIG_ENV_IS_IN_SPI_FLASH
	if (update) {
		saveenv();
	}
#endif

	//TODO unlock .13 scu
	writel(0x1688a8a8, 0x1e6e2010);
	return 0;

}

#ifdef CONFIG_ARMV7_NONSEC
void smp_set_core_boot_addr(unsigned long addr, int corenr)
{
	printf("smp_set_core_boot_addr ");
}

void smp_kick_all_cpus(void)
{
	printf("smp_kick_all_cpus ");
}

void smp_waitloop(unsigned previous_address)
{
	printf("smp_waitloop ");
}
#endif


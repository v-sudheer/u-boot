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
	u32 random;
	uchar board_mac_addr[6];

	for (i = 0; i < 2; i++) {
		if (!eth_getenv_enetaddr_by_index("eth", i, board_mac_addr)) {
			board_mac_addr[0] = 0x00;
			board_mac_addr[1] = 0x0c;
			board_mac_addr[2] = (random >> 20) & 0xff;
			board_mac_addr[3] = (random >> 16) & 0xff;
			board_mac_addr[4] = (random >> 8) & 0xff;
			board_mac_addr[5] = (random) & 0xff + i;

			if (eth_setenv_enetaddr_by_index("eth", i, board_mac_addr)) {
				printf("Failed to set random ethernet address\n");
			} else {
				printf("Setting random ethernet address %pM.\n",
					   (uchar *)&random);
			}
			update++;
		}
	}

	if (update) {
		saveenv();	
	}

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


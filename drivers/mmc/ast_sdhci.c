/*
 * ast_sdhci.c - SDHCI driver for the Aspeed SoC
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
#include <malloc.h>
#include <sdhci.h>
#include <errno.h>
#include <malloc.h>
#include <memalign.h>
#include <mmc.h>

int ast_sdhi_init(u32 regbase, u32 max_clk, u32 min_clk)
{
	struct sdhci_host *host = NULL;
	host = (struct sdhci_host *)calloc(1, sizeof(struct sdhci_host));
	if (!host) {
		printf("sdhci_host malloc fail!\n");
		return 1;
	}

	host->name = "ast_sdhci";
	host->ioaddr = (void *)regbase;
#ifdef CONFIG_ARCH_AST1220
	printf("write 0xf0 == 0x00");
	writel((readl(regbase + 0xf0) | 0x2), regbase + 0xf0);
#endif
	add_sdhci(host, max_clk, min_clk);
	
	return 0;
}

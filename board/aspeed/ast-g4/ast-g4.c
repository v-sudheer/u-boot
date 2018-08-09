/*
 * (C) Copyright 2002 Ryan Chen
 * Copyright 2016 IBM Corporation
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <netdev.h>
#include <asm/arch/platform.h>
#include <asm/arch/ast-sdmc.h>
#include <asm/arch/regs-ahbc.h>
#include <asm/arch/regs-scu.h>
#include <asm/arch/ast-scu.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

#if defined(CONFIG_SHOW_BOOT_PROGRESS)
void show_boot_progress(int progress)
{
    printf("Boot reached stage %d\n", progress);
}
#endif

void enable_caches(void)
{
	/* Enable D-cache. I-cache is already enabled in start.S */
}


int board_init(void)
{
	/* adress of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;
	gd->flags = 0;
	return 0;
}

int misc_init_r(void)
{
	u32 reg;

	/* Unlock AHB controller */
	writel(AHBC_PROTECT_UNLOCK, AST_AHBC_BASE);

	/* Map DRAM to 0x00000000 */
	reg = readl(AST_AHBC_BASE + AST_AHBC_ADDR_REMAP);
	writel(reg | BIT(0), AST_AHBC_BASE + AST_AHBC_ADDR_REMAP);

	/* Unlock SCU */
	writel(SCU_PROTECT_UNLOCK, AST_SCU_BASE);

	/*
	 * The original file contained these comments.
	 * TODO: verify the register write does what it claims
	 *
	 * LHCLK = HPLL/8
	 * PCLK  = HPLL/8
	 * BHCLK = HPLL/8
	 */
	reg = readl(AST_SCU_BASE + AST_SCU_CLK_SEL);
	reg &= 0x1c0fffff;
	reg |= 0x61800000;
	writel(reg, AST_SCU_BASE + AST_SCU_CLK_SEL);

	return 0;
}

int dram_init(void)
{
	gd->ram_size = ast_sdmc_dram_size();
	return 0;
}

#ifdef CONFIG_FTGMAC100
int board_eth_init(bd_t *bd)
{
	int ret = 0, i = 0;
	u32 iobase[2];
	iobase[0] = AST_MAC0_BASE;
	iobase[1] = AST_MAC1_BASE;
	
	for(i = 0; i < 2; i++) {
		ast_scu_multi_func_eth(i);
		ast_scu_init_eth(i);
		ret += ftgmac100_initialize(iobase[i]);
	}
	return 0;
}
#endif


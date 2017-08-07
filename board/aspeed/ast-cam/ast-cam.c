/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
 *
 * (C) Copyright 2002
 * David Mueller, ELSOFT AG, <d.mueller@elsoft.ch>
 *
 * (C) Copyright 2003
 * Texas Instruments, <www.ti.com>
 * Kshitij Gupta <Kshitij@ti.com>
 *
 * (C) Copyright 2004
 * ARM Ltd.
 * Philippe Robin, <philippe.robin@arm.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <netdev.h>
#include <asm/arch/ast-cam-scu.h>
#include <asm/arch/ast-sdmc.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

extern int ast_sdhi_init(u32 regbase, u32 max_clk, u32 min_clk);

#if defined(CONFIG_SHOW_BOOT_PROGRESS)
void show_boot_progress(int progress)
{
    printf("Boot reached stage %d\n", progress);
}
#endif

int board_init(void)
{
	/* adress of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;
	gd->flags = 0;
	return 0;
}

int misc_init_r (void)
{
#ifdef CONFIG_CPU1
	//uart 3/4 shar pin
	//*((volatile ulong*) 0x1e6e2080) = 0xffff0000;
	//uart 1/2 shar pin
	*((volatile ulong*) 0x1e6e2084) = 0xffff0000;
	//mapping table
	*((volatile ulong*) 0x1e6e2104) = CONFIG_CPU1_MAP_FLASH;
	//IO table
	*((volatile ulong*) 0x1e6e211c) = 0x1e600000;
	//IO table
	*((volatile ulong*) 0x1e6e2120) = 0x1e700000;
	//Sram
	*((volatile ulong*) 0x1e6e2118) = CONFIG_CPU1_MAP_SRAM;	

	*((volatile ulong*) 0x1e6e2124) = CONFIG_CPU1_MAP_DRAM;	

	//Enable coldfire V1 clock
//	*((volatile ulong*) 0x1e6e2100) = 0x01;	

	printf("Coldfire V1 : UART1 \n");
#endif

#ifdef CONFIG_AST_WATCHDOG
	wdt_start(CONFIG_AST_WATCHDOG_TIMEOUT);
#endif

	return 0;

}

int dram_init(void)
{
	/* dram_init must store complete ramsize in gd->ram_size */
	gd->ram_size = 512*1024*1024;//ast_sdmc_get_mem_size();

	return 0;
}

int board_eth_init(bd_t *bd)
{
#ifdef CONFIG_FTGMAC100
	return ftgmac100_initialize(bd);
#else
	return 0;
#endif
}

#ifdef CONFIG_GENERIC_MMC

#define CONFIG_SYS_MMC_NUM		2
#define CONFIG_SYS_MMC_BASE		{AST_SDHCI_SLOT0_BASE, AST_SDHCI_SLOT1_BASE}

int board_mmc_init(bd_t *bis)
{
	ulong mmc_base_address[CONFIG_SYS_MMC_NUM] = CONFIG_SYS_MMC_BASE;
	u8 i;

	ast_scu_init_sdhci();
	ast_scu_multi_func_sdhc_slot();

	//multipin.
	for (i = 0; i < CONFIG_SYS_MMC_NUM; i++) {
		if (ast_sdhi_init(mmc_base_address[i], ast_get_sd_clock_src(), 100000))
			return 1;
	}

	return 0;
}
#endif

int power_init_board(void)
{
	return 0;
}

/*
 * (C) Copyright 2012 Michal Simek <monstr@monstr.eu>
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

/******************************************************************************
 *
 * Copyright (c) 2010-2014, Emulex Corporation.
 *
 * Modifications made by Emulex Corporation under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 *****************************************************************************/

#include <common.h>
#include <configs/pilot4_map.h>

#if 0 //EMULEX-TODO
#include <asm/arch/nand.h>
#endif
#include <netdev.h>
DECLARE_GLOBAL_DATA_PTR;

#ifdef CONFIG_BOMBER_PWM_WORKAROUND
extern void start_bomber(void);
#endif
unsigned long power_on_reset = 0;

int board_init(void)
{
    volatile unsigned int temp = 0;
    temp = *(volatile u32 *)(P4_SYS_CLK_BASE + 0x7B0);
    gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

    *(volatile u32 *)(P4_SCRATCH_128_BASE + 0x7C) = temp;

    if ((temp & 0x210) != 0x0)
    {
        power_on_reset = 1;
        *(volatile u32 *)(P4_SYS_CLK_BASE) &= ~0x2;
    }
    else
    {
        power_on_reset = 0;
    }

    // Do not clear the CPU reset status here. Wait till the interrupt fires(if set)
    // and let the handler clear it
    temp &= ~(1 << 10);
    *(volatile u32 *)(P4_SYS_CLK_BASE + 0x7B0) = temp;

    icache_enable();

#ifdef CONFIG_BOMBER_PWM_WORKAROUND
    start_bomber();
#endif
#ifdef CONFIG_USE_SSP_RESET
    start_cm3();
#endif

    return 0;
}

int board_late_init (void)
{
#ifdef CONFIG_SDRAID
	initialize_ssp();
#endif
	return 0;
}

#ifdef CONFIG_CMD_NET
int board_eth_init(bd_t *bis)
{
	u32 ret = 0;
	
	ast_eth_initialize(bis, 0x40200000);
	return ret;
}
#endif

#ifdef CONFIG_CMD_MMC
extern int pilot_mmc_init(bd_t *bd);
int board_mmc_init(bd_t *bd)
{
#ifdef CONFIG_PILOT_MMC
	return pilot_mmc_init(bd);
#endif
}
#endif

#ifdef CONFIG_CMD_NAND
int board_nand_init(struct nand_chip *nand_chip)
{
//EMULEX-TODO
#if 0
	return pilot4_nand_init(nand_chip);
#else
	return 0;
#endif
}
#endif

int dram_init(void)
{
	gd->ram_size = get_ram_size((void *)CONFIG_SYS_SDRAM_BASE,
							CONFIG_SYS_SDRAM_SIZE);
	gd->bd->bi_dram[0].start = CONFIG_SYS_SDRAM_BASE;
	gd->bd->bi_dram[0].size = CONFIG_SYS_SDRAM_SIZE;

	return 0;
}

/*
 * OK, and resets too.
 */
#define SW_RESET_BASE	0x40100000	
#define SYSSRERL2		0x854
#define SYSSRERH2		0x858
#define SYSRCR2		0x850
void reset_cpu(ulong addr)
{
	*(volatile unsigned int *)(SW_RESET_BASE  + SYSSRERL2) = 0x3;
	*(volatile unsigned int *)(SW_RESET_BASE  + SYSSRERH2) = 0;
	*(volatile unsigned int *)(SW_RESET_BASE + SYSRCR2) = 0xD01;

	printf("resetting CPI\n");

	while(1) {;}
}

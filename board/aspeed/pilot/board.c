/*
 * (C) Copyright 2018 Shivah Shankar <shivahshankar.shankarnarayanrao@aspeedtech.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <config.h>
DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	return 0;
}

int board_late_init(void)
{
	return 0;
}

int dram_init(void)
{
	gd->ram_size = CONFIG_SYS_SDRAM_SIZE;
	return 0;
}

/*
 *  * OK, and resets too.
 *   */
#define SW_RESET_BASE   0x40100000
#define SYSSRERL2               0x854
#define SYSSRERH2               0x858
#define SYSRCR2         0x850
void reset_cpu(ulong addr)
{
	*(volatile unsigned int *)(SW_RESET_BASE  + SYSSRERL2) = 0x3;
	*(volatile unsigned int *)(SW_RESET_BASE  + SYSSRERH2) = 0;
	*(volatile unsigned int *)(SW_RESET_BASE + SYSRCR2) = 0xD01;

	printf("resetting CPI\n");

	while(1) {;}
}


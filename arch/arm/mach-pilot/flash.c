/*
 * (C) Copyright 2018 Shivah Shankar <shivahshankar.shankarnarayanrao@aspeedtech.com>
 * 
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/processor.h>
#include <asm/byteorder.h>
#include <environment.h>
#define CONFIG_SYS_MAX_FLASH_BANKS 1
flash_info_t flash_info[CONFIG_SYS_MAX_FLASH_BANKS];		/* FLASH chips info */
unsigned long flash_init (void)
{
	return 0;
}
int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{
	return (0);
}

void reset_cpu(ulong addr)
{
}
#if 0
int set_cpu_clk_info(void)
{
	return 0;
}
unsigned long get_uart_clk(int dev_id)
{
	return 0;
}
int zynq_slcr_get_mio_pin_status(const char *periph)
{
	return 0;
}
#endif
#ifndef CONFIG_SPL_BUILD
u32 spl_boot_device(void)
{
	return 0;
}
#endif
void spl_board_init(void)
{
}

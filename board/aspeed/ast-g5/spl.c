/*
 * Copyright 2011 Freescale Semiconductor, Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <ns16550.h>
#include <asm/io.h>


#include <spl.h>

#include <linux/compiler.h>
#include <asm/global_data.h>
#include <asm/arch/ast-scu.h>
#include <asm/arch/ast-sdmc.h>

DECLARE_GLOBAL_DATA_PTR;

/* Define board data structure */
static bd_t bdata __attribute__ ((section(".data")));

extern int ast_sdhi_init(u32 regbase, u32 max_clk, u32 min_clk);
u32 spl_boot_mode(const u32 boot_device)
{
	return MMCSD_MODE_RAW;
}

u32 spl_boot_device(void)
{
	return BOOT_DEVICE_MMC1;
}

void board_init_f(ulong dummy)
{
	gd->bd = &bdata;
	gd->baudrate = CONFIG_BAUDRATE;

	serial_init();		/* serial communications setup */

	gd->have_console = 1;

	puts("AST1220 ROM boot");
}

void __weak spl_board_init(void)
{
	// DDR init, i2c, usb tty /
#if 0	
//	if(WDT2) {
		gd->malloc_base = 0x84000000;
		gd->malloc_limit =  0x01000000;

	printf("AST1220 ROM boot %x, %x, %x\n", gd->malloc_base, gd->malloc_limit, gd->malloc_ptr);
		
//		drv_usbtty_init();
//	} else {
		//I2c init 
		//read eeprom
		//init DDR 
		//init PLL [MPLL/CPU PLL]
//	}
#endif
}


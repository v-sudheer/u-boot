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
#include <asm/arch/regs-ahbc.h>
#include <asm/arch/regs-bmc-scu.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

int misc_init_r(void)
{
	u32 reg;

	/* Unlock AHB controller */
	writel(AHBC_PROTECT_UNLOCK, AST_AHBC_BASE);

	/* Map DRAM to 0x00000000 */
	reg = readl(AST_AHBC_BASE + AST_AHBC_ADDR_REMAP);
	writel(reg | BIT(0), AST_AHBC_BASE + AST_AHBC_ADDR_REMAP);

	/*
	 * The original file contained these comments.
	 * TODO: verify the register write does what it claims
	 *
	 * LHCLK = HPLL/8
	 * PCLK  = HPLL/8
	 * BHCLK = HPLL/8
	 */
	reg = readl(ASPEED_SCU_BASE + AST_SCU_CLK_SEL);
	reg &= 0x1c0fffff;
	reg |= 0x61800000;
	writel(reg, ASPEED_SCU_BASE + AST_SCU_CLK_SEL);
	
#ifdef CONFIG_SYS_I2C_MAC_OFFSET
	char *s;
	int i, env; 			   // env variable 0: eeprom, 1: environment parameters

	s = getenv ("eeprom");
	env = (s && (*s == 'y')) ? 1 : 0;
	
	if (env) {
		printf("TODO ... eerprom --> \n");
		eeprom_init();
		i2c_set_bus_num(3);
		eeprom_read(CONFIG_SYS_I2C_EEPROM_ADDR, CONFIG_SYS_I2C_MAC_OFFSET, dev->enetaddr, 6);

		for (i = 0; i < 6; i++) {
			if (dev->enetaddr[i] != 0xFF) {
				env = 0;	//Suppose not all 0xFF is valid
			}
		}
	}

	if(env)
		eth_getenv_enetaddr_by_index("eth", dev->index, dev->enetaddr); 
//		eth_setenv_enetaddr("ethaddr", dev->enetaddr);
	else
		eth_getenv_enetaddr_by_index("eth", dev->index, dev->enetaddr); 		
//		eth_getenv_enetaddr("ethaddr", dev->enetaddr);
#else
	int update = 0, i;
	u32 random;
	uchar board_mac_addr[6];

	for (i = 0; i < 2; i++) {
		if (!eth_getenv_enetaddr_by_index("eth", i, board_mac_addr)) {
			random = __raw_readl(0x1e6e2078);		
			board_mac_addr[0] = 0x00;
			board_mac_addr[1] = 0x0c;
			board_mac_addr[2] = (random >> 20) & 0xff;
			board_mac_addr[3] = (random >> 16) & 0xff;
			board_mac_addr[4] = (random >> 8) & 0xff;
			board_mac_addr[5] = (random) & 0xff;

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
#endif	
	return 0;
}


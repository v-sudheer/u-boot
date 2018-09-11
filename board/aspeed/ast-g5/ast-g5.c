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
#ifdef CONFIG_CPU1
	//uart 3/4 shar pin
	//*((volatile ulong*) 0x1e6e2080) = 0xffff0000;
	//uart 1/2 shar pin
	*((volatile ulong*) 0x1e6e2084) = 0xffff0000;
	//LPC UART1/2 reset [clear bit 4/5]
	*((volatile ulong*) 0x1e789098) = 0x00000a00;
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

#if 1
////

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
#endif
	return 0;

}

#if defined(CONFIG_ARCH_AST3200)
int power_init_board(void)
{
	/****************************
		ping mux configuration 
		power control : output 
		GPIOF6	  : output
		GPIOH1&H2 : output 
		GPIOH4&H5 : output 
		GPIOH6&H7 : output 
		SRST2# GPIOAC1 : output 
		// LED 
		GPIOT6,T7 : 16mA config 
		// SWITCH BUTTON 
		SW_BTN#: GPIOF5
		FAC_RST_BTN#: GPIOF3
		MAC1RGMII.O_LINK : GPIOF0
	*****************************/
	/* pin mux */
	//POWER GPIOF6
	__raw_writel( ( __raw_readl(0x1E6E2080) & 0xBFFFFFFF ), 0x1E6E2080); // Clear bit 30
	__raw_writel( ( __raw_readl(0x1E7890A0) & 0xFFFFFFFE ), 0x1E7890A0); // Clear LHCR0 bit 0
	
	//POWER GPIO H0,H1,H2 20150903
	__raw_writel( ( __raw_readl(0x1E6E2090) & 0xFFFFFF3F ), 0x1E6E2090); // Clear bit 6 and 7
	__raw_writel( ( __raw_readl(0x1E6E2094) & 0xFFFFFF9F ), 0x1E6E2094); // Clear bit 5 and 6
	
	//POWER GPIOH4,H5
	__raw_writel( ( __raw_readl(0x1E6E2090) & 0xFFFFFF3F ), 0x1E6E2090); // Clear bit 6 and 7
	__raw_writel( ( __raw_readl(0x1E6E2094) & 0xFFFFFF7F ), 0x1E6E2094); // Clear bit 7
	
	//POWER GPIOH6,H7
	__raw_writel( ( __raw_readl(0x1E6E2090) & 0xFFFFFF3F ), 0x1E6E2090); // Clear bit 6 and 7
	
	// SRST2# : OUTPUT : GPIOAC1
	__raw_writel( ( __raw_readl(0x1E6E2070) & 0xFDFFFFFF ), 0x1E6E2070); // Clear bit 25
	__raw_writel( ( __raw_readl(0x1E6E20AC) & 0xFFFFFFFD ), 0x1E6E20AC); // Clear bit 1
	
	//LED : GPIOT6,T7 - 16mA
	__raw_writel( ( __raw_readl(0x1E6E20A0) | 0x000000C0 ), 0x1E6E20A0); // Set bit 7 6
	__raw_writel( ( __raw_readl(0x1E6E2070) | 0x00000080 ), 0x1E6E2070); // Set bit 7
				  
	// 16mA I/O
	__raw_writel( ( __raw_readl(0x1E6E2090) | 0x00000C00 ), 0x1E6E2090); // Set bit 10 11
	
	// INPUT :	GPIOF0, GPIOF3, GPIOF5
	__raw_writel( ( __raw_readl(0x1E6E2080) & 0xD6FFFFFF ), 0x1E6E2080); // Clear bit 29 27 24
	__raw_writel( ( __raw_readl(0x1E6E2090) & 0xBFFFFFFF ), 0x1E6E2090); // Clear bit 30
										
	/***************************
		direction 
	*****************************/
	//POWER: OUTPUT  GPIOF6,GPIO H0, H1, H2, H4, H5, H6, H7  20150903
	__raw_writel( ( __raw_readl(0x1E780024) | 0xF7004000 ), 0x1E780024); // Set bit 14, 24, 25, 26, 28, 29, 30, 31 20150903
	
	// SRST2# : OUTPUT : GPIOAC1
	__raw_writel( ( __raw_readl(0x1E7801EC) | 0x00000002 ), 0x1E7801EC); // Set bit 1
	
	//LED : GPIOT6,T7
	__raw_writel( ( __raw_readl(0x1E780084) | 0xC0000000 ), 0x1E780084); // Set bit 31 30
	
	// INPUT :	GPIOF0, GPIOF3, GPIOF5
	__raw_writel( ( __raw_readl(0x1E780024) & 0xFFFFFFD6 ), 0x1E780024); // Clear bit 5 3 0
	
	/***************************
		power sequence form 0618
	*****************************/
	// H0 to high 20150903
	__raw_writel( ( __raw_readl(0x1E7800C4) | 0x01000000 ), 0x1E780020); // Set bit 24, GPIO H0  20150903
	
	// SRST2#
	__raw_writel( ( __raw_readl(0x1E7801E8) & 0xFFFFFFFD ), 0x1E7801E8); // Clear bit 1, GPIO AC1
	
	// F6
	__raw_writel( 0x00004000, 0x1E780020);								 // Set bit 15 14, GPIO F6
	mdelay(80);
	
	// H1, H2
	__raw_writel( ( __raw_readl(0x1E7800C4) | 0x06000000 ), 0x1E780020); // Set bit 25 26, GPIO H1 H2
	mdelay(10);
	
	// H5
	__raw_writel( ( __raw_readl(0x1E7800C4) | 0x20000000 ), 0x1E780020); // Set bit 29, GPIO H5 
	mdelay(10);
	
	// H6
	__raw_writel( ( __raw_readl(0x1E7800C4) | 0x40000000 ), 0x1E780020); // Set bit 30, GPIO H6
	mdelay(50);
	
	// H4
	__raw_writel( ( __raw_readl(0x1E7800C4) | 0x10000000 ), 0x1E780020); // Set bit 28, GPIO H4
	
	// AC1, H7
	__raw_writel( 0x00000002, 0x1E7801E8);								 // Set bit 1, GPIO AC1
	__raw_writel( ( __raw_readl(0x1E7800C4) | 0x80000000 ), 0x1E780020); // Set bit 31, GPIO H7
	mdelay(40);    
	
	// H6
	__raw_writel( ( __raw_readl(0x1E7800C4) & 0xBFFFFFFF ), 0x1E780020); // Clear bit 30, GPIO H6
	mdelay(40);    
	
	// H6
	__raw_writel( ( __raw_readl(0x1E7800C4) | 0x40000000 ), 0x1E780020); // Set bit 30, GPIO H6
	
//	printf("Power ON(A1).\n"); 20150623

	return 0;
}
#endif

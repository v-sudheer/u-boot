/*
 * Copyright (C) 2018 Aspeed Technology Inc.
 * Shivah Shankar <shivahshankar.shankarnarayanrao@aspeedtech.com>
 * SPDX-License-Identifier:     GPL-2.0+
 */
#ifndef __ASPEED_PILOT_CONFIG_H
#define __ASPEED_PILOT_CONFIG_H
#include <asm/arch/pilot_map.h>

#define CONFIG_SCUTIMER_BASEADDR    0x40460600
#define CONFIG_CPU_FREQ_HZ              500000000
#define CONFIG_SYS_TIMERBASE            CONFIG_SCUTIMER_BASEADDR
#define CONFIG_SYS_TIMER_COUNTS_DOWN
#define CONFIG_SYS_TIMER_COUNTER        (CONFIG_SYS_TIMERBASE + 0x4)

/* Misc CPU related */
#define CONFIG_CMDLINE_TAG              /* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

#define CONFIG_CMDLINE_EDITING          1

#define CONFIG_EXTRA_ENV_SETTINGS PILOT_ENV_SETTINGS

#define CONFIG_ENV_SPI_MODE			SPI_MODE_0	
#define CONFIG_SF_DEFAULT_MODE			SPI_MODE_0
#define CONFIG_DEFAULT_SPI_MODE			SPI_MODE_0
/*#define CONFIG_SYS_TEXT_BASE 0x8A000000*/
#define CONFIG_SYS_CACHELINE_SIZE		32
#define CONFIG_NR_DRAM_BANKS			1
#define CONFIG_SYS_MAXARGS			32
#define CONFIG_SYS_LOAD_ADDR			0x80B00000
#define CONFIG_PHY_ADDR0                        0x2
#define CONFIG_PHY_ADDR1                        0x1

#define CONFIG_SYS_NS16550_CLK          1843200
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE     (-4)
#define CONFIG_SYS_NS16550_COM1         0x40430000
#define CONFIG_CONS_INDEX               1

/* Environment in Serial Flash */
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_SYS_MAX_FLASH_SECT 32768
#ifdef SHIVAH
/*#define CONFIG_ENV_IS_IN_FLASH*/
#define CONFIG_ENV_IS_IN_SPI_FLASH
/*
#define CONFIG_SYS_NO_FLASH  
#define CONFIG_ENV_IS_NOWHERE 
*/
#else
#endif

#define CONFIG_PILOT4_MAC0 1
#define CONFIG_PILOT4_MAC1 1
#define CONFIG_NET_MULTI
#define CONFIG_HAS_ETH1
#define CONFIG_SYS_FAULT_ECHO_LINK_DOWN
#define CONFIG_SYS_ENET
#define CONFIG_COMMANDS

#define CONFIG_SYS_CBSIZE			2048
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_BAUDRATE                            115200
#define CONFIG_SYS_REDUNDAND_ENVIRONMENT
#define CONFIG_SYS_MALLOC_LEN 			0x400000	
#define CONFIG_SYS_SDRAM_BASE			0x88000000
#define CONFIG_SYS_SDRAM_SIZE			(383*1024*1024) 
#define CONFIG_SYS_MAX_FLASH_BANKS		6 
#define CONFIG_SYS_MAX_BOOT_SPI_BANKS		3
#define CONFIG_SYS_FLASH_BASE			0x00000000
#define CONFIG_SYS_MONITOR_BASE			CONFIG_SYS_FLASH_BASE
#define CONFIG_SYS_TEXT_BASE			CONFIG_SYS_FLASH_BASE
#define CONFIG_ENV_ADDR				0x60000	
#define CONFIG_ENV_ADDR_REDUND			0x70000
#define CONFIG_ENV_SIZE_REDUND         		0x10000
#define CONFIG_ENV_SIZE                        0x10000
#define CONFIG_ENV_SECT_SIZE            CONFIG_ENV_SIZE
#define CONFIG_ENV_OFFSET			0x60000
#define CONFIG_SYS_INIT_RAM_ADDR        0x10006000 
#define CONFIG_SYS_INIT_RAM_SIZE        0x4000
#define CONFIG_SYS_INIT_SP_ADDR         (CONFIG_SYS_INIT_RAM_ADDR + \
					CONFIG_SYS_INIT_RAM_SIZE - \
					GENERATED_GBL_DATA_SIZE)

#define CONFIG_BOOTCOMMAND             "bootm 80000"
#define CONFIG_ENV_OVERWRITE

#define PILOT_ENV_SETTINGS \
        "fdt_high=0x8A000000\0"  \
        "initrd_high=0xFFFFFFFF\0" \
        ""

#endif /*__ASPEED_PILOT_CONFIG_H*/
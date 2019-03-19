/*
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*#define DEBUG 1*/
#include "aspeed-common.h"

#define CONFIG_SYS_UBOOT_BASE			CONFIG_SYS_TEXT_BASE

#define CONFIG_SYS_LOAD_ADDR	0x83000000	/* default load address */

#ifdef CONFIG_CPU_V7
#define CONFIG_SYS_ARCH_TIMER
#define CONFIG_SYS_HZ_CLOCK	(24000000)
#endif

#ifdef CONFIG_PALLADIUM
#undef CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_IS_NOWHERE
#endif
#define CONFIG_BOOTARGS		"bootargs console=ttyS4,115200n8 root=/dev/ram rw init=/linuxrc"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"verify=yes\0"\
	"spi_dma=no\0"\
	"mmcdev=1\0"\
	"update=tftp 80800000 ast2500.scr; so 80800000\0"\
	"ramfs=setenv bootargs console=ttyS4,115200n8 root=/dev/ram rw init=/linuxrc\0"\
	"squashfs=setenv bootargs console=ttyS4,115200n8 root=/dev/mtdblock4 rootfs=squashfs init=/linuxrc\0"\
	"ext2fs=setenv bootargs console=ttyS4,115200n8 root=/dev/mmcblk1p1 rw rootfstype=ext2 init=/linuxrc rootwait\0"\
	"mmc_boot=run ext2fs;mmc dev ${mmcdev};mmc read 80008000 1868 1800;mmc read 83000000 401 50;bootm 80008000 - 83000000\0"\
	""


#define CONFIG_SYS_I2C_EEPROM_ADDR		0x50
/*#define CONFIG_SYS_I2C_MAC_OFFSET		0x800 */

/* ------------------------------------------------------------------------- */
/* mtdparts command line support */
#define CONFIG_CMD_MTDPARTS
#define CONFIG_MTD_DEVICE
#define MTDIDS_DEFAULT          "nor0=spi-flash.0"
#define MTDPARTS_DEFAULT   \
						"mtdparts=spi-flash.0:0x60000@0x0(u-boot)," \
						"0x10000@0x60000(u-boot-env)," \
						"0x10000@0x70000(dtb)," \
						"0x380000@0x80000(uImage)," \
						"0x400000@0x400000(rootfs)," \
						"-(pd_rootfs)"
/* ------------------------------------------------------------------------- */
#define CONFIG_BOOTCOMMAND	"bootm 20080000 20400000 20070000"
#define CONFIG_ENV_OVERWRITE
/* ------------------------------------------------------------------------- */
#define CONFIG_GATEWAYIP		192.168.0.1
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_IPADDR			192.168.0.45
#define CONFIG_SERVERIP			192.168.0.81

/* -------------------------------------------------------------------------
 *  2. UART5 message output
 * #define	 CONFIG_DRAM_UART_38400
 * 3. DRAM Type
 * #define CONFIG_DDR3_8GSTACK     DDR3 8Gbit Stack die
 -------------------------------------------------------------------------*/
/* SPL part */
#ifndef CONFIG_SPL_BUILD
#define CONFIG_SKIP_LOWLEVEL_INIT_ONLY
#endif

#define CONFIG_SPL_FRAMEWORK

#define CONFIG_SPL_TEXT_BASE					0x00000000
#define CONFIG_SPL_MAX_SIZE						0x00010000	/* AST2600 support 64kbyte root of trust */
#define CONFIG_SPL_STACK                        0x88000000
/*
#define CONFIG_SYS_SPL_MALLOC_START     		0x88040000
#define CONFIG_SYS_SPL_MALLOC_SIZE      		0x00100000
*/
#define CONFIG_SPL_RAM_DEVICE

/* BSS setup */
#define CONFIG_SPL_LDSCRIPT     "board/aspeed/ast-g6/u-boot-spl.lds"

/* MMC support */
#ifdef CONFIG_AST_SDHCI
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION     1
/*#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME     "u-boot.img"*/
#endif

#define CONFIG_SPL_BSS_START_ADDR			0x90000000
#define CONFIG_SPL_BSS_MAX_SIZE				0x00100000

#define CONFIG_SPL_FS_LOAD_ARGS_NAME		"ast2600.dtb"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME		"uImage"

#define SYS_LOAD_IMAGE_ADDR					0x80800000
/* Address in RAM where the parameters must be copied by SPL. */
#define CONFIG_SYS_SPL_ARGS_ADDR 			0x80000100
/* Not using MMC raw mode - just for compilation purpose */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR		0
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS		18
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR		19

/* for booting directly linux */

/* FIT load address for RAM boot */
#define CONFIG_SPL_LOAD_FIT_ADDRESS	0x10000000
#endif	/* __CONFIG_H */

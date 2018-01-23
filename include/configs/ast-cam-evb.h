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

#define CONFIG_SYS_LOAD_ADDR	0x83000000	/* default load address */

#define CONFIG_BOOTARGS		"console=ttyS0,115200n8 root=/dev/ram rw init=/linuxrc"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"verify=yes\0"	\
	"mmcdev=0\0" \
	"sddev=1\0" \
	"update=tftp 80800000 ast1220.scr; so 80800000\0" \
	"sd_update=mmc dev 1;fatload mmc 1:1 80800000 all.bin;mmc dev 0;mmc write 80800000 0 20000\0" \
	"ramfs=set bootargs console=ttyS0,115200n8 root=/dev/ram rw init=/linuxrc\0"\
	"squashfs=set bootargs console=ttyS0,115200n8 root=/dev/mtdblock4 rootfs=squashfs init=/linuxrc\0"\
	"ext2fs=set bootargs console=ttyS0,115200n8 root=/dev/mtdblock1 rw rootfstype=ext2 init=/linuxrc\0"\
	"sd_boot=mmc dev 1;fatload mmc ${sddev}:0 80008000 zImage;fatload mmc ${sddev}:0 83000000 ast1220.dtb;bootz 80008000 - 83000000\0" \
	"mmc_load=mmc read 80008000 90 1800;mmc read 83300000 3090 4000;mmc read 90000000 68 15\0"\
	"mmc_boot=mmc dev 0;run mmc_load;bootz 80008000 83300000 90000000\0" \
	"spi_boot=bootm 20080000 20400000 20070000\0" \
	""

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
#define CONFIG_BOOTCOMMAND	"run mmc_boot"
#define CONFIG_ENV_OVERWRITE
/* ------------------------------------------------------------------------- */
#define CONFIG_GATEWAYIP		192.168.0.1
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_IPADDR			192.168.0.45
#define CONFIG_SERVERIP			192.168.0.81

/* SPL part */
#define CONFIG_SPL_FRAMEWORK

#define CONFIG_SPL_TEXT_BASE					0x1e7d0000
#define CONFIG_SPL_MAX_SIZE						0x00010000

#define CONFIG_SPL_STACK                    	0x1e7c8000

#define CONFIG_SYS_SPL_MALLOC_START     		0x88000000
#define CONFIG_SYS_SPL_MALLOC_SIZE      		0x00100000


#define CONFIG_SPL_RAM_DEVICE

/* BSS setup */

#define CONFIG_SPL_LDSCRIPT     "board/aspeed/ast-cam/u-boot-spl.lds"

/* MMC support */
#ifdef CONFIG_AST_SDHCI
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION     1
/*#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME     "u-boot.img"*/
#endif

#define CONFIG_SPL_FS_LOAD_ARGS_NAME            "ast1220.dtb"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME          "uImage"

#define SYS_LOAD_IMAGE_ADDR						0x80800000
/* Address in RAM where the parameters must be copied by SPL. */
#define CONFIG_SYS_SPL_ARGS_ADDR				0x80000100
/* Not using MMC raw mode - just for compilation purpose */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR		64
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS		40
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR		6288
						
/* for booting directly linux */

/* FIT load address for RAM boot */
#define CONFIG_SPL_LOAD_FIT_ADDRESS				0x10000000
#endif	/* __CONFIG_H */

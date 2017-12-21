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
	"update=tftp 80800000 ast1220.scr; so 80800000\0" \
	"ramfs=set bootargs console=ttyS0,115200n8 root=/dev/ram rw init=/linuxrc\0"\
	"squashfs=set bootargs console=ttyS0,115200n8 root=/dev/mtdblock4 rootfs=squashfs init=/linuxrc\0"\
	"sd_boot=fatload mmc 1:0 80000000 all.bin; mmc dev 1; mmc write 80000000 0 20000; run emmc_firmareupdate_done\0" \
	"sd_update=fatload mmc 1:0 83000000 all.bin; mmc dev 0; mmc write 83000000 0 ${load; run emmc_firmareupdate_done\0" \
	"emmc_firmareupdate=fatload mmc 0:1 80000000 all.bin; mmc dev 1; mmc write 80000000 0 20000; run emmc_firmareupdate_done\0"	\
	"update_mode=0\0"	\
	"boot_from_spi=0\0"	\
	"emmc_firmareupdate_done=setenv update_mode 0; save; reset\0"	\
	"updatemode_check=if test ${update_mode} -eq 1; then run emmc_firmareupdate; else run boot_to_emmc; fi\0"	\
	"boot_to_emmc=mmc dev 1; mmc read 80700000 780 80; mmc read 80800000 800 2c00; bootm 80800000 - 80700000\0"	\
	"boot_to_spi=bootm 20080000 - 20070000\0"	\
	"boot_mode_check=if test ${boot_from_spi} -eq 1; then run boot_to_spi; else run updatemode_check; fi\0"	\
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
#define CONFIG_BOOTCOMMAND	"bootm 20080000 20400000 20070000"
#define CONFIG_ENV_OVERWRITE
/* ------------------------------------------------------------------------- */
#define CONFIG_GATEWAYIP		192.168.0.1
#define CONFIG_NETMASK			255.255.255.0
#define CONFIG_IPADDR			192.168.0.45
#define CONFIG_SERVERIP			192.168.0.81

/* SPL part */
#define CONFIG_SPL_FRAMEWORK

#define CONFIG_SPL_TEXT_BASE					0x1e7d0000
#define CONFIG_SPL_MAX_SIZE						0x00020000

#if 1
#define CONFIG_SPL_STACK                    	0x1e7c8000
#else 
#define CONFIG_SPL_STACK                    	0x90000000
#endif
#define CONFIG_SYS_SPL_MALLOC_START     		0x88000000
#define CONFIG_SYS_SPL_MALLOC_SIZE      		0x00100000

#define CONFIG_SPL_RAM_DEVICE

/* BSS setup */
#define CONFIG_SPL_LDSCRIPT     "arch/arm/mach-aspeed/u-boot-spl.lds"

/* MMC support */
#ifdef CONFIG_AST_SDHCI
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION     1
/*#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME     "u-boot.img"*/
#endif

#define CONFIG_SPL_FS_LOAD_ARGS_NAME            "ast1220.dtb"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME          "uImage"

#define SYS_LOAD_IMAGE_ADDR					0x80800000
/* Address in RAM where the parameters must be copied by SPL. */
#define CONFIG_SYS_SPL_ARGS_ADDR			0x80000100
/* Not using MMC raw mode - just for compilation purpose */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR	   	129
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS	  	18
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR 	147

/* for booting directly linux */

/* FIT load address for RAM boot */
#define CONFIG_SPL_LOAD_FIT_ADDRESS	0x10000000
#endif	/* __CONFIG_H */

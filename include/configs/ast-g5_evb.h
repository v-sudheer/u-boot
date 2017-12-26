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
	"verify=yes\0"\
	"update=tftp 80800000 ast2500.scr; so 80800000\0"\
	"ramfs=set bootargs console=ttyS0,115200n8 root=/dev/ram rw init=/linuxrc\0"\
	"squashfs=set bootargs console=ttyS0,115200n8 root=/dev/mtdblock4 rootfs=squashfs init=/linuxrc\0"\
	""


#define CONFIG_SYS_I2C_EEPROM_ADDR		0x50
/*#define CONFIG_SYS_I2C_MAC_OFFSET		0x800 */

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
 */
#endif	/* __CONFIG_H */

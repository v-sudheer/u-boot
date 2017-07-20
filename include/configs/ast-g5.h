/*
 * Copyright (C) 2012-2020  ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*#define DEBUG 1*/

#define CONFIG_MACH_TYPE		MACH_TYPE_ASPEED

#include <asm/arch/platform.h>

/* Misc CPU related */
#define CONFIG_CMDLINE_TAG		/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

#define CONFIG_CMDLINE_EDITING		1	/* command line history */

/* Enable cache controller */
#define CONFIG_SYS_DCACHE_OFF		1
/* ------------------------------------------------------------------------- */
/* additions for new relocation code, must added to all boards */
#define CONFIG_SYS_SDRAM_BASE		(AST_DRAM_BASE)
#define CONFIG_SYS_INIT_RAM_ADDR	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_INIT_RAM_SIZE	(32*1024)
#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_SDRAM_BASE + 0x1000 - GENERATED_GBL_DATA_SIZE)

#define CONFIG_NR_DRAM_BANKS		1

#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE + 0x300000
#define CONFIG_SYS_MEMTEST_END	(CONFIG_SYS_MEMTEST_START + (80*1024*1024))
/*-----------------------------------------------------------------------*/

#define CONFIG_SYS_TEXT_BASE           0
#define CONFIG_SYS_UBOOT_BASE		CONFIG_SYS_TEXT_BASE

/* Memory Info  */
#define CONFIG_SYS_MALLOC_LEN   	(0x1000 + 4*1024*1024) /* malloc() len */

/* NS16550 Configuration */
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE		(-4)
#define CONFIG_SYS_NS16550_CLK			24000000
#define CONFIG_SYS_NS16550_COM1			AST_UART0_BASE
#define CONFIG_SYS_LOADS_BAUD_CHANGE
#define CONFIG_SERIAL1					1
#define CONFIG_CONS_INDEX				1
#define CONFIG_BAUDRATE					115200

/* BOOTP options  */
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_SUBNETMASK

/*
 * Environment Config
 */

#define CONFIG_BOOTFILE		"all.bin"

#define CONFIG_USBD_VENDORID		0x1A03
#define CONFIG_USBD_PRODUCTID_CDCACM	0x2500	/* CDC ACM */


#define CONFIG_USB_DEVICE
#define CONFIG_USB_TTY

#define CONFIG_USBD_HS
#define CONFIG_USBD_PRODUCT_NAME	 "AST"
#define CONFIG_USBD_MANUFACTURER	 "ASPEED"


#define EP0_MAX_PACKET_SIZE			64 /* MUSB_EP0_FIFOSIZE */

#define UDC_INT_ENDPOINT				3
#define UDC_INT_PACKET_SIZE			64
#define UDC_OUT_ENDPOINT				2
#define UDC_OUT_PACKET_SIZE			1024
#define UDC_IN_ENDPOINT				1
#define UDC_IN_PACKET_SIZE				1024
#define UDC_BULK_PACKET_SIZE			1024
#define UDC_BULK_HS_PACKET_SIZE		1024

/*
 * Miscellaneous configurable options
 */
#define CONFIG_SYS_LONGHELP	/* undef to save memory */
#define CONFIG_SYS_CBSIZE		256		/* Console I/O Buffer Size */

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE		(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS	16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE	CONFIG_SYS_CBSIZE /* Boot Argument Buffer Size */

#define CONFIG_SYS_LOAD_ADDR	0x83000000	/* default load address */

#define CONFIG_BOOTARGS		"console=ttyS0,115200n8 ramdisk_size=65536 root=/dev/ram rw init=/linuxrc"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"verify=yes\0"\
	"spi_dma=no\0"\
	"update=tftp 80800000 ast2500.scr; so 80800000\0"\
	"ramfs=set bootargs console=ttyS0,115200n8 root=/dev/ram rw init=/linuxrc\0"\
	"squashfs=set bootargs console=ttyS0,115200n8 root=/dev/mtdblock3 rootfs=squashfs init=/linuxrc\0"\
	""

/* ------------------------------------------------------------------------- */
/* SD/MMC definition */
#ifdef CONFIG_AST_SDHCI
#define CONFIG_GENERIC_MMC
/*#define CONFIG_MMC_TRACE*/
#endif

/* I2C definition */
#ifdef CONFIG_CMD_I2C
#define CONFIG_HARD_I2C			1		/* To enable I2C support	*/
/* #define CONFIG_CMD_EEPROM */
/* #define CONFIG_ENV_EEPROM_IS_ON_I2C */
#define CONFIG_SYS_I2C_EEPROM_ADDR_LEN		2

#define CONFIG_AST_I2C				1
#define CONFIG_I2C_MULTI_BUS		1
#define CONFIG_SYS_MAX_I2C_BUS	8	/* i2c-9 i2c-10 is for SD */

#define CONFIG_SYS_I2C_SPEED		100000 /* 100 kHz */
#define CONFIG_SYS_I2C_INIT_BOARD
#define CONFIG_SYS_I2C_SLAVE		0xfe
#define CONFIG_I2C_EDID

#define CONFIG_SYS_I2C_EEPROM_ADDR		0x50
/*#define CONFIG_SYS_I2C_MAC_OFFSET		0x800 */
#endif

/*-----------------------------------------------------------------------
 * File System Configuration
 */
/* FAT FS */
#define CONFIG_DOS_PARTITION
#define CONFIG_PARTITION_UUIDS
#define CONFIG_SUPPORT_VFAT
#define CONFIG_FS_FAT
#define CONFIG_FAT_WRITE
#define CONFIG_CMD_PART


#ifdef CONFIG_AST_FMC_NOR
#ifdef CONFIG_AST_SPI_NOR
#define CONFIG_SYS_MAX_FLASH_BANKS		(CONFIG_FMC_CS + 1)
#else
#define CONFIG_SYS_MAX_FLASH_BANKS		(CONFIG_FMC_CS)
#endif
#define CONFIG_SYS_MAX_FLASH_SECT	(8192)		/* max number of sectors on one chip */
#define CONFIG_ENV_IS_IN_FLASH		1
#define CONFIG_ENV_ADDR				(AST_FMC_CS0_BASE + 0x60000)
#define CONFIG_ENV_OFFSET		0x60000	/* environment starts here  */
#define CONFIG_ENV_SIZE			0x20000	/* Total Size of Environment Sector */
#endif

#ifdef CONFIG_FMC_SPI
#define CONFIG_SF_DEFAULT_BUS  		0
#define CONFIG_SF_DEFAULT_CS			0
#define CONFIG_SF_DEFAULT_SPEED	 	50000000	
#define CONFIG_SF_DEFAULT_MODE			(SPI_MODE_0)

#define CONFIG_SYS_MAX_FLASH_BANKS		(0)
#define CONFIG_ENV_SIZE			0x20000	/* Total Size of Environment Sector */
#define CONFIG_ENV_OFFSET		0x60000	/* environment starts here  */
#define CONFIG_ENV_SECT_SIZE		(64 * 1024)

#define CONFIG_ENV_IS_IN_SPI_FLASH
#if 0
#define CONFIG_ENV_SPI_MAX_HZ		CONFIG_SF_DEFAULT_SPEED
#define CONFIG_ENV_SPI_MODE		CONFIG_SF_DEFAULT_MODE
#define CONFIG_ENV_SPI_BUS		CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS		CONFIG_SF_DEFAULT_CS
#endif


#define CONFIG_SYS_NO_FLASH


#endif

/* ------------------------------------------------------------------------- */

#define CONFIG_BOOTCOMMAND	"bootm 20080000 20400000"
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
#define CONFIG_CMD_SPL
#define CONFIG_SPL_FRAMEWORK
#define CONFIG_SPL_BOARD_INIT
#define CONFIG_SPL_RAM_DEVICE
#define CONFIG_SPL_STACK                        0x85000000

/* BSS setup */
#define CONFIG_SPL_BSS_START_ADDR               0x90100000
#define CONFIG_SPL_BSS_MAX_SIZE         	0x00100000
#define CONFIG_SYS_SPL_MALLOC_START		0x90200000
#define CONFIG_SYS_SPL_MALLOC_SIZE		0x00080000

#define CONFIG_SPL_LDSCRIPT     "arch/arm/mach-aspeed/u-boot-spl.lds"

/* MMC support */
#ifdef CONFIG_AST_SDHCI
#define CONFIG_SYS_MMCSD_FS_BOOT_PARTITION     1
#define CONFIG_SPL_FS_LOAD_PAYLOAD_NAME     "u-boot.img"
/* Not using MMC raw mode - just for compilation purpose */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR   0
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS  0
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR 0
#endif

/* Address in RAM where the parameters must be copied by SPL. */
#define CONFIG_SYS_SPL_ARGS_ADDR	0x80000000

#define CONFIG_SPL_FS_LOAD_ARGS_NAME            "system.dtb"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME          "uImage"

/* for booting directly linux */

/* SP location before relocation, must use scratch RAM */
#define CONFIG_SPL_TEXT_BASE		0x88000000
/* 36kB blocks of OCM - one is on the top because of bootrom */
#define CONFIG_SPL_MAX_SIZE		0x00400000



/* FIT load address for RAM boot */
#define CONFIG_SPL_LOAD_FIT_ADDRESS	0x10000000

#endif	/* __CONFIG_H */

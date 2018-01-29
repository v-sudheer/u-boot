/*
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef _ASPEED_COMMON_H_
#define _ASPEED_COMMON_H_

#define CONFIG_MACH_TYPE				MACH_TYPE_ASPEED

#include <asm/arch/platform.h>

/* Misc CPU related */
#define CONFIG_CMDLINE_TAG				/* enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_INITRD_TAG

#define CONFIG_CMDLINE_EDITING			1	/* command line history */

/* Enable cache controller */
#define CONFIG_SYS_DCACHE_OFF			1
/* ------------------------------------------------------------------------- */
/* additions for new relocation code, must added to all boards */
#define CONFIG_SYS_SDRAM_BASE			(AST_DRAM_BASE)
#define CONFIG_SYS_INIT_RAM_ADDR		CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_INIT_RAM_SIZE		(32*1024)
#define CONFIG_SYS_INIT_SP_ADDR			(CONFIG_SYS_SDRAM_BASE + 0x1000 - GENERATED_GBL_DATA_SIZE)

#define CONFIG_NR_DRAM_BANKS			1

#define CONFIG_SYS_MEMTEST_START		CONFIG_SYS_SDRAM_BASE + 0x300000
#define CONFIG_SYS_MEMTEST_END			(CONFIG_SYS_MEMTEST_START + (80*1024*1024))
/*-----------------------------------------------------------------------*/
/* Memory Info  */
#define CONFIG_SYS_MALLOC_LEN   		(0x1000 + 4*1024*1024) /* malloc() len */

/***************** NS16550 Configuration *****************/
#ifdef CONFIG_SYS_NS16550
#define CONFIG_SYS_NS16550_SERIAL
#define CONFIG_SYS_NS16550_REG_SIZE		(-4)
#define CONFIG_SYS_NS16550_CLK			24000000
#define CONFIG_SYS_NS16550_COM1			AST_UART0_BASE
#define CONFIG_SYS_LOADS_BAUD_CHANGE
#define CONFIG_SERIAL1					1
#define CONFIG_CONS_INDEX				1
#define CONFIG_BAUDRATE					115200
#endif

/***************** BOOTP options  *****************/
#define CONFIG_BOOTP_BOOTFILESIZE
#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_SUBNETMASK

/***************** Environment Config **********************/
#define CONFIG_BOOTFILE		"all.bin"

#ifdef AST_UDC
#define CONFIG_USBD_VENDORID			0x1A03
#define CONFIG_USBD_PRODUCTID_CDCACM	0x2500	/* CDC ACM */

#define CONFIG_USB_DEVICE
#define CONFIG_USB_TTY

#define CONFIG_USBD_HS
#define CONFIG_USBD_PRODUCT_NAME		"AST"
#define CONFIG_USBD_MANUFACTURER		"ASPEED"

#define EP0_MAX_PACKET_SIZE				64 /* MUSB_EP0_FIFOSIZE */

#define UDC_INT_ENDPOINT				3
#define UDC_INT_PACKET_SIZE				64
#define UDC_OUT_ENDPOINT				2
#define UDC_OUT_PACKET_SIZE				1024
#define UDC_IN_ENDPOINT					1
#define UDC_IN_PACKET_SIZE				1024
#define UDC_BULK_PACKET_SIZE			1024
#define UDC_BULK_HS_PACKET_SIZE			1024
#endif

/*Miscellaneous configurable options  */
/* undef to save memory */ 
#define CONFIG_SYS_LONGHELP			
/* Console I/O Buffer Size */
#define CONFIG_SYS_CBSIZE				256		

/* Print Buffer Size */
#define CONFIG_SYS_PBSIZE				(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_MAXARGS				16		/* max number of command args */
#define CONFIG_SYS_BARGSIZE				CONFIG_SYS_CBSIZE /* Boot Argument Buffer Size */
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
#endif

/* File System Configuration */
/* FAT FS */
#define CONFIG_DOS_PARTITION
#define CONFIG_PARTITION_UUIDS
#define CONFIG_SUPPORT_VFAT
#define CONFIG_FS_FAT
#define CONFIG_FAT_WRITE
#define CONFIG_CMD_PART

/* ------------------------------------------------------------------------- */
/* Serial Flash */
#ifdef CONFIG_CMD_SF
#define CONFIG_SF_DEFAULT_BUS			0
#define CONFIG_SF_DEFAULT_CS			0
#define CONFIG_SF_DEFAULT_SPEED			50000000
#define CONFIG_SF_DEFAULT_MODE			SPI_MODE_3
#endif

#define CONFIG_SYS_NO_FLASH
#define CONFIG_ENV_SPI_BUS				CONFIG_SF_DEFAULT_BUS
#define CONFIG_ENV_SPI_CS				CONFIG_SF_DEFAULT_CS
#define CONFIG_ENV_SPI_MAX_HZ			CONFIG_SF_DEFAULT_SPEED //50MHz
#define CONFIG_ENV_SPI_MODE				CONFIG_SF_DEFAULT_MODE
#define CONFIG_SYS_MAX_FLASH_BANKS		(0)
#define CONFIG_ENV_IS_IN_SPI_FLASH		1
#define CONFIG_ENV_SECT_SIZE			0x10000	/*4K sector*/
#define CONFIG_ENV_OFFSET				0x60000	/* environment starts here  */
#define CONFIG_ENV_SIZE					0x10000	/* Total Size of Environment Sector */

#endif	/* _ASPEED_COMMON_H_ */

/*
 * (C) Copyright 2012 Xilinx
 *
 * Xilinx Zynq common configuration settings
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/******************************************************************************
 *
 * Copyright (c) 2010-2014, Emulex Corporation.
 *
 * Modifications made by Emulex Corporation under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 *****************************************************************************/

#ifndef __CONFIG_PILOT4_COMMON_H
#define __CONFIG_PILOT4_COMMON_H

/*
 * High Level Configuration Options
 */
#define CONFIG_ARMV7 /* CPU */
#define CONFIG_PILOT4 /* SoC */

#define CONFIG_SERIAL2
#define CONFIG_PILOT4_SERIAL
//#define CONFIG_SERIAL3
//#define CONFIG_SERIAL4

/* Default environment */
#define CONFIG_IPADDR   192.168.2.100
#define CONFIG_SERVERIP 192.168.2.101

#define SD_BASEADDR				0x40C00000
#define CONFIG_SYS_SDRAM_BASE	0x81000000
#define CONFIG_SYS_SDRAM_SIZE	PHYS_SDRAM_1_SIZE

#define PROD_IS_PILOT	1

#define CONFIG_PILOT4_MAC0 1
#define CONFIG_PILOT4_MAC1 1
#if defined(CONFIG_PILOT4_MAC0) || defined(CONFIG_PILOT4_MAC1)
#define CONFIG_NET_MULTI
#define CONFIG_HAS_ETH1
#define CONFIG_PILOT4_MAC
#define CONFIG_MII
#define CONFIG_SYS_FAULT_ECHO_LINK_DOWN
#define CONFIG_PHYLIB
#define CONFIG_PHY_MARVELL
#define CONFIG_SYS_ENET
#define CONFIG_COMMANDS
#define CONFIG_CMD_NET
#endif

/* Total Size of Environment Sector */
#define CONFIG_ENV_SIZE		(128 << 10)
/* allow to overwrite serial and ethaddr */
#define CONFIG_ENV_OVERWRITE

/*Needed for mdiord/mdiowr commands*/
#define CONFIG_PILOT4_TESTS 1

/* Serial drivers */
#define CONFIG_BAUDRATE				115200
#ifdef CONFIG_HIGH_SPEED_DEBUG_UART
#define CONFIG_SYS_BAUDRATE_TABLE 	{ 9600, 38400, 115200,230400,460800 }
#else
#define CONFIG_SYS_BAUDRATE_TABLE 	{ 9600, 38400, 115200 }
#endif
#if defined(CONFIG_SERIAL2) 
#define CONFIG_SERIAL_MULTI
#endif


#define CONFIG_SCUTIMER_BASEADDR    0x40460600
#ifndef CONFIG_CPU_FREQ_HZ
#ifdef PILOT4_ORION					
#define CONFIG_CPU_FREQ_HZ     		500000000
#else
#define CONFIG_CPU_FREQ_HZ     		800000000
#endif
#endif
#define CONFIG_SYS_HZ          		1000

#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_HUSH_PARSER	/* use "hush" command parser */
#define CONFIG_SYS_PROMPT_HUSH_PS2	"> "
#define CONFIG_CMDLINE_EDITING
#define CONFIG_SYS_LONGHELP

#define MACH_PILOT4_ASIC	1121
#define CONFIG_MACH_TYPE	MACH_PILOT4_ASIC

#include <config_cmd_default.h>

#if 1
#ifdef CONFIG_SYS_ENET
#define CONFIG_CMD_PING
#define CONFIG_CMD_DHCP
#define CONFIG_CMD_MII
#else
#undef CONFIG_CMD_NET
#undef CONFIG_CMD_NFS
#endif


#undef CONFIG_SYS_FLASH_PROTECTION /* don't use hardware protection */
#define CONFIG_SYS_FLASH_USE_BUFFER_WRITE /* use buffered writes (20x faster) */
#endif

#ifdef CONFIG_PILOT4_SPI
#define CONFIG_SF_DEFAULT_SPEED	20000000
#define CONFIG_SPI_FLASH
/* # define CONFIG_SPI_FLASH_ATMEL */
#define CONFIG_SPI_FLASH_SPANSION
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_CMD_SPI
#define CONFIG_CMD_SF
#endif

//#define CONFIG_SDRAID

#ifdef CONFIG_MMC
# define CONFIG_GENERIC_MMC
# define CONFIG_PILOT_MMC
# define CONFIG_CMD_MMC
# define CONFIG_CMD_FAT
# define CONFIG_CMD_EXT2
# define CONFIG_DOS_PARTITION
# define CONFIG_SYS_MMC_MAX_BLK_COUNT   0x200
#endif


#ifndef CONFIG_ENV_IS_NOWHERE
# ifndef CONFIG_SYS_NO_FLASH
/* Environment in NOR flash */
#define CONFIG_ENV_IS_IN_FLASH
#elif defined(CONFIG_PILOT4_SPI)
/* Environment in Serial Flash */
#define CONFIG_ENV_IS_IN_SPI_FLASH
#elif defined(CONFIG_NAND_PILOT4)
/* Environment in NAND flash */
#define CONFIG_ENV_IS_IN_NAND
#endif

#define CONFIG_ENV_SECT_SIZE		CONFIG_ENV_SIZE
#ifdef PILOT4_ORION					
#define CONFIG_ENV_OFFSET		0x60000
#else
#define CONFIG_ENV_OFFSET		0xE0000
#endif
#define CONFIG_CMD_SAVEENV	/* Command to save ENV to Flash */
#endif

/* For development/debugging */
#ifdef DEBUG
#define CONFIG_CMD_REGINFO
#define CONFIG_PANIC_HANG
#endif

#define CONFIG_SYS_TEXT_BASE 0x8A000000 

/*
 * Open Firmware flat tree
 */
#define CONFIG_OF_LIBFDT

/* Default environment */
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"autoload=n\0"	\
	"kernel_image=uImage\0"	\
	"ramdisk_image=ramdisk.gz\0"	\
	"devicetree_image=pilot4_asic.dtb\0"	\
	"kernel_size=0x400000\0"	\
	"devicetree_size=0x4000\0"	\
	"ramdisk_size=0x500000\0"	\
	"fdt_high=0x81900000\0"	\
	"initrd_high=0xFFFFFFFF\0"	\
	"linux4.4_bootcmd=bootm 100000 500000 4FC000\0"	\
	"sdboot=echo Copying Linux from SD0 to RAM...;" \
		"mmc dev 0;" \
		"mmcinfo;" \
		"fatload mmc 0 0x81000000 ${kernel_image};" \
		"fatload mmc 0 0x81A00000 ${ramdisk_image};" \
		"bootm 0x81000000 \0" \
	"emmcboot=echo Copying Linux from EMMC to RAM...;" \
		"mmc dev 2;" \
		"mmcinfo;" \
		"fatload mmc 2 0x81000000 ${kernel_image};" \
		"fatload mmc 2 0x81A00000 ${ramdisk_image};" \
		"bootm 0x81000000 \0" 

/* default boot is according to the bootmode switch settings */
#define CONFIG_BOOTCOMMAND "bootm 0x100000 0x500000"

#define CONFIG_BOOTDELAY	3 /* -1 to Disable autoboot */

#define CONFIG_SYS_PROMPT	"PILOT_ORION> "


/* Keep L2 Cache Disabled */
#define CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_CACHELINE_SIZE	32

/*
 * Physical Memory map
 */
#define CONFIG_NR_DRAM_BANKS    	1
#define PHYS_SDRAM_1            	CONFIG_SYS_SDRAM_BASE

#define CONFIG_SYS_MEMTEST_START PHYS_SDRAM_1
#define CONFIG_SYS_MEMTEST_END (CONFIG_SYS_MEMTEST_START + \
				    PHYS_SDRAM_1_SIZE - (16 * 1024 * 1024))

#define CONFIG_SYS_INIT_RAM_ADDR	0x10006000 //0xFFFF0000
#define CONFIG_SYS_INIT_RAM_SIZE	0x1000
#define CONFIG_SYS_INIT_SP_ADDR		(CONFIG_SYS_INIT_RAM_ADDR + \
					CONFIG_SYS_INIT_RAM_SIZE - \
					GENERATED_GBL_DATA_SIZE)

#define CONFIG_SYS_MALLOC_LEN 0x400000
#define CONFIG_SYS_MAXARGS 16
#define CONFIG_SYS_CBSIZE 2048
#define CONFIG_SYS_PBSIZE (CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)


/* FIT support */
#define CONFIG_FIT		1
#define CONFIG_FIT_VERBOSE	1 /* enable fit_format_{error,warning}() */

#define CONFIG_BOARD_LATE_INIT

#undef CONFIG_BOOTM_NETBSD

#define ENABLE  1
#define DISABLE 0

#define PARITY_NONE             0
#define PARITY_ODD              1
#define PARITY_EVEN             2
#define PARITY_MARK             3
#define PARITY_SPACE    4


#define ON              1
#define OFF             0

/* -------------------------------------------------------------------------------
 * UART definitions
 * -------------------------------------------------------------------------------
 */
#define SERIAL_THR                     0x00             /*  Transmitter Holding Register(Write).*/
#define SERIAL_RBR                     0x00             /*  Receive Buffer register (Read).*/
#define SERIAL_IER                     0x04             /*  Interrupt Enable register.*/
#define SERIAL_IIR                     0x08             /*  Interrupt Identification register(Read).*/
#define SERIAL_FCR                     0x08             /*  FIFO control register(Write).*/
#define SERIAL_LCR                     0x0C             /*  Line Control register.*/
#define SERIAL_MCR                     0x10             /*  Modem Control Register.*/
#define SERIAL_LSR                     0x14             /*  Line status register(Read) .*/
#define SERIAL_MSR                     0x18             /*  Modem Status register (Read).*/
#define SERIAL_SPR                     0x1C             /*  Scratch pad register */
#define SERIAL_DLL                     0x0              /*  Divisor Register LSB */
#define SERIAL_DLM                     0x4              /*  Divisor Register MSB */

/* IER Register */
#define SERIAL_IER_DR                  0x1              /* Data ready Enable */
#define SERIAL_IER_TE                  0x2              /* THR Empty Enable */
#define SERIAL_IER_RLS                 0x4              /* Receive Line Status Enable */
#define SERIAL_IER_MS                  0x8              /* Modem Staus Enable */

/* IIR Register */
#define SERIAL_IIR_NONE                0x1              /* No interrupt pending */
#define SERIAL_IIR_RLS                 0x6              /* Receive Line Status */
#define SERIAL_IIR_DR                  0x4              /* Receive Data Ready */
#define SERIAL_IIR_TIMEOUT             0xc              /* Receive Time Out */
#define SERIAL_IIR_TE                  0x2              /* THR Empty */
#define SERIAL_IIR_MODEM               0x0              /* Modem Status */

/* FCR Register */
#define SERIAL_FCR_FE                  0x1              /* FIFO Enable */
#define SERIAL_FCR_RXFR                0x2              /* Rx FIFO Reset */
#define SERIAL_FCR_TXFR                0x4              /* Tx FIFO Reset */

/* LCR Register */
#define SERIAL_LCR_LEN5                0x0
#define SERIAL_LCR_LEN6                0x1
#define SERIAL_LCR_LEN7                0x2
#define SERIAL_LCR_LEN8                0x3

#define SERIAL_LCR_STOP                0x4
#define SERIAL_LCR_EVEN                0x18             /* Even Parity */
#define SERIAL_LCR_ODD                 0x8              /* Odd Parity */
#define SERIAL_LCR_PE                  0x8              /* Parity Enable */
#define SERIAL_LCR_SETBREAK            0x40             /* Set Break condition */
#define SERIAL_LCR_STICKPARITY         0x20             /* Stick Parity Enable */
#define SERIAL_LCR_DLAB                0x80             /* Divisor Latch Access Bit */

/* LSR Register */
#define SERIAL_LSR_DR                  0x1              /* Data Ready */
#define SERIAL_LSR_OE                  0x2              /* Overrun Error */
#define SERIAL_LSR_PE                  0x4              /* Parity Error */
#define SERIAL_LSR_FE                  0x8              /* Framing Error */
#define SERIAL_LSR_BI                  0x10             /* Break Interrupt */
#define SERIAL_LSR_THRE                0x20             /* THR Empty */
#define SERIAL_LSR_TE                  0x40             /* Transmitte Empty */
#define SERIAL_LSR_DE                  0x80             /* FIFO Data Error */

/* MCR Register */
#define SERIAL_MCR_DTR                 0x1              /* Data Terminal Ready */
#define SERIAL_MCR_RTS                 0x2              /* Request to Send */
#define SERIAL_MCR_OUT1                0x4              /* output       1 */
#define SERIAL_MCR_OUT2                0x8              /* output2 or global interrupt enable */
#define SERIAL_MCR_LPBK                0x10             /* loopback mode */


/* MSR Register */
#define SERIAL_MSR_DELTACTS            0x1              /* Delta CTS */
#define SERIAL_MSR_DELTADSR            0x2              /* Delta DSR */
#define SERIAL_MSR_TERI                0x4              /* Trailing Edge RI */
#define SERIAL_MSR_DELTACD             0x8              /* Delta CD */
#define SERIAL_MSR_CTS                 0x10             /* Clear To Send */
#define SERIAL_MSR_DSR                 0x20             /* Data Set Ready */
#define SERIAL_MSR_RI                  0x40             /* Ring Indicator */
#define SERIAL_MSR_DCD                 0x80             /* Data Carrier Detect */

#define PILOT_UART_CLOCK			UART_CLOCK        /* defined in board config */
#ifndef CONFIG_HIGH_SPEED_DEBUG_UART
#define PILOT_BAUD_115200           (PILOT_UART_CLOCK /1843200)
#define PILOT_BAUD_57600            (PILOT_UART_CLOCK /921600)
#define PILOT_BAUD_38400            (PILOT_UART_CLOCK /614400)
#define PILOT_BAUD_19200            (PILOT_UART_CLOCK /307200)
#define PILOT_BAUD_14400            (PILOT_UART_CLOCK /230400)
#define PILOT_BAUD_9600             (PILOT_UART_CLOCK /153600)
#endif

#include <configs/pilot4_map.h>
#endif /* __CONFIG_PILOT4_COMMON_H */

/*
 * Copyright (C) 2003-2015 Emulex. All rights reserved. 
 * EMULEX is a trademark of Emulex. 
 * www.emulex.com 
 * This program is free software; you can redistribute it and/or modify it under the terms of version 2
 * of the GNU General Public License as published by the Free Software Foundation. 
 * This program is distributed in the hope that it will be useful. ALL EXPRESS OR IMPLIED CONDITIONS,
 * REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS
 * ARE HELD TO BE LEGALLY INVALID. See the GNU General Public License for more details, a copy of which
 * can be found in the file COPYING included with this package.
 */


#ifndef __CONFIG_PILOT4_ORION_H
#define __CONFIG_PILOT4_ORION_H

#define PILOT4_ORION			1	//Use this flag for ORION specific code
#define ASIC_DUT				1
#define CONFIG_UBOOT_SPI_FLASH_ADDR		0x00000000
//Enable 50MHZ Clock for Debug UART Refer SDK spec and Pilot4 spec before enabling this option
//#define CONFIG_HIGH_SPEED_DEBUG_UART	1

//Video memory is using first 8MB, Last 8MB for gaurd
#define PHYS_SDRAM_1_SIZE (488* 1024 * 1024) 

#define CONFIG_PILOT4_SERIAL

#define CONFIG_MMC

/* PXE boot support */
//#define CONFIG_CMD_PXE
//#define CONFIG_MENU

#define	CONFIG_PHY_ADDR0			0x2	
#define	CONFIG_PHY_ADDR1			0x1

//SPI Related defines
#define CONFIG_SYS_MAX_BOOT_SPI_BANKS 3
#define CONFIG_SYS_MAX_BKUP_SPI_BANKS 3
#define CONFIG_SYS_MAX_HOST_SPI_BANKS 2
#define	CONFIG_SYS_MAX_FLASH_BANKS  (CONFIG_SYS_MAX_BOOT_SPI_BANKS + \
		                             CONFIG_SYS_MAX_BKUP_SPI_BANKS + \
		                             CONFIG_SYS_MAX_HOST_SPI_BANKS)
// The following defines indicates how many to be enabled/used
// by the SW
#define CONFIG_SYS_FLASH_BASE	    0x00000000
#define CONFIG_SYS_LOAD_ADDR		0x80B00000 /* default? */
#define CONFIG_MAX_BOOT_SPI_BANKS   3
#define CONFIG_MAX_BKUP_SPI_BANKS   3
#define CONFIG_MAX_HOST_SPI_BANKS   0
#define CONFIG_MAX_SPI_BANKS   (CONFIG_MAX_BOOT_SPI_BANKS + CONFIG_MAX_BKUP_SPI_BANKS + \
                                CONFIG_MAX_HOST_SPI_BANKS)

/* CS0 should always be accessible */
#define CONFIG_ENABLE_BSPI_CSs		3 

/* use SSP to issue soft reset in case of WD or BMC CPU reset*/
//#define CONFIG_USE_SSP_RESET

/* Use to configure pilot in SAFS mode */
//#define CONFIG_ESPI_SAFS

#define CONFIG_PROJ_MAX_BOOTSPI_SIZE 0xFC00000
#define CONFIG_PROJ_MAX_SPICS0_SIZE  0xFC00000

#define CONFIG_PILOT4_SPI
#define CONFIG_HAS_SPI
#define CONFIG_CMD_FLASH

#define CONFIG_SPI_FLASH
#define CONFIG_SPI_FLASH_ATMEL
#define CONFIG_SPI_FLASH_SPANSION
#define CONFIG_SPI_FLASH_SST
#define CONFIG_SPI_FLASH_STMICRO
#define CONFIG_SPI_FLASH_WINBOND
#define CONFIG_SPI_MICRON
#define CONFIG_SPI_EON
#define CONFIG_SPI_MACRONIX

// Bomber PWM work around
#define CONFIG_BOMBER_PWM_WORKAROUND

//#define CONFIG_BOMBER_RESET

/* Enable this to stop the execution when the wait loops time out */
//#define CONFIG_PANIC_AND_BAIL_OUT	1

#ifdef CONFIG_PANIC_AND_BAIL_OUT 
#define CONFIG_PANIC_HANG	1
#endif

#define CONFIG_SYS_MAX_FLASH_SECT		2048
#define CONFIG_SYS_DIRECT_FLASH_TFTP	1

#define CONFIG_SYS_ENET
#ifdef CONFIG_HIGH_SPEED_DEBUG_UART
#define	UART_CLOCK                              50000000
#else
#define	UART_CLOCK                              1843200
#endif
#define	SYS_CLK		SYSTEM_CLOCK
#define	AHB_CLK		(SYS_CLK/2)
#define	P_CLK		25000000//(AHB_CLK/2)
#define CONFIG_USE_IRQ 1
#ifdef CONFIG_USE_IRQ
#define CONFIG_STACKSIZE_IRQ        (8*1024)
#define CONFIG_STACKSIZE_FIQ        (8*1024)
#endif
#include <configs/pilot4_common.h>

#endif /* __CONFIG_PILOT4_ORION_H */

/*
 * Copyright (C) 2007 American Megatrends Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * Copyright (c) 2010-2015, Emulex Corporation.
 * Modifications made by Emulex Corporation under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */


#ifndef __PILOT_SPI_FLASH_H__
#define __PILOT_SPI_FLASH_H__

#include <config.h>
#include <common.h>

// quad and dual read support
#define READ_FAST	(1 << 0)
#define READ_DUAL	(1 << 1)
#define READ_QUAD	(1 << 2)

struct mtd_erase_region_info 
{
	u_int32_t offset;	/* At which this region starts, from the beginning of the MTD */
	u_int32_t erasesize;	/* For this region */
	u_int32_t numblocks;	/* Number of blocks of erasesize in this region */
};

typedef enum spidir 
{
	SPI_NONE = 0,
	SPI_READ = 1,
	SPI_WRITE = 2
} SPI_DIR;

#ifdef CONFIG_SFDP
typedef struct
{
	unsigned char cmd;
	int cmdlen;		// opcode + address length
	unsigned long addr;
	SPI_DIR dir;
	unsigned int dummycycles;
	// command length for creating command is cmdlen + dummycycles
	unsigned char fastread;
} __attribute__((packed)) spi_cmd_info;
#else
typedef enum fbyte_mode
{
	NO_FBYTE = 0,
	ENTER_FBYTE = 1,
	EXIT_FBYTE = 2,
} FBYTE_MODE;
#endif

struct spi_ctrl_driver 
{
	struct module *module;
	char *name;

	/* Supports fast read at higher frequency */
	int  fast_read;		

	/* Max datasize to be used to read type functions in spi_transfer*/
	int  max_read;		

#ifdef CONFIG_SFDP
	/* spi_transfer can be used for all type of spi access */
	int  (*spi_transfer)(int bank, spi_cmd_info *spi_info, 
			unsigned char *data, unsigned long datalen, int fbyteaddr);

	/* spi_burst_read is not NULL, if the ctrl supports read large data continuosly */
	int  (*spi_burst_read)(int bank, spi_cmd_info spi_info, 
			unsigned char *data, unsigned long  datalen);
#else
	/* spi_transfer can be used for all type of spi access */
	int  (*spi_transfer)(int bank,unsigned char *cmd,int cmdlen, SPI_DIR dir, 
				unsigned char *data, unsigned long datalen, int fbyteaddr);

	/* spi_burst_read is not NULL, if the ctrl supports read large data continuosly */
	int  (*spi_burst_read)(int bank,unsigned char *cmd,int cmdlen, SPI_DIR dir, 
				unsigned char *data, unsigned long  datalen);
#endif
	
	int  (*spi_init)(void);
};

#endif

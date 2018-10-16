/*
 * Copyright 2009(C) Marvell International Ltd. and its affiliates
 * Prafulla Wadaskar <prafulla@marvell.com>
 *
 * Based on drivers/mtd/spi/stmicro.c
 *
 * Copyright 2008, Network Appliance Inc.
 * Jason McMullan <mcmullan@netapp.com>
 *
 * Copyright (C) 2004-2007 Freescale Semiconductor, Inc.
 * TsiChung Liew (Tsi-Chung.Liew@freescale.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
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

#include <common.h>
#include <malloc.h>
#include <spi_flash.h>

#include "spi_flash_internal.h"

struct micron_spi_flash_params {
	u16 idcode;
	u16 nr_blocks;
	const char *name;
	u32 flags;
};

#warning Revisit 4BPP/4BREADS later
static const struct micron_spi_flash_params micron_spi_flash_table[] = {
	{
		.idcode = 0xba19,
		.nr_blocks = 512,
		.name = "N25Q256A13EXX40X",
		//.flags = (SPI_4BREAD_SUPP | SPI_4BWRIT_SUPP | SPI_FLG_STS_RD),
		.flags = (SPI_FBYTE_SUPP | SPI_FLG_STS_RD),
	},
	{
		.idcode = 0xba20,
		.nr_blocks = 1024,
		.name = "N25Q512A13GXX40X",
		//.flags = (SPI_4BREAD_SUPP | SPI_4BWRIT_SUPP | SPI_FLG_STS_RD),
		.flags = (SPI_FBYTE_SUPP | SPI_FLG_STS_RD),
	},
	{
		.idcode = 0xba21,
		.nr_blocks = 2048,
		.name = "N25Q00AA13GXX40X",
		//.flags = (SPI_4BREAD_SUPP | SPI_4BWRIT_SUPP | SPI_FLG_STS_RD),
		.flags = (SPI_FBYTE_SUPP | SPI_FLG_STS_RD),
	},
	{
		.idcode = 0xbb18,
		.nr_blocks = 256,
		.name = "MCRN128_1.8",
		.flags = (SPI_FLG_STS_RD),
	},
	{
		.idcode = 0xbb19,
		.nr_blocks = 512,
		.name = "MCRN256_1.8",
		//.flags = (SPI_4BREAD_SUPP | SPI_4BWRIT_SUPP | SPI_FLG_STS_RD),
		.flags = (SPI_FBYTE_SUPP | SPI_FLG_STS_RD),
	},
	{
		.idcode = 0xbb20,
		.nr_blocks = 1024,
		.name = "MCRN512_1.8",
		//.flags = (SPI_4BREAD_SUPP | SPI_4BWRIT_SUPP | SPI_FLG_STS_RD),
		.flags = (SPI_FBYTE_SUPP | SPI_FLG_STS_RD),
	},
};

struct spi_flash *spi_flash_probe_micron(struct spi_slave *spi, u8 *idcode)
{
	const struct micron_spi_flash_params *params;
	struct spi_flash *flash;
	unsigned int i;
	u16 id = idcode[2] | idcode[1] << 8;

	for (i = 0; i < ARRAY_SIZE(micron_spi_flash_table); i++) {
		params = &micron_spi_flash_table[i];
		if (params->idcode == id)
			break;
	}

	if (i == ARRAY_SIZE(micron_spi_flash_table)) {
		debug("SF: Unsupported Macronix ID %04x\n", id);
		return NULL;
	}

	flash = malloc(sizeof(*flash));
	if (!flash) {
		debug("SF: Failed to allocate memory\n");
		return NULL;
	}

	flash->spi = spi;
	flash->spi->flags = params->flags;
	flash->name = params->name;

	flash->write = spi_flash_cmd_write_multi;
	flash->erase = spi_flash_cmd_erase;
	flash->read = spi_flash_cmd_read_fast;
	flash->page_size = 256;
	flash->sector_size = 256 * 16 * 16;
	flash->size = flash->sector_size * params->nr_blocks;

	/* Clear BP# bits for read-only flash */
	//spi_flash_cmd_write_status(flash, 0);

	return flash;
}

/*******************************************************************************
 *
 * Copyright (C) 2004-2015 Emulex. All rights reserved.
 * EMULEX is a trademark of Emulex.
 * www.emulex.com
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful. ALL EXPRESS
 * OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED
 * WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 * NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS
 * ARE HELD TO BE LEGALLY INVALID. See the GNU General Public License for more
 * details, a copy of which can be found in the file COPYING included
 * with this package.
 *
 ********************************************************************************/

#include <common.h>
#include <stdbool.h>
#include <malloc.h>
#include <spi_flash.h>

#include "spi_flash_internal.h"

#define SPI_PAGE_SIZE		0x100		// page size for spi devices

#define BOOT_SPI_ADDR_MODE	(1 << 6)

static int spi_common_enable4byteaddrmode (struct spi_slave *spi)
{
	int ret = 0;

	ret = spi_flash_cmd (spi, CMD_WRITE_ENABLE, NULL, 0);
	if (ret < 0) {
		debug ("Enabling Write failed\n");
		return ret;
	}

	ret = spi_flash_cmd (spi, CMD_EN4BADDR, NULL, 0);
	if (ret < 0) {
		debug ("Enabling 4-Byte failed\n");
	}

	return ret;
}

static int spi_common_disable4byteaddrmode (struct spi_slave *spi)
{
	int ret = 0;

	ret = spi_flash_cmd (spi, CMD_WRITE_ENABLE, NULL, 0);
	if (ret < 0) {
		debug ("Enabling Write failed\n");
		return ret;
	}

	ret = spi_flash_cmd (spi, CMD_DIS4BADDR, NULL, 0);
	if (ret < 0) {
		debug ("Disabling 4-Byte failed\n");
	}

	return ret;
}

static int spi_common_wait_4byte_flag_ready (struct spi_flash *flash,
			unsigned long timeout)
{
	struct spi_slave *spi = flash->spi;
	unsigned long timebase;
	int ret;
	u8 status;
	u8 cmd = CMD_FLG_STS_REG;

	ret = spi_xfer (spi, 8, &cmd, NULL, SPI_XFER_BEGIN);
	if (ret) {
		debug ("Failed to send command %02x: %d\n", cmd, ret);
		return ret;
	}

	timebase = get_timer (0);
	do {
		ret = spi_xfer (spi, 8, NULL, &status, 0);
		if (ret)
			return -1;

		if ((status & FLAG_STATUS) == FLAG_STATUS)
			break;

	} while (get_timer (timebase) < timeout);

	if ((status & FLAG_STATUS) == FLAG_STATUS)
		return 0;

	// Timed out
	return -1;
}

static int spi_common_wait_ready (struct spi_flash *flash,
			unsigned long timeout)
{
	struct spi_slave *spi = flash->spi;
	unsigned long timebase;
	int ret;
	u8 status;
	u8 cmd = CMD_READ_STATUS;

	ret = spi_xfer (spi, 8, &cmd, NULL, SPI_XFER_BEGIN);
	if (ret) {
		debug ("Failed to send command %02x: %d\n", cmd, ret);
		return ret;
	}

	timebase = get_timer (0);
	do {
		ret = spi_xfer (spi, 8, NULL, &status, 0);
		if (ret)
			return -1;

		if ((status & STATUS_WIP) == 0)
			break;

	} while (get_timer (timebase) < timeout);

	spi_xfer (spi, 0, NULL, NULL, SPI_XFER_END);

	if ((status & STATUS_WIP) == 0)
		return 0;

	/* Timed out */
	return -1;
}

static int spi_common_read (struct spi_flash *flash, u32 offset, size_t len, void *buf)
{
	int ret;
	u8 cmd[5] = {0};
	int cmd_len;

	cmd_len = ((flash->spi->flags & SPI_4BYTE_PART) == SPI_4BYTE_PART)?5:4;

	ret = spi_claim_bus (flash->spi);
	if (ret) {
		debug ("Unable to claim SPI bus\n");
		return ret;
	}

	if ((flash->spi->flags & SPI_4BYTE_ENABLE_NEEDED) == SPI_4BYTE_ENABLE_NEEDED)
	{
		ret = spi_common_enable4byteaddrmode (flash->spi);
		if (ret < 0) {
			debug ("Enabling  4byte addr mode failed\n");
			return ret;
		}
	}

	// Taking read opcode that has been configured while probing in "spi_flash_probe_sfdp"
	// function as per the read mode the device supports. Can be put in other modes by
	// changing the "read_opcode" and flags accordingly in "spi_flash_probe_sfdp"
	cmd[0] = flash->spi->read_opcode;

	memcpy (&cmd[1], &offset, 4);

	debug ("read: 0x%p => cmd = { 0x%02x 0x%02x%02x%02x%02x } len = %d\n",
			buf, cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], len);

	ret = spi_flash_cmd_read (flash->spi, cmd, cmd_len, buf, len);
	if (ret < 0) {
		debug ("SFDP_DEV read failed\n");
		return ret;
	}


	if ((flash->spi->flags & SPI_4BYTE_ENABLE_NEEDED) == SPI_4BYTE_ENABLE_NEEDED)
	{
		ret = spi_common_disable4byteaddrmode (flash->spi);
		if (ret < 0) {
			debug ("Disabling 4byte addr mode failed\n");
			return ret;
		}
	}

	debug ("SFDP_DEV: Successfully read %u bytes @ 0x%x\n", len, offset);
	spi_release_bus (flash->spi);

	return ret;
}

static int spi_common_write (struct spi_flash *flash, u32 offset, size_t len,
			const void *buf)
{
	unsigned long byte_addr;
	size_t chunk_len;
	size_t actual;
	int ret;
	u8 cmd[5] = {0};
	int cmd_len;

	byte_addr = offset % SPI_PAGE_SIZE;
	cmd_len = ((flash->spi->flags & SPI_4BYTE_PART) == SPI_4BYTE_PART)?5:4;

	ret = spi_claim_bus (flash->spi);
	if (ret) {
		debug ("Unable to claim SPI bus\n");
		return ret;
	}

	if ((flash->spi->flags & SPI_4BYTE_ENABLE_NEEDED) == SPI_4BYTE_ENABLE_NEEDED)
	{
		ret = spi_common_enable4byteaddrmode (flash->spi);
		if (ret < 0) {
			debug ("Enabling  4byte addr mode failed\n");
			return ret;
		}
	}

	memset (cmd, 0, 5);
	if ((flash->spi->flags & SPI_4BYTE_ONLY) == SPI_4BYTE_ONLY) {
		cmd[0] = CMD_4B_PP;
	}
	else {
		cmd[0] = CMD_PAGE_PROGRAM;
	}

	for (actual = 0, chunk_len = 0; actual < len; actual += chunk_len) {
		offset += chunk_len;
		chunk_len = min (len - actual, SPI_PAGE_SIZE - byte_addr);

		memcpy (&cmd[1], &offset, 4);

		ret = spi_flash_cmd (flash->spi, CMD_WRITE_ENABLE, NULL, 0);
		if (ret < 0) {
			debug ("Enabling Write failed\n");
			break;
		}

		ret = spi_flash_cmd_write (flash->spi, cmd, cmd_len, buf + actual, chunk_len);
		if (ret < 0) {
			debug ("SFDP_DEV Page Program failed\n");
			break;
		}

		if ((flash->spi->flags & SPI_RD_FLAG_STATUS) == SPI_RD_FLAG_STATUS)
		{
			ret = spi_common_wait_4byte_flag_ready (flash, SPI_FLASH_PROG_TIMEOUT);
			if (ret < 0) {
				debug ("SFDP_DEV read flag status register timed out\n");
				break;
			}
		}

		byte_addr = 0;
	}

	if ((flash->spi->flags & SPI_4BYTE_ENABLE_NEEDED) == SPI_4BYTE_ENABLE_NEEDED)
	{
		ret = spi_common_disable4byteaddrmode (flash->spi);
		if (ret < 0) {
			debug ("Disabling  4byte addr mode failed\n");
			return ret;
		}
	}

	debug ("SFDP_DEV: Successfully programmed %u bytes @ 0x%x\n",
	      len, offset);

	spi_release_bus (flash->spi);
	return ret;
}

// Currently supporting sector erase only
int spi_common_erase (struct spi_flash *flash, u32 offset, size_t len)
{
	unsigned long sector_size;
	size_t actual;
	int ret;
	u8 cmd[5] = {0};
	int cmd_len;

	sector_size = 64*1024;
	cmd_len = ((flash->spi->flags & SPI_4BYTE_PART) == SPI_4BYTE_PART)?5:4;

	if (offset % sector_size || len % sector_size) {
		debug ("Erase offset/length not multiple of sector size\n");
		return -1;
	}

	if ((flash->spi->flags & SPI_4BYTE_ENABLE_NEEDED) == SPI_4BYTE_ENABLE_NEEDED)
	{
		ret = spi_common_enable4byteaddrmode (flash->spi);
		if (ret < 0) {
			debug ("Enabling  4byte addr mode failed\n");
			return ret;
		}
	}

	len /= sector_size;

	ret = spi_claim_bus (flash->spi);
	if (ret) {
		debug ("Unable to claim SPI bus\n");
		return ret;
	}

	memset (cmd, 0, 5);
	if ((flash->spi->flags & SPI_4BYTE_ONLY) == SPI_4BYTE_ONLY) {
		cmd[0] = CMD_4B_BE;
	}
	else {
		cmd[0] = CMD_ERASE_64K;
	}

	for (actual = 0; actual < len; actual++) {
		memcpy (&cmd[1], &offset, 4);

		offset += sector_size;

		ret = spi_flash_cmd (flash->spi, CMD_WRITE_ENABLE, NULL, 0);
		if (ret < 0) {
			debug ("Enabling Write failed\n");
			break;
		}

		ret = spi_flash_cmd_write (flash->spi, cmd, cmd_len, NULL, 0);
		if (ret < 0) {
			debug ("SFDP_DEV page erase failed\n");
			break;
		}

		if ((flash->spi->flags & SPI_RD_FLAG_STATUS) == SPI_RD_FLAG_STATUS)
		{
			ret = spi_common_wait_4byte_flag_ready (flash, SPI_FLASH_PAGE_ERASE_TIMEOUT);
			if (ret < 0) {
				debug ("SFDP_DEV read flag status register timed out\n");
				break;
			}
		}

	}

	if ((flash->spi->flags & SPI_4BYTE_ENABLE_NEEDED) == SPI_4BYTE_ENABLE_NEEDED)
	{
		ret = spi_common_disable4byteaddrmode (flash->spi);
		if (ret < 0) {
			debug ("Disabling  4byte addr mode failed\n");
			return ret;
		}
	}

	debug ("SFDP_DEV: Successfully erased %lu bytes @ 0x%x\n",
	      len * sector_size, offset);

	spi_release_bus (flash->spi);
	return ret;
}

int rd_spi_common (struct spi_slave *spi, unsigned char *idcode,
		struct SFDP_DATA *sfdp_data)
{
	unsigned long size = sizeof (struct SFDPHeader) +
					sizeof (struct ParameterHeader);
	int ret;
	unsigned char cmd[5];
	unsigned long ptable_pointer;

	// Setting SFDP Header and Basic Parameter Header details
	memset (cmd, 0 ,5);
	cmd[0] = CMD_RDSFDP;
	ret = spi_flash_cmd_read (spi, cmd, 4, &sfdp_data->head, size);
	if (ret) {
		debug ("Error reading SFDP Header and standard Parameter Header\n");
		return -1;
	}

	// Verifying SFDP Signature should be SFDP (0x50444653)
	if (sfdp_data->head.signature != 0x50444653)
	{
		debug ("Signature of device not verified, got: %08lx\n",
				sfdp_data->head.signature);
		return -1;
	}

	// Setting all bytes of Basic Parameter Table to its default value (0xFF)
	memset (&sfdp_data->basic_param_table.dword1, 0xff,
			sizeof (struct Basic_Flash_Parameter));

	// Setting all Basic Flash Parameter Header DWORD values
	memset (cmd, 0 ,5);
	cmd[0] = CMD_RDSFDP;
	ptable_pointer = sfdp_data->param_head.ptable_pointer;
	memcpy (&cmd[1], &ptable_pointer, 3);

	ret = spi_flash_cmd_read (spi, cmd, 4,
			(unsigned char*)&sfdp_data->basic_param_table.dword1,
			sfdp_data->param_head.plength * 4);
	if (ret) {
		debug ("Unable to read DWORDS of Basic Parameter Header\n");
		return -1;
	}

	return 0;
}

struct spi_flash *spi_flash_probe_sfdp (struct spi_slave *spi, u8 *idcode)
{
    int ret, strap_status = 0;
    struct spi_flash *flash;

    unsigned long* p_dword = 0;
    int i = 0;

    flash = (struct spi_flash*) malloc (sizeof (struct spi_flash));
    if (flash == NULL) {
        debug ("Unable to allocate memory for flash\n");
        return NULL;
    }

    // In cases where the SPI size strap is set to 4B mode, the controller
    // would automatically generate 4B address cycles. But the device
    // accepts only 3B address for SFDP. Hence it will not be supported
    // when the 4B straps are set.
    strap_status = *(volatile unsigned int*)(P4_SYS_CLK_BASE + 0xC);
    if (spi->bus < CONFIG_MAX_BOOT_SPI_BANKS)
    {
        if ((strap_status & BOOT_SPI_ADDR_MODE) == BOOT_SPI_ADDR_MODE)
        {
            printf("****** SFDP Not supported when BOOT_SPI_ADDR_MODE strap is set *****\n");
            return NULL;
        }
    }

    if (spi->bus >= (CONFIG_MAX_BOOT_SPI_BANKS + CONFIG_MAX_BKUP_SPI_BANKS))
    {
        if (((strap_status >> 10) & 0x3) > 0)
        {
            printf("****** SFDP Not supported when HSPI_SIZE_STRAP > 16MB *********\n");
            return NULL;
        }
    }

    ret = rd_spi_common (spi, idcode, &(spi->sfdp_data));
    if (ret)
    {
        debug ("Non SFDP device idcode: %02x%02x%02x%02x%02x\n",
                idcode[0], idcode[1],idcode[2],idcode[3],idcode[4]);
        return NULL;
    }

    flash->spi = spi;
    flash->size = (spi->sfdp_data.basic_param_table.dword2.flash_mem_density + 1)/8;
    flash->read = spi_common_read;
    flash->write = spi_common_write;
    flash->erase = spi_common_erase;
    flash->page_size = 0x100;	// 256;
    flash->sector_size = flash->page_size * 0x100;
    flash->spi->flags = 0;

#warning "for debugging"
    // Displaying data
    debug ("Signature: %04lx\nMajor: %02x, Minor: %02x\nNo. of headers: %02x\n\n"
            "Basic Flash Parameter:\npmajor: %02x, pminor: %02x\nplength: %02x\n"
            "parameter id: %02x%02x\nptable pointer: %06x\n\nDWORDS:\n",
            spi->sfdp_data.head.signature, spi->sfdp_data.head.major_rev,
            spi->sfdp_data.head.minor_rev, spi->sfdp_data.head.no_of_headers,
            spi->sfdp_data.param_head.pmajor_rev, spi->sfdp_data.param_head.pminor_rev,
            spi->sfdp_data.param_head.plength, spi->sfdp_data.param_head.pid_msb,
            spi->sfdp_data.param_head.pid_lsb, spi->sfdp_data.param_head.ptable_pointer);

    p_dword = (unsigned long*) &spi->sfdp_data.basic_param_table.dword1;
    for (i = 0; i < spi->sfdp_data.param_head.plength; i++) {
        debug ("%2d: %08lx\n", i + 1, *p_dword++);
    } // end warning

    // Setting up flag for 3 byte or 4 byte address modes
    if (spi->sfdp_data.basic_param_table.dword1.address_bytes == 0x0) {
        // 00 is 3 byte only device
        flash->spi->flags |= SPI_3BYTE_ONLY;
    }
    else if (spi->sfdp_data.basic_param_table.dword1.address_bytes == 0x2) {
        // 10 is 4 byte only device
        flash->spi->flags |= SPI_4BYTE_ONLY | SPI_4BYTE_PART;
    }
    else if (spi->sfdp_data.basic_param_table.dword1.address_bytes == 0x1)
    {
        // 01 can be a 3 byte or 4 byte device, have to check size
        flash->spi->flags |= SPI_4BYTE_PART;
        if (flash->size > 16 * 1024 * 1024) {
            flash->spi->flags |= SPI_4BYTE_ENABLE_NEEDED;
        }
    }
    else
    {
        // 0x11 is undefined value
        printf ("ERROR: 0x%02x is not defined for address bytes...\n",
                spi->sfdp_data.basic_param_table.dword1.address_bytes);
        return NULL;
    }

    // Setting up of read_opcode value and flags for quad, dual, fast or standard read
    // read_opcode value will be used in "spi_common_read" while setting Command in command
    // register. Can be put in other modes by changing the "read_opcode" and flags
    // accordingly in "spi_flash_probe_sfdp"
    if (spi->sfdp_data.basic_param_table.dword1.fast_read_114 == 1)
    {
        // Quad read (1-1-4)
        if ((flash->spi->flags & SPI_4BYTE_ONLY) == SPI_4BYTE_ONLY) {
            // JESD216B explicitly tells if the device supports
            // "4B quad read". Since we are compliant to JESD216 only,
            // we assume that if the device is 4B only and if it supports
            // quad mode then following will be the opcode.
            flash->spi->read_opcode = CMD_READ_4B_QUAD;
        }
        else {
            flash->spi->read_opcode = flash->spi->sfdp_data.basic_param_table.dword3.ins_fast_read_114;
        }
        flash->spi->flags |= SPI_QUAD_READ;
    }
    else if (spi->sfdp_data.basic_param_table.dword1.fast_read_112 == 1)
    {
        // Dual read (1-1-2)
        if ((flash->spi->flags & SPI_4BYTE_ONLY) == SPI_4BYTE_ONLY) {
            // JESD216B explicitly tells if the device supports
            // "4B dual read". Since we are compliant to JESD216 only,
            // we assume that if the device is 4B only and if it supports
            // dual mode then following will be the opcode.
            flash->spi->read_opcode = CMD_READ_4B_DUAL;
        }
        else {
            flash->spi->read_opcode = flash->spi->sfdp_data.basic_param_table.dword4.ins_fast_read_112;
        }
        flash->spi->flags |= SPI_DUAL_READ;
    }
    else
    {
        // Fast read
        if ((flash->spi->flags & SPI_4BYTE_ONLY) == SPI_4BYTE_ONLY) {
            flash->spi->read_opcode = CMD_4B_FASTREAD;
        }
        else {
            flash->spi->read_opcode = CMD_READ_ARRAY_FAST;
        }
        flash->spi->flags |= SPI_FAST_READ;
    }

    // Have to issue "read flag status" for micron devices after issuing a command
    if (idcode[0] == 0x20) {
        flash->spi->flags |= SPI_RD_FLAG_STATUS;
    }

    debug ("SFDP device idcode: %02x-%02x-%02x-%02x-%02x size: %u bytes flags: 0x%02x\n",
            idcode[0], idcode[1],idcode[2],idcode[3],idcode[4], flash->size,
            flash->spi->flags);

    return flash;
}

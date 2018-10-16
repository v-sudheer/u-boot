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


#include <common.h>
#include <asm/u-boot.h>
#include <flash.h>
#include <spi.h>
#include <spi_flash.h>
#include <stdbool.h>

#include <configs/pilot4_hw.h>
#include <configs/pilot4_map.h>

#define PILOT_SPI_BMISC_REG	(P4_BOOT_SPI_VA_BASE + 0x1C)
#define PILOT_SPI_HMISC_REG	(P4_HOST_SPI_VA_BASE + 0x1C)
#define PILOT_SPI_BKUPMISC_REG	(P4_BACKUP_SPI_VA_BASE + 0x1C)

#define PILOT_SPI_STRPSTS   (P4_SYS_CLK_VA_BASE + 0x0C)

#define BKUP_SPI_STREN	(1 << 6)

flash_info_t    flash_info[CONFIG_SYS_MAX_FLASH_BANKS];
struct spi_flash * pilot_spiflash_priv[CONFIG_SYS_MAX_FLASH_BANKS];

// The SPI controller at Bank 0 is mapped to 3 chip selects. The BMISC register
// gets programmed with address ranges for the 3 chip selects during flash_init.
// Internally each bank represents one spi device. So to R/W to CS1, one has to
// use address at Bank 1. But get_bank_from_address will pass the 'offset' value to the
// flash's interface. So we have to add the start address of the current bootspi
// bank to get to the right address. Hence the use of this array.
// Eg: Consider CS0 16MB, CS1 16MB, CS2 16MB. If the user does 'md 1000100',
// the flash->read will be called with offset value 100. So the correction value of
// 1000000 will be taken from this array and pilot3_boot_spi_transfer will program
// the address register accordingly.
ulong bootspi_bank_addr_correction[CONFIG_SYS_MAX_BOOT_SPI_BANKS] = {0};
ulong hostspi_bank_addr_correction[CONFIG_SYS_MAX_HOST_SPI_BANKS] = {0};
ulong bkupspi_bank_addr_correction[CONFIG_SYS_MAX_BKUP_SPI_BANKS] = {0};

ulong TOTAL_SIZE;
ulong NONBOOTSPI_TOTAL_SIZE;
ulong BOOTSPI_TOTAL_SIZE;

// Since BootSPI size can be 252MB, it is possible that the address range of
// all SPI put together can go beyond 0x10000000 which is LMEM address. Hence 
// splitting the SPI address ranges across 2 boundaries
#define NONBOOTSPI_START	(0x10000000)
#define NONBOOTSPI_BOUNDARY (0x20000000)

int read_buff(flash_info_t *info, uchar *dest, ulong addr, ulong cnt);

void 
flash_print_info  (flash_info_t *info)
{
	int i;
    	printf("  Size: %ld MB in %d Sectors\n",
        	   info->size >> 20, info->sector_count);
    
	printf("  Sector Start Addresses:");
    	for (i = 0; i < info->sector_count; i++) 
	{
        	if ((i % 5) == 0) 
            		printf ("\n   ");
        	printf (" %08lX%s", info->start[i], info->protect[i] ? " (RO)" : "     ");
    	}
    	printf ("\n");
    	return;
}

int
read_buff_big(flash_info_t *info, uchar *dest, ulong addr, ulong cnt)
{
	return read_buff(info,dest,addr,cnt);
}

void
flash_priv_init(int index, struct spi_flash *flash, ulong totalsize)
{
	int j;
	ulong sector;
	ulong erase_size = 0;
	ulong erase_count=0;
	ulong start = 0;

	flash_info[index].start[0] = CONFIG_SYS_FLASH_BASE+totalsize;
	flash_info[index].size = flash->size;
	flash_info[index].flash_id = 0;  /* TODO : Fill proper value */

	sector=0;
	start = CONFIG_SYS_FLASH_BASE + totalsize;
	erase_size  = flash->sector_size;
	erase_count = flash->size / flash->sector_size;
	for(j=0;j<erase_count;j++)
	{
		flash_info[index].start[sector] = start;
		start += erase_size;
		sector++;
	}

	// printf("CS:%d size:0x%x num_blocks:%d sector_count:%ld\n", index, flash->size, (flash->size/flash->sector_size), sector);
	flash_info[index].sector_count = sector;
	flash_protect(FLAG_PROTECT_SET, flash_info[index].start[0],
			flash_info[index].start[0] + flash->size-1, &flash_info[index]);
}


ulong 
flash_init(void)
{
	ulong totalsize = 0;
	int bank = -1, i;
	int temp, tmp_size = 0;
	ulong nonbootspi_size = 0;

	int cs_size[CONFIG_SYS_MAX_BOOT_SPI_BANKS] = {0};

	for (i =0; i < CONFIG_SYS_MAX_FLASH_BANKS; i++)
	{
		memset(&(pilot_spiflash_priv[i]), 0, sizeof(struct spi_flash));
		memset(&(flash_info[i]), 0, sizeof(flash_info_t));

		flash_info[i].start[0] = 0;
		flash_info[i].size = 0;
		flash_info[i].flash_id = FLASH_UNKNOWN;
	}

	for (i = 0; i < CONFIG_MAX_BOOT_SPI_BANKS; i++)
	{
		bank++;

		if(flash_info[0].size == CONFIG_PROJ_MAX_SPICS0_SIZE ||
		   totalsize >= CONFIG_PROJ_MAX_BOOTSPI_SIZE)
			continue;

		temp = *(unsigned long*)(PILOT_SPI_BMISC_REG);
		temp &= ~(0xC0000000);
		*(unsigned long*)(PILOT_SPI_BMISC_REG) = (temp | (bank << 30));

		//Probing the SPI bus*/
		pilot_spiflash_priv[bank] = spi_flash_probe(bank, 0, 2500000 /*ignored parameter in our case*/, 0);
		if (pilot_spiflash_priv[bank] == NULL)
		{
			// If there is no SPI device at BootSPI CS0 then will not bother continuing furthur.
			if (totalsize == 0)
			{
				printf("ERROR: Unable to Detect PILOT SPI Flash\n");
				return 0;	/* Size */
			}
			else
				continue;
		}


		if (bank > 0)
		{
			// See if the overall size exceeds the max limit. If so trim it to the max limit
			if ((totalsize + pilot_spiflash_priv[bank]->size) > CONFIG_PROJ_MAX_BOOTSPI_SIZE)
				pilot_spiflash_priv[bank]->size = CONFIG_PROJ_MAX_BOOTSPI_SIZE - totalsize;
		}
		cs_size[bank] = (pilot_spiflash_priv[bank]->size/(1024*1024));

		flash_priv_init(bank, pilot_spiflash_priv[bank], totalsize);
		bootspi_bank_addr_correction[bank] = totalsize;
		totalsize += pilot_spiflash_priv[bank]->size;
	}
	BOOTSPI_TOTAL_SIZE = totalsize;

	temp = *(unsigned long*)(PILOT_SPI_BMISC_REG);
	temp &= 0xFFC0C0C0;
	*(unsigned long*)(PILOT_SPI_BMISC_REG) = temp | ((cs_size[0] >> 2) | 
			(((cs_size[0]  + cs_size[1]) >> 2) << 8) |
			(((cs_size[0] + cs_size[1] + cs_size[2]) >> 2) <<16));

	*(unsigned long*)(PILOT_SPI_BMISC_REG) &= ~(0xC0000000); // enable cs-0 by default
	*(unsigned long*)(PILOT_SPI_BMISC_REG) |= (0xC0000000);  // Make it no override

#if (CONFIG_MAX_BKUP_SPI_BANKS > 0)
	bank = CONFIG_SYS_MAX_BOOT_SPI_BANKS - 1;
	for (i = 0; i < CONFIG_MAX_BKUP_SPI_BANKS; i++)
	{
		bank++;
		cs_size[i] = 0;

		temp = *(unsigned long*)(PILOT_SPI_BKUPMISC_REG);
		temp &= ~(0xC0000000);
		*(unsigned long*)(PILOT_SPI_BKUPMISC_REG) = (temp | (i << 30));

		pilot_spiflash_priv[bank] = spi_flash_probe(bank, 0, 2500000 /*ignored parameter in our case*/, 0);
		if (pilot_spiflash_priv[bank] == NULL)
			continue;

		cs_size[i] = (pilot_spiflash_priv[bank]->size/(1024*1024));
		flash_priv_init(bank, pilot_spiflash_priv[bank], (NONBOOTSPI_BOUNDARY + nonbootspi_size));
		bkupspi_bank_addr_correction[i] = tmp_size;
		totalsize += pilot_spiflash_priv[bank]->size;
		tmp_size += pilot_spiflash_priv[bank]->size;
		nonbootspi_size += pilot_spiflash_priv[bank]->size;
	}
	temp = *(unsigned long*)(PILOT_SPI_BKUPMISC_REG);
	temp &= 0xFFC0C0C0;
	*(unsigned long*)(PILOT_SPI_BKUPMISC_REG) = temp | ((cs_size[0] >> 2) |
			(((cs_size[0]  + cs_size[1]) >> 2) << 8) |
			(((cs_size[0] + cs_size[1] + cs_size[2]) >> 2) <<16));
	*(unsigned long*)(PILOT_SPI_BKUPMISC_REG) &= ~(0xC0000000); // enable cs-0 by default
	*(unsigned long*)(PILOT_SPI_BKUPMISC_REG) |= (0xC0000000);  // Make it no override
#endif

#if (CONFIG_MAX_HOST_SPI_BANKS > 0)
	tmp_size = 0;
	bank = (CONFIG_SYS_MAX_BOOT_SPI_BANKS + CONFIG_SYS_MAX_BKUP_SPI_BANKS) - 1;
	for (i = 0; i < CONFIG_MAX_HOST_SPI_BANKS; i++)
	{
		bank++;
		cs_size[i] = 0;

		temp = *(unsigned long*)(PILOT_SPI_HMISC_REG);
		temp &= ~(0xC0000000);
		*(unsigned long*)(PILOT_SPI_HMISC_REG) = (temp | (i << 30));

		pilot_spiflash_priv[bank] = spi_flash_probe(bank, 0, 2500000 /*ignored parameter in our case*/, 0);
		if (pilot_spiflash_priv[bank] == NULL)
			continue;

		cs_size[i] = (pilot_spiflash_priv[bank]->size/(1024*1024));
		flash_priv_init(bank, pilot_spiflash_priv[bank], (NONBOOTSPI_BOUNDARY + nonbootspi_size));
		hostspi_bank_addr_correction[i] = tmp_size;
		totalsize += pilot_spiflash_priv[bank]->size;
		tmp_size += pilot_spiflash_priv[bank]->size;
		nonbootspi_size += pilot_spiflash_priv[bank]->size;
	}
	temp = *(unsigned long*)(PILOT_SPI_HMISC_REG);
	temp &= 0xFF000000;
	*(unsigned long*)(PILOT_SPI_HMISC_REG) = temp | ((cs_size[0] >> 2) |
			(((cs_size[0]  + cs_size[1]) >> 2) << 8) | (((cs_size[0]  + cs_size[1]) >> 2) << 16));
	*(unsigned long*)(PILOT_SPI_HMISC_REG) &= ~(0xC0000000); // enable cs-0 by default
	*(unsigned long*)(PILOT_SPI_HMISC_REG) |= (0xC0000000);  // Make it no override
#endif

	TOTAL_SIZE = totalsize;
	NONBOOTSPI_TOTAL_SIZE = nonbootspi_size;
	return totalsize;
}

int get_bank_from_address(ulong addr, ulong * offset){

	int i;

	for(i=0; i< CONFIG_SYS_MAX_FLASH_BANKS; i++){

		if(flash_info[i].size == 0 || (addr > (flash_info[i].start[0] + (flash_info[i].size-1))))
			continue;

		if (addr < CONFIG_SYS_FLASH_BASE)
			continue;

#ifndef CONFIG_P4_ZYNQ_FPGA
		// 1. See if the address does not fall beyond bootspi size till 0x20000000
		// (0x10000000 - 0x20000000) is a hole as far as SPI address is concerned
		// because 0x10000000 is reserved for LMEM.
		// 2. See if the address does not go beyond 0x20000000 + size of bkup/host spi put together
		if (((addr >= BOOTSPI_TOTAL_SIZE) && (addr < NONBOOTSPI_BOUNDARY)) ||
				(addr >= (NONBOOTSPI_BOUNDARY + NONBOOTSPI_TOTAL_SIZE)))
		{
			//printf("1.addr:0x%x does not belong to SPI\n", addr);
			continue;
		}
#else
		// In case of Zynq uboot there is only BootSPI.
		if (addr >= (CONFIG_SYS_FLASH_BASE + BOOTSPI_TOTAL_SIZE))
		{
			//printf("1.addr:0x%x does not belong to SPI\n", addr);
			continue;
		}
#endif

		*offset = addr - flash_info[i].start[0];
		return i;
	}
	return -1;
}  

int 
read_buff(flash_info_t *info, uchar *dest, ulong addr, ulong cnt)
{
	int bank; 
	unsigned long offset;

	/*TODO: Add code for read crossing bank boundary */
	bank = get_bank_from_address(addr,&offset);
	if (bank == -1)
		return  -ERR_INVAL;

	return spi_flash_read(pilot_spiflash_priv[bank], offset, cnt, dest);
}

int 
write_buff (flash_info_t *info, uchar *src, ulong addr, ulong cnt)
{
	int bank; 
	unsigned long offset;


	bank = get_bank_from_address(addr,&offset);
	if (bank == -1)
		return ERR_INVAL;

	return spi_flash_write(pilot_spiflash_priv[bank], offset, cnt, src);
}

int flash_erase(flash_info_t *info, int s_first, int s_last)
{
        int rcode = 0;
        int prot = 0;
        flash_sect_t sect;
	int bank = 0; 
	unsigned long offset;
	unsigned long len = 0;


        if (info->flash_id == FLASH_UNKNOWN) {
                puts("Can't erase unknown flash type - aborted\n");
                return 1;
        }

        if ((s_first < 0) || (s_first > s_last)) {
                puts("- no sectors to erase\n");
                return 1;
        }

        for (sect = s_first; sect <= s_last; ++sect) {

		//Calculate the true length here as the other way was missing the last sector
		len += info->size/info->sector_count;
                if (info->protect[sect])
                        prot++;
        }
        if (prot) {
                printf("- Warning: %d protected sectors will not be erased!\n",
                       prot);
                return 1;
        } else {
                putc('\n');
        }

        bank = get_bank_from_address(info->start[s_first],&offset);
        if (bank == -1)
            return ERR_INVAL;

        if (spi_flash_erase(pilot_spiflash_priv[bank], offset, len))
            rcode = 1;

        puts(" done\n");
        return rcode;
}

#ifdef CONFIG_SFDP
void spi_sfdp_read (int bank)
{
	struct spi_flash *flash = pilot_spiflash_priv[bank];
	unsigned long *p_dword;
	int i;

	if (bank > CONFIG_SYS_MAX_FLASH_BANKS)
	{
		printf ("bank: %d is not initialized\n", bank + 1);
		return;
	}

	if (flash == NULL)
	{
		printf ("bank: %d doesn't supports SFDP\n", bank + 1);
		return;
	}

	struct SFDP_DATA *sfdp_data = &flash->spi->sfdp_data;
	
	// Displaying data
	printf ("Signature: %04lx\nMajor: %02x, Minor: %02x\nNo. of headers: %02x\n\n"
            "Basic Flash Parameter:\npmajor: %02x, pminor: %02x\nplength: %02x\n"
            "parameter id: %02x%02x\nptable pointer: %06x\n\nDWORDS:\n",
            sfdp_data->head.signature, sfdp_data->head.major_rev,
            sfdp_data->head.minor_rev, sfdp_data->head.no_of_headers,
            sfdp_data->param_head.pmajor_rev, sfdp_data->param_head.pminor_rev,
            sfdp_data->param_head.plength, sfdp_data->param_head.pid_msb,
            sfdp_data->param_head.pid_lsb, sfdp_data->param_head.ptable_pointer);

	p_dword = (unsigned long*) &sfdp_data->basic_param_table.dword1;
	for (i = 0; i < sfdp_data->param_head.plength; i++) {
		printf ("%2d: %08lx\n", i + 1, *p_dword++);
	}
}
#endif

/*
 * (C) Copyright 2018 Shivah Shankar <shivahshankar.shankarnarayanrao@aspeedtech.com>
 * 
 * SPDX-License-Identifier:     GPL-2.0+
 */

#include <common.h>
#include <asm/processor.h>
#include <asm/byteorder.h>
#include <environment.h>
#include <dm.h>
#include <spi.h>
#include <flash.h>
#include <spi_flash.h>
#include "4byteaddressing.h"
#define BOOTSPI_CTRLR_BASE 0x40200000
#define MAX_BOOTSPI_SIZE_LIMIT 0xFC00000
#define BMISC_REG 0x1C
#define PILOT_SPI_BMISC_REG (BOOTSPI_CTRLR_BASE + BMISC_REG)
#define PILOT_STRAP_STATUS_REG 0x4010010c
flash_info_t flash_info[3];		/* FLASH chips info */
struct spi_flash * pilot_spiflash_priv[3];
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

extern void flash_bootspi_enumerate_chipselects(bool hspi, tU32 * flash_size, 
					tU32 * cs_sizes, tU16 * cs_ids);
// Since BootSPI size can be 252MB, it is possible that the address range of
// all SPI put together can go beyond 0x10000000 which is LMEM address. Hence
// splitting the SPI address ranges across 2 boundaries
#define NONBOOTSPI_START        (0x10000000)
#define NONBOOTSPI_BOUNDARY (0x20000000)
#ifdef CONFIG_FLASH_CFI_MTD
static uint flash_verbose=1;
void flash_set_verbose(uint v)
{
flash_verbose = v;
}

unsigned long flash_sector_size(flash_info_t *info, flash_sect_t sect)
{
if (sect != (info->sector_count - 1))
return info->start[sect + 1] - info->start[sect];
else
return info->start[0] + info->size - info->start[sect];
}
#endif
void flash_priv_init(int index, struct spi_flash *flash, ulong totalsize)
{
	int j;
	ulong start = CONFIG_SYS_FLASH_BASE + totalsize;

	flash_info[index].size = flash->size;
	flash_info[index].sector_count = flash->size/flash->sector_size;
	flash_info[index].flash_id = FLASH_MAN_CFI;
	printf("Sector size %x, page size %x, erase size %x\n", flash->sector_size, flash->page_size, flash->erase_size);
	for(j=0;j<flash_info[index].sector_count;j++)
	{
		flash_info[index].start[j] = start;
		start += flash->sector_size;
	}
#if 0
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
	//erase_count = flash->size / flash->sector_size;
	erase_count = flash->size / flash->erase_size;
	//erase_count = flash->sector_count
	for(j=0;j<erase_count;j++)
	{
		flash_info[index].start[sector] = start;
		start += erase_size;
		sector++;
	}

	printf("CS:%d size:0x%x num_blocks:%d sector_count:%ld\n", index, flash->size, (flash->size/flash->sector_size), sector);
	flash_info[index].sector_count = sector;
	/*
	flash_protect(FLAG_PROTECT_SET, flash_info[index].start[0],
			flash_info[index].start[0] + flash->size-1, &flash_info[index]);
	*/
#endif
}
int flash_erase (flash_info_t * info, int s_first, int s_last)
{
	int prot;
	flash_sect_t sect;
	
	if ((s_first < 0) || (s_first > s_last)) {
		puts ("- no sectors to erase\n");
		return 1;
	}

	prot = 0;
	for (sect = s_first; sect <= s_last; ++sect) {
		if (info->protect[sect]) {
			prot++;
		}
	}
	if (prot) {
		printf ("- Warning: %d protected sectors will not be erased!\n", prot);
		return -1;
	} else {
		putc ('\n');
	}
	/* Get the address and convert it to appropriate SPI's address
	 * range and then call the erase function
	 */
	printf("Start is %x and end is %x\n", info->start[s_first], info->start[s_last]);
	int index = 0;
	while((info != flash_info + index)&& (index < 3))
	{
		index ++;
	}
	bootspi_init(index, false);
	bootspi_sector_erase(info->start[s_first], info->start[s_last] - info->start[s_first], false);
	bootspi_deinit(false);
	return 0;
}
// Boot spi will be bus 0
// Backup spi will be bus 1
// Host spi will be bus 2
unsigned long flash_init (void)
{
	unsigned int totalsize = 0;
	int bank = -1, i;
	int temp;
	unsigned int start;
	unsigned int sector;
	unsigned char set4baccess = 0;
	unsigned int cs_sizes[3] = {0};
	unsigned short cs_ids[3] = {FLASH_UNKNOWN};
	if (((* (volatile unsigned int*)(PILOT_STRAP_STATUS_REG)) & 0x40)!=0x40){
		printf("Please enable the 4Byte strap SW1.7\n");
		while(1);
	}
	*(unsigned long*)(PILOT_SPI_BMISC_REG) |= ((0xC0000000) | (1 << 24));  // Make it no override
	flash_bootspi_enumerate_chipselects(false, &totalsize, cs_sizes, cs_ids);

	for (i =0; i < 3; i++)
	{
		memset(&(pilot_spiflash_priv[i]), 0, sizeof(struct spi_flash));
		memset(&(flash_info[i]), 0, sizeof(flash_info_t));

		flash_info[i].start[0] = 0;
		flash_info[i].size = 0;
		flash_info[i].flash_id = FLASH_UNKNOWN;
	}
	flash_info[0].start[0] = 0;
	flash_info[0].size = cs_sizes[0];
	flash_info[0].flash_id = cs_ids[0];
	flash_info[0].sector_count = flash_info[0].size/0x10000;
	start = flash_info[0].start[0];
	sector = 0;
	for(i=0;i<flash_info[0].sector_count;i++)
	{
		flash_info[0].start[sector] = start;
		start += 0x10000;
		sector++;
	}

	flash_info[1].start[0] = cs_sizes[0];
	flash_info[1].size = cs_sizes[1];
	flash_info[1].flash_id = cs_ids[1];
	flash_info[1].sector_count = flash_info[1].size/0x10000;
	start = flash_info[1].start[0];
	sector = 0;
	for(i=0;i<flash_info[1].sector_count;i++)
	{
		flash_info[1].start[sector] = start;
		start += 0x10000;
		sector++;
	}
	flash_info[2].start[0] = cs_sizes[0] + cs_sizes[1];
	flash_info[2].size = cs_sizes[2];
	flash_info[2].flash_id = cs_ids[2];
	flash_info[2].sector_count = flash_info[2].size/0x10000;
	start = flash_info[2].start[0];
	sector = 0;
	for(i=0;i<flash_info[2].sector_count;i++)
	{
		flash_info[2].start[sector] = start;
		start += 0x10000;
		sector++;
	}
	
	for (i =0; i < 3; i++)
	{
		flash_protect(FLAG_PROTECT_SET, flash_info[0].start[0],
			flash_info[i].start[0] + flash_info[i].size-1, 
							&flash_info[i]);
	}
#ifdef CONFIG_FLASH_CFI_MTD
	cfi_mtd_init();
#endif
	printf("Size of flahs from flash_init %x = %x + %x + %x \n",(cs_sizes[0] + cs_sizes[1] + cs_sizes[2]),cs_sizes[0], cs_sizes[1], cs_sizes[2]);
	printf("flash id %x  %x  %x \n",flash_info[0].flash_id, flash_info[1].flash_id , flash_info[2].flash_id);
	return (cs_sizes[0] + cs_sizes[1] + cs_sizes[2]);
#if 0
	for (i = 0; i < CONFIG_MAX_BOOT_SPI_BANKS; i++)
	{
		bank++;

		temp = *(unsigned long*)(PILOT_SPI_BMISC_REG);
		temp &= ~(0xC0000000);
		*(unsigned long*)(PILOT_SPI_BMISC_REG) = (temp | (bank << 30));

		//Probing the SPI bus*/
		pilot_spiflash_priv[bank] = spi_flash_probe(0, bank, 50000000/*ignored parameter in our case*/, 0);
		if (pilot_spiflash_priv[bank] == NULL)
		{
			printf("No SPI device found on bus 0, cs %d\n", i);
		}
		printf("SPI device found on bus 0, cs %d with size %x\n", i, pilot_spiflash_priv[bank]->size);

		cs_size[bank] = (pilot_spiflash_priv[bank]->size/(1024*1024));
		flash_priv_init(bank, pilot_spiflash_priv[bank], totalsize);
		totalsize += pilot_spiflash_priv[bank]->size;
		if(pilot_spiflash_priv[bank]->size > 0x1000000){
			set4baccess = 1;
			if (((* (volatile unsigned int*)(PILOT_STRAP_STATUS_REG)) & 0x40)!=0x40){
				temp = *(unsigned long*)(PILOT_SPI_BMISC_REG);
				temp = temp ^ (bank << 30);
				temp |= (0xC0000000);
				*(unsigned long*)(PILOT_SPI_BMISC_REG) = temp ;
				printf("Devices Populated is >16MB, please enable the 4Byte strap SW1.7\n");
				while(1);
			}

		}else{
			set4baccess = 0;
		}
	}
	if(totalsize > MAX_BOOTSPI_SIZE_LIMIT){
		//truncate it to MAX_BOOTSPI_SIZE_LIMIT
		totalsize = MAX_BOOTSPI_SIZE_LIMIT;
	}
	temp = *(unsigned long*)(PILOT_SPI_BMISC_REG);
	temp &= 0xFFC0C0C0;
	*(unsigned long*)(PILOT_SPI_BMISC_REG) = temp | ((cs_size[0] >> 2) |
			(((cs_size[0]  + cs_size[1]) >> 2) << 8) |
			(((cs_size[0] + cs_size[1] + cs_size[2]) >> 2) <<16));

	*(unsigned long*)(PILOT_SPI_BMISC_REG) &= ~(0xC0000000); // enable cs-0 by default
	*(unsigned long*)(PILOT_SPI_BMISC_REG) |= ((0xC0000000) | (set4baccess << 24));  // Make it no override
	printf("Total size is %x\n", totalsize);
	return totalsize;
err_end:
	return -1;
#endif
}

int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{

#if defined(DEBUG)
	printf("data ptr : %x , addr ptr : %x , count : %x\n", src, addr, cnt);
#endif
	/*void bootspi_read_write(tU8 rd_wr, tU32 address, tU32 idx, tPVU8 data, tU32 len, bool hspi)*/
	bootspi_read_write(0, addr, 0, src, cnt, false);
	return (0);
}

void flash_print_info  (flash_info_t *info)
{
	int i;
	printf("  Size: %ld MB in %d Sectors %d id\n",
				info->size >> 20, info->sector_count, info->flash_id);
#if 0
	printf("  Sector Start Addresses:");
	for (i = 0; i < info->sector_count; i++)
	{
		if ((i % 5) == 0)
			printf ("\n   ");
		printf (" %08lX%s", info->start[i], info->protect[i] ? " (RO)" : "     ");
	}
#endif
	printf ("\n");
	return;
}

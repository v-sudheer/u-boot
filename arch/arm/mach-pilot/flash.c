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
#define BOOTSPI_CTRLR_BASE 0x40200000
#define MAX_BOOTSPI_SIZE_LIMIT 0xFC00000
#define BMISC_REG 0x1C
#define PILOT_SPI_BMISC_REG (BOOTSPI_CTRLR_BASE + BMISC_REG)
flash_info_t flash_info[CONFIG_SYS_MAX_FLASH_BANKS];		/* FLASH chips info */
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

ulong TOTAL_SIZE;


// Since BootSPI size can be 252MB, it is possible that the address range of
// all SPI put together can go beyond 0x10000000 which is LMEM address. Hence
// splitting the SPI address ranges across 2 boundaries
#define NONBOOTSPI_START        (0x10000000)
#define NONBOOTSPI_BOUNDARY (0x20000000)

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
// Boot spi will be bus 0
// Backup spi will be bus 1
// Host spi will be bus 2
unsigned long flash_init (void)
{
	unsigned int totalsize = 0;
	int bank = -1, i;
	int temp;
	unsigned char set4baccess = 0;
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
}
int write_buff (flash_info_t * info, uchar * src, ulong addr, ulong cnt)
{
	return (0);
}
void flash_print_info  (flash_info_t *info)
{
	int i;
	printf("  Size: %ld MB in %d Sectors\n",
				info->size >> 20, info->sector_count);
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

/******************************************************************************
 * Copyright (C) 2018   Priyanjali K <priyanjali.karandi@aspeedtech.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 *****************************************************************************/
#include <common.h>
#include <command.h>
#include "4byteaddressing.h"

extern tU32 cs_addr[];
extern tU32 bootspi_deinit(bool hspi);
extern tU32 bootspi_init(tU8 cs, bool hspi);
extern tU32 bootspi_sector_erase(tU32 address, tU32 size, bool);
extern void bootspi_enumerate_chipselects(bool hspi, tU32 * flashsize);


tU32 flash_size = 0;
int cs = -1;
int file_size = 0;

/*
*
* do_pilot_erase_spi()		- 	the own command is implemented "pilot_erase_spi" to
				 				make erase operations faster.
*							-	pilot_erase_spi bus cs address size 
* @cmdtp					-	pointer to the structure cmd_tbl_t
*
* @flag						-	flag value is 0 for both cs-0 and cs-1
* 
* @argc						-	command line arguments 
*
* @argv						-	pointer to the each of the arguments
*
* called func				-	bootspi_enumerate_chipselects(), bootspi_init(),
*
*								bootspi_sector_erase,bootspi_deinit()
*/

int do_pilot_erase_spi(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	unsigned char bus, cs;
	unsigned long address;
	unsigned long size;
	unsigned int total_size_erased;
	bool hspi;
	tU32  flash_size;
	unsigned int erase_start_addr;
	volatile unsigned int *bmisc = ((volatile unsigned int *)0x4020001c);

	if(argc < 5){
		goto err_print_usage;
	}
	debug("the flag is %d\n",flag);
	bus = simple_strtoul(argv[1], NULL, 16);
	debug("the bus is %d\n",   bus );
	cs = simple_strtoul(argv[2], NULL, 16);
	debug("the cs is %d\n",   cs );
	address = simple_strtoul(argv[3], NULL, 16);
	debug("the address  is %x\n",   address );
	size = simple_strtoul(argv[4], NULL, 16);
	debug("the size  is %x\n", size  );

	if(bus != 0)
		goto err_print_usage;

	if(cs > 2)
		goto err_print_usage;
	if(size == 0)
		goto err_print_usage;
		
	unsigned int bmisc_val= *((volatile unsigned int*) bmisc);
	debug("the bmisc_val is %x\n",bmisc_val);
	bmisc_val= *((volatile unsigned int*) bmisc);
	debug("the bmisc_val is %x\n",bmisc_val);
	debug("the start address is  %x\n",address);
	bootspi_enumerate_chipselects(false,&flash_size);
	debug("the start_address_of cs[0] is %x\n",cs_addr[0]);
	debug("the start_address_of cs[1] is %x\n",cs_addr[1]);
	debug("the start_address_of cs[2] is %x\n",cs_addr[2]);
	debug("the size  is %x\n", size  );
	erase_start_addr=cs_addr[cs]+address;
	printf("Start Add:0x%x Size:0x%x\n",erase_start_addr, size);
	bootspi_init(cs, false);
	bootspi_sector_erase(erase_start_addr, size, false);
	bootspi_deinit(false);
	*((volatile unsigned int*)bmisc)=bmisc_val;
	debug("the bmisc_val is %x\n",bmisc_val);

	return 0;

err_print_usage:
	printf(" Wrong parameters passed \n");
	printf(" Usage - pilot_erase_spi <Bus#> <CS#> <Start Addr> <Size>\n");
	printf("Bus#-Must be 0\n");
	printf("CS#- Can be 0, 1 or 2\n");
	printf("Start Addr - Valid SPI address\n");
	printf("Size - Should be 64K aligned\n");
	return -1;		
}

U_BOOT_CMD(pilot_erase_spi, 5, 1, do_pilot_erase_spi,
        "Erase function, can be used for BOOT SPI fast erase",
        "Usage - pilot_erase_spi <Bus> <CS> <Start Addr> <Size>\n"
        "Bus is 0 for Boot SPI, CS can be 0, 1, 2, \n"
	"Start address and size should be 64KB aligned\n"
);

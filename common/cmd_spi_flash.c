/*******************************************************************************
 *
 *
 * Copyright (C) 2004-2014 Emulex. All rights reserved.
 * EMULEX is a trademark of Emulex.
 * www.emulex.com
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published by the
 * Free Software Foundation.
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
#include <command.h>
#include "4byteaddressing.h"

extern tU32 cs_addr[];
extern tU32 bootspi_deinit(bool hspi);
extern tU32 bootspi_init(tU8 cs, bool hspi);
extern tU32 bootspi_sector_erase(tU32 address, tU32 size, bool);
extern void bootspi_enumerate_chipselects(bool, tU32 *);
extern void bootspi_read_write(tU8 rd_wr, tU32 address, tU32 idx, tPVU8 data, tU32 len, bool hspi);
extern int do_fat_ls(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);
extern long file_fat_read_at(const char *filename, unsigned long pos, void *buffer,
                      unsigned long maxsize);
extern void md5_wd (unsigned char *input, int len, unsigned char output[16],
        unsigned int chunk_sz);


tU32 flash_size = 0;
int cs = -1;
int file_size = 0;

int FLASHBOOTSPI (tVU32 dbg_uart_base, tU8 for_erase, tU8 hspi, tU32 spi_addr, tU32 mem_addr)
{
    unsigned char output[16];
    unsigned char output1[16];
    unsigned int ddr_temp;
    int ret = 0, i = 0;

    //First Make sure it is a device that supports 4 bytes addressing
    tU32 chk = bootspi_init(cs, hspi);
    if(chk !=0)
            return 1;

    printf("Erasing from sector 0x%x", spi_addr);
    chk = bootspi_sector_erase(spi_addr, file_size, hspi);
    if(chk != 0){
            printf("ERR%x\n",ERR_4BYTE_ERASE_FAILED); 
            ret = 1;
            goto cleanup;
    }
    printf("Flashing from 0x%x(DDR) to 0x%x(SPI)", mem_addr, spi_addr);
    bootspi_read_write(0, spi_addr, bootspi_priv, (tPVU8) mem_addr, file_size,hspi);

    tU8 rd_dat= 0;
    printf("\nReading and comparing\n");
    for(chk=0; chk < file_size; chk++){
        bootspi_read_write(1, spi_addr+chk, bootspi_priv, &rd_dat, 1, hspi);
        if(*((tPVU8)(mem_addr+chk)) != rd_dat){
                nc_printf("ERR%x at %x dat is %x\n",ERR_COMPARE_FALIED, chk, rd_dat);
                ret = 1;
                goto cleanup;
        }
        if (chk % PRINT_FREQ == 0) nc_printf(".");
    }
    nc_printf("\nSUCCESS\n");

    // Verify md5
    memset(output, '\0', 16);
    memset(output1, '\0', 16);
    printf("\n\n*** Wait till checksum computation is complete ***\n");
    md5_wd((unsigned char *) mem_addr, file_size, output, CHUNKSZ_MD5);
    printf("md5 checksum of the file\n");
    for (i = 0; i < 16; i++)
        printf("%02x ", output[i]);
    printf("\n");

    ddr_temp = mem_addr + 0x100;
    bootspi_read_write(1, cs_addr[cs], bootspi_priv, (tPVU8)(ddr_temp), file_size, 0);

    printf("md5 checksum of file from SPI\n");
    md5_wd((unsigned char *) (ddr_temp), file_size, output1, CHUNKSZ_MD5);
    for (i = 0; i < 16; i++)
        printf("%02x ", output1[i]);
    printf("\n");

    if (memcmp(output, output1, 16) != 0)
    {
        printf("md5 checksum different, something wrong.. Check and reflash !!!\n");
        ret = 1;
        goto cleanup;
    }
    else
        printf("\nChecksum same, have fun!!!\n\n");


cleanup:
        bootspi_deinit(hspi);
        return ret;
}


#if 0
int do_spiflash(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int i = 0;
	unsigned int spi_addr = 0, mem_addr = 0;
	for(i = 0;i < argc;i++)
		printf("argv[%d] %s\n",i, argv[i]);
	if(strcmp("flash", argv[1]) == 0) {
		mem_addr = simple_strtoul (argv[2], NULL, 16);
		spi_addr = simple_strtoul (argv[3], NULL, 16);
		FLASHBOOTSPI(0, 0, 0, spi_addr, mem_addr);
	}
	else if(strcmp("jump", argv[1]) == 0) {
		void (*jump)(void);
		jump = simple_strtoul (argv[2], NULL, 16);
		printf("Jumping to 0x%x\n", jump);
		jump();
        }else if(strcmp("memjump", argv[1]) == 0) {
		void (*jump)(unsigned int, unsigned char);
		unsigned int mem_addr = 0;
		unsigned int val = 0;
		jump = simple_strtoul (argv[2], NULL, 16);
		mem_addr = simple_strtoul (argv[3], NULL, 16);
		val = simple_strtoul (argv[4], NULL, 16);
		printf("Jumping to address 0x%x with parameters 0x%x 0x%x\n", jump, mem_addr, val);
		jump(mem_addr, val);
        }
}


U_BOOT_CMD(spiflash, 5, 1, do_spiflash,
	"Do spi Flash and jump to SPI Flash\n",
	"spi flash memaddr spi_addr - first tftpboot to memaddr and then call this command\n"
	"spi jump spi_addr\n"
	""
);

int do_flash_bit_file(cmd_tbl_t * cmdtp, int flag, int argc, char *argv[])
{
	int i = 0, res = -1;
	char *my_args[6];
	char arg[6][16];
	extern int do_fat_fsload (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[]);

	if(argc < 3) {
		printf("Enter proper arguments bit_filename flashaddress\n");
		return -1;
	}

	for(i = 0;i < 6;i++)
		my_args[i] = arg[i];

	my_args[0] = "fatload";
	my_args[1] = "mmc";
	my_args[2] = "0";
	my_args[3] = "0x8000";	
	strcpy(arg[4], argv[1]);
	printf("Loading %s filename to %s mem address\n", argv[1], arg[3]);
	res = do_fat_fsload(NULL, 0, 5, my_args);
	printf("res = %d\n", res);
	if(res == 0) {
		my_args[0] = "spiflash";
		my_args[1] = "flash";
		my_args[2] = "0x8000";
		my_args[3] = "0x0";	
		do_spiflash(NULL, 0, 4, my_args);
	}

	return res;
}

U_BOOT_CMD(flashbitfile, 5, 1, do_flash_bit_file,
	"Flash bit file to SPI Flash [0/1]\n",
	"flashbitfile bit_filename flashaddress - \n"
	""
);


#define FILE_NAME   50
#define DDR_ADDR    0x8000
static int do_flashbitfile(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    if (argc < 3) {
        printf("usage: flashbitfile <filename> <cs#>\n");
        printf("Example: flashbitfile pilot4.fpga 0\n");
        return 1;
    }

    file_size = 0; cs = -1;

    cs = simple_strtoul(argv[2], NULL, 16);
    if (cs != 0 && cs != 1)
    {
        printf("Invalid cs:%d\n", cs);
        return 1;
    }

    file_size = file_fat_read_at(argv[1], 0, (void*)DDR_ADDR, 0);
    if (file_size < 1)
    {
        printf("Invalid file\n");
        return 1;
    }

    printf("Loading file:%s to DDR addrs:0x%x\n", argv[1], DDR_ADDR);

    // Copy it to flash
    if (FLASHBOOTSPI(0, 0, 0, cs_addr[cs], DDR_ADDR) != 0)
    {
        printf("Error while flashing CS%d\n", cs);
        return 1;
    }

    return 0;
}


static int do_flashbitls(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{
    char *my_args[6] = {"fatls", "mmc", "0"};
    int ret = -1;

    // List the available bit files
    printf("\n*** Listing available files in SD ***\n");
    ret = do_fat_ls(NULL, 0, 3, my_args);
    if (ret == -1)
    {
        printf("Unable to get files\n");
        return ret;
    }

    // Enumerate and list the available CSs
    printf("*** Listing available SPIs ***\n");
    bootspi_enumerate_chipselects(0, &flash_size);
    printf("\n(NOTE: Use one of the CSs listed above. Sometimes a SPI could\n" \
           "be listed as  DUMMY. This is either because the CS was not\n" \
           "present or could not be recognized. If it is not present then\n" \
           "do not attempt to use it.)\n\n");

    return 0;
}

U_BOOT_CMD(
	flashbitfile, 3, 1, do_flashbitfile,
	"Flash bit file to SPI Flash [0/1]",
        "Example: flashbitfile pilot4.fpga 0"
);

U_BOOT_CMD(
	flashbitls, 1, 1, do_flashbitls,
        "Lists the files in SD and SPI devices",
        "flashbitls"
);

#endif

int display_ss_char(char c)
{
	return 0;
    //Initialize the MIO registers
    *((volatile unsigned long *)(0xE000A204)) = 0x0000FE01;
    *((volatile unsigned long *)(0xE000A208)) = 0x0000FE01;
	switch(c){
		case '0':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE7E00;
		break;
		case '1':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE0C00;
		break;
		case '2':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE3601;
		break;
		case '3':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE1E01;
		break;
		case '4':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE5E01;
		break;
		case '5':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE5A01;
		break;
		case '6':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE7A01;
		break;
		case '7':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE0E00;
		break;
		case '8':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE7E01;
		break;
		case '9':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE4E01;
		break;
		case 'A':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE6E01;
		break;
		case 'b':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE7801;
		break;
		case 'C':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE7200;
		break;
		case 'd':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE3C01;
		break;
		case 'E':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE7201;
		break;
		case 'F':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE6201;
		break;
		case 'H':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE6C01;
		break;
		case 'L':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE7000;
		break;
		case 'o':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE3801;
		break;
		case 'P':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE6601;
		break;
		case 't':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE7001;
		break;
		case 'U':
    		*((volatile unsigned long *)(0xE000A000)) = 0X01FE7C00;
		break;
		default:
        		printf("usage: ss_put <character to print - 0, 1, 2, 3, 4, 5, 6, 7, 8, 9> \n");
	        	printf(" A, b, C, d, E, F, H, L, o, P, t, U\n");
			return 1;
		break;
	}
	return 0;
}
static int do_ss_put(cmd_tbl_t * cmdtp, int flag, int argc, char * const argv[])
{


    if (argc < 2) {
        printf("usage: ss_put <character to print - 0, 1, 2, 3, 4, 5, 6, 7, 8, 9> \n");
        printf(" A, b, C, d, E, F, H, L, o, P, t, U\n");
        return 1;
    }


    unsigned char p = *argv[1];

    printf("Letter is %c\n", p);
    return display_ss_char(*argv[1]);
}

U_BOOT_CMD(
	ss_put, 2, 1, do_ss_put,
        "Lists the files in SD and SPI devices",
        "flashbitls"
);


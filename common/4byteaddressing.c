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
#include "spi_common.h"
#include "4byteaddressing.h"
#include "bootspi.h"

#define BOOTSPI_BASE	0x40200000
tU32 glob_spi_base = BOOTSPI_BASE;

tU32 bootspi_priv;
tU32 bootspi_deinit(bool hspi);
tU32 bootspi_init(tU8 cs, bool hspi);
tU32 bootspi_sector_erase(tU32 address, tU32 size, bool);
void bootspi_enumerate_chipselects(bool, tU32 *);
void bootspi_check_status(tU8 status_bit, tU32 command, bool hspi);

//struct spi_dat id_table[MAX_PARTS]={
struct spi_dat id_table[]={
				//Micron Parts
				{0x20, 0xBA, 0x20, 0x10, "MICRON-512Mb", 1024, 0x4000000,(STS_FLAGS_REG | FBYTE_SUPPORT | SEND_WR_ENA) },
				{0x20, 0xBA, 0x21, 0x1F, "MICRON-1Gb", 1984, 0x7C00000,(STS_FLAGS_REG | FBYTE_SUPPORT | SEND_WR_ENA) },
				{0x20, 0xBA, 0x19, 0x08, "MICRON-256Mb", 512, 0x2000000,(FBYTE_SUPPORT | SEND_WR_ENA | STS_FLAGS_REG)	},
				{0x20, 0xBB, 0x19, 0x08, "MCRN256_NSUPP", 512, 0x2000000,(FBYTE_SUPPORT | SEND_WR_ENA | STS_FLAGS_REG)	},
				//Numonyx Parts
				{0x20, 0xBA, 0x18, 0x04, "Numonyx-128Mb", 256, 0x1000000,(SEND_WR_ENA)		},
				//EON Parts
				{0x1c, 0x70, 0x18, 0x04, "EON-128Mb", 256, 0x1000000,(SEND_WR_ENA) },
				{0x1c, 0x70, 0x19, 0x08, "EON-256Mb", 512, 0x2000000,(FBYTE_SUPPORT | SEND_WR_ENA) },
				//ST Micro
				{0x20, 0x20, 0x15, 0x02, "STMicro-8Mb", 16, 0x0100000,(SEND_WR_ENA) },//cs-0 forcing 8MB
				{0x20, 0x71, 0x15, 0x02, "STMicro-8Mb", 16, 0x0100000,(SEND_WR_ENA) },//cs-0 forcing 8MB
				{0x20, 0x20, 0x16, 0x02, "STMicro-32Mb", 64, 0x0400000,(SEND_WR_ENA) },//cS-0 we are forcing to show 8MB
				{0x20, 0x71, 0x16, 0x02, "STMicro-32Mb", 64, 0x0400000,(SEND_WR_ENA) },//cs-0 we are forcing to show 8MB
				{0x20, 0x20, 0x17, 0x02, "STMicro-64Mb", 128, 0x0800000,(SEND_WR_ENA) },
				{0x20, 0x71, 0x17, 0x02, "STMicro-64Mb", 128, 0x0800000,(SEND_WR_ENA) },
				{0x20, 0x20, 0x18, 0x04, "STMicro-128Mb", 256, 0x1000000,(SEND_WR_ENA) },
				{0x20, 0x71, 0x18, 0x04, "STMicro-128Mb", 256, 0x1000000,(SEND_WR_ENA) },
				//ATMEL
				{0x1F, 0x47, 0x00, 0x02, "ATMEL-32Mb", 64, 0x0400000,(SEND_WR_ENA) },  /*AT25DF321*/ //cs-0 forcing 8MB 
				//SPANSION parts
				{0x01, 0x02, 0x14, 0x02, "SPANSION-8Mb", 16, 0x0100000,(SEND_WR_ENA) },//cs-0 forcing 8MB
				{0x01, 0x02, 0x15, 0x02, "SPANSION-32Mb", 64, 0x0400000,(SEND_WR_ENA) },//cs-0 forcing 8MB
				{0x01, 0x02, 0x16, 0x02, "SPANSION-64Mb", 128, 0x0800000,(SEND_WR_ENA) },
				{0x01, 0x20, 0x18, 0x04, "SPANSION-128Mb", 256, 0x1000000,(SEND_WR_ENA) },
				//INTEL parts
				{0x89, 0x89, 0x11, 0x02, "Intel-16Mb", 32, 0x0200000,(SEND_WR_ENA | UNPROTECT_SECTORS) }, //Special part programming 
															  //CS-0 1MB range
													   		//though the part is 2MB
				{0x89, 0x89, 0x12, 0x02, "Intel-32Mb", 64, 0x0400000,(SEND_WR_ENA | UNPROTECT_SECTORS) }, //cs-0 forcing 8MB
				{0x89, 0x89, 0x13, 0x02, "Intel-64Mb", 128, 0x0800000,(SEND_WR_ENA|UNPROTECT_SECTORS) },
				//SST part
				{0xBF, 0x25, 0x4A, 0x02, "SST-32Mb", 64, 0x0400000,(SEND_WR_ENA|UNPROTECT_SECTORS|REG_RW) },//cs-0 forcing 8MB
				//Macronix Parts
				{0xC2, 0x20, 0x19, 0x08, "Macronix-256Mb", 512, 0x2000000,(FBYTE_SUPPORT | SEND_WR_ENA) },
				{0xC2, 0x20, 0x18, 0x04, "Macronix-128Mb", 256, 0x1000000,(SEND_WR_ENA) },
				{0xC2, 0x20, 0x1a, 0x10, "Macronix-512Mb", 1024,0x4000000,(FBYTE_SUPPORT | SEND_WR_ENA) },
	
				//WINBOND part
				{0xEF, 0x40, 0x16, 0x02, "WINBOND-32Mb", 64, 0x0400000,(SEND_WR_ENA) },//cs-0 forcing 8MB
				{0xEF, 0x40, 0x17, 0x02, "WINBOND-64Mb", 128, 0x0800000,(SEND_WR_ENA) },
				{0xEF, 0x40, 0x18, 0x04, "WINBOND-128Mb", 256, 0x1000000,(SEND_WR_ENA) },
				//SOS Entry - If any chip is not supported does the best of effort to erase and program
				//Few sectors using 3 byte addressing.
				{0x00, 0x00, 0x00, 0x02, "DUMMY"	,  128, 0x0800000, (SEND_WR_ENA)	}	
			  };
#define FLASH_PAGE_SiZE 256
static void wait_for_spi_rd_ff_notempty_ (bool hspi) {
  if(hspi == 0)
  	return;
  while((*(tPVU8)(glob_spi_base + SPI_STS) ) & rd_fifo_empty);
  return;
}
static void wait_for_spi_not_busy (void) {

  while( (*(tPVU8)(glob_spi_base + SPI_STS) ) & spi_busy) ;
  return;
}

static void disable_spi_controller (void) {
  *(tPVU8)(glob_spi_base + SPI_CTRL) &= (~(spi_en)); 
}

// Function to enable Boot SPI controller
static void enable_spi_controller (void) {
  *(tPVU8)(glob_spi_base + SPI_CTRL) |= (spi_en); 
}

static void wait_for_spi_wr_ff_empty_ (bool hspi) {
  if(hspi == 0)
  	return;
  while(((*(tPVU8)(glob_spi_base + SPI_STS) ) & wr_fifo_empty) != wr_fifo_empty);
  return;
}

static void bhspi_set_clkdiv (tU16 val, bool hspi) {
	disable_spi_controller ();
	if (val == 0x02 && !hspi)
		*(tPVU8) (glob_spi_base + SPI_MISC) |= (fast_read_en);
	*(tPVU16) (glob_spi_base + SPI_CLKDIV) = val;
	enable_spi_controller ();
}

//Low-level
//NOTE unaligned address means no address to be programmed
void bootspi_program_command(tU32 address, tU32 command, tU32 * idx, bool hspi){

	//nc_printf("Addr is %x Cmd is %x\n",address, command);
	if(address != 0xFFFFFFFF){
		*(tPVU32)(glob_spi_base + BOOTSPI_ADDR) = address;
	}
	*((tPVU32) (glob_spi_base + BOOTSPI_CMD0)) = command;
	wait_for_spi_not_busy();
}
#if 0
void bootspi_read_only(tU32 address, tU32 idx, tPVU8 data){

	bootspi_program_command((address), (((id_table[idx].flags & FBYTE_SUPPORT) == FBYTE_SUPPORT)? 0x80000503 : 0x80000403),
						&bootspi_priv);

	*data = *((tPVU8) (BOOTSPI_BASE + BOOTSPI_RWDATA));
}
#endif	
void bootspi_reg_read_write(tU8 rd_wr, tU32 address, tU32 idx, tPVU8 data, tU32 len, bool hspi){

        int i;
        for(i=0; i< len; i++){
                bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv,hspi);
                bootspi_program_command((address+i),
                                                0x80001402,
                                                &bootspi_priv,hspi);
                *(tPVU8) (glob_spi_base + BOOTSPI_RWDATA) = *(((tPVU8)data)+ i);
                bootspi_check_status(0, 0x80002105, hspi);
        }

}

//void bootspi_read_write(tU8 rd_wr, tU32 address, tU32 idx, tPVU32 data, tU32 len){
void bootspi_read_write(tU8 rd_wr, tU32 address, tU32 idx, tPVU8 data, tU32 len, bool hspi){
	int i,j;
	tU32 nopgs;
  	nopgs = len/FLASH_PAGE_SiZE;
	tU32 pagesize = 0, loopsize = 0, rdwr_ind = 0;
	tU32 start_address = address;

	if((((id_table[idx].flags & REG_RW)) == REG_RW) && (rd_wr == 0))
                return bootspi_reg_read_write(rd_wr, address, idx, data, len, hspi);

	if ((len%FLASH_PAGE_SiZE) != 0)
		nopgs = nopgs + 1;
	for(j=0; j<(nopgs); j++){//Loop for number of pages

	if((rd_wr == 0) && ((id_table[idx].flags & SEND_WR_ENA) == SEND_WR_ENA))
		//Write enable command
		bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);
	//Write /Read Command
	bootspi_program_command((address), ((rd_wr ==1) ? (((id_table[idx].flags & FBYTE_SUPPORT) == FBYTE_SUPPORT)? 0x80000503 : 0x80000403): 
						(((id_table[idx].flags & FBYTE_SUPPORT) == FBYTE_SUPPORT)? 0x80001502 : 0x80001402)),
						&bootspi_priv, hspi);

	if(!rd_wr && ((j*FLASH_PAGE_SiZE) % PRINT_FREQ) == 0)
		printf(".");
	/* If address is not page aligned then first write data bytes of that page
	  * and from then on the logic will take care of writing to aligned pages
	  */
	if((address % FLASH_PAGE_SiZE) != 0) {
		pagesize =FLASH_PAGE_SiZE - (address & (FLASH_PAGE_SiZE - 1));
		if(rd_wr == 0) {
			nc_printf("Write to non aligned pg size adress %x pgsize %d\n", 
			address, pagesize);
		}
		rdwr_ind = address - start_address;
		address = address + pagesize;
		loopsize = pagesize;
		if(pagesize >= len) 
			loopsize = len;
	} else {
		rdwr_ind = address - start_address;
		loopsize = ((len>= FLASH_PAGE_SiZE)? FLASH_PAGE_SiZE:len);
		address = address + FLASH_PAGE_SiZE;
	}
	for(i=0; i<loopsize; i++){
		if(rd_wr){
			wait_for_spi_rd_ff_notempty_(hspi);
			*(((tPVU8)(data)) + rdwr_ind + i) = *(tPVU8) (glob_spi_base + BOOTSPI_RWDATA);
		}else{
			wait_for_spi_wr_ff_empty_(hspi);
			*(tPVU8) (glob_spi_base + BOOTSPI_RWDATA) = *(((tPVU8)(data)) + rdwr_ind + i);  
	
		}
	}
	len -= (i-1);
	//0x7 means check flag status
	if(((id_table[bootspi_priv].flags & STS_FLAGS_REG) == STS_FLAGS_REG) && (rd_wr != 1)){
		bootspi_check_status(0x7, 0x80000170, hspi);
		//nc_printf("\n");
	}
	bootspi_check_status(0, 0x80002105, hspi);
	}//For loop of pages
}	
void bootspi_unlock_all_sectors(bool hspi){
        tU8 data=0;
        bootspi_program_command(0xFFFFFFFF, 0x80002105, &bootspi_priv, hspi);
        data = (*(tPVU8) (glob_spi_base+ BOOTSPI_RWDATA));
        data &= 0x63;//Disable protect for all sectors bit 4,3,2 and make these bits read writable


        //Just for testing
        bootspi_program_command(0xFFFFFFFF, 0x80002105, &bootspi_priv, hspi);
        data = (*(tPVU8) (glob_spi_base + BOOTSPI_RWDATA));
        //nc_printf("The status register before UNProtect is %x\n", data);
        data &= 0x63;//Disable protect for all sectors bit 4,3,2 and make these bits read writable
        //nc_printf("The status register for UNProtect is %x\n", data);

        //Write enable
        bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);

        //Enable Write Status register
        bootspi_program_command(0xFFFFFFFF, 0x80001150, &bootspi_priv, hspi);
        //Write to status register to Unportect all sectors
        bootspi_program_command(0xFFFFFFFF, ((0x80001101)), &bootspi_priv, hspi);
        *(tPVU8) (glob_spi_base + BOOTSPI_RWDATA) = *((tPVU8)data);
        //Just for testing
        bootspi_program_command(0xFFFFFFFF, 0x80002105, &bootspi_priv, hspi);
        data = (*(tPVU8) (glob_spi_base + BOOTSPI_RWDATA));
        //nc_printf("The status register After UNProtect is %x\n", data);
        //Write enable
        bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);

        //Enable Write Status register
        bootspi_program_command(0xFFFFFFFF, 0x80001150, &bootspi_priv, hspi);
        //Write to status register to Unportect all sectors
        bootspi_program_command(0xFFFFFFFF, ((0x80001101)), &bootspi_priv, hspi);
        *(tPVU8) (glob_spi_base + BOOTSPI_RWDATA) = *((tPVU8)data);
        //Just for testing
        bootspi_program_command(0xFFFFFFFF, 0x80002105, &bootspi_priv, hspi);
        data = (*(tPVU8) (glob_spi_base + BOOTSPI_RWDATA));
        //nc_printf("The status register After UNProtect second time is %x\n", data);
}

void bootspi_check_status(tU8 status_bit, tU32 command, bool hspi){
	tU8 data=0;
	*(tPVU32) (glob_spi_base + BOOTSPI_CMD0) = command;
	wait_for_spi_not_busy();

	wait_for_spi_rd_ff_notempty_ (hspi);

	data = *(tPVU8) (glob_spi_base + BOOTSPI_RWDATA);
	
	if(status_bit) {	
		while((data & (1<<status_bit)) != (1<<status_bit))
		{
			  wait_for_spi_rd_ff_notempty_ (hspi);
			data = *(tPVU8) (glob_spi_base + BOOTSPI_RWDATA);
			//nc_printf("SPI STS : %x\n", data);
		};
	} else {
		while((data & (1<<status_bit)) == (1<<status_bit))
		{
			  wait_for_spi_rd_ff_notempty_ (hspi);
			data = *(tPVU8) (glob_spi_base + BOOTSPI_RWDATA);
			//nc_printf("SPI STS : %x\n", data);
		};
	}
	//bootspi_change_acessmode_tomem();
}
//High-level
void bootspi_full_erase(tU8 cs, bool hspi){
#if 0
	bootspi_enumerate_chipselects();
	if((bootspi_init(cs))){
		return;
	}
#endif
	nc_printf("Erasing CS-%x cs_addr[cs] %x hspi %d\n",cs, cs_addr[cs], hspi);
	bootspi_sector_erase(cs_addr[cs], id_table[bootspi_priv].size, hspi);
	//bootspi_deinit();
}		

tU32 bootspi_readid(bool hspi){
	tU32 id;
	//bootspi_program_command(0xFFFFFFFF, 0x8000019F, MAX_PARTS);
	bootspi_program_command(0xFFFFFFFF, 0x8000019F, 0/*unused*/, hspi);
if (hspi)
{
	wait_for_spi_rd_ff_notempty_ (hspi);
	id = *(tPVU8)(glob_spi_base + SPI_RWDATA);
	wait_for_spi_rd_ff_notempty_ (hspi);
	id |= (*(tPVU8)(glob_spi_base + SPI_RWDATA) << 8);
	wait_for_spi_rd_ff_notempty_ (hspi);
	id |= (*(tPVU8)(glob_spi_base + SPI_RWDATA) << 16);
	wait_for_spi_rd_ff_notempty_ (hspi);
	id |= (*(tPVU8)(glob_spi_base + SPI_RWDATA) << 24);
	return id;
}
else
	return ((*(tPVU32)(glob_spi_base + BOOTSPI_RWDATA)));
}	

tU32 bootspi_deinit(bool hspi){
	tU32 temp_data;
	if((id_table[bootspi_priv].flags & FBYTE_SUPPORT) == (FBYTE_SUPPORT)){
#if 1
		//Write enable command
		if((id_table[bootspi_priv].flags & SEND_WR_ENA) == (SEND_WR_ENA)){
			bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);
			//bootspi_change_acessmode_tomem();
		}
#endif
        	//Exit 4-byte command
        	bootspi_program_command(0xFFFFFFFF, 0x800011e9, &bootspi_priv, hspi);
#if 0
		//Writing to desel the chip select
		*(tPVU8)(glob_spi_base + BOOTSPI_CMD3) &= 0x7f;
		*(tPVU8)(glob_spi_base + BOOTSPI_CMD3) |= 0x01;

		// Waiting for Chip select to be deasserted.
		while(((*(tPVU8)(glob_spi_base + BOOTSPI_CMD3)) & 0x01) != 0x00);
#endif
		//Clearing the "use_4thbyte_addr" bit in MISC register -- MISC[24]
		temp_data = *(tPVU32) (glob_spi_base + BOOTSPI_MISC);
		temp_data = ((temp_data & 0xfeffffff) | 0x00000000);
		*(tPVU32) (glob_spi_base + BOOTSPI_MISC) = temp_data;
        }
	return 0;
}

tU32 cs_addr[3] ={0};
void bootspi_enumerate_chipselects(bool hspi, tU32 * flash_size){
//Here select each of the chipselects and read ID in each of them
//And program the MISC regsiter accordingly
	int i,j;
	tU32 temp_data;
	tU32 id_value;
	tU8  cs_uaddr[3];
	tU8 num = 1;
	if(hspi)
		num = 0;
	bhspi_set_clkdiv(0x2, hspi);
	for(j=num; j>= 0; j--){
		if(hspi == false) {
			id_value = *(tPVU32) (glob_spi_base + BOOTSPI_MISC);
			id_value &= ~(0xC0000000);
			//Enable CS(0,1&2)
			*(tPVU32) (glob_spi_base + BOOTSPI_MISC) = id_value| (j<<30);
		}
		id_value = bootspi_readid(hspi);
		for(i=0; i<(sizeof(id_table)/sizeof(struct spi_dat)); i++){
			if((id_value & 0xFF) == id_table[i].uc_MFRID){
				if(((id_value & 0x0000FF00)>>8) == id_table[i].uc_MTYPE){
					if(((id_value & 0x00FF0000)>>16) == id_table[i].uc_MCAP){
						nc_printf("Part in CS-%x is %s \n", j, id_table[i].auc_name);
						break;
					}
				}
			}
		}
		if(i == (sizeof(id_table)/sizeof(struct spi_dat))){
			nc_printf("ERR for CS%d ID is %x\n", j, ERR_NO_MATCH_ID);
			if((id_value ==0x00) || (id_value == 0xFFFFFFFF)){
                                //No chip is populated
                                cs_addr[j] = 0x00;
                                cs_uaddr[j] = 0x00;
                                continue;
                        }
                        //Unsupported Chip
			cs_addr[j] = id_table[i-1].size;
			continue;
		}
		cs_addr[j] = id_table[i].size;
		cs_uaddr[j] = id_table[i].uc_UPADDR;
	}
	if(hspi) {
		cs_addr[0] = 0x00;
		*flash_size = id_table[i].size;
		return;
	}
	temp_data = *(tPVU32) (glob_spi_base + BOOTSPI_MISC);
	temp_data &= 0xFFE0E0E0;
	*(tPVU32) (glob_spi_base + BOOTSPI_MISC) = temp_data | (cs_uaddr[0] | ((cs_uaddr[0] + cs_uaddr[1]) <<8) | ((cs_uaddr[0] + cs_uaddr[1]+cs_uaddr[2]) << 16)); 
	*(tPVU32) (glob_spi_base + BOOTSPI_MISC) &= ~(0xC0000000); //By default CS-0 enabled after enumeration.
	*(tPVU32) (glob_spi_base + BOOTSPI_MISC) |= (0xC0000000); //Make it no override.
	cs_addr[2] = cs_addr[1]+cs_addr[0];;
	cs_addr[1] = cs_addr[0];
	cs_addr[0] = 0x00;
}
tU32 bootspi_init(tU8 cs, bool hspi){
	int i;
	tU32 id_value;
	if(hspi == 0)
		*(tPVU32)(spi_base + BOOTSPI_ADDR) = cs_addr[cs];
	//Set CS-0 for programming
	id_value = bootspi_readid(hspi);
	if((id_value&0xFFFFFF) == 0xFFFFFF){
		nc_printf("No SPI on this Chip Select\n");
		return ERR_NO_MATCH_ID;
	}
		
	for(i=0; i<(sizeof(id_table)/sizeof(struct spi_dat)); i++){
		if((id_value & 0xFF) == id_table[i].uc_MFRID){
			if(((id_value & 0x0000FF00)>>8) == id_table[i].uc_MTYPE){
				if(((id_value & 0x00FF0000)>>16) == id_table[i].uc_MCAP){
					//nc_printf("Part is %s\n", id_table[i].auc_name);
					break;
				}
			}
		}
	}	
	if(i == (sizeof(id_table)/sizeof(struct spi_dat))){
		nc_printf("No spi device on CS%d ID\n",i);
	}
	bootspi_priv=i;
	//Check if by default they have to be unprotected
	if(((id_table[bootspi_priv].flags & UNPROTECT_SECTORS) == UNPROTECT_SECTORS)){
		bootspi_unlock_all_sectors(hspi);
	}
	//Enable the 4 byte support by default
	if(((id_table[bootspi_priv].flags & FBYTE_SUPPORT) == FBYTE_SUPPORT)){

		//This is the MAC0STNGCTL[16] this works only when BMISC[24] is set to 1
		*(tPVU32) (glob_spi_base + BOOTSPI_MISC) |= 0x01000000;
		*((tPVU32) (0x40100190)) |= 0x10000;

		if((id_table[bootspi_priv].flags & SEND_WR_ENA) == (SEND_WR_ENA)){
			//Write enable command
			bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);
		}
		//Enable 4-byte command
		bootspi_program_command(0xFFFFFFFF, 0x800011b7, &bootspi_priv, hspi);
	}
	return 0;
}

tU32 bootspi_sector_erase(tU32 address, tU32 size, bool hspi){
	tU32 erasesects;
	int i;

	if(size>(64*1024)){
                erasesects=(size/(64*1024))+1;
        }else{
                erasesects=1;
	}
	for(i=0; i < erasesects; i++){

	if((id_table[bootspi_priv].flags & SEND_WR_ENA) == (SEND_WR_ENA)){
		//Write enable command
		bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);
	}
	//Sector Erase Command
	bootspi_program_command((address+(i<<16)), ((id_table[bootspi_priv].flags & FBYTE_SUPPORT) != FBYTE_SUPPORT)? 0x800014D8: 0x800015D8, &bootspi_priv, hspi);
	//0x7 means check flag status
	if((id_table[bootspi_priv].flags & STS_FLAGS_REG) == STS_FLAGS_REG){
		bootspi_check_status(0x7, 0x80000170, hspi);
	}
	bootspi_check_status(0, 0x80002105, hspi);
	nc_printf(".");
	}
	nc_printf("\n");
	return 0;
}

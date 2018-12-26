/******************************************************************************
 * Copyright (C) 2018 Priyanjali K <priyanjali.karandi@aspeedtech.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 *
 *****************************************************************************/
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
/*struct spi_dat id_table[MAX_PARTS]={*/
struct spi_dat id_table[]={
				/*Micron Parts*/
	{0x20, 0xBA, 0x20, 0x10, "MICRON-512Mb", 1024, 0x4000000,\
				(STS_FLAGS_REG | FBYTE_SUPPORT | SEND_WR_ENA)},
	{0x20, 0xBA, 0x21, 0x20, "MICRON-1Gb", 2048, 0x8000000,\
				(STS_FLAGS_REG | FBYTE_SUPPORT | SEND_WR_ENA)},
	{0x20, 0xBA, 0x19, 0x08, "MICRON-256Mb", 512, 0x2000000,\
				(FBYTE_SUPPORT | SEND_WR_ENA | STS_FLAGS_REG)},
	{0x20, 0xBB, 0x19, 0x08, "MCRN256_NSUPP", 512, 0x2000000,\
				(FBYTE_SUPPORT | SEND_WR_ENA | STS_FLAGS_REG)},
				/*Numonyx Parts*/
	{0x20, 0xBA, 0x18, 0x04, "Numonyx-128Mb", 256, 0x1000000,\
				(SEND_WR_ENA)},
				/*EON Parts*/
	{0x1c, 0x70, 0x18, 0x04, "EON-128Mb", 256, 0x1000000,\
				(SEND_WR_ENA)},
	{0x1c, 0x70, 0x19, 0x08, "EON-256Mb", 512, 0x2000000,\
				(FBYTE_SUPPORT | SEND_WR_ENA)},
				/*ST Micro*/
	{0x20, 0x20, 0x15, 0x02, "STMicro-8Mb", 16, 0x0100000,\
				(SEND_WR_ENA)},/*cs-0 forcing 8MB*/
	{0x20, 0x71, 0x15, 0x02, "STMicro-8Mb", 16, 0x0100000,\
				(SEND_WR_ENA)},/*cs-0 forcing 8MB*/
	{0x20, 0x20, 0x16, 0x02, "STMicro-32Mb", 64, 0x0400000,\
				(SEND_WR_ENA)},/*cs-0 we are forcing to show 8MB*/
	{0x20, 0x71, 0x16, 0x02, "STMicro-32Mb", 64, 0x0400000,\
				(SEND_WR_ENA)},/*cs-0 we are forcing to show 8MB*/
	{0x20, 0x20, 0x17, 0x02, "STMicro-64Mb", 128, 0x0800000,\
				(SEND_WR_ENA)},
	{0x20, 0x71, 0x17, 0x02, "STMicro-64Mb", 128, 0x0800000,\
				(SEND_WR_ENA)},
	{0x20, 0x20, 0x18, 0x04, "STMicro-128Mb", 256, 0x1000000,\
				(SEND_WR_ENA)},
	{0x20, 0x71, 0x18, 0x04, "STMicro-128Mb", 256, 0x1000000,\
				(SEND_WR_ENA)},
				/*ATMEL*/
	{0x1F, 0x47, 0x00, 0x02, "ATMEL-32Mb", 64, 0x0400000,\
				(SEND_WR_ENA)},  /*AT25DF321*/ /*cs-0 forcing 8MB*/
				/*SPANSION parts*/
	{0x01, 0x02, 0x14, 0x02, "SPANSION-8Mb", 16, 0x0100000,\
				(SEND_WR_ENA)},/*cs-0 forcing 8MB*/
	{0x01, 0x02, 0x15, 0x02, "SPANSION-32Mb", 64, 0x0400000,\
				(SEND_WR_ENA)},/*cs-0 forcing 8MB*/
	{0x01, 0x02, 0x16, 0x02, "SPANSION-64Mb", 128, 0x0800000,\
				(SEND_WR_ENA)},
	{0x01, 0x20, 0x18, 0x04, "SPANSION-128Mb", 256, 0x1000000,\
				(SEND_WR_ENA)},
				/*INTEL parts*/
	{0x89, 0x89, 0x11, 0x02, "Intel-16Mb", 32, 0x0200000,\
				(SEND_WR_ENA | UNPROTECT_SECTORS)},
			/*Special part programming
			CS-0 1MB range
			though the part is 2MB*/
	{0x89, 0x89, 0x12, 0x02, "Intel-32Mb", 64, 0x0400000,\
				(SEND_WR_ENA | UNPROTECT_SECTORS)},/*cs-0 forcing 8MB*/
	{0x89, 0x89, 0x13, 0x02, "Intel-64Mb", 128, 0x0800000,\
				(SEND_WR_ENA|UNPROTECT_SECTORS)},
				/*SST part*/
	{0xBF, 0x25, 0x4A, 0x02, "SST-32Mb", 64, 0x0400000,\
				(SEND_WR_ENA|UNPROTECT_SECTORS|REG_RW)},/*cs-0 forcing 8MB*/
				/*Macronix Parts*/
	{0xC2, 0x20, 0x19, 0x08, "Macronix-256Mb", 512, 0x2000000,\
				(FBYTE_SUPPORT | SEND_WR_ENA)},
	{0xC2, 0x20, 0x18, 0x04, "Macronix-128Mb", 256, 0x1000000,\
				(SEND_WR_ENA)},
	{0xC2, 0x20, 0x1a, 0x10, "Macronix-512Mb", 1024,0x4000000,\
				(FBYTE_SUPPORT | SEND_WR_ENA)},
	
				/*WINBOND part*/
	{0xEF, 0x40, 0x16, 0x02, "WINBOND-32Mb", 64, 0x0400000,\
				(SEND_WR_ENA)},/*cs-0 forcing 8MB*/
	{0xEF, 0x40, 0x17, 0x02, "WINBOND-64Mb", 128, 0x0800000,\
				(SEND_WR_ENA) },
	{0xEF, 0x40, 0x18, 0x04, "WINBOND-128Mb", 256, 0x1000000,\
				(SEND_WR_ENA)},
	/*SOS Entry - If any chip is not supported does 
	the best of effort to erase and program
	Few sectors using 3 byte addressing.*/
	{0x00, 0x00, 0x00, 0x02, "DUMMY"	,  128, 0x0800000,\
				 (SEND_WR_ENA)}	
			  };
#define FLASH_PAGE_SiZE 256

/*
*	wait_for_spi_rd_ff_notempty_()		-	Reading the FIFO 
*											when its not empty
* 
* 	@hspi								-	True or False
*											(False in our case)
*/

static void wait_for_spi_rd_ff_notempty_ (bool hspi) {
	if(hspi == 0)
		return;
	while((*(tPVU8)(glob_spi_base + SPI_STS) ) & rd_fifo_empty);
	return;
}

/*
*	wait_for_spi_not_busy() 			-	checks untill the spi 
*											is busy and comes out
*											when its no more busy
*/
static void wait_for_spi_not_busy (void) {

	while((*(tPVU8)(glob_spi_base + SPI_STS)) & spi_busy);
	return;
}

/*
*	disable_spi_controller()			-	Disables the Boot 
*											SPI controller
*/
static void disable_spi_controller (void) {
	*(tPVU8)(glob_spi_base + SPI_CTRL) &= (~(spi_en)); 
}

/*
*	enable_spi_controller()				-	Enables the Boot 
*											SPI controller
*/
static void enable_spi_controller (void) {
	*(tPVU8)(glob_spi_base + SPI_CTRL) |= (spi_en); 
}

/*
*	bhspi_set_clkdiv()					-	Enables Clock Divisor 
*											register w.r.t cs
* 
*	@hspi								-	True or False
*											(False in our case)
*/
static void bhspi_set_clkdiv (tU16 val, bool hspi) {
	disable_spi_controller ();
	if (val == 0x02 && !hspi)
		*(tPVU8) (glob_spi_base + SPI_MISC) |= (fast_read_en);
	*(tPVU16) (glob_spi_base + SPI_CLKDIV) = val;
	enable_spi_controller ();
}


/*
*	bootspi_program_command()			-	Details the command bits 
*											to program the command 
*											registers
*			             
*                                 		-	Low-level,NOTE:unaligned 
*											address means no address 
*											to be programmed
* 
*	@address							-	32-bit address 
*
*	@command							-	to enable the write or 
*											read enabling cmd
*
*	@idx								-	the member in the structure 
*
*	@hspi								-	True or False
*											(False in our case)
*/
void bootspi_program_command(tU32 address, tU32 command, tU32 * idx, bool hspi){

	if(address != 0xFFFFFFFF){
		*(tPVU32)(glob_spi_base + BOOTSPI_ADDR) = address;
	}
	*((tPVU32) (glob_spi_base + BOOTSPI_CMD0)) = command;
	wait_for_spi_not_busy();
}

/* 
*	bootspi_reg_read_write()			-	Performs the read and write
*											to the Boot spi reg
*
* 	@rd_wr								-	Read or Write to be performed
*  
*	@address							-	32-bit address 
*
*	@idx								-	Member of struct spi_dat
*
*	@data								-	8-bit data
*
*	@len								-	length of the data
*
*	@hspi								-	True or Flase
*											(False in our case)
*/
void bootspi_reg_read_write(tU8 rd_wr, tU32 address, tU32 idx, 
						tPVU8 data, tU32 len, bool hspi){

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

/*
*	bootspi_unlock_all_sectors()		-	unlocks all the sectors
*
*	@hspi								-	True or False
*											(False in our case)
*/
void bootspi_unlock_all_sectors(bool hspi){
	tU8 data=0;
	bootspi_program_command(0xFFFFFFFF, 0x80002105, &bootspi_priv, hspi);
	data = (*(tPVU8) (glob_spi_base+ BOOTSPI_RWDATA));
	data &= 0x63;/*Disable protect for all sectors bit 4,3,2 
					and make these bits read writable*/


	/*Just for testing*/
	bootspi_program_command(0xFFFFFFFF, 0x80002105, &bootspi_priv, hspi);
	data = (*(tPVU8) (glob_spi_base + BOOTSPI_RWDATA));
	data &= 0x63;/*Disable protect for all sectors bit 4,3,2 
					and make these bits read writable*/

	/*Write enable*/
	bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);
	/*Enable Write Status register*/
	bootspi_program_command(0xFFFFFFFF, 0x80001150, &bootspi_priv, hspi);
	/*Write to status register to Unportect all sectors*/
	bootspi_program_command(0xFFFFFFFF, ((0x80001101)), &bootspi_priv, hspi);
	*(tPVU8) (glob_spi_base + BOOTSPI_RWDATA) = *((tPVU8)data);
	/*Just for testing*/
	bootspi_program_command(0xFFFFFFFF, 0x80002105, &bootspi_priv, hspi);
	data = (*(tPVU8) (glob_spi_base + BOOTSPI_RWDATA));
	/*Write enable*/
	bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);
	/*Enable Write Status register*/
	bootspi_program_command(0xFFFFFFFF, 0x80001150, &bootspi_priv, hspi);
	/*Write to status register to Unportect all sectors*/
	bootspi_program_command(0xFFFFFFFF, ((0x80001101)), &bootspi_priv, hspi);
	*(tPVU8) (glob_spi_base + BOOTSPI_RWDATA) = *((tPVU8)data);
	/*Just for testing*/
	bootspi_program_command(0xFFFFFFFF, 0x80002105, &bootspi_priv, hspi);
	data = (*(tPVU8) (glob_spi_base + BOOTSPI_RWDATA));
}

/*
*
*	bootspi_check_status()		-	checks the status of the Boot SPI
*
*	@status_bit					-	In which status the spi is 
*									present will come to know
*
*	@command					-	32-bit command
*
*	@hspi						-	True or False
*									(False in our case)
*/
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
		};
	
	} else {
		while((data & (1<<status_bit)) == (1<<status_bit))
		{
			wait_for_spi_rd_ff_notempty_ (hspi);
			data = *(tPVU8) (glob_spi_base + BOOTSPI_RWDATA);
		};
	}
}

/*
*	bootspi_readid()		-	Reads the Boot SPI according to id
*
*	@hspi					-	True or False
*								(False in our case)
*/
tU32 bootspi_readid(bool hspi){
	tU32 id;
	/*bootspi_program_command(0xFFFFFFFF, 0x8000019F, MAX_PARTS);*/
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

/*
*	bootspi_deinit()		-	clears theb 24th bit of the 
*								BMISC register
*
*	@hspi					-	True or False
*								(False in our case)
*
*	called func				-	bootspi_program_command()
*/
tU32 bootspi_deinit(bool hspi){
	tU32 temp_data;
	if((id_table[bootspi_priv].flags & FBYTE_SUPPORT) == (FBYTE_SUPPORT)){
#if 1
		/*Write enable command*/
		if((id_table[bootspi_priv].flags & SEND_WR_ENA) == (SEND_WR_ENA)){
			bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);
			/*bootspi_change_acessmode_tomem();*/
		}
#endif
        	/*Exit 4-byte command*/
		bootspi_program_command(0xFFFFFFFF, 0x800011e9, &bootspi_priv, hspi);
		/*Clearing the "use_4thbyte_addr" bit in MISC register -- MISC[24]*/
		temp_data = *(tPVU32) (glob_spi_base + BOOTSPI_MISC);
		temp_data = ((temp_data & 0xfeffffff) | 0x00000000);
		*(tPVU32) (glob_spi_base + BOOTSPI_MISC) = temp_data;
	}
	return 0;
}
/*
* 
*	bootspi_enumerate_chipselects()		-	Here select each of the 
*											chipselects and read 
*											ID in each of them
*											And program the MISC 
*											regsiter accordingly
*	@hspi								-	True or False
*											(False in our case)
*
*	@flash_size							-	pointer to the member of
*											the spi_dat structure
*
*	called func							-	bootspi_readid(),
*											bhspi_set_clkdiv()
*/
tU32 cs_addr[3] ={0};
void bootspi_enumerate_chipselects(bool hspi, tU32 * flash_size){
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
			/*Enable CS(0,1&2)*/
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
                                /*No chip is populated*/
                                cs_addr[j] = 0x00;
                                cs_uaddr[j] = 0x00;
                                continue;
                        }
                        /*Unsupported Chip*/
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
	*(tPVU32) (glob_spi_base + BOOTSPI_MISC) = temp_data | (cs_uaddr[0] |
												((cs_uaddr[0] + cs_uaddr[1]) <<8)|
												((cs_uaddr[0] + cs_uaddr[1]
												+cs_uaddr[2]) << 16)); 
	*(tPVU32) (glob_spi_base + BOOTSPI_MISC) &= ~(0xC0000000); /*By default CS-0 enabled after enumeration.*/
	*(tPVU32) (glob_spi_base + BOOTSPI_MISC) |= (0xC0000000); /*Make it no override*/
	cs_addr[2] = cs_addr[1]+cs_addr[0];
	cs_addr[1] = cs_addr[0];
	cs_addr[0] = 0x00;
}
/*
*
*	bootspi_init()					-		sets the respective cs for programming
*
*	@cs								-		cs-0,cs-1,cs-2
*
*	@hspi							-		True or False
*											(False in our case)
* 
*	called func						-		bootspi_unlock_all_sectors(),
*											bootspi_program_command(),
*											bootspi_readid()
*/
tU32 bootspi_init(tU8 cs, bool hspi){
	int i;
	tU32 id_value;
	if(hspi == 0)
		*(tPVU32)(spi_base + BOOTSPI_ADDR) = cs_addr[cs];
	/*Set CS-0 for programming*/
	id_value = bootspi_readid(hspi);
	if((id_value&0xFFFFFF) == 0xFFFFFF){
		nc_printf("No SPI on this Chip Select\n");
		return ERR_NO_MATCH_ID;
	}
		
	for(i=0; i<(sizeof(id_table)/sizeof(struct spi_dat)); i++){
		if((id_value & 0xFF) == id_table[i].uc_MFRID){
			if(((id_value & 0x0000FF00)>>8) == id_table[i].uc_MTYPE){
				if(((id_value & 0x00FF0000)>>16) == id_table[i].uc_MCAP){
					break;
				}
			}
		}
	}	
	if(i == (sizeof(id_table)/sizeof(struct spi_dat))){
		nc_printf("No spi device on CS%d ID\n",i);
	}
	bootspi_priv=i;
	/*Check if by default they have to be unprotected*/
	if(((id_table[bootspi_priv].flags & UNPROTECT_SECTORS) == UNPROTECT_SECTORS)){
		bootspi_unlock_all_sectors(hspi);
	}
	/*Enable the 4 byte support by default*/
	if(((id_table[bootspi_priv].flags & FBYTE_SUPPORT) == FBYTE_SUPPORT)){

		/*This is the MAC0STNGCTL[16] this works only when BMISC[24] is set to 1*/
		*(tPVU32) (glob_spi_base + BOOTSPI_MISC) |= 0x01000000;
		*((tPVU32) (0x40100190)) |= 0x10000;

		if((id_table[bootspi_priv].flags & SEND_WR_ENA) == (SEND_WR_ENA)){
			/*Write enable command*/
			bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);
		}
		/*Enable 4-byte command*/
		bootspi_program_command(0xFFFFFFFF, 0x800011b7, &bootspi_priv, hspi);
	}
	return 0;
}
/*
*	bootspi_sector_erase()			-	erases the sector of the CS
*
*	@address						-	it is the erase start 
*										addresss of the CS given in 
*										the command line
*
*	@size							-	the size from the start address 
										how much we want to erase
*
*	@hspi							-	True or Flase
*										(False in our case)
*/
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
		/*Write enable command*/
		bootspi_program_command(0xFFFFFFFF, 0x80001106, &bootspi_priv, hspi);
	}
	/*Sector Erase Command*/
	bootspi_program_command((address+(i<<16)), ((id_table[bootspi_priv].flags & FBYTE_SUPPORT) != FBYTE_SUPPORT)? 0x800014D8: 0x800015D8, &bootspi_priv, hspi);
	/*0x7 means check flag status*/
	if((id_table[bootspi_priv].flags & STS_FLAGS_REG) == STS_FLAGS_REG){
		bootspi_check_status(0x7, 0x80000170, hspi);
	}
	bootspi_check_status(0, 0x80002105, hspi);
	nc_printf(".");
	}
	nc_printf("\n");
	return 0;
}

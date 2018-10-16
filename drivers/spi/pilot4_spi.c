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
#include <spi.h>

#include <malloc.h>

#include <configs/pilot4_map.h>
#include <pilot4_spiflash.h>

#ifdef CONFIG_SFDP
#define SPI_TIMEOUT 10

// Defining opcode2dummy look up table
static const unsigned int opcode2dummy[0xff + 1] = {
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0,	// 0x00 - 0x0f
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0x10 - 0x1f
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0x20 - 0x2f
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0, 0, 0,	// 0x30 - 0x3f
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0x40 - 0x4f
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,	// 0x50 - 0x5f
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0x60 - 0x6f
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0x70 - 0x7f
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0x80 - 0x8f
		 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0x90 - 0x9f
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0,	// 0xa0 - 0xaf
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,	// 0xb0 - 0xbf
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	// 0xc0 - 0xcf
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,	// 0xd0 - 0xdf
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 2,	// 0xe0 - 0xef
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0		// 0xf0 - 0xff
	};

extern int pilot4spi_transfer(int bank, spi_cmd_info *spi_info,
                    unsigned char *data, unsigned long datalen, int fbyteaddr);
#else
extern int pilot4spi_transfer(int bank,unsigned char *cmd,int cmdlen, int dir,
                    unsigned char *data, unsigned long datalen, int fbyteaddr);
#endif
extern void InitializeCtrl(void);

/*-----------------------------------------------------------------------
 * Definitions
 */

#ifdef DEBUG_SPI
#define PRINTD(fmt,args...)	printf (fmt ,##args)
#else
#define PRINTD(fmt,args...)
#endif

struct pilot_spi_slave {
	struct spi_slave slave;
	unsigned int mode;
};

static inline struct pilot_spi_slave *to_pilot_spi(struct spi_slave *slave)
{
	return container_of(slave, struct pilot_spi_slave, slave);
}

/*-----------------------------------------------------------------------
 * Initialization
 */
void spi_init (void)
{
	InitializeCtrl();
}

int spi_cs_is_valid(unsigned int bus, unsigned int cs)
{
	return 1;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
		unsigned int max_hz, unsigned int mode)
{
	struct pilot_spi_slave *ps;

	if (!spi_cs_is_valid(bus, cs))
		return NULL;

	ps = malloc(sizeof(struct pilot_spi_slave));
	if (!ps)
		return NULL;

	ps->slave.bus = bus;
	ps->slave.cs = cs;
	ps->slave.flags = 0x0;
	ps->mode = mode;

	return &ps->slave;
}

void spi_free_slave(struct spi_slave *slave)
{
	struct pilot_spi_slave *ps = to_pilot_spi(slave);

	free(ps);
}

int spi_claim_bus(struct spi_slave *slave)
{
	return 0;
}

void spi_release_bus(struct spi_slave *slave)
{
	/* Nothing to do */
}

/*-----------------------------------------------------------------------
 * SPI transfer
 */
unsigned char Opcode;
unsigned char Type;
unsigned char Ctrl;
unsigned char Command[5];
unsigned long cmdlen;
unsigned long datalen;
/* Flash opcodes. */
#define OPCODE_WREN             0x06    /* Write enable */
#define OPCODE_WRDI             0x04    /* Write disable*/
#define OPCODE_SE               0xD8    /* Sector erase */
#define OPCODE_RDID             0x9F    /* Read JEDEC ID */
#define OPCODE_RDSR             0x05    /* Read status register */
#define OPCODE_WRSR             0x01    /* Write status register */
#define OPCODE_READ             0x03    /* Read data bytes */
#define OPCODE_FAST_READ        0x0B    /* Read Fast read */
#define OPCODE_PP               0x02    /* Page program */
#define OPCODE_DP               0xB9    /* Deep Power Down */
#define OPCODE_RES              0xAB    /* Read Electronic Signature */
#define OPCODE_4BYTEENBL        0xB7    /* Enable 4byte addressing mode*/
#define OPCODE_4BYTEDIS         0xE9    /* Disable 4byte addressing mode*/
#define OPCODE_4BREAD           0x13    /* Read from 4-byte address */
#define OPCODE_4B_FASTREAD      0xC     /* Fast Read from 4-byte address */
#define OPCODE_4BWRITE          0x12    /* Write to 4byte address */
#define OPCODE_RDSFDP           0x5A    /* Read SFDP */
#define OPCODE_ERASE_4K         0x20    /* Sector erase 4K */

#define SR_WIP                  0x01    /* Write in progress */

static int wait_till_ready(struct spi_slave *slave, unsigned long timeout)
{
        unsigned long timebase;
        u8 status;
#ifndef CONFIG_SFDP
        int dir = 1;
#endif
        u8 cmd1 = OPCODE_RDSR;

        timebase = get_timer(0);
        do {
                //ret = spi_flash_cmd(spi, OPCODE_RDSR, &status, sizeof(status));
#ifdef CONFIG_SFDP
				spi_cmd_info cmd_info;
				memset (&cmd_info, 0, sizeof (spi_cmd_info));
				cmd_info.cmd = cmd1;
				cmd_info.cmdlen = 1;
				cmd_info.dir = SPI_READ; // =1
				cmd_info.dummycycles = opcode2dummy[(int)cmd_info.cmd];
				cmd_info.fastread = 0;
				pilot4spi_transfer(slave->bus, &cmd_info, &status, 1, 0);
#else
                pilot4spi_transfer(slave->bus,&cmd1,1, dir, &status, 1, 0);
#endif

                if ((status & SR_WIP) == 0)
                        break;

        } while (get_timer(timebase) < timeout);


        if ((status & SR_WIP) == 0)
                return 0;

        /* Timed out */
        //printf("TO");
        return -1;
}

static inline int pilot_spi_verify_command(unsigned char * cmd, unsigned long *datln, unsigned long cmdl){
	if(cmdl == 0){
		//printf("Command len is zero\n");
		datalen = 0;
		cmdlen = 0;
		//printf("CLZ\n");
		return 0;
		//return -1;
	}
	if( ((cmd[0] == OPCODE_WRSR) || (cmd[0] == OPCODE_PP) || (cmd[0] == OPCODE_4BWRITE)) && ((*datln) == 0)){
		//printf("Write command with zero data len refusing command cowardly!!!!!\n");
		datalen = 0;
		cmdlen = 0;
		//printf("WWZ\n");
		return 0;
		//return -1;
	}

	if (((cmd[0] == OPCODE_WREN) || (cmd[0] == OPCODE_WRDI) || (cmd[0] == OPCODE_SE) || (cmd[0] == OPCODE_ERASE_4K)
				|| (cmd[0] == OPCODE_4BYTEENBL) || (cmd[0] == OPCODE_4BYTEDIS))) {
		if ((*datln) != 0){
			//printf("Warning data len is non zero zero for non data command %x command which is not right\n", cmd[0]);
			*datln = 0;
		}
	}
	//If we get here then it means it is a read command
	else {
		if(((*datln) == 0)){
			//printf("Warning a SPI read type command %x with zero data len!!!\n", cmd[0]);
			datalen = 0;
			cmdlen = 0;
			//printf("RLZ\n");
			//return -1;
			return 0;
		}
	}
       return 1;
}

#ifdef CONFIG_SFDP
spi_cmd_info spi_info;
int  spi_xfer(struct spi_slave *slave, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	// dout can be NULL for read operation
	// din can be NULL for write operation

	int fbyteaddr = 0;
	unsigned char *buf = (unsigned char*) dout;

	if ((flags & SPI_XFER_BEGIN) == SPI_XFER_BEGIN)
	{
		// it is the command 
		memset (&spi_info, 0, sizeof (spi_cmd_info));
		spi_info.cmdlen = bitlen >> 3;
		spi_info.cmd = *buf;
		spi_info.dummycycles = opcode2dummy[spi_info.cmd];

		if (spi_info.cmdlen > 1) {
			spi_info.addr = (buf[4] << 24) | (buf[3] << 16) | (buf[2] << 8) | buf[1];
			if ((slave->flags & SPI_4BYTE_PART) == SPI_4BYTE_PART)
				fbyteaddr = 1;
		}

		// taking it not to be fast read
		spi_info.fastread = 0;
		spi_info.dir = SPI_NONE;

		//Start and End in one command may be no data
		if((flags & SPI_XFER_END) == SPI_XFER_END){
			pilot_spi_verify_command (&spi_info.cmd, &datalen, spi_info.cmdlen); 
			wait_till_ready (slave, SPI_TIMEOUT);
			pilot4spi_transfer (slave->bus, &spi_info, NULL, 0, fbyteaddr);
			wait_till_ready (slave, SPI_TIMEOUT);
			datalen = 0;
			cmdlen = 0;
		}
		return 0;
	}

	datalen = bitlen >> 3;
	if (spi_info.cmdlen > 1) {
		if ((slave->flags & SPI_4BYTE_PART) == SPI_4BYTE_PART)
			fbyteaddr = 1;
	}
	if((din == NULL) && (dout != NULL)){
		spi_info.dir = SPI_WRITE;	//2;	
		pilot_spi_verify_command (&spi_info.cmd, &datalen, spi_info.cmdlen); 
		wait_till_ready (slave, SPI_TIMEOUT);
		pilot4spi_transfer (slave->bus, &spi_info, (unsigned char*)dout,
					datalen, fbyteaddr);
		wait_till_ready (slave, SPI_TIMEOUT);

	}
	else if ((din != NULL) && (dout == NULL)){
		spi_info.dir = SPI_READ;	//1

		// Setting fbyteaddr and dummycycles for fast read
		if (spi_info.cmdlen > 1)
		{
			if ((slave->flags & SPI_QUAD_READ) == SPI_QUAD_READ)
			{
				spi_info.dummycycles = (slave->sfdp_data.basic_param_table.dword3.dummy_fast_read_114
						+ slave->sfdp_data.basic_param_table.dword3.mode_clock_fast_read_114) / 8;
				spi_info.fastread = READ_QUAD;
			}
			else if ((slave->flags & SPI_DUAL_READ) == SPI_DUAL_READ)
			{
				spi_info.dummycycles = (slave->sfdp_data.basic_param_table.dword4.dummy_fast_read_112
						+ slave->sfdp_data.basic_param_table.dword4.mode_clock_fast_read_112) / 8;
				spi_info.fastread = READ_DUAL;
			}
			else if ((slave->flags & SPI_FAST_READ) == SPI_FAST_READ) {
				spi_info.fastread = READ_FAST;
			}
		}

		pilot_spi_verify_command (&spi_info.cmd, &datalen, spi_info.cmdlen); 
		wait_till_ready (slave, SPI_TIMEOUT);
		pilot4spi_transfer (slave->bus, &spi_info, (unsigned char*)din, datalen, fbyteaddr);
		wait_till_ready (slave, SPI_TIMEOUT);
	}else{
		pilot_spi_verify_command (&spi_info.cmd, &datalen, spi_info.cmdlen); 
		wait_till_ready (slave, SPI_TIMEOUT);
		pilot4spi_transfer (slave->bus, &spi_info, NULL, 0, fbyteaddr);
		wait_till_ready (slave, SPI_TIMEOUT);
	}
	datalen = 0;
	return(0);
}
#else
int  spi_xfer(struct spi_slave *slave, unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	u8  *txp = (u8*)dout; /* dout can be NULL for read operation */
	u8  *rxp = din;  /* din can be NULL for write operation */
	u8	*cmd;
	int dir;
	int fbyteaddr = ((slave->flags & SPI_FBYTE_SUPP) == SPI_FBYTE_SUPP);

	if ((flags & SPI_XFER_BEGIN) == SPI_XFER_BEGIN){

		//it is the command 
		cmdlen = bitlen >> 3;
		cmd = (u8*)dout;

		/* Fill in Command And Address */
		Command[0] = cmd[0];
		Command[3] = cmd[3];
		Command[2] = cmd[2];
		Command[1] = cmd[1];
		Command[4] = cmd[4];

		//Start and End in one command may be no data
		if((flags & SPI_XFER_END) == SPI_XFER_END){
			pilot_spi_verify_command(Command, &datalen, cmdlen); 
			wait_till_ready(slave, 10);
			pilot4spi_transfer(slave->bus,Command,cmdlen, 0, NULL, 0, fbyteaddr);
			wait_till_ready(slave, 10);
			datalen = 0;
			cmdlen = 0;
		}
		return 0;
	}

	datalen = bitlen >> 3;
	if((rxp == NULL) && (txp != NULL)){
		dir = 2;	
		pilot_spi_verify_command(Command, &datalen, cmdlen); 
		wait_till_ready(slave, 10);
		pilot4spi_transfer(slave->bus,Command,cmdlen, dir, txp, datalen, fbyteaddr);
		wait_till_ready(slave, 10);

	}
	else if ((rxp != NULL) && (txp == NULL)){
		dir = 1;
		pilot_spi_verify_command(Command, &datalen, cmdlen); 
		wait_till_ready(slave, 10);
		pilot4spi_transfer(slave->bus,Command,cmdlen, dir, rxp, datalen, fbyteaddr);
		wait_till_ready(slave, 10);
	}else{
		pilot_spi_verify_command(Command, &datalen, cmdlen); 
		wait_till_ready(slave, 10);
		pilot4spi_transfer(slave->bus,Command,cmdlen, 0, NULL, 0, fbyteaddr);
		wait_till_ready(slave, 10);
	}
	datalen = 0;
	return(0);
}
#endif

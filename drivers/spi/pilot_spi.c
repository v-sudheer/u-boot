/*
 *  (C) Copyright 2018 Aspeed Technology Inc.
 *  Shivah Shankar <shivahshankar.shankarnarayanrao@aspeedtech.com>
 *  
 *  Pilot4 SPI controller driver
 *  
 *  SPDX-License-Identifier:     GPL-2.0+
 */ 

#include <common.h>
#include <dm.h>
#include <malloc.h>
#include <spi.h>
#include <asm/io.h>
#include <errno.h>

DECLARE_GLOBAL_DATA_PTR;

#define PILOT_SPI_NONE  0
#define PILOT_SPI_READ  1
#define PILOT_SPI_WRITE  2

struct pilot_spi_regs{
	volatile union {
		volatile struct {
			volatile unsigned char Addr0;           /* LSB */
			volatile unsigned char Addr1;
			volatile unsigned char Addr2;           /* MSB */
			volatile unsigned char Addr3;
		};
		volatile unsigned long Addr;
	};
	volatile unsigned long pilot4_Dummy;
	volatile union {
		volatile struct {
			volatile unsigned char Opcode;
			volatile unsigned char CmdType;
			volatile unsigned char Dummy;
			volatile unsigned char CmdCtrl;
		};
		volatile unsigned long Command;
	};
	volatile unsigned char Trec;
	volatile unsigned char Tcs;
	volatile unsigned char Tcsl;
	volatile unsigned char Tcsh;
	volatile unsigned char SpiCtrl;
	volatile unsigned char Rsvd2[3];
	volatile unsigned char ClkDivL;
	volatile unsigned char ClkDivH;
	volatile unsigned char Rsvd3[2];
	volatile unsigned long SpiStatus;
	volatile unsigned long BMisc;
	volatile unsigned char Rsvd5[16];//Pilot needs so much of offset as the data register is at offset 0x30
	volatile union {
		volatile unsigned char Data8[4];
		volatile unsigned long Data32;
	} Data;
};
struct pilot_spi_platdata {
	uint speed_hz;
	fdt_addr_t regs_addr;
	uint num_cs;
};
struct pilot_spi_priv{
	struct pilot_spi_regs *regs;
	uint num_cs;
	uint cs;
	unsigned char SPI3B4B_strap;
};
/********************** Helpers************************/
void pilot_write32(volatile unsigned int* srcAddr, volatile unsigned int* dstAddr)  {
	asm volatile ("  PUSH {R2-R9} ");
	asm volatile ("  LDMIA  R0!, {R2-R9} ");
	asm volatile ("  STMIA  R1!, {R2-R9} ");
	asm volatile ("  POP {R2-R9} ");
}
static int wait_till_ready(unsigned long timeout)
{
	return 0;
}
static void waitforspiready(volatile struct pilot_spi_regs* regs)
{
	int retries = 10000000;
	while ((regs->SpiStatus & 0x01) && --retries);
	if (retries == 0)
	{
		debug("Unable to exit while!!! (%s %d)\n", __FUNCTION__, __LINE__);
#ifdef CONFIG_PANIC_AND_BAIL_OUT
		panic("");
#endif
	}
}
static int pilot4_boot_bmc_spi_transfer(struct pilot_spi_priv * priv, 
		int bus, int cs, int bank,unsigned char *cmd,int cmdlen, 
		int dir, unsigned char *data, unsigned long datalen, int fbyteaddr)
{
    unsigned char Opcode;
    unsigned char Type;
    unsigned char Dummy;
    unsigned char Ctrl;
    unsigned long Command;
    struct pilot_spi_regs* regs = priv->regs;
    int i, aligned_addr = 0, unaligned_addr = 0;
    int address_offset = fbyteaddr ? 4 : 3;
    struct pilot_spi_regs* spiregs = priv->regs;

    // See if the address is Dword aligned. If it is aligned then the reads/writes
    // can be optimized further
    if (((unsigned int)data & 0x3) == 0)
    {
        aligned_addr = datalen / 32;
        unaligned_addr = datalen % 32;
    }
    else
        unaligned_addr = datalen;
#if 0
    debug("BMISC before is %x\n", spiregs->BMisc);
    debug("Addr3 before is %x\n", spiregs->Addr3);
	
    spiregs->BMisc &= ~(0x3 << 30); //Override
    spiregs->BMisc |= (cs << 30); //Override
    debug("BMISC after is %x\n", spiregs->BMisc);
    debug("Addr3 after is %x\n", spiregs->Addr3);
#endif
    Ctrl = (0x80|spiregs->CmdCtrl);
    Dummy = 0;

    /* Fill in Command And Address */
    Opcode = cmd[0];
    printf("Opcode is %x\n", Opcode);

    if (cmdlen == 4)
    {
	// This is required with 4 byte strap enabled
	if(priv->SPI3B4B_strap == 0x40)
	{
		printf("4B Strap enabled\n");
		spiregs->Addr0 = 0;
		spiregs->Addr1 = cmd[3];
		spiregs->Addr2 = cmd[2];
		spiregs->Addr3 = cmd[1];
	}else{
		printf("3B Strap \n");
		spiregs->Addr0 = cmd[3];
		spiregs->Addr1 = cmd[2];
		spiregs->Addr2 = cmd[1];
		spiregs->Addr3 = cmd[4];
	}
	cmdlen = 4;
	printf("Address programmed is %x len is %x\n", spiregs->Addr, cmdlen);
    }else if(cmdlen > 4){
	//This is needed with 4 byte strap enabled
	if(priv->SPI3B4B_strap == 0x40)
	{
		printf("4B Strap enabled\n");
		spiregs->Addr0 = cmd[4];
		spiregs->Addr1 = cmd[3];
		spiregs->Addr2 = cmd[2];
		spiregs->Addr3 = cmd[1];
	}else{
		printf("3B Strap \n");
		spiregs->Addr0 = cmd[3];
		spiregs->Addr1 = cmd[2];
		spiregs->Addr2 = cmd[1];
		spiregs->Addr3 = cmd[4];
	}
	printf("Address programmed is %x len is %x\n", spiregs->Addr, cmdlen);
    }

    /* Fill in Command Len and data len */
    Type = 0x10;
    if (dir == PILOT_SPI_READ) {
        Type = 0x0;
    }

    Type |= cmdlen;

    Command = (Opcode) | (Type << 8) | (Dummy << 16) | (Ctrl << 24);

    /* Issue Command */
    waitforspiready(spiregs);
    debug("Command written is %x\n", Command);
    spiregs->Command = Command;

    waitforspiready(spiregs);

    /* Do FIFO write for write commands. Writes are always in page boundary*/
    if (dir == PILOT_SPI_WRITE)
    {
        debug("Direction write \n");
        // Do 32byte block copies if the source address was aligned and the data len
        // is more than 32 bytes length. 
        while(aligned_addr--)
        {
            pilot_write32((volatile unsigned int*)(data), (volatile unsigned int*)&spiregs->Data.Data8[0]);
            data += 32;
        }
        // 1. If there is any unaligned length that is left over from the previous
        //    copy, do it in the following for loop
        // 2. If the destination address is unaligned then do the entire copy in the
        //    following for loop
        for (i = 0; i < unaligned_addr; i++)
            spiregs->Data.Data8[0] = data[i];
    }
    // Read the data from the data register
    else if (dir == PILOT_SPI_READ)
    {
        debug("Direction read\n");
        // Do 32byte block copies if the source address was aligned and the data len
        // is more than 32 bytes length. 
        while(aligned_addr--)
        {
            pilot_write32((volatile unsigned int*)&spiregs->Data.Data8[0], (volatile unsigned int*)(data));
            data += 32;
        }
        // 1. If there is any unaligned length that is left over from the previous
        //    copy, do it in the following for loop
        // 2. If the destination address is unaligned then do the entire copy in the
        //    following for loop
        for (i = 0; i < unaligned_addr; i++)
            data[i] = spiregs->Data.Data8[0];
    }
    waitforspiready(spiregs);

#if 0
    /* Switch back to non-register mode and disable Write Fifo mode */
    spiregs->CmdCtrl &= 0x73;
    spiregs->BMisc |= (0x3 << 30); //Undo Override
#endif

    /* Wait for SPI Controller become Idle */
    waitforspiready(spiregs);
    return 0;
}
int cmdlent;
int datalent;
u8  Command[5];
int  pilot_priv_spi_xfer(struct pilot_spi_priv * priv, 
			int bus, int cs, int bank,unsigned int bitlen,
		const void *dout, void *din, unsigned long flags)
{
	u8  *txp = (u8*)dout; /* dout can be NULL for read operation */
	u8  *rxp = din;  /* din can be NULL for write operation */
	u8	*cmd;
	 struct pilot_spi_regs* regs = priv->regs;
	int dir;
	int fbyteaddr = 0; //((slave->flags & SPI_FBYTE_SUPP) == SPI_FBYTE_SUPP);

	if ((flags & SPI_XFER_BEGIN) == SPI_XFER_BEGIN){

		//it is the command 
		cmdlent = bitlen >> 3;
		cmd = (u8*)dout;

		/* Fill in Command And Address */
		Command[0] = cmd[0];
		Command[3] = cmd[3];
		Command[2] = cmd[2];
		Command[1] = cmd[1];
		Command[4] = cmd[4];

		debug("Command 0 is %x\n", Command[0]);
		debug("Command 1 is %x\n", Command[1]);
		debug("Command 2 is %x\n", Command[2]);
		debug("Command 3 is %x\n", Command[3]);
		debug("Command 4 is %x\n", Command[4]);
		//Start and End in one command may be no data
		if((flags & SPI_XFER_END) == SPI_XFER_END){
			//pilot_spi_verify_command(Command, &datalen, cmdlen); 
			wait_till_ready(10);
			//pilot4_boot_bmc_spi_transfer(regs, bus, cs, bank,Command,cmdlent, 0, NULL, 0, fbyteaddr);
			pilot4_boot_bmc_spi_transfer(priv, bus, cs, bank,Command,cmdlent, 0, NULL, 0, fbyteaddr);
			wait_till_ready(10);
			datalent = 0;
			cmdlent = 0;
		}
		return 0;
	}

	datalent = bitlen >> 3;
	if((rxp == NULL) && (txp != NULL)){
		dir = 2;	
		wait_till_ready(10);
		//pilot4_boot_bmc_spi_transfer(regs, bus, cs, bank,Command,cmdlent, dir, txp, datalent, fbyteaddr);
		pilot4_boot_bmc_spi_transfer(priv, bus, cs, bank,Command,cmdlent, dir, txp, datalent, fbyteaddr);
		wait_till_ready(10);

	}
	else if ((rxp != NULL) && (txp == NULL)){
		dir = 1;
		wait_till_ready(10);
		//pilot4_boot_bmc_spi_transfer(regs, bus, cs, bank,Command,cmdlent, dir, rxp, datalent, fbyteaddr);
		pilot4_boot_bmc_spi_transfer(priv, bus, cs, bank,Command,cmdlent, dir, rxp, datalent, fbyteaddr);
		wait_till_ready(10);
	}else{
		wait_till_ready(10);
		//pilot4_boot_bmc_spi_transfer(regs, bus, cs, bank,Command,cmdlent, 0, NULL, 0, fbyteaddr);
		pilot4_boot_bmc_spi_transfer(priv, bus, cs, bank,Command,cmdlent, 0, NULL, 0, fbyteaddr);
		wait_till_ready(10);
	}
	datalent = 0;
	return(0);
}

static void pilot_spi_ctlr_enable(struct pilot_spi_regs* regs, int en)
{
	if (en){
		regs->SpiCtrl |= 0x1;
	}
	else {
		regs->SpiCtrl &= 0xFE;
	}
}
/*****************End Helpers*********************/
static int pilot_spi_ofdata_to_platdata(struct udevice *bus)
{
	fdt_addr_t addr;
	struct pilot_spi_platdata *plat = dev_get_platdata(bus);
	const void *blob = gd->fdt_blob;
	int node = bus->of_offset;
	debug("Pilot SPI: Trying to %s\n", __func__);
        plat->num_cs= fdtdec_get_int(blob, node, "num-cs", 1);
	addr = dev_get_addr(bus);
	if (addr == FDT_ADDR_T_NONE) {
		debug("Pilot SPI: Can't get base address or size\n");
		return -ENOMEM;
	}
	plat->regs_addr = addr;
	plat->speed_hz = fdtdec_get_int(blob,
			node, "spi-max-frequency", 50000000);
	return 0;
}

static int pilot_spi_child_pre_probe(struct udevice *dev)
{
	struct dm_spi_slave_platdata *slave_plat = dev_get_parent_platdata(dev);
	struct pilot_spi_priv *priv = dev_get_priv(dev->parent);
	struct pilot_spi_platdata *plat = dev_get_platdata(dev->parent);

	debug("Pilot SPI : %s\n", __func__);
	if (slave_plat->cs >= priv->num_cs) {
		debug("Pilot SPI invalid chipselect number %d(max %d)!\n",
		      slave_plat->cs, priv->num_cs - 1);
		return -EINVAL;
	}
	priv->cs = slave_plat->cs;
	debug("Pilot SPI pre_probe slave device on CS %u, max_hz %u, mode 0x%x.\n",
	      slave_plat->cs, slave_plat->max_hz, slave_plat->mode);
	return 0;
}
#define PILOT_STRAP_STATUS_REG 0x4010010c
static int pilot_spi_probe(struct udevice *bus)
{
	struct pilot_spi_platdata *plat = dev_get_platdata(bus);
	struct pilot_spi_priv *priv = dev_get_priv(bus);
	struct dm_spi_bus *dm_spi_bus;
	debug("PilotSPI: %s\n", __func__);
	priv->regs = (struct pilot_spi_regs *)plat->regs_addr;
	priv->num_cs = plat->num_cs;
	priv->SPI3B4B_strap = ((*(volatile unsigned int*)(PILOT_STRAP_STATUS_REG)) & 0x40);
	dm_spi_bus->max_hz = plat->speed_hz;
	pilot_spi_ctlr_enable(priv->regs, 0);
	pilot_spi_ctlr_enable(priv->regs, 1);
	waitforspiready(priv->regs);
	return 0;
}

static int pilot_spi_remove(struct udevice *dev)
{
	debug("SHIVAH: %s\n", __func__);
	return 0;
}

static int pilot_spi_claim_bus(struct udevice *dev)
{
	struct udevice *bus = dev->parent;
	struct dm_spi_slave_platdata *slave_plat =
		dev_get_parent_platdata(dev);
	struct pilot_spi_priv *priv = dev_get_priv(bus);
	debug("SHIVAH: %s bus is %d cs is %d\n", __func__, bus->seq, slave_plat->cs);
#if 1	
	debug("BMISC before is %x\n", priv->regs->BMisc);
	priv->regs->BMisc &= ~(0x3 << 30); //Override
	priv->regs->BMisc |= (slave_plat->cs << 30); //Override
	waitforspiready(priv->regs);
	debug("BMISC after is %x\n", priv->regs->BMisc);
#endif
	/* Program the override CS and register access mode*/
	return 0;
}

static int pilot_spi_release_bus(struct udevice *dev)
{
	struct udevice *bus = dev->parent;
	struct pilot_spi_priv *priv = dev_get_priv(bus);
	struct dm_spi_slave_platdata *slave_plat =
		dev_get_parent_platdata(dev);
	debug("SHIVAH: %s\n", __func__);

	priv->regs->BMisc |= (0x3 << 30); //Undo Override
	waitforspiready(priv->regs);
	//Retain the SYSMISCCLKCTL register
	*((volatile unsigned int *) 0x40100120) = 0x22111222;
	return 0;
}

static int pilot_spi_xfer(struct udevice *dev, unsigned int bitlen,
			    const void *dout, void *din, unsigned long flags)
{
	int i;
	struct udevice *bus = dev->parent;
	struct pilot_spi_priv *priv = dev_get_priv(bus);
	struct dm_spi_slave_platdata *slave_plat =
		dev_get_parent_platdata(dev);
	debug("SHIVAH: %s bitlen %d flags %d\n", __func__, bitlen, flags);
	int byte = bitlen/8;

	//pilot_priv_spi_xfer(priv->regs, bus->seq, slave_plat->cs, 0, bitlen, dout, din, flags);
	pilot_priv_spi_xfer(priv, bus->seq, slave_plat->cs, 0, bitlen, dout, din, flags);
	return 0;
}

static int pilot_spi_set_speed(struct udevice *bus, uint speed)
{
	struct pilot_spi_priv *priv = dev_get_priv(bus);
	unsigned int * clk_reg;
	unsigned int val;
	debug("SHIVAH: %s cs is %d\n", __func__, priv->cs);
	debug("SHIVAH: %s and speed is %x\n", __func__, speed);
	*((volatile unsigned int *) 0x40100120) = 0x22111221;
	//TODO - Fix the clock to be programmed to correct frequency
	if(priv->cs == 0){
		clk_reg = ((unsigned int *) 0x40200014);
		val = (0xFFFFFF00 & (*((volatile unsigned int *) clk_reg)));
		*((volatile unsigned int *) clk_reg) = val | 0x2;	
	}else if(priv->cs == 1){
		clk_reg = ((unsigned int *) 0x40200014);	
		val = (0xFF00FFFF & (*((volatile unsigned int *) clk_reg)));
		*((volatile unsigned int *) clk_reg) = val | 0x20000;	
	}else if(priv->cs == 2){
		clk_reg = ((unsigned int *) 0x40200024);	
		val = (0xFFFFFF00 & (*((volatile unsigned int *) clk_reg)));
		*((volatile unsigned int *) clk_reg) = val | 0x2;	
	}else{
		return -ENODEV;
	}
#if 0
        if(speed != 50000000){
		val = (0xFFFFFF00 & (*((volatile unsigned int *) 0x40200014)));
		*((volatile unsigned int *) 0x40200014) = val | 0x2;
	}else{
		val = (0xFFFFFF00 & (*((volatile unsigned int *) 0x40200014)));
		*((volatile unsigned int *) 0x40200014) = val | 0x2;
	}
#endif	
	return 0;
}

static int pilot_spi_set_mode(struct udevice *bus, uint mode)
{
	debug("SHIVAH: %s and mode is %x\n", __func__, mode);
	return 0;
}

static int pilot_cs_info(struct udevice *bus, uint cs,
			  struct spi_cs_info *info)
{
	debug("SHIVAH: %s\n", __func__);
	return 0;
}

static const struct dm_spi_ops pilot_spi_ops = {
	.claim_bus	= pilot_spi_claim_bus,
	.release_bus	= pilot_spi_release_bus,
	.xfer		= pilot_spi_xfer,
	.set_speed	= pilot_spi_set_speed,
	.set_mode	= pilot_spi_set_mode,
	.cs_info	= pilot_cs_info,
};

static const struct udevice_id pilot_spi_ids[] = {
	{ .compatible = "aspeed,pilot-spi" },
	{ }
};
U_BOOT_DRIVER(pilot_spi) = {
	.name	= "pilot_spi",
	.id	= UCLASS_SPI,
	.of_match = pilot_spi_ids,
	.ops	= &pilot_spi_ops,
	.ofdata_to_platdata = pilot_spi_ofdata_to_platdata,
	.platdata_auto_alloc_size = sizeof(struct pilot_spi_platdata),
	.priv_auto_alloc_size = sizeof(struct pilot_spi_priv),
	.probe	= pilot_spi_probe,
	.child_pre_probe = pilot_spi_child_pre_probe,
	.remove	= pilot_spi_remove,
};



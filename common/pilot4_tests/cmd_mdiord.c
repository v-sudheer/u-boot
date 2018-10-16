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

/*
 * Boot support
 */
#include <common.h>
#include <command.h>
#include "include/mdio.h"

unsigned int ioread32(volatile unsigned int * addr)
{
	return *addr;
}

void iowrite32(unsigned int data, volatile unsigned int * addr)
{
	*addr = data;
}

void pilot_mdio_delay(void)
{
	volatile int i;
	for(i=0; i<1000; i++);
}

/*DataBitLen -  Number of bits to read in (maximum 32 bits) */
unsigned int Lan_MII_MDIO_Read( unsigned char DataBitLen )
{
	unsigned int Mask;
	unsigned int Data = 0;
	//Note in case of our pilot2 the both the PHYs are accessed form MAC-0 itself
	void *ioaddr = (void *)IOADDR_INTERFACE_ETH_A;
	void *mdio_addr = ioaddr + CSR9;

	unsigned int csr9;
	/* Bit bang in the data */
	for (Mask=1<<(DataBitLen-1); Mask!=0; Mask>>=1)
	{
		/* Bit bang in one bit's worth of data */
		/* MDRE    | MDO | MDC */
		pilot_mdio_delay();
		iowrite32(MDIO_RD | 0 | 0  , mdio_addr);
		pilot_mdio_delay();
		iowrite32(MDIO_RD | 0 | MDC, mdio_addr);
		pilot_mdio_delay();
		csr9 = ioread32(mdio_addr);
		pilot_mdio_delay();
		if (csr9 & MDI)
		{
			Data |= Mask;
		}
		else
		{
			Data &= ~Mask;
		}
		iowrite32(MDIO_RD | 0 | 0  , mdio_addr);
		pilot_mdio_delay();
	}

	return Data;
}

/* Data - Data to be written */
/* DataBitLen - Number of relevant bits in Data */
void Lan_MII_MDIO_Write(unsigned int Data, unsigned char  DataBitLen)
{
	//Note in case of our pilot2 the both the PHYs are accessed form MAC-0 itself
	void *ioaddr = (void *)IOADDR_INTERFACE_ETH_A;
	void *mdio_addr = ioaddr + CSR9;
	unsigned int Mask;

	for (Mask=1<<(DataBitLen-1);Mask!=0;Mask>>=1)
	{
		/* Bit bang out a 1 or a 0 */
		if (Data & Mask)
		{
			/* MDRE    | MDO | MDC */
			pilot_mdio_delay();
			iowrite32(MDIO_WR | MDO | 0, mdio_addr);
			pilot_mdio_delay();
			iowrite32(MDIO_WR | MDO | MDC, mdio_addr);
			pilot_mdio_delay();
			iowrite32(MDIO_WR | MDO | 0, mdio_addr);
			pilot_mdio_delay();
		}
		else
		{
			/* MDRE    | MDO | MDC */
			pilot_mdio_delay();
			iowrite32(MDIO_WR | 0   | 0, mdio_addr);
			pilot_mdio_delay();
			iowrite32(MDIO_WR | 0   | MDC, mdio_addr);
			pilot_mdio_delay();
			iowrite32(MDIO_WR | 0   | 0, mdio_addr);
			pilot_mdio_delay();
		}
	}
}


void Lan_MII_MDIO_WriteZ(void)
{
//Note in case of our pilot2 the both the PHYs are accessed form MAC-0 itself
    void *ioaddr = (void *)IOADDR_INTERFACE_ETH_A;
    void *mdio_addr = ioaddr + CSR9;
               /* MDRE    | MDO | MDC */
    pilot_mdio_delay();
    iowrite32(MDIO_RD | 0 | 0  , mdio_addr);
    pilot_mdio_delay();
    iowrite32(MDIO_RD | 0 | MDC  , mdio_addr);
    pilot_mdio_delay();
    iowrite32(MDIO_RD | 0 | 0  , mdio_addr);
    pilot_mdio_delay();
}


void Lan_MII_MDIO_Release(void)
{
//Note in case of our pilot2 the both the PHYs are accessed form MAC-0 itself
    void *ioaddr = (void *)IOADDR_INTERFACE_ETH_A;
    void *mdio_addr = ioaddr + CSR9;

               /* MDRE    | MDO | MDC */
    iowrite32(MDIO_RD | 0 | 0  , mdio_addr);
    pilot_mdio_delay();
}

unsigned int Lan_MII_MDIO_ReadReg16(
        unsigned char phy_id,
        unsigned char RegNum        /* MII PHY register number */
//      unsigned int PhyAdd      /* Keep passing different addresses for PHY*/
)
{
    unsigned int RegData16;


    Lan_MII_MDIO_Write(0xFFFFFFFF, 32);    // Idle = <1111111111111111111111111111111111>
    Lan_MII_MDIO_Write(0x00000001,  2);    // Start = <01>
    Lan_MII_MDIO_Write(0x00000002,  2);    // Command = <Read=10>
    //Lan_MII_MDIO_Write(PhyAdd,  5);  // Phy Address = <InternalPHY=00000>
    Lan_MII_MDIO_Write(phy_id,  5);    // Phy Address = <InternalPHY=00000>
    Lan_MII_MDIO_Write(RegNum,      5);    // PHY reg to write = <XXXXX>
    Lan_MII_MDIO_WriteZ();                   // TurnAround = <Z1>
    RegData16 = Lan_MII_MDIO_Read(16);     // Data = <XXXXXXXXXXXXXXXX>
    Lan_MII_MDIO_WriteZ();                   // TurnAround
    Lan_MII_MDIO_Release();


    return RegData16;
}

void Lan_MII_MDIO_WriteReg16(
        unsigned char  RegNum,      /* MII PHY register number */
        unsigned char phy_id,
        unsigned int Data         /* Data to be written */
)
{
//    printk("%s: phy_id = 0x%x\n", __FUNCTION__, phy_id);
    Lan_MII_MDIO_Write(0xFFFFFFFF, 32);  // Idle = <1111111111111111111111111111111111>
    Lan_MII_MDIO_Write(0x00000001,  2);  // Start = <01>
    Lan_MII_MDIO_Write(0x00000001,  2);  // Command = <Write=01>
    Lan_MII_MDIO_Write(phy_id,  5);  // Phy Address = <InternalPHY=00000>
    Lan_MII_MDIO_Write(RegNum,      5);  // PHY reg to write = <XXXXX>
    Lan_MII_MDIO_Write(0x00000002,  2);  // TurnAround = <01>
    Lan_MII_MDIO_Write(Data,       16);  // Data = <XXXXXXXXXXXXXXXX>
    Lan_MII_MDIO_Release();
}


static int mdio_read(int phy_id, int location);

int mdio_read(int phy_id, int location)
{
	int retval = 0;

	if (location & ~0x1f)
		return 0xffff;

	retval = Lan_MII_MDIO_ReadReg16(phy_id & 0x1f, location);
	printf("[%x] = %x\n", location, (retval & 0xffff));

	return 0;
}

int do_mdiord (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int phy_id;
	int location;

#if 0
	printf("ARGC is %d\n", argc);
	if(argc < 3)
		return	CMD_RET_USAGE;
#endif

	phy_id = simple_strtoul(argv[1], NULL, 16);
	location = simple_strtoul(argv[2], NULL, 16);

	printf("PHY ID %x, location %x\n", phy_id , location);
	
	return mdio_read(phy_id, location);
}

U_BOOT_CMD(
	mdiord,	3,	1,	do_mdiord,
	"MDIO Read from PHY",
	"[Phy ID] [Location]"
);

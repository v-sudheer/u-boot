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


static void mdio_write(int phy_id, int location, unsigned int val);

void mdio_write(int phy_id, int location, unsigned int val)
{
	if (location & ~0x1f)
		return;

	Lan_MII_MDIO_WriteReg16(location, phy_id & 0x1f, val & 0xffff);
	printf("Reg [%x] now has %x \n", location, val);
}

int do_mdiowr (cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	int phy_id;
	int location;
	int val;
#if 0
	if(argc < 4)
		return	CMD_RET_USAGE;
#endif
	phy_id = simple_strtoul(argv[1], NULL, 16);
	location = simple_strtoul(argv[2], NULL, 16);
	val = simple_strtoul(argv[3], NULL, 16);

	printf("PHY ID %x, location %x, Value %x \n", phy_id , location, val);
	
	mdio_write(phy_id, location, val);
	return 0;
}

U_BOOT_CMD(
	mdiowr,	4,	1,	do_mdiowr,
	"MDIO Write to PHY",
	"[Phy ID] [Location] [value]"
);

/*******************************************************************************
 *
 * Copyright (C) 2004-2015 Emulex. All rights reserved.
 * EMULEX is a trademark of Emulex.
 * www.emulex.com
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published by the
 * Free Software Foundation.
 *
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
 
extern void spi_sfdp_read (int bank);

int do_cmddumpsfdp (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	int bank;

	if (argc != 2)
	{
		cmd_usage(cmdtp);
		return -1;
	}

	bank = simple_strtoul (argv[1], NULL, 10);
	if (bank < 1)
	{
		cmd_usage(cmdtp);
		return -1;
	}

	spi_sfdp_read(bank - 1);

	return 0;
}

U_BOOT_CMD (
	dumpsfdp, 2, 1, do_cmddumpsfdp,
	"dumps SFDP data for a bank",
	"DUMPS SFDP DATA FOR A BANK\n\t- dumpsfdp bank#"
);

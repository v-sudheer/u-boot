/*
 * Copyright (C) 2012-2020, 2012 ASPEED Tech., Inc.
 * Ryan Chen (ryan_chen@aspeedtech.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/regs-scu.h>
#include <asm/arch/platform.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	char buf[32];
	printf("Board: AST Coldfire \n");

	return 0;
};

phys_size_t initdram(int board_type)
{
	return 0x800000;
}
int testdram(void)
{
	/* TODO: XXX XXX XXX */
	printf("DRAM test not implemented!\n");

	return (0);
}


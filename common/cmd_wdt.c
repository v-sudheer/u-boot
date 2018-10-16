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

#define BASE			0x40100000
#define SYSWCR			0x880
#define SYSWRERL		0x884
#define SYSWRERH		0x888
#define SYSWCFR			0x88C
#define SYSRST_STATUS   0x8B0
#define SEC				381	
#define tPVU32			volatile unsigned int *

extern void trigger_wdt(const unsigned int wdt_no, const unsigned int timeinsec,
                        const unsigned int pre_trig);

void trigger_wdt(const unsigned int wdt_no, const unsigned int time, const unsigned int pre_trig)
{
	volatile unsigned int offset = ((wdt_no - 1) * 0x10);
	volatile unsigned char * base = (volatile unsigned char * )BASE;

	base+=offset;

    if (0 == pre_trig)
    {
        *(tPVU32)(base + SYSWCR) = 0x101;
        *(tPVU32)(base + SYSWCFR) =  (time * SEC);
    }
    else
    {
        *(tPVU32)(base + SYSWCR) = 0x400101;
        *(tPVU32)(base + SYSWCFR) = (time * SEC) | (((time * SEC) - (pre_trig * SEC)) << 16);
    }

	*(tPVU32)(base + SYSWRERL) = 0x3; 
	*(tPVU32)(base + SYSWCR) = (*(tPVU32)(base + SYSWCR)) | 0x800000;

	printf("!!! watchdog timer %d is triggered with %d sec(%d sec pre-trig) time !!! \n",
            wdt_no, time, pre_trig);
}

int do_wdt(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	unsigned int wdt_no;
	unsigned int timeinsec;
    unsigned int pre_trig = 0;

	if (argc < 3)
	{
		cmd_usage(cmdtp);
		return 1;
	}

	wdt_no = simple_strtoul(argv[1], NULL, 10);
	timeinsec = simple_strtoul(argv[2], NULL, 10);
    if (argc == 4)
    {
        pre_trig = simple_strtoul(argv[3], NULL, 10);
        if (pre_trig >= timeinsec)
        {
            printf("pre-trigger has to be lesser than actual time\n");
            return 1;
        }
    }
#ifndef CONFIG_USE_SSP_RESET
	if(wdt_no == 2)
	{
		printf("Watchdog 2 is used for bmc cpu reset workaround, hence use wdt1 or wdt 3 only\n");
		return 1;
	}
#endif
	if ((wdt_no < 1) || (wdt_no > 3))
	{
		cmd_usage(cmdtp);
		printf("There are only 3 watchdog timers available.\n");
		printf("Please use value for x as 1/2/3 only\n");
		return 1;
	}

	if (timeinsec > 172)
	{
		cmd_usage(cmdtp);
		printf("Max allowed time is 172 sec only\n");
		return 1;
	}


	trigger_wdt(wdt_no, timeinsec, pre_trig);

	return 0;
}

U_BOOT_CMD(
	wdt,        4,      1,      do_wdt,
	"watchdog timer trigger command",
	"wdt x time <pre_trig> - x is the watchdog to be used, time is the time to load in seconds and optional pre-trigger"
);

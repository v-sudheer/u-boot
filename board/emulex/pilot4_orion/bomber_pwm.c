/*******************************************************************************
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

#include <configs/pilot4_hw.h>
#include <configs/pilot4_map.h>

#define BOMBER_LMEM_ADDR 0x1000d000

const unsigned char bomber_code[] =
{
#include "8051.hex"
};

extern unsigned long power_on_reset;

void start_bomber(void)
{
    int i = 0;

    if (!power_on_reset)
        return;

    // Workaround needed for A0/A1 boards only
    if ((*(volatile unsigned long*)(P4_SYS_CLK_VA_BASE + 0x50) & 0xFF) != 0x40)
        return;

    for (i = 0; i < sizeof(bomber_code); i++)
        *(volatile unsigned char*)(BOMBER_LMEM_ADDR+i) = bomber_code[i];

    // Bring bomber out of reset
    *(volatile unsigned long*)(P4_BOMBER_BASE) = 0x0;
    *(volatile unsigned long*)(P4_BOMBER_BASE) = 0x1;

    // Configure the GPIOs - Intentionally not doing RMW
    *(volatile unsigned long*)(P4_GPIO_BASE + 0x304) = 0x25252424;
    // Enable GPIO_40_6 and GPIO_40_7 in GPIOEN3 register
    *(volatile unsigned long*)(P4_SYS_CLK_BASE + 0x80C) = 0xC00000;
}

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

# include "defs.h"
# include "uart.h"

#ifdef ENABLE_PRINTS
void m3_uart_init ( void )
{
    // First program the UPCTL register
    *(volatile u32 *)(UPCTL_REG) = 0x03;

    // Set the baud for the UART
    *(volatile u32 *)(UART_BASE+SERIAL_IIR) = 0x7;
    *(volatile u32 *)(UART_BASE+SERIAL_LCR) = 0x83;
    *(volatile u32 *)(UART_BASE) = 0x1;
    *(volatile u32 *)(UART_BASE+SERIAL_LCR) = 0x3;

    // If everything went fine we should see "A\n" on the uart
    *(volatile u32 *)(UART_BASE+SERIAL_THR) = 0x41;
    *(volatile u32 *)(UART_BASE+SERIAL_THR) = 0x0D;
    *(volatile u32 *)(UART_BASE+SERIAL_THR) = 0x0A;
}


void m3_uart_putc ( u8 c )
{
    volatile u32 status = 0;

    /* Wait for Ready */
    do
    {
        status = *(volatile u32 *)(UART_BASE+SERIAL_LSR);
    }
    while (!((status & SERIAL_LSR_THRE)==SERIAL_LSR_THRE) );

    /* Write Character */
    *(volatile u32 *)(UART_BASE+SERIAL_THR) = c;

    return;
}


void m3_uart_puts (char* buf )
{
    // if ( NULL == buf )
    //	return;

    while ( '\0' != *buf )
    {
        /* Check for LF */
        if ( '\n' == *buf )
        {
            /* Add CR */
            m3_uart_putc ( '\r' );
        }

        m3_uart_putc ( *buf );

        buf++;
    }
}
#endif

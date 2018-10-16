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


# ifndef	__PILOT$_UART_H__
# define	__PILOT$_UART_H__

#define ENABLE_PRINTS
# define UART_BASE   		0x40431000
# define SERIAL_THR			0x00
# define SERIAL_RBR			0x00
# define SERIAL_IER			0x04
# define SERIAL_IIR			0x08
# define SERIAL_FCR			0x08
# define SERIAL_LCR			0x0C
# define SERIAL_MCR			0x10
# define SERIAL_LSR			0x14
# define SERIAL_MSR			0x18
# define SERIAL_SPR			0x1C
# define SERIAL_LSR_THRE	0x20


# define	UPCTL_REG			0x4010012c

#ifndef ENABLE_PRINTS
#define m3_uart_init()  
#define m3_uart_puts(A) 
#define m3_uart_putc(A) 
#else
extern void m3_uart_puts (char* buf );
extern void m3_uart_putc (unsigned char c);
extern void m3_uart_init(void);
#endif

# endif

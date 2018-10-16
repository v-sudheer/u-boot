/*
 * Copyright (C) 2013-14 Emulex. All rights reserved
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

#include <common.h>
#include <watchdog.h>
#include <asm/io.h>
#include <linux/compiler.h>
#include <serial.h>

/********Private functions*******/
typedef unsigned long long      UINT64;
typedef long long               INT64;
typedef unsigned int            UINT32;
typedef int                     INT32;
typedef unsigned short          UINT16;
typedef short                   INT16;
typedef unsigned char           UINT8;
typedef char                    INT8;
typedef unsigned char           BOOL;

#define FALSE   0
#define TRUE    1

#ifdef CONFIG_SERIAL2
        UINT32 DebugSerialPort = P4_UART_2_BASE ;
#elif CONFIG_SERIAL3
        UINT32 DebugSerialPort = P4_UART_3_BASE ;
#elif CONFIG_SERIAL4
        UINT32 DebugSerialPort = P4_UART_4_BASE;
#else
        #error "Bad: you didn't configure serial ..."
#endif

#define serial_inw(addr)                        (*((volatile UINT32 *)(addr)))
#define serial_outw(addr, value)        (*((volatile UINT32 *)(addr)) = value)

void
AST_SerialInit(UINT32 port, UINT32 baudrate, UINT32 parity,UINT32 num,UINT32 len)
{
        UINT32 lcr;

    lcr = serial_inw(port + SERIAL_LCR) & ~SERIAL_LCR_DLAB;
        /* Set DLAB=1 */
    serial_outw(port + SERIAL_LCR,SERIAL_LCR_DLAB);
    /* Set baud rate */
    serial_outw(port + SERIAL_DLM, ((baudrate & 0xf00) >> 8));
    serial_outw(port + SERIAL_DLL, (baudrate & 0xff));

        //clear orignal parity setting
        lcr &= 0xc0;

        switch (parity)
        {
                case PARITY_NONE:
                        //do nothing
                break;
        case PARITY_ODD:
                    lcr|=SERIAL_LCR_ODD;
                        break;
        case PARITY_EVEN:
                lcr|=SERIAL_LCR_EVEN;
                break;
        case PARITY_MARK:
                lcr|=(SERIAL_LCR_STICKPARITY|SERIAL_LCR_ODD);
                break;
        case PARITY_SPACE:
                lcr|=(SERIAL_LCR_STICKPARITY|SERIAL_LCR_EVEN);
                break;

        default:
                break;
    }

    if(num==2)
                lcr|=SERIAL_LCR_STOP;

        len-=5;

        lcr|=len;

    serial_outw(port+SERIAL_LCR,lcr);
}
int AST_TestSerialForChar(UINT32 port)
{
        return ((serial_inw( port+ SERIAL_LSR ) & SERIAL_LSR_DR ) == SERIAL_LSR_DR);
}

UINT32
AST_SerialIntIdentification(UINT32 port)
{
        return serial_inw(port + SERIAL_IIR);
}


void
AST_SetSerialLoopback(UINT32 port, UINT32 onoff)
{
        UINT32 temp;

        temp=serial_inw(port+SERIAL_MCR);
        if(onoff==ON)
                temp|=SERIAL_MCR_LPBK;
        else
                temp&=~(SERIAL_MCR_LPBK);

        serial_outw(port+SERIAL_MCR,temp);
}

void
AST_SetSerialFifoCtrl(UINT32 port, UINT32 level, UINT32 resettx, UINT32 resetrx)
{
        UINT8 fcr = 0;

        fcr |= SERIAL_FCR_FE;

        switch(level)
        {
                case 4:
                        fcr|=0x40;
                        break;
                case 8:
                        fcr|=0x80;
                        break;
                case 14:
                        fcr|=0xc0;
                        break;
                default:
                        break;
        }

        if(resettx)
                fcr|=SERIAL_FCR_TXFR;

        if(resetrx)
                fcr|=SERIAL_FCR_RXFR;

        serial_outw(port+SERIAL_FCR,fcr);
}

void
AST_DisableSerialFifo(UINT32 port)
{
        serial_outw(port+SERIAL_FCR,0);
}

void
AST_SetSerialInt(UINT32 port, UINT32 IntMask)
{
        serial_outw(port + SERIAL_IER, IntMask);
}

char
AST_GetSerialChar(UINT32 port)
{
    char Ch;

        // wait until Rx ready
        while( !((serial_inw(port+SERIAL_LSR) & SERIAL_LSR_DR)==SERIAL_LSR_DR) )
        {
        }

    Ch = serial_inw(port + SERIAL_RBR);
    return (Ch);
}

void
AST_PutSerialChar(UINT32 port, char Ch)
{
        UINT32 status;

    do
        {
                status=serial_inw(port+SERIAL_LSR);
        }while (!((status & SERIAL_LSR_THRE)==SERIAL_LSR_THRE));        // wait until Tx ready
    serial_outw(port + SERIAL_THR,Ch);
}

void
AST_PutSerialStr(UINT32 port, const char *Str)
{
        char *cp;

        for(cp = (char *)Str; *cp != 0; cp++)
        {
        if(*cp == '\n')
                        AST_PutSerialChar(port, '\r');
                AST_PutSerialChar(port, *cp);
        }
}

/*********************End Private Functions******************/
void
serialhandler(void *arg)
{
        AST_SerialIntIdentification(DebugSerialPort);
}

#define FIND_DLL_DLH(x, divisor)(                  \
{                                                       \
       typeof(x) __x = x;                              \
       typeof(divisor) __d = divisor;                  \
       (((typeof(x))-1) > 0 ||                         \
        ((typeof(divisor))-1) > 0 || (__x) > 0) ?      \
               (((__x) + ((__d) / 2)) / (__d)) :       \
               (((__x) - ((__d) / 2)) / (__d));        \
}                                                       \
)

static void
pilot_serial_setbrg(void)
{
        DECLARE_GLOBAL_DATA_PTR;
        unsigned int reg = 0;

	reg = FIND_DLL_DLH(PILOT_UART_CLOCK/16, gd->baudrate);
        switch (gd->baudrate)
        {
                case 9600:
	AST_PutSerialStr(0X40430000, "9600\n");
                        break;
                case 19200:
	AST_PutSerialStr(0X40430000, "19200\n");
                        break;
                case 38400:
	AST_PutSerialStr(0X40430000, "384000\n");
                        break;
                case 57600:
                        break;
//#if (SYS_CLK == 22118400)
                case 115200:
                        break;
//#endif
#ifdef CONFIG_HIGH_SPEED_DEBUG_UART
                case 460800:
	AST_PutSerialStr(0X40430000, "460800\n");
			reg = 7;
			break;
                case 230400:
	AST_PutSerialStr(0X40430000, "230400\n");
			reg = 13;
			break;
#endif
                default:
                        //reg=PILOT_BAUD_38400;
	AST_PutSerialStr(0X40430000, "default\n");
#ifdef CONFIG_HIGH_SPEED_DEBUG_UART
                        reg = 27;//115200
#else
                        reg = 1;//115200 for 1843200 clock
#endif
                        break;
        }

        /* Set Baud Rate */
        AST_SerialInit (DebugSerialPort, reg, PARITY_NONE, 0, 8 );

        /* Enable FiFo */
        AST_SetSerialFifoCtrl(DebugSerialPort, 1, ENABLE, ENABLE);

        return;
}


static int
pilot_serial_init(void)
{
#ifndef CONFIG_HIGH_SPEED_DEBUG_UART
        *((volatile unsigned long *)0x40430008) =0x7;
        *((volatile unsigned long *)0x4043000c) =0x83;
        *((volatile unsigned long *)0x40430000)=1;
        *((volatile unsigned long *)0x4043000c)=0x3;
#else
	pilot_serial_setbrg();
#endif
        //serial_setbrg();
        return 0;
}


static int
pilot_serial_getc(void)
{
        return AST_GetSerialChar( DebugSerialPort );
}

static void
pilot_serial_putc(const char c)
{
        if(c == '\n')
                        AST_PutSerialChar(DebugSerialPort,'\r');
        AST_PutSerialChar( DebugSerialPort, c );

}

static int
pilot_serial_tstc(void)
{
        return AST_TestSerialForChar(DebugSerialPort);
}

static void
pilot_serial_puts (const char *s)
{
        AST_PutSerialStr(DebugSerialPort,s);
}

#if !defined(CONFIG_SERIAL_MULTI)

int serial_init(void)
{
        return uart_zynq_serial_init(0);
}

void serial_setbrg(void)
{
        uart_zynq_serial_setbrg(0);
}

void serial_putc(const char c)
{
        uart_zynq_serial_putc(c, 0);
}

void serial_puts(const char *s)
{
        uart_zynq_serial_puts(s, 0);
}

int serial_getc(void)
{
        return uart_zynq_serial_getc(0);
}
int serial_tstc(void)
{
        return uart_zynq_serial_tstc(0);
}
#endif


struct serial_device pilot_serial_device = {
        "PILOT_SERIAL",
        pilot_serial_init,
		NULL,
        pilot_serial_setbrg,
        pilot_serial_getc,
        pilot_serial_tstc,
        pilot_serial_putc,
        pilot_serial_puts,
};

__weak struct serial_device *default_serial_console(void)
{
                return &pilot_serial_device;

}


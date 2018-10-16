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

//////////////////////////////////////////////////
/// Filename: uart.h
/// Section: BMC
/// Author:
/// Description: Header file for UART module
//////////////////////////////////////////////////


#include "buffer.h"

#define RBROff                  (0x0<<2)
#define THROff                  (0x0<<2)
#define IEROff                  (0x1<<2)
#define ERBI                    0x01
#define ETBEI                   0x02
#define ELSI                    0x04
#define EDSSI                   0x08
#define PTIME		        0x80
#define ETHRE		        0x80

#define IIROff                  (0x2<<2)
#define IIRNoInterrupt	        0x01
#define IIRRecvLStatInt         0x06
#define IIRRecvdDataInt         0x04
#define IIRCharTimeOutInt       0x0C
#define IIRTHREInt              0x02
#define IIRBusyInt              0x07
#define IIRModemStatInt         0x00
#define IIRFiFoEnable           0xC0

#define FCROff                  (0x2<<2)

#define FIFOEN                  0x01
#define RXFRST                  0x02
#define TXFRST                  0x04
#define DMAModeSel              0x08
#define TXFifo16ByteTrig1       0x00
#define TXFifo16ByteTrig4       0x10
#define TXFifo16ByteTrig8       0x20
#define TXFifo16ByteTrig14      0x30

#define RXFifo16ByteTrig1       0x00
#define RXFifo16ByteTrig4       0x40
#define RXFifo16ByteTrig8       0x80
#define RXFifo16ByteTrig14      0xC0


#define LCROff                  (0x3<<2)

#define WordLength5             0x00
#define WordLength6             0x01
#define WordLength7             0x02
#define WordLength8             0x03
#define StopBits                0x04
#define ParityEn                0x08
#define EvenParity              0x10
#define StickParity             0x20
#define Break                   0x40
#define DLAB                    0x80

#define MCROff                  (0x4<<2)

#define DTR                     0x01
#define RTS                     0x02
#define OUT1                    0x04
#define OUT2                    0x08
#define Loop                    0x10
#define AFE                     0x20
#define DTRRTS			0x03

//Line Status Register
#define LSROff                  (0x5<<2)

#define DataRDY                 0x01
#define OverrunErr              0x02
#define ParityErr               0x04
#define FrameErr                0x08
#define BreakInt                0x10
#define THRE                    0x20
#define TEMT                    0x40
#define RxFiFoErr               0x80


//Modem Status Register
#define MSROff                  (0x6<<2)

#define CTSChange               0x01
#define DSRChange               0x02
#define TERI                    0x04
#define DCDChange               0x08
#define CTS                     0x10
#define DSR                     0x20
#define RI                      0x40
#define DCD                     0x80


#define SCROff                  (0x7<<2) 
#define DLLOff                  (0x0<<2)
#define DLMOff                  (0x1<<2)

#define USR			0x7C

#define UARTTXPTR		0
#define UARTTXSIZE		4
#define UARTCTXPTR		8
#define UARTCTXSIZE		0xC
#define UARTRXPTR		0x10
#define UARTRXSIZE		0x14
#define UARTCRXPTR		0x18
#define UARTCRXSIZE		0x1C
#define UARTRXSTATUS		0x20
#define UARTTXSTATUS		0x24


#define UART1baseLPC	        0x0001
#define UART2baseLPC	        0x0002
#define RBROffLPC               (0x0<<8)   	//left shift 8 becos model swaps address msb and lsb
#define THROffLPC               (0x0<<8)
#define DLLOffLPC               (0x0<<8)
#define DLMOffLPC               (0x1<<8)
#define IEROffLPC               (0x1<<8)
#define IIROffLPC               (0x2<<8)
#define FCROffLPC               (0x2<<8)
#define LCROffLPC               (0x3<<8)
#define MCROffLPC               (0x4<<8)
#define LSROffLPC               (0x5<<8)
#define MSROffLPC               (0x6<<8)
#define SCROffLPC               (0x7<<8)
#define USROffLPC	        (0x7C<<8)

#define UART3BASE               UART3 		//USE SP1 to test UART3 0x40500000

#define BAUD_115K               115200
#define BAUD_57K                57600
#define BAUD_38K                38400
#define BAUD_19K                19200
#define BAUD_14K                14400
#define BAUD_9K                 9600
#define BAUD_4K                 4800
#define BAUD_2K                 2400
#define BAUD_1K                 1200
#define BAUD_600                600
#define BAUD_300                300
#define BAUD_0                  0

#define NO_PARITY               0
#define ODD_PARITY              1
#define EVEN_PARITY             2

#define ONE_STOP_BIT            0
#define TWO_STOP_BITS           1


//Common function declarations
extern tU8 p2_readbyte_to (tU32,tU32);
extern tU8 nc_writebyte(tU32, tU8);
extern tU8 nc_readbyte (tU32);
extern void nc_clear_uart_fifo (tU32);

void UART_CONFIGURE(unsigned int, unsigned int, unsigned char, unsigned char, unsigned char);
void UART_EN_FLWCTRL(unsigned int);
void UART_SET_FIFO_THR_LVL(unsigned int, unsigned char, unsigned char);
void UART3_INT_EN(unsigned int);
void UART4_INT_EN(unsigned int);
void UART5_INT_EN(unsigned int);
void VUART0_INT_EN(unsigned int);
void VUART1_INT_EN(unsigned int);
void UART3_int_handler(void);
void UART4_int_handler(void);
void UART5_int_handler(void);
void VUART0_int_handler(void);
void VUART1_int_handler(void);
void uartcommoninthandler(unsigned int, unsigned int);
void uart_line_status_handler(unsigned int);
void uart_rx_handler(volatile unsigned int, volatile unsigned int);
void uart_tx_handler(volatile unsigned int, volatile unsigned int);
void UART_INT_DISABLE(unsigned int, unsigned char);


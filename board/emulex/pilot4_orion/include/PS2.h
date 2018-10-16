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

#include "buffer.h"


#define PS2BASE			0x40412000

#define PS21KMCTL		PS2BASE
#define PS21KMSTS		PS2BASE+0x4
#define PS21KMDAT		PS2BASE+0x8
#define PS21KMCLK		PS2BASE+0xC
#define PS21KMISR		PS2BASE+0x10
#define PS2COMMSTS		PS2BASE+0x14

#define PS22KMCTL		PS2BASE+0x20
#define PS22KMSTS		PS2BASE+0x24
#define PS22KMDAT		PS2BASE+0x28
#define PS22KMCLK		PS2BASE+0x2C
#define PS22KMISR		PS2BASE+0x30


//;KMCTL bit definations
#define FCLKLOW			1<<0
#define FDATLOW			1<<1
#define KBDMOUSE_EN		1<<2
#define TXINTR_EN		1<<3
#define RXINTR_EN		1<<4
#define NOACKINTR_EN		1<<5
#define OVERRUNINTR_EN		1<<6

//;KMSTS bit definations
#define KBDMSDATAIN		1<<0
#define KBDMSCLKIN		1<<1
#define RXPARITY			1<<2
#define RXBUSY			1<<3
#define RXFULL			1<<4
#define TXBUSY			1<<5
#define TXEMPTY			1<<6
#define RXBITGT10		1<<7


//;KMCLK
#define KMCLK_DIV0		0
#define KMCLK_DIV1		1
#define KMCLK_DIV2		2
#define KMCLK_DIV3		3
#define KMCLK_DIV4		4
#define KMCLK_DIV5		5
#define KMCLK_DIV6		6
#define KMCLK_DIV7		7
#define KMCLK_DIV8		8
#define KMCLK_DIV9		9
#define KMCLK_DIV10		10
#define KMCLK_DIV11		11
#define KMCLK_DIV12		12
#define KMCLK_DIV13		13
#define KMCLK_DIV14		14
#define KMCLK_DIV15		15

//;KMISR bit definations
#define KBDMSRXINTR		1
#define KBDMSTXINTR		1<<1
#define KBDMSNOACKINTR		1<<2
#define KBDMSOVERRUNINTR        1<<3

#define PS21TXINTRFLAG		PS2WorkBase
#define PS21RXINTRFLAG		PS2WorkBase+0x4
#define PS22TXINTRFLAG		PS2WorkBase+0x8
#define PS22RXINTRFLAG		PS2WorkBase+0xc

#define PS21TXCBUFFPTR		PS2WorkBase+0x10
#define PS21RXCBUFFPTR		PS2WorkBase+0x14
#define PS22TXCBUFFPTR		PS2WorkBase+0x18
#define PS22RXCBUFFPTR		PS2WorkBase+0x1C

#define PS21TXCCNT		PS2WorkBase+0x20
#define PS21RXCCNT		PS2WorkBase+0x24
#define PS22TXCCNT		PS2WorkBase+0x28
#define PS22RXCCNT		PS2WorkBase+0x2C

#define PS21rxcnt		PS2WorkBase+0x30
#define PS22rxcnt		PS2WorkBase+0x34
#define PS21txcnt		PS2WorkBase+0x30
#define PS22txcnt		PS2WorkBase+0x34

#define PS21TXbuff		PS2XmitBuffer           
#define PS21RXbuff		PS2RecvBuffer+0x200           
#define PS22TXbuff		PS2XmitBuffer+0x400           
#define PS22RXbuff		PS2RecvBuffer+0x600           

//	.end     
#define BUFFER_SIZE             0x200

extern unsigned char PS21TxBuff[BUFFER_SIZE];
extern unsigned char PS21RxBuff[BUFFER_SIZE];

extern unsigned char PS22TxBuff[BUFFER_SIZE];
extern unsigned char PS22RxBuff[BUFFER_SIZE];

extern int PS21TxSize;
extern int PS21TxPtr;
extern int PS21RxHead;
extern int PS21RxTail;

extern int PS22TxSize;
extern int PS22TxPtr;
extern int PS22RxTail;
extern int PS22RxHead;

/*******************************************************************
*                                                                  *
*  Description: PS21NoAckHandler is called by PS21IntHandler       *
*               whenever bit 2 of the PS2-1 KMISR register is      *
*               asserted. The interrupt is generated when there is *
*               no acknowledge from the keyboard for the byte      *
*               transmitted by the PS2 controller.                 *
*                                                                  *
*               The handler will re-transmit the last byte when    *
*               this happens.                                      *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21NoAckHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS22NoAckHandler is called by PS22IntHandler       *
*               whenever bit 2 of the PS2-2 KMISR register is      *
*               asserted. The interrupt is generated when there is *
*               no acknowledge from the keyboard for the byte      *
*               transmitted by the PS2 controller.                 *
*                                                                  *
*               The handler will re-transmit the last byte when    *
*               this happens.                                      *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22NoAckHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS21OverRunHandler is called by PS21IntHandler     *
*               whenever bit 3 of the PS2-1 KMISR register is      *
*               asserted. The interrupt is generated when there is *
*               a new byte received before the previous can be read*
*               by the BMC.                                        *
*                                                                  *
*               The handler will request for a re-transmit from the*
*               keyboard when this happens.                        *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21OverRunHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS22OverRunHandler is called by PS22IntHandler     *
*               whenever bit 3 of the PS2-2 KMISR register is      *
*               asserted. The interrupt is generated when there is *
*               a new byte received before the previous can be read*
*               by the BMC.                                        *
*                                                                  *
*               The handler will request for a re-transmit from the*
*               keyboard when this happens.                        *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22OverRunHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS21RxHandler is called by PS21IntHandler whenever *
*               bit 0 of the PS2-1 KMISR register is asserted.     *
*               The interrupt is generated when a byte has been    *
*               received by the BMC.                               *
*                                                                  *
*               The handler will store the data into PS21RxBuff,   *
*               waiting to be retrieved.                           *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21RxHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS22RxHandler is called by PS22IntHandler whenever *
*               bit 0 of the PS2-2 KMISR register is asserted.     *
*               The interrupt is generated when a byte has been    *
*               received by the BMC.                               *
*                                                                  *
*               The handler will store the data into PS22RxBuff,   *
*               waiting to be retrieved.                           *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22RxHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS21TxHandler is called by PS21IntHandler whenever *
*               bit 1 of the PS2-1 KMISR register is asserted.     *
*               The interrupt is generated when the current byte   *
*               has been trasmitted.                               *
*                                                                  *
*               The handler will check if there is any more data   *
*               to be sent, and trasmit the next byte in the       *
*               PS21TxBuff.                                        *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21TxHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS22TxHandler is called by PS22IntHandler whenever *
*               bit 1 of the PS2-2 KMISR register is asserted.     *
*               The interrupt is generated when the current byte   *
*               has been trasmitted.                               *
*                                                                  *
*               The handler will check if there is any more data   *
*               to be sent, and trasmit the next byte in the       *
*               PS22TxBuff.                                        *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22TxHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS21IntHandler is called by PS2IntHandler whenever *
*               bit 0 of the KMCIR register is asserted.           *
*               The interrupt is generated when there is an        *
*               interrupt generated for PS2-1.                     *
*                                                                  *
*               The handler will determine which PS2-1 interrupt   *
*               handler to call according to the bit set in PS2-1  *
*               KMISR register.                                    *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21IntHandler(void);    

/*******************************************************************
*                                                                  *
*  Description: PS22IntHandler is called by PS2IntHandler whenever *
*               bit 1 of the KMCIR register is asserted.           *
*               The interrupt is generated when there is an        *
*               interrupt generated for PS2-2.                     *
*                                                                  *
*               The handler will determine which PS2-2 interrupt   *
*               handler to call according to the bit set in PS2-2  *
*               KMISR register.                                    *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22IntHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS2IntHandler is called whenever any interrupt is  *
*               generated for PS2.                                 *
*                                                                  *
*               The handler will call either the PS21IntHandler or *
*               PS22IntHandler according to the bit set in KMCIR   *
*               register.                                          *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS2IntHandler(void);

/*******************************************************************
*                                                                  *
*  Description: PS21IntEnable is used to enabled individual        *
*               interrupt or multiple interrupts for PS2-1         *
*               according to "mask".                               *
*                                                                  *
*  Input Parameter: bit mask for each interrupt                    *
*                      bit 3 - Transmit Interrupt Enable           *
*                      bit 4 - Receive Interrupt Enable            *
*                      bit 5 - Tx No ACK Interrupt Enable          *
*                      bit 6 - Rx Overrun Interrupt Enable         *
*                      all other bits should be left as 0          *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21INTEnable(unsigned char mask);

/*******************************************************************
*                                                                  *
*  Description: PS22IntEnable is used to enabled individual        *
*               interrupt or multiple interrupts for PS2-2         *
*               according to "mask".                               *
*                                                                  *
*  Input Parameter: bit mask for each interrupt                    *
*                      bit 3 - Transmit Interrupt Enable           *
*                      bit 4 - Receive Interrupt Enable            *
*                      bit 5 - Tx No ACK Interrupt Enable          *
*                      bit 6 - Rx Overrun Interrupt Enable         *
*                      all other bits should be left as 0          *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22INTEnable(unsigned char mask);

/*******************************************************************
*                                                                  *
*  Description: PS21SendData is used to send data out from PS2-1.  *
*               The function copies the data to PS21TxBuff,        *
*               enabling the trasnmit interrupt and initiate the   *
*               sequence by sending out the 1st byte in the buffer.*
*                                                                  *
*  Input Parameter: data[] - array of bytes to be sent             *
*                   size   - number of bytes to be sent            *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21SendData(unsigned char data[], int size);

/*******************************************************************
*                                                                  *
*  Description: PS22SendData is used to send data out from PS2-2.  *
*               The function copies the data to PS22TxBuff,        *
*               enabling the trasnmit interrupt and initiate the   *
*               sequence by sending out the 1st byte in the buffer.*
*                                                                  *
*  Input Parameter: data[] - array of bytes to be sent             *
*                   size   - number of bytes to be sent            *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22SendData(unsigned char data[], int size);

/*******************************************************************
*                                                                  *
*  Description: PS21InitKB is used to initiatlize the keyboard     *
*               connected to PS2-1.                                *
*               The function resets the keyboard and wait for the  *
*               passing result from the keyboard's self test.      *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21InitKB(void);

/*******************************************************************
*                                                                  *
*  Description: PS22InitKB is used to initiatlize the keyboard     *
*               connected to PS2-2.                                *
*               The function resets the keyboard and wait for the  *
*               passing result from the keyboard's self test.      *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22InitKB(void);

/*******************************************************************
*                                                                  *
*  Description: PS21Init initiatlizes PS2-1 with the appropriate   *
*               settings (i.e. clk divider, interrupt handler).    *
*                                                                  *
*  Input Parameter: clkDivider - used to calculate frequency of the*
*                      clock. f = 1 / (1 + clkDivider)             *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21Init(int clkDivider );

/*******************************************************************
*                                                                  *
*  Description: PS21Disable resets the keyboard connected to PS2-1,*
*               forces data/clk pins to low, and disabes all PS2-1 *
*               related interrupts and interface.                  *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS21Disable(void);

/*******************************************************************
*                                                                  *
*  Description: PS22Init initiatlizes PS2-2 with the appropriate   *
*               settings (i.e. clk divider, interrupt handler).    *
*                                                                  *
*  Input Parameter: clkDivider - used to calculate frequency of the*
*                      clock. f = 1 / (1 + clkDivider)             *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22Init(int clkDivider);

/*******************************************************************
*                                                                  *
*  Description: PS22Disable resets the keyboard connected to PS2-2,*
*               forces data/clk pins to low, and disabes all PS2-2 *
*               related interrupts and interface.                  *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
extern void PS22Disable(void);





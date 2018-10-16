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


#define TAPTRST    (1<<6)

#define OUTPUT_IR    (0x1C)
#define OUTPUT_DR    (0x1D)
#define INPUT_DR     (0x1B)
#define INOUT_DR     (0x19)



//IR Name	Definition	Description
#define EXTEST	        0
//define IDCODE	        2            //EP0 ID=0xEBEBEB00;EP1 ID=0xEBEBEB01
#define IDCODE	        0xE2            //EP0 ID=0xEBEBEB00;EP1 ID=0xEBEBEB01
#define CHAIN_SELECT	3
#define DEBUG_MODE 	        8  




#define SPR_GR_SHIFT    11

//debug value register
#define DVR0         (6<<SPR_GR_SHIFT)+(0)
#define DVR1         (6<<SPR_GR_SHIFT)+(1)
#define DVR2         (6<<SPR_GR_SHIFT)+(2)
#define DVR3         (6<<SPR_GR_SHIFT)+(3)
#define DVR4         (6<<SPR_GR_SHIFT)+(4)
#define DVR5         (6<<SPR_GR_SHIFT)+(5)
#define DVR6         (6<<SPR_GR_SHIFT)+(6)
#define DVR7         (6<<SPR_GR_SHIFT)+(7)

//debug control register
#define DCR0        (6<<SPR_GR_SHIFT)+(8)
#define DCR1        (6<<SPR_GR_SHIFT)+(9)
#define DCR2        (6<<SPR_GR_SHIFT)+(10)
#define DCR3        (6<<SPR_GR_SHIFT)+(11)
#define DCR4        (6<<SPR_GR_SHIFT)+(12)
#define DCR5        (6<<SPR_GR_SHIFT)+(13)
#define DCR6        (6<<SPR_GR_SHIFT)+(14)
#define DCR7        (6<<SPR_GR_SHIFT)+(15)



//debug mode reg1
#define DMR1         (6<<SPR_GR_SHIFT)+(16)
#define DMR2         (6<<SPR_GR_SHIFT)+(17)

//debug watch point counter register
#define DWCR0       (6<<SPR_GR_SHIFT)+(18)
#define DWCR1       (6<<SPR_GR_SHIFT)+(19)

#define DBG_DSR     (6<<SPR_GR_SHIFT)+(20)



//Debug Control bits
#define CC_EQ  (1<<1)
#define DCR_DP (1<<0)
#define SC     (1<<4)
#define CT_IF  (1<<5)



//DMR2 bit definations
#define WGB_1         (1<<13)



//	TAP Module
//Register Description 
	
#define TAP_CONF_A_BMC   0x00
#define TAP_CONF_B_BMC   0x04
#define TAP_STATUS_BMC   0x08
#define TAP_COMMAND_BMC  0x0c
#define TAP_TDO_BMC      0x10
#define TAP_TDI_BMC      0x14
#define TAP_COUNTER_BMC  0x18
#define TAP_DISC_CNTL_BMC 0x1c
#define TAP_IRQ_CNTL_BMC  0x20


#define TDOS             1<<6
#define TDIS             1<<7


typedef struct PII_TAP_REGS
{
  volatile unsigned int tap_conf_a;
  volatile unsigned int tap_conf_b;
  volatile unsigned int tap_status;
  volatile unsigned int tap_command;
  volatile unsigned int tap_tdo;
  volatile unsigned int tap_tdi;
  volatile unsigned int tap_counter;
  volatile unsigned int tap_disc_cntl;
  volatile unsigned int tap_irq_cntl;
} PII_TAP_REGS;

#define  TAP_REGS	((volatile PII_TAP_REGS *)(TAPBASE))

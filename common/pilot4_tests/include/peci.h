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
/// Filename: peci.h
/// Section: PECI
/// Author: Saurabh Kasat
/// Description: This is header file that contains register and 
///     register field description for PECI module
//////////////////////////////////////////////////

// PECI Register Offsets
#define PECI_CTRL 	 	0x00
#define PECI_STS 		0x04	
#define PECI_OPT_RATE 		0x08	
#define PECI_RXPTR		0x0C	
#define PECI_HW_FCS             0x10	
#define PECI_RX_OFF		0x14	
#define PECI_BITBANG_CNT	0x18	
#define PECIPULFLTR             0x1C
#define PECI_TX0_Q              0x20
#define PECI_TX1_Q              0x24
#define PECI_TX2_Q              0x28
#define PECI_TX3_Q              0x2c
#define PECI_TX4_Q              0x30
#define PECI_TX5_Q              0x34
#define PECI_TX6_Q              0x38
#define PECI_TX7_Q              0x3C
                                
#define PECI_RX0_Q              0x20
#define PECI_RX1_Q              0x24
#define PECI_RX2_Q              0x28
#define PECI_RX3_Q              0x2c
#define PECI_RX4_Q              0x30
#define PECI_RX5_Q              0x34
#define PECI_RX6_Q              0x38
#define PECI_RX7_Q              0x3C

#define PECI_POLL_CTRL          0x60
#define PECI_POLL_INFO          0x64
#define PECI_POLL_DLY           0x68


///////// REgister Bit Definitions ///////////

// peci_ctrl
#define peci_en         (1<<0) 
#define peci_trig       (1<<1)
#define peci_irq_en     (1<<2)
#define peci_bitbang_en (1<<3)
#define peci2           (1<<4)
#define peci2_retry     (1<<5)
#define peci2_host      (1<<6)
#define peci_pd         (1<<7)
#define peci_fcserroren (1<<10)
#define peci2_rx_irq_en (1<<11)
#define peci_awfcs_en   (1<<12)

// peci_sts
#define peci_busy       (1<<0)
#define peci_intr       (1<<1)

#define PECIINTRFLAG    PeciWorkBase
#define PECI_ORIG_FLAG  (PeciWorkBase+4)
#define PECI_REGS	PECIBASE
#define PECIPTRS        0x0c

extern void peci_enable (void);
extern void peci_enable_ctrl (tU32);
extern void peci_disable_ctrl (tU32 val);
extern void wait_for_peci_not_busy (void);
extern void peci_trigger (void);
extern void peci_set_irq_handler (tU32);
extern void peci_irqhandler (void);
extern void peci_program_optrate (tU16);
extern void set_xmit_q (tU8, tU8, tU8, tU8);
extern void converttemp (tU32);
extern void peci_set_ping_cmd (tU8);
extern void peci_set_temp0_cmd (tU8);
extern void peci_set_temp1_cmd (tU8);
extern void peci_set_dib_cmd (tU8);
extern void peci_set_resetdevice_cmd (tU8);
extern void peci_set_mbxsend_cmd (tU8);
extern void peci_set_mbxget_cmd (tU8);
extern tU8 peci_hw_tx_fcs (void);
extern tU8 peci_hw_rx_fcs (void);
extern void peci_set_rdpkgconfig_cmd (tU8, tU8);
extern void peci_set_wrpkgconfig_cmd (tU8, tU8);
extern void peci_set_rdiamsr_cmd (tU8, tU8);
extern void peci_set_wriamsr_cmd (tU8, tU8);
extern void peci_set_rdpciconfig_cmd (tU8, tU8);
extern void peci_set_wrpciconfig_cmd (tU8, tU8);

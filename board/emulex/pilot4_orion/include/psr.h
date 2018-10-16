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


#define PSRBASE	        PSR

#define UID0           (PSR+0x00) 
#define UID1           (PSR+0x01)
#define UID2           (PSR+0x02)
#define UID3           (PSR+0x03)
#define UID4           (PSR+0x04)
#define UID5           (PSR+0x05)
#define UID6           (PSR+0x06)
#define RND0           (PSR+0x08)
#define RND1           (PSR+0x09)
#define RND2           (PSR+0x0a)
#define RND3           (PSR+0x0b)
#define LPCCR0         (PSR+0x10)
#define LPCCR1         (PSR+0x11)
#define LPCSR          (PSR+0x12)

#define CHIPID         (PSR+0x20)
#define SIOCFG1        (PSR+0x21)
#define SIOCFG2        (PSR+0x22)
#define SIOCFG3        (PSR+0x23)
#define SIOCFG4        (PSR+0x24)
#define SIOCFG5        (PSR+0x25)
#define SIOCFG6        (PSR+0x26)
#define SIOCFG7        (PSR+0x27)
#define SIOCFG8        (PSR+0x28)

#define SP1DLL         (PSR+0x30)
#define SP1DLH         (PSR+0x31)
#define SP2DLL         (PSR+0x32)
#define SP2DLH         (PSR+0x33)
#define SP1LCR         (PSR+0x34)
#define SP2LCR         (PSR+0x35)

#define PCDR           (PSR+0x40)
#define PCAR_LO        (PSR+0x41)
#define PCSR           (PSR+0x42)
#define PCMR           (PSR+0x43)
#define PCCR           (PSR+0x44)
#define PCCT           (PSR+0x45)
#define PCAR_HI        (PSR+0x46)
#define TBUS           (PSR+0x47)
#define TBUSA          (PSR+0x47)
#define TBUSB          (PSR+0x48)

#define LPCBM_ADDR     (PSR+0x60)
#define LPCBM_ADDR0    (PSR+0x60)
#define LPCBM_ADDR1    (PSR+0x61)
#define LPCBM_ADDR2    (PSR+0x62)
#define LPCBM_ADDR3    (PSR+0x63)
#define LPCBM_DATA     (PSR+0x64)
#define LPCBM_DATA0    (PSR+0x64)
#define LPCBM_DATA1    (PSR+0x65)
#define LPCBM_DATA2    (PSR+0x66)
#define LPCBM_DATA3    (PSR+0x67)
#define LPCBM_CMD      (PSR+0x68)
#define LPCBM_MID      (PSR+0x6C)
#define LPCBM_STS      (PSR+0x70)



#define RXOFF			(0x20
#define PSRRSTINTFLAG		(PSRWorkBase)
#define CAPINTFLAG		(PSRWorkBase+0x30) 
#define COMPVAL			(PSRWorkBase+0x34)
#define CMPINTFLAG   	 	(PSRWorkBase+0x38)
#define USHADINTFLAG   	 	(PSRWorkBase+0x3C)
#define FULLINTFLAG   	 	(PSRWorkBase+0x40)


#define LPCRSTIRQ 	 	(1<<1) 
#define LPCRSTINTEN		(1<<2)
#define CAPMATCHINT		(1<<1)
#define CAPFIFONEMPTY		(1<<2)
#define CAPFIFOFULLINTEN        (1<<5)
#define CAPFIFOCLRPTRS          (1<<6)

#define PCFNEMPTY		(1<<0)
#define CMPVALDETINTSTS		(1<<1)
#define HSPIINTSTS              (1<<2)    
#define USHADOWINTEN		(1<<3)
#define USHADOWINTSTS		(1<<4)
#define FULLDETINTSTS		(1<<5)

// LPC Bus Master bit definitions register
#define LPCBM_SIZE_8            (0<<0)
#define LPCBM_SIZE_16           (1<<0)
#define LPCBM_SIZE_32           (3<<0)
#define LPCBM_MEMCYC            (0<<3)
#define LPCBM_IOCYC             (1<<3)
#define LPCBM_READ              (0<<2)
#define LPCBM_WRITE             (1<<2)
#define LPCBM_TRIG              (1<<4)
#define LPCBM_INTF_EN           (1<<5) // IN SIOCFG8
#define LPCBM_MID0              0x2
#define LPCBM_MID1              0x3

extern void program_cap_addr (tU16);
extern void program_cap_cmpval (tU8);
extern void program_cap_maskval (tU8);
extern void EnableCAPINT (tU8);
extern void InstallPSRHandler (tU32);
extern void EnablePSRInterrupt (void);
extern void PSRhandler (void);
extern void PCRXhandler (void);
extern void Combo11_IntHandler (void);
extern void clr_cap_fifo_ptrs (void);
extern void lpcbm_enable (void);
extern void lpcbm_trigger (void);
extern void lpcbm_prog_io_wr_cmd (tU16, tU32, tU8);
extern void lpcbm_prog_io_rd_cmd (tU16, tU8);
extern void wait_for_lpcbm_busy (void);
extern void wait_for_lpcbm_not_busy (void);
extern void lpcbm_prog_mem_rd_cmd (tU32, tU8);
extern void lpcbm_prog_mem_wr_cmd (tU32, tU32, tU8);

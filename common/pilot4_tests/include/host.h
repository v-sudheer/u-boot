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

#ifdef HW_ENV
#define INDEX 0x2e00
#define DATA  0x2f00
#else
#define INDEX 0x2e00
#define DATA  0x2f00
#endif

#define LDN   0x07
#define MSB   0x60
#define LSB   0x61
#define EXT_MSB 0x62
#define EXT_LSB 0x63

#define LDN_CTL 0x30
#define IRQ_NUM 0x70
#define IRQ_TYP 0x71

#define SPI_LDN 0x0f
#define RTC_LDN 0x0e
#define SWC_LDN 0x03

#ifdef HW_ENV
#define RTCADDRI 0x4006
#define RTCADDRD 0x4106
#define EXT_RTCADDRI 0x4206
#define EXT_RTCADDRD 0x4306
#define RTC_MSB 0x06
#define RTC_LSB 0x40
#define RTC_EXT_MSB 0x06
#define RTC_EXT_LSB 0x42
#define SPI_MSB 0x06
#define SPI_LSB 0x40
#define SPI_INDEX 0x4006
#define SPI_DATA 0x4106
#define GPIOADDR 0x4006
#define GPIOMSB  0x06
#define GPIOLSB  0x40
#define SWCADDRI 0x4406
#define SWCADDRD 0x4506
#define SWC_MSB 0x60 //changed by Saurabh previous value was 0x06
#define SWC_LSB 0x61 //changed by Saurabh previous value was 0x44
#else 
#define RTCADDRI 0x7000
#define RTCADDRD 0x7100
#define EXT_RTCADDRI 0x7200
#define EXT_RTCADDRD 0x7300
#define RTC_MSB 0x00
#define RTC_LSB 0x70
#define RTC_EXT_MSB 0x00
#define RTC_EXT_LSB 0x72
#define SPI_MSB 0x08
#define SPI_LSB 0xa0
#define SPI_INDEX 0xa008
#define SPI_DATA 0xa108
#define GPIOADDR 0x00f2
#define GPIOMSB	 0xf2
#define GPIOLSB	 0x00
#define SWCADDRI 0xa408
#define SWCADDRD 0xa508
#define SWC_MSB 0x08 //0x60 //changed by Saurabh previous value was 0x08
#define SWC_LSB 0xa4 //0x61 //changed by Saurabh previous value was 0xa4
#endif

#define RTC_DOMAO 0xf1
#define RTC_MONAO 0xf2
#define RTC_CENO 0xf3
#define SPIO_SP1 0x02
#define SPIO_SP2 0x01  

#define SPIO_SP1_MSB 0x06
#define SPIO_SP1_LSB 0x40
#define SPIO_SP2_MSB 0x06
#define SPIO_SP2_LSB 0x50

#define SP1_SPDLL     0x4006
#define SP1_SPDLH     0x4106
#define SP1_SPLCR     0x4306
#define SP2_SPDLL     0x5006
#define SP2_SPDLH     0x5106
#define SP2_SPLCR     0x5306

//////////// SP1 equates /////////
#define SP1LDN                  0x2
#define SP1ADDR                 0xf803
#define SP1MSBVAL               0x03
#define SP1LSBVAL               0xf8
#define SP1_MSB                 0x60
#define SP1_LSB                 0x61
#define RBR_HOSTOF              0x0000
#define DLL_HOSTOFF             0x0000
#define THR_HOSTOFF             0x0000
#define DLH_HOSTOFF             0x0100
#define IER_HOSTOFF             0x0100
#define IIR_HOSTOFF             0x0200
#define FCR_HOSTOFF             0x0200
#define LCR_HOSTOFF             0x0300
#define MCR_HOSTOFF             0x0400
#define LSR_HOSTOFF             0x0500
#define MSR_HOSTOFF             0x0600
#define SP1DLL_HOST             SP1ADDR
#define SP1DLH_HOST	        SP1ADDR + 0x0100
#define SP1IER_HOST	        SP1ADDR + 0x0100
#define SP1IIR_HOST	        SP1ADDR + 0x0200
#define SP1FCR_HOST	        SP1ADDR + 0x0200
#define SP1LCR_HOST	        SP1ADDR + 0x0300
#define SP1MCR_HOST	        SP1ADDR + 0x0400
#define SP1LSR_HOST	        SP1ADDR + 0x0500
#define SP1MSR_HOST	        SP1ADDR + 0x0600
#define SP17C_HOST	        SP1ADDR	+ 0x1f00

//////////// SP2 equates /////////
#define SP2LDN                  0x1
#define SP2ADDR                 0xf802
#define SP2MSBVAL               0x02
#define SP2LSBVAL               0xf8
#define SP2DLL_HOST	        SP2ADDR
#define SP2DLH_HOST	        SP2ADDR + 0x0100
#define SP2THR_HOST	        SP2ADDR + 0x0100
#define SP2IER_HOST	        SP2ADDR + 0x0100
#define SP2IIR_HOST	        SP2ADDR + 0x0200
#define SP2FCR_HOST	        SP2ADDR + 0x0200
#define SP2LCR_HOST	        SP2ADDR + 0x0300
#define SP2MCR_HOST	        SP2ADDR + 0x0400
#define SP2LSR_HOST	        SP2ADDR + 0x0500
#define SP2MSR_HOST	        SP2ADDR + 0x0600
#define SP2_MSB                 0x60
#define SP2_LSB                 0x61

//////// KCS3 equates //////
#define KCS3LDN			0x8
#define KCS3DAT_MSB		0x60
#define KCS3DAT_LSB     	0x61
#define KCS3DATADDR		0x4006
#define KCS3DATMSBVAL		0x06
#define KCS3DATLSBVAL		0x40
#define KCS3COM_MSB     	0x62
#define KCS3COM_LSB     	0x63
#define KCS3COMADDR		0x3006
#define KCS3COMMSBVAL   	0x06
#define KCS3COMLSBVAL   	0x30

//////// KCS4 equates //////
#define KCS4LDN			0x9
#define KCS4DAT_MSB		0x60
#define KCS4DAT_LSB		0x61	
#define KCS4DATADDR		0x4006
#define KCS4DATMSBVAL		0x06
#define KCS4DATLSBVAL		0x40
#define KCS4COM_MSB     	0x62
#define KCS4COM_LSB     	0x63
#define KCS4COMADDR		0x3006
#define KCS4COMMSBVAL   	0x06
#define KCS4COMLSBVAL   	0x30

//////// KCS5 equates //////
#define KCS5LDN			0xA
#define KCS5DATADDR		0x4006
#define KCS5DATMSBVAL		0x06
#define KCS5DATLSBVAL		0x40
#define KCS5COMADDR		KCS5DATADDR + 0x0100
#define KCS5COMMSBVAL		KCS5DATMSBVAL
#define KCS5COMLSBVAL		KCS5DATLSBVAL + 0x01
#define KCS5_MSB		0x60
#define KCS5_LSB		0x61

//////////// SMIC Equates ////////////////
#define SMICLDN                 0xC
#define SMICADDR                0x4006
#define SMICMSBVAL              0x06
#define SMICLSBVAL              0x40
#define SMICDTR_HOST            SMICADDR + 0x0100
#define SMICCSR_HOST		SMICADDR + 0x0200
#define SMICFLG_HOST		SMICADDR + 0x0300
#define SMIC_MSB                0x60
#define SMIC_LSB                0x61

//////// BT HOST equates ////////

#define BTADDR			0x3006
#define BTMSBVAL		0x06
#define BTLSBVAL		0x30
#define BTCR_HOST     		BTADDR + 0x0000
#define BTDR_HOST      		BTADDR + 0x0100
#define BTIMSR_HOST    		BTADDR + 0x0200
#define BT_MSB    	  	0x60
#define BT_LSB     		0x61
#define BTLDN                   0xB

//////// PSR HOST equates ////////
#define PSRADDR                 0x0004 
#define PSRMSBVAL               0x04
#define PSRLSBVAL               0x00
#define PSR_MSB                 0x60
#define PSR_LSB                 0x61
#define PSRLDN                  0x00
#define LPCCR1_HOST             PSRADDR + 0x1100
#define LPCCR0_HOST             PSRADDR + 0x1000

//// WDT HOST equates ///////////

#define WDTADDR                 0x3006
#define WDTMSBVAL               0x06
#define WDTLSBVAL               0x30
#define WDCTL_HOST              WDTADDR + 0x0000
#define WDCNT_HOST              WDTADDR + 0x0400
#define WDT_MSB                 0x60
#define WDT_LSB                 0x61
#define WDTLDN                  0x5

//////// SWC HOST equates ////////
#define SWCLDN                  0x3
// SK #define SWC_MSB                 0x60
// SK #define SWC_LSB                 0x61

#ifdef HW_ENV

#define SWCINDEX                0x4006
#define SWCDATA                 0x4106
#define SWCMSBVAL               0x06
#define SWCLSBVAL               0x40
#define SWCADDR                 0x4006
#define PM1EVENTADDR            0x4406
#define PM1EVENTMSBVAL          0x06
#define PM1EVENTLSBVAL          0x44
#define GPE1ADDR                0x4806
#define GPE1MSBVAL              0x06
#define GPE1LSBVAL              0x48
#define PM1CTLADDR              0x3006
#define PM1CTLMSBVAL            0x06
#define PM1CTLLSBVAL            0x30	
#define HOSTWDT                 0x3006

#else

#define SWCINDEX                0xa408
#define SWCDATA                 0xa508
#define SWCMSBVAL               0x08
#define SWCLSBVAL               0xa4
#define SWCADDR                 0xa408
#define PM1EVENTADDR            0xa009
#define PM1EVENTMSBVAL          0x09
#define PM1EVENTLSBVAL          0xa0
#define GPE1ADDR                0xa409
#define GPE1MSBVAL              0x09
#define GPE1LSBVAL              0xa4
#define PM1CTLADDR              0xa809
#define PM1CTLMSBVAL            0x09
#define PM1CTLLSBVAL            0xa8
#define HOSTWDT                 0xc008

#endif

#define WKSTS0_HOST             0x00
#define WKSTS1_HOST             0x01
#define WKEN0_HOST              0x02
#define WKEN1_HOST              0x03
#define WKCFG0_HOST             0x04
#define WKCFG1_HOST             0x05
#define WKSMIEN0_HOST           0x06
#define WKIRQEN0_HOST           0x08
#define SWCTL_HOST              0x09
#define SWCSTAT0_HOST           0x0a
#define SWCSTAT1_HOST           0x0b
#define OCTL0S31_HOST           0x0e
#define OCTL1S31_HOST           0x0f
#define OCTL0S45_HOST           0x10
#define OCTL1S45_HOST           0x11
#define PBOUT0S31_HOST          0x12
#define PBOUT1S31_HOST          0x13
#define PBOUT0S45_HOST          0x14
#define PBOUT1S45_HOST          0x15
#define PS2CTL_HOST             0x16
#define PS2KEY0_HOST            0x18
#define PS2KEY1_HOST            0x19
#define PS2KEY2_HOST            0x1a
#define PS2KEY3_HOST            0x1b
#define PS2KEY4_HOST            0x1c
#define PS2KEY5_HOST            0x1d
#define PS2KEY6_HOST            0x1e
#define PS2KEY7_HOST            0x1f
#define S0SLPTYP_HOST           0x20
#define S1SLPTYP_HOST           0x21
#define S2SLPTYP_HOST           0x22
#define S3SLPTYP_HOST           0x23
#define S4SLPTYP_HOST           0x24
#define S5SLPTYP_HOST           0x25
#define SLPSTCFG_HOST           0x26
#define PASSCTL_HOST            0x27
#define PASSSTS_HOST            0x28
#define GPREG0_HOST             0x40

// ACPI Registers
#define PM1EVENT_MSB            0x62
#define PM1EVENT_LSB            0x63
#define PM1LDN                  0x3
#define PM1BSTSLOW_HOST         PM1EVENTADDR + 0x0000
#define PM1BSTSHIGH_HOST        PM1EVENTADDR + 0x0100
#define PM1BENLOW_HOST          PM1EVENTADDR + 0x0200
#define PM1BENHIGH_HOST         PM1EVENTADDR + 0x0300

// PM1 Control Equates 
#define PM1CTL_MSB              0x64
#define PM1CTL_LSB              0x65
#define PM1BCNTLOW_HOST         PM1CTLADDR + 0x0000
#define PM1BCNTHIGH_HOST        PM1CTLADDR + 0x0100

// GPE1 Blk equates
#define GPE1_MSB                0x66
#define GPE1_LSB                0x67
#define GPE1STS0_HOST           GPE1ADDR + 0x0000
#define GPE1STS1_HOST           GPE1ADDR + 0x0100
#define GPE1EN0_HOST            GPE1ADDR + 0x0400
#define GPE1EN1_HOST            GPE1ADDR + 0x0500

extern void unlock_cfg_c (void); 
extern void unlock_cfg_mode0 (void);
extern void sp1_enable_c (tU8);
extern void sp2_enable_c (tU8);
extern void kcs3_enable(tU8); 
extern void kcs4_enable(tU8); 
extern void kcs5_enable(tU8); 
extern void smic_enable(tU8); 
extern void bt_enable(tU8); 
extern void ldndisable_c(tU8); 
extern void prog_ldn (tU8, tU8, tU8, tU8, tU8);
extern void swc_enable1 (void);
extern void swc_enable (void);
extern void psr_enable (void);
extern void wdt_enable (tU8 inten);

extern void CHKSCIHIGH (void);
extern void CHKSCILOW (void);
extern void CHKSMIHIGH (void);
extern void CHKSMILOW (void);
extern void CHKONCTLHIGH (void);
extern void CHKONCTLLOW (void);
extern void CHKPBOUTHIGH (void) ;
extern void CHKPBOUTLOW (void);
extern void PULLVDDLOW (void);
extern void PULLVDDHIGH (void);
extern void PULLPBTLOW (void);
extern void PULLPBTHIGH (void);
extern void PULLRI1LOW (void);
extern void PULLRI1HIGH (void);
extern void PULLRI2LOW (void);
extern void PULLRI2HIGH (void);
extern void CHKGATEA20LOW (void);
extern void CHKGPIO1STS (void);
extern void CHKGPIO2STS (void);
extern void CHKGATEA20HIGH (void);
extern void CHKKBRSTLOW (void);
extern void CHKKBRSTHIGH (void);
extern void PULLPTHRU1INHIGH (void);
extern void PULLPTHRU1INLOW (void);
extern void PULLPTHRU2INHIGH (void);
extern void PULLPTHRU2INLOW (void);
extern void PULLPTHRU3INHIGH (void);
extern void PULLPTHRU3INLOW (void);
extern void CHKPTHRU1OUTLOW (void);
extern void CHKPTHRU1OUTHIGH (void);
extern void CHKPTHRU2OUTLOW (void);
extern void CHKPTHRU2OUTHIGH (void);
extern void CHKPTHRU3OUTLOW (void);
extern void CHKPTHRU3OUTHIGH (void);
extern void PULLCTSHIGH (void);
extern void PULLCTSLOW (void);
extern void PULLDSRHIGH (void);
extern void PULLDSRLOW (void);
extern void PULLRIHIGH (void);
extern void PULLRILOW (void);
extern void PULLDCDHIGH (void);
extern void PULLDCDLOW (void);
extern void CHKDTR0LOW (void);
extern void CHKDTR0HIGH (void);
extern void CHKRTS0LOW (void);
extern void CHKRTS0HIGH (void);
extern void CHKDTR1LOW (void);
extern void CHKDTR1HIGH (void);
extern void CHKRTS1LOW (void);
extern void CHKRTS1HIGH (void);
extern void PULLGPIO12LOW (void);
extern void CLEARSWCSTS (void);
extern void BMCCLEARSWCSTS (void);
extern void swcgpioconfigure (void);


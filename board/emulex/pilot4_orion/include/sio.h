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

// KCS Equates
#define IDR			KCS1BASE+0x00
#define ODRKB			KCS1BASE+0x00
#define ODRMOUSE		KCS1BASE+0x01
#define KCR			KCS1BASE+0x02
#define KST			KCS1BASE+0x03

#define IDR3			KCS3BASE+0x00
#define ODR3			KCS3BASE+0x00
#define KCR3			KCS3BASE+0x02
#define KST3			KCS3BASE+0x03

#define IDR4			KCS4BASE+0x00
#define ODR4			KCS4BASE+0x00
#define KCR4			KCS4BASE+0x02
#define KST4			KCS4BASE+0x03

#define IDR5			KCS5BASE+0x00
#define ODR5			KCS5BASE+0x00
#define KCR5			KCS5BASE+0x02	
#define KST5			KCS5BASE+0x03

#define IDROFF			0
#define ODROFF			0
#define KCROFF			2
#define KSTOFF			3	

// KST register
#define BMC4			(1<<7) 
#define BMC3			(1<<6)
#define BMC2			(1<<5)
#define BMC1			(1<<4)
#define COMDAT			(1<<3)
#define SMCATN			(1<<2)
#define IBF			(1<<1)
#define OBF			(1<<0)
#define KCSCD			(1<<3)



//SMIC Equates
//Register Offset from KCS5BASE
#define SMICFLG			7
#define SMICCSR			6
#define SMICDTR			5
#define SMICIR0			0X10
#define SMICIR1			0X11


//SMICFLG Register Bit Definitions

#define SMICBUSY		(1<<0) 
#define SMICSMSATN		(1<<2)
#define SMICSEVTATN		(1<<3)
#define SMICSMI			(1<<4)
#define SMICTXRDY		(1<<6)
#define SMICRXRDY		(1<<7)



//SMICIR0-SMICIR1 FIELD DEFINITIONS

#define BUSYI			(1<<0)  
#define CTLWI			(1<<1)
#define STARI			(1<<2)
#define HDTRI			(1<<3)
#define HDTWI			(1<<4)


//BT Equates
//Register Offsets From KCS5BASE
#define BTCR			0x08	//CONTROL REGISTER
#define BTDR			0x09	//DATA REGISTER
#define BTIMSR			0X0A	//INTERRUPT MASK REGISTER


//BTCR Register Bit Definitions

#define CLRWRPTR		(1<<0) 
#define CLRRDPTR		(1<<1)
#define H2BATN			(1<<2)
#define B2HATN			(1<<3)
#define BTSMSATN		(1<<4)
#define OEM0			(1<<5)
#define BTHBUSY			(1<<6)
#define BTBBUSY			(1<<7)


//BTIMSR Register Bit Definitions
#define B2HIRQEN		(1<<0) 
#define B2HIRQ			(1<<1)
#define H2BIRQEN		(1<<2)
#define OEM2			(1<<3)
#define OEM3			(1<<4)
#define BTHWRST			(1<<7)

#define SIRQMBSTS		(1<<15)
#define SIRQKCS1STS		(1<<1)
#define SIRQKCS2STS		(1<<12)
#define SIRQKCS3STS		(1<<5)
#define SIRQKCS4STS		(1<<6)

extern void InstallKCSCOMBOFiqHandler (tU32);
extern void EnableKCSCOMBOFiqInterrupt (tU8);
extern void InstallKCSCOMBOHandler (tU32);
extern void EnableKCSCOMBOInterrupt (tU8);
extern void KCS1Handler (void) ;
extern void KCS2Handler (void) ;
extern void KCS3Handler (void) ;
extern void KCS4Handler (void) ;
extern void KCS5Handler (void) ;
extern void BTHandler (void) ;
extern void SMICHandler (void) ;
extern void MBRXHandler (void) ;
extern void KCSCOMBOhandler (void) ;
extern void DISABLEKCSINTR (tU8);

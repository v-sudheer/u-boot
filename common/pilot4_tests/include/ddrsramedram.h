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

//;**********************************************************
//; Contains the EDRAM, DDRAM, and SSRAM related definitions. 
//;**********************************************************


//;Register definitions from MemoryEDRAM at 0x80300000

#define LEVD			0x10

#define UEVD			0x14

//;Register definitions from MemoryDDRSRAM at 0x80200000

#define DSTC			0x1C

#define DSSYSES			0x20

#define DSERRADD			0x24

#define EDSERRADD		0x28

#define EDSMISC			0x2C

#define DSMISC1			0x30

#define DSMISC2			0x34

#define DSMISC3			0x38


//;////////	DSSYSES

#define INIT_DDRSSRAM			1<<0
#define SINGLE_BIT_ERR			1<<5
#define MULTIPLE_BIT_ERR			1<<6
#define EN_SINGLE_INTR			1<<6
#define EN_MULTIPLE_INTR			1<<6
#define EN_ERR_INJ			1<<16


//;////////	DSMISC2

#define INIT_SMALL_DDRSSRAM		1<<13


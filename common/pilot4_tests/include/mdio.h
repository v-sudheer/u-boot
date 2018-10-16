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

#ifndef MDIO_H 
#define MDIO_H

extern unsigned int ioread32(volatile unsigned int * addr);
extern void iowrite32(unsigned int data, volatile unsigned int * addr);
extern void pilot_mdio_delay(void);
extern unsigned int Lan_MII_MDIO_Read(
        unsigned char DataBitLen        /* Number of bits to read in (maximum 32 bits) */
);
extern void Lan_MII_MDIO_Write(
        unsigned int Data,        /* Data to be written */
        unsigned char  DataBitLen   /* Number of relevant bits in Data */
);
extern void Lan_MII_MDIO_WriteZ(void);
extern void Lan_MII_MDIO_Release(void);
extern unsigned int Lan_MII_MDIO_ReadReg16(
        unsigned char phy_id,
        unsigned char RegNum        /* MII PHY register number */
);
extern void Lan_MII_MDIO_WriteReg16(
        unsigned char  RegNum,      /* MII PHY register number */
        unsigned char phy_id,
        unsigned int Data         /* Data to be written */
);



/* chip types.  careful!  order is VERY IMPORTANT here, as these
 * are used throughout the driver as indices into arrays */
/* Note 21142 == 21143. */
enum chips {
        DC21040 = 0,
        DC21041 = 1,
        DC21140 = 2,
        DC21142 = 3, DC21143 = 3,
        LC82C168,
        MX98713,
        MX98715,
        MX98725,
        AX88140,
        PNIC2,
        COMET,
        COMPEX9881,
        I21145,
        DM910X,
        CONEXANT,
    PILOT2_TULIP,
};


enum tulip_offsets {
        CSR0  = 0,
        CSR1  = 0x08,
        CSR2  = 0x10,
        CSR3  = 0x18,
        CSR4  = 0x20,
        CSR5  = 0x28,
        CSR6  = 0x30,
        CSR7  = 0x38,
        CSR8  = 0x40,
        CSR9  = 0x48,
        CSR10 = 0x50,
        CSR11 = 0x58,
        CSR12 = 0x60,
        CSR13 = 0x68,
        CSR14 = 0x70,
        CSR15 = 0x78,
        CSR16 = 0x80,
        CSR17 = 0x88,
        CSR18 = 0x90,
        CSR19 = 0x98,
        CSR20 = 0xA0
};
#define MII_MGMT_DATA_IN            (1 << 19) /* 0x00080000L */
#define MII_MGMT_RD_WRBAR           (1 << 18) /* 0x00040000L */
#define MII_MGMT_DATA_OUT           (1 << 17) /* 0x00020000L */
#define MII_MGMT_CLK                (1 << 16) /* 0x00010000L */

#define MDI     MII_MGMT_DATA_IN    /* Input */
#define MDRE    MII_MGMT_RD_WRBAR   /* Input if set, Output if not set */
#define MDO     MII_MGMT_DATA_OUT   /* Output */
#define MDC     MII_MGMT_CLK        /* Clock */

#define MDIO_RD 0                   /* BIT19 Clear */
#define MDIO_WR MDRE                /* BIT19 Set   */

/* Read and write the MII registers using software-generated serial
   MDIO protocol.  It is just different enough from the EEPROM protocol
   to not share code.  The maxium data clock rate is 2.5 Mhz. */
#define MDIO_SHIFT_CLK          0x10000
#define MDIO_DATA_WRITE0        0x00000
#define MDIO_DATA_WRITE1        0x20000
#define MDIO_ENB                0x00000 /* Ignore the 0x02000 databook setting. */
#define MDIO_ENB_IN             0x40000
#define MDIO_DATA_READ          0x80000

/* The maximum data clock rate is 2.5 Mhz.  The minimum timing is usually
   met by back-to-back PCI I/O cycles, but we insert a delay to avoid
   "overclocking" issues or future 66Mhz PCI. */
#define mdio_delay() ioread32(mdio_addr)

#define IOADDR_INTERFACE_ETH_A (0x40500000)
#define IOADDR_INTERFACE_ETH_B (0x40600000)

#endif

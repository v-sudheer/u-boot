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

#define ROMSPACE_OFFSET		0x01840000
#define CONFIGSPACE_OFFSET	0x01834400
#define IOSPACE_OFFEST		0x01834000
#define VGAMEMSPACE_OFFSET	0x01804000
#define MGABASE1SPACE_OFFSET	0x01800000
#define MGABASE2SPACE_OFFSET	0x0
#define MGABASE3SPACE_OFFSET	0x01000000

#define AHB2PCI_CONFIG		(AHB2PCIBase + CONFIGSPACE_OFFSET)
#define AHB2PCI_IO		(AHB2PCIBase + IOSPACE_OFFEST)
#define AHB2PCI_ROM		(AHB2PCIBase + ROMSPACE_OFFSET)
#define AHB2PCI_VGAMEM		(AHB2PCIBase + VGAMEMSPACE_OFFSET)
#define AHB2PCI_MGABASE1	(AHB2PCIBase + MGABASE1SPACE_OFFSET)
#define AHB2PCI_MGABASE2	(AHB2PCIBase + MGABASE2SPACE_OFFSET)
#define AHB2PCI_MGABASE3	(AHB2PCIBase + MGABASE3SPACE_OFFSET)

#define MasterAbortInt      	1 << 0
#define AccessOnDisableInt      1 << 1
#define DecodeErrorInt		1 << 2
#define RetryTimeoutInt      	1 << 3

#define AHB2PCI_Intr_Flag	AHB2PCIWorkBase


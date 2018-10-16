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
/// Filename: xdma.h
/// Section: XDMA
/// Author: Saurabh Kasat
/// Description: Header file XDMA modules
//////////////////////////////////////////////////


#define XDMABASE        (FGBBASE + 0x380)

#define XDMACTL         (XDMABASE + 0x0) 
#define XDMASTS         (XDMABASE + 0x4)
#define XDMAQPTR        (XDMABASE + 0x5)
#define XDMAAD0         (XDMABASE + 0x10) 
#define XDMAAD1         (XDMABASE + 0x14)
#define XDMAAD2         (XDMABASE + 0x18)
#define XDMAAD3         (XDMABASE + 0x1C)
#define XDMAQBASE       (XDMABASE + 0x40)

// Register bit definitions

// XDMACTL
#define GLBL_Q_PAUSE    (1<<1)
#define ARM_MODE        (1<<2)

// XDMASTS
#define Q_SKIPPED       (1<<0)
#define Q_PAUSED        (1<<1)
#define Q_PAUSED_GLBL   (1<<2)
#define Q_PAUSED_DPTR   (1<<3)
#define Q_FULL          (1<<4)
#define ARM_DPTR_DONE   (1<<5)


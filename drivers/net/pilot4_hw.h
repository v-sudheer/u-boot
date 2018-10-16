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

#ifndef _PILOT4_HW_H_
#define _PILOT4_HW_H_

#include <config.h>

/* U-Boot does not use MMU. So no mapping */
#define IO_ADDRESS(x)	(x)
#define MEM_ADDRESS(x)  (x)
/*
#include "configs/pilot4_map.h"
#include "hwreg.h"
#include "hwdef.h"
#include "serreg.h"
#include "macreg.h"
#include "pilot_types.h"
*/


#endif

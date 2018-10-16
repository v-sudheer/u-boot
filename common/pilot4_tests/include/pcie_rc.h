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

/*************************************************************************************
*
* Title            : PILOT 4 - PCIE Root Complex Test cases
* File Name        : pcie_rc.h
* Created by       : Ashok Reddy Soma
* Date             : 07/22/2013
* Sub Modules      : None
* Description      : This file contains common defines that will be used in
*                    PCIE Root Complex test cases.
*
*************************************************************************************/

/* PCIE Root Complex is a slave on the AXI bus. 
   The slave address range is 0x6000_0000 -> 0x6fff_ffff. 
*/
#define PCIE_MEM_BASE (0x60700000)
#define BASEREG (0x60000000)


extern void init_PCIE(void);
extern void wait_for_pcie_link_up(void);
extern tU32 ep_config0_rd(tU32 bus, tU32 func, tU32 reg_offset, tU8 byte_enable);

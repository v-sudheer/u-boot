// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2017 Google, Inc
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 */

#include <common.h>
#include <errno.h>
#include <asm/io.h>
#include <asm/arch/aspeed_scu_info.h>

extern int
aspeed_get_mac_phy_interface(u8 num)
{
	u32 strap1 = readl(ASPEED_HW_STRAP1);
#ifdef ASPEED_HW_STRAP2
	u32 strap2 = readl(ASPEED_HW_STRAP2);
#endif
	switch(num) {
		case 0:
			if(strap1 & BIT(6)) {
				return 1;
			} else {
				return 0;
			}
			break;
		case 1:
			if(strap1 & BIT(7)) {
				return 1;
			} else {
				return 0;
			}
			break;
		case 2:
			if(strap2 & BIT(6)) {
				return 1;
			} else {
				return 0;
			}
			break;
		case 3:
			if(strap2 & BIT(7)) {
				return 1;
			} else {
				return 0;
			}
			break;
	}
	return -1;
}

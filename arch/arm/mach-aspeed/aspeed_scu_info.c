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

#define SCU_HW_STRAP_MAC1_RGMII		(0x1 << 7)
#define SCU_HW_STRAP_MAC0_RGMII		(0x1 << 6)

extern int
aspeed_get_mac_phy_interface(u8 num)
{
	u32 strap = readl(ASPEED_HW_STRAP1);
	switch(num) {
		case 0:
			if(strap & SCU_HW_STRAP_MAC0_RGMII) {
				return 1;
			} else {
				return 0;
			}
			break;
		case 1:
			if(strap & SCU_HW_STRAP_MAC1_RGMII) {
				return 1;
			} else {
				return 0;
			}
			break;
	}
	return -1;
}

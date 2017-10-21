/*
 * ehci-ast.c - EHCI driver for the Aspeed SoC
 *
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <common.h>
#include <asm/io.h>
#include <usb.h>
#include <asm/arch/ast-scu.h>
#include "ehci.h"


DECLARE_GLOBAL_DATA_PTR;

/*
 * Create the appropriate control structures to manage
 * a new EHCI host controller.
 */
int ehci_hcd_init(int index, struct ehci_hccr **hccr, struct ehci_hcor **hcor)
{
	ast_scu_multi_func_usb_port1_mode(1);
	ast_scu_init_usb_port1();
//	ast_scu_multi_func_usb_port2_mode(1);
//	ast_scu_init_usb_port2();

	*hccr = (struct ehci_hccr *)(AST_EHCI_BASE);
	*hcor = (struct ehci_hcor *)((uint32_t) *hccr
			+ HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));

	debug("ehci-ast: init hccr %x and hcor %x hc_length %d\n",
		(uint32_t)*hccr, (uint32_t)*hcor,
		(uint32_t)HC_LENGTH(ehci_readl(&(*hccr)->cr_capbase)));

	return 0;
}

/*
 * Destroy the appropriate control structures corresponding
 * the the EHCI host controller.
 */
int ehci_hcd_stop(int index)
{
	return 0;
}

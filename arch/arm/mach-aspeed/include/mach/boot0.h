/*
 * Specialty padding for the ASPEED image
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __BOOT0_H
#define __BOOT0_H

#define ARM_SOC_BOOT0_HOOK						\
	.word	0x1a031220;	/* VENDER ID : CHIP ID */	\
	.word	_end - _start	/* size */		\

#endif /* __BOOT0_H */

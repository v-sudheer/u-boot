/*
 * Specialty padding for the ASPEED image
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __BOOT0_H
#define __BOOT0_H

#define ARM_SOC_BOOT0_HOOK						\
	.word	0x0;	/* Key location */	\
	.word	0x0;	/* start address of image */	\
	.word	0x0;	/* image size */	\
	.word	0x0;	/* signature address */ 	\
	.word	0x0;	/* checksum */			\
	.word	_end - _start	/* size */		\

#endif /* __BOOT0_H */

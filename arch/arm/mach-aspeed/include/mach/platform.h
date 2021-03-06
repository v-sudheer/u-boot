/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _AST_PLATFORM_H_
#define _AST_PLATFORM_H_

#define AST_PLL_25MHZ			25000000
#define AST_PLL_24MHZ			24000000
#define AST_PLL_12MHZ			12000000

/*********************************************************************************/
#if defined(CONFIG_ARCH_AST1010)
#include "ast1010_platform.h"
#elif defined(CONFIG_ARCH_AST1520)
#include <asm/mach/ast1520_platform.h>
#elif defined(CONFIG_ARCH_AST2000)
#include <asm/mach/ast2000_platform.h>
#elif defined(CONFIG_ARCH_AST2100)
#include <asm/mach/ast2100_platform.h>
#elif defined(CONFIG_ARCH_AST2200)
#include <asm/mach/ast2200_platform.h>
#elif defined(CONFIG_MACH_ASPEED_G3)
#include "ast_g3_platform.h"
#elif defined(CONFIG_MACH_ASPEED_G4)
#define ASPEED_MAC_COUNT	2
#include "ast_g4_platform.h"
#define ASPEED_HW_STRAP1	0x1e6e2070
#define ASPEED_REVISION_ID	0x1e6e207C
#define ASPEED_SYS_RESET_CTRL	0x1e6e203C
#define ASPEED_VGA_HANDSHAKE0	0x1e6e2040		/*	VGA fuction handshake register */
#elif defined(CONFIG_MACH_ASPEED_G5)
#define ASPEED_MAC_COUNT	2
#include "ast_g5_platform.h"
#define ASPEED_HW_STRAP1	0x1e6e2070
#define ASPEED_HW_STRAP2	0x1e6e20D0
#define ASPEED_REVISION_ID	0x1e6e207C
#define ASPEED_SYS_RESET_CTRL	0x1e6e203C
#define ASPEED_VGA_HANDSHAKE0	0x1e6e2040		/*	VGA fuction handshake register */
#define ASPEED_MAC_COUNT	2
#elif defined(CONFIG_MACH_ASPEED_G6)
#include "ast_g6_platform.h"
#define ASPEED_HW_STRAP1	0x1e6e2500
#define ASPEED_HW_STRAP2	0x1e6e2510
#define ASPEED_REVISION_ID	0x1e6e2004
#define ASPEED_SYS_RESET_CTRL	0x1e6e2064
#define ASPEED_VGA_HANDSHAKE0	0x1e6e2100		/*	VGA fuction handshake register */
#define ASPEED_MAC_COUNT	4
#else
#err "No define for platform.h"
#endif
/*********************************************************************************/
/* Companion Base Address */
#if defined(CONFIG_ARCH_AST1070)
#include <mach/ast1070_platform.h>
#endif
/*********************************************************************************/

#define AST_NOR_SIZE		               	  		0x01000000	/* AST2300 NOR size 16MB */
 
#endif

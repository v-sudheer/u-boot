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
#include "ast_g4_platform.h"
#define ASPEED_HW_STRAP1	0x1e6e2070
#elif defined(CONFIG_MACH_ASPEED_G5)
#include "ast_g5_platform.h"
#define ASPEED_HW_STRAP1	0x1e6e2070
#elif defined(CONFIG_MACH_ASPEED_G6)
#include "ast_g6_platform.h"
#define ASPEED_HW_STRAP1	0x1e6e2500
#elif defined(AST_SOC_CAM)
#include "ast_cam_platform.h"
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

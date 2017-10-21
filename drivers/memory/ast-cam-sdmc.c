/*
 * ast-cam-sdmc.c - CAM SDMC driver for the Aspeed SoC
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
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/arch/platform.h>

#include <asm/arch/ast-sdmc.h>

/************************************  Registers for SDMC ****************************************/ 
#define AST_SDMC_PROTECT		0x00		/*	protection key register	*/
#define AST_SDMC_CONFIG		0x04		/*	Configuration register */


/*	AST_SDMC_PROTECT: 0x00  - protection key register */
#define SDMC_PROTECT_UNLOCK			0xFC600309

/*	AST_SDMC_CONFIG :0x04	 - Configuration register */
#define SDMC_CONFIG_VER_AST1220		(0x1 << 28)
#define SDMC_CONFIG_CACHE_EN			(0x1 << 10)

#define SDMC_CONFIG_MEM_GET(x)		(x & 0x3)


/****************************************************************************************/

//#define AST_SDMC_LOCK
//#define AST_SDMC_DEBUG

#ifdef AST_SDMC_DEBUG
#define SDMCDBUG(fmt, args...) printf("%s() " fmt, __FUNCTION__, ## args)
#else
#define SDMCDBUG(fmt, args...)
#endif

#define SDMCMSG(fmt, args...) printf(fmt, ## args)
/****************************************************************************************************************/
static u32 ast_sdmc_base = AST_SDMC_BASE;

static inline u32 
ast_sdmc_read(u32 reg)
{
	u32 val;
		
	val = readl(ast_sdmc_base + reg);
	
	SDMCDBUG("ast_sdmc_read : reg = 0x%08x, val = 0x%08x\n", reg, val);
	
	return val;
}

static inline void
ast_sdmc_write(u32 val, u32 reg) 
{
	SDMCDBUG("ast_sdmc_write : reg = 0x%08x, val = 0x%08x\n", reg, val);
#ifdef CONFIG_AST_SDMC_LOCK
	//unlock 
	writel(SDMC_PROTECT_UNLOCK, ast_sdmc_base);
	writel(val, ast_sdmc_base + reg);
	//lock
	writel(0xaa,ast_sdmc_base);	
#else
	writel(SDMC_PROTECT_UNLOCK, ast_sdmc_base);

	writel(val, ast_sdmc_base + reg);
#endif
}
//***********************************Information ***********************************
extern u32
ast_sdmc_get_mem_size(void)
{
	u32 size=0;
	u32 conf = ast_sdmc_read(AST_SDMC_CONFIG);
	
	switch(SDMC_CONFIG_MEM_GET(conf)) {
		case 0:
			size = 512*1024*1024;
			break;
		case 1:
			size = 1024*1024*1024;
			break;
		case 2:
			size = 2048*1024*1024;
			break;
		default:
			SDMCMSG("error ddr size \n");
			break;
	}
	
	return size;
}

/***********************************************************************************************************************/

/*
 * BMC SDMC driver for the Aspeed SoC
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
/***********************  Registers for SDMC ***************************/ 
#define ASPEED_SDMC_PROTECT		0x00		/*	protection key register	*/
#define ASPEED_SDMC_CONFIG		0x04		/*	Configuration register */
#define ASPEED_SDMC_MEM_REQ		0x08		/*	Graphics Memory Protection register */

#define ASPEED_SDMC_ISR		0x50		/*	Interrupt Control/Status Register */
#define ASPEED_SDMC_CACHE_ECC_RANGE                 0x54            /*      ECC/CACHE Address Range Control Register */

/*	ASPEED_SDMC_PROTECT: 0x00  - protection key register */
#define SDMC_PROTECT_UNLOCK			0xFC600309

/*	ASPEED_SDMC_CONFIG :0x04	 - Configuration register */
#define SDMC_CONFIG_VER_GET(x)		((x >> 28) & 0x3)
#define ASPEED_LEGACY_SDMC			0
#define ASPEED_G5_SDMC				1
#define ASPEED_G6_SDMC				3

#define SDMC_G5_CONFIG_CACHE_EN		BIT(10)	//only in ast2500 

#define SDMC_CONFIG_EEC_EN			BIT(7)
#define SDMC_G5_CONFIG_DDR4			BIT(4) //only in ast2500/ast2600 

#define SDMC_CONFIG_VRAM_GET(x)		((x >> 2) & 0x3)
#define SDMC_CONFIG_MEM_GET(x)		(x & 0x3)

/*	#define ASPEED_SDMC_ISR	 : 0x50	- Interrupt Control/Status Register */
#define SDMC_ISR_CLR				BIT(31)
#define SDMC_ISR_RW_ACCESS			BIT(29)

#define SDMC_ISR_GET_ECC_RECOVER(x)	((x >> 16) & 0xff)
#define SDMC_ISR_GET_ECC_UNRECOVER(x)	((x >> 12) & 0xf)
/**************************************************************/

//#define AST_SDMC_LOCK
//#define AST_SDMC_DEBUG

#ifdef AST_SDMC_DEBUG
#define SDMCDBUG(fmt, args...) printf("%s() " fmt, __FUNCTION__, ## args)
#else
#define SDMCDBUG(fmt, args...)
#endif

#define SDMCMSG(fmt, args...) printf(fmt, ## args)

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
extern u8
ast_sdmc_get_ecc(void)
{
	if(ast_sdmc_read(ASPEED_SDMC_CONFIG) & SDMC_CONFIG_EEC_EN)
		return 1;
	else
		return 0;
}

/* aspeed-g5/aspeed-g4/g6 don't have  */
extern u8
ast_sdmc_get_cache(void)
{
#ifdef CONFIG_MACH_ASPEED_G5
	if(ast_sdmc_read(ASPEED_SDMC_CONFIG) & SDMC_G5_CONFIG_CACHE_EN)
		return 1;
	else
		return 0;
#else
	return 0;
#endif	
}

extern u8
ast_sdmc_get_dram(void)
{
#if defined(CONFIG_MACH_ASPEED_G5) || defined(CONFIG_MACH_ASPEED_G5)
	if(ast_sdmc_read(ASPEED_SDMC_CONFIG) & SDMC_G5_CONFIG_DDR4)
		return 1;
	else
		return 0;
#else
	return 0;
#endif
}

static const u32 aspeed_vram_table[] = {
	0x00800000,	//8MB
	0x01000000,	//16MB
	0x02000000,	//32MB
	0x04000000,	//64MB
};

extern u32
ast_sdmc_get_vram_size(void)
{
	u32 size_conf = SDMC_CONFIG_VRAM_GET(ast_sdmc_read(ASPEED_SDMC_CONFIG));
	return aspeed_vram_table[size_conf];
}

static const u32 ast2400_dram_table[] = {
	0x04000000,	//64MB
	0x08000000,	//128MB
	0x10000000, //256MB
	0x20000000,	//512MB
};

static const u32 ast2500_dram_table[] = {
	0x08000000,	//128MB
	0x10000000,	//256MB
	0x20000000,	//512MB
	0x40000000,	//1024MB
};

static const u32 ast2600_dram_table[] = {
	0x10000000,	//256MB
	0x20000000,	//512MB
	0x40000000,	//1024MB
	0x80000000,	//2048MB
};

extern u32
ast_sdmc_get_mem_size(void)
{
	u32 size = 0;
	u32 conf = ast_sdmc_read(ASPEED_SDMC_CONFIG);
	u32 size_conf = SDMC_CONFIG_MEM_GET(conf); 
	
	switch(SDMC_CONFIG_VER_GET(conf)) {
		case ASPEED_LEGACY_SDMC:
			size = ast2400_dram_table[size_conf];
			break;
		case ASPEED_G5_SDMC:
			size = ast2500_dram_table[size_conf];
			break;
		case ASPEED_G6_SDMC:
			size = ast2600_dram_table[size_conf];
			break;
	}

	return size;
}

extern u32
ast_sdmc_get_ecc_size(void)
{
	return ast_sdmc_read(ASPEED_SDMC_CACHE_ECC_RANGE);
}

extern u8
ast_sdmc_get_ecc_recover_count(void)
{
	return SDMC_ISR_GET_ECC_RECOVER(ast_sdmc_read(ASPEED_SDMC_ISR));
}

extern u8
ast_sdmc_get_ecc_unrecover_count(void)
{
	return SDMC_ISR_GET_ECC_UNRECOVER(ast_sdmc_read(ASPEED_SDMC_ISR));
}

extern void
ast_sdmc_set_ecc(u8 enable)
{
	if(enable) 
		ast_sdmc_write(ast_sdmc_read(ASPEED_SDMC_CONFIG) | SDMC_CONFIG_EEC_EN, ASPEED_SDMC_CONFIG);
	else
		ast_sdmc_write(ast_sdmc_read(ASPEED_SDMC_CONFIG) & ~SDMC_CONFIG_EEC_EN, ASPEED_SDMC_CONFIG);	
}

extern void
ast_sdmc_disable_mem_protection(u8 req)
{
	ast_sdmc_write(ast_sdmc_read(ASPEED_SDMC_MEM_REQ) & ~(1<< req), ASPEED_SDMC_MEM_REQ);
}

extern u32
ast_sdmc_dram_size(void)
{
#ifdef CONFIG_DRAM_ECC
	return CONFIG_DRAM_ECC_SIZE;
#else
	u32 vga = ast_sdmc_get_vram_size();
	u32 dram = ast_sdmc_get_mem_size();

	return (dram - vga);
#endif

}

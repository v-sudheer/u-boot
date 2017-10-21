/*
 * ast_spi.c - SPI driver for the Aspeed SoC
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
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>

#include <asm/arch/ast-scu.h>
#include <asm/arch/regs-spi.h>
#include <asm/arch/regs-scu.h>


struct ast_spi_host {
	struct spi_slave slave;	
	void		*regs;
	void 		*buff;			
	u32 	(*get_clk)(void);
};

//#define ASPEED_SPI_DEBUG

#ifdef ASPEED_SPI_DEBUG
#define SPIDBUG(fmt, args...) printf(fmt, ## args)
#else
#define SPIDBUG(fmt, args...)
#endif

static inline struct ast_spi_host *to_ast_spi(struct spi_slave *slave)
{
	return container_of(slave, struct ast_spi_host, slave);
}

static inline void
ast_spi_write(struct ast_spi_host *spi, u32 val, u32 reg)
{
	SPIDBUG(" val: %x , reg : %x \n",val,reg);	
	__raw_writel(val, spi->regs + reg);
}

static inline u32
ast_spi_read(struct ast_spi_host *spi, u32 reg)
{
	return __raw_readl(spi->regs + reg);
}

DECLARE_GLOBAL_DATA_PTR;


void spi_init_f(void)
{
	SPIDBUG("spi_init_fffff \n");
}

void spi_init_r(void)
{
	SPIDBUG("spi_init_rrrrr \n");
}

void spi_init(void)
{
	SPIDBUG("spi_init iiii\n");
}

static u32 ast_spi_calculate_divisor(u32 max_speed_hz)
{
	// [0] ->15 : HCLK , HCLK/16
	u8 SPI_DIV[16] = {16, 7, 14, 6, 13, 5, 12, 4, 11, 3, 10, 2, 9, 1, 8, 0};
	u32 i, hclk, spi_cdvr=0;

	hclk = ast_get_ahbclk();
	for(i=1;i<17;i++) {
		if(max_speed_hz >= (hclk/i)) {
			spi_cdvr = SPI_DIV[i-1];
//			printf("hclk = %d , spi_cdvr = %d \n",hclk, spi_cdvr);
			break;
		}
	}
		
//	printk("hclk is %d, divisor is %d, target :%d , cal speed %d\n", hclk, spi_cdvr, spi->max_speed_hz, hclk/i);
	return spi_cdvr;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	struct ast_spi_host	*spi;
	u32			spi_ctrl;
	u32 		div;

	debug("bus %d , cs %d , max_hz %d , mode %d \n", bus, cs, max_hz, mode);	

	spi = malloc(sizeof(struct ast_spi_host));
	if (!spi)
		return NULL;

	spi->slave.bus = bus;
	spi->slave.cs = cs;

#if defined(CONFIG_ARCH_AST1010)
	if(bus == 1)
		__raw_writel( 0x80800000, AST_SCU_BASE + AST_SCU_CPU_BASE1_ADDR);
#endif
	
	switch (bus) {
	case 0:
		spi->regs = (void *)AST_SPI0_BASE;
		spi->buff = (void *)AST_SPI0_MEM;
		break;
#ifdef AST_SPI1_BASE
	case 1:
		spi->regs = (void *)AST_SPI1_BASE;
		spi->buff = (void *)AST_SPI1_MEM;
		break;
#endif
	default:
		return NULL;
	}

	spi_ctrl = ast_spi_read(spi, AST_SPI_CTRL);	
	
	//TODO MASK first
	spi_ctrl &= ~SPI_CLK_DIV_MASK;
	//TODO ... check clk first and set div ..
	div = ast_spi_calculate_divisor(max_hz);
	spi_ctrl |= SPI_CLK_DIV(div);

//	if (SPI_CPOL & mode) 
//		spi_ctrl |= SPI_CPOL_1;
//	else
//		spi_ctrl &= ~SPI_CPOL_1;

	//ISSUE : ast spi ctrl couldn't use mode 3, so fix mode 0
	spi_ctrl &= ~SPI_CPOL_1;

	ast_spi_write(spi, spi_ctrl, AST_SPI_CTRL);
	
	return &spi->slave;

}

void spi_free_slave(struct spi_slave *slave)
{
	free(slave);
}

int spi_claim_bus(struct spi_slave *slave)
{
	struct ast_spi_host *spi = to_ast_spi(slave);
	SPIDBUG("spi_claim_bus bus %d, cs %d\n",slave->bus, slave->cs);

	ast_spi_write(spi, ast_spi_read(spi,AST_SPI_CONFIG) | SPI_CONF_WRITE_EN, AST_SPI_CONFIG);

	return 0;

}

void spi_release_bus(struct spi_slave *slave)
{
	struct ast_spi_host *spi = to_ast_spi(slave);

	SPIDBUG("spi_release_bus >>>>> >>>> \n");

	ast_spi_write(spi, ast_spi_read(spi, AST_SPI_CTRL) | SPI_CE_INACTIVE, AST_SPI_CTRL);	
	ast_spi_write(spi, ast_spi_read(spi, AST_SPI_CTRL) & ~SPI_CMD_USER_MODE, AST_SPI_CTRL);	
	ast_spi_write(spi, ast_spi_read(spi,AST_SPI_CONFIG) & ~SPI_CONF_WRITE_EN, AST_SPI_CONFIG);	

#if defined(CONFIG_ARCH_AST1010)
	if(spi->slave.bus == 1)
		__raw_writel( MAP_AREA2_BASE, AST_SCU_BASE + AST_SCU_CPU_BASE1_ADDR);
#endif	
#if 0
	//Set to fast read mode ...
	
#if defined(SG5_166_166MHz)	
	ast_spi_write(spi, SPI_CMD_DATA(0x0b) | SPI_CLK_DIV(0xE) | SPI_DUMMY_LOW(1) | SPI_CMD_FAST_R_MODE, AST_SPI_CTRL);
#else   /* 200_100 or 166_83 */
	ast_spi_write(spi, SPI_CMD_DATA(0x0b) | SPI_CLK_DIV(0x7) | SPI_DUMMY_LOW(1) | SPI_CMD_FAST_R_MODE, AST_SPI_CTRL);
#endif
#endif
}

int spi_xfer(struct spi_slave *slave, unsigned int bitlen, const void *dout,
	     void *din, unsigned long flags)
{
	struct ast_spi_host *spi = to_ast_spi(slave);
	/* assume spi core configured to do 8 bit transfers */
	uint bytes = bitlen / 8;
	const uchar *txp = dout;
	uchar *rxp = din;


	SPIDBUG("%s: bus:%i cs:%i bitlen:%i bytes:%i flags:%lx\n", __func__,
		slave->bus, slave->cs, bitlen, bytes, flags);

	if (bitlen == 0)
		goto done;

	if (bitlen % 8) {
		flags |= SPI_XFER_END;
		goto done;
	}

   if (flags & SPI_XFER_BEGIN) {
	   SPIDBUG("\n ----------Xfer BEGIN -------\n");
	   ast_spi_write(spi, ast_spi_read(spi, AST_SPI_CTRL) | SPI_CMD_USER_MODE, AST_SPI_CTRL);
	   ast_spi_write(spi, ast_spi_read(spi, AST_SPI_CTRL) & ~SPI_CE_INACTIVE, AST_SPI_CTRL);
   }   	


   while (bytes--) {
   	uchar d;
   	   if(txp) {
		   d = txp ? *txp++ : 0xff;
//		   SPIDBUG("%s: tx:%x ", __func__, d);
		   __raw_writeb(d, spi->buff);
   	   }
//	   SPIDBUG("\n");
	   if (rxp) {
	   		d = __raw_readb(spi->buff);
		   *rxp++ = d;
//		   SPIDBUG("rx:%x ", d);
	   }
   }
//   	SPIDBUG("\n");
done:
   if (flags & SPI_XFER_END) {
   		SPIDBUG("SPI_XFER_END --- >\n");
	   ast_spi_write(spi, ast_spi_read(spi, AST_SPI_CTRL) | SPI_CE_INACTIVE, AST_SPI_CTRL);
	   ast_spi_write(spi, (ast_spi_read(spi, AST_SPI_CTRL) & ~SPI_CMD_USER_MODE) , AST_SPI_CTRL);
   }

   return 0;

}


/*
 * Configuation settings for the ASPEED AST Chip.
 *
 * Copyright (C) 2012-2020 ASPEED Tech. Inc.
 * Ryan Chen (ryan_chen@aspeedtech.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include <spi.h>
#include <malloc.h>
#include <asm/io.h>

#include <asm/arch/regs-fmc.h>
#include <asm/arch/ast-scu.h>

struct ast_spi_host {
	struct spi_slave slave;	
	void		*base;	
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

DECLARE_GLOBAL_DATA_PTR;

static inline struct ast_spi_host *to_ast_spi(struct spi_slave *slave)
{
	return container_of(slave, struct ast_spi_host, slave);
}

static inline void
ast_spi_write(struct ast_spi_host *spi, u32 val, u32 reg)
{
	SPIDBUG(" val: %x , reg : %x \n",val,reg);	
	if(reg == 0x0)	
		__raw_writel(val, spi->regs + reg);
	else
		__raw_writel(val, spi->base + reg);
}

static inline u32
ast_spi_read(struct ast_spi_host *spi, u32 reg)
{
	if(reg == 0x0)
		return __raw_readl(spi->regs + reg);
	else
		return __raw_readl(spi->base + reg);
}


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
		
//	printf("hclk is %d, divisor is %d, target :%d , cal speed %d\n", hclk, spi_cdvr, max_speed_hz, hclk/SPI_DIV[i]);
	return spi_cdvr;
}

struct spi_slave *spi_setup_slave(unsigned int bus, unsigned int cs,
				  unsigned int max_hz, unsigned int mode)
{
	struct ast_spi_host	*spi;
	u32			spi_ctrl;
	u32 		div;	

	printf("bus %d , cs %d , max_hz %d , mode %d \n", bus, cs, max_hz, mode);	

	spi = spi_alloc_slave(struct ast_spi_host, bus, cs);
	if (!spi)
		return NULL;


//	spi->slave.bus = bus;

	switch(bus) {
		case 0:
			/* Flash Controller */
			*((volatile ulong*) AST_FMC_BASE) |= 0x800f002a; /* enable Flash Write */
			
			spi->base = AST_FMC_BASE;
//			spi->slave.cs = cs;
			switch (cs) {
				case 0:
					spi->regs = (void *)AST_FMC_BASE + 0x10;
					spi->buff = (void *)AST_FMC_CS0_BASE;
					break;
#ifdef AST_FMC_CS1_BASE
				case 1:
					*((volatile ulong*) 0x1e6e2088) |= 0x1000000; 
					spi->regs = (void *)AST_FMC_BASE + 0x14;
					spi->buff = (void *)AST_FMC_CS1_BASE;
					break;
#endif
#ifdef AST_FMC_CS2_BASE
				case 2:
					*((volatile ulong*) 0x1e6e2088) |= 0x2000000; 
					spi->regs = (void *)AST_FMC_BASE + 0x18;
					spi->buff = (void *)AST_FMC_CS2_BASE;
					break;
#endif
#ifdef AST_FMC_CS3_BASE
				case 3:
					spi->regs = (void *)AST_FMC_BASE + 0x1c;
					spi->buff = (void *)AST_CS3_DEF_BASE;
					break;
#endif
				default:
					return NULL;
			}
		
		break;
		case 1:		
			//SCU70
			*((volatile ulong*) 0x1e6e2070) |= 0x1000; /* SPI strap */
			/* Flash Controller */
			*((volatile ulong*) AST_FMC_SPI0_BASE) |= 0x30000; /* enable Flash Write */			
			spi->base = AST_FMC_SPI0_BASE;
//			spi->slave.cs = cs;
			switch (cs) {
#ifdef AST_SPI0_CS0_BASE				
				case 0:
					spi->regs = (void *)AST_FMC_SPI0_BASE + 0x10;
					spi->buff = (void *)AST_SPI0_CS0_BASE;
					break;
#endif					
#ifdef AST_SPI0_CS1_BASE
				case 1:
					spi->regs = (void *)AST_FMC_SPI0_BASE + 0x14;
					spi->buff = (void *)AST_SPI0_CS1_BASE;
					break;
#endif
				default:
					return NULL;
			}
		
		break;
		case 2:
			*((volatile ulong*) 0x1e6e2088) |= 0x3c000000; /* SPI pin*/
			*((volatile ulong*) AST_FMC_SPI1_BASE) |= 0x30000; /* enable Flash Write */
			spi->base = AST_FMC_SPI1_BASE;
//			spi->slave.cs = cs;
			switch (cs) {
#ifdef AST_SPI1_CS0_BASE				
				case 0:
					spi->regs = (void *)AST_FMC_SPI1_BASE + 0x10;
					spi->buff = (void *)AST_SPI1_CS0_BASE;
					break;
#endif					
#ifdef AST_SPI1_CS1_BASE
				case 1:
					spi->regs = (void *)AST_FMC_SPI1_BASE + 0x14;
					spi->buff = (void *)AST_SPI1_CS1_BASE;
					break;
#endif
				default:
					return NULL;
			}			
			break;		
	}

	/* AST2300 limit Max SPI CLK to 50MHz (Datasheet v1.2) */

	spi_ctrl = ast_spi_read(spi, 0x00);	

	//TODO MASK first
	spi_ctrl &= ~SPI_IO_MODE_MASK;
	
	spi_ctrl &= ~SPI_CLK_DIV_MASK;
//	spi_ctrl |= SPI_CLK_DIV(0x7);

	div = ast_spi_calculate_divisor(max_hz);
	spi_ctrl |= SPI_CLK_DIV(div);
	

//	if (SPI_CPOL & mode) 
//		spi_ctrl |= SPI_CPOL_1;
//	else
//		spi_ctrl &= ~SPI_CPOL_1;

	//ISSUE : ast spi ctrl couldn't use mode 3, so fix mode 0
	spi_ctrl &= ~SPI_CPOL_1;

	ast_spi_write(spi, spi_ctrl, 0x00);
//	printf("write ctrl : %x \n");
	return &spi->slave;

}

void spi_free_slave(struct spi_slave *slave)
{
	free(slave);
}

int spi_claim_bus(struct spi_slave *slave)
{
//	struct ast_spi_host *spi = to_ast_spi(slave);
	SPIDBUG("spi_claim_bus bus %d, cs %d\n",slave->bus, slave->cs);

	//
	return 0;

}

void spi_release_bus(struct spi_slave *slave)
{
	struct ast_spi_host *spi = to_ast_spi(slave);

	SPIDBUG("spi_release_bus >>>>> >>>> \n");

	ast_spi_write(spi, ast_spi_read(spi, 0x00) | SPI_CE_INACTIVE, 0x00);	
	ast_spi_write(spi, ast_spi_read(spi, 0x00) & ~SPI_CMD_USER_MODE, 0x00);	
	//Set to fast read mode ...
	ast_spi_write(spi, SPI_CMD_DATA(0x0b) | SPI_CLK_DIV(7) | SPI_DUMMY_LOW(1) | SPI_CMD_FAST_R_MODE, 0x00);
	//Set to dual fast read mode ...
	//ast_spi_write(spi, SPI_IO_MODE(2) | SPI_CMD_DATA(0x3b) | SPI_CLK_DIV(7) | SPI_DUMMY_LOW(1) | SPI_CMD_FAST_R_MODE, 0x00);
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
		ast_spi_write(spi, ast_spi_read(spi, 0x00) & ~SPI_IO_MODE_MASK , 0x00);		
		ast_spi_write(spi, ast_spi_read(spi, 0x00) | SPI_CMD_USER_MODE , 0x00);
		ast_spi_write(spi, ast_spi_read(spi, 0x00) & ~SPI_CE_INACTIVE, 0x00);
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
		ast_spi_write(spi, ast_spi_read(spi, 0x00) | SPI_CE_INACTIVE, 0x00);
		ast_spi_write(spi, (ast_spi_read(spi, 0x00) & ~SPI_CMD_USER_MODE) , 0x00);
	}

	return 0;

}

#if 0
void spi_set_4byte(struct spi_slave *slave)
{
	*((volatile ulong*) (AST_FMC_BASE + 0x4)) |= (0x1 << slave->cs); /* enable Flash 4byte mode */
}
#endif

void spi_dma(struct spi_slave *slave, void *to, void *from, size_t len)
{
	struct ast_spi_host *spi = to_ast_spi(slave);
	ulong poll_time, data;
	
	if (to == from)
		return;

//	printf("spi dma to %x, start %x, len %x \n",to, from, len);
	/* 4-bytes align */
	if(len % 4)
		len += 4 - (len%4);

        /* force end of burst read */
	ast_spi_write(spi, ast_spi_read(spi, 0x0) | SPI_CE_INACTIVE, 0x0);
	ast_spi_write(spi, ast_spi_read(spi, 0x0) & ~SPI_CE_INACTIVE, 0x0);		

	ast_spi_write(spi, ast_spi_read(spi, AST_SPI_DMA_CTRL) & ~SPI_DMA_EN, AST_SPI_DMA_CTRL);
	ast_spi_write(spi, (ulong *) (from), AST_SPI_DMA_FLASH_BASE);
	ast_spi_write(spi, (ulong *) (to), AST_SPI_DMA_DRAM_BASE);
	ast_spi_write(spi, (ulong *) (len), AST_SPI_DMA_LENGTH);
	ast_spi_write(spi, SPI_DMA_EN, AST_SPI_DMA_CTRL);
	
	/* wait poll */
	do {
		udelay(100);	
		data = ast_spi_read(spi, AST_SPI_DMA_STS);
	} while (!(data & FLASH_STATUS_DMA_READY));

	ast_spi_write(spi, ast_spi_read(spi, AST_SPI_DMA_CTRL) & ~SPI_DMA_EN, AST_SPI_DMA_CTRL);
	/* clear status */
	ast_spi_write(spi, ast_spi_read(spi, AST_SPI_DMA_STS) | FLASH_STATUS_DMA_CLEAR, AST_SPI_DMA_STS);

	return;

}

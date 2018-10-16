/*******************************************************************************
 *
 * Copyright (C) 2004-2014 Emulex. All rights reserved.
 * EMULEX is a trademark of Emulex.
 * www.emulex.com
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published by the
 * Free Software Foundation.
 * This program is distributed in the hope that it will be useful. ALL EXPRESS
 * OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED
 * WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 * NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS
 * ARE HELD TO BE LEGALLY INVALID. See the GNU General Public License for more
 * details, a copy of which can be found in the file COPYING included
 * with this package.
 *
 ********************************************************************************/

#define LPC_SPI_MEM_CFG         0x80
#define LPC_SPI_MEM_BAR_LO      0x81
#define LPC_SPI_MEM_BAR_HI      0x82
#define LPC_SPI_MEM_SIZE        0x83
#define LPC_SPI_SWAP_CTRL       0x84
#define LPC_SPI_STS             0x85


// Interrupt flags
#define HSPI_FIFO_FULL_INT_FLAG         (HSPIWorkBase)
#define HSPI_FIFO_EMPTY_INT_FLAG        (HSPIWorkBase + 0x4)
#define HSPI_CMD_PHASE_DONE_INT_FLAG    (HSPIWorkBase + 0x8)
#define HSPI_NOT_BUSY_INT_FLAG          (HSPIWorkBase + 0xc)

extern void mb_wait_for_spi_rd_ff_notempty(void);
extern void wait_for_spi_rd_ff_notempty (void);
extern void  wait_for_spi_not_busy (void);
extern void mb_wait_for_spi_not_busy (void) ;
extern void wait_for_spi_mem_write (void);
extern void mb_wait_for_spi_mem_write (void);
extern void spi_write_en (void);
extern void mb_spi_write_en (void);
extern void spi_page_prog (tU8 wdata, tU32);
extern void mb_spi_page_prog (tU8 wdata, tU8 addr2,tU8 addr1,tU8 addr0);
extern tU8 spi_mem_read (tU32);
extern void spi_bulk_erase (void);
extern void spi_sec_erase (tU32);
extern void mb_spi_sec_erase (tU8 addr2,tU8 addr1,tU8 addr0);
extern void mb_enable_spi (void);
extern void wait_for_spi_dma_write_en (void) ;
extern void wait_for_spi_wr_ff_notfull (void); 
extern void wait_for_spi_wr_ff_empty (void);
extern void spi_setup_dma_write (tU32 addr);
extern void spi_dma_write (tU8 wdata ) ;
extern void spi_stop_dma_write (void) ;
extern void spi_setup_dma_rd(tU32 addr);
extern void mb_spi_setup_dma_rd(tU32 addr);
extern void spi_stop_dma_rd (void);
extern tU8 spi_dma_rd_word (void);
extern void wait_for_dma_rd_ff_notempty(void);
extern void spi_desel_low(void);
extern void mb_wait_for_spi_dma_write_en (void);
extern void mb_wait_for_spi_wr_ff_notfull (void);
extern void mb_spi_setup_dma_write (tU32 addr);
// extern void mb_spi_stop_dma_write (void);
extern void mb_spi_dma_write (tU8 wdata);
extern void mb_enable_spi_wr (void);
extern void mb_enable_spi (void);
extern void spi_set_clkdiv (tU16 val);
extern void disable_spi_controller (void);
extern void enable_spi_controller (void);
extern void spi_disable_host_write (void);
extern void spi_enable_host_write (void);
extern void spi_disable_host_read (void);
extern void spi_enable_host_read (void);
extern void spi_irq_handler(void);
extern void spi_enable_fast_read (void);
extern void spi_disble_fast_read (void);


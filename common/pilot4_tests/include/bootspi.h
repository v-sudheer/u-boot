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

#define BOOTSPI_ADDR     0x00
#define BOOTSPI_RWDATA   0x30
#define BOOTSPI_CMD0     0x08
#define BOOTSPI_CMD1     0x09
#define BOOTSPI_CMD2     0x0a
#define BOOTSPI_CMD3     0x0b
#define BOOTSPI_CTRL     0x10
#define BOOTSPI_CLKDIV   0x14
#define BOOTSPI_STS      0x18
#define BOOTSPI_MISC     0x1c
#define BOOTSPI_INTEN    0x20

//#define  BOOTSPI_REGS	((volatile PII_BOOTSPI_REGS *)(BOOTSPI_BASE))


extern void unlock_intel_bootspi(void);
extern void bootspi_change_acessmode_tomem(void);
extern void wait_for_bootspi_not_busy (void);
extern void wait_for_bootspi_mem_write (void);
extern void bootspi_write_en (void);
extern void bootspi_page_prog (tU8 wdata, tU32 addr);
extern tU8  bootspi_mem_read (tU32 addr);
extern tU32 bootspi_mem_read_word (tU32 addr);
extern tU8  bootspi_mem_fastread (tU32 addr);
extern tU32 bootspi_mem_fastread_word (tU32 addr);
extern tU8  check_fastread_set (void);
extern void bootspi_bulk_erase (void);
extern void bootspi_sec_erase (tU32 addr);
extern void bootspi_setup_dma_write (tU32 addr);  
extern void  wait_for_bootspi_dma_write_en(void);
extern void bootspi_desel_low (void);
extern tU32 bootspi_read_id (void);
extern void wait_for_wrfifo_not_full(void);
extern void bootspi_set_clkdiv (tU16 val);
extern void wait_for_bootspi_not_busy_after_desel (void);
//SPI_NAND Specific -- Start
extern void bootspi_ncreg_wr (tU8 wdata, tU8 dir_addr);
extern tU8 bootspi_ncreg_rd (tU8 dir_addr);
extern void bootspi_ncALE_wr (tU32 address, tU8 cmd);
extern void bootspi_nc_mem_WE_OE_cmd (tU8 we_cmd_encoding);
extern void bootspi_ncALE_Erase (tU32 address, tU8 cmd);
extern void bootspi_page_prog_Reg1to4bytes (tU32 wdata, tU8 no_of_bytes);
extern tU32 bootspi_mem_read_Reg1to4bytes (tU8 no_of_bytes);
extern void bootspi_ncdesel_low (void);
extern void bootspi_nc_same_freq (tU8 spi_same_freq);
extern void bootspi_nc_SetTimingParams (void);
extern tU8 bootspi_nc_ChkTimingParams(tU8 spi_same_freq);
extern tU8 bootspi_nc_BlockErase (tU8 spi_same_freq, tU32 erase_addr);
extern tU8 bootspi_nc_PageProgram_WrFF (tU8 spi_same_freq, tU32 PgPrgStart_addr, tU32 LENGTH, tPU8 pbyte_SrcWrData);
extern void bootspi_nc_PageRead_DMA (tU8 spi_same_freq, tU32 PgPrgStart_addr, tU32 LENGTH, tVU32 dmasetupaddr, tU32 dmadestaddr);
extern tU8 bootspi_nc_PageProgram_RegMode (tU8 spi_same_freq, tU32 PgPrgStart_addr, tU32 LENGTH, tPU32 pword_SrcWrData);
extern void bootspi_nc_PageRead_RegMode (tU8 spi_same_freq, tU32 PgPrgStart_addr, tU32 LENGTH, tPU32 pword_CmpRdData_RegMode);
extern tVU32 nc_gen_random_bits(tPVU32 lfsr, tVU32 nBits);
extern void set_GPIO4_to_SPI_NAND (void);
extern void set_GPIO4_to_BOOT_SPI_DEVICE (void);
extern tU32 bootspi_nc_ReadID (tU8 spi_same_freq);
extern tU8 bootspi_nc_RdSts (tU8 spi_same_freq, tU8 standalone);
extern void spi_nand_setup(tU8 spi_same_freq) ;
extern tU8 spi_nand_BlockErase(tU32 NF_START_BLOCK_NUM, tU32 NF_NUM_BLOCKS, tU8 spi_same_freq);
extern tU8 spi_nand_PageProgram_WrFF(tU32 NF_START_PAGE_NUM, tU32 NF_START_BLOCK_NUM, tU32 NF_BYTES_PER_PAGE, tU32 NF_PAGES_PER_BLK, tU32 NF_NUM_BLOCKS, tU32 DATA_SIZE, tU8 spi_same_freq, tPU8 pbyte_DMASrcData);
extern tU8 spi_nand_PageRead_DMA(tU32 NF_START_PAGE_NUM, tU32 NF_START_BLOCK_NUM, tU32 NF_BYTES_PER_PAGE, tU32 NF_PAGES_PER_BLK, tU32 NF_NUM_BLOCKS, tU32 DATA_SIZE, tU8 spi_same_freq, tVU32 dmasetupaddr, tPU32 pword_DMACmpData);
extern tU8 spi_nand_PageProgram_RegMode(tU32 NF_START_PAGE_NUM, tU32 NF_START_BLOCK_NUM, tU32 NF_BYTES_PER_PAGE, tU32 NF_PAGES_PER_BLK, tU32 NF_NUM_BLOCKS, tU32 DATA_SIZE, tU8 spi_same_freq, tPU32 pword_REGSrcData);
extern tU8 spi_nand_PageRead_RegMode(tU32 NF_START_PAGE_NUM, tU32 NF_START_BLOCK_NUM, tU32 NF_BYTES_PER_PAGE, tU32 NF_PAGES_PER_BLK, tU32 NF_NUM_BLOCKS, tU32 DATA_SIZE, tU8 spi_same_freq, tPU32 pword_REGCmpData);
extern void spi_nand_exit(void);
extern void timer_setup(tU32 timerbase,tU32 count,tU32 mode);
tU32 get_timer_count (tU32 timerbase);
//SPI_NAND Specific -- End

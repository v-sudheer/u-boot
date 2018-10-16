/*******************************************************************************
 *
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

// NAND Controller defines
#define NC_HWECC                    0x000
//
#define NC_IDDATA                   0x01c
#define NC_DMASRCSTART              0x020
#define NC_DMASRCEND                0x024
//
#define NC_DMACTRL                  0x030
#define NC_LMEMBLKCTRL              0x03c
#define NC_STRTCMD                  0x040
#define NC_LMEMCFG0                 0x044
#define NC_LMEMCFG1                 0x048
#define NC_XDDRADDR                 0x04c
#define NC_CS0LOWADDR               0x050
#define NC_CS0HIGHADDR              0x054

//NAND DMA Control register bit defines
#define DMACTRL_RDINTEN		    (1 << 4)
#define DMACTRL_WRINTEN		    (1 << 6)
#define DMACTRL_ERINTEN		    (1 << 8)
#define DMACTRL_RDINT		    (1 << 5)
#define DMACTRL_WRINT		    (1 << 7)
#define DMACTRL_ERINT		    (1 << 9)


//Interrupt flags
#define NAND_WR_INT_FLAG 	    NANDWorkBase_Start
#define NAND_RD_INT_FLAG 	    (NANDWorkBase_Start+0x4)
#define NAND_ER_INT_FLAG 	    (NANDWorkBase_Start+0x8)

//
//Common defines
#define BITS1                       0x1
#define BITS2                       0x3
#define BITS3                       0x7
#define BITS4                       0xf
#define BITS5                       0x1f
#define BITS6                       0x3f
#define BITS7                       0x7f
#define BITS8                       0xff
#define BITS9                       0x1ff
#define BITS10                      0x3ff
#define BITS11                      0x7ff
#define BITS12                      0xfff
#define BITS13                      0x1fff
#define BITS14                      0x3fff
#define BITS15                      0x7fff
#define BITS16                      0xffff
#define BITS17                      0x1ffff
#define BITS18                      0x3ffff
#define BITS19                      0x7ffff
#define BITS20                      0xfffff
#define BITS21                      0x1fffff
#define BITS22                      0x3fffff
#define BITS23                      0x7fffff
#define BITS24                      0xffffff
#define BITS25                      0x1ffffff
#define BITS26                      0x3ffffff
#define BITS27                      0x7ffffff
#define BITS28                      0xfffffff
#define BITS29                      0x1fffffff
#define BITS30                      0x3fffffff
#define BITS31                      0x7fffffff
#define BITS32                      0xffffffff

//BLKCTL_REG bit definitions
#define NAND_BLKLEN_POS                   0
#define NAND_BLKLEN_BITS                  BITS16
#define NAND_BLKLEN                       (NAND_BLKLEN_BITS                 <<NAND_BLKLEN_POS)
#define NAND_BLKCNT_POS                   16
#define NAND_BLKCNT_BITS                  BITS8
#define NAND_BLKCNT                       (NAND_BLKCNT_BITS                 <<NAND_BLKCNT_POS)

//LMEMCFG_REG bit definitions
#define NAND_LMEM_PAGE_SIZE_DWORDS_POS          0
#define NAND_LMEM_PAGE_SIZE_DWORDS_BITS         BITS16
#define NAND_LMEM_PAGE_SIZE_DWORDS              (NAND_LMEM_PAGE_SIZE_DWORDS_BITS << NAND_LMEM_PAGE_SIZE_DWORDS_POS)
#define NAND_LMEM_MAX_NUM_PAGES_POS             16
#define NAND_LMEM_MAX_NUM_PAGES_BITS            BITS5
#define NAND_LMEM_MAX_NUM_PAGES                 (NAND_LMEM_MAX_NUM_PAGES_BITS << NAND_LMEM_MAX_NUM_PAGES_POS)
#define NAND_LMEM_FREE_PAGE_HIGH_WM_POS         21
#define NAND_LMEM_FREE_PAGE_HIGH_WM_BITS        BITS5
#define NAND_LMEM_FREE_PAGE_HIGH_WM             (NAND_LMEM_FREE_PAGE_HIGH_WM_BITS << NAND_LMEM_FREE_PAGE_HIGH_WM_POS)
#define NAND_LMEM_FREE_PAGE_LOW_WM_POS          26
#define NAND_LMEM_FREE_PAGE_LOW_WM_BITS         BITS5
#define NAND_LMEM_FREE_PAGE_LOW_WM              (NAND_LMEM_FREE_PAGE_LOW_WM_BITS << NAND_LMEM_FREE_PAGE_LOW_WM_POS)

  

//
#define NAND_ONFI_DELAY_LINE_VALUE_POS       8


//Function and Tasks
extern void NCAsynchReset (void);
extern tU32 NCBlockErase (tU32 nandpgstaddr);
extern tU32 nand_asynch_wr_2112_poll (tU32 nandaddr, tU32 ddraddr);
extern tU32 nand_asynch_wr_ECC_2112_poll (tU32 nandaddr, tU32 ddraddr);
extern tU32 nand_asynch_rd_2112_poll (tU32 nandaddr, tU32 ddraddr);
extern tU32 nand_asynch_rd_ECC_2112_poll (tU32 nandaddr, tU32 ddraddr);
extern tU32 nand_ReadID (void);
extern tU32 acc_fullNand_NoECC (tU32 NoOfBlks, tU32 NoOfPgsPerBlk, tPVU32 src_addr, tPVU32 dst_addr);
extern tU32 Erase_fullNand (tU32 NoOfBlks);
extern void NCSetKeepCEasserted (void);
extern tU8  GetFeatures (void);
extern tU8  SetFeatures (void);
extern void SwitchToSyncMode  (tU32);
extern void setNandBLKCTL (tU16, tU16);
extern void setupNandLMEMPageCFG (tU8, tU8, tU8, tU16);
extern tU8 genCLECycle  (tU8);
extern tU8 genALECycle (tU16, tU8, tU16, tU8, tU8);
extern tU8 genDataOECycle  (tU8, tU8);
extern tU32 ONFIBlockErase (tU32);
extern tU8 genCmdALECycle (tU8);
extern tU32  nand_find_lmem_address (void);
extern tU8 nand_poll_lrx_pio_rxdt_rdy  (void);
extern tU32 nand_find_lmem_rdaddress (void);
extern tU8 nand_inc_lmemcpq_ptr (void);
extern tU8 nand_inc_freeptr (void);
extern tU32  pilot3_onfi_read_id (tU32);
extern tU8  SyncGetFeatures (void);
extern void NCDeAssertCE (void);
extern void do_copy (tU32 src_addr, tU32 dst_addr, tU32 bytecnt);

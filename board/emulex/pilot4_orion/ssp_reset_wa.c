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

#include <configs/pilot4_hw.h>
#include <configs/pilot4_map.h>

unsigned int DDRSIZE = 0;
#define DDREND          (0x80000000 + DDRSIZE)
#define CM3_CODE_SIZE  0x10000
#define CM3_CODE_START  (DDREND - CM3_CODE_SIZE)

#define WFE_PATTERN_SIZE    0x100000
#define DESC_SIZE   0x4000

#define WFE_PATTERN_ADDR   (DDREND - (WFE_PATTERN_SIZE + DESC_SIZE + CM3_CODE_SIZE))
#define GDMA_DESC_ADDR      (WFE_PATTERN_ADDR + 0x100000)

extern unsigned long power_on_reset;

const unsigned int cm3_code[] =
{
#include "ssp_wa_code.hex"
};

// Cache cntrl
#define igned intC_CTL_TAG_RG_ADD_MASK    ((0xF)<<16)

#define CC_CTL_TAG_WR_REQ         (1<<0)
#define CC_CTL_TAG_RD_REQ         (1<<1)
#define CC_CTL_INV_REQ            (1<<2)
#define CC_CTL_INV_ALL            (1<<3)
#define CC_CTL_FLUSH_REQ          (1<<4)
#define CC_CTL_TAG_I_D_SEL        (1<<5)
#define D_OVFL_STS                (1<<6)
#define I_OVFL_STS                (1<<7)

#define EN_I_CACHE_CTRL             (1<<12)
#define EN_D_CACHE_CTRL             (1<<13)

typedef struct SSP_REGS
{
    volatile unsigned int ddr_base_offset;
    volatile unsigned int ssp_ddr_limit;
    volatile unsigned short ssp_lm_base_i;
    volatile unsigned short ssp_lm_base_d;
    volatile unsigned int cache_cntrl;
    volatile unsigned int tagram_wr_rd_data0;
    volatile unsigned int tagram_wr_rd_data1;
    volatile unsigned int tagram_wr_rd_data2;
    volatile unsigned int ssp_debug;
    volatile unsigned int data_ddr_base_offset;
    volatile unsigned int data_ssp_ddr_limit;
    volatile unsigned int dummy_pad0;
    volatile unsigned int dummy_pad1;
    volatile unsigned int dummy_pad2;
    volatile unsigned int dummy_pad3;
    volatile unsigned int dummy_pad4;
    volatile unsigned int dummy_pad5;
    volatile unsigned int SYSWCR;
    volatile unsigned int SYSWRERL;
    volatile unsigned int SYSWRERH;
    volatile unsigned int SYSWCFR;
}SSP_REGS;

typedef struct gdma_struc{
	volatile unsigned int_enb: 1;
	volatile unsigned chain: 1;
	volatile unsigned rsvd2 : 3;
	//volatile unsigned rsvd2 : 3; //GDMA
	volatile unsigned csum_rst : 1;
	volatile unsigned csum : 1;
	volatile unsigned rsvd1 : 1;
	volatile unsigned len : 24;
	volatile unsigned long RSVD;
	volatile unsigned int src_addr;
	volatile unsigned int dest_addr;
}gdma_addr;


#define FGBBASE         0x40900000
#define SSP_TOPCTL      (FGBBASE + 0x0)
#define ARM_2_SSP_DB    (FGBBASE + 0x10)

#define FGB_SSP_REGS    ((volatile SSP_REGS *)(FGBBASE+0x1000))


void Set_base_limits(void)
{
    FGB_SSP_REGS->ddr_base_offset      = CM3_CODE_START;
    FGB_SSP_REGS->data_ddr_base_offset = CM3_CODE_START;
    FGB_SSP_REGS->ssp_ddr_limit      =  CM3_CODE_SIZE - 1;
    FGB_SSP_REGS->data_ssp_ddr_limit =  CM3_CODE_SIZE - 1;
}

void Enable_Limit_Chk(void)
{
      FGB_SSP_REGS->cache_cntrl|=(1<<31);
}

void Enable_Ssp_I_CCtl(void)
{
      FGB_SSP_REGS->cache_cntrl|=EN_I_CACHE_CTRL;
}

void Enable_Ssp_D_CCtl(void)
{
      FGB_SSP_REGS->cache_cntrl|=EN_D_CACHE_CTRL;
}

void Icache_Invalidate_All(void)
{
    FGB_SSP_REGS->cache_cntrl|=(CC_CTL_INV_ALL|CC_CTL_TAG_I_D_SEL);
    while( FGB_SSP_REGS->cache_cntrl &(CC_CTL_INV_ALL) );
}

void Dcache_Invalidate_All(void)
{
    FGB_SSP_REGS->cache_cntrl&=(~(CC_CTL_TAG_I_D_SEL));
    FGB_SSP_REGS->cache_cntrl|=(CC_CTL_INV_ALL);
    while( FGB_SSP_REGS->cache_cntrl &(CC_CTL_INV_ALL) );
}

void CM3_outof_reset(void)
{
    *(volatile unsigned int *)(0x40900000+0x74) &= 0xfffffeff;
    *(volatile unsigned int *)(SSP_TOPCTL) = 1;
    Set_base_limits();
    Enable_Limit_Chk();
    *(volatile unsigned int*)(ARM_2_SSP_DB) = GDMA_DESC_ADDR;
    Enable_Ssp_I_CCtl();
    Enable_Ssp_D_CCtl();
    Icache_Invalidate_All();
    Dcache_Invalidate_All();
    *(volatile unsigned int *)(0x40900000+0x74) |= (1<<8);
}

#define GDMA_BASE 0x40a00080
#define GDMA_CTRL (GDMA_BASE + 0x0)
#define GDMA_STATUS (GDMA_BASE + 0x4)
#define GDMA_DTB (GDMA_BASE + 0x8)
#define GDMA_CS (GDMA_BASE + 0xC)

#define DDR_START  0x81000000

void setup_descriptors(void)
{
    volatile gdma_addr* tdma = (volatile gdma_addr*)(GDMA_DESC_ADDR);
    volatile unsigned int next_ddr = 0;

    *(volatile unsigned long*)GDMA_CTRL = 0x1;
    next_ddr = DDR_START;

    // Keep copying chunks of 1MB until DDRSIZE - 2MB reached
    while (next_ddr < (DDREND - 0x200000))
    {
        tdma->int_enb=0;
        tdma->chain = 1;
        tdma->rsvd2 = 0;
        tdma->csum_rst= 0;
        tdma->csum= 0;
        tdma->rsvd1= 0;
        tdma->len= WFE_PATTERN_SIZE - 1;
        tdma->src_addr=WFE_PATTERN_ADDR;
        tdma->dest_addr=next_ddr;
        tdma++;
        next_ddr += WFE_PATTERN_SIZE;
    }

    // For the last chunk copy only "1MB - DESCSIZE - SSPCODE"
    tdma->int_enb=0;
    tdma->chain = 0;
    tdma->rsvd2 = 0;
    tdma->csum_rst= 0;
    tdma->csum= 0;
    tdma->rsvd1= 0;
    tdma->len=(WFE_PATTERN_SIZE -(DESC_SIZE + CM3_CODE_SIZE)) - 1;
    tdma->src_addr=WFE_PATTERN_ADDR;
    tdma->dest_addr=next_ddr;
    printf("desc:0x%x start:0x%x des:0x%x len:0x%x\n", tdma, tdma->src_addr, tdma->dest_addr, tdma->len + 1);
}

void start_cm3(void)
{
    int i = 0;
    unsigned int ddr_size_arr[8] = 
    {
        0x8000000,
        0x10000000,
        0x10000000,
        0x20000000,
        0x20000000,
        0x40000000,
        0x40000000,
        0x80000000
    };

    if (!power_on_reset)
        return;

    DDRSIZE = ddr_size_arr[(*(volatile unsigned int*)(0x40300004) >> 25) & 7];
    // If ECC is enabled then re-compute the ddr size
    if ((*(volatile unsigned int*)(0x40300008) & 0x1) == 0x1)
        DDRSIZE -= (*(volatile unsigned int*)(0x4030005C) - *(volatile unsigned int*)(0x40300058));

    volatile unsigned int* cm3_start = (volatile unsigned int*)(CM3_CODE_START);
    volatile unsigned int* wfe_addr = (volatile unsigned int*)(WFE_PATTERN_ADDR);

    // carve out 1MB of WFE pattern
    for (i = 0; i < WFE_PATTERN_SIZE; i+=4)
        *wfe_addr++ = 0xe320f002;

    // Set up the descriptors
    setup_descriptors();

    // Copy ssp code
    for (i = 0; i < sizeof(cm3_code)/4; i++)
        *cm3_start++ = cm3_code[i];

    // Lock the DDR region
    *(volatile unsigned int*)(0x40300084) = WFE_PATTERN_ADDR + 1;
    *(volatile unsigned int*)(0x40300088) = DDREND + 1;

    CM3_outof_reset();
}

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

typedef struct SSP_REGS 
  {
   tVU32 ddr_base_offset;
   tVU32 ssp_ddr_limit;
   tVU16 ssp_lm_base_i;
   tVU16 ssp_lm_base_d;
   tVU32 cache_cntrl;
   tVU32 tagram_wr_rd_data0;
  tVU32 tagram_wr_rd_data1;
  tVU32 tagram_wr_rd_data2;
}SSP_REGS;
#define  FGB_SSP_REGS           ((volatile SSP_REGS *)(FGBBASE+0x1000))
// Register bit defines

// Cache cntrl
#define CC_CTL_TAG_RG_ADD_MASK    ((0xF)<<16)
  
#define CC_CTL_TAG_WR_REQ         (1<<0)
#define CC_CTL_TAG_RD_REQ         (1<<1)
#define CC_CTL_INV_REQ            (1<<2)
#define CC_CTL_INV_ALL            (1<<3)  
#define CC_CTL_FLUSH_REQ          (1<<4)
#define CC_CTL_TAG_I_D_SEL        (1<<5)
#define D_OVFL_STS                (1<<6)
#define I_OVFL_STS                (1<<7)

#define I_CLINE_SIZE_SHIFT         (8)  
#define I_CLINE_SIZE_MASK          (3<<8)
#define I_CLINE_SIZE_1KB           (0<<8)
#define I_CLINE_SIZE_2KB           (1<<8)  
#define I_CLINE_SIZE_4KB           (2<<8)
#define I_CLINE_SIZE_8KB           (3<<8)

#define D_CLINE_SIZE_SHIFT          (10)    
#define D_CLINE_SIZE_MASK           (3<<10)  
#define D_CLINE_SIZE_1KB            (0<<10)
#define D_CLINE_SIZE_2KB            (1<<10)  
#define D_CLINE_SIZE_4KB            (2<<10)
#define D_CLINE_SIZE_8KB            (3<<10)

#define Enable_DDR_Limit_Overflw    (1<<31)  

#define EN_I_CACHE_CTRL             (1<<12)
#define EN_D_CACHE_CTRL             (1<<13)
#define EN_MASK_IREQ_FLUSH_PIPE     (1<<14)  

//bit 20 to bit 23 are to bring out test bus
  
//#define I_CACHE_COUNTER_EN          (1<<17)
//#define D_CACHE_COUNTER_EN          (1<<18)  
  
//#define en_cache_cntrl               (1<<7) //with old RTL

#define CSize_1K  1024
#define CSize_2K  2048
#define CSize_4K  4096
#define CSize_8K  8192


#define LDMAREQ  1
#define SSPIREQ  2
#define SSPDREQ  3
  
#define SSP_TEST_BUFF_SIZE       0x80000   //512k buffers for all Test Cases on ssp side
#define SSP_STACK_MEM_SIZE       0x2000   //8k for stack
#define SSP_GVARS_SIZE           0x2000  //8k for Work Area-Globalvars,mostly used by ssp code.
#define SSP_AHBMEMTEST_SIZE      0x10000  //64k
//some testcase on ssp side in non cache mode would be accessing ddr directly thru AHB bus
//#if defined (SSP_CACHEEN)
#define SSP_DDR_LIMIT           SYSTEST_SSP_CODE_WORKAREA_END
#define SSP_STACK_HIGH         (SYSTEST_SSP_CODE_WORKAREA_END-4)
#define SSP_STACK_LOW          (SSP_STACK_HIGH-SSP_STACK_MEM_SIZE)  
#define SSP_BUFF_START         (SSP_STACK_LOW - SSP_TEST_BUFF_SIZE) 
 //4k for Work Area-Globalvars,mostly used by ssp code.
#define SSPWorkBase_START      (SSP_BUFF_START -SSP_GVARS_SIZE)

#define SSP_DDR_AHBTEST_START  (SSPWorkBase_START-SSP_AHBMEMTEST_SIZE)


//from SYSTEST_SSP_CODE_WORKAREA to SSPWorkBase_START is for ssp code which is 
//close to 1.5MB  
  
//#else

#define SSP_NON_CACHE_TOP_MEM              0x3000
#define SSP_NON_CACHE_STACK_HIGH           SSP_NON_CACHE_TOP_MEM


//following Mem should not be used ,arm should pass mem thru Mbox for ssp  
//#define SSPWorkBase_START      (SYSTEST_SSP_LMEM_DMEM_END-0x800)
//#define SSPWorkBase_END        (SYSTEST_SSP_LMEM_DMEM_END-0xC00)

//#endif  

//#define LFSR   SSPWorkBase_START ..ssp is using this 

  

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

typedef struct BS_ENGINE_REGS 
	{
	 tVU32  bsctl;
	 tVU32  bssts;
	 tVU32  bsdtb;
         tVU32  bssize;
         tVU32  bsbps0;
         tVU32  bsbps1;
         tVU32  bsbps2;
	 } BS_ENGINE_REGS;

/******** Structure for descriptor in Bit Slicing engine ********/
// width in bytes
// height in lines
// source/dest addr aligned to 16 bytes
typedef struct BS_DESCRIPTOR 
	{
	 volatile unsigned int OffsetSI		;  /* [31: 0] */
	 volatile unsigned int WidthHeight      ;  /* [63:32] */
	 volatile unsigned int source_addr	;  /* [95:64] */
	 volatile unsigned int dest_addr	;  /* [127:96] */
	 } BS_DESCRIPTOR;


//// Register Offsets and Bit definitions ////////
// BS_CTL 
#define bs_en		(1<<0)  
#define bs_irq_en	(1<<1)  
#define bs_fiq_en	(1<<2) 
#define bs_dptr_in_lmem	(1<<3) 
#define bs_bpp_bits     (3<<4)
#define bs_bucketcnt_bits (0xf<<8)


// BS_STS
#define bs_busy	        (1<<0)
#define bsintsts	(1<<1)
	
// Descriptor defines
#define BSE_INTERRUPT  (1<<0) // Generates interrupt for current descriptor
#define BSE_NOTLAST    (1<<1) // used for chaining. means current descriptor is not last

#define  FGB_BS_BASE            (FGBBASE+0x200)
#define  FGB_BSE_REGS            ((volatile BS_ENGINE_REGS *)(FGBBASE+0x200))
// Bank3start = 0x09000000

#define  HWBUCKET_BASE          SYSTEST_BSE_WORKBASE
#define  SWBUCKET_BASE          (SYSTEST_BSE_WORKBASE + 0x00400000)
//#define  BS_MODEL_PARAM         (Bank3start + 0x00ff8000)
#define  BS_DTBASE_LMEM         (FGB_LMEM + 0x00005d00)
#define  BS_DTBASE_DDR          SYSTEST_BSE_DPTR_WORKAREA
#define  BS_MODEL_PARAM         (SYSTEST_BSE_DPTR_WORKAREA + 0x00000fC0)

#define SYSTEST_BSE_LMEM_WORKBASE SYSTEST_BSE_LMEM_START
#define SYSTEST_BSE_LMEM_TMP_WB   (SYSTEST_BSE_LMEM_WORKBASE & 0x0000ffff)
#define SYSTEST_BSE_LMEM_DPTR_WB  (SYSTEST_BSE_LMEM_WORKBASE + 0xd00)



////////////////////////////////////////////////////////
extern void bse_enable (void);
extern void bse_set_irq_handler (tU32 handleraddr ) ;
extern void bse_set_fiq_handler (tU32 handleraddr ) ;
extern void wait_for_bse_not_busy (void) ;
extern void bse_dptr_in_ddr (void);
extern void bse_dptr_in_lmem (void);
extern void bse_program_dtb (tU32);
extern void bse_program_mask0 (tU32);
extern void bse_program_mask1 (tU32);
extern void bse_program_mask2 (tU32);
extern void bse_program_bpp (tU8);
extern void bse_program_bucketcnt (tU8);
extern tU8 comparebucket (tU32, tU32 ,tU32 ,tU32 ,tU32 ); 
extern void createbucket (tU32); 

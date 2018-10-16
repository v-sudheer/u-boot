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
typedef struct DMA_REGS 
	{
	 tVU32  dmactl;
	 tVU32  dmasts;
	 tVU32  dmadtb;
	 tVU32  chksum;
	 }DMA_REGS;

/******** Structure for descriptor in DMA engine ********/
// source/dest addr aligned to bytes
// length in bytes

typedef struct DMA_DESCRIPTOR 
	{
	 volatile unsigned int lengthSI		; /* [31:16] */
	 volatile unsigned int rsvd2		; /* [63:32] */
	 volatile unsigned int source_addr	; /* [31: 0] */
	 volatile unsigned int dest_addr	; /* [63:32] */
	 } DMA_DESCRIPTOR;

// Descriptor defines
#define LDMA_INTERRUPT  (1<<0) // Generates interrupt for current descriptor
#define LDMA_NOTLAST    (1<<1) // used for chaining. means current descriptor is not last
#define LDMA_SRCDDR     (0<<2) // source of data is in DDR
#define LDMA_SRCLMEM    (1<<2) // source of data is in LMEM
#define LDMA_DSTDDR     (0<<3) // Destination for data is in DDR
#define LDMA_DSTLMEM    (1<<3) // Destination for data is in LMEM
#define LDMA_CHKSUMRST  (1<<5) // Resets the checksum in hardware b4 processing current descriptor
#define LDMA_CHKSUMONLY (1<<6) // current descriptor to do checksumonly and no data transfer

//// Register Offsets and Bit definitions ////////
// DMA_CTL 
#define dma_en		(1<<0)  
#define dma_irq_en	(1<<1)  
#define dma_fiq_en	(1<<2) 
#define dma_dptr_in_lmem	(1<<7) 
	
// DMA_STS
#define dma_busy	(1<<0)
#define dmaintsts	(1<<1)
	
//// Global DMA offset ////
#define  FGB_DMA_REGS           ((volatile DMA_REGS *)(FGBBASE+0x300))

///// WorkArea Defines

#define DMA_DDR_WORKBASE            (FGB_DMA_WORKBASE)
#define DMA_DDR_SRC_WB              (DMA_DDR_WORKBASE)
#define DMA_DDR_DEST_WB             (DMA_DDR_WORKBASE + 0xC00)
#define DMA_DDR_DESTORIGDATA_WB     (DMA_DDR_WORKBASE + 0x1800)
#define DMA_DDR_DPTR_WB             (DMA_DDR_WORKBASE + 0x1c00)

// LMEM buffer space
#define DMA_LMEM_SRC_WB            (FGB_LMEM + 0x4000)
#define DMA_LMEM_DEST_WB           (FGB_LMEM + 0x4400)
#define DMA_LMEM_DESTORIGDATA_WB   (FGB_LMEM + 0x4800)
#define DMA_LMEM_DPTR_WB           (FGB_LMEM + 0x4c00)

// System TEST defines
#define SYSTEST_DMA_DDR_WORKBASE   SYSTEST_LDMA_WORKBASE
#define SYSTEST_DMA_DDR_SRC_WB     SYSTEST_DMA_DDR_WORKBASE
#define SYSTEST_DMA_DDR_DEST_WB    (SYSTEST_DMA_DDR_WORKBASE + 0x00040000)

#define SYSTEST_DMA_LMEM_SRC_WB    (SYSTEST_LDMA_LMEM_WORKBASE)
#define SYSTEST_DMA_LMEM_DEST_WB   (SYSTEST_LDMA_LMEM_WORKBASE + 0x380)
#define SYSTEST_DMA_LMEM_DPTR_WB   (SYSTEST_LDMA_LMEM_WORKBASE + 0x700)

extern void ldma_enable (void);
extern void ldma_set_irq_handler (tU32 handleraddr ) ;
extern void ldma_set_fiq_handler (tU32 handleraddr ) ;
extern void wait_for_ldma_not_busy (void) ;
extern void ldma_dptr_in_ddr (void);
extern void ldma_dptr_in_lmem (void);
extern void ldma_program_dtb (tU32);


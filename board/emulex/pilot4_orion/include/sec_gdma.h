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

/******************************* DMA CONTROL Registers ***************/
typedef struct GDMA_CTL 
	{
	 volatile unsigned int gdma_en		:1;  /* [0 : 0] */
	 volatile unsigned int gdma_irq_en	:1;  /* [1 : 1] */
	 volatile unsigned int gdma_fiq_en	:1;  /* [2 : 2] */
	 volatile unsigned int rsvd2		:29; /* [31: 3] */
	 } GDMA_CTL;

typedef struct GDMA_STS 
	{
	 volatile unsigned int gdma_busy	:1;  /* [0 : 0] */
	 volatile unsigned int gdma_int_sts	:1;  /* [1 : 1] */
	 volatile unsigned int rsvd1	        :30; /* [31: 2] */
	 } GDMA_STS;

typedef struct GDMA_DTB 
	{
	 volatile unsigned int ro		:2;  /* [1 : 0] */
	 volatile unsigned int desc_base	:30; /* [31: 2] */
	 } GDMA_DTB;


typedef union U_GDMA_CTL { GDMA_CTL s; unsigned int val; } u_gdmactl ;
typedef union U_GDMA_STS { GDMA_STS s; unsigned int val; } u_gdmasts ;
typedef union U_GDMA_DTB { GDMA_DTB s; unsigned int val; } u_gdmadtb ;

typedef struct GDMA_ENGINE_REGS 
	{
	 volatile u_gdmactl     gdmactl ;
	 volatile u_gdmasts     gdmasts ;
	 volatile u_gdmadtb     gdmadtb ;
	 volatile unsigned int  chksum ;
	 } GDMA_ENGINE_REGS;

typedef struct GDMA_REGS 
	{
	 tVU32  gdmactl;
	 tVU32  gdmasts;
	 tVU32  gdmadtb;
	 tVU32  chksum;
	 } GDMA_REGS;



/******** Structure for descriptor in tile fetch engine ********/
// source/dest addr aligned to bytes
// length in bytes

typedef struct GDMA_DESCRIPTOR 
	{
	 volatile unsigned int lengthSI		; /* [31:16] */
	 volatile unsigned int rsvd2		; /* [63:32] */
	 volatile unsigned int source_addr	; /* [31: 0] */
	 volatile unsigned int dest_addr	; /* [63:32] */
	 } GDMA_DESCRIPTOR;


typedef struct GDMA_DESCRIPTOR1 
	{
	 volatile unsigned int I		:1;  /* [0 : 0] */
	 volatile unsigned int S		:1;  /* [1 : 1] */
	 volatile unsigned int rsvd1		:3;  /* [4:  2] */
         volatile unsigned int chksumrst	:1;  /* [5 : 5] */
         volatile unsigned int chksumonly	:1;  /* [6 : 6] */
	 volatile unsigned int rsvd2		:1;  /* [7 : 7] */
	 volatile unsigned int length		:24; /* [31: 8] */
	 volatile unsigned int rsvd3		:32; /* [63:32] */
	 volatile unsigned int source_addr	:32; /* [31: 0] */
	 volatile unsigned int dest_addr	:32; /* [63:32] */
	 } GDMA_DESCRIPTOR1;


//// Register Offsets and Bit definitions ////////
// DMA_CTL 
#define gdma_en		(1<<0)   
#define gdma_irq_en	(1<<1)  
#define gdma_fiq_en	(1<<2) 
	
// DMA_STS
#define gdma_busy	(1<<0)
#define gdmaintsts	(1<<1)
	

#define  SEC_GDMA_REGS         ((volatile GDMA_REGS *)(FGB_SECBASE+0x080))

///// WorkArea Defines

#define GDMA_DDR_WORKBASE               (SYSTEST_GDMA_WORKBASE)
#define GDMA_DDR_SRC_WB                 (GDMA_DDR_WORKBASE)
#define GDMA_DDR_DEST_WB                (GDMA_DDR_WORKBASE + 0x0C00)
#define GDMA_DDR_DESTORIGDATA_WB        (GDMA_DDR_WORKBASE + 0x1800)
#define GDMA_DDR_DPTR_WB                (GDMA_DDR_WORKBASE + 0x1c00)

#define GDMA_LMEM_SRC_WB                (FGB_LMEM + 0x5000)
#define GDMA_LMEM_DEST_WB               (FGB_LMEM + 0x5400)
#define GDMA_LMEM_DESTORIGDATA_WB       (FGB_LMEM + 0x5800)
#define GDMA_LMEM_DPTR_WB               (FGB_LMEM + 0x5c00)

// System TEST defines
#define SYSTEST_GDMA_DDR_WORKBASE       (SYSTEST_GDMA_WORKBASE)
#define SYSTEST_GDMA_DDR_SRC_WB         (SYSTEST_GDMA_DDR_WORKBASE)
#define SYSTEST_GDMA_DDR_DEST_WB        (SYSTEST_GDMA_DDR_WORKBASE + 0x00040000)
#define SYSTEST_GDMA_LMEM_SRC_WB        (SYSTEST_GDMA_LMEM_WORKBASE)
#define SYSTEST_GDMA_LMEM_DEST_WB       (SYSTEST_GDMA_LMEM_WORKBASE + 0x380)
#define SYSTEST_GDMA_LMEM_DPTR_WB       (SYSTEST_GDMA_LMEM_WORKBASE + 0x700)

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

//TFE SPECIFIC DEFINES
#define MODE0_NORMAL		(0<<13)
#define MODE1_4BITPLNR		(1<<13)
#define MODE2_4BITPKD		(2<<13)
#define MODE3_ATTRASCII		(3<<13)
#define MODE4_ASCIIONLY		(4<<13)
#define MODE5_FONTFMODE		(5<<13)

  
#define TFE_MODE0_NORMAL		(0<<13)
#define TFE_MODE1_4BITPLNR		(1<<13)
#define TFE_MODE2_4BITPKD		(2<<13)
#define TFE_MODE3_ATTRASCII		(3<<13)
#define TFE_MODE4_ASCIIONLY		(4<<13)
#define TFE_MODE5_FONTFMODE		(5<<13)
#define TFE_MODE6		        (6<<13)


  
#define TFE_I			(1<<0)
#define TFE_LAST		(0<<1)
#define TFE_NOTLAST		(1<<1)
#define TFE_SRC_DDR             (0<<2)
#define TFE_SRC_LMEM            (1<<2)
#define TFE_DST_DDR             (0<<3)
#define TFE_DST_LMEM            (1<<3)
#define TFE_RLE_EN              (1<<4)
#define FE_CHKSUM_EN            (1<<5)
#define TFE_CHKSUM_RST           (1<<5)
#define TFE_RLE_NO_OVRFLW          (1<<7)  
  
#define TFE_ENABLE              (1<<0)
#define TFE_IRQ_ENABLE          (1<<1)
#define TFE_FIQ_ENABLE          (1<<2)

#define TFE_DescInLMEM           (1<<7)  
#define TFE_INTR_STS            (1<<1)  
#define TFE_8_16_JUMP           (1<<3)
#define TFE_32_JUMP           (1<<4)
#define TFE_BUSY                (1<<0)


#define mode6_jump_one_byte    (0<<8)
#define mode6_jump_two_bytes   (1<<8)
#define mode6_jump_three_bytes (2<<8)
#define mode6_jump_four_bytes  (3<<8)

#define mode6_start_addr_pos_zero  (0<<10)
#define mode6_start_addr_pos_one   (1<<10)
#define mode6_start_addr_pos_two   (2<<10)
#define mode6_start_addr_pos_three (3<<10)

#define RLE_BPP                     (1<<12)  
  
  
#define TFETB1BUFF		TFEXmitBuffer
#define TFETB2BUFF		TFEXmitBuffer+0x800
#define TFETB3BUFF		TFEXmitBuffer+0x1000
#define TFETB4BUFF		TFEXmitBuffer+0x1800
#define TFETB5BUFF		TFEXmitBuffer+0x2000
#define TFETB6BUFF		TFEXmitBuffer+0x2800
#define TFETB7BUFF		TFEXmitBuffer+0x3000
#define TFETB8BUFF		TFEXmitBuffer+0x3800

#define TFERB1BUFF		TFERecvBuffer
#define TFERB2BUFF		TFERecvBuffer+0x800
#define TFERB3BUFF		TFERecvBuffer+0x1000
#define TFERB4BUFF		TFERecvBuffer+0x1800
#define TFERB5BUFF		TFERecvBuffer+0x2000
#define TFERB6BUFF		TFERecvBuffer+0x2800
#define TFERB7BUFF		TFERecvBuffer+0x3000
#define TFERB8BUFF		TFERecvBuffer+0x3800


#define TFEWorkbase1             TFEWorkBase
#define TFEWorkbase2             TFEWorkBase+0x10  
#define TFEWorkbase3             TFEWorkBase+0x20  
#define TFEWorkbase4             TFEWorkBase+0x30    

typedef struct TF_CTL 
	{
	 volatile unsigned int tfe_en		:1;  /* [0 : 0] */
	 volatile unsigned int tfe_irq_en	:1;  /* [1 : 1] */
	 volatile unsigned int tfe_fiq_en	:1;  /* [2 : 2] */
	 volatile unsigned int dptr_src 	:1;  /* [3 : 3] */
	 volatile unsigned int text_align 	:1;  /* [4 : 4] */
	 volatile unsigned int rsvd1		:27; /* [31: 5] */
	 } TF_CTL;

typedef struct TF_STS 
	{
	 volatile unsigned int tfe_busy	:1;  /* [0 : 0] */
	 volatile unsigned int tfe_int_sts	:1;  /* [1 : 1] */
	 volatile unsigned int rsvd1	        :30; /* [31: 2] */
	 } TF_STS;

typedef struct TF_DTB 
	{
	 volatile unsigned int ro		:2;  /* [1 : 0] */
	 volatile unsigned int desc_base	:30; /* [31: 2] */
	 } TF_DTB;


typedef union U_TF_CTL { TF_CTL s; unsigned int val; } u_tfctl ;
typedef union U_TF_STS { TF_STS s; unsigned int val; } u_tfsts ;
typedef union U_TF_DTB { TF_DTB s; unsigned int val; } u_tfdtb ;

typedef struct TILE_FETCH_REGS 
	{
	 volatile u_tfctl      tfctl ;
	 volatile u_tfsts      tfsts ;
	 volatile u_tfdtb      tfdtb ;
	 } TILE_FETCH_REGS;

typedef struct TFE_REGS 
	{
	 tVU32  Tfe_Ctrl;
	 tVU32  Tfe_Sts;
	 tVU32  Tfe_dptrbase;
	 tVU32  Tfe_chksum;
	 tVU32  Tfe_rlecount;
	 tVU32  Tfe_rlelimit;
	 }TFE_REGS;

/******** Structure for descriptor in tile fetch engine ********/
// width in bytes
// height in lines
// source/dest addr aligned to 16 bytes

typedef struct TFE_DESCRIPTOR 
	{
	 volatile unsigned int I		:1;  /* [0 : 0] */
	 volatile unsigned int S		:1;  /* [1 : 1] */
	 volatile unsigned int rsvd0		:1;  /* [2 : 2] */
	 volatile unsigned int destloc		:1;  /* [3 : 3] */
	 volatile unsigned int rle_en		:1;  /* [4 : 4] */
	 volatile unsigned int chksum_en	:1;  /* [5 : 5] */
	 volatile unsigned int rsvd1		:7;  /* [12: 6] */
	 volatile unsigned int mode		:3;  /* [15:13] */
	 volatile unsigned int offset		:12; /* [28:16] */
	 volatile unsigned int rsvd2		:4;  /* [31:29] */
	 volatile unsigned int width		:16; /* [47:32] */
	 volatile unsigned int height		:14; /* [61:48] */
	 volatile unsigned int rsvd3		:2;  /* [63:62] */
	 volatile unsigned int rsvd4		:4;  /* [3 : 0] */
	 volatile unsigned int source_addr	:28; /* [31: 4] */
	 volatile unsigned int rsvd5		:4;  /* [35:32] */
	 volatile unsigned int dest_addr	:12; /* [47:36] */
	 volatile unsigned int rsvd6		:16; /* [63:48] */
	 } TFE_DESCRIPTOR;


typedef struct Tfedptr{
          tVU32 Control;
	  tVU32 Width_Heigth_Reg;
	  tVU32 Srcaddrs;
	  tVU32 Dstaddrs;
	   }Tfedptr;



typedef struct{
  tU32   tile_height;
  tU32   tile_width;
  tU32   frame_width;
  tU32   checksumON;
  tU32   noof_chained_dptrs;
  tPVU32  srcaddrs[8];
  tPVU32  dstaddrs;
  tPVU32  dptraddrs;
  tPVU32  tile_buff[8];
  tPVU32  swrle_buff;
  tU32    Rcode;
  tU32    Tcode;
  tU32    No_overflow;
  tU32    t_align;
  tU32    s_a_pos;
  tPVU32  expected_buff[8];	       
	       
 	       
 }TFE_TEST_PARAMS;
   
////// assigning register space to different structures ///////////
#define  FGB_TF_REGS            ((volatile TILE_FETCH_REGS *)(FGBBASE+0x100))
#define  TFE_BLOCK              ((volatile TFE_REGS *)(FGBBASE+0x100))

#define  TFE_DPTR1MEM    (SYSTEST_TFE_LMEM_START)
#define  TFE_DPTR2MEM    (SYSTEST_TFE_LMEM_START+0x10) 
#define  TFE_DPTR3MEM    (SYSTEST_TFE_LMEM_START+0x20) 

#define  LMEM_TFEBUFF    (SYSTEST_TFE_LMEM_START+0x100)


  
  
extern int  Compare4bitpackedmode(int srcaddrs,int dstaddrs,int count,int mode16,int width, int framewidth);
extern void Convert_Plannar_Packed(int Srcaddress,int Dstaddress,int size,int mode16,int width,int framewidth);
extern int  Compare4bitpackedmode(int srcaddrs,int dstaddrs,int count,int jump8_16,int width,int framewidth);
extern void ConvertAttr2Asciidata(int srcaddrs,int dstaddrs,int count,int jump8_16,int width,int framewidth);
extern void Convert2Asciidata(int srcaddrs,int dstaddrs,int count,int jump8_16,int width,int framewidth);
extern void Convert2fontfetchdata(int srcaddrs,int dstaddrs,int count,int jump8_16,int width,int framewidth);

extern tU32 test_tfe(TFE_TEST_PARAMS *tfe_setup);

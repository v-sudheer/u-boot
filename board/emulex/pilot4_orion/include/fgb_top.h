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


//**TOTAL LMEM IS 32KB***//
//0x04000000 to 0x04001fff      ..first 8Kb is for SSP IRAM
//0x04002000 to 0x04005fff     ..next 16kb is for SSP Dram or for other dma activities
//0x04006000 to 0x04007fff     ..next 8kb is shared between bomber and modexp blocks


#define ORPCODELOAD      0xC0DEC0DE


/////// Interrupt Flag Variables
#define RCEINTRPTFLAG 	 	(FGEEWorkBase+0x608)  
#define TSEINTRPT0FLAG 	 	(FGEEWorkBase+0x60C)
#define TFEINTRPTFLAG 	 	(FGEEWorkBase+0x610)
#define SMBINTRPTFLAG 	 	(FGEEWorkBase+0x614)
#define TSEINTRSTS0		(FGEEWorkBase+0x620)
#define DMAENGINTRPTFLAG 	(FGEEWorkBase+0x624)
//#define LFSR		 	(FGEEWorkBase+0x628)
#define BSINTRPTFLAG 	        (FGEEWorkBase+0x62C)
#define TSEASCIINTRPTFLAG 	(FGEEWorkBase+0x630)
#define TSEATTRINTRPTFLAG 	(FGEEWorkBase+0x634)
#define TSEFONTINTRPTFLAG 	(FGEEWorkBase+0x638)
#define TSEINTRPT1FLAG 	 	(FGEEWorkBase+0x63C)
#define S2AINTRPTFLAG           (FGEEWorkBase+0x640)
#define S2ADB0                  (FGEEWorkBase+0x644)
#define S2ADB1                  (FGEEWorkBase+0x648)  
#define GRCENGINTRPTFLAG        (FGEEWorkBase+0x64c)

#define ORPBSINTRPTFLAG 	(FGEEWorkBase+0x650)
#define ORPDMAENGINTRPTFLAG 	(FGEEWorkBase+0x654)
#define ORPTFEINTRPTFLAG 	(FGEEWorkBase+0x658)



////// Offsets for top registers
#define TOPCTL          0x0
#define TOPINTSTS       0x4


//bit definations for FGB_TOP_CTRL regs
#define RESET_SSP     1<<0
//upper 8Kb of LMEM is shared between modexp and bomber and each of them can decide to keep 4k or 8k
#define BOMBER_SEL4K_MEM                5<<2    // 0x7000 - 0x8000 (BOMBER), MODEXP CANNOT WORK
#define BOMBER_SEL8K_MEM                7<<2    // 0x6000 - 0x8000 (BOMBER), MODEXP CANNOT WORK
#define MODEXP_SEL4K_MEM                9<<2    // 0x7000 - 0x8000 (MODEXP), BOTH CANNOT WORK
#define MODEXP_SEL8K_MEM                0xB<<2  // 0x6000 - 0x8000 (MODEXP), Bomber cannot exist
#define MODEXP_SEL8K_BOMBER_SEL4K       0x13<<2  // 0x5000 - 0x7000 (MODEXP) , 0x7000 - 0x8000 ( BOMBER)
  

/******************************* TOP Control Registers ***************/
typedef struct TOP_CTL 
  {
   volatile unsigned int or1k_reset :1; 
   volatile unsigned int so	    :1; 
   volatile unsigned int bommem_en  :1; 
   volatile unsigned int rsvd1	    :29; 
   }TOP_CTL;


typedef struct TOP_INTSTS 
  {
   volatile unsigned int ts_int0_sts           	:1;  /* [0 : 0] */
   volatile unsigned int ts_int1_sts   		:1;  /* [1 : 1] */
   volatile unsigned int ts_updtasci_sts	:1;  /* [2 : 2] */
   volatile unsigned int ts_updtattr_sts	:1;  /* [3 : 3] */
   volatile unsigned int ts_updtfont_sts	:1;  /* [4 : 4] */
   volatile unsigned int tf_sts			:1;  /* [5 : 5] */
   volatile unsigned int dma_int_sts		:1;  /* [6 : 6] */
   volatile unsigned int bs_int_sts		:1;  /* [7 : 7] */
   volatile unsigned int rsvd1   		:8;  /* [13:13] */
   volatile unsigned int grc_palette_sts	:1;  /* [15:15] */
   volatile unsigned int grc_attr_sts		:1;  /* [16:16] */
   volatile unsigned int grc_seq_sts		:1;  /* [17:17] */
   volatile unsigned int grc_gctl_sts		:1;  /* [18:18] */
   volatile unsigned int grc_crtc_sts		:1;  /* [19:19] */
   volatile unsigned int grc_crtcext_sts	:1;  /* [20:20] */
   volatile unsigned int grc_pltram_sts		:1;  /* [21:21] */
   volatile unsigned int grc_xcura_sts		:1;  /* [22:22] */
   volatile unsigned int grc_xcurb_sts		:1;  /* [23:23] */
   volatile unsigned int rsvd2			:7; /* [31:24] */
   }TOP_INTSTS;

typedef union U_TOP_CTL
  { 
    TOP_CTL s; 
    tVU32  val; 
    }u_topctl;

typedef union U_TOP_INTSTS
  { 
    TOP_INTSTS s; 
    tVU32  val; 
    }u_topintsts;


typedef struct TOP_CTRL_REGS 
	{
	 volatile u_topctl topctl;
	 volatile  u_topintsts topintsts;
	 tVU16  arm2ssp_inten;
	 tVU16  arm2ssp_intsts;
	 tVU16  ssp2arm_inten;
	 tVU16  ssp2arm_intsts;
	 tVU32  arm2ssp_db0;
	 tVU32  arm2ssp_db1;
	 tVU32  ssp2arm_db0;
	 tVU32  ssp2arm_db1;
	 }TOP_CTRL_REGS;

////// assigning register space to different structures ///////////
#define  FGB_TOP_REGS	        ((volatile TOP_CTRL_REGS *)(FGBBASE))
#define  FGB_TOP_BASE	        FGBBASE

/// RAM priority register defines
#define LMEM_RAM0_PRTY          (FGBBASE + 0x40)
#define LMEM_RAM1_PRTY          (FGBBASE + 0x44)
#define LMEM_RAM2_PRTY          (FGBBASE + 0x48)
#define LMEM_RAM3_PRTY          (FGBBASE + 0x4C)
#define LMEM_RAM4_PRTY          (FGBBASE + 0x50)
#define LMEM_RAM5_PRTY          (FGBBASE + 0x54)
#define LMEM_RAM6_PRTY          (FGBBASE + 0x58)
#define LMEM_RAM7_PRTY          (FGBBASE + 0x5C)
#define CMDA_ENG_FIX_REG        (FGBBASE + 0x74)

#define LMEM_RAM03_ENABLE      (FGBBASE + 0x60)
#define LMEM_RAM47_ENABLE       (FGBBASE + 0x64)
  
#define LS0_0                   (0<<0)
#define LS1_0                   (0<<3)
#define LS2_0                   (0<<6)
#define LS3_0                   (0<<9)
#define LS4_0                   (0<<12)
#define LS5_0                   (0<<15)
#define LS6_0                   (0<<18)
#define LS7_0                   (0<<21)

#define LS0_1                   (1<<0)
#define LS1_1                   (1<<3)
#define LS2_1                   (1<<6)
#define LS3_1                   (1<<9)
#define LS4_1                   (1<<12)
#define LS5_1                   (1<<15)
#define LS6_1                   (1<<18)
#define LS7_1                   (1<<21)

#define LS0_2                   (2<<0)
#define LS1_2                   (2<<3)
#define LS2_2                   (2<<6)
#define LS3_2                   (2<<9)
#define LS4_2                   (2<<12)
#define LS5_2                   (2<<15)
#define LS6_2                   (2<<18)
#define LS7_2                   (2<<21)

#define LS0_3                   (3<<0)
#define LS1_3                   (3<<3)
#define LS2_3                   (3<<6)
#define LS3_3                   (3<<9)
#define LS4_3                   (3<<12)
#define LS5_3                   (3<<15)
#define LS6_3                   (3<<18)
#define LS7_3                   (3<<21)

#define LS0_4                   (4<<0)
#define LS1_4                   (4<<3)
#define LS2_4                   (4<<6)
#define LS3_4                   (4<<9)
#define LS4_4                   (4<<12)
#define LS5_4                   (4<<15)
#define LS6_4                   (4<<18)
#define LS7_4                   (4<<21)

#define LS0_5                   (5<<0)
#define LS1_5                   (5<<3)
#define LS2_5                   (5<<6)
#define LS3_5                   (5<<9)
#define LS4_5                   (5<<12)
#define LS5_5                   (5<<15)
#define LS6_5                   (5<<18)
#define LS7_5                   (5<<21)

#define LS0_6                   (6<<0)
#define LS1_6                   (6<<3)
#define LS2_6                   (6<<6)
#define LS3_6                   (6<<9)
#define LS4_6                   (6<<12)
#define LS5_6                   (6<<15)
#define LS6_6                   (6<<18)
#define LS7_6                   (6<<21)

#define LS0_7                   (7<<0)
#define LS1_7                   (7<<3)
#define LS2_7                   (7<<6)
#define LS3_7                   (7<<9)
#define LS4_7                   (7<<12)
#define LS5_7                   (7<<15)
#define LS6_7                   (7<<18)
#define LS7_7                   (7<<21)


//following defines should be moved from here as we have allocated mem inside buffer.h  
#define FGB_LMEM             LMEMSTART
  
#define  ORPWORKAREA     (SSP_MEMSTART+0x2400)  //move this defines to ssp.h  
#define  ORPMAILBOXST    (SSPWorkBase_START)    //
#define  LMEMWORKAREA    (ORPWORKAREA+0x200)    
  
extern  void  FGEEHandler(void);
extern tU32 SwapBytes(tU32 a);
extern tU16 Calcchksum(tU32, tU32);
extern tU16 Calcchksum_wseed(tU32, tU32, tU16);
extern tU32 Scrunch16_P2( tPVU32 theBuffer, tU32 cSourcePixels,tPVU32 dstmem,tU8,tU8 );
extern tU32 Scrunch16( tPVU32 theBuffer, tU32 cSourcePixels,tPVU32 dstmem);
extern void S2A_Handler(void);
extern void systest_s2a_handler(void);

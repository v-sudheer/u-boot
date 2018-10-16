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

#define ORPCODELOAD      0xC0DEC0DE

#define SMBBASE			FGEEBASE+0x400
#define SMBCTL			SMBBASE
#define SMBSTS			SMBBASE+0x4
#define SMBDTB			SMBBASE+0x8
#define SMBTIMEOUTCNT		SMBBASE+0xC
#define SMBBYTECNT		SMBBASE+0x10

//SMB Control and interrupt enable  reg bit definations
#define SMBEN			1
#define SMBINT			1<<1
#define SMBALRT			1<<2
#define SMBTIMEOUT		1<<3
#define SMBWAITBC		1<<4	  	//wait for byte count
#define SMBERR			1<<5		//when we recive NAK condition we get error

//SMB status reg bit definations
#define SMBBUSYSTS			1
#define SMBDONESTS			1<<1
#define SMBALRTSTS			1<<2
#define SMBTIMEOUTSTS			1<<3
#define SMBWAITBCSTS			1<<4	  	//wait for byte count
#define SMBERRSTS			1<<5		//when we recive NAK condition we get error

//descriptor zero bit definations
#define SMBPEC			1		//setting this bit will read PEC
#define SMBR			1<<1            //seting this bit will tell its an rea operation
#define SMBBLK			1<<2	        //this bit tell this is block read(to decide byte count) 
#define SMBH			1<<3            //setting this bit will make hardware to wait for s/w o write byte count
#define SMBNB			1<<4              //byte count size 0=8bits,1=16bits
#define SMBBO			1<<5              //byte order
#define SMBRS			1<<6		  //repeat start  ,for block read


#define SMBWRITEBYTE    0x00
#define SMBWRITEWORD    0x01
#define SMBWRITE        0x02
#define SMBREAD         0x03
#define SMBREADBYTE     0x04
#define SMBREADWORD     0x05
#define SMBREADSTART    0x03
#define SMBWRITESTART  0x06
#define SMBWRITEMIDDLE 0x07
#define SMBWRITEEND    0x08
#define SMBREADMIDDLE  0x09
#define SMBREADRETRY   0x0a
#define SMBREADEND     0x09
#define SMBMAXLENGTH   32
#define SMBMAXBYTESTOSEND  10

//TFE SPECIFIC DEFINES
#define MODE0_NORMAL		0<<13
#define MODE1_4BITPLNR		1<<13
#define MODE2_4BITPKD		2<<13
#define MODE3_ATTRASCII		3<<13
#define MODE4_ASCIIONLY		4<<13
#define MODE5_FONTFMODE		5<<13

#define TFE_MODE0_NORMAL		0<<13
#define TFE_MODE1_4BITPLNR		1<<13
#define TFE_MODE2_4BITPKD		2<<13
#define TFE_MODE3_ATTRASCII		3<<13
#define TFE_MODE4_ASCIIONLY		4<<13
#define TFE_MODE5_FONTFMODE		5<<13
#define TFE_I			1<<0
#define TFE_S			1<<1
#define TFE_ENABLE              1<<0
#define TFE_IRQ_ENABLE          1<<1
#define TFE_FIQ_ENABLE          1<<2
#define TFE_INTR_STS            1<<1  
#define TFE_8_16_JUMP           1<<3 
  
//ENC_DEC_SPECIFIC DEFINES
#define ENCDEC_ENABLE		1<<0
#define ENCDEC_IRQEN            1<<1
#define ENCDEC_FIQEN            1<<2

#define EE_I                    (1<<0)
#define EE_S                    (1<<1)
#define EE_DECRIPT_ENABLE       (1<<2)
#define EE_ENCRIPT_ENABLE       (0<<2)  
#define EE_3DESMODE             (1<<3)
#define EE_1DESMODE             (0<<3)  
#define EE_AESCTR               (1<<3)
#define EE_AESCBC               (0<<3)
#define EE_DESENABLE            (0<<4)
#define EE_AESENABLE            (1<<4)
#define EE_IVSEL                (1<<5)        //when dptrs are chained for 1block then we need to set this bit
#define EE_SWAPORDER            (1<<6)  
#define KEYINDEX(index)         (index<<16)   

//SHA1 Bit defines
#define SHA_I                    (1<<0)
#define SHA_S                    (1<<1)
#define SHA_FIRST_IV             (1<<2)
#define SHA_NOTFIRST_IV          (0<<2)  
#define SHA_SWAPORDER            (1<<6)  
#define SHA_INTRSTS              (1<<1) 


#define ENCRPTFLAG   		FGEEWorkBase+0x600
#define DECRPTFLAG   		FGEEWorkBase+0x604
#define RCEINTRPTFLAG 	 	FGEEWorkBase+0x608
#define TSEINTRPTFLAG 	 	FGEEWorkBase+0x60C
#define TFEINTRPTFLAG 	 	FGEEWorkBase+0x610
#define SMBINTRPTFLAG 	 	FGEEWorkBase+0x614
#define TSEINTRSTS0		FGEEWorkBase+0x620
#define DMAENGINTRPTFLAG 	FGEEWorkBase+0x624
#define LFSR		 	FGEEWorkBase+0x628
#define AUTH_INTRFLAG           FGEEWorkBase+0x62C
#define GRCENGINTRPTFLAG 	FGEEWorkBase+0x630



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


#define SMBED1		SMBWorkBase
#define SMBED2		SMBWorkBase+0x10
#define SMBED3		SMBWorkBase+0x20
#define SMBED4		SMBWorkBase+0x30
#define SMBED5		SMBWorkBase+0x40
#define SMBED6		SMBWorkBase+0x50

#define SMBTB1BUFF		SMBXmitBuffer
#define SMBTB2BUFF		SMBXmitBuffer+0x200
#define SMBTB3BUFF		SMBXmitBuffer+0x400
#define SMBTB4BUFF		SMBXmitBuffer+0x600
  
#define SMBRB1BUFF		SMBRecvBuffer
#define SMBRB2BUFF		SMBRecvBuffer+0x200
#define SMBRB3BUFF		SMBRecvBuffer+0x400
#define SMBRB4BUFF		SMBRecvBuffer+0x600


 
 
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
   volatile unsigned int ee_sts			:1;  /* [0 : 0] */
   volatile unsigned int rc_oflow_int_sts	:1;  /* [1 : 1] */
   volatile unsigned int rc_thld_int_sts	:1;  /* [2 : 2] */
   volatile unsigned int rc_crtc_sts		:1;  /* [3 : 3] */
   volatile unsigned int rc_vsync_sts		:1;  /* [4 : 4] */
   volatile unsigned int rc_blit_begin_sts	:1;  /* [5 : 5] */
   volatile unsigned int rc_blit_end_sts	:1;  /* [6 : 6] */
   volatile unsigned int rc_blit_cnt_sts	:1;  /* [7 : 7] */
   volatile unsigned int ts_sts0		:1;  /* [8 : 0] */
   volatile unsigned int ts_sts1		:1;  /* [9 : 0] */
   volatile unsigned int ts_sts2		:1;  /* [10:10] */
   volatile unsigned int tf_sts			:1;  /* [11:11] */
   volatile unsigned int au_int_sts		:1;  /* [12:12] */
   volatile unsigned int dma_int_sts		:1;  /* [13:13] */
   volatile unsigned int gdm_int_sts		:1;  /* [14:14] */
   volatile unsigned int grc_palette_sts	:1;  /* [15:15] */
   volatile unsigned int grc_attr_sts		:1;  /* [16:16] */
   volatile unsigned int grc_seq_sts		:1;  /* [17:17] */
   volatile unsigned int grc_gctl_sts		:1;  /* [18:18] */
   volatile unsigned int grc_crtc_sts		:1;  /* [19:19] */
   volatile unsigned int grc_crtcext_sts	:1;  /* [20:20] */
   volatile unsigned int grc_pltram_sts		:1;  /* [21:21] */
   volatile unsigned int grc_xcura_sts		:1;  /* [22:22] */
   volatile unsigned int grc_xcurb_sts		:1;  /* [23:23] */
   volatile unsigned int rsvd1			:18; /* [31:24] */
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
	 tVU32  arm2ssp_msgbase;
	 tVU32  ssp2arm_msgbase;
	 tVU32  arm2ssp_pi;
	 tVU32  arm2ssp_ci;
	 tVU32  ssp2arm_pi;
	 tVU32  ssp2arm_ci;
	 }TOP_CTRL_REGS;


/******************************* Encryption Engine Registers ***************/
typedef struct EE_CTL 
	{
	 volatile unsigned int ee_en			:1;  /* [0 : 0] */
	 volatile unsigned int ee_irq_en		:1;  /* [1 : 1] */
	 volatile unsigned int ee_fiq_en		:1;  /* [2 : 2] */
	 volatile unsigned int rsvd1			:29; /* [31: 3] */
	 } EE_CTL;

typedef struct EE_STS 
	{
	 volatile unsigned int ee_int_sts		:1; /* [0 : 0] */
	 volatile unsigned int ee_busy			:1; /* [1 : 1] */
	 volatile unsigned int rsvd1			:30; /*[31: 2] */
	 } EE_STS;

typedef struct EE_DTB 
	{
	 volatile unsigned int ro		         :4;  /* [3 : 0] */
	 volatile unsigned int desc_base		:28; /* [31: 4] */
	 } EE_DTB;

typedef struct EE_KTB 
	{
	 volatile unsigned int ro	                :4;  /* [3 : 0] */
	 volatile unsigned int key_base		:28; /* [31: 4] */
	 } EE_KTB;


typedef union U_EE_CTL { EE_CTL s; unsigned int val; } u_eectl ;
typedef union U_EE_STS { EE_STS s; unsigned int val; } u_eests ;
typedef union U_EE_DTB { EE_DTB s; unsigned int val; } u_eedtb ;
typedef union U_EE_KTB { EE_KTB s; unsigned int val; } u_eektb ;

typedef struct ENCRYPTION_REGS 
	{
	 volatile u_eectl	eectl ;
	 volatile u_eests	eests ;
	 volatile u_eedtb	eedtb ;
	 volatile u_eektb	eektb ;
	 volatile unsigned int rsvd1 ;
	 volatile unsigned int rsvd2 ;
	 volatile unsigned int rsvd3 ;
	 volatile unsigned int rsvd4 ;
	 volatile unsigned int last_key0 ;
	 volatile unsigned int last_key1 ;
	 volatile unsigned int last_key2 ;
	 volatile unsigned int last_key3 ;
	 } ENCRYPTION_REGS;


typedef struct ENC_DEC_REGS 
	{
	 tVU32  Control;
	 tVU32  Status;
	 tVU32  Dptrbase;
	 tVU32  Keybase;
	 tVU32  rsvd1;
	 tVU32  rsvd2;
	 tVU32  rsvd3;
	 tVU32  rsvd4;
	 tVU32  Last_Key0;
	 tVU32  Last_Key1;
	 tVU32  Last_Key2;
	 tVU32  Last_Key3;
	 }ENC_DEC_REGS;

/******************* Tile Snoop Registers ********************/
typedef struct TS_CTL 
	{
	 volatile unsigned int tse_en		:1; /* [0 : 0] */
	 volatile unsigned int mgamode	        :1; /* [1 : 1] */
	 volatile unsigned int bpp		:2; /* [3 : 2] */
	 volatile unsigned int lpt       	:2; /* [5 : 4] */
	 volatile unsigned int cpt      	:2; /* [7 : 6] */
	 volatile unsigned int tse_irq_en       :1; /* [8 : 8] */
	 volatile unsigned int tse_fiq_en	:1; /* [9 : 9] */
	 volatile unsigned int rsvd1		:3; /* [12:10] */
	 volatile unsigned int initscreen0	:1; /* [13:13] */
	 volatile unsigned int initscreen1	:1; /* [14:14] */
	 volatile unsigned int ownscreen	:1; /* [15:15] */
         volatile unsigned int offset		:12; /* [27:16] */
	 volatile unsigned int rsvd2		:4; /* [31:28] */
	 } TS_CTL;

typedef struct TS_STS0 
	{
	 volatile unsigned int rsvd0 		:8; /* [7 : 0] */
	 volatile unsigned int ascii_mod	:1; /* [8 : 8] */
	 volatile unsigned int attr_mod		:1; /* [9 : 9] */
	 volatile unsigned int font_mod		:1; /* [10:10] */
	 volatile unsigned int mgamode		:1; /* [11:11] */
	 volatile unsigned int scroff	  	:1; /* [12:12] */
	 volatile unsigned int rsvd1	  	:19;/* [31:13] */
	 } TS_STS0;

typedef struct TS_CNT 
	{
	 volatile unsigned int tilecount	:12; /* [11: 0] */
	 volatile unsigned int rsvd1	  	:20; /* [31:12] */
	 } TS_CNT;


typedef union U_TS_CTL { TS_CTL s; unsigned int val; } u_tsctl ;
typedef union U_TS_STS0 { TS_STS0 s; unsigned int val; } u_tssts0 ;
typedef union U_TS_CNT { TS_CNT s; unsigned int val; } u_tscnt ;

typedef struct TILE_SNOOP_REGS 
	{
	 volatile u_tsctl       tsctl ;
	 volatile u_tssts0      tssts0 ;
	 volatile unsigned int  tileintrclmn_lo;
	 volatile unsigned int  tileintrclmn_hi;
	 volatile unsigned int  tileintrline_lo;
	 volatile unsigned int  tileintrline_hi;
	 volatile u_tscnt       tilecnt;
	 volatile unsigned int  ts_intrcnt;
	 } TILE_SNOOP_REGS;

/******************************* Tile Fetch Registers ***************/
typedef struct TF_CTL 
	{
	 volatile unsigned int tfe_en		:1;  /* [0 : 0] */
	 volatile unsigned int tfe_irq_en	:1;  /* [1 : 1] */
	 volatile unsigned int tfe_fiq_en	:1;  /* [2 : 2] */
	 volatile unsigned int rsvd1		:29; /* [31: 3] */
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
	 }TFE_REGS;

/******************************* DMA CONTROL Registers ***************/
typedef struct DMA_CTL 
	{
	 volatile unsigned int dma_en		:1;  /* [0 : 0] */
	 volatile unsigned int dma_irq_en	:1;  /* [1 : 1] */
	 volatile unsigned int dma_fiq_en	:1;  /* [2 : 2] */
	 volatile unsigned int dptrsrce 	:1;  /* [3 : 3] */
	 volatile unsigned int rsvd1	        :3;  /* [6 : 4] */
	 volatile unsigned int streamer_en	:1;  /* [7 : 7] */
	 volatile unsigned int rsvd2		:24; /* [31: 8] */
	 } DMA_CTL;

typedef struct DMA_STS 
	{
	 volatile unsigned int dma_busy	:1;  /* [0 : 0] */
	 volatile unsigned int dma_int_sts	:1;  /* [1 : 1] */
	 volatile unsigned int rsvd1	        :30; /* [31: 2] */
	 } DMA_STS;

typedef struct DMA_DTB 
	{
	 volatile unsigned int ro		:2;  /* [1 : 0] */
	 volatile unsigned int desc_base	:30; /* [31: 2] */
	 } DMA_DTB;


typedef union U_DMA_CTL { DMA_CTL s; unsigned int val; } u_dmactl ;
typedef union U_DMA_STS { DMA_STS s; unsigned int val; } u_dmasts ;
typedef union U_DMA_DTB { DMA_DTB s; unsigned int val; } u_dmadtb ;

typedef struct DMA_ENGINE_REGS 
	{
	 volatile u_dmactl      dmactl ;
	 volatile u_dmasts      dmasts ;
	 volatile u_dmadtb      dmadtb ;
	 volatile unsigned int  chksum ;
	 } DMA_ENGINE_REGS;

////////// GRC CONTROL OFFSETS ////////////////
typedef struct GRC_CTL_IRQEN 
	{
	 volatile unsigned int palette_en	:1;  /* [0 : 0] */
	 volatile unsigned int attr_en	        :1;  /* [1 : 1] */
	 volatile unsigned int seq_en	        :1;  /* [2 : 2] */
	 volatile unsigned int gctl_en		:1; /*  [3 : 3] */
	 volatile unsigned int crtc_en		:1; /*  [4 : 4] */
	 volatile unsigned int crtcext_en	:1; /*  [5 : 5] */
	 volatile unsigned int pltram_en	:1; /*  [6 : 6] */
	 volatile unsigned int xcurcol_en	:1; /*  [7 : 7] */
	 volatile unsigned int xcurctl_en	:1; /*  [8 : 8] */
	 volatile unsigned int rsvd		:1; /*  [9 :31] */
	 } GRC_CTL_IRQEN;

typedef struct GRC_CTL_FIQEN 
	{	 
         volatile unsigned int palette_en	:1;  /* [0 : 0] */
	 volatile unsigned int attr_en	        :1;  /* [1 : 1] */
	 volatile unsigned int seq_en	        :1;  /* [2 : 2] */
	 volatile unsigned int gctl_en		:1; /*  [3 : 3] */
	 volatile unsigned int crtc_en		:1; /*  [4 : 4] */
	 volatile unsigned int crtcext_en	:1; /*  [5 : 5] */
	 volatile unsigned int pltram_en	:1; /*  [6 : 6] */
	 volatile unsigned int xcurcol_en	:1; /*  [7 : 7] */
	 volatile unsigned int xcurctl_en	:1; /*  [8 : 8] */
	 volatile unsigned int rsvd		:1; /*  [9 :31] */
	 } GRC_CTL_FIQEN;

typedef struct GRC_CTL_INTSTS 
	{
         volatile unsigned int palette_sts	:1;  /* [0 : 0] */
	 volatile unsigned int attr_sts	        :1;  /* [1 : 1] */
	 volatile unsigned int seq_sts	        :1;  /* [2 : 2] */
	 volatile unsigned int gctl_sts		:1; /*  [3 : 3] */
	 volatile unsigned int crtc_sts		:1; /*  [4 : 4] */
	 volatile unsigned int crtcext_sts	:1; /*  [5 : 5] */
	 volatile unsigned int pltram_sts	:1; /*  [6 : 6] */
	 volatile unsigned int xcurcol_sts	:1; /*  [7 : 7] */
	 volatile unsigned int xcurctl_sts	:1; /*  [8 : 8] */
	 volatile unsigned int rsvd		:1; /*  [9 :31] */
	 } GRC_CTL_INTSTS;


typedef union U_GRC_CTL_IRQEN { GRC_CTL_IRQEN s; unsigned int val; } u_grc_ctl_irqen ;
typedef union U_GRC_CTL_FIQEN { GRC_CTL_FIQEN s; unsigned int val; } u_grc_ctl_fiqen ;
typedef union U_GRC_CTL_INTSTS { GRC_CTL_INTSTS s; unsigned int val; } u_grc_ctl_intsts ;

typedef struct GRC_CTL_REGS 
	{
	 volatile u_grc_ctl_irqen      irqen ;
	 volatile u_grc_ctl_fiqen      fiqen ;
	 volatile u_grc_ctl_intsts     intsts;
	 volatile unsigned int         count;
	 } GRC_CTL_REGS;


#define GRC_PLTRAM_DATA_OFF     0x0000
#define GRC_CURSORCOL_DATA_OFF  0x0400
#define GRC_CURSORCTL_DATA_OFF  0x0440
#define GRC_CRTC_DATA_OFF       0x0480
#define GRC_CRTCEXT_DATA_OFF    0x04C0
#define GRC_SEQ_DATA_OFF        0x04C8
#define GRC_GCTL_DATA_OFF       0x04D0
#define GRC_ATTR_DATA_OFF       0x04E0
#define GRC_CTL_OFF             0x0500

/******** Structure for descriptor in tile fetch engine ********/
// source/dest addr aligned to bytes
// length in bytes

typedef struct DMA_DESCRIPTOR 
	{
	 volatile unsigned int lengthSI		; /* [31:16] */
	 volatile unsigned int rsvd2		; /* [63:32] */
	 volatile unsigned int source_addr	; /* [31: 0] */
	 volatile unsigned int dest_addr	; /* [63:32] */
	 } DMA_DESCRIPTOR;


typedef struct DMA_DESCRIPTOR1 
	{
	 volatile unsigned int I		:1;  /* [0 : 0] */
	 volatile unsigned int S		:1;  /* [1 : 1] */
	volatile unsigned int rsvd1		:14; /* [15: 2] */
	 volatile unsigned int length		:16; /* [31:16] */
	 volatile unsigned int rsvd2		:32; /* [63:32] */
	 volatile unsigned int source_addr	:32; /* [31: 0] */
	 volatile unsigned int dest_addr	:32; /* [63:32] */
	 } DMA_DESCRIPTOR1;

/******** Structure for descriptor in tile fetch engine ********/
// width in bytes
// height in lines
// source/dest addr aligned to 16 bytes

typedef struct TFE_DESCRIPTOR 
	{
	 volatile unsigned int I		:1;  /* [0 : 0] */
	 volatile unsigned int S		:1;  /* [1 : 1] */
	 volatile unsigned int rsvd1		:11; /* [12: 2] */
	 volatile unsigned int mode		:3;  /* [15:13] */
	 volatile unsigned int offset		:12; /* [27:16] */
	 volatile unsigned int rsvd2		:4;  /* [31:28] */
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

typedef struct ENC_DEC_DPTR 
	{
	 tVU32  Control;
	 tVU32  Length;
	 tVU32  Srcaddrs;
	 tVU32  Dstaddrs;	 
	 }ENC_DEC_DPTR;

typedef struct EE_DESCRIPTOR 
	{
	 volatile unsigned int I		:1;  /* [0 : 0] */
	 volatile unsigned int S		:1;  /* [1 : 1] */
	 volatile unsigned int D		:1;  /* [2 : 2] */
	 volatile unsigned int DesAes_option	:1;  /* [3 : 3] */
	 volatile unsigned int IV_sel		:1;  /* [4 : 4] */
	 volatile unsigned int DesAes		:1;  /* [5 : 5] */
	 volatile unsigned int rsvd1		:9;  /* [15: 7] */
	 volatile unsigned int key_index	:8;  /* [23:16] */
	 volatile unsigned int rsvd2		:8;  /* [31:24] */
	 volatile unsigned int length		:12; /* [43:32] */
	 volatile unsigned int rsvd3		:20; /* [63:44] */
	 volatile unsigned int rsvd4		:4;  /* [3 : 0] */
	 volatile unsigned int source_addr	:12; /* [15: 4] */
	 volatile unsigned int rsvd5		:16; /* [31:16] */
	 volatile unsigned int rsvd6		:4;  /* [35:32] */
	 volatile unsigned int dest_addr	:12; /* [47:36] */
	 volatile unsigned int rsvd7		:16; /* [63:48] */
	 } EE_DESCRIPTOR;

/******** Structure for key table in Encryption engine ********/

typedef struct EE_KEY 
	{
	 volatile unsigned int key1_lo ;
	 volatile unsigned int key1_hi ;
	 volatile unsigned int key2_lo ;
	 volatile unsigned int key2_hi ;
	 volatile unsigned int key3_lo ;
	 volatile unsigned int key3_hi ;
	 volatile unsigned int IV_lo ;
	 volatile unsigned int IV_hi ;
	 } EE_KEY;



typedef struct AES_KEYSTRUCT 
	{
	 volatile unsigned int KEY1_lo;
	 volatile unsigned int KEY1_hi;
	 volatile unsigned int KEY2_lo;
	 volatile unsigned int KEY2_hi;
	 volatile unsigned int IV0_lo;
	 volatile unsigned int IV0_hi;
	 volatile unsigned int IV1_lo;
	 volatile unsigned int IV1_hi;
	 }AES_KEYSTRUCT;


#define ENCDEC_BASE     SECMAP_BASE+0x100

#define ENCDEC_CTRL         ENCDEC_BASE  
#define ENCDEC_STS          ENCDEC_BASE+0x4 
#define ENCDEC_DescTabBase  ENCDEC_BASE+0x8 
#define ENCDEC_KeyTabBase   ENCDEC_BASE+0xC
#define ENCDEC_FifoEn       ENCDEC_BASE+0x10
#define ENCDEC_DmaEn        ENCDEC_BASE+0x14  
#define ENCDEC_fb_iv0        ENCDEC_BASE+0x18 
#define ENCDEC_fb_iv1        ENCDEC_BASE+0x1C
#define ENCDEC_last_key0    ENCDEC_BASE+0x20
#define ENCDEC_last_key1    ENCDEC_BASE+0x24
#define ENCDEC_last_key2    ENCDEC_BASE+0x28
#define ENCDEC_last_key3    ENCDEC_BASE+0x2c
#define ENCDEC_Descriptor0  ENCDEC_BASE+0x30
#define ENCDEC_Descriptor1  ENCDEC_BASE+0x34
#define ENCDEC_Descriptor2  ENCDEC_BASE+0x38
#define ENCDEC_Descriptor3  ENCDEC_BASE+0x3c
#define ENCDEC_key0         ENCDEC_BASE+0x40
#define ENCDEC_key1         ENCDEC_BASE+0x44
#define ENCDEC_key2         ENCDEC_BASE+0x48
#define ENCDEC_key3         ENCDEC_BASE+0x4c
#define ENCDEC_key4         ENCDEC_BASE+0x50
#define ENCDEC_key5         ENCDEC_BASE+0x54
#define ENCDEC_IV0         ENCDEC_BASE+0x58
#define ENCDEC_IV1         ENCDEC_BASE+0x5c
#define ENCDEC_data0        ENCDEC_BASE+0x60

#define SHA_BASE        SECMAP_BASE + 0x200

#define sha_ctrl         SHA_BASE+0x00
#define sha_sts          SHA_BASE+0x04
#define sha_DescTabBase  SHA_BASE+0x08
#define sha_KeyTabBase   SHA_BASE+0x0C
#define sha_FifoEn       SHA_BASE+0x10
#define sha_DmaEn        SHA_BASE+0x14
#define sha_result_data0         SHA_BASE+0x1C
#define sha_result_data1         SHA_BASE+0x20
#define sha_result_data2         SHA_BASE+0x24
#define sha_result_data3         SHA_BASE+0x28
#define sha_result_data4         SHA_BASE+0x2C
#define sha_Descriptor0   SHA_BASE+0x30
#define sha_Descriptor1   SHA_BASE+0x34
#define sha_Descriptor2   SHA_BASE+0x38
#define sha_Descriptor3   SHA_BASE+0x3C
#define sha_key0          SHA_BASE+0x40
#define sha_key1          SHA_BASE+0x44
#define sha_key2          SHA_BASE+0x48
#define sha_key3          SHA_BASE+0x4C
#define sha_key4          SHA_BASE+0x50
#define sha_key5          SHA_BASE+0x54
#define sha_src_data      SHA_BASE+0x60
  

////// assigning register space to different structures ///////////

#define  FGBBASE                FrameGrabber
#define  FGB_TOP_REGS	        ((volatile TOP_CTRL_REGS *)(FGBBASE))
#define  FGB_TF_REGS            ((volatile TILE_FETCH_REGS *)(FGBBASE+0x100))
#define  TFE_BLOCK              ((volatile TFE_REGS *)(FGBBASE+0x100))    
#define  FGB_DMA_REGS           ((volatile DMA_ENGINE_REGS *)(FGBBASE+0x200))
#define  FGB_GDMA_REGS          FGBBASE+0x300
#define  FGB_TS_REGS            ((volatile TILE_SNOOP_REGS *)(FGBBASE+0x400))
#define  FGB_SM                 (FGBBASE+0x600)
#define  FGB_GRC                (FGBBASE+0x800)
#define  FGB_GRCCTL_REGS        ((volatile GRC_CTL_REGS *)(FGB_GRC+ GRC_CTL_OFF))
#define  FGB_GRCATTR_REGS       (FGB_GRC + GRC_ATTR_DATA_OFF)
#define  FGB_GRCSEQ_REGS        (FGB_GRC + GRC_SEQ_DATA_OFF)
#define  FGB_GRCGCTL_REGS       (FGB_GRC + GRC_GCTL_DATA_OFF)
#define  FGB_GRCCRTC_REGS       (FGB_GRC + GRC_CRTC_DATA_OFF)
#define  FGB_GRCCRTCEXT_REGS    (FGB_GRC + GRC_CRTCEXT_DATA_OFF)
#define  FGB_GRCCURSORCOL_REGS  (FGB_GRC + GRC_CURSORCOL_DATA_OFF)
#define  FGB_GRCCURSORCTL_REGS  (FGB_GRC + GRC_CURSORCTL_DATA_OFF)
#define  FGB_GRCPLTRAM_REGS     (FGB_GRC + GRC_PLTRAM_DATA_OFF)

///// Secondary Map Equates

#define SECMAP_BASE             FGB_SECBASE
#define  FGB_EE_REGS            ((volatile ENCRYPTION_REGS *)(SECMAP_BASE))
#define  ENC_DEC_BLOCK          ((volatile ENC_DEC_REGS *)(SECMAP_BASE+0x100))
#define  FGB_SHA_REGS           ((volatile ENC_DEC_REGS  *)(SECMAP_BASE+0x200))
#define  MOD_EXPBASE            SECMAP_BASE+0x300
#define  MOD_EXPMEM_START       SECMAP_BASE+0x1000

  




  
  

#define  LMEMOFFSET             0x1000  
#define  FGB_LMEM	        (LPCREGBASE+LMEMOFFSET)


#define  ORPWORKAREA     FGB_LMEM+0x2400          //0x2000 to 0x2400 for code data 1K
#define  ORPMAILBOXST    ORPWORKAREA
#define  LMEMWORKAREA    ORPWORKAREA+0x200           //space for mailbox  256 bytes
#define  TFE_DPTR1MEM    ORPWORKAREA+0x200           //TFE DPTR 256 bytes
#define  TFE_DPTR2MEM    ORPWORKAREA+0x210           
#define  TFE_DPTR3MEM    ORPWORKAREA+0x220           

#define  ENCDEC_DPTR1MEM   ORPWORKAREA+0x300         //ENC DPTRS 256 bytes
#define  ENCDEC_DPTR2MEM   ORPWORKAREA+0x310
#define  ENCDEC_DPTR3MEM   ORPWORKAREA+0x320  

#define  EEKEY_MEMBASE   ORPWORKAREA+0x400

  
#define  LMEM_TFEBUFF    ORPWORKAREA+0x800           //space for descriptors 0x600 =1.5K
#define  LMEM_ENCBUFF    ORPWORKAREA+0x1000           //2k
#define  LMEM_DECBUFF    ORPWORKAREA+0x1800           //2K  
  
  


extern  void  FGEEHandler(void);
extern  int   Compare4bitpackedmode(int srcaddrs,int dstaddrs,int count,int mode16,int width, int framewidth);
extern void  Convert_Plannar_Packed(int Srcaddress,int Dstaddress,int size,int mode16,int width,int framewidth);
extern int Compare4bitpackedmode(int srcaddrs,int dstaddrs,int count,int jump8_16,int width,int framewidth);
extern void ConvertAttr2Asciidata(int srcaddrs,int dstaddrs,int count,int jump8_16,int width,int framewidth);
extern void Convert2Asciidata(int srcaddrs,int dstaddrs,int count,int jump8_16,int width,int framewidth);
extern void Convert2fontfetchdata(int srcaddrs,int dstaddrs,int count,int jump8_16,int width,int framewidth);
extern tU32 SwapBytes(tU32 a);


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

//ENC_DEC_SPECIFIC DEFINES
#define ENCDEC_ENABLE		(1<<0)
#define ENCDEC_IRQEN            (1<<1)
#define ENCDEC_FIQEN            (1<<2)

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
#define EE_KEYORDER            (1<<7)
#define EE_IVORDER             (1<<8)
#define EE_FUSE_KEY_SEL        (1<<9)    
#define KEYINDEX(index)         (index<<16)

   

//SHA1 Bit defines
#define SHA_I                    (1<<0)
#define SHA_S                    (1<<1)
#define SHA_FIRST_IV             (1<<2)
#define SHA_NOTFIRST_IV          (0<<2)  
#define SHA_SWAPORDER            (1<<6)
#define SHA_SRCDATA_SWAP           (1<<6)  
#define SHA_RESULT_SWAP            (1<<7)    
#define SHA_INTRSTS              (1<<1) 

#define ENCRPTFLAG   		FGBSecWorkBase+0x600
#define DECRPTFLAG   		FGBSecWorkBase+0x604
#define GDMAENGINTRPTFLAG 	FGBSecWorkBase+0x624
#define AUTH_INTRFLAG           FGBSecWorkBase+0x62C

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

/******** Structure for descriptor in tile fetch engine ********/
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

typedef struct PKCP_regs
  {
   tVU32 PKA_APTR;
   tVU32 PKA_BPTR;
   tVU32 PKA_CPTR;
   tVU32 PKA_DPTR;
   tVU32 PKA_ALENGTH;
   tVU32 PKA_BLENGTH;
   tVU32 PKA_SHIFT;
   tVU32 PKA_FUNCTION;
   tVU32 PKA_COMPARE;
   tVU32 PKA_MSW;
   tVU32 PKA_DIVMSW;
   }PKCP_regs;

//PKA_FUNCTION_bit definations
#define MULTIPLY    1<<0
#define ADDSUB      1<<1
#define MSONE       1<<3
#define ADD         1<<4
#define SUB         1<<5
#define RSHIFT      1<<6
#define LSHIFT      1<<7
#define DIV         1<<8
#define MODULO      1<<9
#define COMPARE     1<<10
#define COPY        1<<11
#define SEQ_EXPMOD_ACT4  2<<12
#define SEQ_EXPMOD_ACT2  4<<12
#define SEQ_EXPMOD_VAR   6<<12
#define RUN_PKA          1<<15

  
///// Secondary Map Equates
#define  SECMAP_BASE            FGB_SECBASE
#define  FGB_EE_REGS           ((volatile ENCRYPTION_REGS *)(SECMAP_BASE + 0x100))
#define  ENC_DEC_BLOCK         ((volatile ENC_DEC_REGS *)(SECMAP_BASE+0x100))
#define  FGB_SHA_REGS          (SECMAP_BASE+0x200)
#define  MOD_EXPBASE           SECMAP_BASE+0x300
#define  MOD_EXPMEM_START      SECMAP_BASE+0x1000
#define  MODEXPREGS            ((volatile PKCP_regs *)(MOD_EXPBASE))

  

//defines for modexp..
#define  Modexp_control    SECMAP_BASE
#define  Modexp_intrenable  (SECMAP_BASE+0x4)       
#define  Modexp_intrsts  (SECMAP_BASE+0x8)     

//bit defines for modexp control
#define  RESET_MODEXP     1<<2  

  
//bit defines for modexp_interrupt status register
//when set engine is done ..write one to clear this  
#define LNME_DONE                1<<0
#define PKCP_OR_SEQUENCER_DONE   1<<1
#define SEQUENCER_DONE           1<<2            

//when set engine is ready to use else its busy  
#define LNME_RDY                1<<4
#define PKCP_OR_SEQUENCER_RDY   1<<5
#define SEQUENCER_RDY           1<<6            
  

  

//R/W buffers
#define ENC_BUFF1    SEC_ENCBUFF_START
#define ENC_BUFF2    SEC_ENCBUFF_START+0x400
#define ENC_BUFF3    SEC_ENCBUFF_START+0x800
#define ENC_BUFF4    SEC_ENCBUFF_START+0xC00  



//R/W buffers
#define DEC_BUFF1    SEC_DECBUFF_START
#define DEC_BUFF2    SEC_DECBUFF_START+0x400
#define DEC_BUFF3    SEC_DECBUFF_START+0x800
#define DEC_BUFF4    SEC_DECBUFF_START+0xC00  


//mod exp can go to any of these two based on mux selctions
#define ModexpIram0    0x04005000
#define ModexpIram1    0x04006000

#define ModexpDram0    0x04006000
#define ModexpDram1    0x04007000
  
  

extern   tPVU8  Enc_Read_Bytes(tPVU8 dstaddrs,tU32 Count);
extern   tU32   write_crypto_data(tPVU32 srcaddrs,tPVU32 dstaddrs,tU32 Count);


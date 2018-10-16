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

/*************************************************************************************************
 * File name:  pilotII_hw_sec.h
 *
 * Description:
 *             This file contains all the bit definitions and data structures related to security
 *             block on pilot-II.
 *
 * Date Created: 
 *
 * Date Modified:
 *      08-15-2006
 *
 * Changes:
 *     Initial Creation          ---  Dhanraj
 *     Modified data structures  ---  Murali  (08-15-2006)
 *
 *
************************************************************************************************/
/*
 * Security Work Request Block (WRB) definitions
*/

/*
 * Control filed bit definitions
*/
#define P2_EE_ENCDEC_ENABLE 	      1<<0
#define P2_EE_ENCDEC_IRQEN            1<<1
#define P2_EE_ENCDEC_FIQEN            1<<2

/*
 * Status field bit definitons
*/
#define P2_EE_ENCDEC_DONE             1<<0
#define P2_EE_ENCDEC_BUSY             1<<1          

/*
 * Security Flags bit definitions for encryption engine
 * Encrypton engine supports DES-CBC/3DES-CBC/AES-CBC/AES-CTR encryption alorithms 
*/

#define P2_EE_INTR_EN              (1<<0)  /* EE_I */
#define P2_EE_LAST_DESC            (1<<1)  /* EE_S */
#define P2_EE_DECRYPT_ENABLE       (1<<2)  /* EE_DECRIPT_ENABLE */
#define P2_EE_ENCRYPT_ENABLE       (0<<2)  /* EE_ENCRIPT_ENABLE */
#define P2_EE_3DES                 (1<<3)
#define P2_EE_1DES                 (0<<3)  
#define P2_EE_CTRMODE              (1<<3)
#define P2_EE_CBCMODE              (0<<3)
#define P2_EE_DESENABLE            (0<<4)
#define P2_EE_AESENABLE            (1<<4)
#define P2_EE_IVSEL                (1<<5)        //when dptrs are chained for 1block then we need to set this bit
#define P2_EE_SWAPORDER            (1<<6)  
#define P2_KEYINDEX(index)         (index<<16)   

/*
 * Security Flags bit definitions for authentication engine
 * Authentication engine supports HMAC-SHA1 with explicit padding
*/
#define P2_SHA_INTR_EN              (1<<0)  /* SHA_I */
#define P2_SHA_LAST_DESC            (1<<1)  /* SHA_S */
#define P2_SHA_FIRST_IV             (1<<2)
#define P2_SHA_NOTFIRST_IV          (0<<2)  
#define P2_SHA_SWAPORDER            (1<<6)
#define P2_SHA_SRCDATA_SWAP         (1<<6)  
#define P2_SHA_RESULT_SWAP          (1<<7)    
#define P2_SHA_INTRSTS              (1<<1) 


/******************************* Encryption Engine Registers ***************/
typedef struct _P2_EE_CTL 
{
	 tVU32 ee_en		:1;  /* [0 : 0] */
	 tVU32 ee_irq_en	:1;  /* [1 : 1] */
	 tVU32 ee_fiq_en	:1;  /* [2 : 2] */
	 tVU32 rsvd1		:29; /* [31: 3] */
} P2_EE_CTL;

typedef struct _P2_EE_STS 
{
	 tVU32 ee_int_sts	:1; /* [0 : 0] */
	 tVU32 ee_busy		:1; /* [1 : 1] */
	 tVU32 rsvd1		:30; /*[31: 2] */
} P2_EE_STS;

typedef struct _P2_EE_DTB 
{
	 tVU32 ro	        :4;  /* [3 : 0] */
	 tVU32 desc_base	:28; /* [31: 4] */
} P2_EE_DTB;

typedef struct _P2_EE_KTB 
{
	 tVU32 ro	        :4;  /* [3 : 0] */
	 tVU32 key_base		:28; /* [31: 4] */
} P2_EE_KTB;

typedef struct _P2_EE_FB_IV
{
         tVU32 iv_0;
         tVU32 iv_1;
} P2_EE_FB_IV;


typedef struct _P2_EE_DESCRIPTOR 
{
	 tVU32 intr_en		:1;  /* [0 : 0] */
	 tVU32 last_desc	:1;  /* [1 : 1] */
	 tVU32 decrypt_en	:1;  /* [2 : 2] */
	 tVU32 encr_mode	:1;  /* [3 : 3]  1DES/3DES or CBC/CTR*/
	 tVU32 encr_algo	:1;  /* [4 : 4]  DES or AES */
	 tVU32 iv_sel		:1;  /* [5 : 5] */
	 tVU32 rsvd1		:9;  /* [15: 7] */
	 tVU32 key_index	:8;  /* [23:16] */
	 tVU32 rsvd2		:8;  /* [31:24] */
	 tVU32 length		:12; /* [43:32] */
	 tVU32 rsvd3		:20; /* [63:44] */
	 tVU32 rsvd4		:4;  /* [3 : 0] */
	 tVU32 source_addr	:12; /* [15: 4] */
	 tVU32 rsvd5		:16; /* [31:16] */
	 tVU32 rsvd6		:4;  /* [35:32] */
	 tVU32 dest_addr	:12; /* [47:36] */
	 tVU32 rsvd7		:16; /* [63:48] */
 } P2_EE_DESCRIPTOR;

/******** Structure for key table in Encryption engine ********/

typedef struct _P2_EE_KEY 
{
	 tVU32 key1_lo ;
	 tVU32 key1_hi ;
	 tVU32 key2_lo ;
	 tVU32 key2_hi ;
	 tVU32 key3_lo ;
	 tVU32 key3_hi ;
	 tVU32 IV_lo ;
	 tVU32 IV_hi ;
} P2_EE_KEY;

typedef struct _P2_AES_KEYSTRUCT 
{
	 tVU32 KEY1_lo;
	 tVU32 KEY1_hi;
	 tVU32 KEY2_lo;
	 tVU32 KEY2_hi;
	 tVU32 IV0_lo;
	 tVU32 IV0_hi;
	 tVU32 IV1_lo;
	 tVU32 IV1_hi;
} P2_AES_KEY;

typedef struct _P2_AUTH_KEY
{
	 tVU32 KEY1_lo;
	 tVU32 KEY1_hi;
	 tVU32 KEY2_lo;
	 tVU32 KEY2_hi;
	 tVU32 KEY3_lo;
} P2_AUTH_KEY;

typedef union U_P2_EE_CTL { P2_EE_CTL c; tU32 val; } u_p2_eectl ;
typedef union U_P2_EE_STS { P2_EE_STS s; tU32 val; } u_p2_eests ;
typedef union U_P2_EE_DTB { P2_EE_DTB d; tU32 val; } u_p2_eedtb ;
typedef union U_P2_EE_KTB { P2_EE_KTB k; tU32 val; } u_p2_eektb ;
typedef union U_P2_EE_DESC { P2_EE_DESCRIPTOR desc; tU32 val; } u_p2_eedesc;
typedef union U_P2_EE_KEY { P2_EE_KEY des_key; P2_AES_KEY aes_key; } u_p2_eekey;

/* 
 * Encryption and Decryption Register format in union format
 */
typedef struct _P2_ENCRYPTION_REGS 
{
	 u_p2_eectl	eectl ;
	 u_p2_eests	eests ;
	 u_p2_eedtb	eedtb ;
	 u_p2_eektb	eektb ;
	 tVU32 		FifoEn ;
	 tVU32 		DMAEn ;
	 tVU32 		fb_iv0 ;
	 tVU32 		fb_iv1 ;
	 tVU32 		last_key0 ;
	 tVU32 		last_key1 ;
	 tVU32 		last_key2 ;
	 tVU32 		last_key3 ;
	 tVU32 		rsvd3 ;
	 tVU32 		rsvd4 ;
	 tVU32 		rsvd5 ;
         u_p2_eedesc	eedesc;
         u_p2_eekey	eekey;
         tVU32 		data_fifo;
} P2_ENCRYPTION_REGS;


/*
 * Encrypton or Decryption Register format
 */ 
typedef struct _P2_ENC_DEC_REGISTERS 
{
	 tVU32  Control;     /* [0x00] */
	 tVU32  Status;      /* [0x04] */
	 tVU32  Dptrbase;    /* [0x08] */
	 tVU32  Keybase;     /* [0x0C] */
	 tVU32  FifoEn;       /* [0x10] */
	 tVU32  DMAEn;       /* [0x14] */
	 tVU32  fb_iv0;      /* [0x18] */
	 tVU32  fb_iv1;      /* [0x1C] */
	 tVU32  Last_Key0;   /* [0x20] */
	 tVU32  Last_Key1;   /* [0x24] */
	 tVU32  Last_Key2;   /* [0x28] */
	 tVU32  Last_Key3;   /* [0x2C] */
         tVU32  SecFlags;    /* [0x30] */
         tVU32  rsvd3;      
         tVU32  rsvd4;
         tVU32  rsvd5;
         tVU32  key0_lo;     /* [0x40] */
         tVU32  key0_hi;     /* [0x44] */
         tVU32  key1_lo;     /* [0x48] */
         tVU32  key1_hi;     /* [0x4C] */
         tVU32  key2_lo;     /* [0x50] */
         tVU32  key2_hi;     /* [0x54] */
         tVU32  iv_lo;       /* [0x58] */
         tVU32  iv_hi;       /* [0x5C] */
         tVU32  data_fifo;   /* [0x60] */
} P2_ENC_DEC_REGISTERS;

/*
 * Authentication Register format
 */ 
typedef struct _P2_AUTH_REGS 
{
	 tVU32  Control;     /* [0x00] */
	 tVU32  Status;      /* [0x04] */
	 tVU32  Dptrbase;    /* [0x08] */
	 tVU32  Keybase;     /* [0x0C] */
	 tVU32  FifoEn;       /* [0x10] */
	 tVU32  rsvd2;       /* [0x14] */
	 tVU32  rsvd3;       /* [0x18] */
	 tVU32  icv_output0; /* [0x1C] */
	 tVU32  icv_output1; /* [0x20] */
	 tVU32  icv_output2; /* [0x24] */
	 tVU32  icv_output3; /* [0x28] */
	 tVU32  icv_output4; /* [0x2C] */
         tVU32  SecFlags;    /* [0x30] */
         tVU32  rsvd4;      
         tVU32  rsvd5;
         tVU32  rsvd6;
         tVU32  key0_lo;     /* [0x40] */
         tVU32  key0_hi;     /* [0x44] */
         tVU32  key1_lo;     /* [0x48] */
         tVU32  key1_hi;     /* [0x4C] */
         tVU32  key2_lo;     /* [0x50] */
         tVU32  key2_hi;     /* [0x54] */
         tVU32  rsvd8;       /* [0x58] */
         tVU32  rsvd9;       /* [0x5C] */
         tVU32  data_fifo;   /* [0x60] */
} P2_AUTH_REGS;

/******** Structure for descriptor in tile fetch engine ********/
typedef struct _P2_ENC_DEC_DPTR 
{
	 tVU32  Control;
	 tVU32  Length;
	 tVU32  Srcaddrs;
	 tVU32  Dstaddrs;	 
} P2_ENC_DEC_DPTR;

///// Secondary Map Equates
#define  P2_SECMAP_BASE           FGB_SECBASE
#define  P2_FGB_EE_REGS           ((volatile P2_ENCRYPTION_REGS *)(P2_SECMAP_BASE + 0x100))
#define  P2_ENC_DEC_REGS          ((volatile P2_ENC_DEC_REGISTERS *)(P2_SECMAP_BASE+0x100))
#define  P2_FGB_SHA_REGS          ((volatile P2_AUTH_REGS *) (P2_SECMAP_BASE+0x200))

#define P2_ENCDEC_BASE         P2_SECMAP_BASE+0x100

#define P2_ENCDEC_CTRL         ENCDEC_BASE  
#define P2_ENCDEC_STS          ENCDEC_BASE+0x4 
#define P2_ENCDEC_DescTabBase  ENCDEC_BASE+0x8 
#define P2_ENCDEC_KeyTabBase   ENCDEC_BASE+0xC
#define P2_ENCDEC_FifoEn       ENCDEC_BASE+0x10
#define P2_ENCDEC_DmaEn        ENCDEC_BASE+0x14  
#define P2_ENCDEC_fb_iv0       ENCDEC_BASE+0x18 
#define P2_ENCDEC_fb_iv1       ENCDEC_BASE+0x1C
#define P2_ENCDEC_last_key0    ENCDEC_BASE+0x20
#define P2_ENCDEC_last_key1    ENCDEC_BASE+0x24
#define P2_ENCDEC_last_key2    ENCDEC_BASE+0x28
#define P2_ENCDEC_last_key3    ENCDEC_BASE+0x2c
#define P2_ENCDEC_Descriptor0  ENCDEC_BASE+0x30
#define P2_ENCDEC_Descriptor1  ENCDEC_BASE+0x34
#define P2_ENCDEC_Descriptor2  ENCDEC_BASE+0x38
#define P2_ENCDEC_Descriptor3  ENCDEC_BASE+0x3c
#define P2_ENCDEC_key0         ENCDEC_BASE+0x40
#define P2_ENCDEC_key1         ENCDEC_BASE+0x44
#define P2_ENCDEC_key2         ENCDEC_BASE+0x48
#define P2_ENCDEC_key3         ENCDEC_BASE+0x4c
#define P2_ENCDEC_key4         ENCDEC_BASE+0x50
#define P2_ENCDEC_key5         ENCDEC_BASE+0x54
#define P2_ENCDEC_IV0          ENCDEC_BASE+0x58
#define P2_ENCDEC_IV1          ENCDEC_BASE+0x5c
#define P2_ENCDEC_data0        ENCDEC_BASE+0x60

#define P2_SHA_BASE            P2_SECMAP_BASE + 0x200

#define P2_SHA_ctrl            SHA_BASE+0x00
#define P2_SHA_sts             SHA_BASE+0x04
#define P2_SHA_DescTabBase     SHA_BASE+0x08
#define P2_SHA_KeyTabBase      SHA_BASE+0x0C
#define P2_SHA_FifoEn          SHA_BASE+0x10
#define P2_SHA_DmaEn           SHA_BASE+0x14
#define P2_SHA_result_data0    SHA_BASE+0x1C
#define P2_SHA_result_data1    SHA_BASE+0x20
#define P2_SHA_result_data2    SHA_BASE+0x24
#define P2_SHA_result_data3    SHA_BASE+0x28
#define P2_SHA_result_data4    SHA_BASE+0x2C
#define P2_SHA_Descriptor0     SHA_BASE+0x30
#define P2_SHA_Descriptor1     SHA_BASE+0x34
#define P2_SHA_Descriptor2     SHA_BASE+0x38
#define P2_SHA_Descriptor3     SHA_BASE+0x3C
#define P2_SHA_key0            SHA_BASE+0x40
#define P2_SHA_key1            SHA_BASE+0x44
#define P2_SHA_key2            SHA_BASE+0x48
#define P2_SHA_key3            SHA_BASE+0x4C
#define P2_SHA_key4            SHA_BASE+0x50
#define P2_SHA_key5            SHA_BASE+0x54
#define P2_SHA_src_data        SHA_BASE+0x60

#define P2_ENCRPTFLAG          FGBSecWorkBase+0x600
#define P2_DECRPTFLAG          FGBSecWorkBase+0x604
#define P2_GDMAENGINTRPTFLAG   FGBSecWorkBase+0x624
#define P2_AUTH_INTRFLAG       FGBSecWorkBase+0x62C

/*
 * Read Write  buffers
 */
#define P2_ENC_BUFF1    SEC_ENCBUFF_START
#define P2_ENC_BUFF2    SEC_ENCBUFF_START+0x400
#define P2_ENC_BUFF3    SEC_ENCBUFF_START+0x800
#define P2_ENC_BUFF4    SEC_ENCBUFF_START+0xC00  

#define P2_DEC_BUFF1    SEC_DECBUFF_START
#define P2_DEC_BUFF2    SEC_DECBUFF_START+0x400
#define P2_DEC_BUFF3    SEC_DECBUFF_START+0x800
#define P2_DEC_BUFF4    SEC_DECBUFF_START+0xC00  


extern   tU32 write_crypto_data(tPVU32 srcaddrs,tPVU32 dstaddrs,tU32 Count);

void
P2_ProgramHWEncryptEngine(tU8 *src, tU8 *dst, tU32 length, tU8 *key, tU8 *iv, tU32 key_len, tU32 iv_len, tU32 flags, tU8 encrypt);

void
P2_ProgramHWAuthEngine(tU8 *src, tU8 *dst, tU32 length, tU8 *key, tU32 algo);


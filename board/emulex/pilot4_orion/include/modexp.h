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


/************************************************************************
* File name:     modexp.h
*
* Description: 
*	      This file contains all register definitions for Safenet's MODEXP(PKA)
*             block. These base addresses are specific to Pilot II. 
*	      
*	      
* Created on :	10/10/05
*
* History:
*
*
*************************************************************************/

#ifndef __MODEXP_H__
#define __MODEXP_H__


/*
 * P2_SECMAP_BASE is defined in pilotII_hw_sec.h as FGB_SECBASE which is
 * 0x40A00000
*/
#define  P2_PKA_BASE			(P2_SECMAP_BASE+0x300)
/* 
 *PKA Instruction RAM Base addres (Sequencer code is downloaded and runs from here)
 *PKA Instructcion RAM is of 2KBytes in size
*/
#define P2_PKA_ROM_ADDRESS		(0x04006000)
/*
 *PKA Data Ram Base Address (All the vectors will be loaded in this place)
 *PKA Data RAM is of 4K in size
*/
#define P2_PKA_RAM_ADDRESS_FROM_ARM	(0x04007000)
#define P2_PKA_RAM_ADDRESS              (P2_SECMAP_BASE + 0x1000)        
/*
 *PKA Register Base Address
*/
#define P2_PKA_REGISTER_ADDRESS	        (P2_PKA_BASE)

#define P2_PKA_ROM_SIZE		        (2 * 1024)
#define P2_PKA_ROM_SIZE_WORDS		(512) 
#define P2_PKA_RAM_SIZE                 (4 * 1024)
#define P2_PKA_RAM_SIZE_WORDS           (1024)


/*
* MODEXP Control, Interrupt Enable and Interrupt Status regs
*/
#define  P2_PKA_CONTROL_REG   		(P2_SECMAP_BASE)
#define  P2_PKA_INTR_REG      		(P2_SECMAP_BASE+0x4)       
#define  P2_PKA_INTR_STATUS_REG		(P2_SECMAP_BASE+0x8)     

/*
 * bit defines for modexp control
*/
#define  P2_RESET_PKA     		(1<<2)  
#define  P2_PKA_SEL8K_MEM               (0x0b<<2)
  
/*
 *bit defines for modexp_interrupt status register
 *when set engine is done ..write one to clear this  
*/
#define P2_LNME_DONE               	 (1<<0)
#define P2_PKCP_OR_SEQUENCER_DONE  	 (1<<1)
#define P2_SEQUENCER_DONE          	 (1<<2)            

/*
* when set engine is ready to use else its busy  
*/
#define P2_LNME_RDY                	(1<<4)
#define P2_PKCP_OR_SEQUENCER_RDY   	(1<<5)
#define P2_SEQUENCER_RDY           	(1<<6)            

			
/*
 * PKA function opcodes
*/
#define PKA_FUNCTION_MULTIPLY		0x0001
#define PKA_FUNCTION_ADDSUB		0x0002
#define PKA_FUNCTION_MSONE		0x0008
#define PKA_FUNCTION_ADD		0x0010
#define PKA_FUNCTION_SUBTRACT		0x0020
#define PKA_FUNCTION_RSHIFT		0x0040
#define PKA_FUNCTION_LSHIFT		0x0080
#define PKA_FUNCTION_DIVIDE		0x0100
#define PKA_FUNCTION_MODULO		0x0200
#define PKA_FUNCTION_COMPARE		0x0400
#define PKA_FUNCTION_COPY		0x0800
#define PKA_FUNCTION_EXPMOD_ACT4	0x2000
#define PKA_FUNCTION_EXPMOD_ACT2	0x4000
#define PKA_FUNCTION_RUN		0x8000
#define PKA_FUNCTION_STALL		0x01000000L

/*
 * register offsets are 6 bits long
*/
#define PKA_VECTOR_A_ADDRESS_REGISTER	0x00
#define PKA_VECTOR_B_ADDRESS_REGISTER	0x01
#define PKA_VECTOR_C_ADDRESS_REGISTER	0x02
#define PKA_VECTOR_D_ADDRESS_REGISTER	0x03
#define PKA_VECTOR_A_LENGTH_REGISTER	0x04
#define PKA_VECTOR_B_LENGTH_REGISTER	0x05
#define PKA_BIT_SHIFT_VALUE_REGISTER	0x06
#define PKA_FUNCTION_REGISTER		0x07

/*
 * Modular Exponentiation (MOD_EXP) register offsets definitons
 */ 
typedef struct _P2_PKCP_REGS
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
} P2_PKCP_REGS;

#define  P2_PKA_REGS            ((volatile P2_PKCP_REGS *)(P2_PKA_REGISTER_ADDRESS))

#define MAX_VECTOR_LEN_BITS     2048
#define MAX_VECTOR_LEN_BYTES    (MAX_VECTOR_LEN_BITS/8 )
#define MAX_VECTOR_LEN_WORDS    (MAX_VECTOR_LEN_BYTES/4 )


/*
 * PKA RAM in pilot is of 4K in size (we can go upto 4096 bit vectors with this memory with address overlapping)
 */
typedef struct _P2_PKCP_RAM
{
  tU32 VectorA[MAX_VECTOR_LEN_WORDS];
  tU32 VectorB[MAX_VECTOR_LEN_WORDS];
  tU32 Reserved1;
  tU32 VectorC[MAX_VECTOR_LEN_WORDS];
  tU32 Reserved2;
  tU32 VectorD[MAX_VECTOR_LEN_WORDS];
  /*
   * The last 32 bytes of the PKA RAM is used as scratch pad area
   */
}P2_PKCP_RAM;

#define  P2_PKA_RAM            ((volatile P2_PKCP_RAM *) (P2_PKA_RAM_ADDRESS))
//#define  P2_PKA_RAM            ((volatile P2_PKCP_RAM *) (P2_PKA_RAM_ADDRESS_FROM_ARM))
  

/*
 * flags definitions
 */
#define  ADDRESS_OVERLAP         1
#define  MAX_WAIT_COUNT         1000000
int Init_PKA(void);
void select_pka_mem(void);
int PKA_Download_Sequencer_ROM(void);
int pka_in_reset(void);
int pka_idle(void);
void start_pka(void);
int wait_for_pka_done(void);
int sequencer_ready(void);
void Clear_PKA_RAM(void);
int
Do_PKA(unsigned int *vectorA, unsigned int vectorA_size, \
       unsigned int *vectorB, unsigned int vectorB_size, \
       unsigned int *vectorC, unsigned int vectorC_size, \
       unsigned int *vectorD, unsigned int vectorD_size, \
       unsigned int shift_count,  unsigned int function, \
       unsigned int flags
       );
int CompareResults(unsigned int *expected, unsigned int *calculated, unsigned int vector_size);
#if 0
/*
 * MODEXP hardware operation
*/

int modexp_powm(SshMPInteger ret,
                 SshMPIntegerConst base,
                 SshMPIntegerConst exponent,
                 SshMPIntegerConst modulus);
#endif
#endif  // __MODEXP_H__

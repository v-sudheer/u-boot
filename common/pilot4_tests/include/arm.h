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

// control register bits

#define ARMintMMUenable                1
#define ARMintMMUfaultenable           2
#define ARMintMMUcacheenable           4
#define ARMintMMUwritebufenable        8
#define ARMintMMU32bitprogram          0x10
#define ARMintMMU32bitdata             0x20
#define ARMintMMUmandatory             0x40
#define ARMintMMUbigendian             0x80
#define ARMintMMUsystem                0x100
#define ARMintMMUROM                   0x200
#define ARMintMMUICache			  1<<12
#define ARMintMMUVbit			  1<<13
#define ARMintMMUnFastbus		  1<<30
#define ARMintMMUiAsync			  1<<31

// CPSR bits

#define CPSR_M0                        1
#define CPSR_M1                        2
#define CPSR_M2                        4
#define CPSR_M3                        8
#define CPSR_M4                        16
#define CPSR_MMASK                     31
#define CPSR_USR                       CPSR_M4
#define CPSR_FIQ                       CPSR_M4 | CPSR_M0
#define CPSR_IRQ                       CPSR_M4 | CPSR_M1
#define CPSR_SVC                       CPSR_M4 | CPSR_M1 | CPSR_M0
#define CPSR_ABT                       CPSR_M4 | CPSR_M2 | CPSR_M1 | CPSR_M0
#define CPSR_UND                       CPSR_M4 | CPSR_M3 | CPSR_M1 | CPSR_M0
#define CPSR_F                         1<<6
#define CPSR_I                         1<<7
#define CPSR_V                         1<<28
#define CPSR_C                         1<<29
#define CPSR_Z                         1<<30
#define CPSR_N                         1<<31




#define User26Mode                   0x0
#define FIQ26Mode                    0x1
#define IRQ26Mode                    0x2
#define SVC26Mode                    0x3

#define User32Mode                   0x10
#define FIQ32Mode                    0x11
#define IRQ32Mode                    0x12
#define SVC32Mode                    0x13
#define Abort32Mode                  0x17
#define Undef32Mode                  0x1b
#define SYSTEMMODE		     0x1f

#define Mask32Mode                   0x1f

#define PSR_32Bit                    0x10
#define ModeMask                     0x0000001f 
#define SubModeMask                  0x0000000f 
#define NoInt                        0x000000c0
#define IFConfig                     0x000000d0
#define CCMask                       0xf0000000


#define MMUCP                         15


#define PageTable1Size               4 * 4 * 1024 // one per megabyte

#define MMU                          1 // 0 is off, 1 is on
#define AlignFault                   0 // 0 is off, 2 is on
#define Cache                        4 // 0 is off, 4 is on
#define WriteBuffer                  8 // 0 is off, 8 is on
#define Config26Bit                  0 // mmu, idc, wb off, 26 bit prog/data, little endian
#define Config32Bit                  0x10 + 0x20 + ARMintMMUnFastbus // mmu, idc, wb off, 32 bit prog/data, little endian
#define EnableICache                 ARMintMMUICache    //Config32Bit + ARMintMMUICache
#define EnableMMU26                  Config26Bit + MMU + AlignFault + Cache + WriteBuffer
#define EnableMMU32                  EnableICache + MMU + AlignFault + Cache + WriteBuffer
#define EnableMMU32Nocache           EnableICache + MMU + AlignFault  // + Cache ; + WriteBuffer
#define NoMMU                        EnableICache + AlignFault



			//.end     

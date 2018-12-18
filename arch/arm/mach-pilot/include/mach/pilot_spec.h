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
#ifndef __PILOT_SPEC_H__
#define __PILOT_SPEC_H__
#define tVU16	volatile unsigned short
#define tVU32	volatile unsigned long
#define tPVU32	volatile unsigned long *
#define tVU8	volatile unsigned char
#define tPVU8	volatile unsigned char *
#define tU16	unsigned short
#define tU32	unsigned long
#define tU8	unsigned char
#include <asm/arch/pilot_map.h>
#include <asm/arch/ddr.h>
//#include <asm/arch/ddr_p4.h>
#include <asm/arch/sysctrl.h>
#define READ_REG8(a)      (*((tPVU8)  (a)))
#define READ_REG16(a)     (*((tPVU16) (a)))
#define READ_REG32(a)     (*((tPVU32) (a)))


#define WRITE_REG8(a,d)   (*((tPVU8) (a)) = d)
#define WRITE_REG16(a,d)  (*((tPVU16) (a)) = d)
#define WRITE_REG32(a,d)  (*((tPVU32) (a)) = d)


#define SET_BIT8(a,b)     (WRITE_REG8 (a, (READ_REG8(a)  | (tU8) (1<<b))))
#define SET_BIT16(a,b)    (WRITE_REG16(a, (READ_REG16(a) | (tU16)(1<<b))))
#define SET_BIT32(a,b)    (WRITE_REG32(a, (READ_REG32(a) | (tU32) (1<<b))))
#define SET_BIT(a,b)      (SET_BIT32(a,b))


#define CLEAR_BIT8(a,b)   (WRITE_REG8 (a, (READ_REG8(a) & (tU8)(~(1<<b)))))
#define CLEAR_BIT16(a,b)  (WRITE_REG16(a, (READ_REG16(a) & (tU16)(~(1<<b)))))
#define CLEAR_BIT32(a,b)  (WRITE_REG32(a, (READ_REG32(a) & (tU32)(~(1<<b)))))
#define CLEAR_BIT(a,b)    (CLEAR_BIT32(a,b))


#define TOGGLE_BIT8(a,b)  (WRITE_REG8 (a, (READ_REG8(a)  ^ (tU8) (1<<b))))
#define TOGGLE_BIT16(a,b) (WRITE_REG16(a, (READ_REG16(a) ^ (tU16)(1<<b))))
#define TOGGLE_BIT32(a,b) (WRITE_REG32(a, (READ_REG32(a) ^ (tU32)(1<<b))))
#define TOGGLE_BIT(a,b)   (TOGGLE_BIT32(a,b))

#endif //__PILOT_SPEC_H__

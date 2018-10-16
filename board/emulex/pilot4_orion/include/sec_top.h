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

/******************************* TOP Control Registers ***************/

typedef struct TOP_CTL_REG 
  {
   volatile unsigned int sec_top_ctl    :8; 
   volatile unsigned int sec_dbg_sel	:3;
   volatile unsigned int rsvd           :21; 
   }TOP_CTL_REG;

typedef struct SEC_CTL 
  {
   volatile unsigned int sec_ctl    :8; 
   volatile unsigned int rsvd1	    :24; 
   }SEC_CTL;

typedef struct SEC_INTSTS 
  {
   volatile unsigned int mexp_int_sts      	:3;  /* [2 : 0] */
   volatile unsigned int gdma_int_sts  		:1;  /* [3 : 3] */
   volatile unsigned int rsvd2			:28; /* [31: 4] */
   }SEC_INTSTS;



typedef union U_TOP_CTL_REG { TOP_CTL_REG s;  tVU32  val; } u_topctlreg;
typedef union U_SEC_CTL { SEC_CTL s;  tVU32  val; } u_secctl;
typedef union U_SEC_INTSTS { SEC_INTSTS s; tVU32  val; } u_secintsts;


typedef struct TOP_REGS 
	{
	 volatile u_topctlreg topctlreg;
	 volatile u_secctl secctl;
	 volatile u_secintsts secintsts;
	 }TOP_REGS;

////// assigning register space to different structures ///////////
#define  SEC_TOP_REGS	        ((volatile TOP_REGS *)(FGB_SECBASE))
#define  SEC_TOP_BASE	        FGB_SECBASE


extern  void  SecIntHandler(void);


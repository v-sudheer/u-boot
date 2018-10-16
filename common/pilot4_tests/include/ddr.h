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

#define MC_REG_ADDRESS00 (DDR_REG_BASE + 0x0)
#define MC_REG_ADDRESS04 (DDR_REG_BASE + 0x04) 
#define MC_REG_ADDRESS08 (DDR_REG_BASE + 0x08)
#define MC_REG_ADDRESS0C (DDR_REG_BASE + 0x0C)
#define MC_REG_ADDRESS10 (DDR_REG_BASE + 0x10)
#define MC_REG_ADDRESS14 (DDR_REG_BASE + 0x14)
#define MC_REG_ADDRESS18 (DDR_REG_BASE + 0x18)
#define MC_REG_ADDRESS1C (DDR_REG_BASE + 0x1C)
#define MC_REG_ADDRESS20 (DDR_REG_BASE + 0x20)
#define MC_REG_ADDRESS24 (DDR_REG_BASE + 0x24)
#define MC_REG_ADDRESS28 (DDR_REG_BASE + 0x28)
#define MC_REG_ADDRESS2C (DDR_REG_BASE + 0x2C)
#define MC_REG_ADDRESS30 (DDR_REG_BASE + 0x30)
#define MC_REG_ADDRESS34 (DDR_REG_BASE + 0x34)
#define MC_REG_ADDRESS38 (DDR_REG_BASE + 0x38)
#define MC_REG_ADDRESS3C (DDR_REG_BASE + 0x3C)
#define MC_REG_ADDRESS40 (DDR_REG_BASE + 0x40)
#define MC_REG_ADDRESS44 (DDR_REG_BASE + 0x44)
#define MC_REG_ADDRESS48 (DDR_REG_BASE + 0x48)
#define MC_REG_ADDRESS4C (DDR_REG_BASE + 0x4C)
#define MC_REG_ADDRESS50 (DDR_REG_BASE + 0x50)
#define MC_REG_ADDRESS54 (DDR_REG_BASE + 0x54)
#define MC_REG_ADDRESS58 (DDR_REG_BASE + 0x58)
#define MC_REG_ADDRESS5C (DDR_REG_BASE + 0x5C)
#define MC_REG_ADDRESS60 (DDR_REG_BASE + 0x60)
#define MC_REG_ADDRESS64 (DDR_REG_BASE + 0x64)
#define MC_REG_ADDRESS68 (DDR_REG_BASE + 0x68)
#define MC_REG_ADDRESS6C (DDR_REG_BASE + 0x6C)  
#define MC_REG_ADDRESS70 (DDR_REG_BASE + 0x70)  

#define PHY_REG_ADDRESS00 (DDR_REG_BASE + 0x8000)
#define PHY_REG_ADDRESS04 (DDR_REG_BASE + 0x8004)

#define APB_REG_ADDRESS00 (DDR_REG_BASE + 0xc000)
#define APB_REG_ADDRESS01 (DDR_REG_BASE + 0xc004)
#define APB_REG_ADDRESS02 (DDR_REG_BASE + 0xc008)
#define APB_REG_ADDRESS03 (DDR_REG_BASE + 0xc00c)
#define APB_REG_ADDRESS04 (DDR_REG_BASE + 0xc010)
#define APB_REG_ADDRESS05 (DDR_REG_BASE + 0xc014)
#define APB_REG_ADDRESS06 (DDR_REG_BASE + 0xc018)
#define APB_REG_ADDRESS07 (DDR_REG_BASE + 0xc01c)
#define APB_REG_ADDRESS10 (DDR_REG_BASE + 0xc040)
#define APB_REG_ADDRESS30 (DDR_REG_BASE + 0xc0c0)
#define APB_REG_ADDRESS32 (DDR_REG_BASE + 0xc0c8)
#define APB_REG_ADDRESS34 (DDR_REG_BASE + 0xc0d0)
#define APB_REG_ADDRESS36 (DDR_REG_BASE + 0xc0d8)
#define APB_REG_ADDRESS38 (DDR_REG_BASE + 0xc0e0)
#define APB_REG_ADDRESS39 (DDR_REG_BASE + 0xc0e4)
#define APB_REG_ADDRESS3A (DDR_REG_BASE + 0xc0e8)
#define APB_REG_ADDRESS3D (DDR_REG_BASE + 0xc0f4)
#define APB_REG_ADDRESS3E (DDR_REG_BASE + 0xc0f8)
#define APB_REG_ADDRESS3F (DDR_REG_BASE + 0xc0fc)
#define APB_REG_ADDRESS43 (DDR_REG_BASE + 0xc10c)
#define APB_REG_ADDRESS44 (DDR_REG_BASE + 0xc110)
#define APB_REG_ADDRESS47 (DDR_REG_BASE + 0xc11c)
#define APB_REG_ADDRESS48 (DDR_REG_BASE + 0xc120)
						
#define MAGM_VALUE       0x007fffff
#define UAGM_VALUE       0x00000000
#if defined (FPGA_DUT)
#define LOWER_ECC_ADDRESS  (Bank1start + 0x2000)
#define UPPER_ECC_ADDRESS  (LOWER_ECC_ADDRESS + 4*1024*1024 - 1)
#else
#define LOWER_ECC_ADDRESS 0x80800000
#define UPPER_ECC_ADDRESS 0x83ffffff
#endif
#define QEMPTY_VALUE      0x081c0000
#define PAGE_OPEN_TIME    0x00004c30
  
#if defined (CLK_SW_FROM_ASRAM)
#define DDR_LPWR_FUNCTION_INSPI   0x20000040
#else
#define DDR_LPWR_FUNCTION_INSPI   0x40
#endif
  
#define DDR_LPWR_FUNCTION_INLMEM  0x10005000
#define DDR_LPWR_LMEM_STACK       0x10002000  
tU32 (* ddr_lpwr)(void);
tU32 ddr_lower_power_mode(tU8, tU8, tU8, tU8, tU8);
void chg_ddrfreq(void);
void ddr_go_to_lp(void);
void ddr_selfrefresh (void);
void ddr_activemode (void);
void DELAY(tU32);
void ddrcfg(tU8);
void prg_gate(tU32);
int gateon_training(void); 
int  redeye_training(void);
extern void Burst_Write(tPVU32 , tU32, tU32);
extern int Burst_Read(tPVU32, tU32, tU32);
extern void Enable_stby_intr_vic(void);
extern void Enable_standbyfi_intr(void);
extern void Wait_For_PLLock(void);
extern void DDR_Pll_In_Standby(void);
extern void DDR_Pll_In_Runmode(void);
extern void Switch_Clk_DDR_Pll(void);
extern void Switch_Clk_DDR_OSC(void);
extern void Pll_In_Standby(void);
extern void Disable_standbyfi_intr(void);
extern void save_and_change_stack_to_intram(tU32);
extern void retrive_and_change_stack_to_ddr(tU32);
extern void put_arm_in_stdby(void);
extern void Disable_MMU_Cache(void);
extern void Enable_MMU_Cache(void);
extern void TCCacheD_922(void);
extern void TCCacheD_926(void);
extern void arm_invalidate_icache(void);
extern int  Burst_Read_LSI_FIFO(int);
extern int  Burst_Read_PHY_TRAINING(int);
void clean_fifo(void);
void ddrcfg_asic(tU8);
void ddrsize_calc(void);

typedef struct 
	{
	 tVU8 mode;
	 tVU8 on;
	 tVU8 keepfreq;
	 tVU8 multof25mhz;
	 tVU8 timeout;
	 tVU8 reg_18_19;
	 tVU8 reg_10_3031;
	 } DdrlpDataType;
#if defined (CLK_SW_FROM_ASRAM)
#define Ddrlp  ((volatile DdrlpDataType *)(0x2001e000))
#else
#define Ddrlp  ((volatile DdrlpDataType *)(DDR_CLK_SW_LMEM_LOW))


#endif


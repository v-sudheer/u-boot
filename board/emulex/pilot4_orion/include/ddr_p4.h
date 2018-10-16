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

#define HM_PVT_CONFIG    (DDR_REG_BASE + 0xC090)
#define HM_DP_IO_VREF_SETTING (DDR_REG_BASE + 0xC094)
#define HM_AP0_IO_SETTING (DDR_REG_BASE + 0xC098)
#define HM_AP1_IO_SETTING (DDR_REG_BASE + 0xC298)
#define HM_AP2_IO_SETTING (DDR_REG_BASE + 0xC498)
#define HM_AP0_IO_PUPD    (DDR_REG_BASE + 0xC09C)
#define HM_AP1_IO_PUPD    (DDR_REG_BASE + 0xC29C)
#define HM_AP2_IO_PUPD    (DDR_REG_BASE + 0xC49C)
#define HM_PHY_CONFIG0   (DDR_REG_BASE + 0xC000)
#define HM_PHY_CONFIG1   (DDR_REG_BASE + 0xC004)
#define HM_PHY_CONFIG2   (DDR_REG_BASE + 0xC080)
#define HM_SM_CONTROL    (DDR_REG_BASE + 0xC084)
#define HM_PHASE_90_SEL0 (DDR_REG_BASE + 0xA000)
#define HM_PHASE_90_SEL1 (DDR_REG_BASE + 0xA004)
#define HM_DP_CONFIG0    (DDR_REG_BASE + 0x8000)
#define HM_DP_CONFIG2    (DDR_REG_BASE + 0x8008)
#define HM_DP0_IO_LPR_CTRL (DDR_REG_BASE + 0xC0B0)
#define HM_DP1_IO_LPR_CTRL (DDR_REG_BASE + 0xC2B0)
#define HM_AP0_IO_LPR_CTRL (DDR_REG_BASE + 0xC0B8)
#define HM_AP1_IO_LPR_CTRL (DDR_REG_BASE + 0xC2B8)
#define HM_AP2_IO_LPR_CTRL (DDR_REG_BASE + 0xC4B8)
#define HM_SMC_DEBUG2     (DDR_REG_BASE + 0xC0D8)
#define HM_DP0_PUPP_DQS_DELAY (DDR_REG_BASE + 0x8010)
#define HM_DP0_NUPP_DQS_DELAY (DDR_REG_BASE + 0x8014)
#define HM_DP0_PLOW_DQS_DELAY (DDR_REG_BASE + 0x8018)
#define HM_DP0_NLOW_DQS_DELAY (DDR_REG_BASE + 0x801C)
#define HM_DP1_PUPP_DQS_DELAY (DDR_REG_BASE + 0x8210)
#define HM_DP1_NUPP_DQS_DELAY (DDR_REG_BASE + 0x8214)
#define HM_DP1_PLOW_DQS_DELAY (DDR_REG_BASE + 0x8218)
#define HM_DP1_NLOW_DQS_DELAY (DDR_REG_BASE + 0x821C)
#define HM_DP0_GATE_UPP_CONTROL (DDR_REG_BASE + 0x8048)
#define HM_DP0_GATE_LOW_CONTROL (DDR_REG_BASE + 0x8058)
#define HM_DP1_GATE_UPP_CONTROL (DDR_REG_BASE + 0x8248)
#define HM_DP1_GATE_LOW_CONTROL (DDR_REG_BASE + 0x8258)
#define HM_DP_READ_DATA (DDR_REG_BASE + 0xC0A4)

#define HM_MISC_CTRL        (DDR_REG_BASE + 0x200)
#define HM_IOBZ_CTRL        (DDR_REG_BASE + 0x204)
#define HM_RDPAT            (DDR_REG_BASE + 0x230)
#define HM_DQSCLOCKSKEW     (DDR_REG_BASE + 0x260)
#define HM_PROGGATEON       (DDR_REG_BASE + 0x290)
#define HM_ADRCLOCK         (DDR_REG_BASE + 0x2B8)
#define HM_DPBASEDDELAY     (DDR_REG_BASE + 0x2E0)
#define HM_TRAINREQ         (DDR_REG_BASE + 0x2E4)
#define HM_ALLOWTRAIN       (DDR_REG_BASE + 0x2E8)
#define HM_DPLOADDELAY      (DDR_REG_BASE + 0x360)
#define HM_DPREADDELAY      (DDR_REG_BASE + 0x364)
#define HM_CORETYPE         (DDR_REG_BASE + 0x3EC)
#define HM_PHYCOMPILE       (DDR_REG_BASE + 0x3F0)
#define HM_PHYSYSTEMCODE    (DDR_REG_BASE + 0x3F4)
#define HM_PHYMODULEVERSION (DDR_REG_BASE + 0x3F8)
#define HM_PHYMODULECODE    (DDR_REG_BASE + 0x3FC)
#define HM_FIFO_DELAY_1     (DDR_REG_BASE + 0x274)
#define HM_FIFO_DELAY_2     (DDR_REG_BASE + 0x278)
#define HM_FIFO_ALIGN       (DDR_REG_BASE + 0x27C)


#define LSI_FIFO_TRAINING   (0x10007c00)
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
void ddrcfg(tU32, tU8);
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
int lsi_phy_fifo_training(void);
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


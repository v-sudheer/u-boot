/*******************************************************************************
 * File Name     : arch/arm/cpu/ast-common/ast-scu.c
 * Author         : Ryan Chen
 * Description   : AST SCU
 *
 * Copyright (C) 2012-2020 ASPEED Technology Inc.  This program is free
 * software; you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 *
 * CLK24M
 *  |
 *  |--> H-PLL -->HCLK
 *  |
 *  |--> M-PLL -xx->MCLK
 *  |
 *  |--> V-PLL1 -xx->DCLK
 *  |
 *  |--> V-PLL2 -xx->D2CLK
 *  |
 *  |--> USB2PHY -->UTMICLK
 *
 *   History      :
 *    1. 2012/08/15 Ryan Chen Create
 *
 ******************************************************************************/
#include <common.h>
#include <asm/processor.h>
#include <asm/io.h>
#include <asm/arch/regs-cam-scu.h>
#include <asm/arch/ast-cam-scu.h>
#include <asm/arch/platform.h>
#include <asm/arch/aspeed.h>

//#define ASPEED_SCU_LOCK
//#define ASPEED_SCU_DEBUG

#ifdef ASPEED_SCU_DEBUG
#define SCUDBUG(fmt, args...) printf("%s() " fmt, __FUNCTION__, ## args)
#else
#define SCUDBUG(fmt, args...)
#endif

#define SCUMSG(fmt, args...) printf(fmt, ## args)

static u32 ast_scu_base = AST_SCU_BASE;

static inline u32 
ast_scu_read(u32 reg)
{
	u32 val;
		
	val = readl(ast_scu_base + reg);
	
	SCUDBUG("ast_scu_read : reg = 0x%08x, val = 0x%08x\n", reg, val);
	
	return val;
}

static inline void
ast_scu_write(u32 val, u32 reg) 
{
	SCUDBUG("ast_scu_write : reg = 0x%08x, val = 0x%08x\n", reg, val);
#ifdef CONFIG_AST_SCU_LOCK
	//unlock 
	writel(SCU_PROTECT_UNLOCK, ast_scu_base);
	writel(val, ast_scu_base + reg);
	//lock
	writel(0xaa,ast_scu_base);	
#else
	writel(SCU_PROTECT_UNLOCK, ast_scu_base);
	writel(val, ast_scu_base + reg);
#endif
}

/* SoC mapping Table */
struct soc_id {
	const char *name;
	u32	   rev_id;
};

#define SOC_ID(str, rev) { .name = str, .rev_id = rev, }

static struct soc_id soc_map_table[] = {
	SOC_ID("AST1220-A0", 0x04030000),
};
//***********************************Initial control***********************************
extern void
ast_scu_init_eth(u8 num)
{
	switch(num) {
		case 0:
			ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_MAC0, 
							AST_SCU_RESET);		
			udelay(100);
			ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & ~SCU_MAC_CLK_STOP_EN, 
							AST_SCU_CLK_STOP);		
			udelay(1000);
			ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_MAC0, 
							AST_SCU_RESET);		
			break;
	}		
}

extern void
ast_scu_init_usb_port1(void)
{
	//reset USB20
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_UDC0, AST_SCU_RESET);

	//enable USB20 clock
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & ~SCU_UDC20_CLK_STOP_EN, AST_SCU_CLK_STOP);
	mdelay(10);

	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_UDC0, AST_SCU_RESET);

	//device mode
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) & ~SCU_FUC_PIN_USB20_HOST, 
				AST_SCU_FUN_PIN_CTRL5);
	
	printf("TODO ast_scu_init_usb_port1 ~\n");
}

extern void
ast_scu_init_sdhci(void)
{
	//SDHCI Host's Clock Enable and Reset
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_SDHCI, AST_SCU_RESET);
	
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & ~SCU_SDHCI_CLK_STOP_EN, AST_SCU_CLK_STOP);
	mdelay(10);

	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_SDHCI, AST_SCU_RESET);
}

extern void
ast_scu_init_i2c(void)
{

	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_I2C, AST_SCU_RESET);
	udelay(3);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_I2C, AST_SCU_RESET);

}

//***********************************CLK Information***********************************
extern u32
ast_get_clk_source(void)
{
	return AST_PLL_24MHZ;
}

extern u32
ast_get_h_pll_clk(void)
{
	u32 clk=0;
	u32 h_pll_set = ast_scu_read(AST_SCU_H_PLL);

	if(h_pll_set & SCU_PLL_OFF)
		return 0;
	
	// Programming
	clk = ast_get_clk_source();
	if(h_pll_set & SCU_PLL_BYPASS) {
		return clk;
	} else {
		//P = SCU24[22:19]
		//N = SCU24[18:13]
		//M = SCU24[12:0]
		//hpll = clkin * [(M+1) /(N+1)] / (P+1)
		clk = ((clk * (SCU_PLL_GET_MNUM(h_pll_set) + 1)) / (SCU_PLL_GET_NNUM(h_pll_set) + 1)) /(SCU_PLL_GET_PNUM(h_pll_set) + 1);
	}
	SCUDBUG("h_pll = %d\n",clk);
	return clk;
}


extern u32
ast_get_m_pll_clk(void)
{
	u32 clk=0;
	u32 m_pll_set = ast_scu_read(AST_SCU_M_PLL);

	if(m_pll_set & SCU_PLL_OFF)
		return 0;
	
	// Programming
	clk = ast_get_clk_source();
	if(m_pll_set & SCU_PLL_BYPASS) {
		return clk;
	} else {
		//P = SCU24[22:19]
		//N = SCU24[18:13]
		//M = SCU24[12:0]
		//hpll = clkin * [(M+1) /(N+1)] / (P+1)
		clk = ((clk * (SCU_PLL_GET_MNUM(m_pll_set) + 1)) / (SCU_PLL_GET_NNUM(m_pll_set) + 1)) /(SCU_PLL_GET_PNUM(m_pll_set) + 1);
	}
	SCUDBUG("h_pll = %d\n",clk);
	return clk;

}


extern u32
ast_get_ahbclk(void)
{
#ifdef CONFIG_FPGA_ASPEED
	return 52000000;
#else
	unsigned int axi_div, ahb_div, hpll;

	hpll = ast_get_h_pll_clk();
	 
 	ahb_div = (SCU_HW_STRAP_GET_AHB_RATIO(ast_scu_read(AST_SCU_HW_STRAP1)) * 2); 
	if(!ahb_div) ahb_div = 4;
	SCUDBUG("HPLL=%d, AXI_Div=%d, AHB_DIV = %d, AHB CLK=%d\n", hpll, axi_div, ahb_div, hpll/ahb_div);	
	return (hpll/ahb_div);
#endif
}

extern u32
ast_get_pclk(void)
{
#ifdef CONFIG_FPGA_ASPEED
		return 52000000;
#else
	unsigned int apb_div, hpll;

	hpll = ast_get_h_pll_clk();
	 
	apb_div = ((SCU_GET_PCLK_DIV(ast_scu_read(AST_SCU_CLK_SEL)) + 1) * 4); 

	SCUDBUG("HPLL=%d, AXI_Div=%d, AHB_DIV = %d, AHB CLK=%d\n", hpll, axi_div, ahb_div, hpll/ahb_div);	
	return (hpll/apb_div);
#endif
}

extern u32
ast_get_d_pll_clk(void)
{

	return 10000000;
}

extern u32
ast_get_sd_clock_src(void)
{
#ifdef CONFIG_FPGA_ASPEED
	return 104000000;
#else
	unsigned int apb_div, hpll;

	hpll = ast_get_h_pll_clk();
	apb_div = SCU_SDHCI_GET_CLK_DIV(ast_scu_read(AST_SCU_CLK_SEL2));
	
	if(apb_div & 0x8) {
		apb_div = ((apb_div & 0x7) + 1) * 2;
	} else {
		apb_div = 1;
	}

	SCUDBUG("HPLL=%d, APB DIV =%d\n", hpll, apb_div);
	return (hpll/apb_div);
#endif
}

extern void
ast_scu_show_system_info (void)
{
	return ;
}


//*********************************** Multi-function pin control ***********************************
extern void
ast_scu_spi_master(u8 mode)
{
}

extern void
ast_scu_multi_func_uart(u8 uart)
{
}

extern void
ast_scu_multi_func_eth(u8 num)
{
}


extern void
ast_scu_multi_func_i2c(u8 bus_no)
{
}	


extern void
ast_scu_multi_func_sdhc_slot(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | SCU_FUC_PIN_SD1 | SCU_FUC_PIN_SD2, 
				AST_SCU_FUN_PIN_CTRL5);						
}	


extern void
ast_scu_multi_nic_switch(u8 enable)
{
		
}

//***********************************Information ***********************************
extern u32
ast_scu_revision_id(void)
{
	int i;
	u32 rev_id = ast_scu_read(AST_SCU_REVISION_ID);
	for(i=0;i<ARRAY_SIZE(soc_map_table);i++) {
		if(rev_id == soc_map_table[i].rev_id)
			break;
	}
	if(i == ARRAY_SIZE(soc_map_table))
		SCUMSG("UnKnow-SOC : %x \n",rev_id);
	else
		SCUMSG("SOC : %4s \n",soc_map_table[i].name);
	
	return rev_id;
}	


extern void
ast_scu_sys_rest_info(void)
{
	u32 rest = ast_scu_read(AST_SCU_SYS_CTRL);

	if(rest & SCU_SYS_EXT_RESET_FLAG) {
		SCUMSG("RST : External \n");
		ast_scu_write(ast_scu_read(AST_SCU_SYS_CTRL) & ~SCU_SYS_EXT_RESET_FLAG, AST_SCU_SYS_CTRL);
	}
	if (rest & SCU_SYS_WDT1_RESET_FLAG) {
		SCUMSG("RST : WDT1 \n");		
		ast_scu_write(ast_scu_read(AST_SCU_SYS_CTRL) & ~SCU_SYS_WDT1_RESET_FLAG, AST_SCU_SYS_CTRL);
	}
#ifdef SCU_SYS_WDT2_RESET_FLAG		
	if (rest & SCU_SYS_WDT2_RESET_FLAG) {
		SCUMSG("RST : WDT2 - 2nd Boot \n");
		ast_scu_write(ast_scu_read(AST_SCU_SYS_CTRL) & ~SCU_SYS_WDT2_RESET_FLAG, AST_SCU_SYS_CTRL);
	}
#endif
#ifdef SCU_SYS_WDT3_RESET_FLAG
	if (rest & SCU_SYS_WDT3_RESET_FLAG) {
		SCUMSG("RST : WDT3 - Boot\n");
		ast_scu_write(ast_scu_read(AST_SCU_SYS_CTRL) & ~SCU_SYS_WDT3_RESET_FLAG, AST_SCU_SYS_CTRL);
	}
#endif		
	if (rest & SCU_SYS_PWR_RESET_FLAG) {
		SCUMSG("RST : Power On \n");
		ast_scu_write(ast_scu_read(AST_SCU_SYS_CTRL) & ~SCU_SYS_PWR_RESET_FLAG, AST_SCU_SYS_CTRL);
	}
}	


/*
* D[15:11] in 0x1E6E2040 is NCSI scratch from U-Boot. D[15:14] = MAC1, D[13:12] = MAC2
* The meanings of the 2 bits are:
* 00(0): Dedicated PHY
* 01(1): ASPEED's EVA + INTEL's NC-SI PHY chip EVA
* 10(2): ASPEED's MAC is connected to NC-SI PHY chip directly
* 11: Reserved
*/

extern u32
ast_scu_get_phy_config(u8 mac_num)
{
	u32 scatch = ast_scu_read(AST_SCU_SOC_SCRATCH0);

	switch(mac_num) {
		case 0:
			return (SCU_MAC0_GET_PHY_MODE(scatch));
			break;
		case 1:
			return (SCU_MAC1_GET_PHY_MODE(scatch));
			break;
		default:
			SCUMSG("error mac number \n");
			break;
	}
	return -1;
}

extern u32
ast_scu_get_phy_interface(u8 mac_num)
{
	u32 trap1 = ast_scu_read(AST_SCU_HW_STRAP1);

	switch(mac_num) {
		case 0:
			if(SCU_HW_STRAP_MAC0_RGMII & trap1)
				return 1;
			else
				return 0;
			break;
		case 1:
			if(SCU_HW_STRAP_MAC1_RGMII & trap1)
				return 1;
			else
				return 0;
			break;
		default:
			SCUMSG("error mac number \n");
			break;
	}
	return -1;
}


extern int
ast_scu_2nd_wdt_mode(void)
{
	if(ast_scu_read(AST_SCU_HW_STRAP1) & SCU_HW_STRAP_2ND_BOOT_WDT)
		return 1;
	else 
		return 0;
}

extern void
ast_scu_hw_random_enable(u8 enable)
{
	if(enable)
		ast_scu_write(ast_scu_read(AST_SCU_RAMDOM_GEN) | RNG_ENABLE, AST_SCU_RAMDOM_GEN);
	else
		ast_scu_write(ast_scu_read(AST_SCU_RAMDOM_GEN) & ~RNG_ENABLE, AST_SCU_RAMDOM_GEN);
}

extern u32
ast_scu_hw_random_read(void)
{
	return (ast_scu_read(AST_SCU_RAMDOM_DATA));
}

extern u8
ast_scu_get_hw_random_type(void)
{
	return (RNG_GET_TYPE(ast_scu_read(AST_SCU_RAMDOM_GEN)));
}

extern void
ast_scu_set_hw_random_type(u8 type)
{
	ast_scu_write(((ast_scu_read(AST_SCU_RAMDOM_GEN) & ~RNG_TYPE_MASK) | RNG_SET_TYPE(type)), AST_SCU_RAMDOM_GEN);
}

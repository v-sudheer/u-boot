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
#include <asm/arch/regs-bmc-scu.h>
#include <asm/arch/ast-bmc-scu.h>
#include <asm/arch/platform.h>
#include <asm/arch/clk_aspeed.h>

//#define ASPEED_SCU_LOCK
//#define ASPEED_SCU_DEBUG

#ifdef ASPEED_SCU_DEBUG
#define SCUDBUG(fmt, args...) printf("%s() " fmt, __FUNCTION__, ## args)
#else
#define SCUDBUG(fmt, args...)
#endif

#define SCUMSG(fmt, args...) printf(fmt, ## args)

static u32 ast_scu_base = ASPEED_SCU_BASE;

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

//***********************************Initial control***********************************
#ifdef SCU_RESET_VIDEO
extern void
ast_scu_reset_video(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_VIDEO, AST_SCU_RESET);
	udelay(100);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_VIDEO, AST_SCU_RESET);
}

extern void
ast_scu_init_video(u8 dynamic_en)
{
	//Video Engine Clock Enable and Reset
	//  Enable Clock & ECLK = inverse of (M-PLL / 2)
	if(dynamic_en)
		ast_scu_write((ast_scu_read(AST_SCU_CLK_SEL) & ~SCU_CLK_VIDEO_SLOW_MASK) | SCU_CLK_VIDEO_SLOW_EN | SCU_CLK_VIDEO_SLOW_SET(0), AST_SCU_CLK_SEL);
	else {
		if(GET_CHIP_REVISION(ast_scu_read(AST_SCU_REVISION_ID)) == 4)
			ast_scu_write((ast_scu_read(AST_SCU_CLK_SEL) & ~(SCU_ECLK_SOURCE_MASK | SCU_CLK_VIDEO_SLOW_MASK | SCU_CLK_VIDEO_SLOW_EN)), AST_SCU_CLK_SEL);
		else
			ast_scu_write((ast_scu_read(AST_SCU_CLK_SEL) & ~(SCU_ECLK_SOURCE_MASK | SCU_CLK_VIDEO_SLOW_EN)) | SCU_ECLK_SOURCE(2), AST_SCU_CLK_SEL);
	}
	
	// Enable CLK
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & ~(SCU_ECLK_STOP_EN | SCU_VCLK_STOP_EN), AST_SCU_CLK_STOP);	
	mdelay(10);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_VIDEO, AST_SCU_RESET);
	udelay(100);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_VIDEO, AST_SCU_RESET);
}
#endif

#ifdef SCU_UART1CLK_STOP_EN
extern void
ast_scu_init_uart(u8 uart)
{
	u32 clk_stop_en = 0;	

	//uart 1
	if(uart & 0x2) {
		clk_stop_en |= SCU_UART1CLK_STOP_EN;
	}

	if(uart & 0x4) {
		clk_stop_en |= SCU_UART2CLK_STOP_EN;
	}

	if(uart & 0x8) {
		clk_stop_en |= SCU_UART3CLK_STOP_EN;
	}

	if(uart & 0x10) {
		clk_stop_en |= SCU_UART4CLK_STOP_EN;
	}
	
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & ~(clk_stop_en), AST_SCU_CLK_STOP);
	
}
#endif

#ifdef SCU_RESET_USB11
extern void
ast_scu_init_uhci(void)
{
	//USB1.1 Host's Clock Enable and Reset
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & ~SCU_USB11CLK_STOP_EN, AST_SCU_CLK_STOP);
	mdelay(10);

	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_USB11, AST_SCU_RESET);
}
#endif

#ifdef SCU_RESET_USB20
extern void
ast_scu_init_usb_port1(void)
{
	/* EHCI controller engine init. Process similar to VHub. */
	/* Following reset sequence can resolve "vhub dead on first power on" issue on V4 board. */
	//reset USB20
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_USB20, AST_SCU_RESET);

	//enable USB20 clock
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) | SCU_USB20_PHY_CLK_EN, AST_SCU_CLK_STOP);
	mdelay(10);

	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_USB20, AST_SCU_RESET);
}

extern void
ast_scu_init_usb_port2(void)
{
#ifdef CONFIG_MACH_ASPEED_G5
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_USB_P1, AST_SCU_RESET);

	//enable USB20 clock
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & ~SCU_USB_P1_STOP_EN, AST_SCU_CLK_STOP);
	mdelay(10);

	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_USB_P1, AST_SCU_RESET);
#endif
}
#endif

extern void
ast_scu_init_pwm_tacho(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_PWM, AST_SCU_RESET);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_PWM, AST_SCU_RESET);
}


extern void
ast_scu_init_adc(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_ADC, AST_SCU_RESET);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_ADC, AST_SCU_RESET);
}

#ifdef SCU_RESET_PCIE
extern void
ast_scu_init_pcie(void)
{
	//pcie host reset 
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_PCIE, AST_SCU_RESET);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_PCIE_DIR, AST_SCU_RESET);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_PCIE, AST_SCU_RESET);
	mdelay(10);	
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_PCIE, AST_SCU_RESET);

	//p2x reset
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_P2X, AST_SCU_RESET);

	//use 0x7c for clr 
	ast_scu_write(SCU_HW_STRAP_VGA_MASK, AST_SCU_REVISION_ID);
	ast_scu_write(SCU_HW_STRAP_VGA_SIZE_SET(VGA_64M_DRAM), AST_SCU_HW_STRAP1);

	ast_scu_write(ast_scu_read(AST_SCU_MISC2_CTRL) | SCU_PCIE_MAPPING_HIGH | SCU_MALI_RC_MODE | SCU_MALI_DTY_MODE, AST_SCU_MISC2_CTRL);	
}
#endif

#ifdef SCU_RESET_MCTP
extern void
ast_scu_init_mctp(void)
{
	//Notice : Must after Host reset 
//	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_MCTP, AST_SCU_RESET);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_MCTP, AST_SCU_RESET);
}
#endif

#ifdef SCU_RESET_XDMA
extern void
ast_scu_init_xdma(void)
{
	//Notice : 1. Must after Host reset, 2. DRAM Controller 0x08 memory protection must disable [Protect REQ#] 3. VGA PCI Bus master enable offset 0x04[bit2]
//	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_XDMA, AST_SCU_RESET);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_XDMA, AST_SCU_RESET);
}
#endif

extern void
ast_scu_init_peci(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_PECI, AST_SCU_RESET);
	udelay(3);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_PECI, AST_SCU_RESET);
}


extern void
ast_scu_init_jtag(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_JTAG, AST_SCU_RESET);
	udelay(3);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_JTAG, AST_SCU_RESET);
}

#ifdef SCU_RESET_HACE
extern void
ast_scu_init_hace(void)
{
	//enable YCLK for HAC
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) &
					~(SCU_YCLK_STOP_EN | SCU_RSACLK_STOP_EN), 
					AST_SCU_CLK_STOP);
	mdelay(1);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) &
					~SCU_RESET_HACE, 
					AST_SCU_RESET);
}
#endif

extern void
ast_scu_reset_espi(void)
{
	//Note .. It have been enable in U-boot..... 
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_ESPI, AST_SCU_RESET);	

	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_ESPI, AST_SCU_RESET);
}


extern void
ast_scu_reset_lpc(void)
{
	//Note .. It have been enable in U-boot..... 
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_LPC, AST_SCU_RESET);	

	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_LPC, AST_SCU_RESET);
}

#define SCU_LHCLK_SOURCE_EN			BIT(19)
#define SCU_SET_LHCLK_DIV(x)		(x << 20)

extern void
ast_scu_init_lpc(void)
{
	//Note .. It have been enable in U-boot..... 
//	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_LPC, AST_SCU_RESET);

	//enable LPC clock LHCLK = H-PLL/8, SOC_G5 H-PLL/16
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) | 
					SCU_SET_LHCLK_DIV(3) | 
					SCU_LHCLK_SOURCE_EN, 
					AST_SCU_CLK_STOP);
}


//////1 : lpc plus modes
extern u8
ast_scu_get_lpc_plus_enable(void)
{
	if(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) & SCU_FUN_PIN_LPC_PLUS)
		return 1;
	else 
		return 0;
}


#if 0
extern void
ast_scu_init_rfx(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_RFX, AST_SCU_RESET);
	ast_scu_write(ast_scu_read(AST_SCU_RESET2) | (SCU_RESET_RFXDEC | SCU_RESET_RFXCMQ | SCU_RESET_BITBLT), AST_SCU_RESET2);	

	//Use D1-PLL
	ast_scu_write((ast_scu_read(AST_SCU_CLK_SEL) & ~(SCU_ECLK_SOURCE_MASK | SCU_CLK_VIDEO_SLOW_MASK | SCU_CLK_VIDEO_SLOW_EN)), AST_SCU_CLK_SEL);
	ast_scu_write(ast_scu_read(AST_SCU_CLK_SEL) | SCU_ECLK_SOURCE(2), AST_SCU_CLK_SEL);
	
	ast_scu_write((ast_scu_read(AST_SCU_MISC1_CTRL) & ~SCU_MISC_D_PLL_DIS) | SCU_MISC_D_PLL_SOURCE, AST_SCU_MISC1_CTRL);

	ast_scu_write(0x75402031, AST_SCU_D_PLL);
	ast_scu_write(0x00000580, AST_SCU_DPLL_PAR0);
	ast_scu_write(0x00000000, AST_SCU_DPLL_PAR1);
	ast_scu_write(0x0004AB1C, AST_SCU_DPLL_PAR2);
	
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & 	~(SCU_RFX_CLK_STOP_EN), AST_SCU_CLK_STOP);	
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP2) & ~(SCU_CMQCLK_STOP | SCU_RFXCLK_STOP | SCU_BITBLTCLK_STOP), AST_SCU_CLK_STOP2);	
	udelay(3);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_RFX, AST_SCU_RESET);
	ast_scu_write(ast_scu_read(AST_SCU_RESET2) & ~(SCU_RESET_RFXDEC | SCU_RESET_RFXCMQ | SCU_RESET_BITBLT), AST_SCU_RESET2);	

	//Multi fun pin
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL6) | SCU_FUN_PIN_DVO_24BIT, AST_SCU_FUN_PIN_CTRL6); 
	
}
#else
extern void
ast_scu_init_rfx(void)
{
}
#endif

#ifdef SCU_RESET_H264
extern void
ast_scu_init_h264(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_H264, AST_SCU_RESET);
	ast_scu_write(ast_scu_read(AST_SCU_CLK_SEL2) & 	~SCU_H264CLK_MASK, AST_SCU_CLK_SEL2);
	ast_scu_write(ast_scu_read(AST_SCU_CLK_SEL2) | SCU_SET_H264CLK_DIV(3), AST_SCU_CLK_SEL2);	
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & 	~SCU_H264_STOP_EN, AST_SCU_CLK_STOP);
	udelay(3);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_H264, AST_SCU_RESET);
}
#endif

/* 0:disable spi 1: enable spi master 2:enable spi master and spi slave to ahb 3: enable spi pass-through*/
extern void
ast_scu_spi_master(u8 mode)
{
#ifdef CONFIG_MACH_ASPEED_G5
	switch(mode) {
		case 0:
			ast_scu_write(SCU_HW_STRAP_SPI_MODE_MASK, AST_SCU_REVISION_ID);
			break;
		case 1:
			ast_scu_write(SCU_HW_STRAP_SPI_MODE_MASK, AST_SCU_REVISION_ID);
			ast_scu_write(SCU_HW_STRAP_SPI_MASTER, AST_SCU_HW_STRAP1);
			break;
		case 2:
			ast_scu_write(SCU_HW_STRAP_SPI_MODE_MASK, AST_SCU_REVISION_ID);
			ast_scu_write(SCU_HW_STRAP_SPI_M_S_EN, AST_SCU_HW_STRAP1);
			break;
		case 3:
			ast_scu_write(SCU_HW_STRAP_SPI_MODE_MASK, AST_SCU_REVISION_ID);
			ast_scu_write(SCU_HW_STRAP_SPI_PASS_THROUGH, AST_SCU_HW_STRAP1);			
			break;
	}
#else
	switch(mode) {
		case 0:			
			ast_scu_write(ast_scu_read(AST_SCU_HW_STRAP1) & ~SCU_HW_STRAP_SPI_MODE_MASK, AST_SCU_HW_STRAP1);
			break;
		case 1:
			ast_scu_write((ast_scu_read(AST_SCU_HW_STRAP1) & ~SCU_HW_STRAP_SPI_MODE_MASK) |SCU_HW_STRAP_SPI_MASTER, AST_SCU_HW_STRAP1);
			break;
		case 2:
			ast_scu_write((ast_scu_read(AST_SCU_HW_STRAP1) & ~SCU_HW_STRAP_SPI_MODE_MASK) |SCU_HW_STRAP_SPI_MASTER, AST_SCU_HW_STRAP1);
			break;
		case 3:
			ast_scu_write((ast_scu_read(AST_SCU_HW_STRAP1) & ~SCU_HW_STRAP_SPI_MODE_MASK) |SCU_HW_STRAP_SPI_PASS_THROUGH, AST_SCU_HW_STRAP1);
			break;
	}

#endif
}


#ifdef SCU_RESET_CRT
extern void
ast_scu_init_crt(void)
{
	//ast2400 : VGA use D1 clk, CRT use D2 clk
	//ast2500 : VGA use D1 clk, CRT use 40Mhz 
	//ast3200/ast1520 : VGA use D1 clk, CRT use D1/D2 clk select L: SCU08[bit 8] - H SCU2C[bit 21]

#ifdef CONFIG_MACH_ASPEED_G5
	
#ifdef CONFIG_ARCH_AST3200
	//Select D2 CLK source 00:D-PLL, 01: D2-PLL, 1x : 40Mhz
	//H: 2c[bit : 21], L: 08[bit : 8]
	//Select D2-PLL parameter source [01]
	ast_scu_write(ast_scu_read(AST_SCU_CLK_SEL) | SCU_CRT_CLK_L_SOURCE , AST_SCU_CLK_SEL);
	ast_scu_write(ast_scu_read(AST_SCU_MISC1_CTRL) & ~SCU_MISC_CRT_CLK_H_SOURCE , AST_SCU_MISC1_CTRL);
	
	//Off D2-PLL
//	ast_scu_write(ast_scu_read(AST_SCU_D2_PLL_EXTEND) |  SCU_D2_PLL_OFF | SCU_D2_PLL_RESET , AST_SCU_D2_PLL_EXTEND);
	ast_scu_write(0x585, AST_SCU_D2_PLL_EXTEND);

	//set D2-PLL parameter 
	ast_scu_write((0x15 << 27) | (0xE << 22) | (0x03D << 13) | (0x40), AST_SCU_D2_PLL);

	//enable D2-PLL
//	ast_scu_write(ast_scu_read(AST_SCU_D2_PLL_EXTEND) &  ~(SCU_D2_PLL_OFF | SCU_D2_PLL_RESET) , AST_SCU_D2_PLL_EXTEND);
	ast_scu_write(0x580, AST_SCU_D2_PLL_EXTEND);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_CRT, AST_SCU_RESET);	

	ast_scu_write(ast_scu_read(AST_SCU_RESET2) & ~(SCU_RESET_CRT0 | SCU_RESET_CRT1 | SCU_RESET_CRT2 | SCU_RESET_CRT3), AST_SCU_RESET2);

	//For DVO output timing
	ast_scu_write((ast_scu_read(AST_SCU_CLK_SEL2) & SCU_VIDEO1_OUTPUT_CLK_DELAY_MASK) | SCU_VIDEO1_OUTPUT_CLK_DELAY(5), AST_SCU_CLK_SEL2);	

	//enable CRT CLK
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & ~SCU_D2CLK_STOP_EN , AST_SCU_CLK_STOP);
	
#else
	//ast2500 use 40Mhz (init @ platform.S)
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_CRT, AST_SCU_RESET);	

	ast_scu_write(ast_scu_read(AST_SCU_RESET2) & ~SCU_RESET_CRT0, AST_SCU_RESET2);	
#endif

	ast_scu_write(0x1df, 0xd4); 

#else
	//SOC VER < G5
	/* Enable D2 - PLL */
	ast_scu_write(ast_scu_read(AST_SCU_MISC1_CTRL) & ~SCU_MISC_D2_PLL_DIS, AST_SCU_MISC1_CTRL);

	/* Reset CRT */ 
	ast_scu_write(ast_scu_read(AST_SCU_RESET) | SCU_RESET_CRT, AST_SCU_RESET);
	
	/* Set Delay 5 Compensation TODO ...*/
	ast_scu_write((ast_scu_read(AST_SCU_CLK_SEL) & ~SCU_CLK_VIDEO_DELAY_MASK) |
						SCU_CLK_VIDEO_DELAY(5), AST_SCU_CLK_SEL);

	//enable D2 CLK
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP) & ~SCU_D2CLK_STOP_EN , AST_SCU_CLK_STOP);

	udelay(10);
	ast_scu_write(ast_scu_read(AST_SCU_RESET) & ~SCU_RESET_CRT, AST_SCU_RESET);
	
#endif

	
}
#endif

extern void
ast_scu_uartx_init(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP2) &
				~(SCU_UART_DIV13 | SCU_UARTXCLK_STOP),
		AST_SCU_CLK_STOP2); 
	
}


//***********************************CLK control***********************************
extern void
ast_scu_uart_div(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_CLK_STOP2) &
				~SCU_UART_DIV13,
		AST_SCU_CLK_STOP2); 
}


extern void
ast_scu_clk_stop(u32 clk_name,u8 stop_enable)
{
	switch(clk_name){
		default:
			SCUMSG("ERRO clk_name :%d \n",clk_name);
			break;
	}
}

//***********************************CLK Information***********************************
extern void
ast_scu_osc_clk_output(void)
{
	//in ast3200 for usb audio code clock
//	if (!(ast_scu_read(AST_SCU_HW_STRAP1) & CLK_25M_IN))
//	{
		ast_scu_write(ast_scu_read(AST_SCU_MISC1_CTRL) | SCU_MISC_OSC_CLK_OUT_PIN, AST_SCU_MISC1_CTRL);
		ast_scu_write((ast_scu_read(AST_SCU_COUNT_CTRL) & ~SCU_FREQ_SOURCE_FOR_MEASU_MASK) | SCU_FREQ_SOURCE_FOR_MEASU(SCU_FREQ_SOURCE_FOR_MEASU_12MHZ), AST_SCU_COUNT_CTRL);
//	}
}

extern void
ast_scu_set_lpc_mode(void)
{
#ifdef CONFIG_MACH_ASPEED_G5
	ast_scu_write(SCU_HW_STRAP_ESPI_MODE , AST_SCU_REVISION_ID);
#endif
}

//*********************************** Multi-function pin control ***********************************
extern void
ast_scu_multi_func_uart(u8 uart)
{
	switch(uart) {
		case 1:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL2) |
						SCU_FUN_PIN_UART1_RXD |
						SCU_FUN_PIN_UART1_TXD |
						SCU_FUN_PIN_UART1_NRTS |
						SCU_FUN_PIN_UART1_NDTR |
						SCU_FUN_PIN_UART1_NRI |
						SCU_FUN_PIN_UART1_NDSR |
						SCU_FUN_PIN_UART1_NDCD |
						SCU_FUN_PIN_UART1_NCTS, 
				AST_SCU_FUN_PIN_CTRL2); 
			break;		
		case 2:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL2) |
						SCU_FUN_PIN_UART2_RXD |
						SCU_FUN_PIN_UART2_TXD |
						SCU_FUN_PIN_UART2_NRTS |
						SCU_FUN_PIN_UART2_NDTR |
						SCU_FUN_PIN_UART2_NRI |
						SCU_FUN_PIN_UART2_NDSR |
						SCU_FUN_PIN_UART2_NDCD |
						SCU_FUN_PIN_UART2_NCTS, 
				AST_SCU_FUN_PIN_CTRL2); 
			break;		
		case 3:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL1) |
						SCU_FUN_PIN_UART3_RXD |
						SCU_FUN_PIN_UART3_TXD |
						SCU_FUN_PIN_UART3_NRTS |
						SCU_FUN_PIN_UART3_NDTR |
						SCU_FUN_PIN_UART3_NRI |
						SCU_FUN_PIN_UART3_NDSR |
						SCU_FUN_PIN_UART3_NDCD |
						SCU_FUN_PIN_UART3_NCTS, 
				AST_SCU_FUN_PIN_CTRL1); 
			break;
		case 4:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL1) |
						SCU_FUN_PIN_UART4_RXD |
						SCU_FUN_PIN_UART4_TXD |
						SCU_FUN_PIN_UART4_NRTS |
						SCU_FUN_PIN_UART4_NDTR |
						SCU_FUN_PIN_UART4_NRI |
						SCU_FUN_PIN_UART4_NDSR |
						SCU_FUN_PIN_UART4_NDCD |
						SCU_FUN_PIN_UART4_NCTS, 
				AST_SCU_FUN_PIN_CTRL1); 			
			break;
		case 6:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) |
						SCU_FUC_PIN_UART6, 
				AST_SCU_FUN_PIN_CTRL5); 
			break;
#ifdef CONFIG_MACH_ASPEED_G5			
		case 7:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL6) |
						SCU_FUN_PIN_UART7,
				AST_SCU_FUN_PIN_CTRL6); 
			break;
		case 8:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL6) |
						SCU_FUN_PIN_UART8,
				AST_SCU_FUN_PIN_CTRL6); 
			break;
		case 9:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL6) |
						SCU_FUN_PIN_UART9,
				AST_SCU_FUN_PIN_CTRL6); 
			break;
		case 10:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL2) &
						~(SCU_FUN_PIN_VGAVS | SCU_FUN_PIN_VGAHS),
				AST_SCU_FUN_PIN_CTRL2); 
			
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL6) |
						SCU_FUN_PIN_UART10,
				AST_SCU_FUN_PIN_CTRL6); 
			break;
		case 11:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL2) &
						~(SCU_FUN_PIN_DDCDAT | SCU_FUN_PIN_DDCCLK),
				AST_SCU_FUN_PIN_CTRL2); 
			
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL6) |
						SCU_FUN_PIN_UART11,
				AST_SCU_FUN_PIN_CTRL6); 
			break;
		case 12:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL6) |
						SCU_FUN_PIN_UART12,
				AST_SCU_FUN_PIN_CTRL6); 
			break;
		case 13:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL6) |
						SCU_FUN_PIN_UART13,
				AST_SCU_FUN_PIN_CTRL6); 
			break;
#endif
	}


}

extern void
ast_scu_multi_func_video(void)
{
#if defined(CONFIG_ARCH_2100) || defined(CONFIG_ARCH_2200)
	ast_scu_write(ast_scu_read(AST_SCU_MULTI_FUNC_2) |
				MULTI_FUNC_VIDEO_RGB18 |
				MULTI_FUNC_VIDEO_SINGLE_EDGE, 
		AST_SCU_MULTI_FUNC_2); 
#elif defined(CONFIG_ARCH_1100) || defined(CONFIG_ARCH_2050)
	ast_scu_write(ast_scu_read(AST_SCU_MULTI_FUNC_2) |
				MULTI_FUNC_VIDEO_RGB18 |
				MULTI_FUNC_VIDEO_SINGLE_EDGE, 
		AST_SCU_MULTI_FUNC_2); 
#else

#endif
}

extern void
ast_scu_multi_func_nand(void)
{
#ifdef CONFIG_MACH_ASPEED_G5
#else
	//enable NAND flash multipin FLBUSY and FLWP
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL2) | 
				SCU_FUN_PIN_NAND_FLBUSY | SCU_FUN_PIN_NAND_FLWP, 
		AST_SCU_FUN_PIN_CTRL2); 
#endif

}

#if 0
extern void
ast_scu_multi_func_nor(void)
{
	//Address 
	//ROMA2~17
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL8) | 
				SCU_FUN_PIN_ROMA2 | SCU_FUN_PIN_ROMA3 |
				SCU_FUN_PIN_ROMA4 | SCU_FUN_PIN_ROMA5 |
				SCU_FUN_PIN_ROMA6 | SCU_FUN_PIN_ROMA7 |
				SCU_FUN_PIN_ROMA8 | SCU_FUN_PIN_ROMA9 |
				SCU_FUN_PIN_ROMA10 | SCU_FUN_PIN_ROMA11 |
				SCU_FUN_PIN_ROMA12 | SCU_FUN_PIN_ROMA13 |
				SCU_FUN_PIN_ROMA14 | SCU_FUN_PIN_ROMA15 |
				SCU_FUN_PIN_ROMA16 | SCU_FUN_PIN_ROMA17, 
		AST_SCU_FUN_PIN_CTRL8);

	//ROMA18~21
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL9) | 
				SCU_FUN_PIN_ROMA18 | SCU_FUN_PIN_ROMA19 |
				SCU_FUN_PIN_ROMA20 | SCU_FUN_PIN_ROMA21, 
		AST_SCU_FUN_PIN_CTRL9);	
	
	//ROMA22,23
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL4) | SCU_FUN_PIN_ROMA22 | SCU_FUN_PIN_ROMA23, 
		AST_SCU_FUN_PIN_CTRL4);
	
	//ROMA24,25
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL3) | SCU_FUN_PIN_ROMA24 | SCU_FUN_PIN_ROMA25, 
		AST_SCU_FUN_PIN_CTRL3);

	//SCU94 [1] = 0
 	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL6) & SCU_VIDEO_OUT_MASK, 
		AST_SCU_FUN_PIN_CTRL6);

	
	//data
	//ROMD 4~7 //ROMWE#, OE#
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL4) | 
			SCU_FUN_PIN_ROMOE | SCU_FUN_PIN_ROMWE |
			SCU_FUN_PIN_ROMD4 | SCU_FUN_PIN_ROMD5 |
			SCU_FUN_PIN_ROMD6 | SCU_FUN_PIN_ROMD7,
			AST_SCU_FUN_PIN_CTRL4);
	
	//ROMD 8~15
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | 
			SCU_FUC_PIN_ROM_16BIT, 
		AST_SCU_FUN_PIN_CTRL5);

}
#endif

extern void
ast_scu_multi_func_romcs(u8 num)
{
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL3) | 
			SCU_FUN_PIN_ROMCS(num), 
		AST_SCU_FUN_PIN_CTRL3);
}

extern void
ast_scu_multi_func_pwm_tacho(void)
{
	u32 sts = 0;

#ifdef CONFIG_ARCH_AST1010
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL2) |
					SCU_FUN_PIN_PWM1 |
					SCU_FUN_PIN_PWM2 |
					SCU_FUN_PIN_PWM3 |
					SCU_FUN_PIN_PWM4 |
					SCU_FUN_PIN_TACH1 |
					SCU_FUN_PIN_TACH2 |
					SCU_FUN_PIN_TACH3 |
					SCU_FUN_PIN_TACH4 |
					SCU_FUN_PIN_TACH5 |
					SCU_FUN_PIN_TACH6 |
					SCU_FUN_PIN_TACH7 |
					SCU_FUN_PIN_TACH8,
			AST_SCU_FUN_PIN_CTRL2);

	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL1) |
					SCU_FUN_PIN_PWM5 |
					SCU_FUN_PIN_PWM6 |
					SCU_FUN_PIN_PWM7 |
					SCU_FUN_PIN_PWM8 |
					SCU_FUN_PIN_TACH9 |
					SCU_FUN_PIN_TACH10 |
					SCU_FUN_PIN_TACH11 |
					SCU_FUN_PIN_TACH12,
			AST_SCU_FUN_PIN_CTRL1);
#else
	sts = ast_scu_read(AST_SCU_FUN_PIN_CTRL3) &~0xcfffff;
	ast_scu_write(sts | 0xc000ff, AST_SCU_FUN_PIN_CTRL3);

#endif	
}	


//0 : usb 2.0 hub mode, 1:usb 2.0 host2 controller
extern void
ast_scu_multi_func_usb_port1_mode(u8 mode)
{
	if(mode)
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | SCU_FUC_PIN_USB20_HOST, 
					AST_SCU_FUN_PIN_CTRL5);
	else
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) & ~SCU_FUC_PIN_USB20_HOST, 
					AST_SCU_FUN_PIN_CTRL5);
}	


//0 : 1.1 hid 1, 1.1 host , 2, 2.0 host 3, 2.0 device 
extern void
ast_scu_multi_func_usb_port2_mode(u8 mode)
{
#if defined(CONFIG_MACH_ASPEED_G5)
	if(mode == 0)
		ast_scu_write((ast_scu_read(AST_SCU_FUN_PIN_CTRL6) & ~SCU_FUN_PIN_USBP1_MASK), 
					AST_SCU_FUN_PIN_CTRL6);
	else if ((mode == 1) || (mode == 2))
		ast_scu_write((ast_scu_read(AST_SCU_FUN_PIN_CTRL6) & ~SCU_FUN_PIN_USBP1_MASK) |
					SCU_FUN_PIN_USBP1_MODE(0x2), 
					AST_SCU_FUN_PIN_CTRL6);
	else if (mode == 3)
		ast_scu_write((ast_scu_read(AST_SCU_FUN_PIN_CTRL6) & ~SCU_FUN_PIN_USBP1_MASK) |
					SCU_FUN_PIN_USBP1_MODE(0x1), 
					AST_SCU_FUN_PIN_CTRL6);
	else {
		printf("nothing\n");
	}
#else
	if(mode == 0)
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) & ~SCU_FUC_PIN_USB11_PORT2, 
					AST_SCU_FUN_PIN_CTRL5);
	else if (mode == 1)
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | SCU_FUC_PIN_USB11_PORT2, 
			AST_SCU_FUN_PIN_CTRL5);
	else if (mode == 2)
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | SCU_FUC_PIN_USB20_HOST, 
					AST_SCU_FUN_PIN_CTRL5);
	else {
		printf("nothing\n");
	}
#endif	
}	


//0 : gpioQ6,7 mode , 1: usb1.1 host port 4 mode
extern void
ast_scu_multi_func_usb_port34_mode(u8 mode)
{
#if defined(CONFIG_MACH_ASPEED_G5)
	if(mode) {
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | SCU_FUC_PIN_USB11_PORT4, 
					AST_SCU_FUN_PIN_CTRL5);
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL3) | 
					(SCU_FUN_PIN_USBP3_DP |SCU_FUN_PIN_USBP3_DN | SCU_FUN_PIN_USBP4_DP | SCU_FUN_PIN_USBP4_DN),
					AST_SCU_FUN_PIN_CTRL3);		
	} else {
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) & ~SCU_FUC_PIN_USB11_PORT4, 
					AST_SCU_FUN_PIN_CTRL5);
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL3) & 
					~(SCU_FUN_PIN_USBP3_DP |SCU_FUN_PIN_USBP3_DN | SCU_FUN_PIN_USBP4_DP | SCU_FUN_PIN_USBP4_DN),
					AST_SCU_FUN_PIN_CTRL3);		
	}
#else
	if(mode) {
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | SCU_FUC_PIN_USB11_PORT4, 
					AST_SCU_FUN_PIN_CTRL5);
	} else {
		ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) & ~SCU_FUC_PIN_USB11_PORT4, 
					AST_SCU_FUN_PIN_CTRL5);
	}
#endif
}	

extern void
ast_scu_multi_func_sdhc_slot(u8 slot)
{
	switch(slot) {
		case 1:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | SCU_FUC_PIN_SD1, 
						AST_SCU_FUN_PIN_CTRL5);						
			break;
		case 2:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | SCU_FUC_PIN_SD2, 
						AST_SCU_FUN_PIN_CTRL5);									
			break;
		case 3:
			ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL5) | SCU_FUC_PIN_SD1 | SCU_FUC_PIN_SD2, 
						AST_SCU_FUN_PIN_CTRL5);						
			break;			
	}
}	


//0: VGA , 1 : CRT, 2 : PASS through Port -A, 3 : PASS through Port -B  
extern void
ast_scu_set_crt_source(u8 dac_soource)
{
	ast_scu_write((ast_scu_read(AST_SCU_MISC1_CTRL) & ~SCU_MISC_DAC_MASK) | 
				SCU_MISC_SET_DAC_SOURCE(dac_soource)  , AST_SCU_MISC1_CTRL);

}


extern void
ast_scu_multi_func_crt(void)
{	
	/* multi-pin for DVO enable DVO (bit18) is VGA , enable DAC (bit16) is CRT  */
#if defined(CONFIG_AST_DAC) || defined(CONFIG_AST_DVO)
		ast_scu_write((ast_scu_read(AST_SCU_MISC1_CTRL) & ~SCU_MISC_DAC_MASK)
					| SCU_MISC_DAC_SOURCE_CRT | SCU_MISC_DVO_SOURCE_CRT | SCU_MISC_2D_CRT_EN , AST_SCU_MISC1_CTRL);
#elif defined(CONFIG_AST_DVO) 
		ast_scu_write(ast_scu_read(AST_SCU_MISC1_CTRL) | SCU_MISC_DVO_SOURCE_CRT| SCU_MISC_2D_CRT_EN, AST_SCU_MISC1_CTRL);
#else //default(CONFIG_AST_DAC) 
		ast_scu_write((ast_scu_read(AST_SCU_MISC1_CTRL) & ~SCU_MISC_DAC_MASK) 
					| SCU_MISC_DAC_SOURCE_CRT | SCU_MISC_2D_CRT_EN, AST_SCU_MISC1_CTRL);
#endif

	//Digital vodeo input function pins : 00 disable, 10 24bits mode 888,
	ast_scu_write((ast_scu_read(AST_SCU_FUN_PIN_CTRL6) &
			~SCU_FUC_PIN_DIGI_V_OUT_MASK) |
			SCU_FUC_PIN_DIGI_V_OUT(VIDEO_24BITS), AST_SCU_FUN_PIN_CTRL6);

	//VPI input
#if 0	
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL2) |
			SCU_FUN_PIN_VPIB9 | SCU_FUN_PIN_VPIB8 |
			SCU_FUN_PIN_VPIB7 | SCU_FUN_PIN_VPIB6 |
			SCU_FUN_PIN_VPIB5 | SCU_FUN_PIN_VPIB4 |
			SCU_FUN_PIN_VPIB3 | SCU_FUN_PIN_VPIB2 |
			SCU_FUN_PIN_VPIB1 | SCU_FUN_PIN_VPIB0 |
			SCU_FUN_PIN_VPICLK | SCU_FUN_PIN_VPIVS |
			SCU_FUN_PIN_VPIHS | SCU_FUN_PIN_VPIODD |
			SCU_FUN_PIN_VPIDE ,AST_SCU_FUN_PIN_CTRL2);

	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL3) |
			SCU_FUN_PIN_VPIR9 | SCU_FUN_PIN_VPIR8 |
			SCU_FUN_PIN_VPIR7 | SCU_FUN_PIN_VPIR6 |	
			SCU_FUN_PIN_VPIR5 | SCU_FUN_PIN_VPIR4 |
			SCU_FUN_PIN_VPIR3 | SCU_FUN_PIN_VPIR2 |
			SCU_FUN_PIN_VPIR1 | SCU_FUN_PIN_VPIR0 |
			SCU_FUN_PIN_VPIG9 | SCU_FUN_PIN_VPIG8 |	
			SCU_FUN_PIN_VPIG7 | SCU_FUN_PIN_VPIG6 |
			SCU_FUN_PIN_VPIG5 | SCU_FUN_PIN_VPIG4 |
			SCU_FUN_PIN_VPIG3 | SCU_FUN_PIN_VPIG2 |
			SCU_FUN_PIN_VPIG1 | SCU_FUN_PIN_VPIG0 ,AST_SCU_FUN_PIN_CTRL3);
#endif
}


extern void
ast_scu_multi_nic_switch(u8 enable)
{
	if(enable) { 
		ast_scu_write((0x1 << 28) | ast_scu_read(AST_SCU_MAC_CLK), AST_SCU_MAC_CLK);
		ast_scu_write(~(0x1 << 11) & ast_scu_read(AST_SCU_RESET), AST_SCU_RESET);	//A1 ECO
	} else {
		ast_scu_write(~(0x1 << 28) & ast_scu_read(AST_SCU_MAC_CLK), AST_SCU_MAC_CLK);
		ast_scu_write((0x1 << 11) | ast_scu_read(AST_SCU_RESET), AST_SCU_RESET);	
	}
		
}

extern void
ast_scu_multi_func_sgpio(void)
{
	ast_scu_write(ast_scu_read(AST_SCU_FUN_PIN_CTRL2) |
			SCU_FUN_PIN_SGPMI |
			SCU_FUN_PIN_SGPMO |
			SCU_FUN_PIN_SGPMLD |
			SCU_FUN_PIN_SGPMCK, AST_SCU_FUN_PIN_CTRL2);	

}	


//***********************************Information ***********************************
extern void
ast_scu_set_vga_display(u8 enable)
{
	if(enable)
		ast_scu_write(ast_scu_read(AST_SCU_MISC1_CTRL) & ~SCU_MISC_VGA_CRT_DIS, AST_SCU_MISC1_CTRL);
	else
		ast_scu_write(ast_scu_read(AST_SCU_MISC1_CTRL) | SCU_MISC_VGA_CRT_DIS, AST_SCU_MISC1_CTRL);
}

extern u8
ast_scu_get_vga_display(void)
{
	if(ast_scu_read(AST_SCU_MISC1_CTRL) & SCU_MISC_VGA_CRT_DIS)
		return 0;
	else
		return 1;
}

extern u8
ast_scu_adc_trim_read(void)
{
	return (ast_scu_read(AST_SCU_OTP1) >> 28);
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

#ifdef AST_SCU_OTP_READ_CTRL
extern u8
ast_scu_otp_read(u8 reg)
{
        ast_scu_write(SCU_OTP_TRIGGER | SCU_OTP_READ_ADDR(reg), AST_SCU_OTP_READ_CTRL);
        while(SCU_OTP_TRIGGER_STS & ast_scu_read(AST_SCU_OTP_READ_CTRL));
        return (SCU_OTP_READ_DATA(ast_scu_read(AST_SCU_OTP_READ_CTRL)));
}

#endif

// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2016 Google, Inc
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 */

#include <common.h>
//#include <clk-uclass.h>
//#include <dm.h>
#include <asm/io.h>
//#include <dm/lists.h>
//#include <asm/arch/scu_ast2500.h>

//#include <dt-bindings/clock/ast2500-scu.h>

#ifdef CONFIG_CLK
/*
 * MAC Clock Delay settings, taken from Aspeed SDK
 */
#define RGMII_TXCLK_ODLY 8
#define RMII_RXCLK_IDLY 2

/*
 * TGMII Clock Duty constants, taken from Aspeed SDK
 */
#define RGMII2_TXCK_DUTY 0x66
#define RGMII1_TXCK_DUTY 0x64

#define D2PLL_DEFAULT_RATE (250 * 1000 * 1000)

DECLARE_GLOBAL_DATA_PTR;

/*
 * Clock divider/multiplier configuration struct.
 * For H-PLL and M-PLL the formula is
 * (Output Frequency) = CLKIN * ((M + 1) / (N + 1)) / (P + 1)
 * M - Numerator
 * N - Denumerator
 * P - Post Divider
 * They have the same layout in their control register.
 *
 * D-PLL and D2-PLL have extra divider (OD + 1), which is not
 * yet needed and ignored by clock configurations.
 */
struct ast2500_div_config
{
	unsigned int num;
	unsigned int denum;
	unsigned int post_div;
};

/*
 * Get the rate of the M-PLL clock from input clock frequency and
 * the value of the M-PLL Parameter Register.
 */
static ulong ast2500_get_mpll_rate(ulong clkin, u32 mpll_reg)
{
	const ulong num = (mpll_reg & SCU_MPLL_NUM_MASK) >> SCU_MPLL_NUM_SHIFT;
	const ulong denum = (mpll_reg & SCU_MPLL_DENUM_MASK) >> SCU_MPLL_DENUM_SHIFT;
	const ulong post_div = (mpll_reg & SCU_MPLL_POST_MASK) >> SCU_MPLL_POST_SHIFT;

	return (clkin * ((num + 1) / (denum + 1))) / (post_div + 1);
}

/*
 * Get the rate of the H-PLL clock from input clock frequency and
 * the value of the H-PLL Parameter Register.
 */
static ulong ast2500_get_hpll_rate(ulong clkin, u32 hpll_reg)
{
	const ulong num = (hpll_reg & SCU_HPLL_NUM_MASK) >> SCU_HPLL_NUM_SHIFT;
	const ulong denum = (hpll_reg & SCU_HPLL_DENUM_MASK) >> SCU_HPLL_DENUM_SHIFT;
	const ulong post_div = (hpll_reg & SCU_HPLL_POST_MASK) >> SCU_HPLL_POST_SHIFT;

	return (clkin * ((num + 1) / (denum + 1))) / (post_div + 1);
}

static ulong ast2500_get_clkin(struct ast2500_scu *scu)
{
	return readl(&scu->hwstrap) & SCU_HWSTRAP_CLKIN_25MHZ
		   ? 25 * 1000 * 1000
		   : 24 * 1000 * 1000;
}

/**
 * Get current rate or uart clock
 *
 * @scu SCU registers
 * @uart_index UART index, 1-5
 *
 * @return current setting for uart clock rate
 */
static ulong ast2500_get_uart_clk_rate(struct ast2500_scu *scu, int uart_index)
{
	/*
	 * ast2500 datasheet is very confusing when it comes to UART clocks,
	 * especially when CLKIN = 25 MHz. The settings are in
	 * different registers and it is unclear how they interact.
	 *
	 * This has only been tested with default settings and CLKIN = 24 MHz.
	 */
	ulong uart_clkin;

	if (readl(&scu->misc_ctrl2) &
	    (1 << (uart_index - 1 + SCU_MISC2_UARTCLK_SHIFT)))
		uart_clkin = 192 * 1000 * 1000;
	else
		uart_clkin = 24 * 1000 * 1000;

	if (readl(&scu->misc_ctrl1) & SCU_MISC_UARTCLK_DIV13)
		uart_clkin /= 13;

	return uart_clkin;
}

static ulong ast2500_clk_get_rate(struct clk *clk)
{
	struct ast2500_clk_priv *priv = dev_get_priv(clk->dev);
	ulong clkin = ast2500_get_clkin(priv->scu);
	ulong rate;

	switch (clk->id)
	{
	case PLL_HPLL:
	case ARMCLK:
		/*
		 * This ignores dynamic/static slowdown of ARMCLK and may
		 * be inaccurate.
		 */
		rate = ast2500_get_hpll_rate(clkin,
					     readl(&priv->scu->h_pll_param));
		break;
	case MCLK_DDR:
		rate = ast2500_get_mpll_rate(clkin,
					     readl(&priv->scu->m_pll_param));
		break;
	case BCLK_PCLK:
	{
		ulong apb_div = 4 + 4 * ((readl(&priv->scu->clk_sel1) & SCU_PCLK_DIV_MASK) >> SCU_PCLK_DIV_SHIFT);
		rate = ast2500_get_hpll_rate(clkin,
					     readl(&priv->scu->h_pll_param));
		rate = rate / apb_div;
	}
	break;
	case PCLK_UART1:
		rate = ast2500_get_uart_clk_rate(priv->scu, 1);
		break;
	case PCLK_UART2:
		rate = ast2500_get_uart_clk_rate(priv->scu, 2);
		break;
	case PCLK_UART3:
		rate = ast2500_get_uart_clk_rate(priv->scu, 3);
		break;
	case PCLK_UART4:
		rate = ast2500_get_uart_clk_rate(priv->scu, 4);
		break;
	case PCLK_UART5:
		rate = ast2500_get_uart_clk_rate(priv->scu, 5);
		break;
	default:
		return -ENOENT;
	}

	return rate;
}

/*
 * @input_rate - the rate of input clock in Hz
 * @requested_rate - desired output rate in Hz
 * @div - this is an IN/OUT parameter, at input all fields of the config
 * need to be set to their maximum allowed values.
 * The result (the best config we could find), would also be returned
 * in this structure.
 *
 * @return The clock rate, when the resulting div_config is used.
 */
static ulong ast2500_calc_clock_config(ulong input_rate, ulong requested_rate,
				       struct ast2500_div_config *cfg)
{
	/*
	 * The assumption is that kHz precision is good enough and
	 * also enough to avoid overflow when multiplying.
	 */
	const ulong input_rate_khz = input_rate / 1000;
	const ulong rate_khz = requested_rate / 1000;
	const struct ast2500_div_config max_vals = *cfg;
	struct ast2500_div_config it = {0, 0, 0};
	ulong delta = rate_khz;
	ulong new_rate_khz = 0;

	for (; it.denum <= max_vals.denum; ++it.denum)
	{
		for (it.post_div = 0; it.post_div <= max_vals.post_div;
		     ++it.post_div)
		{
			it.num = (rate_khz * (it.post_div + 1) / input_rate_khz) * (it.denum + 1);
			if (it.num > max_vals.num)
				continue;

			new_rate_khz = (input_rate_khz * ((it.num + 1) / (it.denum + 1))) / (it.post_div + 1);

			/* Keep the rate below requested one. */
			if (new_rate_khz > rate_khz)
				continue;

			if (new_rate_khz - rate_khz < delta)
			{
				delta = new_rate_khz - rate_khz;
				*cfg = it;
				if (delta == 0)
					return new_rate_khz * 1000;
			}
		}
	}

	return new_rate_khz * 1000;
}

static ulong ast2500_configure_ddr(struct ast2500_scu *scu, ulong rate)
{
	ulong clkin = ast2500_get_clkin(scu);
	u32 mpll_reg;
	struct ast2500_div_config div_cfg = {
	    .num = (SCU_MPLL_NUM_MASK >> SCU_MPLL_NUM_SHIFT),
	    .denum = (SCU_MPLL_DENUM_MASK >> SCU_MPLL_DENUM_SHIFT),
	    .post_div = (SCU_MPLL_POST_MASK >> SCU_MPLL_POST_SHIFT),
	};

	ast2500_calc_clock_config(clkin, rate, &div_cfg);

	mpll_reg = readl(&scu->m_pll_param);
	mpll_reg &= ~(SCU_MPLL_POST_MASK | SCU_MPLL_NUM_MASK | SCU_MPLL_DENUM_MASK);
	mpll_reg |= (div_cfg.post_div << SCU_MPLL_POST_SHIFT) | (div_cfg.num << SCU_MPLL_NUM_SHIFT) | (div_cfg.denum << SCU_MPLL_DENUM_SHIFT);

	ast_scu_unlock(scu);
	writel(mpll_reg, &scu->m_pll_param);
	ast_scu_lock(scu);

	return ast2500_get_mpll_rate(clkin, mpll_reg);
}

static ulong ast2500_configure_mac(struct ast2500_scu *scu, int index)
{
	ulong clkin = ast2500_get_clkin(scu);
	ulong hpll_rate = ast2500_get_hpll_rate(clkin,
						readl(&scu->h_pll_param));
	ulong required_rate;
	u32 hwstrap;
	u32 divisor;
	u32 reset_bit;
	u32 clkstop_bit;

	/*
	 * According to data sheet, for 10/100 mode the MAC clock frequency
	 * should be at least 25MHz and for 1000 mode at least 100MHz
	 */
	hwstrap = readl(&scu->hwstrap);
	if (hwstrap & (SCU_HWSTRAP_MAC1_RGMII | SCU_HWSTRAP_MAC2_RGMII))
		required_rate = 100 * 1000 * 1000;
	else
		required_rate = 25 * 1000 * 1000;

	divisor = hpll_rate / required_rate;

	if (divisor < 4)
	{
		/* Clock can't run fast enough, but let's try anyway */
		debug("MAC clock too slow\n");
		divisor = 4;
	}
	else if (divisor > 16)
	{
		/* Can't slow down the clock enough, but let's try anyway */
		debug("MAC clock too fast\n");
		divisor = 16;
	}

	switch (index)
	{
	case 1:
		reset_bit = SCU_SYSRESET_MAC1;
		clkstop_bit = SCU_CLKSTOP_MAC1;
		break;
	case 2:
		reset_bit = SCU_SYSRESET_MAC2;
		clkstop_bit = SCU_CLKSTOP_MAC2;
		break;
	default:
		return -EINVAL;
	}

	ast_scu_unlock(scu);
	clrsetbits_le32(&scu->clk_sel1, SCU_MACCLK_MASK,
			((divisor - 2) / 2) << SCU_MACCLK_SHIFT);

	/*
	 * Disable MAC, start its clock and re-enable it.
	 * The procedure and the delays (100us & 10ms) are
	 * specified in the datasheet.
	 */
	setbits_le32(&scu->sysreset_ctrl1, reset_bit);
	udelay(100);
	clrbits_le32(&scu->clk_stop_ctrl1, clkstop_bit);
	mdelay(10);
	clrbits_le32(&scu->sysreset_ctrl1, reset_bit);

	writel((RGMII2_TXCK_DUTY << SCU_CLKDUTY_RGMII2TXCK_SHIFT) | (RGMII1_TXCK_DUTY << SCU_CLKDUTY_RGMII1TXCK_SHIFT),
	       &scu->clk_duty_sel);

	ast_scu_lock(scu);

	return required_rate;
}

static ulong ast2500_configure_d2pll(struct ast2500_scu *scu, ulong rate)
{
	/*
	 * The values and the meaning of the next three
	 * parameters are undocumented. Taken from Aspeed SDK.
	 */
	const u32 d2_pll_ext_param = 0x2c;
	const u32 d2_pll_sip = 0x11;
	const u32 d2_pll_sic = 0x18;
	u32 clk_delay_settings =
	    (RMII_RXCLK_IDLY << SCU_MICDS_MAC1RMII_RDLY_SHIFT) | (RMII_RXCLK_IDLY << SCU_MICDS_MAC2RMII_RDLY_SHIFT) | (RGMII_TXCLK_ODLY << SCU_MICDS_MAC1RGMII_TXDLY_SHIFT) | (RGMII_TXCLK_ODLY << SCU_MICDS_MAC2RGMII_TXDLY_SHIFT);
	struct ast2500_div_config div_cfg = {
	    .num = SCU_D2PLL_NUM_MASK >> SCU_D2PLL_NUM_SHIFT,
	    .denum = SCU_D2PLL_DENUM_MASK >> SCU_D2PLL_DENUM_SHIFT,
	    .post_div = SCU_D2PLL_POST_MASK >> SCU_D2PLL_POST_SHIFT,
	};
	ulong clkin = ast2500_get_clkin(scu);
	ulong new_rate;

	ast_scu_unlock(scu);
	writel((d2_pll_ext_param << SCU_D2PLL_EXT1_PARAM_SHIFT) | SCU_D2PLL_EXT1_OFF | SCU_D2PLL_EXT1_RESET, &scu->d2_pll_ext_param[0]);

	/*
	 * Select USB2.0 port1 PHY clock as a clock source for GCRT.
	 * This would disconnect it from D2-PLL.
	 */
	clrsetbits_le32(&scu->misc_ctrl1, SCU_MISC_D2PLL_OFF,
			SCU_MISC_GCRT_USB20CLK);

	new_rate = ast2500_calc_clock_config(clkin, rate, &div_cfg);
	writel((d2_pll_sip << SCU_D2PLL_SIP_SHIFT) | (d2_pll_sic << SCU_D2PLL_SIC_SHIFT) | (div_cfg.num << SCU_D2PLL_NUM_SHIFT) | (div_cfg.denum << SCU_D2PLL_DENUM_SHIFT) | (div_cfg.post_div << SCU_D2PLL_POST_SHIFT),
	       &scu->d2_pll_param);

	clrbits_le32(&scu->d2_pll_ext_param[0],
		     SCU_D2PLL_EXT1_OFF | SCU_D2PLL_EXT1_RESET);

	clrsetbits_le32(&scu->misc_ctrl2,
			SCU_MISC2_RGMII_HPLL | SCU_MISC2_RMII_MPLL | SCU_MISC2_RGMII_CLKDIV_MASK |
			    SCU_MISC2_RMII_CLKDIV_MASK,
			(4 << SCU_MISC2_RMII_CLKDIV_SHIFT));

	writel(clk_delay_settings | SCU_MICDS_RGMIIPLL, &scu->mac_clk_delay);
	writel(clk_delay_settings, &scu->mac_clk_delay_100M);
	writel(clk_delay_settings, &scu->mac_clk_delay_10M);

	ast_scu_lock(scu);

	return new_rate;
}

static ulong ast2500_clk_set_rate(struct clk *clk, ulong rate)
{
	struct ast2500_clk_priv *priv = dev_get_priv(clk->dev);

	ulong new_rate;
	switch (clk->id)
	{
	case PLL_MPLL:
	case MCLK_DDR:
		new_rate = ast2500_configure_ddr(priv->scu, rate);
		break;
	case PLL_D2PLL:
		new_rate = ast2500_configure_d2pll(priv->scu, rate);
		break;
	default:
		return -ENOENT;
	}

	return new_rate;
}

static int ast2500_clk_enable(struct clk *clk)
{
	struct ast2500_clk_priv *priv = dev_get_priv(clk->dev);

	switch (clk->id)
	{
	/*
	 * For MAC clocks the clock rate is
	 * configured based on whether RGMII or RMII mode has been selected
	 * through hardware strapping.
	 */
	case PCLK_MAC1:
		ast2500_configure_mac(priv->scu, 1);
		break;
	case PCLK_MAC2:
		ast2500_configure_mac(priv->scu, 2);
		break;
	case PLL_D2PLL:
		ast2500_configure_d2pll(priv->scu, D2PLL_DEFAULT_RATE);
	default:
		return -ENOENT;
	}

	return 0;
}

struct clk_ops ast2500_clk_ops = {
    .get_rate = ast2500_clk_get_rate,
    .set_rate = ast2500_clk_set_rate,
    .enable = ast2500_clk_enable,
};

static int ast2500_clk_probe(struct udevice *dev)
{
	struct ast2500_clk_priv *priv = dev_get_priv(dev);

	priv->scu = devfdt_get_addr_ptr(dev);
	if (IS_ERR(priv->scu))
		return PTR_ERR(priv->scu);

	return 0;
}

static int ast2500_clk_bind(struct udevice *dev)
{
	int ret;

	/* The reset driver does not have a device node, so bind it here */
	ret = device_bind_driver(gd->dm_root, "ast_sysreset", "reset", &dev);
	if (ret)
		debug("Warning: No reset driver: ret=%d\n", ret);

	return 0;
}

static const struct udevice_id ast2500_clk_ids[] = {
    {.compatible = "aspeed,ast2500-scu"},
    {}};

U_BOOT_DRIVER(aspeed_ast2500_scu) = {
    .name = "aspeed_ast2500_scu",
    .id = UCLASS_CLK,
    .of_match = ast2500_clk_ids,
    .priv_auto_alloc_size = sizeof(struct ast2500_clk_priv),
    .ops = &ast2500_clk_ops,
    .bind = ast2500_clk_bind,
    .probe = ast2500_clk_probe,
};
#else

#define CLKIN_25MHZ_EN BIT(23)
#define AST2400_CLK_SOURCE_SEL BIT(18)

#ifdef CONFIG_FPGA_ASPEED
extern u32 aspeed_get_clk_in_rate(void)
{
	return 24000000;
}
#else
extern u32 aspeed_get_clk_in_rate(void)
{
	u32 clkin;
	u32 strap = readl(ASPEED_HW_STRAP1);
#ifdef CONFIG_MACH_ASPEED_G6
	if (strap & CLKIN_25MHZ_EN)
		clkin = 25000000;
	else
		clkin = 24000000;
#elif defined(CONFIG_MACH_ASPEED_G5)
	if (strap & CLKIN_25MHZ_EN)
		clkin = 25000000;
	else
		clkin = 24000000;
#elif defined(CONFIG_MACH_ASPEED_G4)
	if (strap & CLKIN_25MHZ_EN)
	{
		clkin = 25000000;
	}
	else if (strap & AST2400_CLK_SOURCE_SEL)
	{
		clkin = 48000000;
	}
	else
	{
		clkin = 24000000;
	}
#else
#err "No define for clk"
#endif
	return clkin;
}
#endif

#ifdef CONFIG_MACH_ASPEED_G6
#define ASPEED_HPLL_PARAM 0x200
#else
#define ASPEED_HPLL_PARAM 0x24
#endif

#define AST2600_HPLL_BYPASS_EN BIT(20)

#define AST2500_HPLL_BYPASS_EN BIT(20)
#define AST2400_HPLL_PROGRAMMED BIT(18)
#define AST2400_HPLL_BYPASS_EN BIT(17)

#ifdef CONFIG_FPGA_ASPEED
extern u32 aspeed_get_hpll_clk_rate(void)
{
	return 24000000;
}
#else
extern u32 aspeed_get_hpll_clk_rate(void)
{
	unsigned int mult, div;
	u32 val = readl(ASPEED_SCU_BASE + ASPEED_HPLL_PARAM);
	u32 clkin = aspeed_get_clk_in_rate();

#ifdef CONFIG_MACH_ASPEED_G6
	if (val & AST2600_HPLL_BYPASS_EN)
	{
		/* Pass through mode */
		mult = div = 1;
	}
	else
	{
		/* F = clkin * [(M+1) / (N+1)] / (P + 1) */
		u32 p = (val >> 13) & 0x3f;
		u32 m = (val >> 5) & 0xff;
		u32 n = val & 0x1f;

		mult = (m + 1) / (n + 1);
		div = p + 1;
	}
#elif defined(CONFIG_MACH_ASPEED_G5)
	if (val & AST2500_HPLL_BYPASS_EN)
	{
		/* Pass through mode */
		mult = div = 1;
	}
	else
	{
		/* F = clkin * [(M+1) / (N+1)] / (P + 1) */
		u32 p = (val >> 13) & 0x3f;
		u32 m = (val >> 5) & 0xff;
		u32 n = val & 0x1f;

		mult = (m + 1) / (n + 1);
		div = p + 1;
	}
#elif defined(CONFIG_MACH_ASPEED_G4)
	const u16 hpll_rates[][4] = {
	    {384, 360, 336, 408},
	    {400, 375, 350, 425},
	};

	if (val & AST2400_HPLL_PROGRAMMED)
	{
		if (val & AST2400_HPLL_BYPASS_EN)
		{
			/* Pass through mode */
			mult = div = 1;
		}
		else
		{
			/* F = 24Mhz * (2-OD) * [(N + 2) / (D + 1)] */
			u32 n = (val >> 5) & 0x3f;
			u32 od = (val >> 4) & 0x1;
			u32 d = val & 0xf;

			mult = (2 - od) * (n + 2);
			div = d + 1;
		}
	}
	else
	{
		//fix
		u32 strap = readl(ASPEED_HW_STRAP1);
		u16 rate = (val >> 8) & 3;
		if (strap & CLKIN_25MHZ_EN)
		{
			clkin = hpll_rates[1][rate];
		}
		else if (strap & AST2400_CLK_SOURCE_SEL)
		{
			clkin = hpll_rates[0][rate];
		}
		else
		{
			clkin = hpll_rates[0][rate];
		}
		clkin *= 1000000;
		mult = 1;
		div = 1;
	}

#else
#err "No define for clk"
#endif
	return (clkin * mult / div);
}
#endif

#ifdef CONFIG_FPGA_ASPEED
extern u32 aspeed_get_h_clk_rate(void)
{
	return 24000000;
}
#else
extern u32 aspeed_get_h_clk_rate(void)
{
	u32 axi_div, ahb_div, clk;
	u32 strap = readl(ASPEED_HW_STRAP1);
	u32 hpll = aspeed_get_hpll_clk_rate();
#if defined(CONFIG_MACH_ASPEED_G6) || (CONFIG_MACH_ASPEED_G5)
	axi_div = 2;
	ahb_div = ((strap >> 9) & 0x7) + 1;
	clk = (hpll / axi_div) / ahb_div;
#elif defined(CONFIG_MACH_ASPEED_G4)
	ahb_div = ((strap >> 10) & 0x3) + 1;
	clk = hpll / ahb_div;
#else
#err "No define for h clk"
#endif
	return clk;
}
#endif

#define ASPEED_MPLL_PARAM 0x20
#define AST2500_MPLL_BYPASS_EN BIT(20)
#define AST2500_MPLL_OFF BIT(19)
#define AST2400_MPLL_BYPASS_EN BIT(17)
#define AST2400_MPLL_OFF BIT(16)

extern u32 aspeed_get_mpll_clk_rate(void)
{
	unsigned int mult, div;
	u32 m_pll_set = readl(ASPEED_SCU_BASE + ASPEED_MPLL_PARAM);
	u32 clkin = aspeed_get_clk_in_rate();

#if defined(CONFIG_MACH_ASPEED_G6) || (CONFIG_MACH_ASPEED_G5)
	if (m_pll_set & AST2500_MPLL_OFF)
		return 0;

	if (m_pll_set & AST2500_MPLL_BYPASS_EN)
	{
		return clkin;
	}
	else
	{
		u32 p = (m_pll_set >> 13) & 0x3f;
		u32 m = (m_pll_set >> 5) & 0xff;
		u32 n = m_pll_set & 0xf;

		//mpll =  24MHz * [(M+1) /(N+1)] / (P+1)
		mult = (m + 1) * (n + 1);
		div = (p + 1);
	}
#elif defined(CONFIG_MACH_ASPEED_G4)
	if (m_pll_set & AST2400_MPLL_OFF)
		return 0;

	if (m_pll_set & AST2400_MPLL_BYPASS_EN)
	{
		return clkin;
	}
	else
	{
		u32 od = (m_pll_set >> 4) & 0x1;
		u32 n = (m_pll_set >> 5) & 0x3f;
		u32 d = m_pll_set & 0xf;

		//mpll = 24MHz * (2-OD) * ((Numerator+2)/(Denumerator+1))
		mult = (2 - od) * (n + 2);
		div = (d + 1);
	}
#else
#err "No define for h clk"
#endif
	return (clkin * mult / div);
}

#ifdef CONFIG_MACH_ASPEED_G5
#define ASPEED_DPLL_PARAM 0x28

#define ASPEED_DPLL_EXT0_PARAM 0x130
#define AST2500_DPLL_BYPASS_EN BIT(1)
#define AST2500_DPLL_OFF BIT(0)

/*	AST_SCU_D_PLL : 0x28 - D-PLL Parameter  register	*/
#define SCU_D_PLL_GET_ODNUM(x)
#define SCU_D_PLL_GET_PNUM(x)
#define SCU_D_PLL_GET_NNUM(x)

/*	AST_SCU_D_PLL_EXTEND : 0x130 - D-PLL Extended Parameter  register	*/
#define SCU_D_PLL_SET_MODE(x) ((x & 0x3) << 3)

extern u32 aspeed_get_dpll_clk_rate(void)
{
	unsigned int mult, div;
	u32 d_pll_set = readl(ASPEED_SCU_BASE + ASPEED_DPLL_PARAM);
	u32 d_pll_ext = readl(ASPEED_SCU_BASE + ASPEED_DPLL_EXT0_PARAM);
	u32 clkin = aspeed_get_clk_in_rate();

	if (d_pll_ext & AST2500_DPLL_OFF)
		return 0;

	if (d_pll_ext & AST2500_DPLL_BYPASS_EN)
	{
		return clkin;
	}
	else
	{
		u32 m = d_pll_set & 0xff;
		u32 n = (d_pll_set >> 8) & 0x1f;
		u32 p = (d_pll_set >> 13) & 0x3f;
		u32 o = (d_pll_set >> 19) & 0x7;

		//dpll = 24MHz * [(M + 1) /(N + 1)] / (P + 1) / (OD + 1)
		mult = m + 1;
		div = (n + 1) * (p + 1) * (o + 1);
	}
	return (clkin * mult / div);
}
#endif

#define ASPEED_D2PLL_PARAM 0x1C
#define AST2400_D2PLL_OFF BIT(17)
#define AST2400_D2PLL_BYPASS_EN BIT(18)

#define ASPEED_D2PLL_EXTEND_PARAM 0x13C
#define AST2500_D2PLL_OFF BIT(0)
#define AST2500_D2PLL_BYPASS_EN BIT(1)

extern u32 aspeed_get_d2pll_clk_rate(void)
{
	unsigned int mult, div;
	u32 d2_pll_set = readl(ASPEED_SCU_BASE + ASPEED_D2PLL_PARAM);
	u32 d2_pll_ext = readl(ASPEED_SCU_BASE + ASPEED_D2PLL_EXTEND_PARAM);
	u32 clkin = aspeed_get_clk_in_rate();

#ifdef CONFIG_MACH_ASPEED_G6

#elif defined(CONFIG_MACH_ASPEED_G5)
	if (d2_pll_ext & AST2500_D2PLL_OFF)
		return 0;

	if (d2_pll_ext & AST2500_D2PLL_BYPASS_EN)
	{
		return clkin;
	}
	else
	{
		u32 m = d2_pll_set * 0xff;
		u32 n = (d2_pll_set >> 8) & 0x1f;
		u32 p = (d2_pll_set >> 13) & 0x3f;
		u32 od = (d2_pll_set >> 19) & 0x3;
		//hpll = 24MHz * [(M + 1) /(N + 1)] / (P + 1) / (OD + 1)
		mult = m + 1;
		div = (n + 1) * (p + 1) * (od + 1);
	}
#elif defined(CONFIG_MACH_ASPEED_G4)
	if (d2_pll_set & AST2400_D2PLL_OFF)
		return 0;

	// Programming
	if (d2_pll_set & AST2400_D2PLL_BYPASS_EN)
	{
		return clkin;
	}
	else
	{
		u32 n = (d2_pll_set & 0xff);
		u32 d = (d2_pll_set >> 8) & 0x1f;
		u32 o = (d2_pll_set >> 13) & 0x3;
		o = (1 << (o - 1));
		u32 p = (d2_pll_set >> 15) & 0x3;
		if (p == 2)
			p = 2;
		else
			p = (0x1 << p);
		u32 p2 = (d2_pll_set >> 19) & 0x7;
		p2 += 1;
		//FOUT (Output frequency) = 24MHz * (Num * 2) / (Denum * OD * PD * PD2)
		mult = (n * 2);
		div = (o * p * p2);
	}
#else
#err "No define for h clk"
#endif
	return (clkin * mult / div);
}

#ifdef CONFIG_MACH_ASPEED_G6
#define ASPEED_CLK_SELECT 0x300
#else
#define ASPEED_CLK_SELECT 0x08
#endif

#ifdef CONFIG_FPGA_ASPEED
extern u32 aspeed_get_p_clk_rate(void)
{
	return 24000000;
}
#else
extern u32 aspeed_get_p_clk_rate(void)
{
	unsigned int div;
	u32 hpll = aspeed_get_hpll_clk_rate();
	u32 set = readl(ASPEED_SCU_BASE + ASPEED_CLK_SELECT);
	div = (set >> 23) & 0x7;
#if defined(CONFIG_MACH_ASPEED_G6) || defined(CONFIG_MACH_ASPEED_G5)
	div = (div + 1) << 2;
#elif defined(CONFIG_MACH_ASPEED_G4)
	div = (div + 1) << 1;
#else
#err "No define for p clk"
#endif
	return (hpll / div);
}
#endif


#define SCU_LHCLK_SOURCE_EN BIT(19)

#ifndef CONFIG_MACH_ASPEED_G6
extern u32 aspeed_get_lpc_host_clk_rate(void)
{
	unsigned int div;
	u32 clk_sel = readl(ASPEED_SCU_BASE + ASPEED_CLK_SELECT);
	u32 hpll = aspeed_get_hpll_clk_rate();
	if (SCU_LHCLK_SOURCE_EN & clk_sel)
	{
		div = (clk_sel >> 20) & 0x7;
#ifdef CONFIG_MACH_ASPEED_G5
		div = (div + 1) << 2;
#elif defined(CONFIG_MACH_ASPEED_G4)
		div = (div + 1) << 1;
#else
#err "No define for lpc clk"
#endif
		return (hpll / div);
	}
	else
	{
		return 0;
	}
}
#endif

extern u32 aspeed_get_sd_clk_rate(void)
{
	u32 hpll = aspeed_get_hpll_clk_rate();
	u32 clk_sel = readl(ASPEED_SCU_BASE + ASPEED_CLK_SELECT);
	u32 sd_div = (clk_sel >> 12) & 0x7;

#if defined(CONFIG_MACH_ASPEED_G6) || defined(CONFIG_MACH_ASPEED_G5)
	sd_div = (sd_div + 1) << 2;
#elif defined(CONFIG_MACH_ASPEED_G4)
	sd_div = (sd_div + 1) << 1;
#else
#err "No define for sd clk"
#endif
	return (hpll / sd_div);
}

struct aspeed_clock {
	char *ctrl_name;
	u32 reg;
	u32 clk_en_bit;
	int flag;
};

#ifdef CONFIG_MACH_ASPEED_G6
static struct aspeed_clock ast2600_clk[] = {
	{ "MAC1", ASPEED_SCU_BASE + 0x84, BIT(20), 1 },
	{ "MAC2", ASPEED_SCU_BASE + 0x84, BIT(21), 1 },
	{ "MAC3", ASPEED_SCU_BASE + 0x94, BIT(20), 1 },
	{ "MAC4", ASPEED_SCU_BASE + 0x94, BIT(21), 1 },	
};
#elif defined(CONFIG_MACH_ASPEED_G4) || defined(CONFIG_MACH_ASPEED_G5)
static struct aspeed_clock ast2500_clk[] = {
	{ "MAC1", ASPEED_SCU_BASE + 0x0C, BIT(20), 0 },
	{ "MAC2", ASPEED_SCU_BASE + 0x0C, BIT(21), 0 },
};
#endif

extern void aspeed_clk_enable(char *ctrl_name)
{
	int i = 0;
#if defined(CONFIG_MACH_ASPEED_G6) 
	struct aspeed_clock *clk_config = ast2600_clk;
	int array_size = ARRAY_SIZE(ast2600_clk);
#elif defined(CONFIG_MACH_ASPEED_G4) || defined(CONFIG_MACH_ASPEED_G5)
	struct aspeed_clock *clk_config = ast2500_clk;
	int array_size = ARRAY_SIZE(ast2500_clk);
#else
#err "No define for clk enable"xx
#endif

	for(i = 0; i < array_size; i++) {
		if(!strcmp(ctrl_name, clk_config[i].ctrl_name )) {
			if(clk_config[i].flag) {
				writel(readl(clk_config[i].reg) | clk_config[i].clk_en_bit, clk_config[i].reg);
			} else {
				writel(readl(clk_config[i].reg) & ~(clk_config[i].clk_en_bit), clk_config[i].reg);
			}
			break;
		}
	}
}

#endif

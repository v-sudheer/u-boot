// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2017 Google, Inc
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com> 
 */

#include <common.h>
#include <misc.h>
#include <asm/io.h>
#include <asm/arch/aspeed-reset.h>

#ifdef CONFIG_DM_RESET
struct ast2500_reset_priv {
	/* WDT used to perform resets. */
	struct udevice *wdt;
	struct ast2500_scu *scu;
};

static int ast2500_ofdata_to_platdata(struct udevice *dev)
{
	struct ast2500_reset_priv *priv = dev_get_priv(dev);
	int ret;

	ret = uclass_get_device_by_phandle(UCLASS_WDT, dev, "aspeed,wdt",
					   &priv->wdt);
	if (ret) {
		debug("%s: can't find WDT for reset controller", __func__);
		return ret;
	}

	return 0;
}

static int ast2500_reset_assert(struct reset_ctl *reset_ctl)
{
	struct ast2500_reset_priv *priv = dev_get_priv(reset_ctl->dev);
	u32 reset_mode, reset_mask;
	bool reset_sdram;
	int ret;

	/*
	 * To reset SDRAM, a specifal flag in SYSRESET register
	 * needs to be enabled first
	 */
	reset_mode = ast_reset_mode_from_flags(reset_ctl->id);
	reset_mask = ast_reset_mask_from_flags(reset_ctl->id);
	reset_sdram = reset_mode == WDT_CTRL_RESET_SOC &&
		(reset_mask & WDT_RESET_SDRAM);

	if (reset_sdram) {
		ast_scu_unlock(priv->scu);
		setbits_le32(&priv->scu->sysreset_ctrl1,
			     SCU_SYSRESET_SDRAM_WDT);
		ret = wdt_expire_now(priv->wdt, reset_ctl->id);
		clrbits_le32(&priv->scu->sysreset_ctrl1,
			     SCU_SYSRESET_SDRAM_WDT);
		ast_scu_lock(priv->scu);
	} else {
		ret = wdt_expire_now(priv->wdt, reset_ctl->id);
	}

	return ret;
}

static int ast2500_reset_request(struct reset_ctl *reset_ctl)
{
	debug("%s(reset_ctl=%p) (dev=%p, id=%lu)\n", __func__, reset_ctl,
	      reset_ctl->dev, reset_ctl->id);

	return 0;
}

static int ast2500_reset_probe(struct udevice *dev)
{
	struct ast2500_reset_priv *priv = dev_get_priv(dev);

	priv->scu = ast_get_scu();

	return 0;
}

static const struct udevice_id ast2500_reset_ids[] = {
	{ .compatible = "aspeed,ast2500-reset" },
	{ }
};

struct reset_ops ast2500_reset_ops = {
	.rst_assert = ast2500_reset_assert,
	.request = ast2500_reset_request,
};

U_BOOT_DRIVER(ast2500_reset) = {
	.name		= "ast2500_reset",
	.id		= UCLASS_RESET,
	.of_match = ast2500_reset_ids,
	.probe = ast2500_reset_probe,
	.ops = &ast2500_reset_ops,
	.ofdata_to_platdata = ast2500_ofdata_to_platdata,
	.priv_auto_alloc_size = sizeof(struct ast2500_reset_priv),
};
#else

struct aspeed_reset_config {
	char *ctrl_name;
	u32 reg;
	u32 reset_bit;
};

#ifdef CONFIG_MACH_ASPEED_G6
static struct aspeed_reset_config ast2600_reset[] = {
	{ "EMMC", ASPEED_SCU_BASE + 0x40, BIT(16) },
	{ "MAC1", ASPEED_SCU_BASE + 0x40, BIT(11) },
	{ "MAC2", ASPEED_SCU_BASE + 0x40, BIT(12) },
	{ "SDIO", ASPEED_SCU_BASE + 0x50, BIT(24) },	
	{ "MAC3", ASPEED_SCU_BASE + 0x50, BIT(20) },
	{ "MAC4", ASPEED_SCU_BASE + 0x50, BIT(21) },
	{ "MDIO", ASPEED_SCU_BASE + 0x50, BIT(3) },
	{ "I2C", ASPEED_SCU_BASE + 0x50, BIT(2) },
};
#elif defined(CONFIG_MACH_ASPEED_G4) || defined(CONFIG_MACH_ASPEED_G5)
static struct aspeed_reset_config ast2500_reset[] = {
	{ "SDIO", ASPEED_SCU_BASE + 0x04, BIT(16) },
	{ "MAC1", ASPEED_SCU_BASE + 0x04, BIT(11) },
	{ "MAC2", ASPEED_SCU_BASE + 0x04, BIT(12) },
	{ "I2C", ASPEED_SCU_BASE + 0x04, BIT(2) },
};
#else
#endif
extern void aspeed_reset_assert(char *ctrl_name)
{
	int i = 0;
#if defined(CONFIG_MACH_ASPEED_G6) 
	struct aspeed_reset_config *reset_config = ast2600_reset;
	int arrary_size = ARRAY_SIZE(ast2600_reset);
#elif defined(CONFIG_MACH_ASPEED_G4) || defined(CONFIG_MACH_ASPEED_G5)
	struct aspeed_reset_config *reset_config = ast2500_reset;
	int arrary_size = ARRAY_SIZE(ast2500_reset);
#else
#err "No define for aspeed_reset_assert"
#endif	
	for(i = 0; i < arrary_size; i++) {
		if(!strcmp(ctrl_name, reset_config[i].ctrl_name )) {
			writel(readl(reset_config[i].reg) | reset_config[i].reset_bit, reset_config[i].reg);
			break;
		}
	}

}

extern void aspeed_reset_deassert(char *ctrl_name)
{
	int i = 0;
#if defined(CONFIG_MACH_ASPEED_G6) 
	struct aspeed_reset_config *reset_config = ast2600_reset;
	int arrary_size = ARRAY_SIZE(ast2600_reset);
#elif defined(CONFIG_MACH_ASPEED_G4) || defined(CONFIG_MACH_ASPEED_G5)
	struct aspeed_reset_config *reset_config = ast2500_reset;
	int arrary_size = ARRAY_SIZE(ast2500_reset);
#else
#err "No define for aspeed_reset_assert"
#endif	
	for(i = 0; i < arrary_size; i++) {
		if(!strcmp(ctrl_name, reset_config[i].ctrl_name )) {
#ifdef CONFIG_MACH_ASPEED_G6
			writel(readl(reset_config[i].reg + 0x04) | (reset_config[i].reset_bit), reset_config[i].reg + 0x04);
#else
			writel(readl(reset_config[i].reg) & ~(reset_config[i].reset_bit), reset_config[i].reg);			
#endif
			break;
		}
	}
}

#endif

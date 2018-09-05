// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright 2017 Google, Inc
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

static const struct aspeed_reset_config ast2500_reset[] = {
	{ "MAC1", ASPEED_SCU_BASE + 0x04, BIT(11) },
	{ "MAC2", ASPEED_SCU_BASE + 0x04, BIT(12) },
};

extern void aspeed_reset_assert(char *ctrl_name)
{
	int i = 0;
#if defined(CONFIG_MACH_ASPEED_G6) 
		xx
#elif defined(CONFIG_MACH_ASPEED_G4) || defined(CONFIG_MACH_ASPEED_G5)
	for(i = 0; i < ARRAY_SIZE(ast2500_reset); i++) {
		if(!strcmp(ctrl_name, ast2500_reset[i].ctrl_name )) {
			writel(readl(ast2500_reset[i].reg) | ast2500_reset[i].reset_bit, ast2500_reset[i].reg);
			break;
		}
	}
#else
#err "No define for aspeed_reset_assert"
#endif	
}

extern void aspeed_reset_deassert(char *ctrl_name)
{
#if defined(CONFIG_MACH_ASPEED_G6) 
			xx
#elif defined(CONFIG_MACH_ASPEED_G4) || defined(CONFIG_MACH_ASPEED_G5)
	int i = 0;
	for(i = 0; i < ARRAY_SIZE(ast2500_reset); i++) {
		if(!strcmp(ctrl_name, ast2500_reset[i].ctrl_name )) {
			writel(readl(ast2500_reset[i].reg) & ~(ast2500_reset[i].reset_bit), ast2500_reset[i].reg);
			break;
		}
	}
#else
#err "No define for aspeed_reset_assert"
#endif		
}

#endif

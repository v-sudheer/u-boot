// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2017 Google, Inc
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 */

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <asm/io.h>
//#include <asm/arch/pinctrl.h>
//#include <asm/arch/scu_ast2500.h>
//#include <dm/pinctrl.h>
//#include <asm/arch/pinctrl_aspeed.h>

#ifdef CONFIG_PINCTRL
/*
 * This driver works with very simple configuration that has the same name
 * for group and function. This way it is compatible with the Linux Kernel
 * driver.
 */

struct ast2500_pinctrl_priv {
	struct ast2500_scu *scu;
};

static int ast2500_pinctrl_probe(struct udevice *dev)
{
	struct ast2500_pinctrl_priv *priv = dev_get_priv(dev);

	priv->scu = ast_get_scu();

	return 0;
}

struct ast2500_group_config {
	char *group_name;
	/* Control register number (1-10) */
	unsigned reg_num;
	/* The mask of control bits in the register */
	u32 ctrl_bit_mask;
};

static const struct ast2500_group_config ast2500_groups[] = {
	{ "I2C1", 8, (1 << 13) | (1 << 12) },
	{ "I2C2", 8, (1 << 15) | (1 << 14) },
	{ "I2C3", 8, (1 << 16) },
	{ "I2C4", 5, (1 << 17) },
	{ "I2C4", 5, (1 << 17) },
	{ "I2C5", 5, (1 << 18) },
	{ "I2C6", 5, (1 << 19) },
	{ "I2C7", 5, (1 << 20) },
	{ "I2C8", 5, (1 << 21) },
	{ "I2C9", 5, (1 << 22) },
	{ "I2C10", 5, (1 << 23) },
	{ "I2C11", 5, (1 << 24) },
	{ "I2C12", 5, (1 << 25) },
	{ "I2C13", 5, (1 << 26) },
	{ "I2C14", 5, (1 << 27) },
	{ "MAC1LINK", 1, (1 << 0) },
	{ "MDIO1", 3, (1 << 31) | (1 << 30) },
	{ "MAC2LINK", 1, (1 << 1) },
	{ "MDIO2", 5, (1 << 2) },
};

static int ast2500_pinctrl_get_groups_count(struct udevice *dev)
{
	debug("PINCTRL: get_(functions/groups)_count\n");

	return ARRAY_SIZE(ast2500_groups);
}

static const char *ast2500_pinctrl_get_group_name(struct udevice *dev,
						  unsigned selector)
{
	debug("PINCTRL: get_(function/group)_name %u\n", selector);

	return ast2500_groups[selector].group_name;
}

static int ast2500_pinctrl_group_set(struct udevice *dev, unsigned selector,
				     unsigned func_selector)
{
	struct ast2500_pinctrl_priv *priv = dev_get_priv(dev);
	const struct ast2500_group_config *config;
	u32 *ctrl_reg;

	debug("PINCTRL: group_set <%u, %u>\n", selector, func_selector);
	if (selector >= ARRAY_SIZE(ast2500_groups))
		return -EINVAL;

	config = &ast2500_groups[selector];
	if (config->reg_num > 6)
		ctrl_reg = &priv->scu->pinmux_ctrl1[config->reg_num - 7];
	else
		ctrl_reg = &priv->scu->pinmux_ctrl[config->reg_num - 1];

	ast_scu_unlock(priv->scu);
	setbits_le32(ctrl_reg, config->ctrl_bit_mask);
	ast_scu_lock(priv->scu);

	return 0;
}

static struct pinctrl_ops ast2500_pinctrl_ops = {
	.set_state = pinctrl_generic_set_state,
	.get_groups_count = ast2500_pinctrl_get_groups_count,
	.get_group_name = ast2500_pinctrl_get_group_name,
	.get_functions_count = ast2500_pinctrl_get_groups_count,
	.get_function_name = ast2500_pinctrl_get_group_name,
	.pinmux_group_set = ast2500_pinctrl_group_set,
};

static const struct udevice_id ast2500_pinctrl_ids[] = {
	{ .compatible = "aspeed,ast2500-pinctrl" },
	{ .compatible = "aspeed,g5-pinctrl" },
	{ }
};

U_BOOT_DRIVER(pinctrl_ast2500) = {
	.name = "aspeed_ast2500_pinctrl",
	.id = UCLASS_PINCTRL,
	.of_match = ast2500_pinctrl_ids,
	.priv_auto_alloc_size = sizeof(struct ast2500_pinctrl_priv),
	.ops = &ast2500_pinctrl_ops,
	.probe = ast2500_pinctrl_probe,
};
#else
struct aspeed_pinctrl_group_config {
	char *group_name;
	u32 reg;
	u32 bit_mask;
	u32 reg_unmask;
	u32 bit_unmask;
};

#if defined(CONFIG_MACH_ASPEED_G6) 
static struct aspeed_pinctrl_group_config ast2600_pin_groups[] = {
	{ "MAC1LINK", ASPEED_SCU_BASE + 0x410, BIT(4) },
	{ "MAC2LINK", ASPEED_SCU_BASE + 0x410, BIT(5) },
	{ "MAC3LINK", ASPEED_SCU_BASE + 0x410, BIT(6) },
	{ "MAC4LINK", ASPEED_SCU_BASE + 0x410, BIT(7) },	
//	{ "MDIO1", ASPEED_SCU_BASE + 0x88, BIT(31) | BIT(30) },	
	{ "MDIO2", ASPEED_SCU_BASE + 0x410, BIT(14) | BIT(13) },
	{ "MDIO3", ASPEED_SCU_BASE + 0x410, BIT(1) | BIT(0) },
	{ "MDIO4", ASPEED_SCU_BASE + 0x410, BIT(3) | BIT(2) },	
};
#elif defined(CONFIG_MACH_ASPEED_G4) || defined(CONFIG_MACH_ASPEED_G5)
static struct aspeed_pinctrl_group_config ast2500_pin_groups[] = {
	{ "MAC1LINK", ASPEED_SCU_BASE + 0x80, BIT(0), 0, 0 },
	{ "MDIO1", ASPEED_SCU_BASE + 0x88, BIT(31) | BIT(30), 0, 0 },
	{ "MAC2LINK", ASPEED_SCU_BASE + 0x80, BIT(1), 0, 0},
	{ "MDIO2", ASPEED_SCU_BASE + 0x90, BIT(2), 0, 0 },
	{ "I2C3", ASPEED_SCU_BASE + 0x90, BIT(16), 0, 0 },
	{ "I2C4", ASPEED_SCU_BASE + 0x90, BIT(17), 0, 0 },
	{ "I2C5", ASPEED_SCU_BASE + 0x90, BIT(18), 0, 0 },
	{ "I2C6", ASPEED_SCU_BASE + 0x90, BIT(19), 0, 0 },
	{ "I2C7", ASPEED_SCU_BASE + 0x90, BIT(20), 0, 0 },
	{ "I2C8", ASPEED_SCU_BASE + 0x90, BIT(21), 0, 0 },
	{ "I2C9", ASPEED_SCU_BASE + 0x90, BIT(22), ASPEED_SCU_BASE + 0x90, BIT(6) },
	{ "I2C10", ASPEED_SCU_BASE + 0x90, BIT(23), ASPEED_SCU_BASE + 0x90, BIT(0) },
	{ "I2C11", ASPEED_SCU_BASE + 0x90, BIT(24), ASPEED_SCU_BASE + 0x90, BIT(0) },
	{ "I2C12", ASPEED_SCU_BASE + 0x90, BIT(25), ASPEED_SCU_BASE + 0x90, BIT(0) },
	{ "I2C13", ASPEED_SCU_BASE + 0x90, BIT(26), ASPEED_SCU_BASE + 0x90, BIT(0) },
	{ "I2C14", ASPEED_SCU_BASE + 0x90, BIT(27), 0, 0 },
};
#else
#err "No define for clk enable"xx
#endif

extern void aspeed_pinctrl_group_set(char *group_name)
{
	int i = 0;
#if defined(CONFIG_MACH_ASPEED_G6) 
	struct aspeed_pinctrl_group_config *pin_config = ast2600_pin_groups;
	int array_size = ARRAY_SIZE(ast2600_pin_groups);
#elif defined(CONFIG_MACH_ASPEED_G4) || defined(CONFIG_MACH_ASPEED_G5)
	struct aspeed_pinctrl_group_config *pin_config = ast2500_pin_groups;
	int array_size = ARRAY_SIZE(ast2500_pin_groups);
#else
#err "No define for clk enable"
#endif

	for(i = 0; i < array_size; i++) {
		if(!strcmp(group_name, pin_config[i].group_name )) {
			writel(readl(pin_config[i].reg) | pin_config[i].bit_mask, pin_config[i].reg);
			if(pin_config[i].reg_unmask) 
				writel(readl(pin_config[i].reg_unmask) & ~pin_config[i].bit_unmask, pin_config[i].reg_unmask);
			break;
			
		}
		
	}
}
#endif

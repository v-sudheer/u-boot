/*
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <netdev.h>
#include <asm/arch/ast-scu.h>
#include <asm/arch/pinctrl_aspeed.h>
#include <asm/arch/clk_aspeed.h>
#include <asm/arch/aspeed-reset.h>
#include <asm/arch/ast-sdmc.h>
#include <asm/io.h>

DECLARE_GLOBAL_DATA_PTR;

extern int ast_sdhi_init(u32 regbase, u32 max_clk, u32 min_clk);

#if defined(CONFIG_SHOW_BOOT_PROGRESS)
void show_boot_progress(int progress)
{
    printf("Boot reached stage %d\n", progress);
}
#endif

int board_init(void)
{
	/* adress of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;
	gd->flags = 0;
	return 0;
}

int dram_init(void)
{
	gd->ram_size = ast_sdmc_dram_size();
	return 0;
}

#ifdef CONFIG_FTGMAC100
int board_eth_init(bd_t *bd)
{
	int ret = 0, i = 0;
	char *ctrl_name;
	unsigned long iobase[] = {
		AST_MAC0_BASE,
		AST_MAC1_BASE,
#ifdef AST_MAC2_BASE		
		AST_MAC2_BASE,
#endif
#ifdef AST_MAC3_BASE
		AST_MAC3_BASE,
#endif		
	};

	unsigned long mdiobase[] = {
		ASPEED_MDIO0_BASE,
		ASPEED_MDIO1_BASE,
#ifdef ASPEED_MDIO2_BASE
		ASPEED_MDIO2_BASE,
#endif
#ifdef ASPEED_MDIO3_BASE
		ASPEED_MDIO3_BASE,
#endif		
	};

#ifdef CONFIG_MACH_ASPEED_G6
	ctrl_name = "MDIO";
 	aspeed_reset_assert(ctrl_name);
 	aspeed_reset_deassert(ctrl_name);
#endif

	for(i = 0; i < ASPEED_MAC_COUNT; i++) {
		switch(i) {
			case 0:
				ctrl_name = "MAC1";
				aspeed_pinctrl_group_set("MAC1LINK");
				aspeed_pinctrl_group_set("MDIO1");
				break;
			case 1:
				ctrl_name = "MAC2";
				aspeed_pinctrl_group_set("MAC2LINK");
				aspeed_pinctrl_group_set("MDIO2");				
				break;
			case 2:
				ctrl_name = "MAC3";
				aspeed_pinctrl_group_set("MAC3LINK");
				aspeed_pinctrl_group_set("MDIO3");
				break;
			case 3:
				ctrl_name = "MAC4";
				aspeed_pinctrl_group_set("MAC4LINK");
				aspeed_pinctrl_group_set("MDIO4");				
				break;
		};
		aspeed_reset_assert(ctrl_name);
		udelay(100);
		aspeed_clk_enable(ctrl_name);
		udelay(1000);
		aspeed_reset_deassert(ctrl_name);

		ret += ftgmac100_initialize(iobase[i], mdiobase[i]);
	}

	return 0;
}

#endif

#ifdef CONFIG_GENERIC_MMC

#define CONFIG_SYS_MMC_NUM		2
#define CONFIG_SYS_MMC_BASE		{AST_SDHCI_SLOT0_BASE, AST_SDHCI_SLOT1_BASE}

int board_mmc_init(bd_t *bis)
{
	ulong mmc_base_address[CONFIG_SYS_MMC_NUM] = CONFIG_SYS_MMC_BASE;
	u8 i;

	ast_scu_init_sdhci();
	ast_scu_multi_func_sdhc_slot(3);
	//multipin. Remind: AST2300FPGA only supports one port at a time
	for (i = 0; i < CONFIG_SYS_MMC_NUM; i++) {
		if (ast_sdhi_init(mmc_base_address[i], aspeed_get_sd_clk_rate(), 100000))
			return 1;
	}
	return 0;
}
#endif

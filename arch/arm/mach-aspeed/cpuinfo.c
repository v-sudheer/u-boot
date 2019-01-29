/*
 * This file is released under the terms of GPL v2 and any later version.
 * See the file COPYING in the root directory of the source tree for details.
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <asm/arch/clk_aspeed.h>
#include <asm/arch/ast-scu.h>
#include <asm/arch/ast-sdmc.h>
#include <asm/arch/aspeed_scu_info.h>

#if defined(CONFIG_DISPLAY_CPUINFO)
int print_cpuinfo(void)
{
	char buf[32];
	int i = 0;
	ulong size = 0;

	aspeed_get_revision_id();

	aspeed_sys_reset_info();

	aspeed_security_info();

	printf("PLL :   %4s MHz\n", strmhz(buf, aspeed_get_clk_in_rate()));
	printf("CPU :   %4s MHz\n", strmhz(buf, aspeed_get_hpll_clk_rate()));
	printf("MPLL :	%4s MHz, ECC: %s, ",
	       strmhz(buf, aspeed_get_mpll_clk_rate()),
	       ast_sdmc_get_ecc() ? "Enable" : "Disable");
	if(ast_sdmc_get_ecc())
		printf("recover %d, un-recover %d, ", ast_sdmc_get_ecc_recover_count(), ast_sdmc_get_ecc_unrecover_count());
	if(ast_sdmc_get_ecc())
		printf("Size : %d MB, ", ast_sdmc_get_ecc_size()/1024/1024);

#if defined(CONFIG_MACH_ASPEED_G5)
	printf("Cache: %s ",ast_sdmc_get_cache() ? "Enable" : "Disable");
#endif
	aspeed_who_init_dram();

	size = ast_sdmc_get_vram_size();

	puts("VGA :    ");
	print_size(size, "- ");

	size = ast_sdmc_get_mem_size();
	puts("Total DRAM : ");
	print_size(size, "\n");

	aspeed_2nd_wdt_mode();

	aspeed_spi_strap_mode();

	aspeed_espi_mode();

	puts("Eth :    ");
	for(i = 0; i < ASPEED_MAC_COUNT; i++) {
		printf("MAC%d: %s ",i, aspeed_get_mac_phy_interface(i) ? "RGMII" : "RMII/NCSI");
		if(i != (ASPEED_MAC_COUNT - 1))
			printf(",");
	}
	puts("\n");

	return 0;
}
#endif

#if defined(CONFIG_MACH_ASPEED_G6)
int arch_cpu_init(void)
{
	//unlock 13 scu
	writel(0x1688a8a8, 0x1e6e2010);

	return 0;
}
#endif

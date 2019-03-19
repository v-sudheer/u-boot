/*
 * This file is released under the terms of GPL v2 and any later version.
 * See the file COPYING in the root directory of the source tree for details.
 */

#include <common.h>
#include <command.h>
#include <asm/io.h>
#include <asm/arch/ast-scu.h>
#include <asm/arch/ast-sdmc.h>

#if defined(CONFIG_DISPLAY_CPUINFO)
int print_cpuinfo(void)
{
	char buf[32];

#ifdef AST_SOC_CAM
#ifdef CONFIG_FPGA_ASPEED
	printf("AST1220 FPGA\n");
#else
	ast_scu_revision_id();

	ast_scu_sys_rest_info();

	printf("PLL :   %4s MHz\n", strmhz(buf, ast_get_clk_source()));
	printf("CPU :   %4s MHz\n", strmhz(buf, ast_get_h_pll_clk()));
	printf("MEM :	%4s MHz\n", strmhz(buf, ast_get_m_pll_clk() * 2));
#endif

#else
	ulong size = 0;

	ast_scu_revision_id();

	ast_scu_sys_rest_info();

#ifdef CONFIG_MACH_ASPEED_G5
	ast_scu_security_info();
#endif
	printf("PLL :   %4s MHz\n", strmhz(buf, ast_get_clk_source()));
	printf("CPU :   %4s MHz\n", strmhz(buf, ast_get_h_pll_clk()));
#if defined(CONFIG_MACH_ASPEED_G5)
	printf("MEM :	%4s MHz, ECC: %s, ",
	       strmhz(buf, ast_get_m_pll_clk() * 2),
	       ast_sdmc_get_ecc() ? "Enable" : "Disable");

	if(ast_sdmc_get_ecc())
		printf("recover %d, un-recover %d", ast_sdmc_get_ecc_recover_count(), ast_sdmc_get_ecc_unrecover_count());

	if(ast_sdmc_get_ecc())
		printf("Size : %d MB, ", ast_sdmc_get_ecc_size()/1024/1024);
	printf("Cache: %s ",ast_sdmc_get_cache() ? "Enable" : "Disable");
#else
	printf("MEM :   %4s MHz, EEC:%s ",
	       strmhz(buf, ast_get_m_pll_clk()),
	       ast_sdmc_get_ecc() ? "Enable" : "Disable");

	if(ast_sdmc_get_ecc())
		printf("recover %d, un-recover %d", ast_sdmc_get_ecc_recover_count(), ast_sdmc_get_ecc_unrecover_count());

	if(ast_sdmc_get_ecc())
		printf("Size : %d MB, ", ast_sdmc_get_ecc_size()/1024/1024);
#endif
	ast_scu_get_who_init_dram();

	size = ast_scu_get_vga_memsize();

	puts("VGA :    ");
	print_size(size, ", ");

	puts("VIDEO : ");
	print_size(CONFIG_AST_VIDEO_SIZE, " - ");

	size = ast_sdmc_get_mem_size();
	puts("Total DRAM : ");
	print_size(size, "\n");

	if(ast_scu_2nd_wdt_mode()) {
		puts("2nd Boot : Enable\n");
	}

	if(ast_scu_espi_mode())
		printf("eSPI Mode : SuperIO-%02x\n", ast_scu_get_superio_addr_config());
	else
		printf("LPC Mode : SuperIO-%02x\n", ast_scu_get_superio_addr_config());
#endif

	return 0;
}
#endif

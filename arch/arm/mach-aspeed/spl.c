/*
 * (C) Copyright 2030 ASPEED Technology Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <common.h>
#include <debug_uart.h>
#include <spl.h>

#include <asm/io.h>
#include <asm/spl.h>


DECLARE_GLOBAL_DATA_PTR;

void board_init_f(ulong dummy)
{
	int ret;

//	board_early_init_f();

	/* Example code showing how to enable the debug UART on MVEBU */
	ret = spl_init();
	if (ret) {
		debug("spl_init() failed: %d\n", ret);
		hang();
	}

	/* Use special translation offset for SPL */
//	dm_set_translation_offset(0xd0000000 - 0xf1000000);

	preloader_console_init();

	timer_init();

	/*
	 * Return to the BootROM to continue the Marvell xmodem
	 * UART boot protocol. As initiated by the kwboot tool.
	 *
	 * This can only be done by the BootROM and not by the
	 * U-Boot SPL infrastructure, since the beginning of the
	 * image is already read and interpreted by the BootROM.
	 * SPL has no chance to receive this information. So we
	 * need to return to the BootROM to enable this xmodem
	 * UART download.
	 */
//	if (get_boot_device() == BOOT_DEVICE_UART)
//		return_to_bootrom();
}

#ifdef CONFIG_SPL_BOARD_INIT
void spl_board_init(void)
{
	preloader_console_init();
	board_init();
}
#endif

u32 spl_boot_device(void)
{
	u32 mode = 0;
#if 0
	switch ((zynq_slcr_get_boot_mode()) & ZYNQ_BM_MASK) {
#ifdef CONFIG_SPL_SPI_SUPPORT
	case ZYNQ_BM_QSPI:
		puts("qspi boot\n");
		mode = BOOT_DEVICE_SPI;
		break;
#endif
	case ZYNQ_BM_NAND:
		mode = BOOT_DEVICE_NAND;
		break;
	case ZYNQ_BM_NOR:
		mode = BOOT_DEVICE_NOR;
		break;
#ifdef CONFIG_SPL_MMC_SUPPORT
	case ZYNQ_BM_SD:
		puts("mmc boot\n");
		mode = BOOT_DEVICE_MMC1;
		break;
#endif
	case ZYNQ_BM_JTAG:
		mode = BOOT_DEVICE_RAM;
		break;
	default:
		puts("Unsupported boot mode selected\n");
		hang();
	}
#endif
	return mode;
}

#ifdef CONFIG_SPL_MMC_SUPPORT
u32 spl_boot_mode(const u32 boot_device)
{
	return MMCSD_MODE_FS;
}
#endif

#ifdef CONFIG_SPL_OS_BOOT
int spl_start_uboot(void)
{
	/* boot linux */
	return 0;
}
#endif

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	/* Just empty function now - can't decide what to choose */
	debug("%s: %s\n", __func__, name);

	return 0;
}
#endif


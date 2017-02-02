#include <common.h>
#include <malloc.h>
#include <sdhci.h>
#include <errno.h>
#include <malloc.h>
#include <memalign.h>
#include <mmc.h>

int ast_sdhi_init(u32 regbase, u32 max_clk, u32 min_clk)
{
	struct sdhci_host *host = NULL;
	host = (struct sdhci_host *)calloc(1, sizeof(struct sdhci_host));
	if (!host) {
		printf("sdh_host malloc fail!\n");
		return 1;
	}
	host->voltages = MMC_VDD_32_33 | MMC_VDD_33_34 | MMC_VDD_165_195;

//	host->host_caps = MMC_MODE_HS;

	host->name = "ast_sdhci";
	host->ioaddr = (void *)regbase;
#if 1	
	host->quirks = SDHCI_QUIRK_NO_HISPD_BIT | SDHCI_QUIRK_BROKEN_VOLTAGE |
		SDHCI_QUIRK_BROKEN_R1B | SDHCI_QUIRK_32BIT_DMA_ADDR | SDHCI_QUIRK_REG32_RW |
		SDHCI_QUIRK_WAIT_SEND_CMD;
#else
host->quirks = SDHCI_QUIRK_NO_HISPD_BIT | SDHCI_QUIRK_BROKEN_VOLTAGE |
	SDHCI_QUIRK_BROKEN_R1B | SDHCI_QUIRK_REG32_RW |
	SDHCI_QUIRK_WAIT_SEND_CMD;

#endif
	if (host->quirks & SDHCI_QUIRK_REG32_RW)
		host->version = sdhci_readl(host, SDHCI_HOST_VERSION - 2) >> 16;
	else
		host->version = sdhci_readw(host, SDHCI_HOST_VERSION);

	add_sdhci(host, max_clk, min_clk);
	
	return 0;
}


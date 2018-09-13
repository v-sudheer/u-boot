/*
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <miiphy.h>
#include <phy.h>
#include <asm/io.h>
#include <linux/errno.h>

/*
 * PHY control register
 */
#define FTGMAC100_PHYCR_MDC_CYCTHR_MASK	0x3f
#define FTGMAC100_PHYCR_MDC_CYCTHR(x)	((x) & 0x3f)
#define FTGMAC100_PHYCR_PHYAD(x)	(((x) & 0x1f) << 16)
#define FTGMAC100_PHYCR_REGAD(x)	(((x) & 0x1f) << 21)
#define FTGMAC100_PHYCR_MIIRD		(1 << 26)
#define FTGMAC100_PHYCR_MIIWR		(1 << 27)

#ifdef CONFIG_MACH_ASPEED_G6
//G6 MDC/MDIO 
#define FTGMAC100_PHYCR_NEW_FIRE		BIT(31)
#define FTGMAC100_PHYCR_ST_22			BIT(28)
#define FTGMAC100_PHYCR_NEW_WRITE		BIT(26)
#define FTGMAC100_PHYCR_NEW_READ		BIT(27)
#define FTGMAC100_PHYCR_NEW_WDATA(x)	(x & 0xffff)
#define FTGMAC100_PHYCR_NEW_PHYAD(x)	(((x) & 0x1f) << 21)
#define FTGMAC100_PHYCR_NEW_REGAD(x)	(((x) & 0x1f) << 16)
#else
//New MDC/MDIO 
#define FTGMAC100_PHYCR_NEW_FIRE		BIT(15)
#define FTGMAC100_PHYCR_ST_22			BIT(12)
#define FTGMAC100_PHYCR_NEW_WRITE		BIT(10)
#define FTGMAC100_PHYCR_NEW_READ		BIT(11)
#define FTGMAC100_PHYCR_NEW_WDATA(x)	((x & 0xffff) << 16)
#define FTGMAC100_PHYCR_NEW_PHYAD(x)	(((x) & 0x1f) << 5)
#define FTGMAC100_PHYCR_NEW_REGAD(x)	((x) & 0x1f)
#endif

/*
 * PHY data register
 */
#define FTGMAC100_PHYDATA_MIIWDATA(x)		((x) & 0xffff)
#define FTGMAC100_PHYDATA_MIIRDATA(phydata)	(((phydata) >> 16) & 0xffff)

#define FTGMAC100_PHYDATA_NEW_MIIWDATA(x)		((x) & 0xffff)

struct aspeed_mdio_regs {
	unsigned int	phycr;
	unsigned int	phydata;
};

static int aspeed_mdio_read(struct mii_dev *bus, int addr, int devad, int reg)
{
	struct aspeed_mdio_regs __iomem *mdio_regs = (struct aspeed_mdio_regs __iomem *)bus->priv;
	int phycr;
	int i;
	
	//Use New MDC and MDIO interface
#if 1
	phycr = FTGMAC100_PHYCR_NEW_FIRE | FTGMAC100_PHYCR_ST_22 | FTGMAC100_PHYCR_NEW_READ |
			FTGMAC100_PHYCR_NEW_PHYAD(addr) | // 20141114
			FTGMAC100_PHYCR_NEW_REGAD(reg); // 20141114
	
	writel(phycr, &mdio_regs->phycr);
	
	for (i = 0; i < 10; i++) {
		phycr = readl(&mdio_regs->phycr);
	
		if ((phycr & FTGMAC100_PHYCR_NEW_FIRE) == 0) {
			int data;
	
			data = readl(&mdio_regs->phydata);
			return FTGMAC100_PHYDATA_NEW_MIIWDATA(data);
		}
	
		mdelay(10);
	}
#else
	phycr = readl(&mdio_regs->phycr);

	/* preserve MDC cycle threshold */
//	phycr &= FTGMAC100_PHYCR_MDC_CYCTHR_MASK;

	phycr = FTGMAC100_PHYCR_PHYAD(addr)
		  |  FTGMAC100_PHYCR_REGAD(reg)
		  |  FTGMAC100_PHYCR_MIIRD | 0x34;

	writel(phycr, &mdio_regs->phycr);

	for (i = 0; i < 10; i++) {
		phycr = readl(&mdio_regs->phycr);

		if ((phycr & FTGMAC100_PHYCR_MIIRD) == 0) {
			int data;

			data = readl(&mdio_regs->phydata);
			return FTGMAC100_PHYDATA_MIIRDATA(data);
		}

		mdelay(10);
	}
#endif
	debug("mdio read timed out\n");
	return -1;

}

static int aspeed_mdio_write(struct mii_dev *bus, int addr, int devad, int reg,
			u16 val)
{
	struct aspeed_mdio_regs __iomem *mdio_regs = (struct aspeed_mdio_regs __iomem *)bus->priv;
	int phycr;
	int i;
	
#ifdef CONFIG_MACH_ASPEED_G4
	int data;

	phycr = readl(&mdio_regs->phycr);

	/* preserve MDC cycle threshold */
//	phycr &= FTGMAC100_PHYCR_MDC_CYCTHR_MASK;

	phycr = FTGMAC100_PHYCR_PHYAD(addr)
		  |  FTGMAC100_PHYCR_REGAD(reg)
		  |  FTGMAC100_PHYCR_MIIWR | 0x34;

	data = FTGMAC100_PHYDATA_MIIWDATA(val);

	writel(data, &mdio_regs->phydata);
	writel(phycr, &mdio_regs->phycr);

	for (i = 0; i < 10; i++) {
		phycr = readl(&mdio_regs->phycr);

		if ((phycr & FTGMAC100_PHYCR_MIIWR) == 0) {
			debug("(phycr & FTGMAC100_PHYCR_MIIWR) == 0: " \
				"phy_addr: %x\n", addr);
			return 0;
		}

		mdelay(1);
	}
#else

	phycr = FTGMAC100_PHYCR_NEW_WDATA(val) |
			FTGMAC100_PHYCR_NEW_FIRE | FTGMAC100_PHYCR_ST_22 |
			FTGMAC100_PHYCR_NEW_WRITE |
			FTGMAC100_PHYCR_NEW_PHYAD(addr) | // 20141114
			FTGMAC100_PHYCR_NEW_REGAD(reg); // 20141114
	
	writel(phycr, &mdio_regs->phycr);
	
	for (i = 0; i < 10; i++) {
		phycr = readl(&mdio_regs->phycr);
	
		if ((phycr & FTGMAC100_PHYCR_NEW_FIRE) == 0) {
			debug("(phycr & FTGMAC100_PHYCR_MIIWR) == 0: " \
				"phy_addr: %x\n", addr);
			return 0;
		}
	
		mdelay(10);
	}
#endif
	debug("mdio write timed out\n");

	return -1;
}

static int aspeed_mdio_reset(struct mii_dev *bus)
{
//	struct eth_device *dev = bus->priv;
//	printf("aspeed_mdio_reset \n");
	return 0;
}

extern int aspeed_mdio_init(const char *name, ulong ioaddr)
{
	struct mii_dev *bus = mdio_alloc();

	if (!bus) {
		printf("Failed to allocate MDIO bus\n");
		return -ENOMEM;
	}
	
	bus->read = aspeed_mdio_read;
	bus->write = aspeed_mdio_write;
	bus->reset = aspeed_mdio_reset;
	snprintf(bus->name, sizeof(bus->name), "%s", name);	

	bus->priv = (void *) ioaddr;

	return mdio_register(bus);
}

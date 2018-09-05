// SPDX-License-Identifier: GPL-2.0+
/*
 * Faraday FTGMAC100 Ethernet
 *
 * (C) Copyright 2009 Faraday Technology
 * Po-Yu Chuang <ratbert@faraday-tech.com>
 *
 * (C) Copyright 2010 Andes Technology
 * Macpaul Lin <macpaul@andestech.com>
 */

#include <config.h>
#include <common.h>
#include <malloc.h>
#include <net.h>
#include <miiphy.h>
#include <asm/io.h>
#include <asm/dma-mapping.h>
#include <linux/mii.h>

#include "ftgmac100.h"

#define ETH_ZLEN	60
#define CFG_XBUF_SIZE	1536

/* RBSR - hw default init value is also 0x640 */
#define RBSR_DEFAULT_VALUE	0x640

/* PKTBUFSTX/PKTBUFSRX must both be power of 2 */
#define PKTBUFSTX	4	/* must be power of 2 */

struct ftgmac100_data {
	ulong txdes_dma;
	struct ftgmac100_txdes *txdes;
	ulong rxdes_dma;
	struct ftgmac100_rxdes *rxdes;
	int tx_index;
	int rx_index;
	struct mii_dev *bus;
	struct phy_device *phydev;
};

/*
 * struct mii_bus functions
 */
static int ftgmac100_mdiobus_read(struct eth_device *dev, int phy_addr,
	int regnum)
{
	struct ftgmac100 *ftgmac100 = (struct ftgmac100 *)dev->iobase;
	int fear0;
	int phycr;
	int i;

	fear0 = __raw_readl(&ftgmac100->fear0);
	if(fear0 & (1 << 31)) {	//New MDC/MDIO
		phycr = FTGMAC100_PHYCR_NEW_FIRE | FTGMAC100_PHYCR_ST_22 | FTGMAC100_PHYCR_NEW_READ |
				FTGMAC100_PHYCR_NEW_PHYAD(phy_addr) | // 20141114
				FTGMAC100_PHYCR_NEW_REGAD(regnum); // 20141114
	
		__raw_writel(phycr, &ftgmac100->phycr);
	
		for (i = 0; i < 10; i++) {
			phycr = __raw_readl(&ftgmac100->phycr);
		
			if ((phycr & FTGMAC100_PHYCR_NEW_FIRE) == 0) {
				int data;
		
				data = __raw_readl(&ftgmac100->phydata);
				return FTGMAC100_PHYDATA_NEW_MIIWDATA(data);
			}
		
			mdelay(10);
		}
	
		debug("mdio read timed out\n");
		return -1;

	} else {
		phycr = __raw_readl(&ftgmac100->phycr);

		/* preserve MDC cycle threshold */
	//	phycr &= FTGMAC100_PHYCR_MDC_CYCTHR_MASK;

		phycr = FTGMAC100_PHYCR_PHYAD(phy_addr)
		      |  FTGMAC100_PHYCR_REGAD(regnum)
		      |  FTGMAC100_PHYCR_MIIRD | 0x34;

		writel(phycr, &ftgmac100->phycr);

		for (i = 0; i < 10; i++) {
			phycr = readl(&ftgmac100->phycr);

			if ((phycr & FTGMAC100_PHYCR_MIIRD) == 0) {
				int data;

				data = readl(&ftgmac100->phydata);
				return FTGMAC100_PHYDATA_MIIRDATA(data);
			}

			mdelay(10);
		}

		debug("mdio read timed out\n");
		return -1;
	}
}

static int ftgmac100_mdiobus_write(struct eth_device *dev, int phy_addr,
	int regnum, u16 value)
{
	struct ftgmac100 *ftgmac100 = (struct ftgmac100 *)dev->iobase;
	int fear0;	
	int phycr;
	int data;
	int i;

	fear0 = readl(&ftgmac100->fear0);
	if(fear0 & (1 << 31)) {	//New MDC/MDIO
		phycr = (value << 16) |
				FTGMAC100_PHYCR_NEW_FIRE | FTGMAC100_PHYCR_ST_22 |
				FTGMAC100_PHYCR_NEW_WRITE |
				FTGMAC100_PHYCR_NEW_PHYAD(phy_addr) | // 20141114
				FTGMAC100_PHYCR_NEW_REGAD(regnum); // 20141114
	
		writel(phycr, &ftgmac100->phycr);
	
		for (i = 0; i < 10; i++) {
			phycr = __raw_readl(&ftgmac100->phycr);
		
			if ((phycr & FTGMAC100_PHYCR_NEW_FIRE) == 0) {
				debug("(phycr & FTGMAC100_PHYCR_MIIWR) == 0: " \
					"phy_addr: %x\n", phy_addr);
				return 0;
			}
		
			mdelay(10);
		}
	
		debug("mdio write timed out\n");
		return -1;
	
	} else {
	
		phycr = readl(&ftgmac100->phycr);

		/* preserve MDC cycle threshold */
	//	phycr &= FTGMAC100_PHYCR_MDC_CYCTHR_MASK;

		phycr = FTGMAC100_PHYCR_PHYAD(phy_addr)
		      |  FTGMAC100_PHYCR_REGAD(regnum)
		      |  FTGMAC100_PHYCR_MIIWR | 0x34;

		data = FTGMAC100_PHYDATA_MIIWDATA(value);

		writel(data, &ftgmac100->phydata);
		writel(phycr, &ftgmac100->phycr);

		for (i = 0; i < 10; i++) {
			phycr = readl(&ftgmac100->phycr);

			if ((phycr & FTGMAC100_PHYCR_MIIWR) == 0) {
				debug("(phycr & FTGMAC100_PHYCR_MIIWR) == 0: " \
					"phy_addr: %x\n", phy_addr);
				return 0;
			}

			mdelay(1);
		}

		debug("mdio write timed out\n");
		return -1;
	}
}

int ftgmac100_phy_read(struct eth_device *dev, int addr, int reg, u16 *value)
{
	*value = ftgmac100_mdiobus_read(dev , addr, reg);

	if (*value == -1)
		return -1;

	return 0;
}

int  ftgmac100_phy_write(struct eth_device *dev, int addr, int reg, u16 value)
{
	if (ftgmac100_mdiobus_write(dev, addr, reg, value) == -1)
		return -1;

	return 0;
}

static int ftgmac100_phy_reset(struct eth_device *dev)
{
	struct ftgmac100_data *priv = dev->priv;
	int i;
	u16 status, adv;

	adv = ADVERTISE_CSMA | ADVERTISE_ALL;

	ftgmac100_phy_write(dev, priv->phydev->addr, MII_ADVERTISE, adv);

	printf("%s: Starting autonegotiation...\n", dev->name);

	ftgmac100_phy_write(dev, priv->phydev->addr,
		MII_BMCR, (BMCR_ANENABLE | BMCR_ANRESTART));

	for (i = 0; i < 100000 / 100; i++) {
		ftgmac100_phy_read(dev, priv->phydev->addr, MII_BMSR, &status);

		if (status & BMSR_ANEGCOMPLETE)
			break;
		mdelay(1);
	}

	if (status & BMSR_ANEGCOMPLETE) {
		printf("%s: Autonegotiation complete\n", dev->name);
	} else {
		printf("%s: Autonegotiation timed out (status=0x%04x)\n",
		       dev->name, status);
		return 0;
	}

	return 1;
}

#define CONFIG_PHY_MAX_ADDR	32
#define CONFIG_FTGMAC100_EGIGA

static int ftgmac100_phy_init(struct eth_device *dev)
{
	struct ftgmac100_data *priv = dev->priv;

	int phy_addr;
//	u32 phy_id;
	u16 phy_id1, status, adv, lpa, stat_ge;
	u16 phy_id2;
	int media, speed, duplex;
	int i;

	/* Check if the PHY is up to snuff... */
	for (phy_addr = 0; phy_addr < CONFIG_PHY_MAX_ADDR; phy_addr++) {

		ftgmac100_phy_read(dev, phy_addr, MII_PHYSID1, &phy_id1);
		ftgmac100_phy_read(dev, phy_addr, MII_PHYSID2, &phy_id2);

		/*
		 * When it is unable to found PHY,
		 * the interface usually return 0xffff or 0x0000
		 */
		if (phy_id1 != 0xffff && phy_id1 != 0x0) {
//			printf("%s: found PHY at 0x%02x\n",
//				dev->name, phy_addr);
			priv->phydev->addr = phy_addr;
			break;
		}
	}

	if (phy_id1 == 0xffff || phy_id1 == 0x0) {
		printf("%s: no PHY present\n", dev->name);
		return 0;
	}

	ftgmac100_phy_read(dev, priv->phydev->addr, MII_BMSR, &status);

//	printf("phy_id : %x , status %x \n",((phy_id1 << 16) | phy_id2), status );

	if (!(status & BMSR_LSTATUS)) {
		/* Try to re-negotiate if we don't have link already. */
		ftgmac100_phy_reset(dev);

		for (i = 0; i < 100000 / 100; i++) {
			ftgmac100_phy_read(dev, priv->phydev->addr,
				MII_BMSR, &status);
			if (status & BMSR_LSTATUS)
				break;
			udelay(100);
		}
	}

	if (!(status & BMSR_LSTATUS)) {
		printf("%s: link down\n", dev->name);
		return 0;
	}

#ifdef CONFIG_FTGMAC100_EGIGA
	/* 1000 Base-T Status Register */
	ftgmac100_phy_read(dev, priv->phydev->addr,
		MII_STAT1000, &stat_ge);

	speed = (stat_ge & (LPA_1000FULL | LPA_1000HALF)
		 ? 1 : 0);

	duplex = ((stat_ge & LPA_1000FULL)
		 ? 1 : 0);

	if (speed) { /* Speed is 1000 */
		printf("%s: link up, 1000bps %s-duplex\n",
			dev->name, duplex ? "full" : "half");
		return 0;
	}
#endif

	ftgmac100_phy_read(dev, priv->phydev->addr, MII_ADVERTISE, &adv);
	ftgmac100_phy_read(dev, priv->phydev->addr, MII_LPA, &lpa);

	media = mii_nway_result(lpa & adv);
	speed = (media & (ADVERTISE_100FULL | ADVERTISE_100HALF) ? 1 : 0);
	duplex = (media & ADVERTISE_FULL) ? 1 : 0;

	printf("%s: link up, %sMbps %s-duplex\n",
	       dev->name, speed ? "100" : "10", duplex ? "full" : "half");

	return 1;
}

static int ftgmac100_update_link_speed(struct eth_device *dev)
{
	struct ftgmac100 *ftgmac100 = (struct ftgmac100 *)dev->iobase;
	struct ftgmac100_data *priv = dev->priv;

	unsigned short stat_fe;
	unsigned short stat_ge;
	unsigned int maccr;

#ifdef CONFIG_FTGMAC100_EGIGA
	/* 1000 Base-T Status Register */
	ftgmac100_phy_read(dev, priv->phydev->addr, MII_STAT1000, &stat_ge);
#endif

	ftgmac100_phy_read(dev, priv->phydev->addr, MII_BMSR, &stat_fe);

	if (!(stat_fe & BMSR_LSTATUS))	/* link status up? */
		return 0;

	/* read MAC control register and clear related bits */
	maccr = readl(&ftgmac100->maccr) &
		~(FTGMAC100_MACCR_GIGA_MODE |
		  FTGMAC100_MACCR_FAST_MODE |
		  FTGMAC100_MACCR_FULLDUP);

#ifdef CONFIG_FTGMAC100_EGIGA
	if (stat_ge & LPA_1000FULL) {
		/* set gmac for 1000BaseTX and Full Duplex */
		maccr |= FTGMAC100_MACCR_GIGA_MODE | FTGMAC100_MACCR_FULLDUP;
	}

	if (stat_ge & LPA_1000HALF) {
		/* set gmac for 1000BaseTX and Half Duplex */
		maccr |= FTGMAC100_MACCR_GIGA_MODE;
	}
#endif

	if (stat_fe & BMSR_100FULL) {
		/* set MII for 100BaseTX and Full Duplex */
		maccr |= FTGMAC100_MACCR_FAST_MODE | FTGMAC100_MACCR_FULLDUP;
	}

	if (stat_fe & BMSR_10FULL) {
		/* set MII for 10BaseT and Full Duplex */
		maccr |= FTGMAC100_MACCR_FULLDUP;
	}

	if (stat_fe & BMSR_100HALF) {
		/* set MII for 100BaseTX and Half Duplex */
		maccr |= FTGMAC100_MACCR_FAST_MODE;
	}

	if (stat_fe & BMSR_10HALF) {
		/* set MII for 10BaseT and Half Duplex */
		/* we have already clear these bits, do nothing */
		;
	}

	/* update MII config into maccr */
	writel(maccr, &ftgmac100->maccr);

	return 1;
}

/*
 * Reset MAC
 */
static void ftgmac100_reset(struct eth_device *dev)
{
	struct ftgmac100 *ftgmac100 = (struct ftgmac100 *)dev->iobase;

	debug("%s()\n", __func__);

	//Ryan modify
	writel(readl(&ftgmac100->maccr) | FTGMAC100_MACCR_SW_RST, &ftgmac100->maccr);

	while (readl(&ftgmac100->maccr) & FTGMAC100_MACCR_SW_RST);
	
	//Use New MDC and MDIO interface
	writel(readl(&ftgmac100->fear0) | 0x80000000, &ftgmac100->fear0);
}

/*
 * Set MAC address
 */
static void ftgmac100_set_mac(struct eth_device *dev,
	const unsigned char *mac)
{
	struct ftgmac100 *ftgmac100 = (struct ftgmac100 *)dev->iobase;
	unsigned int maddr = mac[0] << 8 | mac[1];
	unsigned int laddr = mac[2] << 24 | mac[3] << 16 | mac[4] << 8 | mac[5];

	debug("%s(%x %x)\n", __func__, maddr, laddr);

	writel(maddr, &ftgmac100->mac_madr);
	writel(laddr, &ftgmac100->mac_ladr);
}

static int ftgmac100_write_hwaddr(struct eth_device *dev)
{
	ftgmac100_set_mac(dev, dev->enetaddr);
	return 0;
}

static void ftgmac100_set_mac_from_env(struct eth_device *dev)
{
	eth_getenv_enetaddr("ethaddr", dev->enetaddr);

	ftgmac100_set_mac(dev, dev->enetaddr);
}

/*
 * disable transmitter, receiver
 */
static void ftgmac100_halt(struct eth_device *dev)
{
	struct ftgmac100 *ftgmac100 = (struct ftgmac100 *)dev->iobase;

	debug("%s()\n", __func__);

//	writel(0, &ftgmac100->maccr);
	//ryan modify
	writel(__raw_readl(&ftgmac100->maccr) & ~(FTGMAC100_MACCR_TXDMA_EN | FTGMAC100_MACCR_RXDMA_EN | FTGMAC100_MACCR_TXMAC_EN | FTGMAC100_MACCR_RXMAC_EN), &ftgmac100->maccr);

}

static int ftgmac100_phy(struct eth_device *dev)
{
	struct phy_device *phydev;
	int mask = 0xffffffff;
	struct mii_dev *bus = miiphy_get_dev_by_name(dev->name);

	phydev = phy_find_by_mask(bus, mask, 0);
	if (!phydev)
		return -ENODEV;

	phy_connect_dev(phydev, dev);

	phydev->supported &= PHY_GBIT_FEATURES;
	phydev->advertising = phydev->supported;

	phy_config(phydev);
	phy_startup(phydev);

	return 0;
}

static int ftgmac100_init(struct eth_device *dev, bd_t *bd)
{
	struct ftgmac100 *ftgmac100 = (struct ftgmac100 *)dev->iobase;
	struct ftgmac100_data *priv = dev->priv;
	struct ftgmac100_txdes *txdes;
	struct ftgmac100_rxdes *rxdes;
	unsigned int maccr;
	void *buf;
	int i;

	debug("%s()\n", __func__);

	//Ryan Add
	ftgmac100_reset(dev);

	if (!priv->txdes) {
		txdes = dma_alloc_coherent(
			sizeof(*txdes) * PKTBUFSTX, &priv->txdes_dma);
		if (!txdes)
			panic("ftgmac100: out of memory\n");
		memset(txdes, 0, sizeof(*txdes) * PKTBUFSTX);
		priv->txdes = txdes;
	}
	txdes = priv->txdes;

	if (!priv->rxdes) {
		rxdes = dma_alloc_coherent(
			sizeof(*rxdes) * PKTBUFSRX, &priv->rxdes_dma);
		if (!rxdes)
			panic("ftgmac100: out of memory\n");
		memset(rxdes, 0, sizeof(*rxdes) * PKTBUFSRX);
		priv->rxdes = rxdes;
	}
	rxdes = priv->rxdes;

	/* set the ethernet address */
	ftgmac100_set_mac_from_env(dev);

	/* disable all interrupts */
	writel(0, &ftgmac100->ier);

	/* initialize descriptors */
	priv->tx_index = 0;
	priv->rx_index = 0;

	txdes[PKTBUFSTX - 1].txdes0	= FTGMAC100_TXDES0_EDOTR;
	rxdes[PKTBUFSRX - 1].rxdes0	= FTGMAC100_RXDES0_EDORR;

	for (i = 0; i < PKTBUFSTX; i++) {
		/* TXBUF_BADR */
		if (!txdes[i].txdes2) {
			buf = memalign(ARCH_DMA_MINALIGN, CFG_XBUF_SIZE);
			if (!buf)
				panic("ftgmac100: out of memory\n");
			txdes[i].txdes3 = virt_to_phys(buf);
			txdes[i].txdes2 = (uint)buf;
		}
		txdes[i].txdes1 = 0;
	}

	for (i = 0; i < PKTBUFSRX; i++) {
		/* RXBUF_BADR */
		if (!rxdes[i].rxdes2) {
			buf = net_rx_packets[i];
			rxdes[i].rxdes3 = virt_to_phys(buf);
			rxdes[i].rxdes2 = (uint)buf;
		}
		rxdes[i].rxdes0 &= ~FTGMAC100_RXDES0_RXPKT_RDY;
	}

	/* transmit ring */
	writel(priv->txdes_dma, &ftgmac100->txr_badr);

	/* receive ring */
	writel(priv->rxdes_dma, &ftgmac100->rxr_badr);

	/* poll receive descriptor automatically */
	writel(FTGMAC100_APTC_RXPOLL_CNT(1), &ftgmac100->aptc);

	/* config receive buffer size register */
	writel(FTGMAC100_RBSR_SIZE(RBSR_DEFAULT_VALUE), &ftgmac100->rbsr);

	/* enable transmitter, receiver */
	maccr = FTGMAC100_MACCR_TXMAC_EN |
		FTGMAC100_MACCR_RXMAC_EN |
		FTGMAC100_MACCR_TXDMA_EN |
		FTGMAC100_MACCR_RXDMA_EN |
		FTGMAC100_MACCR_CRC_APD |
		FTGMAC100_MACCR_FULLDUP |
		FTGMAC100_MACCR_RX_RUNT |
		FTGMAC100_MACCR_RX_BROADPKT;

	writel(maccr, &ftgmac100->maccr);

//Ryan modify
#if 1
	ftgmac100_phy_init(dev);
	if (!ftgmac100_update_link_speed(dev))
		return -1;
#else
	if (!ftgmac100_phy_init(dev)) {
		if (!ftgmac100_update_link_speed(dev))
			return -1;
	}
#endif

	return 0;
}

/*
 * Get a data block via Ethernet
 */
static int ftgmac100_recv(struct eth_device *dev)
{
	struct ftgmac100_data *priv = dev->priv;
	struct ftgmac100_rxdes *curr_des;
	unsigned short rxlen;

	curr_des = &priv->rxdes[priv->rx_index];

	if (!(curr_des->rxdes0 & FTGMAC100_RXDES0_RXPKT_RDY))
		return -1;

	if (curr_des->rxdes0 & (FTGMAC100_RXDES0_RX_ERR |
				FTGMAC100_RXDES0_CRC_ERR |
				FTGMAC100_RXDES0_FTL |
				FTGMAC100_RXDES0_RUNT |
				FTGMAC100_RXDES0_RX_ODD_NB)) {
		return -1;
	}

	rxlen = FTGMAC100_RXDES0_VDBC(curr_des->rxdes0);

	debug("%s(): RX buffer %d, %x received\n",
	       __func__, priv->rx_index, rxlen);

	/* invalidate d-cache */
	dma_map_single((void *)curr_des->rxdes2, rxlen, DMA_FROM_DEVICE);

	/* pass the packet up to the protocol layers. */
	net_process_received_packet((void *)curr_des->rxdes3, rxlen);

	/* release buffer to DMA */
	curr_des->rxdes0 &= ~FTGMAC100_RXDES0_RXPKT_RDY;

	priv->rx_index = (priv->rx_index + 1) % PKTBUFSRX;

	return 0;
}

/*
 * Send a data block via Ethernet
 */
static int ftgmac100_send(struct eth_device *dev, void *packet, int length)
{
	struct ftgmac100 *ftgmac100 = (struct ftgmac100 *)dev->iobase;
	struct ftgmac100_data *priv = dev->priv;
	struct ftgmac100_txdes *curr_des = &priv->txdes[priv->tx_index];

	if (curr_des->txdes0 & FTGMAC100_TXDES0_TXDMA_OWN) {
		debug("%s(): no TX descriptor available\n", __func__);
		return -1;
	}

	debug("%s(%x, %x)\n", __func__, (int)packet, length);

	length = (length < ETH_ZLEN) ? ETH_ZLEN : length;

	memcpy((void *)curr_des->txdes2, (void *)packet, length);
	dma_map_single((void *)curr_des->txdes2, length, DMA_TO_DEVICE);

	/* only one descriptor on TXBUF */
	curr_des->txdes0 &= FTGMAC100_TXDES0_EDOTR;
	curr_des->txdes0 |= FTGMAC100_TXDES0_FTS |
			    FTGMAC100_TXDES0_LTS |
			    FTGMAC100_TXDES0_TXBUF_SIZE(length) |
			    FTGMAC100_TXDES0_TXDMA_OWN ;

	/* start transmit */
	writel(1, &ftgmac100->txpd);

	debug("%s(): packet sent\n", __func__);

	priv->tx_index = (priv->tx_index + 1) % PKTBUFSTX;

	return 0;
}

static int ftgmac100_mdio_read(struct mii_dev *bus, int addr, int devad, int reg)
{
	struct eth_device *dev = bus->priv;
	return ftgmac100_mdiobus_read(dev, addr, reg);
}

static int ftgmac100_mdio_write(struct mii_dev *bus, int addr, int devad, int reg,
			u16 val)
{
	struct eth_device *dev = bus->priv;
	ftgmac100_mdiobus_write(dev, addr, reg, val);
	return 0;
}

static int ftgmac100_mdio_reset(struct mii_dev *bus)
{
	struct eth_device *dev = bus->priv;
//	ftgmac100_phy_reset(dev);
	return 0;
}

static int ftgmac100_mdio_init(const char *name, void *priv)
{
	struct mii_dev *bus = mdio_alloc();

	if (!bus) {
		printf("Failed to allocate MDIO bus\n");
		return -ENOMEM;
	}

	bus->read = ftgmac100_mdio_read;
	bus->write = ftgmac100_mdio_write;
	snprintf(bus->name, sizeof(bus->name), "%s", name);
	bus->reset = ftgmac100_mdio_reset;

	bus->priv = priv;

	return mdio_register(bus);
}

int ftgmac100_initialize(unsigned long base_addr)
{
	struct eth_device *dev;
	struct ftgmac100_data *priv;
	char *ctrl_name;

	dev = malloc(sizeof *dev);
	if (!dev) {
		printf("%s(): failed to allocate dev\n", __func__);
		goto out;
	}

	/* Transmit and receive descriptors should align to 16 bytes */
	priv = memalign(16, sizeof(struct ftgmac100_data));
	if (!priv) {
		printf("%s(): failed to allocate priv\n", __func__);
		goto free_dev;
	}

	memset(dev, 0, sizeof(*dev));
	memset(priv, 0, sizeof(*priv));

	sprintf(dev->name, "FTGMAC100.@0x%8lx", base_addr);
	dev->iobase	= base_addr;
	dev->init	= ftgmac100_init;
	dev->halt	= ftgmac100_halt;
	dev->send	= ftgmac100_send;
	dev->recv	= ftgmac100_recv;
	dev->priv	= priv;
	dev->write_hwaddr = ftgmac100_write_hwaddr;		//20130209, ryan chen add
	
	eth_register(dev);

	switch(dev->index) {
		case 0:
			ctrl_name = "MAC1";
			break;
		case 1:
			ctrl_name = "MAC2";
			break;
		case 2:
			ctrl_name = "MAC3";
			break;
		case 3:
			ctrl_name = "MAC4";
			break;
	}

	aspeed_reset_assert(ctrl_name);
	udelay(100);
	aspeed_clk_enable(ctrl_name);
	udelay(1000);
	aspeed_reset_deassert(ctrl_name);


	ftgmac100_reset(dev);

	ftgmac100_mdio_init(dev->name, dev);

	return 0;

free_dev:
	free(dev);
out:
	return 1;

}

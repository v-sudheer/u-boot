/*
    Copyright (C) 2009-2011 American Megatrends Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

/*
    Copyright (c) 2009-2011, Intel Corporation.

    Modications made by Intel Corporation under the terms of the
    GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option)
    any later version.
*/


/* Tulip based Ethernet MAC 10/100 Controller for U-boot (1.1.4)*/
/*
    Copyright (c) 2010-2014, Emulex Corporation.

    Modifications made by Emulex  Corporation under the terms of the
    GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option)
    any later version.
*/



#include <config.h>
#include <common.h>
#if defined (CONFIG_COMMANDS) && defined (CONFIG_CMD_NET)
//#if defined (CONFIG_CMD_NET)
#include <malloc.h>
#include <command.h>
#include <miiphy.h>
//#include "soc_hw.h"
#include <configs/pilot4_map.h>
#include <configs/pilot4_hw.h>
#include "pilotmac.h"
#include "pilotmacreg.h"
#include <net.h>

#define DEFAULT_CSR11  (CSR11_TT_VAL(0)| CSR11_NTP_VAL(0) | CSR11_RT_VAL(0) | CSR11_NRP_VAL(0))
#define DEFAULT_CSR6   ((CSR6_TR_VAL(0))|(CSR6_SF))
#define DEFAULT_CSR0   (CSR0_BAR_RXH | CSR0_DSL_VAL(0) | CSR0_PBL_VAL(0) | CSR0_TAP_VAL(0))

#define NCSI_PHY_ADDR                   0xff    // Indicates no phy for NC-SI channels

#define RX_PBL_REG	0x40100A08
#define TXPBL		0x0
#define RXPBL		0x0

extern int ast_miiphy_write (const char *devname, unsigned char addr, unsigned char reg, unsigned short value);
extern int ast_miiphy_read (const char *devname, unsigned char addr, unsigned char reg,  unsigned short *value);
extern int GetLinkStatus(char *devname);

#ifndef PROD_IS_PILOT
extern int PDK_ProductIsBlade(void);
#endif

extern int GetPhyAddr(char *devname, unsigned char *addr);
extern int GetPhySpeed(char *devname);
extern int GetPhyDuplex(char *devname);
extern int  GetDevNum(char *devname);

#define inl(addr) 			(*((volatile u32 *)(addr)))
#define inw(addr)			(*((volatile u16 *)(addr)))
#define outl(value, addr)  	(*((volatile u32 *)(addr)) = value)
#define outb(value, addr)	(*((volatile u8 *)(addr)) = value)

#define virt2phys(x)   (x)

//#define ASTMAC_DEBUG

#define ETH_ZLEN 60

extern int eth_init(bd_t *bd);
extern void eth_halt(void);
extern int eth_rx(void);
//extern int eth_send(volatile void *packet, int length);


/* Notes:
      Functions beginning with ast_mac100_ interfaces with hardware
	  Functions beginning with ast_eth_ interfaces with U-boot
*/

static  int  ast_mac100_open(struct eth_device *dev);
static void  ast_mac100_close(struct eth_device *dev);
static void  ast_mac100_reset(struct eth_device *dev);
static int 	 ast_mac100_alloc_buffers(AST_ETH_PRIV_DATA *lp);
static void  ast_mac100_enable( struct eth_device *dev );
static int   ast_mac100_recv(struct eth_device *dev);
static  int  ast_mac100_send_packet(struct eth_device *dev, void *packet, int length);

static int 	 ast_eth_init(struct eth_device *dev,bd_t *bd);
static int   ast_eth_rx(struct eth_device *dev);
static void  ast_eth_halt(struct eth_device *dev);
static int   ast_eth_send(struct eth_device *dev, void *packet, int length);

#ifdef  ASTMAC_DEBUG
void
ast_mac100_dump(char *Mesg,struct eth_device *dev)
{
	unsigned int ioaddr;
	AST_ETH_PRIV_DATA *lp;
	int i;

	lp = (AST_ETH_PRIV_DATA *)dev->priv;
	ioaddr = lp->hw_addr;

	printf("-----------------------------------------------------\n");
	printf("         AST MAC Dump : %s\n",Mesg);
	printf("-----------------------------------------------------\n");

	printf("CSR0 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR0)& 0xFF);
	printf("CSR1 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR1));
	printf("CSR2 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR2));
	printf("CSR3 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR3));
	printf("CSR4 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR4));
	printf("CSR5 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR5)& 0x7FFFFF);
	printf("CSR6 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR6));
	printf("CSR7 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR7)& 0x1FFFF);
	printf("CSR8 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR8));
	printf("CSR11 = 0x%08X\n",inl(ioaddr+AST_MAC_CSR11));

	for(i=0;i<RXDES_NUM;i++)
	{
		printf("RX Desc[%d].Status  = 0x%08x\n",i, lp->rx_descs[i].Status);
		printf("RX Desc[%d].Length  = 0x%08x\n",i, lp->rx_descs[i].Length);
		printf("RX Desc[%d].Buffer1 = 0x%08x\n",i, lp->rx_descs[i].Buffer1);
		printf("RX Desc[%d].Buffer2 = 0x%08x\n",i, lp->rx_descs[i].Buffer2);
	}

	for(i=0;i<TXDES_NUM;i++)
	{
		printf("TX Desc[%d].Status  = 0x%08x\n",i, lp->tx_descs[i].Status);
		printf("TX Desc[%d].Length  = 0x%08x\n",i, lp->tx_descs[i].Length);
		printf("TX Desc[%d].Buffer1 = 0x%08x\n",i, lp->tx_descs[i].Buffer1);
		printf("TX Desc[%d].Buffer2 = 0x%08x\n",i, lp->tx_descs[i].Buffer2);
	}
	return;
}
#else
#define ast_mac100_dump(dev)
#endif

static
int
ast_mac100_open(struct eth_device *dev)
{
#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Open\n",dev->name);
#endif

	/* Rset the hardware */
	ast_mac100_reset( dev );
//	ast_mac100_dump("After Reset",dev);

	/* Enable device for communication */
	ast_mac100_enable( dev );

//	ast_mac100_dump("After Open",dev);
	return 0;
}


static
void
ast_mac100_reset_rings(struct eth_device *dev)
{
	AST_ETH_PRIV_DATA *lp;
	int i;

#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Reset Rings\n",dev->name);
#endif

	lp = (AST_ETH_PRIV_DATA *)dev->priv;

	lp->rx_idx = 0;
	lp->tx_idx = 0;

	for(i=0;i<RXDES_NUM;i++)
	{
		lp->rx_descs[i].Status = RDESC0_OWN;
		lp->rx_descs[i].Buffer1 = (u32)&(lp->rx_buf_dma[i*RX_BUF_SIZE]);
		lp->rx_descs[i].Length = RX_BUF_SIZE | RDESC1_RCH ;
//		lp->rx_descs[i].Length = RX_BUF_SIZE ;
		if ((i+1) != RXDES_NUM)
			lp->rx_descs[i].Buffer2 = (u32)&(lp->rx_descs_dma[i+1]);
	//		lp->rx_descs[i].Buffer2 = 0;
		else
		{
			lp->rx_descs[i].Buffer2 = (u32)&(lp->rx_descs_dma[0]);
	//		lp->rx_descs[i].Buffer2 = 0 ;
			lp->rx_descs[i].Length |= RDESC1_RER ;
		}
	}

	for(i=0;i<TXDES_NUM;i++)
	{
		lp->tx_descs[i].Status = 0;
		lp->tx_descs[i].Length = TDESC1_TCH ;
	//	lp->tx_descs[i].Length = 0 ;
		lp->tx_descs[i].Buffer1 = (u32)&(lp->tx_buf_dma[i*TX_BUF_SIZE]);
		if ((i+1) != TXDES_NUM)
			lp->tx_descs[i].Buffer2 = (u32)&(lp->tx_descs_dma[i+1]);
		//	lp->tx_descs[i].Buffer2 = 0 ;
		else
		{
			lp->tx_descs[i].Buffer2 = (u32)&(lp->tx_descs_dma[0]);
		//	lp->tx_descs[i].Buffer2 =  0 ;
			lp->tx_descs[i].Length |= TDESC1_TER | TDESC1_TCH ;
		//	lp->tx_descs[i].Length |= TDESC1_TER ;
		}
	}


	return;
}

static
void
ast_mac100_close(struct eth_device *dev)
{
	unsigned int ioaddr;
	AST_ETH_PRIV_DATA *hw;
	u32 val;
	volatile int timeout = 1000000;

#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Close\n",dev->name);
#endif

	hw = (AST_ETH_PRIV_DATA *)dev->priv;
	ioaddr = hw->hw_addr;

	/*Clear interrupts */
	outl(0, ioaddr + AST_MAC_CSR7 );

	/* Disable Transmission and Receive if it is started */
	val = inl(ioaddr + AST_MAC_CSR6);
	if (val & (CSR6_ST | CSR6_SR))
	{
		val &= (~(CSR6_ST | CSR6_SR));
		outl(val,ioaddr + AST_MAC_CSR6);

		/* Wait until the process state becomes stopped */
#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Wating for xmit and recv process to stop ...\n",dev->name);
#endif
		do
		{
			val = inl(ioaddr+AST_MAC_CSR5);
			/* Before the MAC stops receive processes, recv descriptor will
			   become full and it enters to suspend mode. In this condition,
    	       we cannot stop recv process. So clear all descriptors */
			if (val & CSR5_RU)
			{
				ast_mac100_recv(dev);
				outl(CSR5_RU,ioaddr+AST_MAC_CSR5);
				outl(0,ioaddr + AST_MAC_CSR2);
			}
#ifdef  ASTMAC_DEBUG
			printf(".");
#endif
			val &= (CSR5_TS_MSK | CSR5_RS_MSK);

			/* Patch : if Xmit is stopped and Receive buffer is available. exit */
			if (((val & CSR5_TS_MSK) == CSR5_TS_ST) && (!(val & CSR5_RU)))
				break;
				break;

				timeout--;
				if(timeout <= 0) 
				{
					printf("%s %d Condition NOT met even after wait!!! Breaking while loop!!!\n", __FUNCTION__, __LINE__);
#ifdef CONFIG_PANIC_AND_BAIL_OUT
					panic("");
#endif
					break;
				}
				udelay(1);
		}
		while (val != (CSR5_TS_ST| CSR5_RS_ST));
#ifdef  ASTMAC_DEBUG
		printf("Stopped\n");
#endif
	}

	return;
}


static
void
ast_mac100_reset(struct eth_device *dev)
{
	unsigned int ioaddr;
	volatile int timeout = 1000000;
	u32 val;
	AST_ETH_PRIV_DATA *hw;

#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Reset\n",dev->name);
#endif

	hw = (AST_ETH_PRIV_DATA *)dev->priv;
	ioaddr = hw->hw_addr;
//Issue stop tx/rx and do s/w reset
	outl(0,hw->hw_addr+AST_MAC_CSR6);
	val = inl(hw->hw_addr+AST_MAC_CSR5);
	val |= 0x1FFFF;
	outl(val,hw->hw_addr+AST_MAC_CSR5);
	udelay(100);	/* Wait for stabilize */

	outl(CSR0_SWR, ioaddr + AST_MAC_CSR0 );
	val=inl(ioaddr + AST_MAC_CSR0 );
	while(val&CSR0_SWR)
	{
		val=inl(ioaddr + AST_MAC_CSR0 );
		timeout--;
		if(timeout <= 0) 
		{
			printf("%s %d Condition NOT met even after wait!!! Breaking while loop!!!\n", __FUNCTION__, __LINE__);
#ifdef CONFIG_PANIC_AND_BAIL_OUT
			panic("");
#endif
			break;
		}
		udelay(1);
	}

	return;
}


static
int
ast_mac100_alloc_buffers(AST_ETH_PRIV_DATA *lp)
{
#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:Alloc Buffers\n");
#endif
	/*---------------------------------- Receive Descriptors ----------------------------------*/
	/* Allocate Memory for Descriptors */
#ifndef CONFIG_P4_ZYNQ_FPGA
	lp->rx_descs = malloc( sizeof(RECV_DESC)*(RXDES_NUM+1));
#else
	lp->rx_descs = 0x85000000;
#endif
//	printf("rx_descs size=0x%x, addr=0x%x\n", (sizeof(RECV_DESC)*(RXDES_NUM+1)), lp->rx_descs);
	if (lp->rx_descs == NULL)
	{
		printf("ERROR: Receive Ring Descriptor allocation error\n");
		return -1;
	}

	/* Align to the 16 byte Boundary */
	lp->rx_descs =  (RECV_DESC *)((int)(((char *)lp->rx_descs)+sizeof(RECV_DESC)-1)&0xfffffff0);

	/* Get Physical Address for DMA */
	lp->rx_descs_dma = virt2phys(lp->rx_descs);

	/* Clear out Descriptors */
	memset((void *)lp->rx_descs, 0, sizeof(RECV_DESC)*RXDES_NUM);


	/* Allocate Memory for Buffers */
#ifndef CONFIG_P4_ZYNQ_FPGA
	lp->rx_buf = malloc(RX_BUF_SIZE*(RXDES_NUM+1));
#else
	lp->rx_buf = 0x85000200;
#endif
//	printf("rx_buf size=0x%x, addr=0x%x\n", (RX_BUF_SIZE*(RXDES_NUM+1)), lp->rx_buf);
	if (lp->rx_buf == NULL)
	{
		printf("ERROR:Receive Ring Buffer allocation error\n");
		return -1;
	}

	/* Align to the 64 bit Boundary */
	lp->rx_buf =  (char *)((int)((((char *)lp->rx_buf)+7))&0xfffffff8);

	/* Get Physical Address for DMA */
	lp->rx_buf_dma = virt2phys(lp->rx_buf);

	/*---------------------------------- Xmit Descriptors ----------------------------------*/

	/* Allocate Memory for Descriptors */
#ifndef CONFIG_P4_ZYNQ_FPGA
	lp->tx_descs = malloc( sizeof(XMIT_DESC)*(TXDES_NUM+1));
#else
	lp->tx_descs = 0x85010000;
#endif
//	printf("tx_descs size=0x%x, addr=0x%x\n", (sizeof(XMIT_DESC)*(TXDES_NUM+1)), lp->tx_descs);
	if (lp->tx_descs == NULL)
	{
		printf("ERROR:Transmit Ring Descriptors allocation error\n");
		return -1;
	}

	/* Align to the 16 byte Boundary */
	lp->tx_descs =  (XMIT_DESC *)((int)(((char *)lp->tx_descs)+sizeof(XMIT_DESC)-1)&0xfffffff0);

	/* Get Physical Address for DMA */
	lp->tx_descs_dma = virt2phys(lp->tx_descs);

	/* Clear out Descriptors */
	memset((void *)lp->tx_descs, 0, sizeof(XMIT_DESC)*TXDES_NUM);

	/* Allocate Memory for Buffers */
#ifndef CONFIG_P4_ZYNQ_FPGA
	lp->tx_buf = malloc( TX_BUF_SIZE*(TXDES_NUM+1));
#else
	lp->tx_buf = 0x85010200;
#endif
//	printf("tx_buf size=0x%x, addr=0x%x\n", (TX_BUF_SIZE*(TXDES_NUM+1)), lp->tx_buf);
	if (lp->tx_buf == NULL)
	{
		printf("ERROR:Transmit Ring Buffer allocation error\n");
		return -1;
	}

	/* Align to the 64 bit Boundary */
	lp->tx_buf =  (char *)((int)((((char *)lp->tx_buf)+7))&0xfffffff8);

	/* Get Physical Address for DMA */
	lp->tx_buf_dma = virt2phys(lp->tx_buf);

	/*---------------------------------- Setup Frame ----------------------------------*/
	/* Allocate Memory for Descriptors */
#ifndef CONFIG_P4_ZYNQ_FPGA
	lp->setup_descs = malloc(sizeof(XMIT_DESC) *2 );
#else
	lp->setup_descs = 0x85020000;
#endif
//	printf("setup_descs size=0x%x, addr=0x%x\n", (sizeof(XMIT_DESC) *2 ), lp->setup_descs);
	if (lp->setup_descs == NULL)
	{
		printf("ERROR:Setup Descriptors allocation error\n");
		return -1;
	}

	/* Align to the 16 byte Boundary */
	lp->setup_descs =  (XMIT_DESC *)((int)(((char *)lp->setup_descs)+sizeof(XMIT_DESC)-1)&0xfffffff0);

	/* Get Physical Address for DMA */
	lp->setup_descs_dma = virt2phys(lp->setup_descs);

	/* Clear out Descriptors */
	memset((void *)lp->setup_descs, 0, sizeof(XMIT_DESC));

	/* Allocate Memory for Buffers */
#ifndef CONFIG_P4_ZYNQ_FPGA
	lp->setup_frame = malloc(SETUP_FRAME_SIZE+7);
#else
	lp->setup_frame = 0x85020200;
#endif
//	printf("setup_frame size=0x%x, addr=0x%x\n", (SETUP_FRAME_SIZE+3), lp->setup_frame);
	if (lp->setup_frame == NULL)
	{
		printf("ERROR:Setup Ring  Buffer allocation error\n");
		return -1;
	}

	/* Align to the 64 bit Boundary */
	lp->setup_frame =  (char *)((int)((((char *)lp->setup_frame)+7))&0xfffffff8);

	/* Get Physical Address for DMA */
	lp->setup_frame_dma = virt2phys(lp->setup_frame);



#ifdef  ASTMAC_DEBUG
	printf("lp->rx_descs    = %x, lp->rx_descs_dma    = %x\n", lp->rx_descs, lp->rx_descs_dma);
	printf("lp->rx_buf      = %x, lp->rx_buf_dma      = %x\n", lp->rx_buf, lp->rx_buf_dma);
	printf("lp->tx_descs    = %x, lp->tx_descs_dma    = %x\n", lp->tx_descs, lp->tx_descs_dma);
	printf("lp->tx_buf      = %x, lp->tx_buf_dma      = %x\n", lp->tx_buf, lp->tx_buf_dma);
	printf("lp->setup_descs = %x, lp->setup_descs_dma = %x\n", lp->setup_descs, lp->setup_descs_dma);
	printf("lp->setup_frame = %x, lp->setup_frame_dma = %x\n", lp->setup_frame,lp->setup_frame_dma);
#endif

	return 0;
}

static
void
ast_send_setupframe( struct eth_device *dev, unsigned char *mac_addr)
{
	unsigned int ioaddr;
	AST_ETH_PRIV_DATA *lp;
	u32 val;
	//int i;

#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Send Setup Frame\n",dev->name);
#endif

	lp = (AST_ETH_PRIV_DATA *)dev->priv;
	ioaddr = lp->hw_addr;

#if 1
	/* Set all to broadcast address */
	memset((char *)(&lp->setup_frame[0]),0xFF,192);

	/* Set our MAC Address */
	lp->setup_frame[0] = mac_addr[0];
	lp->setup_frame[1] = mac_addr[1];
	lp->setup_frame[4] = mac_addr[2];
	lp->setup_frame[5] = mac_addr[3];
	lp->setup_frame[8] = mac_addr[4];
	lp->setup_frame[9] = mac_addr[5];

#ifdef PROD_IS_PILOT
	printf("Mac Addr is %x%x%x%x%x%x\n", mac_addr[0],mac_addr[1],mac_addr[2],mac_addr[3],mac_addr[4],mac_addr[5]);
#endif

/*
	for(i=0;i<SETUP_FRAME_SIZE;i++)
	{
		printf("%02X ",(unsigned char)lp->setup_frame[i]);
		if (!((i+1) % 16 ))
			printf("\n");
	}
	printf("\n");
*/


	/* Create the setup descriptor */
	lp->setup_descs[0].Length = SETUP_FRAME_SIZE |TDESC1_TER | TDESC1_PERFECT | TDESC1_SET;
	lp->setup_descs[0].Buffer1 = (u32)lp->setup_frame_dma;
	lp->setup_descs[0].Buffer2 = 0;
	lp->setup_descs[0].Status = TDESC0_OWN;

	/* Load setup descriptor address */
	outl((u32)lp->setup_descs_dma,ioaddr+ AST_MAC_CSR4);

	/* Enable xmit start */
	val = inl(ioaddr+AST_MAC_CSR6);
	val |= (CSR6_ST);
	outl(val,ioaddr + AST_MAC_CSR6);

	/* Tell the mac to do a immediate TX of  setup frame */
	outl(0,ioaddr + AST_MAC_CSR1);

	udelay(1000);
	/* Set Xmit stop */
	val = inl(ioaddr+AST_MAC_CSR6);
	val &= (~CSR6_ST);
	outl(val,ioaddr + AST_MAC_CSR6);

#else

	val = inl(ioaddr+AST_MAC_CSR6);
	val |= (CSR6_PM | CSR6_PR);			/* Set Promiscuous Mode */
	outl(val,ioaddr + AST_MAC_CSR6);
#endif
	return;
}


static
void
ast_mac100_enable( struct eth_device *dev )
{
	unsigned int ioaddr;
	AST_ETH_PRIV_DATA *lp;
	unsigned long csr6;
	u32 val;
	u32 lnkspeed;
	u32 lnkmode;

#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Enable\n",dev->name);
#endif

	lp = (AST_ETH_PRIV_DATA *)dev->priv;
	ioaddr = lp->hw_addr;

	/* Setup Default settings of registers */
	outl(DEFAULT_CSR0,ioaddr + AST_MAC_CSR0);
	outl(DEFAULT_CSR11,ioaddr + AST_MAC_CSR11);
#if 0
//prog realteck phy to provide 125 mhz clk
    ast_miiphy_write(dev->name, addr, 0x1F, 0x5);
    ast_miiphy_write(dev->name, addr, 0xc, 0x0);
    ast_miiphy_read(dev->name, addr, 1,&data);
    data|=0x80;
    ast_miiphy_write(dev->name, addr, 0x1, data);
    ast_miiphy_write(dev->name, addr, 0x1F, 0x0);

#endif
	/* Setup CSR6 for 10/100 Mbps Half/Full Duplex depending upon PHY info*/
	csr6=0;

	lnkspeed=GetPhySpeed(dev->name);

	if (lnkspeed == 2)
	  {
	    printf("Setting 1000 BASE-X\n");
	    csr6 |= ((0x1 << 16) | (0x1 << 17));

	  }
	else if(lnkspeed == 1)
	  {
	  printf("Setting 100 BASE-X\n");
	  csr6 &= ~((0x1 << 16) | (0x1 << 17));
	}
	else
	  {
	    printf("Setting 10 BASE-T\n");
	    csr6 |= (0x1 << 17);
	    csr6 &= ~(0x1 << 16);
	  }
	
	lnkmode=GetPhyDuplex(dev->name);
	if(lnkmode==1)
	  {
	    printf("Setting full duplex mode.\n");
	    csr6 |= (0x1 << 9);
	  }
	else
	  {
	    csr6 &=( ~(0x1 << 9) );
	    printf("Setting half duplex mode\n");
	  }
	
	
#if 0
	if (GetPhySpeed(dev->name) == 1)
	  
		csr6 |= CSR6_SPEED | CSR6_TTM;
		csr6 |= CSR6_TTM;
	if (GetPhyDuplex(dev->name) ==1)
		csr6 |= CSR6_FD;
#endif

	outl(DEFAULT_CSR6|csr6,ioaddr + AST_MAC_CSR6);

#ifndef	NO_DDMA
		/* Setting Tx PBL */
		val = inl(ioaddr + AST_MAC_CSR0);
		val = (val & 0xFFFFC0FF) | (TXPBL << 8);
		outl(val, ioaddr + AST_MAC_CSR0);

		/* Setting Rx PBL */
		val = inl((volatile u32 *)RX_PBL_REG);
		if(ioaddr == P4_MAC_A_BASE)
			val = (val & 0xFFC0FFFF) | (RXPBL << 16);
		if(ioaddr == P4_MAC_B_BASE)
			val = (val & 0xC0FFFFFF) | (RXPBL << 24);
		outl(val, (volatile u32 *)RX_PBL_REG);

		/* Enable DDMA for both Tx and Rx*/
		val = 0x3 << 30;
		outl(val, ioaddr + AST_MAC_CSR21);
#endif

	/* Init Ring */
	ast_mac100_reset_rings(dev);

	/* Mask interrupts */
	outl(0,ioaddr + AST_MAC_CSR7);

	/* Clear interrupts */
	val = inl(ioaddr+AST_MAC_CSR5);
	val |= 0x1FFFF;
	outl(val,ioaddr + AST_MAC_CSR5);

	/*Send Setup Frame to setup MAC address*/
	ast_send_setupframe(dev,dev->enetaddr);

	/* Load recv and send descriptors */
	outl((u32)lp->rx_descs_dma,ioaddr+ AST_MAC_CSR3);
	outl((u32)lp->tx_descs_dma,ioaddr+ AST_MAC_CSR4);

	/* Set xmit and recv to running state */
	val = inl(ioaddr+AST_MAC_CSR6);
	val |= (CSR6_ST | CSR6_SR);
	outl(val,ioaddr + AST_MAC_CSR6);

	/* Tell the MAC to acquire the receive descriptors */
	outl(0,ioaddr + AST_MAC_CSR2);

	return;
}

static int ast_prog_phy_get_speed( struct eth_device *dev )
{
#ifndef PROD_IS_PILOT
    unsigned short data;
	unsigned char addr;
    u16 phyid=0;

	if (GetPhyAddr(dev->name, &addr) != 0)
        return;

    // If no PHY, done
    if (addr == NCSI_PHY_ADDR)
        return;	

    ast_miiphy_read(dev->name, addr, MII_PHY_ID1, &phyid);

    if ((phyid & 0xfff0) == 0x1610)
    {
        printf("Applying Micrel PHY delay tuning\n");

        //Micrel clk skew programming
        // Reg 0x104 <= 0xf000
        data=0x8104;
        ast_miiphy_write(dev->name, addr, 0xb, data);
        data=0xf000;
        ast_miiphy_write(dev->name, addr, 0xc, data);

        // Reg 0x105 <= 0
        data=0x8105;
        ast_miiphy_write(dev->name, addr, 0xb, data);
        data=0x0000;
        ast_miiphy_write(dev->name, addr, 0xc, data);

        //set PILOT-3 MACRGMIICTL register to 0x313, per experimental findings for Fab1 RMM4 DNM
        *((volatile unsigned long *)(P4_SYS_CLK_VA_BASE + 0x88)) = 0x313;

        //enable micrel clk jitter work around
        ast_miiphy_read(dev->name, addr, 0x9, &data);
        data|=(0x3<<11);
        ast_miiphy_write(dev->name, addr, 0x9, data);

        //restart auto negotiation
        ast_miiphy_read(dev->name, addr, 0x0, &data);
        data|=(0x1<<9);
        ast_miiphy_write(dev->name, addr, 0x0, data);

      //suppse to wait here for link up but we cannot assume that we will get link up
      //always so added small delay for speed negotiation to go thru.
      udelay(10);


    }
    else if ((phyid & 0xfff0) == 0xc910)
    {
        // Realtek PHY. Turn off unused 125MHz clock output
        printf("Found Realtek Phy, Configuring.\n");

        ast_miiphy_read(dev->name, addr, 0x10, &data);
        data |= 0x0010;
        ast_miiphy_write(dev->name, addr, 0x10, data);

        //set PILOT-3 MACRGMIICTL register to 0x300, per experimental findings for Fab2 RMM4 DNM
        *((volatile unsigned long *)(P4_SYS_CLK_VA_BASE + 0x88)) = 0x300;

        // Configure LEDs. LED0 = Activity (at all speeds), LED1 (Amber) = 100MBps speed, LED2 (Green) = 1GBps, Off = 10MBps
        ast_miiphy_write(dev->name, addr, 0x1f, 0x0007);    // Change to extension page
        ast_miiphy_write(dev->name, addr, 0x1e, 0x002c);    // Change to page 44
        ast_miiphy_write(dev->name, addr, 0x1c, 0x0427);    // Configure LED1 & LED2
        ast_miiphy_read(dev->name, addr, 0x1a, &data);      // Configure LED0
        data &= ~0x00f0;
        data |=  0x0010;
        ast_miiphy_write(dev->name, addr, 0x1a, data);
        ast_miiphy_write(dev->name, addr, 0x1f, 0x0000);    // Return to page 0
    }
    return;

#else // PROD_IS_PILOT
  unsigned char addr;
  int devnum;
  unsigned short data;
    volatile int timeout = 1000000;

  devnum = GetDevNum(dev->name);
  u16 phyid=0;
  u32 link_chk_iterations=20;
  u32 link1_chk_iterations=20;

  switch (devnum)
    {
    case 0:
      addr = CONFIG_PHY_ADDR0;
      break;
    case 1:
      addr = CONFIG_PHY_ADDR1;
      break;
    default:
      return 0;
    }
  
  
#ifdef  ASTMAC_DEBUG
  printf("ASTMAC:(%s):prog_phy\n",dev->name);
#endif
  
  
  ast_miiphy_read(dev->name, addr, MII_PHY_ID1,&phyid);
  if(((phyid&0x3f0)>>0x4)==0x11)
    {
      printf("Found Real Tek Phy,Programme 125Mhz clk source\n");
      //prog realteck phy to provide 125 mhz clk
      ast_miiphy_write(dev->name, addr, 0x1F, 0x5);
      ast_miiphy_write(dev->name, addr, 0xc, 0x0);
      ast_miiphy_read(dev->name, addr, 1,&data);
       data|=0x80;
       ast_miiphy_write(dev->name, addr, 0x1, data);
       ast_miiphy_write(dev->name, addr, 0x1F, 0x0);
     //As per H/w set MACRGMICTRL value
     *((volatile unsigned long *)(P4_SYS_CLK_VA_BASE + 0x88)) = 0x293;
    }
  else if((((phyid&0x3f0)>>0x4)==0x21) || (((phyid&0x3f0)>>0x4)==0x22))
    {
      printf("Found Mircel phy %x %x\n",devnum,phyid);
   //Micrel Tx clk skew programming
#if 0
      data=0x104;
      ast_miiphy_write(dev->name, addr,0xb,data);
      ast_miiphy_read(dev->name, addr, 0xd, &data);
      printf("RGMII CLK and Control Pad Skew Default values %x\n",data);
      data=( (data)&(0xff0f) );
      data=( (data)|(0xf<<4));
      
      ast_miiphy_write(dev->name, addr, 0xc, data);
      data=0x8104;
      ast_miiphy_write(dev->name, addr,0xb,data);
      
      //read back and make sure that our bits are programmed correctly
      data=0x104;
      ast_miiphy_write(dev->name, addr,0xb,data);
      ast_miiphy_read(dev->name, addr, 0xd, &data);
      printf("RGMII CLK and Control Pad Skew Modified values %x\n",data);
      
      //enable micrel clk jitter work around
      ast_miiphy_read(dev->name, addr,0x9,&data);
      data|=(0x3<<11);
      ast_miiphy_write(dev->name, addr,0x9, data);
#endif
	  //set PILOT4 MACRGMIICTL register to 0x200, Otherwise we will see 
	  //Tx issues on MAC0 in 1000Mbps speed
	  *((volatile unsigned long *)(P4_SYS_CLK_VA_BASE + 0x88)) = 0x200;

//	  Issue soft reset to PHY and wait for the PHY to come out of soft reset
	  ast_miiphy_write(dev->name, addr, 0x0, 0x8000);

      ast_miiphy_read(dev->name, addr, 0x0,&data);
	  while((data&0x8000)==0x8000)
	  {
		  ast_miiphy_read(dev->name, addr, 0x0,&data);
		  timeout--;
		  if(timeout <= 0) 
		  {
			  printf("%s %d Condition NOT met even after wait!!! Breaking while loop!!!\n", __FUNCTION__, __LINE__);
#ifdef CONFIG_PANIC_AND_BAIL_OUT
			  panic("");
#endif
			  break;
		  }
		  udelay(1);
	  }
      //restart auto negotiation
      ast_miiphy_read(dev->name, addr,0x0,&data);
      data|=(0x1<<9);
      ast_miiphy_write(dev->name, addr, 0x0, data);
      //As Per Micrel's Errata ,some times it may take 10secs for linkup

      while( (link_chk_iterations)&&(GetLinkStatus(dev->name)==0x0) )
	{
	  link_chk_iterations--;
	  udelay(1000);
	}
      
      if(link_chk_iterations==0x0)
	{	
          ast_miiphy_read(dev->name, addr, 0x1, &data);
	  printf("Link Is Down For Device%x data=%x\n",devnum, data);
	}
	else{
          ast_miiphy_read(dev->name, addr, 0x1, &data);
	  printf("Link Is UP For Device%x data=%x\n",devnum, data);
	}


    }
  else if( (((phyid&0x3f0)>>0x4)==0x0f)|| (((phyid&0x3f0)>>0x4)==0x0c) )
    {
      printf("Found Smsc phy %x PhyId %x\n",devnum,phyid);
      phyid=0x8000;
      ast_miiphy_write(dev->name, addr, 0x0, phyid);
	  timeout = 1000000;
	  while(phyid==0x8000)
	  {
		  ast_miiphy_read(dev->name, addr, 0x0,&phyid);
		  timeout--;
		  if(timeout <= 0) 
		  {
			  printf("%s %d Condition NOT met even after wait!!! Breaking while loop!!!\n", __FUNCTION__, __LINE__);
#ifdef CONFIG_PANIC_AND_BAIL_OUT
			  panic("");
#endif
			  break;
		  }
		  udelay(1);
	  }
      printf("phy reset is done and autoneg started\n");
      
      ast_miiphy_write(dev->name, addr, 0x0, 0x1200);

    }
  else if( ((phyid&0x3f0)>>0x4) == 0x1D )
    {
      printf("Found Marvell phy %x PhyId %x\n",devnum,phyid);
      //restart auto negotiation
      ast_miiphy_read(dev->name, addr,0x0,&data);
      data|=(0x1<<9);
      ast_miiphy_write(dev->name, addr, 0x0, data);

      while( (link1_chk_iterations)&&(GetLinkStatus(dev->name)==0x0) )
        {
          link1_chk_iterations--;
          udelay(1000);
        }

      if(link1_chk_iterations==0x0)
        {
          ast_miiphy_read(dev->name, addr, 0x1, &data);
          printf("Link Is Down For Device%x data=%x\n",devnum, data);
        }
        else{
          ast_miiphy_read(dev->name, addr, 0x1, &data);
          printf("Link Is UP For Device%x data=%x\n",devnum, data);
        }

		/* Programming the LED's of Marvell phy */
      ast_miiphy_write(dev->name, addr, 0x16, 3); //select page 3
      ast_miiphy_write(dev->name, addr, 0x10, 0x1064);
      ast_miiphy_write(dev->name, addr, 0x12, 0x4A05);
      ast_miiphy_write(dev->name, addr, 0x16, 0); //bring back to page 0

    }
  else
    {
      printf("Phy Id %x  not matching dev_num is %x\n",phyid,devnum);
      
    }
#endif // PROD_IS_PILOT

	return 0;
}




static
int
ast_mac100_send_packet(struct eth_device *dev,  void *packet, int length)
{
	volatile XMIT_DESC *cur_desc;
	unsigned int ioaddr;
	AST_ETH_PRIV_DATA *lp;
    unsigned long count = 0;

#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Send\n",dev->name);
#endif

	lp = (AST_ETH_PRIV_DATA *)dev->priv;
	ioaddr =lp->hw_addr;

	cur_desc = &lp->tx_descs[lp->tx_idx];
	while (cur_desc->Status & TDESC0_OWN)
	{
#ifdef  ASTMAC_DEBUG
		printf("ASTMAC:(%s):Send : Transmit Busy\n",dev->name);
#endif
		udelay(10);

        if (++count >= 2000)  // 20 msec
        {
#ifdef  ASTMAC_DEBUG
            printf("ASTMAC:(%s):Send Timeout\n",dev->name);
#endif
            return -1;
        }
   	}

	/* Compute length to be transfered */
	length = ETH_ZLEN < length ? length : ETH_ZLEN;
	length = length > TX_BUF_SIZE ? TX_BUF_SIZE : length;

	/* Copy to Xmit buffer */
	memcpy((char *)cur_desc->Buffer1, packet, length);

	/* Set necessary bits and Length of data */
	cur_desc->Length = TDESC1_TCH | TDESC1_FS | TDESC1_LS |length;
//	cur_desc->Length =  TDESC1_FS | TDESC1_LS |length;

	/* If last descriptor, set end of chain */
	if (lp->tx_idx  == (TXDES_NUM -1))
		cur_desc->Length |= TDESC1_TER;


	/* Hand it over to MAC ownership */
	cur_desc->Status = TDESC0_OWN;

	/* Move curdesc ptr to next desc */
	lp->tx_idx = (lp->tx_idx+1)%TXDES_NUM;

	/* Start Xmit */
	outl(0,ioaddr + AST_MAC_CSR1);			/* Issue Xmit Poll Demand */
#if 0
	volatile unsigned int cont = 0;

	do{
		cont++;
	} while(((inl(ioaddr + AST_MAC_CSR5) & 0x1) != 0x1) && (cont < 10000));
#endif


	//printf("Packet sent\n");
	return length;
}


static
int
ast_mac100_recv(struct eth_device *dev)
{
	unsigned int ioaddr;
	int 	packet_length;
	volatile RECV_DESC *cur_desc;
	int 	cpy_length;
	int		start_idx,org_start_idx;
	int		seg_length;
	AST_ETH_PRIV_DATA *lp;
	int 	FrameFS = 0;
	uchar *data;
#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Recv\n",dev->name);
#endif
	lp= (AST_ETH_PRIV_DATA *)dev->priv;
	ioaddr = lp->hw_addr;

	packet_length = 0;
	start_idx = lp->rx_idx;

	while (1)
	{
		/* Get the current descriptor*/
		cur_desc = &lp->rx_descs[lp->rx_idx];

		/* Still owned by MAC. No data yet */
		if (cur_desc->Status & RDESC0_OWN)
		{
			/* If we already got partial frame, wait for the full frame*/
			if (FrameFS)
				continue;
			goto ReleaseDesc;
		}

		/* Got a descriptor. Increment our pointer with MAC's pointer */
		lp->rx_idx = (lp->rx_idx+1)%RXDES_NUM;

		/* Check if error is set */
		if (cur_desc->Status & RDESC0_ES)
		{
			// Software fix for PILOT-II Mac Bug 
			// Ignore Collisions on Full Duplex
			if ((cur_desc->Status & RDESC0_CS) && 
				(GetPhyDuplex(dev->name) ==1))
				cur_desc->Status &= (~RDESC0_CS);	
				
			if (cur_desc->Status & (RDESC0_CE | RDESC0_TL | RDESC0_DE | RDESC0_RF))
			{
				printf("ASTMAC:(%s) : Recv Error : 0x%x\n",dev->name,cur_desc->Status);
				goto ReleaseDesc;
			}
		}



		/* Check if we got a first descriptor */
		if (FrameFS == 0)
		{
			if (!(cur_desc->Status & RDESC0_FD))
			{
				printf("ASRMAC(%s): Recv Error : First Descriptor of Frame not got\n",dev->name);
				goto ReleaseDesc;
			}
			else
				FrameFS = 1;
		}

		/* If we get a last descriptor of the frame, get the frame length */
		if (cur_desc->Status & RDESC0_LS)
		{
			packet_length = RDESC0_FL_GET(cur_desc->Status);
#ifdef  ASTMAC_DEBUG
			printf("ASTMAC:(%s):Recevied Frame of size 0x%x\n",dev->name,packet_length);
#endif
//			printf("R\n");
			break;
		}
#ifdef  ASTMAC_DEBUG
		printf("ASTMAC:(%s):Waiting for full frame \n",dev->name,packet_length);
#endif
	}

	/* Check if valid packet */
	if (packet_length == 0)
		goto ReleaseDesc;

	/* Copy the data to U-Boot buffer */
	data = (uchar *)NetRxPackets[0];
	cpy_length = 0;
	org_start_idx = start_idx;
	for (; start_idx!=lp->rx_idx; start_idx=(start_idx+1)%RXDES_NUM)
	{
		seg_length = min(packet_length - cpy_length, RX_BUF_SIZE);
		memcpy(data+cpy_length, (char *)lp->rx_descs[start_idx].Buffer1,seg_length);
#ifdef  ASTMAC_DEBUG
		printf("ASTMAC:(%s):Copying Partial Frame of size 0x%x\n",dev->name,seg_length);
#endif
		cpy_length += seg_length;
	}
	start_idx = org_start_idx;
/*
	int z ;
	for(z=0;z<12;z++) {
		printf("%x ",data[z]);
	}
	printf("\n");
*/
	//printf("calling NetReceive\n");
	NetReceive(NetRxPackets[0], packet_length);		/* Send to U-Boot layer */
	


ReleaseDesc:
	/* Release the descriptors back to MAC */
	for (; start_idx!=lp->rx_idx; start_idx=(start_idx+1)%RXDES_NUM)
			lp->rx_descs[start_idx].Status = RDESC0_OWN;

	/* Check if Recv is suspended : Due to unavailable desc */
	if (inl(ioaddr +AST_MAC_CSR5) & CSR5_RU)
	{
#ifdef  ASTMAC_DEBUG
//		printf("ASTMAC:(%s):Recv process is in Suspended State 0x%08x\n",dev->name,inl(ioaddr+AST_MAC_CSR5));
#endif
		outl(CSR5_RU,ioaddr+AST_MAC_CSR5);		/* Clear RU Bit */
		outl(0,ioaddr + AST_MAC_CSR2);			/* Issue Recv Poll Demand */
	}
#ifdef  ASTMAC_DEBUG
//		printf("ASTMAC:(%s): CSR5 Status = 0x%x\n",dev->name,inl(ioaddr+AST_MAC_CSR5));
#endif

	return packet_length;
}

/*----------------------------------------------------------------------*/
/*    				 U-Boot Interface Functions							*/
/*----------------------------------------------------------------------*/
static int eth_init_called[MAX_P4_ETH];
static
int
ast_eth_init(struct eth_device *dev,bd_t *bd)
{
	AST_ETH_PRIV_DATA *hw;

#ifdef  ASTMAC_DEBUG
	printf("ASTMAC:(%s):Init \n",dev->name);
#endif
	hw = (AST_ETH_PRIV_DATA *)dev->priv;

	/* Allocate the required decsriptors and buffers */
	if (!(eth_init_called[hw->devnum]))
	{
		if (ast_mac100_alloc_buffers(hw) == -1)
			return -1;
	}

	ast_mac100_open(dev);
	eth_init_called[hw->devnum] = 1;
	return 1;
}


static void ast_eth_halt(struct eth_device *dev)
{
	ast_mac100_close(dev);
}

static int ast_eth_rx(struct eth_device *dev)
{
	return ast_mac100_recv(dev);
}

static int ast_eth_send(struct eth_device *dev, void *packet, int length)
{
	return ast_mac100_send_packet(dev,(void *)packet,length);
}


static int eth_initialized = 0;
//int ast_eth_initialize(bd_t *bis)
int ast_eth_initialize(bd_t *bis, int base_addr)
{
	struct eth_device *dev;
	int eth_num;
	AST_ETH_PRIV_DATA *hw;
	volatile u32 val;
	u32 i;
	unsigned char default_ethaddr[6]={0,0,0,0,0,0};
	unsigned int unique_id, temp;
	unsigned char bi_enetaddr[6];
	unsigned char bi_enet1addr[6];

#ifdef  ASTMAC_DEBUG
	printf("ASTMAC: Initialize Called\n");
#endif

	/*Clearing statistical counters*/
	outl(0, 0x405000B0);	//Clear counters after read 
	outl(0, 0x406000B0);	//Clear counters after read 
	for(i=0;i<22; i++)
	{
		val = inl(0x40500200 + (8*i));
		val = inl(0x40600200 + (8*i));
		val = inl(0x40500300 + (8*i));
		val = inl(0x40600300 + (8*i));
		val = 0;
	}
	outl(0x80000000, 0x405000B0);	//set counters to accumulate
	outl(0x80000000, 0x406000B0);	//set counters to accumulate

	if (eth_initialized != 0)
	{
		printf("ast_eth_initialize: WARNING: Already Initialized\n");
		return -1;
	}

	for(eth_num=0;eth_num < MAX_P4_ETH;eth_num++)
	{

		/* Allocate Device Structure */
		dev = (struct eth_device *) malloc(sizeof(struct eth_device));
//		printf("dev size=0x%x, addr=0x%x\n", sizeof(struct eth_device), dev);
		if (dev == NULL)
		{
			printf("ast_eth_initialize: Cannot allocate eth_device %d\n",eth_num);
			return 1;
		}
		memset(dev,0,sizeof(*dev));

		/* Allocate Our Private Strcuture */
		hw = (AST_ETH_PRIV_DATA *) malloc(sizeof (AST_ETH_PRIV_DATA));
//		printf("hw size=0x%x, addr=0x%x\n", sizeof(AST_ETH_PRIV_DATA), hw);
		if (hw == NULL)
		{
			printf("ast_eth_initialize: Cannot allocate private data %d\n",eth_num);
			return -1;
		}
		memset(hw,0,sizeof(*hw));

		switch (eth_num)
		{
#if defined (CONFIG_NET_MULTI)
#ifdef CONFIG_HAS_ETH1
			case 1:
				eth_getenv_enetaddr("eth1addr", bi_enet1addr);
				if(memcmp(default_ethaddr,bi_enet1addr,6) == 0)
				{
					unique_id = *(volatile unsigned int *)(0x40426500);
					bi_enet1addr[0]=0;
					bi_enet1addr[1]=0x16;
					bi_enet1addr[2]=0x88;
					temp = (unique_id >> 8) & 0xFF0000;
					unique_id &= ~(0xFF<<16);
					unique_id |= temp;
					unique_id = htonl(unique_id);
					unique_id = unique_id >> 8;
					memcpy((unsigned char *)&(bi_enet1addr[3]), &unique_id, 3);
					eth_setenv_enetaddr("eth1addr", bi_enet1addr);
					saveenv();
				}
				memcpy(dev->enetaddr,bi_enet1addr,6);
				//memcpy(dev->enetaddr,bis->bi_enet1addr,6);
				break;
#endif
#endif
			case 0:
			  eth_getenv_enetaddr("ethaddr", bi_enetaddr);
			  if(memcmp(default_ethaddr,bi_enetaddr,6) == 0)
			  {
				unique_id = *(volatile unsigned int *)(0x40426500);
				bi_enetaddr[0]=0;
				bi_enetaddr[1]=0x16;
				bi_enetaddr[2]=0x88;
				unique_id = htonl(unique_id);
				unique_id = unique_id >> 8;
				memcpy((unsigned char *)&(bi_enetaddr[3]), &unique_id, 3);
				eth_setenv_enetaddr("ethaddr", bi_enetaddr);
#ifndef CONFIG_HAS_ETH1
				saveenv();
#endif
			  }
			  //memcpy(dev->enetaddr,bis->bi_enetaddr,6);
			  memcpy(dev->enetaddr,bi_enetaddr,6);
			  break;
			default:
				printf("Invalid MAC %d\n",eth_num);
				return -1 ;
				break;
		}

#ifdef PROD_IS_PILOT
        if (eth_num == 1)
				hw->hw_addr = P4_MAC_B_BASE;
        else
				hw->hw_addr = P4_MAC_A_BASE;
#else   // Port A & B are switched on Intel platforms (except for King Island)

        if (PDK_ProductIsBlade())
        {
			printf("King Island port swap\n");
            if (eth_num == 1)
    				hw->hw_addr = P4_MAC_B_BASE;                    // eth1 = Eth Port B == I/O Mezz
            else
    				hw->hw_addr = P4_MAC_A_BASE;                    // eth0 = Eth Port A == baseboard NCSI
        }            
        else
        {
            if (eth_num == 1)
    				hw->hw_addr = P4_MAC_A_BASE;                    // eth1 = Eth Port A == RMM3
            else
    				hw->hw_addr = P4_MAC_B_BASE;                    // eth0 = Eth Port B == baseboard NCSI
        }
#endif

		hw->devnum = eth_num;
		eth_init_called[hw->devnum] = 0;

		sprintf (dev->name, "pilot_eth%d", eth_num);
      	dev->priv = (void *) hw;
      	dev->init = ast_eth_init;
        dev->halt = ast_eth_halt;
        dev->send = ast_eth_send;
        dev->recv = ast_eth_rx;

		/* Install IRQ Handlers here if needed */

		/* Stop Rx/Tx and Clear Pending Interrupts. 
		   This is needed because when linux reboots using 
		   warm reset, the mac is still active and interrupts 
		   will be pending. In this state if the linux network
		   driver is loaded, it crashes*/
		outl(0,hw->hw_addr+AST_MAC_CSR6);
		val = inl(hw->hw_addr+AST_MAC_CSR5);
		val |= 0x1FFFF;
		outl(val,hw->hw_addr+AST_MAC_CSR5);

#ifdef  ASTMAC_DEBUG
	printf("ASTMAC: Registering %s\n",dev->name);
#endif
		eth_register(dev);

//#if defined (CONFIG_MII) || (CONFIG_COMMANDS & CFG_CMD_MII)
#if defined (CONFIG_MII) 
		/* register mii command access routines */
		miiphy_register(dev->name, ast_miiphy_read, ast_miiphy_write);
#endif

		ast_prog_phy_get_speed(dev );

	}		/* for ethnum */



	eth_initialized = 1;
	return 1;
}


/* --------------------------------------------------------------------------------
 *  If CONFIG_NET_MULTI is not defined => Only one ethernet device, define
 *  eth_xxx functions here (eth.c will compile its eth_xxx functions)
 *---------------------------------------------------------------------------------
*/
#if !defined(CONFIG_NET_MULTI)

struct eth_device *eth_dev = NULL;

struct
eth_device *
eth_get_dev(void)
{
	return eth_dev;
}

int
eth_register(struct eth_device *dev)
{
	eth_dev = dev;
	return 0;
}

void
eth_halt(void)
{
	if (eth_dev)
		ast_eth_halt(eth_dev);
	return;
}

int
eth_init(bd_t *bis)
{
		if (!eth_initialized)
		{
			if (ast_eth_initialize(bis) == -1)
				return -1;
		}
		if (eth_dev)
			return ast_eth_init(eth_dev,bis);
		else
			return -1;
}

int
eth_send(volatile void *packet,int length)
{
		return ast_eth_send(eth_dev,packet,length);
}

int
eth_rx(void)
{
		return  ast_eth_rx(eth_dev);
}

#endif    /* !CONFIG_NET_MULTI */

#endif

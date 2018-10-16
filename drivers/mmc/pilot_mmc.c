/*******************************************************************************
 *
 * Copyright (C) 2004-2014 Emulex. All rights reserved.
 * EMULEX is a trademark of Emulex.
 * www.emulex.com
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published by the
 * Free Software Foundation.
 * This program is distributed in the hope that it will be useful. ALL EXPRESS
 * OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED
 * WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
 * NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS
 * ARE HELD TO BE LEGALLY INVALID. See the GNU General Public License for more
 * details, a copy of which can be found in the file COPYING included
 * with this package.
 *
 ********************************************************************************/
//#define  DEBUG
#include <common.h>
#include <asm/errno.h>
#include <malloc.h>
#include <mmc.h>
#include "pilot_mmc.h"

//#define PILOT_DEBUG		1
#define PILOT_MMC_DMA		1

#ifdef PILOT_MMC_DMA
volatile struct dma_desc *gstruc;
volatile struct dma_desc *astruc;
#endif

static void sd_out32(void * sd_no, u32 OutAddress, u32 Value)
{
	u32 sd = (u32)sd_no;
	OutAddress += (SD_BASEADDR + (sd * 0x1000));
	*(volatile u32 *) OutAddress = Value;
}

static u32 sd_in32(void * sd_no, u32 InAddress)
{
	volatile u32 temp=0;
	u32 sd = (u32)sd_no;

	InAddress += (SD_BASEADDR + (sd * 0x1000));
	temp = *(volatile u32 *) InAddress;
	return temp;
}

void dump_reg(struct mmc *mmc)
{
	printf("\nRegister dump\n");
	printf("SDDMAADDR= %x \n",  sd_in32(mmc->priv, SDDMAADDR));
	printf("BLKCTL = %x \n",  sd_in32(mmc->priv, BLOCKCTL));
	printf("SDHCCTL = %x \n",  sd_in32(mmc->priv, SDHCCTL));
	printf("SDCLKCTL = %x \n",  sd_in32(mmc->priv, SDCLKCTL));
	printf("RSPTMR_REG = %x \n",  sd_in32(mmc->priv, RSPTMR_REG));
	printf("CMDCTL = %x \n",  sd_in32(mmc->priv, CMDCTL));
	printf("CMDARG = %x \n",  sd_in32(mmc->priv, CMDARG));
	printf("PRSNTST = %x \n",  sd_in32(mmc->priv, PRSNTST));
	printf("LTXSTS_REG = %x \n",  sd_in32(mmc->priv, LTXSTS_REG));
	printf("LMEMSTATUS_REG = %x \n",  sd_in32(mmc->priv, LMEMSTATUS_REG));
	printf("LMEMCNT_REG = %x \n",  sd_in32(mmc->priv, LMEMCNT_REG));
	printf("LMEMADDR_REG = %x \n",  sd_in32(mmc->priv, LMEMADDR_REG));
	printf("SDINTSTS = %x \n",  sd_in32(mmc->priv, SDINTSTS));
}

/* Initialize the SD/EMMC controller */
static void init_port(struct mmc *mmc)
{
	volatile u32 temp=0;
	volatile int timeout = 1000000;
	debug("init_port mmc %d\n", (u32)mmc->priv);

	/* Reset SDHC controllers if SSP dont claim them*/
	if((*(tPVU32)0x40800718 & 0x1) != 0x1)
	{
		*(tPVU32)(SYSSRERL2) = 0x1 << 30;	//SDHC0
		*(tPVU32)(SYSSRERH2) = 0x1;		//SDHC1
	}
	*(tPVU32)(SYSSRERH2) |= 0x1 << 5; 	//EMMC

	*(tPVU32)(SYSRCR2) = 0xD01;

	udelay(5000);
	while ((*(tPVU32)(SYSRCR2) & 0x1) == 0x1)
	{
		debug("SYSRCR2 %x\n", *(tPVU32)(SYSRCR2));
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

	timeout = 1000000;

	/* Wait for the controller to come out of reset, safe side */
	do {
		sd_out32(mmc->priv, TXDTTMR_REG, 0x1854232);
		temp = sd_in32(mmc->priv, TXDTTMR_REG);

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
	} while(temp != 0x1854232);

	/* Clear control2 */
	sd_out32(mmc->priv, AUTOCMD12, 0x00);
	sd_out32(mmc->priv, SDCLKCTL, 0xFA5);
	sd_out32(mmc->priv, RXDTTMR_REG, 0x7FFFFFFF);
	/* Reset to normal 400Khz mode */
	sd_out32(mmc->priv, FSBLK_CTRL_REG, 0x0); 

	/* increase the Ncr timer */
	sd_out32(mmc->priv, RSPTMR_REG, 0xFF);
	sd_in32(mmc->priv, RSPTMR_REG);

	/* Clearing the SDHC_CTL register bit-width etc */
	sd_out32(mmc->priv, SDHCCTL, 0);

	/* Program LMEM PG REG */
	sd_out32(mmc->priv, LMEMCTL_REG, 0x2010000);
	sd_out32(mmc->priv, LMEMCFG_REG, 0x480);

	sd_out32(mmc->priv, SDINTEN, 0x1ffffff);
	sd_out32(mmc->priv, SDINTSEN, 0x1ffffff);
}

/* set command control register bits based on the response type */
int set_resp_type(tU32 resp_type)
{
	volatile tU32 cmdreg=0;

	if(resp_type == MMC_RSP_NONE)
	{
		cmdreg = SD_CMD_RESP_NONE;
	}
	else if (resp_type == MMC_RSP_R1)
	{
		cmdreg =  SD_CMD_RESP_48 | SD_CMD_INDEX | SD_CMD_CRC;
	}
	else if (resp_type == MMC_RSP_R1b)
	{
		cmdreg = SD_CMD_RESP_48_BUSY | SD_CMD_INDEX | SD_CMD_CRC;
	}
	else if (resp_type == MMC_RSP_R2)
	{
		cmdreg = SD_CMD_RESP_136;
	}
	else if (resp_type == MMC_RSP_R3)
	{
		cmdreg = SD_CMD_RESP_48;
	}
	else if (resp_type == MMC_RSP_R6)
	{
		cmdreg = SD_CMD_RESP_48 | SD_CMD_CRC;
	}
	else if (resp_type == MMC_RSP_R7)
	{
		cmdreg = SD_CMD_RESP_48 | SD_CMD_CRC;
	}
	else
	{
		printf("Unknown Response type = %x\n", resp_type);
	}
	return cmdreg;
}

int pilot_sdh_request(struct mmc *mmc, struct mmc_cmd *cmd,
		struct mmc_data *data)
{
	volatile u32 status;
	volatile u32 dma=0;
	volatile u32 cmdreg=0;
	volatile u32 temp;
	volatile int result = 0;
	volatile u32 sdhci_clkctrl;
	volatile u32 cnt=0;
	volatile u32 make_sure;
	volatile int timeout = 1000000;

	debug("\ncmdidx: %d arg: 0x%x\n", cmd->cmdidx, cmd->cmdarg);
#ifdef CONFIG_P4_ZYNQ_FPGA
	udelay(5000);
#endif

	cmd->response[0] = 0;
	cmdreg = set_resp_type(cmd->resp_type);
	cmdreg |= (cmd->cmdidx << CMDINDEX_POS);

	if(data)
	{
		/* fatwrite command was issuing a mmc write with data->blocks=0 some times, 
		 * which was making mmc writes to fail. Hence return if data->blocks=0 */
		if(data->blocks == 0)
			return 0;

		cmdreg |= SD_CMD_DATA;
	}

	/* Wait until the device is willing to accept commands */
	do {
		status = sd_in32(mmc->priv, PRSNTST);
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
	} while (status & (SD_CMD_INHIBIT|SD_DATA_INHIBIT));

	/*
	 * 512 bytes
	 * This is only relevant for data commands.
	 */
	if (cmdreg & SD_CMD_DATA)
	{
#ifdef PILOT_MMC_DMA
		gstruc->data_addr = (u32)data->dest; 
		gstruc->len = (data->blocksize * data->blocks) - 1;
		gstruc->crc_en = 0;
		if( !((cmd->cmdidx==CMD24) || (cmd->cmdidx==CMD25)) )
		{
			debug("DMA READ\n");
			gstruc->dir = 0;
		} else {
			debug("DMA WRITE\n");
			gstruc->dir = 1;
			dma=1;
		}
		gstruc->chain = 0; /* single desc used */
		gstruc->int_enb = 0;

		sd_out32(mmc->priv, SDDMAADDR, (u32)gstruc);

		cmdreg |= ENABLE_DMA;
#endif
		sdhci_clkctrl = sd_in32(mmc->priv, SDHCCTL);
		if (mmc->bus_width == 8)
		{
			debug("bus_width=8\n");
			sdhci_clkctrl |= SD_HOST_8BIT;
		}
		else if (mmc->bus_width == 4)
		{
			debug("bus_width=4\n");
			sdhci_clkctrl |= SD_HOST_4BIT;
		}
		else if (mmc->bus_width == 1)
		{
			debug("bus_width=1\n");
			sdhci_clkctrl &= ~SD_HOST_4BIT;
		}
		else
			printf("Invalid bus_width\n");

		sd_out32(mmc->priv, SDHCCTL, sdhci_clkctrl);

		if( !((cmd->cmdidx==CMD24) || (cmd->cmdidx==CMD25)) )
		{
			debug("DATA_READ cmd %d\n", cmd->cmdidx);
			cmdreg |= DATA_READ;
		}

		if (data->blocks > 1)
			cmdreg |= MULTI_BLOCK_TX | ENABLE_ACMD12;

		temp = data->blocks;
		temp = temp << 12;
		temp |= data->blocksize;
		sd_out32(mmc->priv, BLOCKCTL, temp);
		debug("blkcnt=%d, blksize=%d, blkAddr=%x, buffer=%x\n", data->blocks,
				data->blocksize, cmd->cmdarg, (u32)data->dest);
	}

	sd_out32(mmc->priv, CMDARG, cmd->cmdarg);

	timeout = 1000000;
	do {
		make_sure = sd_in32(mmc->priv, PRSNTST);
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
	} while((make_sure & 0x00F00000) != 0x00F00000);

	/* Clear all pending interrupt status */
	sd_out32(mmc->priv, SDINTSTS, 0x1ffffff);

	cmdreg &= ~(CMDINDEX);
	sd_out32(mmc->priv, CMDCTL, cmdreg);
	cmdreg |= (cmd->cmdidx << CMDINDEX_POS);
	/* Initiate the command */
	cmdreg |= SD_CMD_TRIG;
	debug("cmdreg=%x\n", cmdreg);
	sd_out32(mmc->priv, CMDCTL, cmdreg);

	if( ((cmd->cmdidx==CMD24) || (cmd->cmdidx==CMD25)) && (dma == 0))
	{
		result = do_pio_write(mmc, data->blocks, data->blocksize, cmd->cmdarg, (u32)data->dest);
	}
	else
	{
		/* Poll until operation complete */
		while (1) 
		{
			status = sd_in32(mmc->priv, SDINTSTS);
			debug("INT_STATUS = %x\n", status);
			if (status & SD_INT_ERROR)
			{
				debug("send_cmd: Error: (0x%08x) cmd: %d arg: 0x%x\n",
						status, cmd->cmdidx, cmd->cmdarg);
				if ((cmd->cmdidx == CMD17) || (cmd->cmdidx == CMD18))
				{
					printf("Interrupt status %x\n", status);
					printf("MMC: Error reading sector %d (0x%08x)\n",
							cmd->cmdarg, cmd->cmdarg);
				}
				if (status & (SD_INT_ERR_CTIMEOUT|SD_INT_ERR_DTIMEOUT))
				{
					result = TIMEOUT;
					goto exit;
				}
				else
				{
					result = COMM_ERR;
					goto exit;
				}
			}
			if (status & SD_INT_CMD_CMPL)
			{
				debug("cmd complete received\n");
				if (cmdreg & SD_CMD_DATA)
				{
#ifdef PILOT_MMC_DMA
					if (status & (SD_INT_DMA | SD_INT_TRNS_CMPL)) 
					{
						debug("DMA/transfer complete received\n");
						break;
					}
#else
					if ( (status & SD_INT_TRNS_CMPL) || (data->blocks > 1) )
					{
						debug("transfer complete received\n");
						do_pio_read(mmc, data->blocks, data->blocksize, cmd->cmdarg, (u32)data->dest);
						break;
					}
#endif
					cnt++;
					udelay(5000);
					if(cnt>20)
					{
						debug("CMD timeout exit\n");
						result = TIMEOUT;
						goto exit;
					}
				}
				else
					break;
			}
		}
	}

	if (cmd->resp_type & MMC_RSP_PRESENT)
	{
		if (cmd->resp_type & MMC_RSP_136)
		{
			/* CRC is stripped so we need to do some shifting */
			/* response type 2 */
			cmd->response[0] = sd_in32(mmc->priv, SDRESP3);
			cmd->response[1] = sd_in32(mmc->priv, SDRESP2);
			cmd->response[2] = sd_in32(mmc->priv, SDRESP1);
			cmd->response[3] = sd_in32(mmc->priv, SDRESP0);
			debug("CMD%d response[0], SDRESP3=%x\n",  cmd->cmdidx, sd_in32(mmc->priv, SDRESP3));
			debug("CMD%d response[1], SDRESP2=%x\n",  cmd->cmdidx, sd_in32(mmc->priv, SDRESP2));
			debug("CMD%d response[2], SDRESP1=%x\n",  cmd->cmdidx, sd_in32(mmc->priv, SDRESP1));
			debug("CMD%d response[3], SDRESP0=%x\n",  cmd->cmdidx, sd_in32(mmc->priv, SDRESP0));
		}
		else
		{
			/* response type 1, 1b, 3, 6 */
			cmd->response[0] = sd_in32(mmc->priv, SDRESP1);
			cmd->response[1] = sd_in32(mmc->priv, SDRESP2);
			cmd->response[2] = sd_in32(mmc->priv, SDRESP3);
			debug("CMD%d cmd->response[0], SDRESP1 = %x\n", cmd->cmdidx, cmd->response[0]);
		}
	}

exit:
	/* Clear all pending interrupt status */
	sd_out32(mmc->priv, SDINTSTS, 0x1ffffff);

	return result;
}

void pilot_sdh_set_ios(struct mmc *mmc)
{
 	volatile u32 sdhci_clkctrl;
	volatile u32 clk = PILOT_SDHC_CLK_400KHz;

	debug("%s: voltages: 0x%x clock: 0x%x bus_width: 0x%x\n",
			__func__, mmc->voltages, mmc->clock, mmc->bus_width);

	sdhci_clkctrl = sd_in32(mmc->priv, SDHCCTL);

	setCLKCTL(mmc, PILOT_SDHC_CLK_400KHz, 0x0, 0x0, 0x01);
	sd_out32(mmc->priv, RXDTTMR_REG, 0x7fffffff);
	sd_out32(mmc->priv, TXDTTMR_REG, 0x1854232);
	udelay(5000);	

	if(mmc->clock>=50000000)
	{
		debug("50Mhz\n");
		clk = PILOT_SDHC_CLK_50MHz;
		setCLKCTL(mmc, clk, 0x0, 0x0, 0x1);
		sd_out32(mmc->priv, FSBLK_CTRL_REG, 0xc0c00000);
	}
	else if(mmc->clock>=25000000)
	{
		debug("25Mhz\n");
		clk = ((sd_in32(mmc->priv, SDCLKCTL) & FSCLK_DIVISOR_BITS) >> FSCLK_DIVISOR_POS);
	
		clk = PILOT_SDHC_CLK_25MHz;
		setCLKCTL(mmc, clk, 0x0, 0x0, 0x1);
		sd_out32(mmc->priv, FSBLK_CTRL_REG, 0x0);
	}
	else if(mmc->clock>=20000000)
	{
		debug("20Mhz\n");
		clk = ((sd_in32(mmc->priv, SDCLKCTL) & FSCLK_DIVISOR_BITS) >> FSCLK_DIVISOR_POS);
	
		clk = PILOT_SDHC_CLK_20MHz;
		setCLKCTL(mmc, clk, 0x0, 0x0, 0x1);
		sd_out32(mmc->priv, FSBLK_CTRL_REG, 0x0);
	}
	else
	{
		debug("400khz\n");
		clk = ((sd_in32(mmc->priv, SDCLKCTL) & FSCLK_DIVISOR_BITS) >> FSCLK_DIVISOR_POS);

		clk = PILOT_SDHC_CLK_400KHz;
		setCLKCTL(mmc, clk, 0x0, 0x0, 0x01);
		sd_out32(mmc->priv, FSBLK_CTRL_REG, 0x0);
	}

	udelay(5000);
	setCLKCTL(mmc, clk, 0x0, 0x1, 0x01);
	udelay(5000);

	/* Configure the bus_width */
	if (mmc->bus_width == 8)
	{
		debug("8bit\n");
		sdhci_clkctrl |= SD_HOST_8BIT;
	}
	else if (mmc->bus_width == 4)
	{
		debug("4bit\n");
		sdhci_clkctrl |= SD_HOST_4BIT;
	}
	else if (mmc->bus_width == 1)
	{
		debug("1bit\n");
		sdhci_clkctrl &= ~SD_HOST_4BIT;
	}
	else
		printf("Invalid bus_width\n");

	sd_out32(mmc->priv, SDHCCTL, sdhci_clkctrl);
}

int pilot_sdh_init(struct mmc *mmc)
{
	debug("pilot_sdh_init called \n");
	init_port(mmc);
	return 0;
}

int pilot_mmc_init(bd_t *bd)
{
	int i=0;

	struct mmc *mmc[3];

#ifdef PILOT_MMC_DMA
	/* TODO: We need to free astruc. Findout a way. */
	astruc = (struct dma_desc *)malloc(sizeof(struct dma_desc) + 15);
	/* Make sure we use 16 byte alligned address atleast */
	gstruc = ((u8 *)astruc + 15);
	gstruc = (u32)(gstruc) & ~(0xF);
	debug("astru %x, gtruct %x\n", (u32)(astruc), (u32)gstruc);
#endif	
	//just initialize EMMC if SSP claims SD0 and SD1
	if((*(tPVU32)0x40800718 & 0x1) == 0x1)
	{
		i=2;
	}

	for(;i<3; i++)
	{
		mmc[i] = calloc((sizeof(struct mmc) + 10), 1);
		if (!mmc[i]) {
			return -ENOMEM;
		}
		if(i == EMMC)
		{
			sprintf(mmc[i]->name, "EMMC");
			mmc[i]->host_caps = MMC_MODE_4BIT | MMC_MODE_8BIT | MMC_MODE_HS | MMC_MODE_HC | MMC_MODE_HS_52MHz;
		}
		else
		{
			sprintf(mmc[i]->name, "SDHC");
			mmc[i]->host_caps = MMC_MODE_4BIT | MMC_MODE_HS | MMC_MODE_HC | MMC_MODE_HS_52MHz;
		}
		mmc[i]->send_cmd = pilot_sdh_request;
		mmc[i]->set_ios = pilot_sdh_set_ios;
		mmc[i]->init = pilot_sdh_init;

		mmc[i]->voltages =  MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30 | MMC_VDD_30_31
			| MMC_VDD_31_32 |MMC_VDD_32_33 | MMC_VDD_33_34; 

		mmc[i]->f_max = 50000000;
		mmc[i]->f_min = 400000;

		mmc[i]->block_dev.part_type = PART_TYPE_DOS;

		mmc[i]->priv = (void *)i;

		mmc_register(mmc[i]);
	}

	return 0;
}


void setCLKCTL(struct mmc *mmc, u16 fsclk_divisor, u8 sdclk_polarity, 
		u8 sdclk_enable, u8 sdclk_select)
{
	volatile u32 tempData;
	volatile int timeout = 1000000;

	debug("Entered %s \n", __FUNCTION__);
	tempData = sd_in32(mmc->priv, SDCLKCTL);

	//clear all the required fields
	tempData &= (~(SDCLK_SELECT));
	tempData &= (~(SDCLK_CLK_ENABLE));
	tempData &= (~(SDCLK_POLARITY));
	tempData &= (~(FSCLK_DIVISOR));

	//store all the required values
	tempData |= ((fsclk_divisor & FSCLK_DIVISOR_BITS)<<FSCLK_DIVISOR_POS);
	tempData |= ((sdclk_polarity & SDCLK_POLARITY_BITS)<<SDCLK_POLARITY_POS);
	tempData |= ((sdclk_enable & SDCLK_CLK_ENABLE_BITS)<<SDCLK_CLK_ENABLE_POS);
	tempData |= ((sdclk_select & SDCLK_SELECT_BITS)<<SDCLK_SELECT_POS);
	
#ifdef PILOT_DEBUG
	printf("Value written to CLKCTRL REG is %x \n", tempData);
#endif
	sd_out32(mmc->priv, SDCLKCTL, tempData);

	if(sdclk_enable == 0)
	{
		do{
			tempData = sd_in32(mmc->priv, SDCLKCTL);
#ifdef PILOT_DEBUG
			printf("Clock register value inside loop is %x\n", tempData);
#endif
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
		}while((tempData & 0x2) != 0x2);
	}
}

int do_pio_read(struct mmc *mmc, u32 blkcnt, u32 blklen, u32 deviceAddr, u32 hostAddr)
{
	volatile u32 tempData;
	volatile u32 hostmemAddr;
	volatile u16 LMEMAddr;
	volatile u16 LMEMBaseAddr;
	volatile u16 LMEMLimitAddr;
	volatile u16 tmp_blkcnt;
	volatile u16 LMEMPGValidCnt;
	volatile u16 LMEMCompPtr;
	volatile u16 tmp_LMEMPGValidCnt;
	volatile u16 rollover_LMEMPGValidCnt;
	volatile u16 new_blkcnt;
	volatile u16 new_blklen;
	volatile u8 rxDtResult;
	volatile u8 LMEMPGSize;
	volatile int timeout = 1000000;
#ifdef PILOT_DEBUG
	volatile u32 i;
#endif

	if ((blkcnt & 0x1ff) == 0) {
		new_blkcnt = 0x200;
	} else {
		new_blkcnt = (blkcnt & 0x1ff);
	}

	if ((blklen & 0xfff) == 0) {
		new_blklen = 0x1000;
	} else {
		new_blklen = (blklen & 0xfff);
	}

	hostmemAddr = hostAddr;
	tmp_blkcnt = 0;
	LMEMBaseAddr = 0x400;
	LMEMLimitAddr = 0xBFF;
	LMEMPGSize = 0x80;

	while (tmp_blkcnt < new_blkcnt)
	{
		rollover_LMEMPGValidCnt = 0;
#ifdef PILOT_DEBUG
		printf("Block Count is %d\n", tmp_blkcnt);
#endif
		//Check Rx Data Ready
		rxDtResult = checkRxDtRdy(mmc);
		if (rxDtResult != 0)
		{
			printf("Pio data read fail \n");
			return -1;
		}
		LMEMPGValidCnt = getLMEMPGValidCnt(mmc);
		if(LMEMPGValidCnt == 0)
		{
			debug("In %s RxDtis ready but count is ZERO\n", __FUNCTION__);
		}
		LMEMCompPtr = getLMEMPGCompletionPtr(mmc);

		tmp_LMEMPGValidCnt = 0;
		while (tmp_LMEMPGValidCnt < LMEMPGValidCnt)
		{
			if (rollover_LMEMPGValidCnt == 0)
			{
				if ((LMEMBaseAddr + ((LMEMCompPtr + (tmp_LMEMPGValidCnt * LMEMPGSize)) << 2)) > LMEMLimitAddr)
				{
					rollover_LMEMPGValidCnt = tmp_LMEMPGValidCnt;
					LMEMAddr = LMEMBaseAddr;
				}
				else
				{
					LMEMAddr = LMEMBaseAddr + ((LMEMCompPtr + (tmp_LMEMPGValidCnt * LMEMPGSize)) << 2);
				}
			}
			else
			{
				if ((LMEMBaseAddr + (((tmp_LMEMPGValidCnt - rollover_LMEMPGValidCnt) * LMEMPGSize) << 2)) > LMEMLimitAddr)
				{
					rollover_LMEMPGValidCnt = tmp_LMEMPGValidCnt;
					LMEMAddr = LMEMBaseAddr;
				}
				else
				{
					LMEMAddr = LMEMBaseAddr + (((tmp_LMEMPGValidCnt - rollover_LMEMPGValidCnt) * LMEMPGSize) << 2);
				}
			}

			StorePIODataTask(mmc, new_blklen, hostmemAddr, LMEMAddr);
#ifdef PILOT_DEBUG
			printf("New Block Length is %d\n", new_blklen);
			printf("LMEMaddr = %x \n", LMEMAddr);
			printf("hostmemaddr = %x \n", hostmemAddr);
			for(i=0;i<new_blklen/4; i++){
				printf("data = %x \n",( *(volatile u32 *)(hostmemAddr + (i*4))));
			}
#endif
			hostmemAddr += new_blklen;
			tmp_LMEMPGValidCnt = tmp_LMEMPGValidCnt + 1;
			tmp_blkcnt = tmp_blkcnt + 1;
			clearRxDtRdy(mmc);
		}
	}

	//Check CMD done
	checkCMDDatadone(mmc);
	clearBufRdReadyInt(mmc);

	tempData  = sd_in32(mmc->priv, PRSNTST);
	tempData = tempData & 0x00f00000;
	while (tempData != 0x00f00000)
	{
		tempData  = sd_in32(mmc->priv, PRSNTST);
		tempData = tempData & 0x00f00000;
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

	tmp_blkcnt = 0;
	return 0;
}

int do_pio_write(struct mmc *mmc, u32 blkcnt, u32 blklen, u32 deviceAddr, u32 hostAddr)
{
	volatile tU32 tempData;
	volatile tU32 hostmemAddr;
	volatile tU16 tmp_blkcnt;
	volatile tU16 AllocIndex;
	volatile tU16 new_blkcnt;
	volatile tU16 new_blklen;
	volatile tU16 LMEMBaseAddr;
	volatile tU16 LMEMAddr;
	volatile int ret=0;
	volatile int timeout = 1000000;

#ifdef PILOT_DEBUG
	printf("\ndo_pio_write\n");
	printf("blkcnt: %x\n", blkcnt);
	printf("blklen: %x\n", blklen);
	printf("deviceAddr: %x\n\n", deviceAddr);
	printf("hostAddr: %x\n\n", hostAddr);
#endif

	new_blkcnt = blkcnt;
	new_blklen = blklen;

	tmp_blkcnt = 0;
	hostmemAddr = hostAddr;
	LMEMBaseAddr = 0x400;

	while (tmp_blkcnt < new_blkcnt)
	{
		AllocIndex = checkLMEMPGAvailable(mmc);
		LMEMAddr = (LMEMBaseAddr + (AllocIndex << 2));
#ifdef PILOT_DEBUG
		printf("tmp_blkcnt is %x\n", tmp_blkcnt);
		printf("blkcnt is %x\n", new_blkcnt);
		printf("AllocIndex is %x\n", AllocIndex);
		printf("LMEM OFFSET Addr is %x\n", LMEMAddr);
#endif
		LoadPIODataTask(mmc, new_blklen, hostmemAddr, LMEMAddr);
		setTxDtRdy(mmc);
		hostmemAddr += new_blklen;
		tmp_blkcnt = tmp_blkcnt + 1;
	}

	//Check CMD done
	ret = checkCMDDatadone(mmc);
	clearBufWrReadyInt(mmc);

	tempData  = sd_in32(mmc->priv, PRSNTST);
	tempData = tempData & 0x00f00000;
	while (tempData != 0x00f00000)
	{
		tempData  = sd_in32(mmc->priv, PRSNTST);
		tempData = tempData & 0x00f00000;
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

	return ret;
}

tU8 checkRxDtRdy (struct mmc *mmc)
{
	volatile tU32 tempData;
	volatile tU32 readData;
	volatile tU8 vldEmpty;
	volatile int timeout = 1000000;

#ifdef PILOT_DEBUG
	printf("checkRxDtRdy\n");
#endif

	tempData  = 0x0;
	readData  = 0x0;
	vldEmpty  = 1;

	while (((tempData & 0x00000800) != 0x00000800) && (vldEmpty == 1))
	{
		tempData  = sd_in32(mmc->priv, PRSNTST);
		readData  = sd_in32(mmc->priv, LTXSTS_REG);
		vldEmpty  = ((readData & LMEM_PG_VALID_EMPTY) >> LMEM_PG_VALID_EMPTY_POS);
#ifdef PILOT_DEBUG
		printf("RxDtrdy PRSNTST_reg is %x, Vld_empty is %d,  readData is %x \n", tempData, vldEmpty, readData);
#endif
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
	return (0);
}

tU16 checkLMEMPGAvailable(struct mmc *mmc)
{
	volatile tU32 tempData;
	volatile int timeout = 1000000;
#ifdef PILOT_DEBUG
	printf("checkLMEMPGAvailable\n");
#endif

	tempData = LMEM_ALL_PAGES_ALLOCATED;
	while ((tempData & LMEM_ALL_PAGES_ALLOCATED) != 0x00000000)
	{
		tempData = sd_in32(mmc->priv, LMEMSTATUS_REG);
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
#ifdef PILOT_DEBUG
	printf("LMEMCNT_REG is %x\n", tempData);
#endif
	tempData = sd_in32(mmc->priv, LMEMADDR_REG);
#ifdef PILOT_DEBUG
	printf("LMEMADDR_REG is %x\n", tempData);
#endif
	tempData &= LMEM_FREE_PTR;
	tempData = tempData >> LMEM_FREE_PTR_POS;

	return(tempData & LMEM_FREE_PTR_BITS);
}

tU16 getLMEMPGValidCnt(struct mmc *mmc)
{
	volatile tU32 tempData;
#ifdef PILOT_DEBUG
	printf("getLMEMPGValidCnt\n");
#endif
	tempData = sd_in32(mmc->priv, LMEMCNT_REG);
	tempData &= LMEM_VALID_DATA_PAGES_CNT;
	tempData = tempData >> LMEM_VALID_DATA_PAGES_CNT_POS;
#ifdef PILOT_DEBUG
	printf("LMEMPGValidCnt is %x\n", tempData);
#endif
	return(tempData & LMEM_VALID_DATA_PAGES_CNT_BITS);
}

tU16 getLMEMPGCompletionPtr (struct mmc *mmc)
{
	volatile tU32 tempData;
#ifdef PILOT_DEBUG
	printf("getLMEMPGCompletionPtr\n");
#endif
	tempData = sd_in32(mmc->priv, LMEMSTATUS_REG);
	tempData &= LMEM_COMPLETION_PTR;
	tempData = tempData >> LMEM_COMPLETION_PTR_POS;
#ifdef PILOT_DEBUG
	printf("LMEMPGCompletionPtr is %x\n", tempData);
#endif
	return(tempData & LMEM_COMPLETION_PTR_BITS);
}

tU8 checkCMDDatadone (struct mmc *mmc)
{
	volatile tU32 tempData;
	volatile tU32 f=0x00000003;
	volatile tU32 i=0;
	volatile int timeout = 1000000;
#ifdef PILOT_DEBUG
	printf("checkCMDDatadone\n");
#endif
	tempData = 0x0;

	while (((tempData & 0x00000003) != 0x3)) 
	{
		tempData = sd_in32(mmc->priv, SDINTSTS);

		if(tempData !=0)
		{
#ifdef PILOT_DEBUG
			printf("INTERRUPT reg is %x\n", tempData);
			printf("complete flag is %x\n", f);
#endif
			f &= ~(tempData);
			sd_out32(mmc->priv, SDINTSTS, (tempData & 0x3));
			//tempData &= ~(f);
#ifdef PILOT_DEBUG
			printf("complete flag is %x\n", f);
#endif
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
		udelay(5000);
		i++;
		debug("wait %d\n", i);
		if(i==20)	//timeout
			return TIMEOUT;
		if(f == 0)
		{
			break;
		}
	}
#ifdef PILOT_DEBUG
	printf("going out %x\n", f);
#endif
	return 0;
}

void clearBufWrReadyInt(struct mmc *mmc)
{
	volatile tU32 tempData = 0;
	volatile int timeout = 1000000;
#ifdef PILOT_DEBUG
	printf("clearBufWrReadyInt1:\n");
#endif

	while (((tempData & PIO_BUF_WRITE_READY_INT) >> PIO_BUF_WRITE_READY_INT_POS) != 0x1)
	{
		tempData = sd_in32(mmc->priv, SDINTSTS);
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
	tempData &= PIO_BUF_WRITE_READY_INT;
	sd_out32(mmc->priv, SDINTSTS, tempData);
}

void clearBufRdReadyInt (struct mmc *mmc)
{
	volatile tU32 tempData = 0;
	volatile int timeout = 1000000;
#ifdef PILOT_DEBUG
	printf("clearBufRdReadyInt:\n");
#endif

	while (((tempData & PIO_BUF_READ_READY_INT) >> PIO_BUF_READ_READY_INT_POS) != 0x1)
	{
		tempData = sd_in32(mmc->priv, SDINTSTS);
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
	tempData &= PIO_BUF_READ_READY_INT;
	sd_out32(mmc->priv, SDINTSTS, tempData);
}

void setTxDtRdy (struct mmc *mmc)
{
	volatile tU32 tempData;
#ifdef PILOT_DEBUG
	printf("setTxDtRdy\n");
#endif
	tempData = sd_in32(mmc->priv, PRSNTST);
	tempData = tempData | ((0x1) << 10);
	sd_out32(mmc->priv, PRSNTST, tempData);
}

void clearRxDtRdy (struct mmc *mmc)
{
	volatile tU32 tempData;
#ifdef PILOT_DEBUG
	printf("clearRxDtRdy\n");
#endif
	tempData = sd_in32(mmc->priv, PRSNTST);
	tempData = tempData | ((0x1) << 11);
	sd_out32(mmc->priv, PRSNTST, tempData);
}

void StorePIODataTask (struct mmc *mmc, tU16 blklen, tU32 hostAddr, tU16 LMEMAddr)
{
	volatile tU16 tmp_blklen;
	volatile tU16 tmp_blklen_cnt;
	volatile tU32 tempData;
	volatile tU32 hostData;
	volatile tU32 sd_no;
	volatile tU32 LMEMFullAddr;

#ifdef PILOT_DEBUG
	printf("StorePIODataTask \n" );
#endif

	sd_no = (tU32)mmc->priv;

	tempData = SD_BASEADDR + (sd_no * 0x1000);

	LMEMFullAddr = tempData + LMEMAddr;
#ifdef PILOT_DEBUG
	printf("LMEM_FULL_BASE_ADDR is %x\n", (LMEMFullAddr));
#endif

	//Dwords
	tmp_blklen = (blklen >> 2);
	tmp_blklen_cnt = 0;
#ifdef PILOT_DEBUG
	printf("blklen is %x\n", blklen);
#endif
	while (tmp_blklen_cnt < tmp_blklen)
	{
//		printf("tmp_blklen_cnt is %x\n", tmp_blklen_cnt);
//		printf("tmp_blklen is %x\n", tmp_blklen);
		hostData = *(tPVU32)(LMEMFullAddr + (tmp_blklen_cnt << 2));
//		printf("LmemData is %x\n", hostData);
		*(tPVU8)(hostAddr + (tmp_blklen_cnt << 2) + 0) = (hostData & (0xff));
		hostData = (hostData >> 8);
		*(tPVU8)(hostAddr + (tmp_blklen_cnt << 2) + 1) = (hostData & (0xff));
		hostData = (hostData >> 8);
		*(tPVU8)(hostAddr + (tmp_blklen_cnt << 2) + 2) = (hostData & (0xff));
		hostData = (hostData >> 8);
		*(tPVU8)(hostAddr + (tmp_blklen_cnt << 2) + 3) = (hostData & (0xff));
		hostData = (hostData >> 8);

		tmp_blklen_cnt += 1;
	}
}

void LoadPIODataTask(struct mmc *mmc, tU16 blklen, tU32 hostAddr, tU16 LMEMAddr)
{
	volatile tU16 tmp_blklen;
	volatile tU16 tmp_blklen_cnt;
	volatile tU32 hostData;
	volatile tU8 hostDataByte;
	volatile tU32 tempData;
	volatile tU32 LMEMFullAddr;
	volatile tU32 sd_no;

#ifdef PILOT_DEBUG
	printf("LoadPIODataTask \n");
#endif

	sd_no = (tU32)mmc->priv;

	tempData = SD_BASEADDR + (sd_no * 0x1000);

	LMEMFullAddr = tempData + LMEMAddr;
#ifdef PILOT_DEBUG
	printf("LMEM_FULL_BASE_ADDR is %x\n", (LMEMFullAddr));
#endif

	//Dwords
	tmp_blklen = (blklen >> 2);
	tmp_blklen_cnt = 0;
#ifdef PILOT_DEBUG
	printf("blklen is %x\n", blklen);
#endif
	while (tmp_blklen_cnt < tmp_blklen)
	{
#ifdef PILOT_DATA_DEBUG
		printf("tmp_blklen_cnt is %x\n", tmp_blklen_cnt);
		printf("tmp_blklen is %x\n", tmp_blklen);
		printf("LmemAddr is %x\n", (LMEMFullAddr + (tmp_blklen_cnt << 2)));
		printf("hostAddr is %x\n", (hostAddr + (tmp_blklen_cnt << 2)));
#endif
		hostData = 0;
		hostDataByte = *(tPVU8)(hostAddr + (tmp_blklen_cnt << 2) + 0);
		hostData |= (hostDataByte << 0);
		hostDataByte = *(tPVU8)(hostAddr + (tmp_blklen_cnt << 2) + 1);
		hostData |= (hostDataByte << 8);
		hostDataByte = *(tPVU8)(hostAddr + (tmp_blklen_cnt << 2) + 2);
		hostData |= (hostDataByte << 16);
		hostDataByte = *(tPVU8)(hostAddr + (tmp_blklen_cnt << 2) + 3);
		hostData |= (hostDataByte << 24);

#ifdef PILOT_DATA_DEBUG
		printf("LmemData is %x\n", hostData);
#endif
		*(tPVU32)(LMEMFullAddr + (tmp_blklen_cnt << 2)) = hostData;
		tmp_blklen_cnt += 1;
	}
}

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

#ifndef __SDHCI_MMC_H_
#define __SDHCI_MMC_H_

typedef unsigned char tU8;
typedef unsigned short tU16;
typedef unsigned int tU32;
typedef volatile unsigned char * tPVU8;
typedef volatile unsigned short * tPVU16;
typedef volatile unsigned int * tPVU32;

#define SDHC0	0
#define SDHC1	1
#define EMMC	2

#define SYSRCR2			0x40100850
#define SYSSRERL2		0x40100854
#define SYSSRERH2		0x40100858

/* Register defines */
#define SDDMAADDR			0x00
#define BLOCKCTL	        0x04
#define CMDARG				0x08
#define CMDCTL				0x0C
#define  ENABLE_DMA			 0x01
#define  ENABLE_ACMD12		 0x04
#define  DATA_READ			 0x10
#define  MULTI_BLOCK_TX		 0x20
#define  SD_CMD_RESP_NONE    (0<<16)
#define  SD_CMD_RESP_136     (1<<16)
#define  SD_CMD_RESP_48      (2<<16)
#define  SD_CMD_RESP_48_BUSY (3<<16)
#define  SD_CMD_CRC			 (1<<19) 
#define  SD_CMD_INDEX        (1<<20)
#define  SD_CMD_DATA         (1<<21)
#define  SD_CMD_TRIG         (1<<31)

#define SDRESP0				0x10
#define SDRESP1				0x14
#define SDRESP2				0x18
#define SDRESP3				0x1C
#define PRSNTST				0x24
#define  SD_CMD_INHIBIT      0x00000001
#define  SD_DATA_INHIBIT     0x00000002
#define  SD_WRITE_ACTIVE     0x00000100
#define  SD_READ_ACTIVE      0x00000200
#define  SD_CARD_INS         0x00010000
#define  SD_CARD_DB          0x00020000
#define  SD_CARD_DPL         0x00040000
#define  SD_CARD_WP          0x00080000

#define SDHCCTL				0x28
#define  SD_HOST_8BIT		 0x04
#define  SD_HOST_4BIT		 0x02
#define  SD_CD_TEST_INS      0x40
#define  SD_CD_TEST          0x80

#define SDCLKCTL			0x2C
#define  SD_DIV_SHIFT        8
#define  SD_CLK_SD_EN        0x0004
#define  SD_CLK_INT_STABLE   0x0002
#define  SD_CLK_INT_EN       0x0001

#define SD_REFF_CLK_50M		50000000
#define SD_HOST_HS			0x4

#define SDINTSTS			0x30
#define SDINTSEN			0x34
#define SDINTEN				0x38
#define  SD_INT_CMD_CMPL     0x00000001
#define  SD_INT_TRNS_CMPL    0x00000002
#define  SD_INT_DMA          0x00000008
#define  SD_INT_CINS         0x00000040
#define  SD_INT_ERR_CREM     0x00000080
#define  SD_INT_ERR_WP       0x00000200
#define  SD_INT_ERR_CTIMEOUT 0x00010000
#define  SD_INT_ERR_CCRC     0x00020000
#define  SD_INT_ERR_CEB      0x00040000
#define  SD_INT_ERR_IDX      0x00080000
#define  SD_INT_ERR_DTIMEOUT 0x00100000
#define  SD_INT_ERR_DCRC     0x00200000
#define  SD_INT_ERR_DEB      0x00400000
#define  SD_INT_ERR_CLMT     0x00800000
#define  SD_INT_ERR_ACMD12   0x01000000
#define  SD_INT_ERR_XDMA     0x04000000
#define  SD_INT_ERR_TRESP    0x08000000
#define  SD_INT_ERROR        (SD_INT_ERR_WP | SD_INT_ERR_CTIMEOUT | SD_INT_ERR_CCRC | \
								SD_INT_ERR_CEB | SD_INT_ERR_IDX | SD_INT_ERR_DTIMEOUT | \
								SD_INT_ERR_DCRC | SD_INT_ERR_DEB | SD_INT_ERR_CLMT | \
								SD_INT_ERR_XDMA | SD_INT_ERR_TRESP)

#define AUTOCMD12			0x3C
#define FSBLK_CTRL_REG      0x84
#define RSPTMR_REG          0x104
#define TXDTTMR_REG         0x108
#define RXDTTMR_REG         0x10C
#define LTXSTS_REG          0x118
#define LMEMADDR_REG        0x130
#define LMEMCNT_REG         0x134
#define LMEMCFG_REG         0x138
#define LMEMCTL_REG         0x13C
#define LMEMSTATUS_REG      0x140

/* Clock Divisor values */
#define PILOT_SDHC_CLK_400KHz   0x0FA
#define PILOT_SDHC_CLK_20MHz    0x006
#define PILOT_SDHC_CLK_25MHz    0x004
#define PILOT_SDHC_CLK_50MHz    0x002

/* SDCLKCTL bit definations */
#define SDCLK_SELECT_POS                   0
#define SDCLK_SELECT_BITS                  BITS2
#define SDCLK_SELECT                       (SDCLK_SELECT_BITS<<SDCLK_SELECT_POS)
#define SDCLK_CLK_ENABLE_POS               2
#define SDCLK_CLK_ENABLE_BITS              BITS1
#define SDCLK_CLK_ENABLE                   (SDCLK_CLK_ENABLE_BITS<<SDCLK_CLK_ENABLE_POS)
#define SDCLK_POLARITY_POS                 3
#define SDCLK_POLARITY_BITS                BITS1
#define SDCLK_POLARITY                     (SDCLK_POLARITY_BITS<<SDCLK_POLARITY_POS)
#define FSCLK_DIVISOR_POS                  4
#define FSCLK_DIVISOR_BITS                 BITS10
#define FSCLK_DIVISOR                      (FSCLK_DIVISOR_BITS<<FSCLK_DIVISOR_POS)

/* LTXSTS_REG bit definitions */
#define LMEM_PG_VALID_EMPTY_POS            12
#define LMEM_PG_VALID_EMPTY_BITS           BITS1
#define LMEM_PG_VALID_EMPTY                (LMEM_PG_VALID_EMPTY_BITS << LMEM_PG_VALID_EMPTY_POS)

/* LMEMCNT_REG bit definitions */
#define LMEM_ALLOCATED_PG_CNT_POS          0
#define LMEM_ALLOCATED_PG_CNT_BITS         BITS16
#define LMEM_ALLOCATED_PG_CNT              (LMEM_ALLOCATED_PG_CNT_BITS << LMEM_ALLOCATED_PG_CNT_POS)
#define LMEM_VALID_DATA_PAGES_CNT_POS      16
#define LMEM_VALID_DATA_PAGES_CNT_BITS     BITS16
#define LMEM_VALID_DATA_PAGES_CNT          (LMEM_VALID_DATA_PAGES_CNT_BITS << LMEM_VALID_DATA_PAGES_CNT_POS)

/* SDINTSTS_REG, SDINTSEN_REG, SDINTEN_REG bit definitions */
#define PIO_BUF_WRITE_READY_INT_POS        4
#define PIO_BUF_WRITE_READY_INT_BITS       BITS1
#define PIO_BUF_WRITE_READY_INT            (PIO_BUF_WRITE_READY_INT_BITS << PIO_BUF_WRITE_READY_INT_POS)

#define PIO_BUF_READ_READY_INT_POS         5
#define PIO_BUF_READ_READY_INT_BITS        BITS1
#define PIO_BUF_READ_READY_INT             (PIO_BUF_READ_READY_INT_BITS << PIO_BUF_READ_READY_INT_POS)

/* LMEMSTATUS_REG bit definitions */
#define LMEM_COMPLETION_PTR_POS            0
#define LMEM_COMPLETION_PTR_BITS           BITS10
#define LMEM_COMPLETION_PTR                (LMEM_COMPLETION_PTR_BITS << LMEM_COMPLETION_PTR_POS)
#define LMEM_ALL_PAGES_ALLOCATED_POS       17
#define LMEM_ALL_PAGES_ALLOCATED_BITS      BITS1
#define LMEM_ALL_PAGES_ALLOCATED           (LMEM_ALL_PAGES_ALLOCATED_BITS << LMEM_ALL_PAGES_ALLOCATED_POS)

/* LMEMADDR_REG bit definitions */
#define LMEM_FREE_PTR_POS                  0
#define LMEM_FREE_PTR_BITS                 BITS10
#define LMEM_FREE_PTR                      (LMEM_FREE_PTR_BITS << LMEM_FREE_PTR_POS)

/* CMDCTL_REG bit definitions */
#define CMDINDEX_POS                       24
#define CMDINDEX_BITS                      BITS6
#define CMDINDEX                           (CMDINDEX_BITS << CMDINDEX_POS)

#define BITS1                       0x1
#define BITS2                       0x3
#define BITS3                       0x7
#define BITS4                       0xf
#define BITS5                       0x1f
#define BITS6                       0x3f
#define BITS7                       0x7f
#define BITS8                       0xff
#define BITS9                       0x1ff
#define BITS10                      0x3ff
#define BITS11                      0x7ff
#define BITS12                      0xfff
#define BITS13                      0x1fff
#define BITS14                      0x3fff
#define BITS15                      0x7fff
#define BITS16                      0xffff
#define BITS17                      0x1ffff
#define BITS18                      0x3ffff
#define BITS19                      0x7ffff
#define BITS20                      0xfffff
#define BITS21                      0x1fffff
#define BITS22                      0x3fffff
#define BITS23                      0x7fffff
#define BITS24                      0xffffff
#define BITS25                      0x1ffffff
#define BITS26                      0x3ffffff
#define BITS27                      0x7ffffff
#define BITS28                      0xfffffff
#define BITS29                      0x1fffffff
#define BITS30                      0x3fffffff
#define BITS31                      0x7fffffff
#define BITS32                      0xffffffff

#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */

struct dma_desc
{
	volatile unsigned int_enb: 1;       /* Enable Interrupt */
	volatile unsigned chain: 1;         /* 0-last entry, 1-NOT last entry */
	volatile unsigned dir: 1;           /* 0-SDHC-DDR, 1-DDR-SDHC */
	volatile unsigned crc_en: 1;        /* Enable CRC */
	volatile unsigned rsvd1: 4;
	volatile unsigned len: 24;          /* Max 16MB */
	volatile unsigned long crc_addr;    /* CRC address */
	volatile unsigned long data_addr;   /* src/dest Address */
	volatile unsigned long rsvd2;
};

/* function prototype declaration */
int set_resp_type(tU32 resp_type);
tU8 checkRxDtRdy (struct mmc *);
void clearRxDtRdy (struct mmc *);
void setTxDtRdy (struct mmc *);
tU8 checkCMDDatadone (struct mmc *);
tU16 checkLMEMPGAvailable(struct mmc *);
void clearBufWrReadyInt(struct mmc *);
void clearBufRdReadyInt (struct mmc *);
tU16 getLMEMPGValidCnt(struct mmc *);
tU16 getLMEMPGCompletionPtr (struct mmc *);
void setCLKCTL(struct mmc *, u16 , u8, u8, u8);
int do_pio_read(struct mmc *, u32, u32, u32, u32);
int do_pio_write(struct mmc *, u32, u32, u32, u32);
void StorePIODataTask (struct mmc *, tU16, tU32, tU16);
void LoadPIODataTask (struct mmc *, tU16, tU32, tU16);

#endif /* __SDHCI_MMC_H_ */

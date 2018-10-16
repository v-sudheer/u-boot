/*******************************************************************************
 *
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


// SDHC register defines

#define SDMAADDR_REG           0x000
#define BLKCTL_REG             0x004
#define CMDARG_REG             0x008
#define CMDCTL_REG             0x00C

#define SDRESP0_REG            0x010
#define SDRESP1_REG            0x014
#define SDRESP2_REG            0x018
#define SDRESP3_REG            0x01C

#define SDHCRSVD0_REG          0x020
#define PRSNTST_REG            0x024
#define SDHCCTL_REG            0x028
#define SDCLKCTL_REG           0x02C

#define SDINTSTS_REG           0x030
#define SDINTSEN_REG           0x034
#define SDINTEN_REG            0x038
#define AUTOCMD12_REG          0x03C

#define HBLK_CTRL_REG          0x080
#define FSBLK_CTRL_REG         0x084

#define CMDTMR_REG             0x100
#define RSPTMR_REG             0x104
#define TXDTTMR_REG            0x108
#define RXDTTMR_REG            0x10C

#define CMDSTS_REG             0x110
#define RSPSTS_REG             0x114
#define LTXSTS_REG             0x118
#define LRXSTS_REG             0x11C

#define TXDTSTS_REG            0x120
#define RXDTSTS_REG            0x124
#define XFRSTS_REG             0x128
#define XDMASTS_REG            0x12C

#define LMEMADDR_REG           0x130
#define LMEMCNT_REG            0x134
#define LMEMCFG_REG            0x138
#define LMEMCTL_REG            0x13C

#define LMEMSTATUS_REG         0x140
#define RXTX_FFSTS_REG         0x144

#define CMDRSP_CRC_REG         0x150
#define RXTX_CRC0_REG          0x154
#define RXTX_CRC1_REG          0x158

#define AUTO_CMDARG_REG        0x160
#define DBGCTRL_REG            0x164
#define DBGSTS_REG             0x168

  
// SDHC Tasks defines
#define SDCLK_FOD              0x0
#define SDCLK_FPP_25MHZ        0x1
#define SDCLK_FPP_50MHZ        0x2
#define SDCLK_RSVD             0x3

//SDCLK SET RESET
#define SDCLK_ENABLE           0x1
#define SDCLK_DISABLE          0x0

//SDCLK POLARITY
#define SDCLK_POL0             0x0
#define SDCLK_POL1             0x1

//FSCLK DIVISOR
#define FSCLK_100KHZ           0x3E8
#define FSCLK_400KHZ           0x0FA
#define FSCLK_25MHZ            0x004
#define FSCLK_50MHZ            0x002

//Response Type
#define RESP_NONE              0x0
#define RESP_136BIT            0x1
#define RESP_48BIT             0x2
#define RESP_48BIT_BUSY        0x3

//CRC CHECK ENABLE
#define CRC_CHK_ENABLE         0x1
#define CRC_CHK_DISABLE        0x0

//CMD INDEX CHECK ENABLE
#define CMDINDX_CHK_ENABLE     0x1
#define CMDINDX_CHK_DISABLE    0x0

//DATA PRESENT
#define DATA_PRESENT           0x1
#define DATA_NOT_PRESENT       0x0

//CMD TYPE
#define CMD_NORMAL             0x0
#define CMD_SUSPEND            0x1
#define CMD_RESUME             0x2
#define CMD_ABORT              0x3

//MODE
#define DMA                    0x1
#define PIO                    0x0

//BLKCNT ENABLE
#define BLKCNT_ENABLE          0x1
#define BLKCNT_DISABLE         0x0

//AUTOCMD12 ENABLE
#define AUTOCMD12_ENABLE       0x1
#define AUTOCMD12_DISABLE      0x0

//DIR
#define READ                   0x1
#define WRITE                  0x0

//DIR
#define SDHC_READ              0x1
#define SDHC_WRITE             0x0

//BUSWIDTH
#define WIDTH_1                0x0
#define WIDTH_4                0x2

//BLK TYPE
#define MULTI_BLOCK            0x1
#define SINGLE_BLOCK           0x0

//BUSY SET RESET
#define SD_BUSY                   0x1
#define SD_NO_BUSY                0x0

//CMD INDEX 
//Class0
#define CMD000                 0
#define CMD001                 1
#define CMD002                 2
#define CMD003                 3
#define CMD004                 4
#define CMD006                 6
#define CMD007                 7
#define CMD008                 8
#define CMD009                 9
#define CMD010                 10
#define CMD012                 12
#define CMD013                 13
#define CMD015                 15
//Class2
#define CMD016                 16
#define CMD017                 17
#define CMD018                 18
//Class4
#define CMD024                 24
#define CMD025                 25
#define CMD027                 27
//Class6
#define CMD028                 28
#define CMD029                 29
#define CMD030                 30
//Class5
#define CMD032                 32
#define CMD033                 33
#define CMD038                 38
//Class7
#define CMD042                 42
//Class8
#define CMD055                 55
#define CMD056                 56
#define ACMD006                6
#define ACMD013                13
#define ACMD022                22
#define ACMD023                23
#define ACMD041                41
#define ACMD042                42
#define ACMD051                51

#define CMDRSVD                63

//Common defines
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


//BLKCTL_REG bit definitions
#define BLKLEN_POS                         0
#define BLKLEN_BITS                        BITS12
#define BLKLEN                             (BLKLEN_BITS                 <<BLKLEN_POS)
#define BLKCNT_POS                         12
#define BLKCNT_BITS                        BITS9
#define BLKCNT                             (BLKCNT_BITS                 <<BLKCNT_POS)

//CMDCTL_REG bit definitions
#define MODE_POS                           0
#define MODE_BITS                          BITS1
#define MODE                               (MODE_BITS                   <<MODE_POS)
#define AUTOCMD12EN_POS                    2
#define AUTOCMD12EN_BITS                   BITS1
#define AUTOCMD12EN                        (AUTOCMD12EN_BITS            <<AUTOCMD12EN_POS)
#define DIRECTION_POS                      4
#define DIRECTION_BITS                     BITS1
#define DIRECTION                          (DIRECTION_BITS              <<DIRECTION_POS)
#define MULTIBLOCK_POS                     5
#define MULTIBLOCK_BITS                    BITS1
#define MULTIBLOCK                         (MULTIBLOCK_BITS             <<MULTIBLOCK_POS)
#define RESPTYPE_POS                       16
#define RESPTYPE_BITS                      BITS2
#define RESPTYPE                           (RESPTYPE_BITS               <<RESPTYPE_POS)
#define CRCCHECKENABLE_POS                 19
#define CRCCHECKENABLE_BITS                BITS1
#define CRCCHECKENABLE                     (CRCCHECKENABLE_BITS               <<CRCCHECKENABLE_POS)
#define CMDINDEXCHECK_POS                  20
#define CMDINDEXCHECK_BITS                 BITS1
#define CMDINDEXCHECK                      (CMDINDEXCHECK_BITS         <<CMDINDEXCHECK_POS)
#define DATAPRESENT_POS                    21
#define DATAPRESENT_BITS                   BITS1
#define DATAPRESENT                        (DATAPRESENT_BITS            <<DATAPRESENT_POS)
#define CMDTYPE_POS                        22
#define CMDTYPE_BITS                       BITS2
#define CMDTYPE                            (CMDTYPE_BITS            <<CMDTYPE_POS)
#define CMDINDEX_POS                       24
#define CMDINDEX_BITS                      BITS6
#define CMDINDEX                           (CMDINDEX_BITS               <<CMDINDEX_POS)
#define PIO_DT_READY_POS                   30
#define PIO_DT_READY_BITS                  BITS1
#define PIO_DT_READY                       (PIO_DT_READY_BITS                 <<PIO_DT_READY_POS)
#define CMDSET_POS                         31
#define CMDSET_BITS                        BITS1
#define CMDSET                             (CMDSET_BITS                 <<CMDSET_POS)

//PRSNTST_REG bit definitions
#define CMD_INHIBIT_CMD_POS                0
#define CMD_INHIBIT_CMD_BITS               BITS1
#define CMD_INHIBIT_CMD                    (CMD_INHIBIT_CMD_BITS << CMD_INHIBIT_CMD_POS)
#define CMD_INHIBIT_DAT_POS                1
#define CMD_INHIBIT_DAT_BITS               BITS1
#define CMD_INHIBIT_DAT                    (CMD_INHIBIT_DAT_BITS << CMD_INHIBIT_DAT_POS)
#define DAT_LINE_ACTIVE_POS                2
#define DAT_LINE_ACTIVE_BITS               BITS1
#define DAT_LINE_ACTIVE                    (DAT_LINE_ACTIVE_BITS << DAT_LINE_ACTIVE_POS)
#define WR_TRANSFER_ACTIVE_POS             8
#define WR_TRANSFER_ACTIVE_BITS            BITS1
#define WR_TRANSFER_ACTIVE                 (WR_TRANSFER_ACTIVE_BITS << WR_TRANSFER_ACTIVE_POS)
#define RD_TRANSFER_ACTIVE_POS             9
#define RD_TRANSFER_ACTIVE_BITS            BITS1
#define RD_TRANSFER_ACTIVE                 (RD_TRANSFER_ACTIVE_BITS << RD_TRANSFER_ACTIVE_POS)
#define BUF_WR_ENABLE_POS                  10
#define BUF_WR_ENABLE_BITS                 BITS1
#define BUF_WR_ENABLE                      (BUF_WR_ENABLE_BITS << BUF_WR_ENABLE_POS)
#define BUF_RD_ENABLE_POS                  11
#define BUF_RD_ENABLE_BITS                 BITS1
#define BUF_RD_ENABLE                      (BUF_RD_ENABLE_BITS << BUF_RD_ENABLE_POS)
#define CARD_INSERTED_POS                  16
#define CARD_INSERTED_BITS                 BITS1
#define CARD_INSERTED                      (CARD_INSERTED_BITS << CARD_INSERTED_POS)
#define CARD_STATE_STABLE_POS              17
#define CARD_STATE_STABLE_BITS             BITS1
#define CARD_STATE_STABLE                  (CARD_STATE_STABLE_BITS << CARD_STATE_STABLE_POS)
#define CARD_DETECT_PIN_STATUS_POS         18
#define CARD_DETECT_PIN_STATUS_BITS        BITS1
#define CARD_DETECT_PIN_STATUS             (CARD_DETECT_PIN_STATUS_BITS << CARD_DETECT_PIN_STATUS_POS)
#define WRITE_PROTECT_PIN_STATUS_POS       19
#define WRITE_PROTECT_PIN_STATUS_BITS      BITS1
#define WRITE_PROTECT_PIN_STATUS           (WRITE_PROTECT_PIN_STATUS_BITS << WRITE_PROTECT_PIN_STATUS_POS)
#define DAT_3_0_LEVEL_POS                  20
#define DAT_3_0_LEVEL_BITS                 BITS4
#define DAT_3_0_LEVEL                      (DAT_3_0_LEVEL_BITS << DAT_3_0_LEVEL_POS)
#define CMD_LINE_SIGNAL_LEVEL_POS          24
#define CMD_LINE_SIGNAL_LEVEL_BITS         BITS1
#define CMD_LINE_SIGNAL_LEVEL              (CMD_LINE_SIGNAL_LEVEL_BITS << CMD_LINE_SIGNAL_LEVEL_POS)

//SDHCCTL_REG bit definitions
#define DATA_TRAN_WIDTH_POS                1
#define DATA_TRAN_WIDTH_BITS               BITS1
#define DATA_TRAN_WIDTH                    (DATA_TRAN_WIDTH_BITS << DATA_TRAN_WIDTH_POS)
#define SDIO_INTERRUPT_MODE_POS            5
#define SDIO_INTERRUPT_MODE_BITS           BITS1
#define SDIO_INTERRUPT_MODE                (SDIO_INTERRUPT_MODE_BITS << SDIO_INTERRUPT_MODE_POS)
#define CARD_DET_CTL_POS                   6
#define CARD_DET_CTL_BITS                  BITS2
#define CARD_DET_CTL                       (CARD_DET_CTL_BITS << CARD_DET_CTL_POS)

//SDCLKCTL bit definations
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
#define TX_CLK_DELAY_POS                   20
#define TX_CLK_DELAY_BITS                  BITS1
#define TX_CLK_DELAY                       (TX_CLK_DELAY_BITS << TX_CLK_DELAY_POS)
#define RX_CLK_DELAY_POS                   21
#define RX_CLK_DELAY_BITS                  BITS1
#define RX_CLK_DELAY                       (RX_CLK_DELAY_BITS << RX_CLK_DELAY_POS)
#define DEB_SEL_POS                        22
#define DEB_SEL_BITS                       BITS2
#define DEB_SEL                            (DEB_SEL_BITS << DEB_SEL_POS)
#define SOFTRESET_CMD_DAT_LINES_POS        24
#define SOFTRESET_CMD_DAT_LINES_BITS       BITS1
#define SOFTRESET_CMD_DAT_LINES            (SOFTRESET_CMD_DAT_LINES_BITS << SOFTRESET_CMD_DAT_LINES_POS)
#define SOFTRESET_CMD_LINES_POS            25
#define SOFTRESET_CMD_LINES_BITS           BITS1
#define SOFTRESET_CMD_LINES                (SOFTRESET_CMD_LINES_BITS << SOFTRESET_CMD_LINES_POS)
#define SOFTRESET_DAT_LINES_POS            26
#define SOFTRESET_DAT_LINES_BITS           BITS1
#define SOFTRESET_DAT_LINES                (SOFTRESET_DAT_LINES_BITS << SOFTRESET_DAT_LINES_POS)

//SDINTSTS_REG, SDINTSEN_REG, SDINTEN_REG bit definitions
#define CMD_COMPLETE_INT_POS               0
#define CMD_COMPLETE_INT_BITS              BITS1
#define CMD_COMPLETE_INT                   (CMD_COMPLETE_INT_BITS << CMD_COMPLETE_INT_POS)
#define XFR_COMPLETE_INT_POS               1
#define XFR_COMPLETE_INT_BITS              BITS1
#define XFR_COMPLETE_INT                   (XFR_COMPLETE_INT_BITS << XFR_COMPLETE_INT_POS)
#define PIO_BUF_WRITE_READY_INT_POS        4
#define PIO_BUF_WRITE_READY_INT_BITS       BITS1
#define PIO_BUF_WRITE_READY_INT            (PIO_BUF_WRITE_READY_INT_BITS << PIO_BUF_WRITE_READY_INT_POS)
#define PIO_BUF_READ_READY_INT_POS         5
#define PIO_BUF_READ_READY_INT_BITS        BITS1
#define PIO_BUF_READ_READY_INT             (PIO_BUF_READ_READY_INT_BITS << PIO_BUF_READ_READY_INT_POS)
#define CARD_INSERTION_INT_POS             6
#define CARD_INSERTION_INT_BITS            BITS1
#define CARD_INSERTION_INT                 (CARD_INSERTION_INT_BITS << CARD_INSERTION_INT_POS)
#define CARD_REMOVAL_INT_POS               7
#define CARD_REMOVAL_INT_BITS              BITS1
#define CARD_REMOVAL_INT                   (CARD_REMOVAL_INT_BITS << CARD_REMOVAL_INT_POS)
#define CARD_INTERRUPT_INT_POS             8
#define CARD_INTERRUPT_INT_BITS            BITS1
#define CARD_INTERRUPT_INT                 (CARD_INTERRUPT_INT_BITS << CARD_INTERRUPT_INT_POS)
#define CMD_TIMEOUT_ERR_INT_POS            16
#define CMD_TIMEOUT_ERR_INT_BITS           BITS1
#define CMD_TIMEOUT_ERR_INT                (CMD_TIMEOUT_ERR_INT_BITS << CMD_TIMEOUT_ERR_INT_POS)
#define CMD_CRC_ERR_INT_POS                17
#define CMD_CRC_ERR_INT_BITS               BITS1
#define CMD_CRC_ERR_INT                    (CMD_CRC_ERR_INT_BITS << CMD_CRC_ERR_INT_POS)
#define CMD_ENDBIT_ERR_INT_POS             18
#define CMD_ENDBIT_ERR_INT_BITS            BITS1
#define CMD_ENDBIT_ERR_INT                 (CMD_ENDBIT_ERR_INT_BITS << CMD_ENDBIT_ERR_INT_POS)
#define CMD_INDEX_ERR_INT_POS              19
#define CMD_INDEX_ERR_INT_BITS             BITS1
#define CMD_INDEX_ERR_INT                  (CMD_INDEX_ERR_INT_BITS << CMD_INDEX_ERR_INT_POS)
#define DATA_TIMEOUT_ERR_INT_POS           20
#define DATA_TIMEOUT_ERR_INT_BITS          BITS1
#define DATA_TIMEOUT_ERR_INT               (DATA_TIMEOUT_ERR_INT_BITS << DATA_TIMEOUT_ERR_INT_POS)
#define DATA_CRC_ERR_INT_POS               21
#define DATA_CRC_ERR_INT_BITS              BITS1
#define DATA_CRC_ERR_INT                   (DATA_CRC_ERR_INT_BITS << DATA_CRC_ERR_INT_POS)
#define DATA_ENDBIT_ERR_INT_POS            22
#define DATA_ENDBIT_ERR_INT_BITS           BITS1
#define DATA_ENDBIT_ERR_INT                (DATA_ENDBIT_ERR_INT_BITS << DATA_ENDBIT_ERR_INT_POS)
#define LIMIT_ERR_INT_POS                  23
#define LIMIT_ERR_INT_BITS                 BITS1
#define LIMIT_ERR_INT                      (LIMIT_ERR_INT_BITS << LIMIT_ERR_INT_POS)
#define AUTOCMD12_ERR_INT_POS              24
#define AUTOCMD12_ERR_INT_BITS             BITS1
#define AUTOCMD12_ERR_INT                  (AUTOCMD12_ERR_INT_BITS << AUTOCMD12_ERR_INT_POS)

//AUTOCMD12_REG bit definitions
#define AUTOCMD12_NOT_EXECUTED_POS         0
#define AUTOCMD12_NOT_EXECUTED_BITS        BITS1
#define AUTOCMD12_NOT_EXECUTED             (AUTOCMD12_NOT_EXECUTED_BITS << AUTOCMD12_NOT_EXECUTED_POS)
#define AUTOCMD12_TIMEOUT_ERR_POS          1
#define AUTOCMD12_TIMEOUT_ERR_BITS         BITS1
#define AUTOCMD12_TIMEOUT_ERR              (AUTOCMD12_TIMEOUT_ERR_BITS << AUTOCMD12_TIMEOUT_ERR_POS)
#define AUTOCMD12_CRC_ERR_POS              2
#define AUTOCMD12_CRC_ERR_BITS             BITS1
#define AUTOCMD12_CRC_ERR                  (AUTOCMD12_CRC_ERR_BITS << AUTOCMD12_CRC_ERR_POS)
#define AUTOCMD12_ENDBIT_ERR_POS           3
#define AUTOCMD12_ENDBIT_ERR_BITS          BITS1
#define AUTOCMD12_ENDBIT_ERR               (AUTOCMD12_ENDBIT_ERR_BITS << AUTOCMD12_ENDBIT_ERR_POS)
#define AUTOCMD12_INDEX_ERR_POS            4
#define AUTOCMD12_INDEX_ERR_BITS           BITS1
#define AUTOCMD12_INDEX_ERR                (AUTOCMD12_INDEX_ERR_BITS << AUTOCMD12_INDEX_ERR_POS)
#define AUTOCMD12_UNISSUED_CMD_ERR_POS     7
#define AUTOCMD12_UNISSUED_CMD_ERR_BITS    BITS1
#define AUTOCMD12_UNISSUED_CMD_ERR         (AUTOCMD12_UNISSUED_CMD_ERR_BITS << AUTOCMD12_UNISSUED_CMD_ERR_POS)

//CMDTMR_REG bit definitions
#define NRC_MIN_TIMER_POS                  0
#define NRC_MIN_TIMER_BITS                 BITS4
#define NRC_MIN_TIMER                      (NRC_MIN_TIMER_BITS << NRC_MIN_TIMER_POS)
#define NCC_MIN_TIMER_POS                  4
#define NCC_MIN_TIMER_BITS                 BITS4
#define NCC_MIN_TIMER                      (NCC_MIN_TIMER_BITS << NCC_MIN_TIMER_POS)
#define NRC_TIMER_CLR_POS                  14
#define NRC_TIMER_CLR_BITS                 BITS1
#define NRC_TIMER_CLR                      (NRC_TIMER_CLR_BITS << NRC_TIMER_CLR_POS)
#define NCC_TIMER_CLR_POS                  14
#define NCC_TIMER_CLR_BITS                 BITS1
#define NCC_TIMER_CLR                      (NCC_TIMER_CLR_BITS << NCC_TIMER_CLR_POS)

//RSPTMR_REG bit definitions
#define NCR_MIN_TIMER_POS                  0
#define NCR_MIN_TIMER_BITS                 BITS8
#define NCR_MIN_TIMER                      (NCR_MIN_TIMER_BITS << NCR_MIN_TIMER_POS)
#define NCR_TIMER_CLR_POS                  8
#define NCR_TIMER_CLR_BITS                 BITS1
#define NCR_TIMER_CLR                      (NCR_TIMER_CLR_BITS << NCR_TIMER_CLR_POS)

//LMEMADDR_REG bit definitions
#define LMEM_FREE_PTR_POS                  0
#define LMEM_FREE_PTR_BITS                 BITS10
#define LMEM_FREE_PTR                      (LMEM_FREE_PTR_BITS << LMEM_FREE_PTR_POS)
#define LMEM_DATA_PTR_POS                  16
#define LMEM_DATA_PTR_BITS                 BITS10
#define LMEM_DATA_PTR                      (LMEM_DATA_PTR_BITS << LMEM_DATA_PTR_POS)

//LMEMCNT_REG bit definitions
#define LMEM_ALLOCATED_PG_CNT_POS          0
#define LMEM_ALLOCATED_PG_CNT_BITS         BITS16
#define LMEM_ALLOCATED_PG_CNT              (LMEM_ALLOCATED_PG_CNT_BITS << LMEM_ALLOCATED_PG_CNT_POS)
#define LMEM_VALID_DATA_PAGES_CNT_POS      16
#define LMEM_VALID_DATA_PAGES_CNT_BITS     BITS16
#define LMEM_VALID_DATA_PAGES_CNT          (LMEM_VALID_DATA_PAGES_CNT_BITS << LMEM_VALID_DATA_PAGES_CNT_POS)

//LMEMCFG_REG bit definitions
#define LMEM_PAGE_SIZE_DWORDS_POS          0
#define LMEM_PAGE_SIZE_DWORDS_BITS         BITS8
#define LMEM_PAGE_SIZE_DWORDS              (LMEM_PAGE_SIZE_DWORDS_BITS << LMEM_PAGE_SIZE_DWORDS_POS)
#define LMEM_MAX_NUM_PAGES_POS             8
#define LMEM_MAX_NUM_PAGES_BITS            BITS11
#define LMEM_MAX_NUM_PAGES                 (LMEM_MAX_NUM_PAGES_BITS << LMEM_MAX_NUM_PAGES_POS)
#define LMEM_LIMIT_ADDR_POS                19
#define LMEM_LIMIT_ADDR_BITS               BITS13
#define LMEM_LIMIT_ADDR                    (LMEM_LIMIT_ADDR_BITS << LMEM_LIMIT_ADDR_POS)

//LMEMCTL_REG bit definitions
#define LMEM_FREE_PAGE_LOW_WM_POS          16
#define LMEM_FREE_PAGE_LOW_WM_BITS         BITS8
#define LMEM_FREE_PAGE_LOW_WM              (LMEM_FREE_PAGE_LOW_WM_BITS << LMEM_FREE_PAGE_LOW_WM_POS)
#define LMEM_FREE_PAGE_HIGH_WM_POS         24
#define LMEM_FREE_PAGE_HIGH_WM_BITS        BITS8
#define LMEM_FREE_PAGE_HIGH_WM             (LMEM_FREE_PAGE_HIGH_WM_BITS << LMEM_FREE_PAGE_HIGH_WM_POS)


//LMEMSTATUS_REG bit definitions
#define LMEM_COMPLETION_PTR_POS            0
#define LMEM_COMPLETION_PTR_BITS           BITS10
#define LMEM_COMPLETION_PTR                (LMEM_COMPLETION_PTR_BITS << LMEM_COMPLETION_PTR_POS)
#define LMEM_ALL_PAGES_EMPTY_POS           16
#define LMEM_ALL_PAGES_EMPTY_BITS          BITS1
#define LMEM_ALL_PAGES_EMPTY               (LMEM_ALL_PAGES_EMPTY_BITS << LMEM_ALL_PAGES_EMPTY_POS)
#define LMEM_ALL_PAGES_ALLOCATED_POS       17
#define LMEM_ALL_PAGES_ALLOCATED_BITS      BITS1
#define LMEM_ALL_PAGES_ALLOCATED           (LMEM_ALL_PAGES_ALLOCATED_BITS << LMEM_ALL_PAGES_ALLOCATED_POS)
#define LMEM_ALLOC_WRITE_ERROR_POS         18
#define LMEM_ALLOC_WRITE_ERROR_BITS        BITS1
#define LMEM_ALLOC_WRITE_ERROR             (LMEM_ALLOC_WRITE_ERROR_BITS << LMEM_ALLOC_WRITE_ERROR_POS)
#define LMEM_ALLOC_READ_ERROR_POS          19
#define LMEM_ALLOC_READ_ERROR_BITS         BITS1
#define LMEM_ALLOC_READ_ERROR              (LMEM_ALLOC_READ_ERROR_BITS << LMEM_ALLOC_READ_ERROR_POS)
#define LMEM_PAGES_NO_VALID_DATA_POS       20
#define LMEM_PAGES_NO_VALID_DATA_BITS      BITS1
#define LMEM_PAGES_NO_VALID_DATA           (LMEM_PAGES_NO_VALID_DATA_BITS << LMEM_PAGES_NO_VALID_DATA_POS)
#define LMEM_PAGES_ALL_VALID_DATA_POS      21
#define LMEM_PAGES_ALL_VALID_DATA_BITS     BITS1
#define LMEM_PAGES_ALL_VALID_DATA          (LMEM_PAGES_ALL_VALID_DATA_BITS << LMEM_PAGES_ALL_VALID_DATA_POS)
#define LMEM_VALID_WRITE_ERROR_POS         22
#define LMEM_VALID_WRITE_ERROR_BITS        BITS1
#define LMEM_VALID_WRITE_ERROR             (LMEM_VALID_WRITE_ERROR_BITS << LMEM_VALID_WRITE_ERROR_POS)
#define LMEM_VALID_READ_ERROR_POS          23
#define LMEM_VALID_READ_ERROR_BITS         BITS1
#define LMEM_VALID_READ_ERROR              (LMEM_VALID_READ_ERROR_BITS << LMEM_VALID_READ_ERROR_POS)



//LMEM_BASE and LMEM_LIMIT ADDRESSES from fgb, bit definitions
#define LMEM_BASE_ADDR_REG                 0x40900028
#define LMEM_BASE_ADDR_POS                 16
#define LMEM_BASE_ADDR_BITS                BITS16
#define LMEM_BASE_ADDR                     (LMEM_BASE_ADDR_BITS << LMEM_BASE_ADDR_POS)
#define LMEM_LIMIT_ADDR_REG                0x40900038
#define LMEM_LIMIT_POS                     16
#define LMEM_LIMIT_BITS                    BITS16
#define LMEM_LIMIT                         (LMEM_LIMIT_BITS << LMEM_LIMIT_POS)

//LMEM_BASE and LMEM_LIMIT ADDRESSES from fgb, bit definitions
#define LMEM1_BASE_ADDR_REG                 0x4090002c
#define LMEM1_BASE_ADDR_POS                 16
#define LMEM1_BASE_ADDR_BITS                BITS16
#define LMEM1_BASE_ADDR                     (LMEM1_BASE_ADDR_BITS << LMEM1_BASE_ADDR_POS)
#define LMEM1_LIMIT_ADDR_REG                0x4090003c
#define LMEM1_LIMIT_POS                     16
#define LMEM1_LIMIT_BITS                    BITS16
#define LMEM1_LIMIT                         (LMEM1_LIMIT_BITS << LMEM1_LIMIT_POS)


//LTXSTS_REG bit definitions
#define LTX_BLOCK_CNT_POS                  0
#define LTX_BLOCK_CNT_BITS                 BITS10
#define LTX_BLOCK_CNT                      (LTX_BLOCK_CNT_BITS << LTX_BLOCK_CNT_POS)
#define LMEM_PG_ALLOC_EMPTY_POS            10
#define LMEM_PG_ALLOC_EMPTY_BITS           BITS1
#define LMEM_PG_ALLOC_EMPTY                (LMEM_PG_ALLOC_EMPTY_BITS << LMEM_PG_ALLOC_EMPTY_POS)
#define LMEM_PG_ALLOC_FULL_POS             11
#define LMEM_PG_ALLOC_FULL_BITS            BITS1
#define LMEM_PG_ALLOC_FULL                 (LMEM_PG_ALLOC_FULL_BITS << LMEM_PG_ALLOC_FULL_POS)
#define LMEM_PG_VALID_EMPTY_POS            12
#define LMEM_PG_VALID_EMPTY_BITS           BITS1
#define LMEM_PG_VALID_EMPTY                (LMEM_PG_VALID_EMPTY_BITS << LMEM_PG_VALID_EMPTY_POS)
#define LMEM_PG_VALID_FULL_POS             13
#define LMEM_PG_VALID_FULL_BITS            BITS1
#define LMEM_PG_VALID_FULL                 (LMEM_PG_VALID_FULL_BITS << LMEM_PG_VALID_FULL_POS)
#define TXDT_FIFO_WR_ERROR_POS             14
#define TXDT_FIFO_WR_ERROR_BITS            BITS1
#define TXDT_FIFO_WR_ERROR                 (TXDT_FIFO_WR_ERROR_BITS << TXDT_FIFO_WR_ERROR_POS)
#define TXDT_FIFO_FULL_POS                 15
#define TXDT_FIFO_FULL_BITS                BITS1
#define TXDT_FIFO_FULL                     (TXDT_FIFO_FULL_BITS << TXDT_FIFO_FULL_POS)
#define TXDT_FULUSH_ACTIVE_FSLCK_POS       16
#define TXDT_FULUSH_ACTIVE_FSLCK_BITS      BITS1
#define TXDT_FULUSH_ACTIVE_FSLCK           (TXDT_FULUSH_ACTIVE_FSLCK_BITS << TXDT_FULUSH_ACTIVE_FSLCK_POS)
#define LTX_XDMA_DSCR_SEL_POS              17
#define LTX_XDMA_DSCR_SEL_BITS             BITS1
#define LTX_XDMA_DSCR_SEL                  (LTX_XDMA_DSCR_SEL_BITS << LTX_XDMA_DSCR_SEL_POS)
#define LTX_LMEM_REQ_POS                   18
#define LTX_LMEM_REQ_BITS                  BITS1
#define LTX_LMEM_REQ                       (LTX_LMEM_REQ_BITS << LTX_LMEM_REQ_POS)
#define LMEM_LTX_RD_OK_POS                 19
#define LMEM_LTX_RD_OK_BITS                BITS1
#define LMEM_LTX_RD_OK                     (LMEM_LTX_RD_OK_BITS << LMEM_LTX_RD_OK_POS)
#define TXDT_FIFO_WENTRIES_POS             20
#define TXDT_FIFO_WENTRIES_BITS            BITS4
#define TXDT_FIFO_WENTRIES                 (TXDT_FIFO_WENTRIES_BITS << TXDT_FIFO_WENTRIES_POS)
#define XDRQ_CTRL_PS_POS                   24
#define XDRQ_CTRL_PS_BITS                  BITS4
#define XDRQ_CTRL_PS                       (XDRQ_CTRL_PS_BITS << XDRQ_CTRL_PS_POS)
#define LTX_CTRL_PS_POS                    28
#define LTX_CTRL_PS_BITS                   BITS4
#define LTX_CTRL_PS                        (LTX_CTRL_PS_BITS << LTX_CTRL_PS_POS)

//LRXSTS_REG bit definitions
#define LRX_BLOCK_CNT_POS                  0
#define LRX_BLOCK_CNT_BITS                 BITS10
#define LRX_BLOCK_CNT                      (LRX_BLOCK_CNT_BITS << LRX_BLOCK_CNT_POS)
#define RXDT_FIFO_EMPTY_POS                10
#define RXDT_FIFO_EMPTY_BITS               BITS1
#define RXDT_FIFO_EMPTY                    (RXDT_FIFO_EMPTY_BITS << RXDT_FIFO_EMPTY_POS)
#define RXDT_FIFO_ERROR_POS                11
#define RXDT_FIFO_ERROR_BITS               BITS1
#define RXDT_FIFO_ERROR                    (RXDT_FIFO_ERROR_BITS << RXDT_FIFO_ERROR_POS)
#define LRX_BYTE_CNT_POS                   12
#define LRX_BYTE_CNT_BITS                  BITS10
#define LRX_BYTE_CNT                       (LRX_BYTE_CNT_BITS << LRX_BYTE_CNT_POS)
#define RXDT_XFR_ABORT_POS                 27
#define RXDT_XFR_ABORT_BITS                BITS1
#define RXDT_XFR_ABORT                     (RXDT_XFR_ABORT_BITS << RXDT_XFR_ABORT_POS)
#define LRX_CTRL_PS_POS                    28
#define LRX_CTRL_PS_BITS                   BITS4
#define LRX_CTRL_PS                        (LRX_CTRL_PS_BITS << LRX_CTRL_PS_POS)

//TXDTSTS_REG bit definitions
#define TX_CRC_STATUS_POS                  0
#define TX_CRC_STATUS_BITS                 BITS3
#define TX_CRC_STATUS                      (TX_CRC_STATUS_BITS << TX_CRC_STATUS_POS)
#define TXDT_FIFO_RD_ERROR_POS             10
#define TXDT_FIFO_RD_ERROR_BITS            BITS1
#define TXDT_FIFO_RD_ERROR                 (TXDT_FIFO_RD_ERROR_BITS << TXDT_FIFO_RD_ERROR_POS)
#define TXDT_FIFO_EMPTY_POS                11
#define TXDT_FIFO_EMPTY_BITS               BITS1
#define TXDT_FIFO_EMPTY                    (TXDT_FIFO_EMPTY_BITS << TXDT_FIFO_EMPTY_POS)
#define TX_BLOCK_CNT_POS                   12
#define TX_BLOCK_CNT_BITS                  BITS10
#define TX_BLOCK_CNT                       (TX_BLOCK_CNT_BITS << TX_BLOCK_CNT_POS)
#define Nwr_WINDOW_POS                     23
#define Nwr_WINDOW_BITS                    BITS1
#define Nwr_WINDOW                         (Nwr_WINDOW_BITS << Nwr_WINDOW_POS)
#define TX_DATA_CTRL_PS_POS                24
#define TX_DATA_CTRL_PS_BITS               BITS5
#define TX_DATA_CTRL_PS                    (TX_DATA_CTRL_PS_BITS << TX_DATA_CTRL_PS_POS)
#define TXDT_SDHC_CLK_DISABLE_POS          29
#define TXDT_SDHC_CLK_DISABLE_BITS         BITS1
#define TXDT_SDHC_CLK_DISABLE              (TXDT_SDHC_CLK_DISABLE_BITS << TXDT_SDHC_CLK_DISABLE_POS)
#define TXDT_XFR_IN_PROG_POS               30
#define TXDT_XFR_IN_PROG_BITS              BITS1
#define TXDT_XFR_IN_PROG                   (TXDT_XFR_IN_PROG_BITS << TXDT_XFR_IN_PROG_POS)
#define EOS_XFR_DONE_P2_POS                31
#define EOS_XFR_DONE_P2_BITS               BITS1
#define EOS_XFR_DONE_P2                    (EOS_XFR_DONE_P2_BITS << EOS_XFR_DONE_P2_POS)

//RXDTSTS_REG bit definitions
#define RX_BYTE_CNT_POS                    0
#define RX_BYTE_CNT_BITS                   BITS10
#define RX_BYTE_CNT                        (RX_BYTE_CNT_BITS << RX_BYTE_CNT_POS)
#define RXDT_FIFO_WR_ERROR_POS             10
#define RXDT_FIFO_WR_ERROR_BITS            BITS1
#define RXDT_FIFO_WR_ERROR                 (RXDT_FIFO_WR_ERROR_BITS << RXDT_FIFO_WR_ERROR_POS)
#define RXDT_FIFO_FULL_POS                 11
#define RXDT_FIFO_FULL_BITS                BITS1
#define RXDT_FIFO_FULL                     (RXDT_FIFO_FULL_BITS << RXDT_FIFO_FULL_POS)
#define RX_BLOCK_CNT_POS                   12
#define RX_BLOCK_CNT_BITS                  BITS10
#define RX_BLOCK_CNT                       (RX_BLOCK_CNT_BITS << RX_BLOCK_CNT_POS)
#define RXDT_XFR_IN_PROG_POS               22
#define RXDT_XFR_IN_PROG_BITS              BITS1
#define RXDT_XFR_IN_PROG                   (RXDT_XFR_IN_PROG_BITS << RXDT_XFR_IN_PROG_POS)
#define RXDT_SDHC_CLK_DISABLE_POS          23
#define RXDT_SDHC_CLK_DISABLE_BITS         BITS1
#define RXDT_SDHC_CLK_DISABLE              (RXDT_SDHC_CLK_DISABLE_BITS << RXDT_SDHC_CLK_DISABLE_POS)
#define Nac_WINDOW_POS                     24
#define Nac_WINDOW_BITS                    BITS1
#define Nac_WINDOW                         (Nac_WINDOW_BITS << Nac_WINDOW_POS)
#define SDIO_INTR_WINDOW_N_POS             25
#define SDIO_INTR_WINDOW_N_BITS            BITS1
#define SDIO_INTR_WINDOW_N                 (SDIO_INTR_WINDOW_N_BITS << SDIO_INTR_WINDOW_N_POS)
#define SDIO_INTR_LVL_POS                  26
#define SDIO_INTR_LVL_BITS                 BITS1
#define SDIO_INTR_LVL                      (SDIO_INTR_LVL_BITS << SDIO_INTR_LVL_POS)
#define RX_DATA_CTRL_PS_POS                28
#define RX_DATA_CTRL_PS_BITS               BITS4
#define RX_DATA_CTRL_PS                    (RX_DATA_CTRL_PS_BITS << RX_DATA_CTRL_PS_POS)

//RXTX_FFSTS_REG bit definitions
#define RXTX_FFSTS_TXDT_FIFO_EMPTY_POS     0
#define RXTX_FFSTS_TXDT_FIFO_EMPTY_BITS    BITS1
#define RXTX_FFSTS_TXDT_FIFO_EMPTY         (RXTX_FFSTS_TXDT_FIFO_EMPTY_BITS << RXTX_FFSTS_TXDT_FIFO_EMPTY_POS)
#define RXTX_FFSTS_TXDT_FIFO_FULL_POS      1
#define RXTX_FFSTS_TXDT_FIFO_FULL_BITS     BITS1
#define RXTX_FFSTS_TXDT_FIFO_FULL          (RXTX_FFSTS_TXDT_FIFO_FULL_BITS << RXTX_FFSTS_TXDT_FIFO_FULL_POS)
#define RXTX_FFSTS_TXDT_FIFO_WR_ERROR_POS  2
#define RXTX_FFSTS_TXDT_FIFO_WR_ERROR_BITS BITS1
#define RXTX_FFSTS_TXDT_FIFO_WR_ERROR      (RXTX_FFSTS_TXDT_FIFO_WR_ERROR_BITS << RXTX_FFSTS_TXDT_FIFO_WR_ERROR_POS)
#define RXTX_FFSTS_TXDT_FIFO_RD_ERROR_POS  3
#define RXTX_FFSTS_TXDT_FIFO_RD_ERROR_BITS BITS1
#define RXTX_FFSTS_TXDT_FIFO_RD_ERROR      (RXTX_FFSTS_TXDT_FIFO_RD_ERROR_BITS << RXTX_FFSTS_TXDT_FIFO_RD_ERROR_POS)
#define RXTX_FFSTS_TXDT_FIFO_WENTRIES_POS  4
#define RXTX_FFSTS_TXDT_FIFO_WENTRIES_BITS BITS3
#define RXTX_FFSTS_TXDT_FIFO_WENTRIES      (RXTX_FFSTS_TXDT_FIFO_WENTRIES_BITS << RXTX_FFSTS_TXDT_FIFO_WENTRIES_POS)
#define RXTX_FFSTS_TXDT_FIFO_RENTRIES_POS  8
#define RXTX_FFSTS_TXDT_FIFO_RENTRIES_BITS BITS3
#define RXTX_FFSTS_TXDT_FIFO_RENTRIES      (RXTX_FFSTS_TXDT_FIFO_RENTRIES_BITS << RXTX_FFSTS_TXDT_FIFO_RENTRIES_POS)
#define RXTX_FFSTS_RXDT_FIFO_EMPTY_POS     16
#define RXTX_FFSTS_RXDT_FIFO_EMPTY_BITS    BITS1
#define RXTX_FFSTS_RXDT_FIFO_EMPTY         (RXTX_FFSTS_RXDT_FIFO_EMPTY_BITS << RXTX_FFSTS_RXDT_FIFO_EMPTY_POS)
#define RXTX_FFSTS_RXDT_FIFO_FULL_POS      17
#define RXTX_FFSTS_RXDT_FIFO_FULL_BITS     BITS1
#define RXTX_FFSTS_RXDT_FIFO_FULL          (RXTX_FFSTS_RXDT_FIFO_FULL_BITS << RXTX_FFSTS_RXDT_FIFO_FULL_POS)
#define RXTX_FFSTS_RXDT_FIFO_WR_ERROR_POS  18
#define RXTX_FFSTS_RXDT_FIFO_WR_ERROR_BITS BITS1
#define RXTX_FFSTS_RXDT_FIFO_WR_ERROR      (RXTX_FFSTS_RXDT_FIFO_WR_ERROR_BITS << RXTX_FFSTS_RXDT_FIFO_WR_ERROR_POS)
#define RXTX_FFSTS_RXDT_FIFO_RD_ERROR_POS  19
#define RXTX_FFSTS_RXDT_FIFO_RD_ERROR_BITS BITS1
#define RXTX_FFSTS_RXDT_FIFO_RD_ERROR      (RXTX_FFSTS_RXDT_FIFO_RD_ERROR_BITS << RXTX_FFSTS_RXDT_FIFO_RD_ERROR_POS)
#define RXTX_FFSTS_RXDT_FIFO_WENTRIES_POS  20
#define RXTX_FFSTS_RXDT_FIFO_WENTRIES_BITS BITS3
#define RXTX_FFSTS_RXDT_FIFO_WENTRIES      (RXTX_FFSTS_RXDT_FIFO_WENTRIES_BITS << RXTX_FFSTS_RXDT_FIFO_WENTRIES_POS)
#define RXTX_FFSTS_RXDT_FIFO_RENTRIES_POS  24
#define RXTX_FFSTS_RXDT_FIFO_RENTRIES_BITS BITS3
#define RXTX_FFSTS_RXDT_FIFO_RENTRIES      (RXTX_FFSTS_RXDT_FIFO_RENTRIES_BITS << RXTX_FFSTS_RXDT_FIFO_RENTRIES_POS)









//SD CARD STATUS bit definitions // R1, R1b resp status
#define OUT_OF_RANGE_POS             31
#define OUT_OF_RANGE_BITS            BITS1
#define OUT_OF_RANGE                 (OUT_OF_RANGE_BITS << OUT_OF_RANGE_POS)
#define ADDRESS_ERROR_POS            30
#define ADDRESS_ERROR_BITS           BITS1
#define ADDRESS_ERROR                (ADDRESS_ERROR_BITS << ADDRESS_ERROR_POS)
#define BLOCK_LEN_ERROR_POS          29
#define BLOCK_LEN_ERROR_BITS         BITS1
#define BLOCK_LEN_ERROR              (BLOCK_LEN_ERROR_BITS << BLOCK_LEN_ERROR_POS)
#define ERASE_SEQ_ERROR_POS          28
#define ERASE_SEQ_ERROR_BITS         BITS1
#define ERASE_SEQ_ERROR              (ERASE_SEQ_ERROR_BITS << ERASE_SEQ_ERROR_POS)
#define ERASE_PARAM_POS              27
#define ERASE_PARAM_BITS             BITS1
#define ERASE_PARAM                  (ERASE_PARAM_BITS << ERASE_PARAM_POS)
#define WP_VIOLATION_POS             26
#define WP_VIOLATION_BITS            BITS1
#define WP_VIOLATION                 (WP_VIOLATION_BITS << WP_VIOLATION_POS)
#define CARD_IS_LOCKED_POS           25
#define CARD_IS_LOCKED_BITS          BITS1
#define CARD_IS_LOCKED               (CARD_IS_LOCKED_BITS << CARD_IS_LOCKED_POS)
#define LOCK_UNLOCK_FAILED_POS       24
#define LOCK_UNLOCK_FAILED_BITS      BITS1
#define LOCK_UNLOCK_FAILED           (LOCK_UNLOCK_FAILED_BITS << LOCK_UNLOCK_FAILED_POS)
#define COM_CRC_ERROR_POS            23
#define COM_CRC_ERROR_BITS           BITS1
#define COM_CRC_ERROR                (COM_CRC_ERROR_BITS << COM_CRC_ERROR_POS)
#define ILLEGAL_COMMAND_POS          22
#define ILLEGAL_COMMAND_BITS         BITS1
#define ILLEGAL_COMMAND              (ILLEGAL_COMMAND_BITS << ILLEGAL_COMMAND_POS)
#define CARD_ECC_FAILED_POS          21
#define CARD_ECC_FAILED_BITS         BITS1
#define CARD_ECC_FAILED              (CARD_ECC_FAILED_BITS << CARD_ECC_FAILED_POS)
#define CC_ERROR_POS                 20
#define CC_ERROR_BITS                BITS1
#define CC_ERROR                     (CC_ERROR_BITS << CC_ERROR_POS)
#define UNKNOWN_ERROR_POS            19
#define UNKNOWN_ERROR_BITS           BITS1
#define UNKNOWN_ERROR                (UNKNOWN_ERROR_BITS << UNKNOWN_ERROR_POS)
#define CSD_OVERWRITE_POS            16
#define CSD_OVERWRITE_BITS           BITS1
#define CSD_OVERWRITE                (CSD_OVERWRITE_BITS << CSD_OVERWRITE_POS)
#define WP_ERASE_SKIP_POS            15
#define WP_ERASE_SKIP_BITS           BITS1
#define WP_ERASE_SKIP                (WP_ERASE_SKIP_BITS << WP_ERASE_SKIP_POS)
#define CARD_ECC_DISABLED_POS        14
#define CARD_ECC_DISABLED_BITS       BITS1
#define CARD_ECC_DISABLED            (CARD_ECC_DISABLED_BITS << CARD_ECC_DISABLED_POS)
#define ERASE_RESET_POS              13
#define ERASE_RESET_BITS             BITS1
#define ERASE_RESET                  (ERASE_RESET_BITS << ERASE_RESET_POS)
#define CURRENT_STATE_POS            9
#define CURRENT_STATE_BITS           BITS4
#define CURRENT_STATE                (CURRENT_STATE_BITS << CURRENT_STATE_POS)
#define READY_FOR_DATA_POS           8
#define READY_FOR_DATA_BITS          BITS1
#define READY_FOR_DATA               (READY_FOR_DATA_BITS << READY_FOR_DATA_POS)
#define APP_CMD_POS                  5
#define APP_CMD_BITS                 BITS1
#define APP_CMD                      (APP_CMD_BITS << APP_CMD_POS)
#define AKE_SEQ_ERROR_POS            3
#define AKE_SEQ_ERROR_BITS           BITS1
#define AKE_SEQ_ERROR                (AKE_SEQ_ERROR_BITS << AKE_SEQ_ERROR_POS)

//SD CARD STATUS bit definitions // R3 resp status
#define OCR_REG_POS                  0
#define OCR_REG_BITS                 BITS32
#define OCR_REG                      (OCR_REG_BITS << OCR_REG_POS)

#define OCR_REG_VW_POS               15
#define OCR_REG_VW_BITS              BITS9
#define OCR_REG_VW                   (OCR_REG_VW_BITS << OCR_REG_VW_POS)
#define OCR_REG_CCS_POS              30
#define OCR_REG_CCS_BITS             BITS1
#define OCR_REG_CCS                  (OCR_REG_CCS_BITS << OCR_REG_CCS_POS)
#define OCR_REG_CPS_POS              31
#define OCR_REG_CPS_BITS             BITS1
#define OCR_REG_CPS                  (OCR_REG_CPS_BITS << OCR_REG_CPS_POS)




//SD CARD STATUS bit definitions // R6 resp status
#define R6_RCA_POS                   16
#define R6_RCA_BITS                  BITS16
#define R6_RCA                       (R6_RCA_BITS << R6_RCA_POS)
#define R6_COM_CRC_ERROR_POS         15
#define R6_COM_CRC_ERROR_BITS        BITS1
#define R6_COM_CRC_ERROR             (R6_COM_CRC_ERROR_BITS << R6_COM_CRC_ERROR_POS)
#define R6_ILLEGAL_COMMAND_POS       14
#define R6_ILLEGAL_COMMAND_BITS      BITS1
#define R6_ILLEGAL_COMMAND           (R6_ILLEGAL_COMMAND_BITS << R6_ILLEGAL_COMMAND_POS)
#define R6_UNKNOWN_ERROR_POS         13
#define R6_UNKNOWN_ERROR_BITS        BITS1
#define R6_UNKNOWN_ERROR             (R6_UNKNOWN_ERROR_BITS << R6_UNKNOWN_ERROR_POS)
#define R6_CURRENT_STATE_POS         9
#define R6_CURRENT_STATE_BITS        BITS4
#define R6_CURRENT_STATE             (R6_CURRENT_STATE_BITS << R6_CURRENT_STATE_POS)
#define R6_READY_FOR_DATA_POS        8
#define R6_READY_FOR_DATA_BITS       BITS1
#define R6_READY_FOR_DATA            (R6_READY_FOR_DATA_BITS << R6_READY_FOR_DATA_POS)
#define R6_APP_CMD_POS               5
#define R6_APP_CMD_BITS              BITS1
#define R6_APP_CMD                   (R6_APP_CMD_BITS << R6_APP_CMD_POS)
#define R6_AKE_SEQ_ERROR_POS         3
#define R6_AKE_SEQ_ERROR_BITS        BITS1
#define R6_AKE_SEQ_ERROR             (R6_AKE_SEQ_ERROR_BITS << R6_AKE_SEQ_ERROR_POS)

//SD CARD STATUS bit definitions // R7 resp status
#define R7_ECHO_CHECK_PATTERN_POS    0
#define R7_ECHO_CHECK_PATTERN_BITS   BITS8
#define R7_ECHO_CHECK_PATTERN        (R7_ECHO_CHECK_PATTERN_BITS << R7_ECHO_CHECK_PATTERN_POS)
#define R7_VOLT_ACCEPTED_POS         8
#define R7_VOLT_ACCEPTED_BITS        BITS4
#define R7_VOLT_ACCEPTED             (R7_VOLT_ACCEPTED_BITS << R7_VOLT_ACCEPTED_POS)

//SD STATUS bit definitions 
#define ERASE_OFFSET_POS             400
#define ERASE_OFFSET_BITS            BITS2
#define ERASE_OFFSET                 (ERASE_OFFSET_BITS << ERASE_OFFSET_POS)
#define ERASE_TIMEOUT_POS            402
#define ERASE_TIMEOUT_BITS           BITS6
#define ERASE_TIMEOUT                (ERASE_TIMEOUT_BITS << ERASE_TIMEOUT_POS)
#define ERASE_SIZE_POS               408
#define ERASE_SIZE_BITS              BITS16
#define ERASE_SIZE                   (ERASE_SIZE_BITS << ERASE_SIZE_POS)
#define AU_SIZE_POS                  428
#define AU_SIZE_BITS                 BITS4
#define AU_SIZE                      (AU_SIZE_BITS << AU_SIZE_POS)
#define PERFOMANCE_MOVE_POS          432
#define PERFOMANCE_MOVE_BITS         BITS8
#define PERFOMANCE_MOVE              (PERFOMANCE_MOVE_BITS << PERFOMANCE_MOVE_POS)
#define SPEED_CLASS_POS              440
#define SPEED_CLASS_BITS             BITS8
#define SPEED_CLASS                  (SPEED_CLASS_BITS << SPEED_CLASS_POS)
#define SIZE_PROTECTED_AREA_POS      448
#define SIZE_PROTECTED_AREA_BITS     BITS32
#define SIZE_PROTECTED_AREA          (SIZE_PROTECTED_AREA_BITS << SIZE_PROTECTED_AREA_POS)
#define SD_CARD_TYPE_POS             480
#define SD_CARD_TYPE_BITS            BITS16
#define SD_CARD_TYPE                 (SD_CARD_TYPE_BITS << SD_CARD_TYPE_POS)
#define SECURED_MODE_POS             509
#define SECURED_MODE_BITS            BITS1
#define SECURED_MODE                 (SECURED_MODE_BITS << SECURED_MODE_POS)
#define DAT_BUS_WIDTH_PATTERN_POS    510
#define DAT_BUS_WIDTH_PATTERN_BITS   BITS2
#define DAT_BUS_WIDTH_PATTERN        (DAT_BUS_WIDTH_PATTERN_BITS << DAT_BUS_WIDTH_PATTERN_POS)

//CID register bit definitions 
#define MFG_DATE_POS                 8
#define MFG_DATE_BITS                BITS12
#define MFG_DATE                     (MFG_DATE_BITS << MFG_DATE_POS)
#define PROD_SER_NUM_POS             24
#define PROD_SER_NUM_BITS            BITS32
#define PROD_SER_NUM                 (PROD_SER_NUM_BITS << PROD_SER_NUM_POS)
#define PROD_REV_POS                 56
#define PROD_REV_BITS                BITS8
#define PROD_REV                     (PROD_REV_BITS << PROD_REV_POS)
#define PROD_NAME_POS                64
#define PROD_NAME_BITS               BITS40
#define PROD_NAME                    (PROD_NAME_BITS << PROD_NAME_POS)
#define OEM_ID_POS                   104
#define OEM_ID_BITS                  BITS16
#define OEM_ID                       (OEM_ID_BITS << OEM_ID_POS)
#define MFG_ID_POS                   120
#define MFG_ID_BITS                  BITS8
#define MFG_ID                       (MFG_ID_BITS << MFG_ID_POS)

//CSD register bit definitions 
#define CSD_STRUCTURE_POS            126
#define CSD_STRUCTURE_BITS           BITS2
#define CSD_STRUCTURE                (CSD_STRUCTURE_BITS << CSD_STRUCTURE_POS)
#define TAAC_POS                     112
#define TAAC_BITS                    BITS8
#define TAAC                         (TAAC_BITS << TAAC_POS)
#define NSAC_POS                     104
#define NSAC_BITS                    BITS8
#define NSAC                         (NSAC_BITS << NSAC_POS)
#define TRAN_SPEED_POS               96
#define TRAN_SPEED_BITS              BITS8
#define TRAN_SPEED                   (TRAN_SPEED_BITS << TRAN_SPEED_POS)
#define CCC_POS                      84
#define CCC_BITS                     BITS12
#define CCC                          (CCC_BITS << CCC_POS)
#define READ_BL_LEN_POS              80
#define READ_BL_LEN_BITS             BITS4
#define READ_BL_LEN                  (READ_BL_LEN_BITS << READ_BL_LEN_POS)
#define READ_BL_PARTIAL_POS          79
#define READ_BL_PARTIAL_BITS         BITS1
#define READ_BL_PARTIAL              (READ_BL_PARTIAL_BITS << READ_BL_PARTIAL_POS)
#define WRITE_BLK_MISALIGN_POS       78
#define WRITE_BLK_MISALIGN_BITS      BITS1
#define WRITE_BLK_MISALIGN           (WRITE_BLK_MISALIGN_BITS << WRITE_BLK_MISALIGN_POS)
#define READ_BLK_MISALIGN_POS        77
#define READ_BLK_MISALIGN_BITS       BITS1
#define READ_BLK_MISALIGN            (READ_BLK_MISALIGN_BITS << READ_BLK_MISALIGN_POS)
#define DSR_IMP_POS                  76
#define DSR_IMP_BITS                 BITS1
#define DSR_IMP                      (DSR_IMP_BITS << DSR_IMP_POS)
#define CSD1_C_SIZE_POS              62
#define CSD1_C_SIZE_BITS             BITS12
#define CSD1_C_SIZE                  (CSD1_C_SIZE_BITS << CSD1_C_SIZE_POS)
#define CSD2_C_SIZE_POS              48
#define CSD2_C_SIZE_BITS             BITS22
#define CSD2_C_SIZE                  (CSD2_1_SIZE_BITS << CSD2_C_SIZE_POS)
#define CSD1_C_SIZE_MULT_POS         47
#define CSD1_C_SIZE_MULT_BITS        BITS3
#define CSD1_C_SIZE_MULT             (CSD1_C_SIZE_MULT_BITS << CSD1_C_SIZE_MULT_POS)
#define ERASE_BLK_EN_POS             46
#define ERASE_BLK_EN_BITS            BITS1
#define ERASE_BLK_EN                 (ERASE_BLK_EN_BITS << ERASE_BLK_EN_POS)
#define ERASE_SECTOR_SIZE_POS        39
#define ERASE_SECTOR_SIZE_BITS       BITS7
#define ERASE_SECTOR_SIZE            (ERASE_SECTOR_SIZE_BITS << ERASE_SECTOR_SIZE_POS)
#define WP_GRP_SIZE_POS              32
#define WP_GRP_SIZE_BITS             BITS7
#define WP_GRP_SIZE                  (WP_GRP_SIZE_BITS << WP_GRP_SIZE_POS)
#define WP_GRP_ENABLE_POS            31
#define WP_GRP_ENABLE_BITS           BITS1
#define WP_GRP_ENABLE                (WP_GRP_ENABLE_BITS << WP_GRP_ENABLE_POS)
#define R2W_FACTOR_POS               26
#define R2W_FACTOR_BITS              BITS3
#define R2W_FACTOR                   (R2W_FACTOR_BITS << R2W_FACTOR_POS)
#define WRITE_BL_LEN_POS             22
#define WRITE_BL_LEN_BITS            BITS4
#define WRITE_BL_LEN                 (WRITE_BL_LEN_BITS << WRITE_BL_LEN_POS)
#define WRITE_BL_PARTIAL_POS         21
#define WRITE_BL_PARTIAL_BITS        BITS1
#define WRITE_BL_PARTIAL             (WRITE_BL_PARTIAL_BITS << WRITE_BL_PARTIAL_POS)
#define FILE_FORMAT_GRP_POS          15
#define FILE_FORMAT_GRP_BITS         BITS1
#define FILE_FORMAT_GRP              (FILE_FORMAT_GRP_BITS << FILE_FORMAT_GRP_POS)
#define CSD_COPY_POS                 14
#define CSD_COPY_BITS                BITS1
#define CSD_COPY                     (CSD_COPY_BITS << CSD_COPY_POS)
#define PERM_WRITE_PROTECT_POS       13
#define PERM_WRITE_PROTECT_BITS      BITS1
#define PERM_WRITE_PROTECT           (PERM_WRITE_PROTECT_BITS << PERM_WRITE_PROTECT_POS)
#define TMP_WRITE_PROTECT_POS        12
#define TMP_WRITE_PROTECT_BITS       BITS1
#define TMP_WRITE_PROTECT            (TMP_WRITE_PROTECT_BITS << TMP_WRITE_PROTECT_POS)
#define FILE_FORMAT_POS              10
#define FILE_FORMAT_BITS             BITS2
#define FILE_FORMAT                  (FILE_FORMAT_BITS << FILE_FORMAT_POS)

//MEMORY LAYOUT
// Cached memory
#define SDHC_TX_START_ADDR      SDHC_START              // 80CA_B800
#define SDHC_TX_END_ADDR        SDHC_START +  0x3ffff   // 256KB
#define SDHC_RX_START_ADDR      SDHC_TX_END_ADDR + 1    // 80CE_B800
#define SDHC_RX_END_ADDR        SDHC_RX_START_ADDR + + 0x3ffff // 256KB

// Un-Cached memory
#define SDHC_UC_TX_START_ADDR   SYSTEST_SDHC_UC_WORKAREA
#define SDHC_UC_RX_START_ADDR   SYSTEST_SDHC_UC_WORKAREA +  0x80000
#define SDHC_UC_RX_END_ADDR     SDHC_UC_RX_START_ADDR  +  0x7ffff
#define SDHC1_UC_TX_START_ADDR  SYSTEST_PCIE_SECOND_FUNC
#define SDHC1_UC_RX_START_ADDR  SYSTEST_PCIE_SECOND_FUNC_END
#define SDHC1_UC_RX_END_ADDR    SDHC1_UC_RX_START_ADDR  +  0x7ffff

#define SDHC_LMEM_ADDR          SDHC_LMEM_START
#define SDHC1_LMEM_ADDR          SDHC1_LMEM_START

#define SDHC_Intr_Flag          SDHCWorkBase_START + 0x0
//Verilog Tasks
#define VERILOG_TASK_SDHC_PARAM VERILOG_TASK_SDHC //0x09ff8250
#define SDHC_VT_MODE            0x0
#define SDHC_VT_BLKCNT          0x4
#define SDHC_VT_BLKLEN          0x6
#define SDHC_VT_DEVICE_ADDR     0x8
#define SDHC_VT_HOST_ADDR       0xC

#define SD_COMPARE_DATA_TASK    0xFFFF0032
#define SD_READ_PIO_DATA_TASK   0xFFFF0033
#define SD_WRITE_PIO_DATA_TASK  0xFFFF0034

//function and tasks
extern void enableAllInterrupts (void);
extern void secCLKCTL (tU16, tU8, tU8, tU8);
extern void setBLKCTL (tU16, tU16);
extern void setCMDARG (tU32);
extern void setCMDCTL (tU8, tU8, tU8, tU8, tU8, tU8, tU8, tU8);
extern void setSDMAADDR (tU32);
extern tU8 checkCMDdone (void);
extern tU8 checkCMDDatadone (void);
extern tU8 checkAUTOCMD12done (void);
extern tU8 checkRxDtRdy (void);
extern void clearRxDtRdy (void);
extern void setTxDtRdy (void);
extern void checkTxDtDone (void);
extern void SDHCInit (void);
extern void setCLKCTL (tU16, tU8, tU8, tU8);
extern void setupLMEMPageCFG (tU8, tU8, tU16, tU8);
extern void cmd000Task (void);
extern tU8  cmd001Task (void);
extern tU8  cmd002Task (void);
extern tU16 cmd003Task (tU8);
extern void slectCard (tU16);
extern tU8 cmd008Task (void);
extern void cmd009Task (tU16);
extern void cmd010Task (tU16);
extern tU8 cmd016Task (tU16);
extern void cmd055Task (tU16);
extern void setBusWidth (tU16, tU8);
extern void acmd041Task (tU16, tU32, tU8);
extern tU8 BlockReadTask (tU8, tU16, tU16, tU32, tU32);
extern void BlockWriteTask (tU8, tU16, tU16, tU32, tU32);
extern tU16 CardIdentTest (void);
extern void setup_WTS_SD_PIO_DATA_TASK(tU16, tU32);
extern void setup_WTS_SD_COMPARE_DATA_TASK(tU8, tU16, tU16, tU32, tU32);
extern void selectCard(tU16);
extern void StorePIODataTask (tU16, tU32, tU16);
extern void LoadPIODataTask (tU16, tU32, tU16);
extern tU32 SDCompareDataTask (tU16, tU16, tU32, tU32);
extern void SDCardStatus (void);
extern tU16 checkLMEMPGAvailable (void);
extern tU16 getLMEMPGValidCnt (void);
extern tU16 getLMEMPGCompletionPtr (void);
extern tU8  getLMEMPGSize(void);
extern tU16 getLMEMBaseAddr(void);
extern tU16 getLMEMLimitAddr(void);
extern void checkCardDetect (void);
extern void getCardCIDFromResp (void);
extern tU8 getCardIntfCondFromResp(void);
extern void getCardCSDFromResp (void);
extern void getCardOCRFromResp (void);
extern tU16 getRCAFromResp(void);
extern void initHostData (tU16, tU16, tU32);
tU32 checkRegisters (void);
extern void setCLK400KHz_25MHz (void);
extern void setCLK400KHz_12_5MHz (void);
extern void clearCRCError (void);
extern void getSDStatus (tU16);
extern void clearHostData (tU16, tU16, tU32);
extern void clearLMEMData (void);
extern void SDHCINThandler (void);
extern void enableARMInterrupt (void);
extern void  PrintTestConfig (tU8, tU8, tU8, tU16, tU16, tU32, tU32, tU32);
extern void mmcSendEXTCSD (tU8, tU16, tU16, tU32, tU32);
extern void mmcCmd006Task (tU8 );
extern void setMMCBusWidth (tU8, tU8, tU32);
extern void setSDHCBusCTLReg (tU8);
extern void setMMCClkSpeed (tU8, tU8, tU32);
extern void setSDHighSpeed (tU8, tU8, tU32);
extern void ReadSDHighSpeed (tU8, tU8, tU32);

extern void enableAllInterrupts1 (void);
extern void secCLKCTL1 (tU16, tU8, tU8, tU8);
extern void setBLKCTL1 (tU16, tU16);
extern void setCMDARG1 (tU32);
extern void setCMDCTL1 (tU8, tU8, tU8, tU8, tU8, tU8, tU8, tU8);
extern void setSDMAADDR1 (tU32);
extern tU8 checkCMDdone1 (void);
extern tU8 checkCMDDatadone1 (void);
extern tU8 checkAUTOCMD12done1 (void);
extern tU8 checkRxDtRdy1 (void);
extern void clearRxDtRdy1 (void);
extern void setTxDtRdy1 (void);
extern void checkTxDtDone1 (void);
extern void SDHCInit1 (void);
extern void setCLKCTL1 (tU16, tU8, tU8, tU8);
extern void setupLMEMPageCFG1 (tU8, tU8, tU16, tU8);
extern void cmd000Task1 (void);
extern tU8  cmd001Task1 (void);
extern tU8  cmd002Task1 (void);
extern tU16 cmd003Task1 (tU8);
extern void slectCard1 (tU16);
extern tU8 cmd008Task1 (void);
extern void cmd009Task1 (tU16);
extern void cmd010Task1 (tU16);
extern tU8 cmd016Task1 (tU16);
extern void cmd055Task1 (tU16);
extern void setBusWidth1 (tU16, tU8);
extern void acmd041Task1 (tU16, tU32, tU8);
extern tU8 BlockReadTask1 (tU8, tU16, tU16, tU32, tU32);
extern void BlockWriteTask1 (tU8, tU16, tU16, tU32, tU32);
extern tU16 CardIdentTest1 (void);
extern void setup_WTS_SD_PIO_DATA_TASK1(tU16, tU32);
extern void setup_WTS_SD_COMPARE_DATA_TASK1(tU8, tU16, tU16, tU32, tU32);
extern void selectCard1(tU16);
extern void StorePIODataTask1 (tU16, tU32, tU16);
extern void LoadPIODataTask1 (tU16, tU32, tU16);
extern tU32 SDCompareDataTask1 (tU16, tU16, tU32, tU32);
extern void SDCardStatus1 (void);
extern tU16 checkLMEMPGAvailable1 (void);
extern tU16 getLMEMPGValidCnt1 (void);
extern tU16 getLMEMPGCompletionPtr1 (void);
extern tU8  getLMEMPGSize1(void);
extern tU16 getLMEMBaseAddr1(void);
extern tU16 getLMEMLimitAddr1(void);
extern void checkCardDetect1 (void);
extern void getCardCIDFromResp1 (void);
extern tU8 getCardIntfCondFromResp1(void);
extern void getCardCSDFromResp1 (void);
extern void getCardOCRFromResp1 (void);
extern tU16 getRCAFromResp1(void);
extern void initHostData1 (tU16, tU16, tU32);
tU32 checkRegisters1 (void);
extern void setCLK400KHz_25MHz_1 (void);
extern void setCLK400KHz_12_5MHz_1 (void);
extern void clearCRCError1 (void);
extern void getSDStatus1 (tU16);
extern void clearHostData1 (tU16, tU16, tU32);
extern void clearLMEMData1 (void);
extern void SDHCINThandler1 (void);
extern void enableARMInterrupt1 (void);
extern void  PrintTestConfig1 (tU8, tU8, tU8, tU16, tU16, tU32, tU32, tU32);
extern void mmcSendEXTCSD1 (tU8, tU16, tU16, tU32, tU32);
extern void mmcCmd006Task1 (tU8 );
extern void setMMCBusWidth1 (tU8, tU8, tU32);
extern void setSDHCBusCTLReg1 (tU8);
extern void setMMCClkSpeed1 (tU8, tU8, tU32);
extern void setSDHighSpeed1 (tU8, tU8, tU32);
extern void ReadSDHighSpeed1 (tU8, tU8, tU32);

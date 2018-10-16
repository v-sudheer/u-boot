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

//////// Bomber Sys control ////////////////
#define BOMBER_SYS_CTRL (SYSCONTROL+0x58)
#define BOMBER_IMEM	(0x04007000)

// Bit definitions for sys control
#define bom_swc_mux             1<<0
#define bom_sgpio_mux           1<<1
#define bom_pthru_mux           1<<2
#define bom_gpio_mux            1<<3
#define bom_chasi_mux           1<<4
#define bom_fantach_mux         1<<5

#define bom_i2c_sda0_mux        1<<6
#define bom_i2c_sda1_mux        2<<6
#define bom_i2c_sda2_mux        3<<6
#define bom_i2c_sda3_mux        4<<6
#define bom_i2c_sda4_mux        5<<6
#define bom_i2c_sda5_mux        6<<6
#define bom_i2c_sda6_mux        7<<6
#define bom_i2c_sda7_mux        1<<22

#define bom_i2c_sdc0_mux        1<<6
#define bom_i2c_sdc1_mux        2<<6
#define bom_i2c_sdc2_mux        3<<6
#define bom_i2c_sdc3_mux        4<<6
#define bom_i2c_sdc4_mux        5<<6
#define bom_i2c_sdc5_mux        6<<6
#define bom_i2c_sdc6_mux        7<<6
#define bom_i2c_sdc7_mux        1<<22

#define bom_adc_mux             1<<9

//////////Bomber APB Control //////////////
#define BOMBERBASE               Bomber

#define BOMBER_APB_CTRL          0x00
#define ARM_BASE_ADDR_MSB        0x10
#define ARM_BASE_ADDR_LSB        0x14
#define ARM_MSG_SIZE             0x18
#define ARM_TOTAL_ENTRY          0x1c
#define ARM_WATERMARK            0x20
#define ARM_IRQ                  0x24
#define ARM_PI                   0x28
#define ARM_CI                   0x2c
#define ARM_MSG                  0x30
#define ARM_SYS_APB_INT_CTL0     0x34
#define ARM_SYS_APB_INT_CTL1     0x38
#define ARM_PER_APB_INT_CTL0     0x3C
#define ARM_PER_APB_INT_CTL1     0x40
#define ARM_SYS_APB_INT_STS0     0x44
#define ARM_SYS_APB_INT_STS1     0x48
#define ARM_PER_APB_INT_STS0     0x4C
#define ARM_PER_APB_INT_STS1     0x50

  
#define MCS8051_BASE_ADDR_MSB    0x58
#define MCS8051_BASE_ADDR_LSB    0x5C
#define MCS8051_MSG_SIZE         0x60
#define MCS8051_TOTAL_ENTRY      0x64
#define MCS8051_WATERMARK        0x68
#define MCS8051_IRQ              0x6C
#define MCS8051_PI               0x70
#define MCS8051_CI               0x74
#define MCS8051_MSG              0x78
#define MCS8051_CTRL             0x7C


#define BOMOVFLOINTRFLAG        BomberWorkBase
#define BOMMBINTRFLAG           BomberWorkBase+0x04

#define BOM_CTRL  Bomber+0x00
#define BOM_PC0   Bomber+0x04
#define BOM_PC1   Bomber+0x08
#define BOM_ID    Bomber+0x0C

  
//paging related registers
#define BOM_MCS8051_BASE_ADDR_MSB         0x58
#define BOM_MCS8051_BASE_ADDR_LSB         0x5C
#define BOM_MCS8051_MSG_SIZE              0x60
#define BOM_MCS8051_TOTAL_ENTRY           0x64
#define BOM_MCS8051_WATERMARK             0x68
#define BOM_MCS8051_IRQ                   0x6C
#define BOM_MCS8051_PI                    0x70
#define BOM_MCS8051_CI                    0x74
#define BOM_MCS8051_MSG                   0x78
#define BOM_MCS8051_CTRL                  0x7C


#define BOM_RAM_CONTROL                   0x80
#define BOM_RAM_PSTART_ADDR_MSB           0x84
#define BOM_RAM_PSTART_ADDR_LSB           0x88
#define BOM_RAM_CACHE_SIZE                0x8C
#define BOM_RAM_BYTES_PER_PAGE            0x90
#define BOM_RAM_PAGE_FETCH_STS            0x94
#define BOM_RAM_LMEM_BASE_MSB             0x98
#define BOM_RAM_LMEM_BASE_LSB             0x9C  
#define BOM_RAM_PAUSE_CONTROL             0xA0

#define BOM_RAM_DDR_ADDR_3                0xA4
#define BOM_RAM_DDR_ADDR_2                0xA8
#define BOM_RAM_DDR_ADDR_1                0xAC
#define BOM_RAM_DDR_ADDR_0                0xB0
#define BOM_RAM_LMEM_OFFSET_MSB           0xB4
#define BOM_RAM_LMEM_OFFSET_LSB           0xB8
#define BOM_RAM_INTERRUPT_STS             0xBC
#define BOM_RAM_INSTRUCTION_MSB           0xC0
#define BOM_RAM_INSTRUCTION_LSB           0xC4


#define BOM_DATA_SRC_ADDR_3               0xC8
#define BOM_DATA_SRC_ADDR_2               0xCC
#define BOM_DATA_SRC_ADDR_1               0xD0
#define BOM_DATA_SRC_ADDR_0               0xD4
#define BOM_DATA_DST_ADDR_3               0xD8
#define BOM_DATA_DST_ADDR_2               0xDC
#define BOM_DATA_DST_ADDR_1               0xE0
#define BOM_DATA_DST_ADDR_0               0xE4
#define BOM_DATA_LENGTH_MSB               0xE8
#define BOM_DATA_LENGTH_LSB               0xEC
#define BOM_DATA_XFR_CTL                  0xF0
#define BOM_DATA_XFR_STS                  0xF4
#define BOM_DATA_XFR_ERR_STS              0xF8

// Page related programming  
#define   bom_xdma_ddr_addr_3             0x81
#define   bom_xdma_ddr_addr_2             0xC9
#define   bom_xdma_lmem_addr_3            0x10
#define   bom_xdma_lmem_addr_2            0x00
#define   no_of_pages_8                   0x00
#define   no_of_pages_4                   0x01
#define   no_of_pages_2                   0x02
#define   bytes_per_page_128              0x00  
#define   bytes_per_page_256              0x01  
#define   bytes_per_page_512              0x02  

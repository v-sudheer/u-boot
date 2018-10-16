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


#define FUNC1_CONFIGSPACE_OFFSET	0x0F00
#define FUNC1_MMIOSPACE_OFFEST		0xA000
#define SERDES_TEST_OFFSET              0x1A00

#define FUNCTION1_CFG		        (FUNCTION1_BASE + FUNC1_CONFIGSPACE_OFFSET)
#define FUNCTION1_MMIO		        (FUNCTION1_BASE + FUNC1_MMIOSPACE_OFFEST)
#define FUNCTION1_MMIO_H2BQ1	        (FUNCTION1_MMIO + 0x0)
#define FUNCTION1_MMIO_H2BQ2	        (FUNCTION1_MMIO + 0x10)
#define FUNCTION1_MMIO_B2HQ1	        (FUNCTION1_MMIO + 0x20)
#define FUNCTION1_MMIO_B2HQ2	        (FUNCTION1_MMIO + 0x30)
#define PCIE_SERDES_TEST_BASE           (FUNCTION1_BASE + SERDES_TEST_OFFSET)

#define FUNCTION1_B2H_DOORBELL          (FUNCTION1_MMIO + 0x40)
#define FUNCTION1_H2B_DOORBELL          (FUNCTION1_MMIO + 0x44)

#define PCIE_FUNCTION1_Intr_Flag        PCIE_FUNCTION1_Start
#define PCIE_FUNCTION1_H2B_Q1_Flag      (PCIE_FUNCTION1_Start + 0x4)
#define PCIE_FUNCTION1_H2B_Q2_Flag      (PCIE_FUNCTION1_Start + 0x8)
#define PCIE_FUNCTION1_Error_Flag       (PCIE_FUNCTION1_Start + 0xC)
#define PCIE_FUNCTION1_HOST_ADDR	(PCIE_FUNCTION1_Start + 0x10)
#define PCIE_FUNCTION1_HOST_CHKSM	(PCIE_FUNCTION1_Start + 0x14)
#define PCIE_FUNCTION1_BMC_ADDR		(PCIE_FUNCTION1_Start + 0x18)
#define PCIE_FUNCTION1_BMC_CHKSM	(PCIE_FUNCTION1_Start + 0x1C)
#define PCIE_FUNCTION1_SCRATCH		(PCIE_FUNCTION1_Start + 0x20)

//Individual Register bit definitions
#define B2H_RING_DOORBELL               1 << 31 //b2h doorbell register, write a 1 to request host attention
#define B2H_ENABLE_INTB                 1 << 30 //enable INTB
//29-28 Rsvd(read 0)
#define B2H_QUEUE1_FULL                 1 << 27 //b2hq1 full
#define B2H_QUEUE1_EMPTY                1 << 26 //b2hq1 empty
#define B2H_QUEUE2_FULL                 1 << 25 //b2hq2 full
#define B2H_QUEUE2_EMPTY                1 << 24 //b2hq2 empty
#define B2H_QUEUE1_FULL_UNMASK          1 << 23 //Interrupt mask for b2hq1 full (write 1 to enable F1 interrupt for b2hq1_full)
#define B2H_QUEUE1_EMPTY_UNMASK         1 << 22 //Interrupt mask for b2hq1 empty (write 1 to enable F1 interrupt for b2hq1_empty)
#define B2H_QUEUE2_FULL_UNMASK          1 << 21 //Interrupt mask for b2hq2 full write 1 to enable F1 interrupt for b2hq1_full)
#define B2H_QUEUE2_EMPTY_UNMASK         1 << 20 //Mask for b2hq1 empty (write 1 to enable F1 interrupt for b2hq2_empty)
//19-0 Rsvd (read 0)

#define H2B_RING_DOORBELL               1 << 31 //b2h doorbell register, write a 1 to request host attention
#define H2B_ENABLE_INTB                 1 << 30 //enable INTB
//29-28 Rsvd(read 0)
#define H2B_QUEUE1_FULL                 1 << 27 //b2hq1 full
#define H2B_QUEUE1_EMPTY                1 << 26 //b2hq1 empty
#define H2B_QUEUE2_FULL                 1 << 25 //b2hq2 full
#define H2B_QUEUE2_EMPTY                1 << 24 //b2hq2 empty
#define H2B_QUEUE1_FULL_UNMASK          1 << 23 //Interrupt mask for b2hq1 full (write 1 to enable F1 interrupt for b2hq1_full)
#define H2B_QUEUE1_EMPTY_UNMASK         1 << 22 //Interrupt mask for b2hq1 empty (write 1 to enable F1 interrupt for b2hq1_empty)
#define H2B_QUEUE2_FULL_UNMASK          1 << 21 //Interrupt mask for b2hq2 full write 1 to enable F1 interrupt for b2hq1_full)
#define H2B_QUEUE2_EMPTY_UNMASK         1 << 20 //Mask for b2hq1 empty (write 1 to enable F1 interrupt for b2hq2_empty)
//19-0 Rsvd (read 0)

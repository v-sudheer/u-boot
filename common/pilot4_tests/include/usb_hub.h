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

/*****************************************************************************/
/*PROJECT  :  PILOT4                                                         */
/*MODULE   :  USB2.HOST SFRS/DMA - Specical Function Register, DMA register  */
/*****************************************************************************/

#define  DevENDPRST	        (USB2BASE +0x1A2)
#define  DEVUSBBYTCNTL            (USB2BASE+0x008)
#define  DEVUSBBYTCNTH            (USB2BASE+0x009)
#define  DEVUSB1_INTSTS           (SYSCONTROL+0x6B)    // (USB2BASE+0x1B0)
#define  DEVUSBIRQ                (USB2BASE+0x18C)
#define  devin07ien              (USB2BASE+0x196)
#define  devin815ien             (USB2BASE+0x197)
#define  DEVUSBIEN                (USB2BASE+0x198)
#define  DEVUSBIVECT              (USB2BASE+0x1A0)
#define  DEVUSB2_fnaddr           (USB2BASE+0x1A6)    
#define  DevFNADDR               (USB2BASE+0x1A6)
#define  DEVUSB2_fifoctrl         (USB2BASE+0x1A8)
#define  DEVUSB1_INTSTS_WRITE     (USB2BASE+0x1B0)
#define  DEVUSB1_INTENABLE        (USB2BASE+0x1B1)  
#define  DEVUSB2_DMA_IRQ          (USB2BASE+0x1B1)
#define  DEVOTGCTRL               (USB2BASE+0x1BE)
#define  DEVOTG2STS               (USB2BASE+0x1BF)
#define  devout07irq             (USB2BASE+0x188)
#define  devout815irq            (USB2BASE+0x189)
#define  devin07irq              (USB2BASE+0x18A)
#define  devout07ien             (USB2BASE+0x194)
#define  devout815ien            (USB2BASE+0x195)
#define  devin07errirq           (USB2BASE+0x1B4)
#define  devin815errirq          (USB2BASE+0x1B5)
#define  devinxerrien            (USB2BASE+0x1B8)
#define  devin815irq             (USB2BASE+0x18B)
#define  DEVDma_Sysmem_Addrs      (USB2BASE+0x200)
#define  DEVDma_Control_Reg       (USB2BASE+0x204)
#define  devin07errien           (USB2BASE+0x1B8)
#define  devin815errien          (USB2BASE+0x1B9)
#define  devout07errirq          (USB2BASE+0x1B6)
#define  devout815errirq         (USB2BASE+0x1B7)
#define  devout07errien          (USB2BASE+0x1BA) 
#define  devout815errien         (USB2BASE+0x1BB)
#define  out0maxpak              (USB2BASE+0x1E0)
#if 0
#define  devout0err              (USB2BASE+0x0C1)
#define  devout1err              (USB2BASE+0x0C5)
#define  devout2err              (USB2BASE+0x0C9)
#define  devout3err              (USB2BASE+0x0CD)
#define  devout4err              (USB2BASE+0x0D1)
#define  devout5err              (USB2BASE+0x0D5)
#define  devout6err              (USB2BASE+0x0D9)
#define  devout7err              (USB2BASE+0x0DD)
#define  devout8err              (USB2BASE+0x0E1)
#endif
#define  DEVDMA_DIR_IN            (0<<30)
#define  DEVDMA_DIR_OUT           (1<<30)
#define  DEVDMA_SHORT_IRQ          (USB2BASE+0x408)


#define  DEVOUT1_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x0c))
#define  DEVIN1_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x008))  

#define  DEVOUT2_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x14))
#define  DEVIN2_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x10))  

#define  DEVOUT3_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x1c))
#define  DEVIN3_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x18))  

#define  DEVOUT4_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x24))
#define  DEVIN4_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x20))  

#define  DEVOUT5_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x2c))
#define  DEVIN5_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x28))  

#define  DEVOUT6_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x34))
#define  DEVIN6_Regs   ((volatile usb1_eptsregs_host *)(USB2BASE+0x30))  



#define SE_PILOT_USB0_USB7_OUT_IRQ_REG_OFFSET 0x18A
#define SE_PILOT_USB8_USB15_OUT_IRQ_REG_OFFSET 0x18B
#define SETUP_SETUP_TOKEN_VALID_INTR_STATUS_REG_OFF     0x800
#define SETUP_DATA_VALID_INTR_STATUS_REG_OFF    0x801
#define EP0_IN_COMPL_INTR_STATUS_REG_OFF        0x802
#define EP0_IN_COMPL_INTR_ENABLE        0x806
#define SE_PILOT_USB0_USB7_IN_IRQ_REG_OFFSET 0x188
#define SE_PILOT_USB8_USB15_IN_IRQ_REG_OFFSET 0x189
#define EP0_OUT_COMPL_INTR_STATUS_REG_OFF       0x803
#define EP0_OUT_COMPL_INTR_ENABLE       0x807

#define SETUP_DATA_VALID_INTR_ENABLE    0x805
#define SETUP_TOKEN_INTR_ENABLE    0x804
#define SE_PILOT_USBIRQ_REG_OFFSET      0x18C
#define SE_PILOT_FIFO_CTRL_REG_OFFSET   0x1A8
#define SE_PILOT_ENDPRST_REG_OFFSET     0x1A2
#define DISCONNECT_DEVICES              0x80A

#define USB_BASE_ADDR USB2BASE

#define USB_DIR_OUT			0		/* to device */
#define USB_DIR_IN			0x80		/* to host */

/*
 * USB types, the second of three bRequestType fields
 */
#define USB_TYPE_MASK			(0x03 << 5)
#define USB_TYPE_STANDARD		(0x00 << 5)
#define USB_TYPE_CLASS			(0x01 << 5)
#define USB_TYPE_VENDOR			(0x02 << 5)
#define USB_TYPE_RESERVED		(0x03 << 5)

/*
 * USB recipients, the third of three bRequestType fields
 */
#define USB_RECIP_MASK			0x1f
#define USB_RECIP_DEVICE		0x00
#define USB_RECIP_INTERFACE		0x01
#define USB_RECIP_ENDPOINT		0x02
#define USB_RECIP_OTHER			0x03
/* From Wireless USB 1.0 */
#define USB_RECIP_PORT			0x04
#define USB_RECIP_RPIPE		0x05





#define USB_REQ_GET_STATUS		0x00
#define USB_REQ_CLEAR_FEATURE		0x01
#define USB_REQ_SET_FEATURE		0x03
#define USB_REQ_SET_ADDRESS		0x05
#define USB_REQ_GET_DESCRIPTOR		0x06
#define USB_REQ_SET_DESCRIPTOR		0x07
#define USB_REQ_GET_CONFIGURATION	0x08
#define USB_REQ_SET_CONFIGURATION	0x09
#define USB_REQ_GET_INTERFACE		0x0A
#define USB_REQ_SET_INTERFACE		0x0B


struct usb_ctrlrequest {
        tU8 bRequestType;
        tU8 bRequest;
        tU16 wValue;
        tU16 wIndex;
        tU16 wLength;
} __attribute__ ((packed));


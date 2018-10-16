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


#define  HCENDPRST	        (USB1BASE +0x1A2)
#define  HOTGIEN		(USB1BASE+0x1C0)
#define  HOTGSTATE		(USB1BASE+0x1BD)
#define  HOTGIRQ 		(USB1BASE+0x1BC)
#define  HUSBBYTCNTL            (USB1BASE+0x008)
#define  HUSBBYTCNTH            (USB1BASE+0x009)
#define  HUSB1_INTSTS           (SYSCONTROL+0x6B)    // (USB2BASE+0x1B0)
#define  HUSBIRQ                (USB1BASE+0x18C)
#define  hcin07ien              (USB1BASE+0x196)
#define  hcin815ien             (USB1BASE+0x197)
#define  HUSBIEN                (USB1BASE+0x198)
#define  HUSBIVECT              (USB1BASE+0x1A0)
#define  HUSB2_fnaddr           (USB1BASE+0x1A6)    
#define  HCFNADDR               (USB1BASE+0x1A6)
#define  HUSB2_fifoctrl         (USB1BASE+0x1A8)
#define  HUSB1_INTSTS_WRITE     (USB1BASE+0x1B0)
#define  HUSB1_INTENABLE        (USB1BASE+0x1B1)  
#define  HUSB2_DMA_IRQ          (USB1BASE+0x1B1)
#define  HOTGCTRL               (USB1BASE+0x1BE)
#define  HOTG2STS               (USB1BASE+0x1BF)
#define  hcout07irq             (USB1BASE+0x188)
#define  hcout815irq            (USB1BASE+0x189)
#define  hcin07irq              (USB1BASE+0x18A)
#define  hcout07ien             (USB1BASE+0x194)
#define  hcout815ien            (USB1BASE+0x195)
#define  hcin07errirq           (USB1BASE+0x1B4)
#define  hcin815errirq          (USB1BASE+0x1B5)
#define  hcinxerrien            (USB1BASE+0x1B8)
#define  hcin815irq             (USB1BASE+0x18B)
#define  HDma_Sysmem_Addrs      (USB1BASE+0x200)
#define  HDma_Control_Reg       (USB1BASE+0x204)
#define  hcin07errien           (USB1BASE+0x1B8)
#define  hcin815errien          (USB1BASE+0x1B9)
#define  hcout07errirq          (USB1BASE+0x1B6)
#define  hcout815errirq         (USB1BASE+0x1B7)
#define  hcout07errien          (USB1BASE+0x1BA) 
#define  hcout815errien         (USB1BASE+0x1BB)
#define  hcout0err              (USB1BASE+0x0C1)
#define  hcout1err              (USB1BASE+0x0C5)
#define  hcout2err              (USB1BASE+0x0C9)
#define  hcout3err              (USB1BASE+0x0CD)
#define  hcout4err              (USB1BASE+0x0D1)
#define  hcout5err              (USB1BASE+0x0D5)
#define  hcout6err              (USB1BASE+0x0D9)
#define  hcout7err              (USB1BASE+0x0DD)
#define  hcout8err              (USB1BASE+0x0E1)
#define  DMA_USB_STATUS         (USB1BASE+0x404)
#define  DMA_IVECT		(USB1BASE+0x410)
#define  HDMA_DIR_IN            (0<<30)
#define  HDMA_DIR_OUT           (1<<30)
#define  HDMA_SHORT_IRQ          (USB1BASE+0x408)
#define  HCin0err                (USB1BASE+0x0C3)  
#define  HCin1err                (USB1BASE+0x0C7)  
#define  HCin2err                (USB1BASE+0x0CB)  
#define  HCin3err                (USB1BASE+0x0CF)
#define  HCin4err                (USB1BASE+0x0D3)  
#define  HCin5err                (USB1BASE+0x0D7)
#define  HCin6err                (USB1BASE+0x0DB)   
#define  HCin7err                (USB1BASE+0x0DF)
#define  HCin8err                (USB1BASE+0x0E3)  
#define  HCIN1FNADDR             (USB1BASE+0x600)
#define  HCIN2FNADDR             (USB1BASE+0x601)
#define  HCIN3FNADDR             (USB1BASE+0x602)
#define  HCIN4FNADDR             (USB1BASE+0x603)
#define  HCIN5FNADDR             (USB1BASE+0x604)
#define  HCIN6FNADDR             (USB1BASE+0x605)
#define  HCIN7FNADDR             (USB1BASE+0x606)
#define  HCIN8FNADDR             (USB1BASE+0x607)
#define  HCOUT1FNADDR            (USB1BASE+0x608)
#define  HCOUT2FNADDR            (USB1BASE+0x609)
#define  HCOUT3FNADDR            (USB1BASE+0x60A)
#define  HCOUT4FNADDR            (USB1BASE+0x60B)
#define  HCOUT5FNADDR            (USB1BASE+0x60C)
#define  HCOUT6FNADDR            (USB1BASE+0x60D)
#define  HCOUT7FNADDR            (USB1BASE+0x60E)
#define  HCOUT8FNADDR            (USB1BASE+0x60F)


extern volatile int ENDP_0_15_OUT_INTR;
extern volatile int ENDP_0_15_IN_INTR;
extern volatile int ENDP_OUT1_INCR;
extern volatile int HIGH_SPEED_INTR;
extern volatile int START_OF_FRAME_INTR;
extern volatile int EP_015_INERR_INTR;
extern volatile int EP_015_OUTERR_INTR;
extern volatile int BUS_RESET_INTR;
extern volatile int ENDP_INCR;
extern volatile int ENDP8_INCR;
extern volatile int DMA_INTERRUPT;

typedef struct usb1neptsregs_host{
   tVU16 bytecount;
   tVU8  control;
   tVU8  cstatus;
   }usb1_eptsregs_host;


#define  HOUT1_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x0c))
#define  HIN1_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x008))  

#define  HOUT2_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x14))
#define  HIN2_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x10))  

#define  HOUT3_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x1c))
#define  HIN3_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x18))  

#define  HOUT4_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x24))
#define  HIN4_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x20))  

#define  HOUT5_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x2c))
#define  HIN5_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x28))  

#define  HOUT6_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x34))
#define  HIN6_Regs   ((volatile usb1_eptsregs_host *)(USB1BASE+0x30))  









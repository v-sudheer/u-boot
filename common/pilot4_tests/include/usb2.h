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

//#include "pilot_II.h"
//#include "pilot_types.h"
  
//#define USB2BASE                        0x40800000                       

#define P3_A1_USB2_DMA_ECO           1


#define   usb2_out0bc      (USB2BASE+0x0)
#define   usb2_in0bc       (USB2BASE+0x1)     
#define   usb2_ep0cs       (USB2BASE+0x2)

#define   IN0_BUSY          1<<2
  

//out1
#define   usb2_out1bc      (USB2BASE+0x8)   //2 bytes
#define   usb2_out1con     (USB2BASE+0xA) 
#define   usb2_out1cs      (USB2BASE+0xB)
//in1
#define   usb2_in1bc       (USB2BASE+0xC)
#define   usb2_in1con      (USB2BASE+0xE)
#define   usb2_in1cs       (USB2BASE+0xF)  



typedef struct usb2_eptsregs{
   tVU16 bytecount;
   tVU8  control;
   tVU8  cstatus;
   }usb2_eptsregs;


#define Single_Buff  0
#define Double_Buff  1
#define Tripple_Buff 2
#define Quad_Buff    3

#define Type_Bulk    (2<<2)
#define Type_Iso     (1<<2)
#define Type_Intrpt  (3<<2)

#define ep0stall         (1)
#define ep0hsnak         (1<<1)
#define Valid            (1<<7)
#define Busy             1<<1

#define Stall_bit    (1<<6)  

//Device 

#define  OUT1_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x8))
#define  IN1_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0xC))  

#define  OUT2_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x10))
#define  IN2_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x14))  

#define  OUT3_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x18))
#define  IN3_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x1C))  

#define  OUT4_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x20))
#define  IN4_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x24))  

#define  OUT5_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x28))
#define  IN5_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x2C))  

#define  OUT6_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x28))
#define  IN6_Regs   ((volatile usb2_eptsregs *)(USB2BASE+0x2C))  

//Host


  
  
  
#define  Fifo1data   (USB2BASE+0x84)
#define  Fifo2data   (USB2BASE+0x88)  			     
#define  Fifo3data   (USB2BASE+0x8C)  			     
#define  Fifo4data   (USB2BASE+0x90)  			     
#define  Fifo5data   (USB2BASE+0x94)  			     
#define  Fifo6data   (USB2BASE+0x98)  			     
#define  Fifo7data   (USB2BASE+0x9C)  			     
#define  Fifo8data   (USB2BASE+0xA0)  			       

#define Usb2IN0eptbuff   (USB2BASE+0x100)
#define Usb2Out0eptbuff   (USB2BASE+0x140)  
#define Usb2Setupbuffer0 (USB2BASE+0x180)

  
#define  USB2IN07IRQ     (USB2BASE+0x188)
#define  USB2IN815IRQ    (USB2BASE+0x189)
  
#define  USB2OUT07IRQ         (USB2BASE+0x18A)
#define  USB2OUT815IRQ       (USB2BASE+0x18B)
#define  USB2IRQ          (USB2BASE+0x18C)
#define  USB2OUT07PNGIRQ     (USB2BASE+0x18E)
#define  USB2OUT815PNGIRQ    (USB2BASE+18F)
#define  USB2IN07FULLIRQ     (USB2BASE+0x190)
#define  USB2IN815FULLIRQ    (USB2BASE+0x191)    
#define  USB2OUT07EMPTIRQ    (USB2BASE+0x192)  
#define  USB2OUT815EMPTIRQ   (USB2BASE+0x193)    
#define  USB2IN07IEN         (USB2BASE+0x194)                    
#define  USB2IN815IEN        (USB2BASE+0x195)
#define  USB2OUT07IEN        (USB2BASE+0x196)
#define  USB2OUT815IEN       (USB2BASE+0x197)      
#define  USB2_USBIEN          (USB2BASE+0x198)     
#define  USB2_OUT07PNGIEN     (USB2BASE+0x19A)    
#define  USB2_OUT815PNGIEN    (USB2BASE+0x19B)    
#define  USB2_IN07FULLIEN     (USB2BASE+0x19C)    
#define  USB2_IN815FULLIEN    (USB2BASE+0x19D)    
#define  USB2_OUT07EMPTIEN    (USB2BASE+0x19E)    
#define  USB2_OUT815EMPTIEN   (USB2BASE+0x19F)    
#define  USB2_IVect           (USB2BASE+0x1A0)
#define  USB2_fifoVect        (USB2BASE+0x1A1)      
#define  USB2_endprst         (USB2BASE+0x1A2)    
#define  USB2_usbcs           (USB2BASE+0x1A3)    
#define  USB2_frmnlow           (USB2BASE+0x1A4)    
#define  USB2_frmnhigh          (USB2BASE+0x1A5)    
#define  USB2_fnaddr           (USB2BASE+0x1A6)    
#define  USB2_clkgate          (USB2BASE+0x1A7)    
#define  USB2_fifoctrl         (USB2BASE+0x1A8)


//#if defined (P3_A1_USB2_DMA_ECO)
//#define  USB2_INTSTS           (USB2BASE+0x1B0) //dmabusy bit is moved back to systrl
//#else  
#define  USB2_INTSTS           (SYSCONTROL+0x6B)    // (USB2BASE+0x1B0)
//#endif
  
#define  USB2_INTSTS_WRITE     (USB2BASE+0x1B0)

//#define  USB2_INTSTS           (USB2BASE+0x1B0)
//#define  USB2_INTSTS_WRITE           (USB2BASE+0x1B0)
  
#define  USB2_INTENABLE         (USB2BASE+0x1B1)  



//DMA bit defines
//1B4 : dma start address register 
//1B8 : dma cntrl register
//1B8[15:0] : dma length , #of dwrods to transfer
//1B8[19:16] : destinationEP number 
//1B8[30]   : direction, 1 => memory to IN-EP, 0 => OUT-EP to memory
//1B8[31] : start dma
  
//#define Dma_Sysmem_Addrs       (USB2BASE+0x1B4)
//#define Dma_Control_Reg        (USB2BASE+0x1B8)
#define Dma_Sysmem_Addrs       (USB2BASE+0x200)
#define Dma_Control_Reg        (USB2BASE+0x204)



#define DMA_EPT1               (1<<16)
#define DMA_EPT2               (2<<16)
#define DMA_EPT3               (3<<16)
#define DMA_EPT4               (4<<16)
#define DMA_SETSTOP            (1<<24)  
#define DMA_SETPAUSE           (1<<25)
//#define DMA_IN_PROGRESS        (1<<29)
//#define USB2_DMA_BUSY        (1<<29)    
  
#define DMA_DIR_IN             (1<<30)
#define DMA_DIR_OUT            (0<<30)





#define DMA_START              (1<<31)


  
//bit defines for usb cs
#define USB_Slfpwr       1<<2
#define USB_rwaken       1<<3
#define USB_Enum         1<<4
#define USB_sigrsume     1<<5
#define USB_DISCON       1<<6
#define USB_wakesrc      1<<7



  
//bit defines for usb2_intsts
#define  usb_inrpt           1
#define  dma_inrpt           (1<<1)
#define  wkup_inrpt          (1<<2)
#define  fifo_inrpt          (1<<3)
#define USB2_DMA_BUSY        (1<<4)    
  


//bit defines for USBIEN
#define	USB2_SUDAVIR		(1)
#define	USB2_SOFIR		(1<<1)
#define	USB2_SUTOKIR		(1<<2)
#define	USB2_SUSPIR		(1<<3)
#define	USB2_URESIR		(1<<4)
#define	USB2_HSPIR		(1<<5)

// Defines
typedef void (*interrupthandler)(void);  

typedef struct usb2globalvars{
  tVU32   GetDesptrcnt;   
  tVU32   SendDesptrcnt;
  tVU32   Ept0Lastpkt;
  tVU32   Ept1Lastpkt;
  tVU32   Bulkout1cnt;
  tVU32   Bulkin1cnt;
  tVU32   EP0Lpktflg;
  tVU32   EP1Lpktflg;
  tVU32   EP2Lpktflg;
  tVU32   EP3Lpktflg;
  tVU32   EP4Lpktflg;
  tVU32   EP0INCNT;
  tVU32   EP1INCNT;
  tVU32   EP2INCNT;
  tVU32   EP3INCNT;
  tVU32   EP4INCNT;
  tPVU32  Ept0Inptr;
  tPVU32  Ept0Outptr;
  tPVU32  IN0databuffptr;
  tPVU32  IN1databuffptr;
  tPVU32  IN2databuffptr;
  tPVU32  IN3databuffptr;
  tPVU32  IN4databuffptr;
  tVU32   EP0OUTCNT;
  tVU32   EP1OUTCNT;
  tVU32   EP2OUTCNT;
  tVU32   EP3OUTCNT;
  tVU32   EP4OUTCNT;
  tPVU32  Out0databuffptr;
  tPVU32  Out1databuffptr;
  tPVU32  Out2databuffptr;
  tPVU32  Out3databuffptr;
  tPVU32  Out4databuffptr;
  tVU32   suspIntrflag;
  tVU32   usb2_dmaintr_flag;	
  tVU32   usb2_wupintr_flag;
  tVU32   usb2_wupintr_cnts;
  tVU32   usb2_rwkup_intr_cnts;
  tVU32   usb2_fifointr_flag;
  tVU32   EPT1_Sent;
  tVU32   EPT2_Sent;
  tVU32   EPT3_Sent;
  tVU32   EPT4_Sent;
  tVU32   ept0in_flag;
  tVU32   ept4pairing;
  tVU32   TotalCount;	
  tVU32   EPT1OUTTotalCount;
  tVU32   EPT2OUTTotalCount;
  tVU32   EPT3OUTTotalCount;
  tVU32   EPT4OUTTotalCount;
  tVU32   usb2_hspeed;			      
}usb2gloablvars;



//Define Working Buffer Space 


#define   SYSUSB2IN4databuff          SYSTEST_USB2_WORKBASE+0x200
#define   SYSUSB2OUT4databuff         ( (SYSUSB2IN4databuff)+(1024*13) ) 

#define	 USB2DATABUFFER	  USB2XmitBuffer        
#define  Usb2VendorInbuff      USB2DATABUFFER  	
#define  Usb2Ddescriptorbuff   USB2DATABUFFER+0x100	
#define	 Usb2Cfgdescriptorbuff USB2DATABUFFER+0x200
#define  Usb2String0descriptor USB2DATABUFFER+0x300
#define  Usb2String1descriptor USB2DATABUFFER+0x400
#define  Usb2String2descriptor USB2DATABUFFER+0x500
#define  Usb2String3descriptor USB2DATABUFFER+0x600
#define  Usb2String4descriptor USB2DATABUFFER+0x700


  
#define	USB2IN0databuff	USB2DATABUFFER+0x800

#if defined (CODE_IN_ASRAM)&& defined (HW_ENV)
#define	USB2IN1databuff	0x10000400
#else
#define	USB2IN1databuff	USB2DATABUFFER+0xC00
#endif
  
#define	USB2IN2databuff	USB2DATABUFFER+0x1000
#define	USB2IN3databuff	USB2DATABUFFER+0x1400
#define	USB2IN4databuff	USB2DATABUFFER+0x3400 
#define	USB2SPECBUFFER	0x809a0000		//Experiment to test usb2 64k crossing case


//OUT buffers
#define Usb2VendorOutbuff       USB2RecvBuffer    //USB2DATABUFFER+0x1800
#define	USB2Out0databuff	USB2RecvBuffer+0x400 //USB2DATABUFFER+0x1800
#if defined (CODE_IN_ASRAM)&& defined (HW_ENV)
#define	USB2Out1databuff	(USB2IN1databuff+(12*1024))// (USB2RecvBuffer+0x800)        //USB2DATABUFFER+0x1C00  
#else
#define	USB2Out1databuff	(USB2RecvBuffer+0x800)        //USB2DATABUFFER+0x1C00
#endif
  
#define	USB2Out2databuff	USB2RecvBuffer+0x1800             //USB2DATABUFFER+0x2000  
#define	USB2Out3databuff	USB2RecvBuffer+0x2800                 //USB2DATABUFFER+0x2400
#define	USB2Out4databuff	USB2RecvBuffer+0x3800   //USB2DATABUFFER+0x2800    


#define	USB2IN0IR		(0x1)
#define	USB2IN1IR		(1<<1)
#define	USB2IN2IR		(1<<2)
#define	USB2IN3IR		(1<<3)
#define	USB2IN4IR		(1<<4)

  
#define	USB2OUT0IR		(0x1)
#define	USB2OUT1IR		(1<<1)
#define	USB2OUT2IR		(1<<2)
#define	USB2OUT3IR		(1<<3)
#define	USB2OUT4IR		(1<<4)
  
#define	USB2IN0IEN		(0x1)
#define	USB2IN1IEN		(1<<1)
#define	USB2IN2IEN		(1<<2)

#define	USB2OUT0IEN		(0x1)
#define	USB2OUT1IEN		(1<<1)
#define	USB2OUT2IEN		(1<<2)



#define EPT1     (1)
#define EPT2     (2)
#define EPT3     (3)
#define EPT4     (4)
#define EPT5     (5)

#define DIR_OUT    (0<<4)
#define DIR_IN     (1<<4)  
#define FIFOAUTO   (1<<5)
#define FIFOCMIT   (1<<6)
#define FIFOACC    (1<<7)  
#define FIFORST    (1<<6)
#define TOGRST      (1<<5)

#define HDIR_OUT    (1<<4)
#define HDIR_IN     (0<<4)  
  
  
#define SignalTBtoStartUSB   0xFFFF0029

//vect ints
#define USB2_SUDAV         0
#define USB2_SOF           1
#define USB2_SUTOK         2
#define USB2_SUSPND        3
#define USB2_BUSRESET      4
#define USB2_HSPEED        5
#define USB2_EP0IN         6
#define USB2_EP0OUT        7
#define USB2_EP0PING       8
#define USB2_EP1IN         9
#define USB2_EP1OUT        10
#define USB2_EP1PING       11  
#define USB2_EP2IN         12
#define USB2_EP2OUT        13
#define USB2_EP2PING       14  
#define USB2_EP3IN         15
#define USB2_EP3OUT        16
#define USB2_EP3PING       17  
#define USB2_EP4IN         18
#define USB2_EP4OUT        19
#define USB2_EP4PING       20  
#define USB2_EP5IN         21
#define USB2_EP5OUT        22
#define USB2_EP5PING       23  

#define USB2_DBUFF        (1<<5) //this should have been ddma
#define USB2_DDMA        (1<<5) //this should have been ddma  

#define USB2_SPDUPSIMS   (1<<7)  
#define USB2_A2_ECO_EN   (1<<4)

  
		 
		 
//
extern void usb2_yDecodeStdCmd(tU32 Command);
extern void Set_FifoCtrl(tU32 control,tU32 dir,tU32 endptno);
extern tU32  Writeeptfifo(tPVU32 Srcaddr,tPVU32 Dstaddr,tU32  Cnt);
extern void Set_In_Ready(tU32 eptno);
extern tU32 Get_ByteCount(tU32 eptno);
extern tU32 Readeptfifo(tPVU32 Srcaddr,tPVU32 Dstaddr,tU32 Cnt);
extern void Set_Out_Ready(tU32 eptno);

extern interrupthandler usb2_Setupdatavalidhandler(void);
extern interrupthandler usb2_ept0_outhandler(void);
extern interrupthandler usb2_ept0_inhandler(void);
extern interrupthandler usb2_ept1_outhandler(void);
extern interrupthandler usb2_ept1_inhandler(void);
extern interrupthandler usb2_ept2_outhandler(void);
extern interrupthandler usb2_ept2_inhandler(void);
extern interrupthandler usb2_ept3_outhandler(void);
extern interrupthandler usb2_ept3_inhandler(void);
extern interrupthandler usb2_ept4_outhandler(void);
extern interrupthandler usb2_ept4_inhandler(void);

extern interrupthandler usb2_ept4_outhandler_usedma(void);
extern interrupthandler usb2_ept4_inhandler_usedma(void);
extern interrupthandler usb2_ept4_outhandler_use_systest_buffs(void);



extern tU32 get_toggle_bit(tU32, tU32);
extern void usb2_clear_wakeup (void);
extern void usb2_install_irq_handler(tU32 irqnum, interrupthandler handler);
extern void USB2_Main_Handler(void);
extern void usb2_dmahandler(void);
extern void usb2_fifo_handler(void);
extern void usb2_wakeup_handler(void);
extern void  usb2_vechandler(void);
extern tU32 Get_Out_npack(tU32 eptno);
extern tU32 Get_In_npack(tU32 eptno);
extern void usb_tophandler(void);
extern void Do_Fifo_Reset(tU32 dir,tU32 endptno);
extern void usb2_hspeed_handler(void);
extern void Do_Connect(void);
extern interrupthandler usb2_sus_handler(void);


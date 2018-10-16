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

#define	SHIFT	(2)
#define	PASSADDR	 (0x04001ffe)
#define	FAILADDR	 (0x04001fff)	

#define	USBBASE		(USB1BASE>>2)	//chgd bcos of left shift
#define	FIFOSIZE	(0x40)
#define	USBFIFOBASE	(USBBASE+0x340)

#define	Out7eptbuff	((USBFIFOBASE)<<SHIFT)
#define	Out6eptbuff	((USBFIFOBASE+0x80)<<SHIFT)
#define	Out5eptbuff	((USBFIFOBASE+0x100)<<SHIFT)
#define	Out4eptbuff	((USBFIFOBASE+0x180)<<SHIFT)
#define	Out3eptbuff	((USBFIFOBASE+0x200)<<SHIFT)
#define	Out2eptbuff	((USBFIFOBASE+0x280)<<SHIFT)
#define	Out1eptbuff	((USBFIFOBASE+0x300)<<SHIFT)
#define	Out0eptbuff	((USBFIFOBASE+0x380)<<SHIFT)

#define	IN7eptbuff	((USBFIFOBASE+0x40)<<SHIFT)
#define	IN6eptbuff	((USBFIFOBASE+0xC0)<<SHIFT)
#define	IN5eptbuff	((USBFIFOBASE+0x140)<<SHIFT)
#define	IN4eptbuff	((USBFIFOBASE+0x1C0)<<SHIFT)
#define	IN3eptbuff	((USBFIFOBASE+0x240)<<SHIFT)
#define	IN2eptbuff	((USBFIFOBASE+0x2C0)<<SHIFT)
#define	IN1eptbuff	((USBFIFOBASE+0x340)<<SHIFT)
#define	IN0eptbuff	((USBFIFOBASE+0x3C0)<<SHIFT)


#define	Dmabuffaddr0	((USBBASE+0x740)<<SHIFT)
#define	Dmabuffaddr1	((USBBASE+0x741)<<SHIFT)
#define	DmaSysmemaddr0	((USBBASE+0x742)<<SHIFT)
#define	DmaSysmemaddr1	((USBBASE+0x743)<<SHIFT)
#define	DmaSysmemaddr2	((USBBASE+0x744)<<SHIFT)
#define	DmaSysmemaddr3	((USBBASE+0x745)<<SHIFT)
#define	Dmalen		((USBBASE+0x746)<<SHIFT)
#define	DmaCtrl		((USBBASE+0x747)<<SHIFT)

#define	Out8data	((USBBASE+0x760)<<SHIFT)
#define	Out9data	((USBBASE+0x761)<<SHIFT)
#define	Out10data	((USBBASE+0x762)<<SHIFT)
#define	Out11data	((USBBASE+0x763)<<SHIFT)
#define	Out12data	((USBBASE+0x764)<<SHIFT)
#define	Out13data	((USBBASE+0x765)<<SHIFT)
#define	Out14data	((USBBASE+0x766)<<SHIFT)
#define	Out15data	((USBBASE+0x767)<<SHIFT)

#define	In8data		((USBBASE+0x768)<<SHIFT)
#define	In9data		((USBBASE+0x769)<<SHIFT)
#define	In10data	(USBBASE+0x76A)<<SHIFT)
#define	In11data	((USBBASE+0x76B)<<SHIFT)
#define	In12data	((USBBASE+0x76C)<<SHIFT)
#define	In13data	((USBBASE+0x76D)<<SHIFT)
#define	In14data	((USBBASE+0x76E)<<SHIFT)
#define	In15data	((USBBASE+0x76F)<<SHIFT)


#define	Out8bcl		((USBBASE+0x770)<<SHIFT)
#define	Out8bch		((USBBASE+0x771)<<SHIFT)
#define	Out9bcl		((USBBASE+0x772)<<SHIFT)
#define	Out9bch		((USBBASE+0x773)<<SHIFT)
#define	Out10bcl	((USBBASE+0x774)<<SHIFT)
#define	Out10bch	((USBBASE+0x775)<<SHIFT)
#define	Out11bcl	((USBBASE+0x776)<<SHIFT)
#define	Out11bch	((USBBASE+0x777)<<SHIFT)
#define	Out12bcl	((USBBASE+0x778)<<SHIFT)
#define	Out12bch	((USBBASE+0x779)<<SHIFT)
#define	Out13bcl	((USBBASE+0x77A)<<SHIFT)
#define	Out13bch	((USBBASE+0x77B)<<SHIFT)
#define	Out14bcl	((USBBASE+0x77C)<<SHIFT)
#define	Out14bch	((USBBASE+0x77D)<<SHIFT)
#define	Out15bcl	((USBBASE+0x77E)<<SHIFT)
#define	Out15bch	((USBBASE+0x77F)<<SHIFT)

#define	Clkgate		((USBBASE+0x790)<<SHIFT)

#define	Isoerr		((USBBASE+0x7A0)<<SHIFT)
#define	Zbcount		((USBBASE+0x7A2)<<SHIFT)

#define	IVec		((USBBASE+0x7A8)<<SHIFT)
#define	IN07IRQ		((USBBASE+0x7A9)<<SHIFT)
#define	OUT07IRQ		((USBBASE+0x7AA)<<SHIFT)
#define	USBIRQ		((USBBASE+0x7AB)<<SHIFT)
#define	IN07IEN		((USBBASE+0x7AC)<<SHIFT)
#define	OUT07IEN		((USBBASE+0x7AD)<<SHIFT)
#define	USBIEN		((USBBASE+0x7AE)<<SHIFT)
#define	USBBAV		((USBBASE+0x7AF)<<SHIFT)

#define	EP0CS		((USBBASE+0x7B4)<<SHIFT)
#define	IN0BC		((USBBASE+0x7B5)<<SHIFT)
#define	IN1CS		((USBBASE+0x7B6)<<SHIFT)
#define	IN1BC		((USBBASE+0x7B7)<<SHIFT)
#define	IN2CS		((USBBASE+0x7B8)<<SHIFT)
#define	IN2BC		((USBBASE+0x7B9)<<SHIFT)
#define	IN3CS		((USBBASE+0x7BA)<<SHIFT)
#define	IN3BC		((USBBASE+0x7BB)<<SHIFT)
#define	IN4CS		((USBBASE+0x7BC)<<SHIFT)
#define	IN4BC		((USBBASE+0x7BD)<<SHIFT)
#define	IN5CS		((USBBASE+0x7BE)<<SHIFT)
#define	IN5BC		((USBBASE+0x7BF)<<SHIFT)
#define	IN6CS		((USBBASE+0x7C0)<<SHIFT)
#define	IN6BC		((USBBASE+0x7C1)<<SHIFT)
#define	IN7CS		((USBBASE+0x7C2)<<SHIFT)
#define	IN7BC		((USBBASE+0x7C3)<<SHIFT)

#define	OUT0BC		((USBBASE+0x7C5)<<SHIFT)
#define	OUT1CS		((USBBASE+0x7C6)<<SHIFT)
#define	OUT1BC		((USBBASE+0x7C7)<<SHIFT)
#define	OUT2CS		((USBBASE+0x7C8)<<SHIFT)
#define	OUT2BC		((USBBASE+0x7C9)<<SHIFT)
#define	OUT3CS		((USBBASE+0x7CA)<<SHIFT)
#define	OUT3BC		((USBBASE+0x7CB)<<SHIFT)
#define	OUT4CS		((USBBASE+0x7CC)<<SHIFT)
#define	OUT4BC		((USBBASE+0x7CD)<<SHIFT)
#define	OUT5CS		((USBBASE+0x7CE)<<SHIFT)
#define	OUT5BC		((USBBASE+0x7CF)<<SHIFT)
#define	OUT6CS		((USBBASE+0x7D0)<<SHIFT)
#define	OUT6BC		((USBBASE+0x7D1)<<SHIFT)
#define	OUT7CS		((USBBASE+0x7D2)<<SHIFT)
#define	OUT7BC		((USBBASE+0x7D3)<<SHIFT)

#define	USBCS		((USBBASE+0x7D6)<<SHIFT)
#define	TOGCTL		((USBBASE+0x7D7)<<SHIFT)
#define	USBFRML		((USBBASE+0x7D8)<<SHIFT)
#define	USBFRMH		((USBBASE+0x7D9)<<SHIFT)
#define	FNADDR		((USBBASE+0x7DB)<<SHIFT)
#define	USBPAIR		((USBBASE+0x7DD)<<SHIFT)
#define	IN07VAL		((USBBASE+0x7DE)<<SHIFT)
#define	OUT07VAL		((USBBASE+0x7DF)<<SHIFT)
#define	INISOVAL		((USBBASE+0x7E0)<<SHIFT)
#define	OUTISOVAL	((USBBASE+0x7E1)<<SHIFT)

#define	Setupbuffer0	((USBBASE+0x7E8)<<SHIFT)
#define	Setupbuffer1	((USBBASE+0x7E9)<<SHIFT)
#define	Setupbuffer2	((USBBASE+0x7EA)<<SHIFT)
#define	Setupbuffer3	((USBBASE+0x7EB)<<SHIFT)
#define	Setupbuffer4	((USBBASE+0x7EC)<<SHIFT)
#define	Setupbuffer5	((USBBASE+0x7ED)<<SHIFT)
#define	Setupbuffer6	((USBBASE+0x7EE)<<SHIFT)
#define	Setupbuffer7	((USBBASE+0x7EF)<<SHIFT)

#define	Out8addr		((USBBASE+0x7F0)<<SHIFT)
#define	Out9addr		((USBBASE+0x7F1)<<SHIFT)
#define	Out10addr	((USBBASE+0x7F2)<<SHIFT)
#define	Out11addr	((USBBASE+0x7F3)<<SHIFT)
#define	Out12addr	((USBBASE+0x7F4)<<SHIFT)
#define	Out13addr	((USBBASE+0x7F5)<<SHIFT)
#define	Out14addr	((USBBASE+0x7F6)<<SHIFT)
#define	Out15addr	((USBBASE+0x7F7)<<SHIFT)


#define	In8addr		((USBBASE+0x7F8)<<SHIFT)
#define	In9addr		((USBBASE+0x7F9)<<SHIFT)
#define	In10addr		((USBBASE+0x7FA)<<SHIFT)
#define	In11addr		((USBBASE+0x7FB)<<SHIFT)
#define	In12addr		((USBBASE+0x7FC)<<SHIFT)
#define	In13addr		((USBBASE+0x7FD)<<SHIFT)
#define	In14addr		((USBBASE+0x7FE)<<SHIFT)
#define	In15addr		((USBBASE+0x7FF)<<SHIFT)
#define	USBINTRSTS		((USBBASE+0x800)<<SHIFT)
#define	USBINTREN		((USBBASE+0x801)<<SHIFT)


/*Reg bit definations*/
/*Dma Control Reg*/
#define	DmaSize8bit	(0x0)
#define	DmaSize16bit	(0x1)
#define	DmaSize32bit	(0x2)

#define	DmaDirWriteEptBuff  (1<<2)
#define	DmaNoIncExtMem	  (1<<3)
#define	DmaNoIncBuff	  (1<<4)
#define	DmaDone		  (1<<5)
#define	DmaBurst		(1<<6)

#define	SUDAVIR		(1)
#define	SOFIR		(1<<1)
#define	SUTOKIR		(1<<2)
#define	SUSPIR		(1<<3)
#define	URESIR		(1<<4)
#define	IBNIR		(1<<5)

#define	IN0IR		(0x1)
#define	IN1IR		(1<<1)
#define	IN2IR		(1<<2)
#define	IN3IR		(1<<3)
#define	IN4IR		(1<<4)
#define	IN5IR		(1<<5)
#define	IN6IR		(1<<6)
#define	IN7IR		(1<<7)

#define	OUT0IR		(0x1)
#define	OUT1IR		(1<<1)
#define	OUT2IR		(1<<2)
#define	OUT3IR		(1<<3)
#define	OUT4IR		(1<<4)
#define	OUT5IR		(1<<5)
#define	OUT6IR		(1<<6)
#define	OUT7IR		(1<<7)

#define	IN0IEN		(0x1)
#define	IN1IEN		(1<<1)
#define	IN2IEN		(1<<2)
#define	IN3IEN		(1<<3)
#define	IN4IEN		(1<<4)
#define	IN5IEN		(1<<5)
#define	IN6IEN		(1<<6)
#define	IN7IEN		(1<<7)

#define	OUT0IEN		(0x1)
#define	OUT1IEN		(1<<1)
#define	OUT2IEN		(1<<2)
#define	OUT3IEN		(1<<3)
#define	OUT4IEN		(1<<4)
#define	OUT5IEN		(1<<5)
#define	OUT6IEN		(1<<6)
#define	OUT7IEN		(1<<7)

#define	SUDAVEN		(1)
#define	SOFEN		(1<<1)
#define	SUTOKEN		(1<<2)
#define	SUSPEN		(1<<3)
#define	URESEN		(1<<4)
#define	IBNEN		(1<<5)

#define	AVEN		(1)
#define	EP0STALL	(1)
#define	HSNAK		(1<<1)
#define	IN0BUSY		(1<<2)
#define	OUT0BUSY	(1<<3)

#define	IN1STALL	(1)
#define	IN1BUSY		(1<<1)
#define	IN2STALL	(1)
#define	IN2BUSY		(1<<1)
#define	IN3STALL	(1)
#define	IN3BUSY		(1<<1)
#define	IN4STALL	(1)
#define	IN4BUSY		(1<<1)
#define	IN5STALL	(1)
#define	IN5BUSY		(1<<1)
#define	IN6STALL	(1)
#define	IN6BUSY		(1<<1)
#define	IN7STALL	(1)
#define	IN7BUSY		(1<<1)

#define	OUT1STALL	(1)
#define	OUT1BUSY	(1<<1)
#define	OUT2STALL	(1)
#define	OUT2BUSY	(1<<1)
#define	OUT3STALL	(1)
#define	OUT3BUSY	(1<<1)
#define	OUT4STALL	(1)
#define	OUT4BUSY	(1<<1)
#define	OUT5STALL	(1)
#define	OUT5BUSY	(1<<1)
#define	OUT6STALL	(1)
#define	OUT6BUSY	(1<<1)
#define	OUT7STALL	(1)
#define	OUT7BUSY	(1<<1)

#define	SIGRESUME	(1)
#define	DISCON		(1<<3)
#define	WAKESRC		(1<<7)

#define	TOGCTL_IO	(1<<4)
#define	TOGCTL_R		(1<<5)
#define	TOGCTL_S		(1<<6)
#define	TOGCTL_Q		(1<<7)

#define	USBPAIR_PR2IN	(1<<0)
#define	USBPAIR_PR4IN	(1<<1)
#define	USBPAIR_PR6IN	(1<<2)
#define	USBPAIR_PR2OUT	(1<<3)
#define	USBPAIR_PR4OUT	(1<<4)
#define	USBPAIR_PR6OUT	(1<<5)


#define	IN0VAL		(0x1)
#define	IN1VAL		(1<<1)
#define	IN2VAL		(1<<2)
#define	IN3VAL		(1<<3)
#define	IN4VAL		(1<<4)
#define	IN5VAL		(1<<5)
#define	IN6VAL		(1<<6)
#define	IN7VAL		(1<<7)

#define	OUT0VAL		(0x1)
#define	OUT1VAL		(1<<1)
#define	OUT2VAL		(1<<2)
#define	OUT3VAL		(1<<3)
#define	OUT4VAL		(1<<4)
#define	OUT5VAL		(1<<5)
#define	OUT6VAL		(1<<6)
#define	OUT7VAL		(1<<7)


#define	IN8VAL		(0x1)
#define	IN9VAL		(1<<1)
#define	IN10VAL		(1<<2)
#define	IN11VAL		(1<<3)
#define	IN12VAL		(1<<4)
#define	IN13VAL		(1<<5)
#define	IN14VAL		(1<<6)
#define	IN15VAL		(1<<7)

#define	OUT8VAL		(0x1)
#define	OUT9VAL		(1<<1)
#define	OUT10VAL		(1<<2)
#define	OUT11VAL		(1<<3)
#define	OUT12VAL		(1<<4)
#define	OUT13VAL		(1<<5)
#define	OUT14VAL		(1<<6)
#define	OUT15VAL		(1<<7)


//Define Vector No for each interrupt,this way interrupt handler can jump directly 
//using index into array of function pointers.
#define	SUDAV_INTNO	(0)
#define	SOF_INTNO	(1)
#define	SUTOK_INTNO	(2)
#define	SUS_INTNO	(3)
#define	USBREST_INTNO	(4)
#define	EP0IN_INTNO	(6)
#define	EP0OUT_INTNO	(7)
#define	EP1IN_INTNO	(8)
#define	EP1OUT_INTNO	(9)
#define	EP2IN_INTNO	(10)
#define	EP2OUT_INTNO	(11)
#define	EP3IN_INTNO	(12)
#define	EP3OUT_INTNO	(13)
#define	EP4IN_INTNO	(14)
#define	EP4OUT_INTNO	(15)
#define	EP5IN_INTNO	(16)
#define	EP5OUT_INTNO	(17)
#define	EP6IN_INTNO	(18)
#define	EP6OUT_INTNO	(19)
#define	EP7IN_INTNO	(20)
#define	EP8OUT_INTNO	(21)



//Define Working Buffer Space  in SDRAM
#define	USBDATABUFFER	(USBXmitBuffer)        
#define	Setupdatabuff	(USBDATABUFFER)
//IN buffers
#define	IN0databuff	(USBDATABUFFER+0x100)
#if defined (CODE_IN_ASRAM)&& defined (HW_ENV)
#define	IN1databuff	(0x10000400)   //IN0databuff+0x2f0
#else
#define	IN1databuff	(IN0databuff+0x2f0)
#endif
#define	IN2databuff	(IN1databuff+0x200)
#define	IN3databuff	(IN2databuff+0x200)
#define	IN4databuff	(IN3databuff+0x200)
#define	IN5databuff	(IN4databuff+0x200)
#define	IN6databuff	(IN5databuff+0x200)
#define	IN7databuff	(IN6databuff+0x200)
#define  VendorInbuff   	(VendorOutbuff+0x200)

#define  Ddescriptorbuff	(VendorOutexpectedbuff+0x200)
#define	Cfgdescriptorbuff (Ddescriptorbuff+0x200)
#define  String0descriptor (Cfgdescriptorbuff+0x200)
#define  String1descriptor (String0descriptor+0x200)
#define  String2descriptor (String1descriptor+0x200)
#define  String3descriptor (String2descriptor+0x200)
#define  String4descriptor (String3descriptor+0x200)


//OUT buffers
#define	Out0databuff	USBRecvBuffer       
#if defined (CODE_IN_ASRAM)&& defined (HW_ENV)
#define	Out1databuff	(0x10001000)
#else
#define	Out1databuff	(Out0databuff+0x3f0)
#endif
#define	Out2databuff	(Out1databuff+0x200)  
#define	Out3databuff	(Out2databuff+0x200)  
#define	Out4databuff	(Out3databuff+0x200)  
#define	Out5databuff	(Out4databuff+0x200)  
#define	Out6databuff	(Out5databuff+0x200)  
#define	Out7databuff	(Out6databuff+0x200)  
#define	VendorOutbuff	(Out7databuff+0x200)

//Expected buffers
#define	Out0expectedbuff	(VendorOutbuff+0x200)
#define	Out1expectedbuff	(Out0expectedbuff+0x200)  
#define	Out2expectedbuff	(Out1expectedbuff+0x200)  
#define	Out3expectedbuff	(Out2expectedbuff+0x200)  
#define	Out4expectedbuff	(Out3expectedbuff+0x200)  
#define	Out5expectedbuff	(Out4expectedbuff+0x200)  
#define	Out6expectedbuff	(Out5expectedbuff+0x200)  
#define	Out7expectedbuff	(Out6expectedbuff+0x200)  
#define	VendorOutexpectedbuff	(Out7expectedbuff+0x200)

// Defines
//typedef void (*interrupthandler)(void);

//Interrupt bits
#define	SUDAV	(0)
#define	SOF	(1)
#define	SUTOK	(2)
#define	SUS	(3)
#define	USBRES	(4)
#define	EP0IN	(6)
#define	EP0OUT	(7)
#define	EP1IN	(8)
#define	EP1OUT	(9)
#define	EP2IN	(10)
#define	EP2OUT	(11)
#define	EP3IN	(12)
#define	EP3OUT	(13)
#define	EP4IN	(14)
#define	EP4OUT	(15)
#define	EP5IN	(16)
#define	EP5OUT	(17)
#define	EP6IN	(18)
#define	EP6OUT	(19)
#define	EP7IN	(20)
#define	EP7OUT	(21)


//#define	USBINT	(1<<3)
//#define	USBINTNO	3

#define  DMAIN0BUFF   (0x0700)
#define  DMAIN1BUFF   (0x0680)
#define  DMAIN2BUFF   (0x0600)
#define  DMAIN3BUFF   (0x0580)
#define  DMAIN4BUFF   (0x0500)
#define  DMAIN5BUFF   (0x0480)
#define  DMAIN6BUFF   (0x0400)
#define  DMAIN7BUFF   (0x0380)


#define  DMAOUT0BUFF   (0x06C0)
#define  DMAOUT1BUFF   (0x0640)
#define  DMAOUT2BUFF   (0x05C0)
#define  DMAOUT3BUFF   (0x0540)
#define  DMAOUT4BUFF   (0x04C0)
#define  DMAOUT5BUFF   (0x0440)
#define  DMAOUT6BUFF   (0x03C0)
#define  DMAOUT7BUFF   (0x0340)




typedef struct usb1globalvars
  {
  tVU32 DMAIntrflag;
  tVU32  WakeupIntrflag;
tVU32  suspIntrflag;
tVU32  GetDesptrcnt;   
tVU32  SendDesptrcnt;
tVU32  Ept0Lastpkt;
tVU32  Ept1Lastpkt;
tPVU32 Ept0Inptr;
tPVU32 Ept0Outptr;
tVU32  Bulkout1cnt;
tVU32  Bulkin1cnt;
tVU32  EP0Lpktflg;
tVU32  EP1Lpktflg;
tVU32  EP2Lpktflg;
tVU32  EP3Lpktflg;
tVU32  EP4Lpktflg;
tVU32  EP5Lpktflg;
tVU32  EP6Lpktflg;
tVU32  EP7Lpktflg;
tVU32  EP0INCNT;
tVU32  EP1INCNT;
tVU32  EP2INCNT;
tVU32  EP3INCNT;
tVU32  EP4INCNT;
tVU32  EP5INCNT;
tVU32  EP6INCNT;
tVU32  EP7INCNT;
tPVU32 IN0databuffptr;
tPVU32 IN1databuffptr;
tPVU32 IN2databuffptr;
tPVU32 IN3databuffptr;
tPVU32 IN4databuffptr;
tPVU32 IN5databuffptr;
tPVU32 IN6databuffptr;
tPVU32 IN7databuffptr;
tVU32  EP0OUTCNT;
tVU32  EP1OUTCNT;
tVU32  EP2OUTCNT;
tVU32  EP3OUTCNT;
tVU32  EP4OUTCNT;
tVU32  EP5OUTCNT;
tVU32  EP6OUTCNT;
tVU32  EP7OUTCNT;
tPVU32 Out0databuffptr;
tPVU32 Out1databuffptr;
tPVU32 Out2databuffptr;
tPVU32 Out3databuffptr;
tPVU32 Out4databuffptr;
tPVU32 Out5databuffptr;
tPVU32 Out6databuffptr;
tPVU32 Out7databuffptr;
tVU32  ept0in_flag;
tVU32  INMPKT;
tVU32 device_addrs;
tVU32 INDmaEnable;
tVU32 OUTDmaEnable;
tVU32 PIOFixEnable;

}usb1gloablvars;



extern tU32 WriteWord(tU32 Addr,tU32 Value);
extern tU32 Writebyte(tU32 Addr,tU32 Value);
extern tU32 Writeonly(tU32 Addr,tU32 Value);
extern tU32 BitClearWordReg(tU32 Addr,tU32 Value);
extern tU32 BitClearByteReg(tU32 Addr,tU32 Value);
extern tU32 ReadWordReg(tU32 Addr);
extern tU32 ReadByteReg(tU32 Addr);
extern void INITBuffer(tPVU32 Addrs,tU32 initial,tU32 incrval,tU32 count);
extern tU32 Compare_Word(tPVU32 Srcaddr,tPVU32 DstAddrs,tU32 Count);
extern tU32 Compare_Byte(tPVU8 Srcaddr,tPVU8 DstAddrs,tU32 Count);
extern interrupthandler usb1_Setupdatavalidhandler();
extern interrupthandler usb1_ept0_inhandler(void);
extern interrupthandler usb1_ept1_inhandler(void);
extern interrupthandler usb1_ept2_inhandler(void);
extern interrupthandler usb1_ept3_inhandler(void);
extern interrupthandler usb1_ept4_inhandler(void);
extern interrupthandler usb1_ept5_inhandler(void);
extern interrupthandler usb1_ept6_inhandler(void);
extern interrupthandler usb1_ept7_inhandler(void);


extern interrupthandler usb1_ept0_outhandler(void);
extern interrupthandler usb1_ept1_outhandler(void);
extern interrupthandler usb1_ept2_outhandler(void);
extern interrupthandler usb1_ept3_outhandler(void);
extern interrupthandler usb1_ept4_outhandler(void);
extern interrupthandler usb1_ept5_outhandler(void);
extern interrupthandler usb1_ept6_outhandler(void);
extern interrupthandler usb1_ept7_outhandler(void);
  
extern interrupthandler SUShandler(void);
extern tVU32 se_inhandler(tU32  eptbuff,tPVU32 databuff,tU32  bcreg,tU32 csreg,tU32 count,tU8 stallbit,tU8 irbit,tU8 Lpktflg,tU8 mpkt);
extern tVU32 se_outhandler(tU32 eptbuff,tPVU32 databuff,tU32 bcreg,tU32 csreg,tU32 count,tU8 stallbit,tU8 irbit);



extern void usb1_clear_wakeup(void);
extern void USB1_Main_Handler(void);
extern void usb1_install_irq_handler(tU32 irqnum, interrupthandler handler);
extern void Wakeuphandler(void);
extern void USBDMAhandler(void);
extern void ENC_HANDLER(void);
extern void DMASetup(tU32 buffaddrs,tU32 memaddrs,tU32 count,tU8 control);

extern tU32 usb2_get_wakeup_src(void);

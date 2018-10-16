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
//#include "buffer.h"

//;MACBUFFERS	EQU  0x0408A000		;0x0400C000  	     ;base +32 Kb   ;bank2 start address - 
#define MACXmitBuffer	  MAC1XmitBuffer           //;MACBUFFERS			
#define MACRecvBuffer	  MAC1RecvBuffer           //;MACBUFFERS+0x4000

//MAC2XmitBuffer           =    Bank2start+0x18000	
//MAC2RecvBuffer           =    Bank2start+0x1D000


#define MAC1BASE	0x40500000	//0x40400000
#define MAC2BASE	0x40600000	//0x40460000
//;Control and status registers
#define CSR0BMR			(0x0) 
#define CSR1TPDR		(0x8)
#define CSR2RPDR		(0x10)
#define CSR3RDBAR		(0x18)
#define CSR4TDBAR		(0x20)
#define CSR5SR			(0x28)
#define CSR6OMR			(0x30)
#define CSR7IER			(0x38)
#define CSR8MFOCR		(0x40)
#define CSR9MIIR		(0x48)
#define CSR10MIISMR             (0x50)
#define CSR11GPTCR		(0x58)
#define CSR18PTCTR              (0x90)
#define CSR19FTR                (0x98)
#define CSR20FCR                (0xA0)

//;bit definations for bus mode register CSR0
#define CSR0SWR			(0x1<<0 )   		//;S/W reset
#define CSR0BAR			(0x1<<1	)	//;transmit and receive process have equal priority
#define CSR0DSLMASK		(0x1f<<2)		//;descriptor skip length mask
#define CSR0BIGEN		(0x1<<7	)	//;Big/little endian
#define CSR0PBL0		(0x0<<8	)	//;burst length=0
#define CSR0PBL1		(0x1<<8	)	//;burst length=1
#define CSR0PBL2		(0x2<<8	)	//;burst lenght=2
#define CSR0PBL4		(0x4<<8	)	//;burst length=4
#define CSR0PBL8		(0x8<<8	)	//;burst length=8
#define CSR0PBL16		(0x10<<8)		//;burst length=8
#define CSR0PBL32		(0x20<<8)		//;burst length=8
#define CSR0TAPDISABLED		(0x0<<17)		//;transmit automatic polling disabled
#define CSR0TAP819		(0x1<<17)		//;TAP 819us/81.9us
#define CSR0TAP245		(0x2<<17)		//;TAP=2.45ms/245us
#define CSR0TAP573		(0x3<<17)	//;TAP=5.73ms/573us
#define CSR0TAP512		(0x4<<17)	//;TAP=51.2us/5.12us
#define CSR0TAP1024		(0x5<<17)	//;TAP=102.4us/10.24 us
#define CSR0TAP1536		(0x6<<17)	//;TAP=153.6us/15.3us
#define CSR0TAP3584		(0x7<<17)	//;TAP=358.4us/35.84us	
#define CSR0DBO			(0x1<<20)	//;big endian mode used for data descriptors

//;bit definations for status register CSR5
#define CSR5TI			(0x1<<0	) 	//;Trasmit interrupt indicates frame done
#define CSR5TPS			(0x1<<1	)	//;Transmit process is in stopped state
#define CSR5TU			(0x1<<2	)	//;Trasmit bufferis unavaible ,owned by host not MAC
#define CSR5UNF			(0x1<<5	)	//;Transmit fifo underflow
#define CSR5RI			(0x1<<6	)	//;receive interrupt,recevice frame done
#define CSR5RU			(0x1<<7	)	//;receive buffer unavailable,owned by host not by MAC.
#define CSR5RPS			(0x1<<8	)	//;receive process stopped
#define CSR5ETI			(0x1<<10)		//;Early transmit interrupt
#define CSR5GTE			(0x1<<11)		//;general pupose timer expired
#define CSR5ERI			(0x1<<14)		//;Early receive interrupt
#define CSR5AIS			(0x1<<15)		//;Abnormal interrupt summary
#define CSR5NIS			(0x1<<16)		//;Normal interrupt summary
#define CSR5RS0			(0x0<<17)		//;stopped,reset,stop receive command issued
#define CSR5RS1			(0x1<<17)		//;running ,feteching receive descriptor
#define CSR5RS2			(0x2<<17)		//;running,waiting for end of receive pkt.
#define CSR5RS3			(0x3<<17)		//;running,waiting for receive pkt to be arrived
#define CSR5RS4			(0x4<<17)		//;suspended,unavailable receive buffer
#define CSR5RS5			(0x5<<17)		//;running,closing receive descriptor
#define CSR5RS6			(0x6<<17) 		//;not used
#define CSR5RS7			(0x7<<17)		//;running,transferring data from fifo to host mem'ry
#define CSR5RSMASK		(0x7<<17)
#define CSR5TS0			(0x0<<20)	//;stopped,reset,stop transmit command issued
#define CSR5TS1			(0x1<<20)		//;running ,feteching transmit descriptor
#define CSR5TS2			(0x2<<20)		//;running,waiting for end of transmission.
#define CSR5TS3			(0x3<<20)		//;running,trasferrring data from external mem to fifo
#define CSR5TS4			(0x4<<20)		//;reserved
#define CSR5TS5			(0x5<<20)		//;running,setup packet
#define CSR5TS6			(0x6<<20) 		//;suspended,fifo underflow or unavailable descriptor
#define CSR5TS7			(0x7<<20)		//;running,closing transmit descriptor
#define CSR5TSMASK		(0x7<<20)

//;operation mode register bit definations
#define CSR6HP			(0x1	) 	//;hash/perfect receive filtering mode
#define CSR6SR			(0x1<<1	)	//;start/stop receive command
#define CSR6HO			(0x1<<2	)	//;hash only filtering mode
#define CSR6PB			(0x1<<3	)	//;pass bad frames.
#define CSR6IF			(0x1<<4	)	//;Inverse filtering
#define CSR6PR			(0x1<<6	)	//;promiscuous mode
#define CSR6PM			(0x1<<7	)	//;pass all multicast
#define CSR6FD			(0x1<<9	)	//;full duplex mode
#define CSR6ST			(0x1<<13)		//;start/stop transmit command
#define CSR6TR64_128		(0x0<<14)		//;64bytes in 10mbs/128bytes in case of 100mbs
#define CSR6TR128_256		(0x1<<14)		//;128bytes in 10mbs/256bytes in case of 100mbs
#define CSR6TR128_512		(0x2<<14)		//;128bytes in 10mbs/512bytes in case of 100mbs
#define CSR6TR256_1024		(0x3<<14)		//;256bytes in 10mbs/1024bytes in case of 100mbs
#define CSR6SF			(0x1<<21)		//;store and forward,send only when full pkt is avaible in fifo
#define CSR6TTM			(0x1<<22)		//;transmit threshold mode
#define CSR6RA			(0x1<<30)		//;Receive all
#define CSR61000                (0x1<<16)         //@;1 Gbps
#define CSR6100                 (0x0<<16)         //@;100 Mbps
#define CSR610                  (0x2<<16)         //@;10 Mbps
#define CSR6LP                  (0x1<<10)		//Internal Loopback Mode

//;Interrupt Enable register
#define CSR7TIE			(0x1	) 	//;Transmit interrupt enable
#define CSR7TSE			(0x1<<1	)	//;Tx processes stopped interrupt enable
#define CSR7TUE			(0x1<<2	)	//;Tx buffer unavailable interrupt enable
#define CSR7UNE			(0x1<<5	)	//;Tx underflow interrupt enable
#define CSR7RIE			(0x1<<6	)	//;Rx interrupt enable
#define CSR7RUE			(0x1<<7	)	//;Rx buffer unavailable interrupt enable
#define CSR7RSE			(0x1<<8	)	//;Rx stopped interrupt enable
#define CSR7ETE			(0x1<<10)		//;early transmit interrupt enable
#define CSR7GTE			(0x1<<11)		//;general-purpose timer overflow interrupt enable
#define CSR7ERE			(0x1<<14)		//;early receive interrupt enable
#define CSR7AIE			(0x1<<15)		//;Abnormal interrupt summary enable
#define CSR7NIE			(0x1<<16)		//;Normal interrupt summary enable

//;Missed frame and overflow counter register
#define CSR8MFCMASK		(0xffff) 	//;missed frame counter
#define CSR8MFO			(0x1<<16)      	//;missed frame overflow
#define CSR8FOCMASK		(0x3ff<<17)	//;fifo overflow counter
#define CSR8OCO			(0x1<<28)      	//;overflow counter overflow

#define CSR9SCS			(0x1)      //;Serial Rom chip select
#define CSR9SCLK		(0x1<<1	)      //;Serial rom clock
#define CSR9SDI			(0x1<<2	)      //;Serial rom data input
#define CSR9SDO			(0x1<<3	)      //;serial rom data output
#define CSR9MDC			(0x1<<16)      	//;MII management clock
#define CSR9MDO			(0x1<<17)      	//;MII management write data
#define CSR9MII			(0x1<<18)      	//;MII management operations mode
#define CSR9MDI			(0x1<<19)      	//;MII management data in signal

//MII Serial Management Register
#define CSR10OPCLKDIS           (0x0<<26) 
#define CSR10OPREGWR            (0x1<<26)
#define CSR10OPREGRD            (0x2<<26)
#define CSR10OPCLKDIVSET        (0x3<<26)

#define CSR10CLKDIV8            (0x0<<28)
#define CSR10CLKDIV16           (0x1<<28)
#define CSR10CLKDIV32           (0x2<<28)
#define CSR10CLKDIV64           (0x3<<28)
#define CSR10CLKDIV128          (0x4<<28)
#define CSR10CLKDIV256          (0x5<<28)
#define CSR10CLKDIV512          (0x6<<28)
#define CSR10CLKDIV1024         (0x7<<28)

#define CSR10START              (0x1<<31)

//;general-purpose timer and interrupt mitigation control register
#define CSR11TIM		(0x1	) 	//;timer value
#define CSR11CON		(0x1<<16)		//;timer works in continous mode
#define CSR11NRP00		(0x0<<17)		//;disable receive interrupt mitigation
#define CSR11NRP01		(0x1<<17)		//;receive interrupt is generated after one frames
#define CSR11NRP02		(0x2<<17)		//;receive interrupt is generated after two frames
#define CSR11NRP03		(0x3<<17)		//;receive interrupt is generated after three frames
#define CSR11NRP04		(0x4<<17)		//;receive interrupt is generated after four frames
#define CSR11NRP05		(0x5<<17)		//;receive interrupt is generated after five frames
#define CSR11NRP06		(0x6<<17)		//;receive interrupt is generated after six frames
#define CSR11NRP07		(0x7<<17)		//;receive interrupt is generated after seven frames
#define CSR11RTMASK		(0xf<<20)		//;receive timer
#define CSR11NTP00		(0x0<<24)		//;disable transmit interrupt mitigation mechanism
#define CSR11NTP01		(0x1<<24)		//;transmit interrupt is generated after one frame
#define CSR11NTP02		(0x2<<24)		//;transmit interrupt is generated after two frames
#define CSR11NTP03		(0x3<<24)		//;transmit interrupt is generated after three frames
#define CSR11NTP04		(0x4<<24)		//;transmit interrupt is generated after four frames
#define CSR11NTP05		(0x5<<24)		//;transmit interrupt is generated after five frames
#define CSR11NTP06		(0x6<<24)		//;transmit interrupt is generated after six frames
#define CSR11NTP07		(0x7<<24)		//;transmit interrupt is generated after seven frames
#define CSR11TT			(0xf<<27)		//;transmit timer
#define CSR11CS			(0x1<<31)		//;mii 100mb mode 5.12us/10mb mode 51.2us else 81.92us/819.2us

//Bit definitions for CSR20 - Flow Control Register
#define CSR20FCRHTP             (1<<0 ) 
#define CSR20FCRPRS             (1<<1 )
#define CSR20FCRBPE             (1<<27)
#define CSR20FCRRPE             (1<<28)
#define CSR20FCRTPE             (1<<29)
#define CSR20FCRTUE             (1<<30)
#define CSR20FCRFCE             (1<<31)

//MII Serial Management Register (new implementation)

//;Receive descriptor word 0 bit definations
#define RDES0ZERO		(0x1<<0	) 	//;this bit is cleared for legal length frames
#define RDES0CE			(0x1<<1	)	//;when set indicates CRC error in receive pkt.
#define RDES0DB			(0x1<<2	)	//;dribbling bit,when set indicates that frame was not byte aligned.
#define RDES0RE			(0x1<<3	)	//;error reported from MII.
#define RDES0FT			(0x1<<5	)	//;when set,indicates frame length is greater than 1500bytes.
#define RDES0CS			(0x1<<6	)	//;collision detected
#define RDES0TL			(0x1<<7	)	//;frame too long
#define RDES0LS			(0x1<<8	)	//;when set indicates,this is last descriptor of a frame.
#define RDES0FS			(0x1<<9	)	//;when set,indicates this is first descriptor of a frame.
#define RDES0MF			(0x1<<10)		//;indicates that the frame has multicast address.
#define RDES0RF			(0x1<<11)		//;runt frame,frame is damaged by collision or premature-termination.
#define RDES0DE			(0x1<<14)		//;descriptor error
#define RDES0ES			(0x1<<15)		//;error summary(RDES0.1+RDES0.6+RDES0.7+RDES0.11+RDES0.14)
#define RDES0FLMASK		(0x3fff	)	//;frame length
#define RDES0FF			(0x1<<30)		//;Filtering fail,receive frame did not pass address recognition process
#define RDES0OWN		(0x1<<31)		//;Ownership bit

//;Receive descriptor word1 bit defination
#define RDES1RCH		(0x1<<24) 		//;second address chained.
#define RDES1RER		(0x1<<25)		//;when set,indicates that this is last descriptor in the RING descriptor.

//;Transmit descriptor word 0 bit definations..
#define TDES0DE			(0x1    ) 	//;when set indicates that the frame was deffered before transmison.
#define TDES0UF			(0x1<<1	)	//;transmit fifo underflow error.
#define TDES0CC			(0xf<<3	)	//;collision count
#define TDES0EC			(0x1<<8	)	//;excessive collisions,ie more than 16
#define TDES0LC			(0x1<<9	)	//;late collision,after tx'miting 64 bytes.
#define TDES0NC			(0x1<<10)		//;indicates that carrier was not asserted by external xver.
#define TDES0LO			(0x1<<11)		//;indicates loss of carrier during xmision.
#define TDES0ES			(0x1<<15)		//;Error summary(TDES0.1+TDES0.8+TDES0.9+TDES0.10+TDES0.11)
#define TDES0OWN		(0x1<<31)		//;ownership bit

//;Transmit descriptor woed1 bit definations
#define TDES1FTO		(0x1<<22) 		//;filtering type
#define TDES1DPD		(0x1<<23)		//;disable padding,when set automatic padding is disabled.
#define TDES1TCH		(0x1<<24)		//;second address is chained to descriptor not to data buffer.
#define TDES1TER		(0x1<<25)		//;transmit end of ring,last descriptor in descriptors ring.
#define TDES1AC			(0x1<<26)		//;Add CRC disabled.
#define TDES1SET		(0x1<<27)		//;setup packet.
#define TDES1FT1		(0x1<<28)		//;filtering type.
#define TDES1FS			(0x1<<29)		//;first descriptor
#define TDES1LS			(0x1<<30)		//;last descriptor
#define TDES1IC			(0x1<<31)		//;Interrupt on completion.

#define TD1			(MAC1WORKBASE+0x00) 		//0x0408A000
#define TD2			(MAC1WORKBASE+0x10)
#define TD3			(MAC1WORKBASE+0x20)
#define TD4			(MAC1WORKBASE+0x30)
#define TD5			(MAC1WORKBASE+0x40)
#define TD6			(MAC1WORKBASE+0x50)
#define TD7			(MAC1WORKBASE+0x60)
#define TD8			(MAC1WORKBASE+0x70)
#define TD9			(MAC1WORKBASE+0x80)
#define TD10			(MAC1WORKBASE+0x90)
#define TD11			(MAC1WORKBASE+0xa0)
#define TD12			(MAC1WORKBASE+0xb0)
#define TD13			(MAC1WORKBASE+0xc0)
#define TD14			(MAC1WORKBASE+0xd0)
#define TD15			(MAC1WORKBASE+0xe0)



#define TD31			(MAC2WORKBASE+0x00)		
#define TD32			(MAC2WORKBASE+0x10)
#define TD33			(MAC2WORKBASE+0x20)
#define TD34			(MAC2WORKBASE+0x30)
#define TD35			(MAC2WORKBASE+0x40)
#define TD36			(MAC2WORKBASE+0x50)
#define TD37			(MAC2WORKBASE+0x60)
#define TD38			(MAC2WORKBASE+0x70)
#define TD39			(MAC2WORKBASE+0x80)
#define TD40			(MAC2WORKBASE+0x90)
#define TD41			(MAC2WORKBASE+0xa0)
#define TD42			(MAC2WORKBASE+0xb0)
#define TD43			(MAC2WORKBASE+0xc0)
#define TD44			(MAC2WORKBASE+0xd0)
#define TD45			(MAC2WORKBASE+0xe0)



//#define PASSADDR	  MAC1WORKBASE+0x200
//#define FAILADDR	  MAC1WORKBASE+0x210
#define MAC1TIFLAG	        (MAC1WORKBASE+0x220)
#define MAC1RIFLAG		(MAC1WORKBASE+0x230)

#define MAC2TIFLAG		(MAC1WORKBASE+0x240) 
#define MAC2RIFLAG		(MAC1WORKBASE+0x250)


#define RD1			(MAC1WORKBASE+0x400)		
#define RD2			(MAC1WORKBASE+0x410)	
#define RD3			(MAC1WORKBASE+0x420)	
#define RD4			(MAC1WORKBASE+0x430)	
#define RD5			(MAC1WORKBASE+0x440)		
#define RD6			(MAC1WORKBASE+0x450)	
#define RD7			(MAC1WORKBASE+0x460)	
#define RD8			(MAC1WORKBASE+0x470)
#define RD9			(MAC1WORKBASE+0x480)	
#define RD10			(MAC1WORKBASE+0x490)	
#define RD11			(MAC1WORKBASE+0x4A0)


#define RD31			(MAC2WORKBASE+0x400)		
#define RD32			(MAC2WORKBASE+0x410)	
#define RD33			(MAC2WORKBASE+0x420)	
#define RD34			(MAC2WORKBASE+0x430)	
#define RD35			(MAC2WORKBASE+0x440)		
#define RD36			(MAC2WORKBASE+0x450)	
#define RD37			(MAC2WORKBASE+0x460)	
#define RD38			(MAC2WORKBASE+0x470)
#define RD39			(MAC2WORKBASE+0x480)	
#define RD40			(MAC2WORKBASE+0x490)	
#define RD41			(MAC2WORKBASE+0x4A0)


#define SETUPBUFF		(MACXmitBuffer	    )  
#define TB1Buff			(MACXmitBuffer+0x100)   
#define TB2Buff			(MACXmitBuffer+0x500)
#define TB3Buff			(MACXmitBuffer+0x900)   
#define TB4Buff			(MACXmitBuffer+0xD00)
#define TB5Buff			(MACXmitBuffer+0x1100)
#define TB6Buff			(MACXmitBuffer+0x1500)	
#define TB7Buff			(MACXmitBuffer+0x1900)
#define TB8Buff			(MACXmitBuffer+0x1d00)	

//MAC2
#define SETUPBUFFMAC2		(MAC2XmitBuffer	     ) 
#define TB21Buff		(MAC2XmitBuffer+0x100)  
#define TB22Buff		(MAC2XmitBuffer+0x500)
#define TB23Buff		(MAC2XmitBuffer+0x900)  
#define TB24Buff		(MAC2XmitBuffer+0xD00)
#define TB25Buff		(MAC2XmitBuffer+0x1100)
#define TB26Buff		(MAC2XmitBuffer+0x1500)	
#define TB27Buff		(MAC2XmitBuffer+0x1900)
#define TB28Buff		(MAC2XmitBuffer+0x1d00)

#define RB1Buff			(MACRecvBuffer	     ) 
#define RB2Buff			(MACRecvBuffer+0x400 )   
#define RB3Buff			(MACRecvBuffer+0x800 )   
#define RB4Buff			(MACRecvBuffer+0xC00 )   
#define RB5Buff			(MACRecvBuffer+0x1000)	
#define RB6Buff			(MACRecvBuffer+0x1400)	
#define RB7Buff			(MACRecvBuffer+0x1800)	
#define RB8Buff			(MACRecvBuffer+0x1C00)	


#define RB21Buff		(MAC2RecvBuffer	      ) 
#define RB22Buff		(MAC2RecvBuffer+0x400 )  
#define RB23Buff		(MAC2RecvBuffer+0x800 )  
#define RB24Buff		(MAC2RecvBuffer+0xC00 )  
#define RB25Buff		(MAC2RecvBuffer+0x1000)	
#define RB26Buff		(MAC2RecvBuffer+0x1400)	
#define RB27Buff		(MAC2RecvBuffer+0x1800)	
#define RB28Buff		(MAC2RecvBuffer+0x1C00)

#define MAC1RXCOUNT		(MACRecvBuffer)    
#define MAC1TXCOUNT		(MACXmitBuffer)
#define MAC2RXCOUNT		(MAC2RecvBuffer)
#define MAC2TXCOUNT		(MAC2XmitBuffer)


//	.end     

typedef struct macdptr{
          tVU32 Status;
	  tVU32 Control;
	  tVU32 Srcaddrs1;
	  tVU32 Srcaddrs2;		       
	   }macdptr;




  

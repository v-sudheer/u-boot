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

//	RTC Module
//Register Description 
	
#define RTC_CCVR 	 	(0x00) 	 	//Current Counter Value Register 
#define RTC_CMR 		(0x04)		// Counter Match Register 
#define RTC_CLR 		(0x08)		// Counter Load Register 
#define RTC_CCR			(0x0C)		//Counter Control Register 
#define RTC_STAT		(0x10)		// Interrupt Status Register 
#define RTC_INTEN		(0x14)		//interrupt enable

#define RTC_SEC_MIN             (0x00)
#define RTC_HOR_DOW             (0x04)
#define RTC_MON_YER_AB          (0x08)
#define RTC_CRC_CRD             (0x0c)
#define RTC_UPDATE_CCR          (0x18)
#define RTC_UPDATE_THLD         (0x1C)

#define RTC_RLR_BMC             (0x20)
#define RTC_DOMAO_BMC           (0x24)
#define RTC_MONAO_BMC           (0x28)
#define RTC_CENO_BMC            (0x2C)

#ifdef	ZERO
#define rtc_wen			(1<<3)  		//1 wrap enable,0 wrap disabled
#define rtc_en			(1<<2)		//1 counter enabled,0 =counter disabled
#define rtc_mask 		(1<<1)		//1 interrupt masked, 0 =interrupt unmasked
#define rtc_ien 		(1<<0)		//1= Interrups enable , 0 =disabled
#define rtc_Match_CLR		(1<<8)		//1 to clear Match INTR
#define rtc_Load_CLR		(1<<9)		//1 to clear Load INTR
#define rtc_Intrusion_CLR	(1<<10)		//1 to clear Intrusion INTR
#endif

#define rtcen		        (1<<0) 
#define wrapen		        (1<<1)		//default wrap to zero
#define wrap2load	        (1<<2)

#define MatchInten	        (1<<0)
#define LoadInten	        (1<<1)
#define IntrusionInten	        (1<<2)

/// Stdrtc_ctrl registers
#define swen            (1<<0) 
#define update_end      (1<<1)

///////////////////////////////////////////////////////

#define RTCCount		(RTCWorkBase)
#define RTCRXPTR		(RTCWorkBase+0x4)
#define RTCCRXPTR		(RTCWorkBase+0x8)
#define STDRTC_PERIODIC_CNT	(RTCWorkBase+0xc)
#define STDRTC_PERIODICINT_FLAG (RTCWorkBase+0x10)
#define STDRTC_ALARMINT_FLAG    (RTCWorkBase+0x14)
#define STDRTC_UPDATEINT_FLAG   (RTCWorkBase+0x18)
#define RTC_1SECINT_FLAG	(RTCWorkBase+0x1c)
#define RTC_MATCHINT_FLAG	(RTCWorkBase+0x20) 
#define RTC_INTRUSIONINT_FLAG	(RTCWorkBase+0x24)
#define GENRTC_INT_FLAG		(RTCWorkBase+0x28)
#define STDRTC_HOURINT_FLAG     (RTCWorkBase+0x2c)
#define STDRTC_MININT_FLAG      (RTCWorkBase+0x30)
#define RTCCURRVAL		RTCRecvBuffer
#define sec                     (RTCWorkBase+0x34)  
#define min                     (RTCWorkBase+0x38)
#define hor                     (RTCWorkBase+0x3c)
#define dow                     (RTCWorkBase+0x40)
#define dom                     (RTCWorkBase+0x44)
#define mon                     (RTCWorkBase+0x48)
#define yer                     (RTCWorkBase+0x4c)  
#define STDRTC_UEIFINT_FLAG     (RTCWorkBase+0x50)


  
typedef struct PII_RTC_REGS 
{
  volatile unsigned int rtc_ccvr;
  volatile unsigned int rtc_cmr;
  volatile unsigned int rtc_clr;
  volatile unsigned int rtc_ccr;
  volatile unsigned int rtc_stat;
  volatile unsigned int rtc_inten;

} PII_RTC_REGS;

#define  RTC_REGS	((volatile PII_RTC_REGS *)(RTCbase))

typedef struct PII_STDRTC_REGS 
{
  volatile unsigned char rtc_sec;
  volatile unsigned char rtc_seca;
  volatile unsigned char rtc_min;
  volatile unsigned char rtc_mina;
  volatile unsigned char rtc_hor;
  volatile unsigned char rtc_hora;
  volatile unsigned char rtc_dow;
  volatile unsigned char rtc_dom;
  volatile unsigned char rtc_mon;
  volatile unsigned char rtc_yer;
  volatile unsigned char rtc_cra;
  volatile unsigned char rtc_crb;
  volatile unsigned char rtc_crc;
  volatile unsigned char rtc_crd;
} PII_STDRTC_REGS;


  
extern void RTCHandler (void);
extern void RTCIntHandlerC (void);
extern void RTCEnableIntC (tU8, tU32);
extern void RTCSet_LoadnMatchC (tU32, tU32);
extern void RTCInitC (void);
extern void CONFIGnENRTCC (tU8);
extern void DisableRTCC (void);
extern void Combo13_IntHandler();


#define  STDRTC_REGS	((volatile PII_STDRTC_REGS *)(RTCSTDbase))
#define  EXTRTC_REGS	RTCEXTbase

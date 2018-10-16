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

#define PWMBASE		FanTachandPWM			//0x40433000
#define PWCFG		PWMBASE
#define FMCFG		PWMBASE+0x4
#define PWOCF		PWMBASE+0x8
#define FTISR		PWMBASE+0xC
#define PWCFG1		PWMBASE+0x100
#define FMCFG1		PWMBASE+0x104
#define PWOCF1		PWMBASE+0x108
#define FTISR1		PWMBASE+0x10C

#define PWCRCSR         PWMBASE+0x10
#define PWCRCSR1        PWMBASE+0x110
#define FMPFSR0         PWMBASE+0xC0
#define FMPFSR1         PWMBASE+0xC4
#define FMPFSR2         PWMBASE+0xC8
#define FMPFSR3         PWMBASE+0xCC
#define FMPFSR4         PWMBASE+0xD0
#define FMPFSR5         PWMBASE+0xD4
#define FMPFSR6         PWMBASE+0xD8
#define FMPFSR7         PWMBASE+0xDC
#define FMPFSR8         PWMBASE+0xE0
#define FMPFSR9         PWMBASE+0xE4
#define FMPFSR10        PWMBASE+0xE8
#define FMPFSR11        PWMBASE+0xEC
#define FMPFSR12        PWMBASE+0xF0
#define FMPFSR13        PWMBASE+0xF4
#define FMPFSR14        PWMBASE+0xF8
#define FMPFSR15        PWMBASE+0xFC

#define PWPSR0OFF		0x40
#define PWDCR0OFF		0x44
#define PWPSR1OFF		0x48
#define PWDCR1OFF		0x4C
#define PWPSR2OFF		0x50
#define PWDCR2OFF		0x54
#define PWPSR3OFF		0x58
#define PWDCR3OFF		0x5C
#define PWPSR4OFF		0x140
#define PWDCR4OFF		0x144
#define PWPSR5OFF		0x148
#define PWDCR5OFF		0x14C
#define PWPSR6OFF		0x150
#define PWDCR6OFF		0x154
#define PWPSR7OFF		0x158
#define PWDCR7OFF		0x15C


#define FMTHR0OFF			0x60
#define FMSPR0OFF			0x64
#define FMCSR0OFF			0x68

#define FMTHR1OFF			0x6C
#define FMSPR1OFF			0x70
#define FMCSR1OFF			0x74

#define FMTHR2OFF			0x78
#define FMSPR2OFF			0x7C
#define FMCSR2OFF			0x80

#define FMTHR3OFF			0x84
#define FMSPR3OFF			0x88
#define FMCSR3OFF			0x8C

#define FMTHR4OFF			0x90
#define FMSPR4OFF			0x94
#define FMCSR4OFF			0x98

#define FMTHR5OFF			0x9C
#define FMSPR5OFF			0xA0
#define FMCSR5OFF			0xA4

#define FMTHR6OFF			0xA8
#define FMSPR6OFF			0xAC
#define FMCSR6OFF			0xB0

#define FMTHR7OFF			0xB4
#define FMSPR7OFF			0xB8
#define FMCSR7OFF			0xBC

#define FMTHR8OFF			0x160
#define FMSPR8OFF			0x164
#define FMCSR8OFF			0x168

#define FMTHR9OFF			0x16C
#define FMSPR9OFF			0x170
#define FMCSR9OFF			0x174

#define FMTHR10OFF			0x178
#define FMSPR10OFF			0x17C
#define FMCSR10OFF			0x180

#define FMTHR11OFF			0x184
#define FMSPR11OFF			0x188
#define FMCSR11OFF			0x18C

#define FMTHR12OFF			0x190
#define FMSPR12OFF			0x194
#define FMCSR12OFF			0x198

#define FMTHR13OFF			0x19C
#define FMSPR13OFF			0x1A0
#define FMCSR13OFF			0x1A4

#define FMTHR14OFF			0x1A8
#define FMSPR14OFF			0x1AC
#define FMCSR14OFF			0x1B0

#define FMTHR15OFF			0x1B4
#define FMSPR15OFF			0x1B8
#define FMCSR15OFF			0x1BC



//PWM control config
#define PWMCTRLEN0			1
#define PWMINVEN0			1<<1
#define PWMCTRLEN1			1<<2
#define PWMINVEN1			1<<3
#define PWMCTRLEN2			1<<4
#define PWMINVEN2			1<<5
#define PWMCTRLEN3			1<<6
#define PWMINVEN3			1<<7
#define PWMCTRLEN4			1<<8
#define PWMINVEN4			1<<9
#define PWMCTRLEN5			1<<10
#define PWMINVEN5			1<<11
#define PWMCTRLEN6			1<<12
#define PWMINVEN6			1<<13
#define PWMCTRLEN7			1<<14
#define PWMINVEN7			1<<15


//Fantach Monitor
#define FTM0EN			1
#define FTM1EN			1<<1
#define FTM2EN			1<<2
#define FTM3EN			1<<3
#define FTM4EN			1<<4
#define FTM5EN			1<<5
#define FTM6EN			1<<6
#define FTM7EN			1<<7
#define FTM8EN			1<<8
#define FTM9EN			1<<9
#define FTM10EN			1<<10
#define FTM11EN			1<<11
#define FTM12EN			1<<12
#define FTM13EN			1<<13
#define FTM14EN			1<<14
#define FTM15EN			1<<15

//PWM OTS CONFIG REG
#define FAN0ENOTS		1
#define FAN1ENOTS		1<<1
#define FAN2ENOTS		1<<2
#define FAN3ENOTS		1<<3


#define PWCLKSEL200k		1<<7
#define FTCLK16k		0x0<<6
#define FTCLK8k			0x1<<6
#define FTCLK4k			0x2<<6
#define FTCLK2k			0x3<<6

#define FTDISABLE		1<<5
#define FTINTENABLE		1<<4
#define FTERR			1<<3
#define FTOVERFLW		1<<2
#define FTOVERTHR		1<<1
#define FTRDY			1


#define FT0			1
#define FT1			1<<1
#define FT2			1<<2
#define FT3			1<<3
#define FT4			1<<4
#define FT5			1<<5
#define FT6			1<<6
#define FT7			1<<7
#define FT8			1
#define FT9			1<<1
#define FT10			1<<2
#define FT11			1<<3
#define FT12			1<<4
#define FT13			1<<5
#define FT14			1<<6
#define FT15			1<<7

//PWM OTS CONFIG REG
#define FAN0ENOTS		1
#define FAN1ENOTS		1<<1
#define FAN2ENOTS		1<<2
#define FAN3ENOTS		1<<3


#define PWCLKSEL200k		1<<7
#define FTCLK16k		0x0<<6
#define FTCLK8k			0x1<<6
#define FTCLK4k			0x2<<6
#define FTCLK2k			0x3<<6

#define FTDISABLE		1<<5
#define FTINTENABLE		1<<4
#define FTERR			1<<3
#define FTOVERFLW		1<<2
#define FTOVERTHR		1<<1
#define FTRDY			1

#define FMPFSRFT0780            0x00
#define FMPFSRFT0530            0x01
#define FMPFSRFT0280            0x10
#define FMPFSRFT0155            0x11

#define FMPFSRFT1780            0x00 << 2
#define FMPFSRFT1530            0x01 << 2
#define FMPFSRFT1280            0x10 << 2
#define FMPFSRFT1155            0x11 << 2

#define FMPFSRFT2780            0x00 << 4
#define FMPFSRFT2530            0x01 << 4
#define FMPFSRFT2280            0x10 << 4
#define FMPFSRFT2155            0x11 << 4

#define FMPFSRFT3780            0x00 << 6
#define FMPFSRFT3530            0x01 << 6
#define FMPFSRFT3280            0x10 << 6
#define FMPFSRFT3155            0x11 << 6

#define FMPFSRFT4780            0x00
#define FMPFSRFT4530            0x01
#define FMPFSRFT4280            0x10
#define FMPFSRFT4155            0x11

#define FMPFSRFT5780            0x00 << 2
#define FMPFSRFT5530            0x01 << 2
#define FMPFSRFT5280            0x10 << 2
#define FMPFSRFT5155            0x11 << 2

#define FMPFSRFT6780            0x00 << 4
#define FMPFSRFT6530            0x01 << 4
#define FMPFSRFT6280            0x10 << 4
#define FMPFSRFT6155            0x11 << 4

#define FMPFSRFT7780            0x00 << 6
#define FMPFSRFT7530            0x01 << 6
#define FMPFSRFT7280            0x10 << 6
#define FMPFSRFT7155            0x11 << 6

#define PWCRVR0OFF              0x14
#define PWCRVR1OFF              0x18
#define PWCRVR2OFF              0x1C
#define PWCRVR3OFF              0x20
#define PWCRVR4OFF              0x114
#define PWCRVR5OFF              0x118
#define PWCRVR6OFF              0x11C
#define PWCRVR7OFF              0x120


#define PWM0			1
#define PWM1			1<<1
#define PWM2			1<<2
#define PWM3			1<<3
#define PWM4			1
#define PWM5			1<<1
#define PWM6			1<<2
#define PWM7			1<<3



#define FT0INTFLAG		FTWorkBase
#define FT1INTFLAG		FTWorkBase+0x4
#define FT2INTFLAG		FTWorkBase+0x8
#define FT3INTFLAG		FTWorkBase+0xC
#define FT4INTFLAG		FTWorkBase+0x10
#define FT5INTFLAG		FTWorkBase+0x14
#define FT6INTFLAG		FTWorkBase+0x18
#define FT7INTFLAG		FTWorkBase+0x1C
#define FT8INTFLAG		FTWorkBase+0x24
#define FT9INTFLAG		FTWorkBase+0x28
#define FT11INTFLAG		FTWorkBase+0x2C
#define FT12INTFLAG		FTWorkBase+0x30
#define FT13INTFLAG		FTWorkBase+0x34
#define FT14INTFLAG		FTWorkBase+0x38
#define FT15INTFLAG		FTWorkBase+0x3C

#define NUMPWM          8
#define NUMFT           16


/*******************************************************************
*                                                                  *
*  Description: FTIntHandler is called by int13_commonhandler      *
*               whenever a fantach related interrupt is generated. * 
*               The interrupt is generated when there is a reading *
*               overflow, over threshold or a fantach pulse is not *
*               read after 1 second.                               *
*                                                                  *
*               The FTIntHandler will print out an error message   *
*               and disable the interrupt.                         *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
void FTIntHandler(void);

/*******************************************************************
*                                                                  *
*  Description: int13_combinehandler is called when interrupt # 13 *
*               is generated. Interrupt # 13 is shared by a few    *
*               modules, i.e. fantach, PS2                         *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
//void int13_combinehandler(void);

/*******************************************************************
*                                                                  *
*  Description: SetPrescale allows user to set the prescale value  *
*               for one or multiple PWMs.                          *
*                                                                  *
*               Prescale value must not be set when PWM is enabled.*
*                                                                  *
*  Input Parameter: prescale_mask - specify the PWM to configure.  *
*                                   bit 0 = 1 - PWM0               *
*                                   bit 3 = 1 - PWM3               *
*                                   etc...                         *
*                   prescale[]    - holds the prescale value for   *
*                                   each PWM.                      *
*                                                                  *
*  Return value: 1 for success, 0 for fail to set                  *
*                                                                  *
*******************************************************************/
int SetPrescale(unsigned char prescale_mask, unsigned char prescale[]);

/*******************************************************************
*                                                                  *
*  Description: SetDutyCycle allows user to set the duty cycle     *
*               for one or multiple PWMs.                          *
*                                                                  *
*  Input Parameter: dutyCycle_mask - specify the PWM to configure. *
*                                    bit 0 = 1 - PWM0              *
*                                    bit 3 = 1 - PWM3              *
*                                    etc...                        *
*                   dutyCycle[]    - holds the duty cycle for      *
*                                    each PWM.                     *
*                                                                  *
*  Return value: NONE                                              *
*                                                                  *
*******************************************************************/
void SetDutyCycle(unsigned char dutyCycle_mask, unsigned char dutyCycle[]);

/*******************************************************************
*                                                                  *
*  Description: SetResolution allows user to set the resolution    *
*               for one or multiple PWMs.                          *
*                                                                  *
*  Input Parameter: resolution_mask - specify the PWM to configure.*
*                                     bit 0 = 1 - PWM0             *
*                                     bit 3 = 1 - PWM3             *
*                                     etc...                       *
*                   resolution[]    - holds the resolution for     *
*                                     each PWM.                    *
*                                                                  *
*  Return value: NONE                                              *
*                                                                  *
*******************************************************************/
void SetResolution(unsigned char resolution_mask, unsigned char resolution[]);

/*******************************************************************
*                                                                  *
*  Description: CntResolutionEn enables/disables the programmable  *
*               counter resolution for one or multiple PWMs.       *
*                                                                  *
*  Input Parameter: control - enable/disable programmable          *
*                             resolution counter.                  *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
void CntResolutionEn(unsigned char control);

/*******************************************************************
*                                                                  *
*  Description: InvertPWM allows the indiviual setting of PWM      *
*               waveform to be inverted.                           *
*                                                                  *
*  Input Parameter: invert - select the PWM to invert.             *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
void InvertPWM(unsigned invert);

/*******************************************************************
*                                                                  *
*  Description: The "Divided by 128/64" bit and PWDCR[7] controls  *
*               the divisor to be used. Pls refer to the spec for  *
*               detail.                                            *
*                                                                  *
*  Input Parameter: divisor                                        *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
void SetDivisor(unsigned divisor);
   
/*******************************************************************
*                                                                  *
*  Description: CheckCntResErr reports if a resolution setup error *
*               is detected for any of the PWMs.                   *
*               The error is generated when duty cycle is greater  *
*               than the resolution counter.                       *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: status - bit asserted when an error detected.     *
*                                                                  *
*******************************************************************/
unsigned char CheckCntResErr(void);

/*******************************************************************
*                                                                  *
*  Description: InitSetupPWM configures all PWM settings           *
*               Refer to other individual sub-functions            *
*                                                                  *
*  Input Parameter: prescale_mask, prescale[], dutyCycle_mask,     *
*                   dutyCycle[], resolution_mask, resolution[],    *
*                   control, divisor, invert                       *
*                                                                  *
*  Return value: 1 for success, 0 for fail to set                  *
*                                                                  *
*******************************************************************/
int InitSetupPWM(unsigned char prescale_mask, unsigned char prescale[], unsigned char dutyCycle_mask, unsigned char dutyCycle[], unsigned char resolution_mask, unsigned char resolution[], unsigned char control, unsigned char divisor, unsigned char invert);

/*******************************************************************
*                                                                  *
*  Description: EnablePWM starts the PWM                           *
*                                                                  *
*  Input Parameter: enable - bit mask for each PWM                 *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
void EnablePWM(unsigned enable);

/*******************************************************************
*                                                                  *
*  Description: PWMEnableOTS drives the PWM high (100% DC)         *
*               according to the bit mask.                         *
*                                                                  *
*  Input Parameter: control - bit mask for PWMs                    *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
void PWMEnableOTS(unsigned control);

/*******************************************************************
*                                                                  *
*  Description: SetFanTachThreshold allows user to set the         *
*               threshold value  for one or multiple fantachs.     *
*                                                                  *
*               Threshold value must not be set when fantach is    *
*               enabled.                                           *
*                                                                  *
*  Input Parameter: threshold_mask - specify the FT to configure.  *
*                                    bit 0 = 1 - FT0               *
*                                    bit 7 = 1 - FT7               *
*                                    etc...                        *
*                   threshold[]    - holds the threshold value for *
*                                    each FT.                      *
*                                                                  *
*  Return value: 1 for success, 0 for fail to set                  *
*                                                                  *
*******************************************************************/
int SetFanTachThreshold(unsigned char threshold_mask, unsigned char threshold[]);

/*******************************************************************
*                                                                  *
*  Description: SetFanTachCtrl allows user to set the control      *
*               register for one or multiple fantachs.             *
*               The control register allows setting of clock,      *
*               filter and interrupt.                              *
*                                                                  *
*               Clock and filter settings must not be set when     *
*               fantach is enabled.                                *
*                                                                  *
*  Input Parameter: control_mask - specify the FT to configure.    *
*                                  bit 0 = 1 - FT0                 *
*                                  bit 7 = 1 - FT7                 *
*                                  etc...                          *
*                   control[]    - holds the control setting for   *
*                                  each FT.                        *
*                                                                  *
*  Return value: 1 for success, 0 for fail to set                  *
*                                                                  *
*******************************************************************/
int SetFanTachCtrl(unsigned char control_mask, unsigned char control[]);
         
/*******************************************************************
*                                                                  *
*  Description: SetFanTachFilter0 sets the filter value for FT 0-3 *
*               Settings allow: 780us, 530us, 280us, 155us         *
*                                                                  *
*  Input Parameter: filter0 - filter settings for FT 0-3           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
void SetFanTachFilter0(unsigned char filter0);

/*******************************************************************
*                                                                  *
*  Description: SetFanTachFilter1 sets the filter value for FT 4-7 *
*               Settings allow: 780us, 530us, 280us, 155us         *
*                                                                  *
*  Input Parameter: filter1 - filter settings for FT 4-7           *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
void SetFanTachFilter1(unsigned char filter1);

/*******************************************************************
*                                                                  *
*  Description: EnableFanTach starts/stops getting fan reading from*
*               fan tach                                           *
*                                                                  *
*                                                                  *
*  Input Parameter: enable - bit mask for fan tachs                *
*                                                                  *
*  Return value: None                                              *
*                                                                  *
*******************************************************************/
void EnableFanTach(unsigned char enable);
   
/*******************************************************************
*                                                                  *
*  Description: InitFanTach configures all fan tach settings       *
*               Refer to other individual sub-functions            *
*                                                                  *
*  Input Parameter: threshold_mask, threshold[], control_mask,     *
*                   control[], filter0, filter1                    *
*                                                                  *
*  Return value: 1 for success, 0 for fail to set                  *
*                                                                  *
*******************************************************************/
int InitFanTach(unsigned char threshold_mask, unsigned char threshold[], unsigned char control_mask, unsigned char control[], unsigned char filter0, unsigned char filter1);

/*******************************************************************
*                                                                  *
*  Description: GetFanSpeed0 return the reading from the FanTach   *
*               Monitor Speed Register                             *
*               Reading could be invalid, check Speed Ready bit.   *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: speed reading from fan tach 0                     *
*                                                                  *
*******************************************************************/
int GetFanSpeed0(void);

/*******************************************************************
*                                                                  *
*  Description: Speed0Ready returns a 1 if there is a new valid    *
*               reading from fan tach 0                            *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: 1 if reading is valid, 0 if invalid               *
*                                                                  *
*******************************************************************/
int Speed0Ready(void);

/*******************************************************************
*                                                                  *
*  Description: GetFanSpeed1 return the reading from the FanTach   *
*               Monitor Speed Register                             *
*               Reading could be invalid, check Speed Ready bit.   *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: speed reading from fan tach 1                     *
*                                                                  *
*******************************************************************/
int GetFanSpeed1(void);

/*******************************************************************
*                                                                  *
*  Description: Speed1Ready returns a 1 if there is a new valid    *
*               reading from fan tach 1                            *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: 1 if reading is valid, 0 if invalid               *
*                                                                  *
*******************************************************************/
int Speed1Ready(void);

/*******************************************************************
*                                                                  *
*  Description: GetFanSpeed2 return the reading from the FanTach   *
*               Monitor Speed Register                             *
*               Reading could be invalid, check Speed Ready bit.   *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: speed reading from fan tach 2                     *
*                                                                  *
*******************************************************************/
int GetFanSpeed2(void);

/*******************************************************************
*                                                                  *
*  Description: Speed2Ready returns a 1 if there is a new valid    *
*               reading from fan tach 2                            *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: 1 if reading is valid, 0 if invalid               *
*                                                                  *
*******************************************************************/
int Speed2Ready(void);

/*******************************************************************
*                                                                  *
*  Description: GetFanSpeed3 return the reading from the FanTach   *
*               Monitor Speed Register                             *
*               Reading could be invalid, check Speed Ready bit.   *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: speed reading from fan tach 3                     *
*                                                                  *
*******************************************************************/

/******************************GetFanSpeed3*************************/
/* Get speed from fan tach 3                                       */
/* ARGUMENTS : NONE                                                */
/* RETURN VALUE: speed reading from fan tach 3                     */
/*******************************************************************/
int GetFanSpeed3(void);

/*******************************************************************
*                                                                  *
*  Description: Speed3Ready returns a 1 if there is a new valid    *
*               reading from fan tach 3                            *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: 1 if reading is valid, 0 if invalid               *
*                                                                  *
*******************************************************************/
int Speed3Ready(void);

/*******************************************************************
*                                                                  *
*  Description: GetFanSpeed4 return the reading from the FanTach   *
*               Monitor Speed Register                             *
*               Reading could be invalid, check Speed Ready bit.   *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: speed reading from fan tach 4                     *
*                                                                  *
*******************************************************************/
int GetFanSpeed4(void);

/*******************************************************************
*                                                                  *
*  Description: Speed4Ready returns a 1 if there is a new valid    *
*               reading from fan tach 4                            *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: 1 if reading is valid, 0 if invalid               *
*                                                                  *
*******************************************************************/
int Speed4Ready(void);

/*******************************************************************
*                                                                  *
*  Description: GetFanSpeed5 return the reading from the FanTach   *
*               Monitor Speed Register                             *
*               Reading could be invalid, check Speed Ready bit.   *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: speed reading from fan tach 5                     *
*                                                                  *
*******************************************************************/
int GetFanSpeed5(void);

/*******************************************************************
*                                                                  *
*  Description: Speed5Ready returns a 1 if there is a new valid    *
*               reading from fan tach 5                            *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: 1 if reading is valid, 0 if invalid               *
*                                                                  *
*******************************************************************/
int Speed5Ready(void);

/*******************************************************************
*                                                                  *
*  Description: GetFanSpeed6 return the reading from the FanTach   *
*               Monitor Speed Register                             *
*               Reading could be invalid, check Speed Ready bit.   *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: speed reading from fan tach 6                     *
*                                                                  *
*******************************************************************/
int GetFanSpeed6(void);

/*******************************************************************
*                                                                  *
*  Description: Speed6Ready returns a 1 if there is a new valid    *
*               reading from fan tach 6                            *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: 1 if reading is valid, 0 if invalid               *
*                                                                  *
*******************************************************************/
int Speed6Ready(void);

/*******************************************************************
*                                                                  *
*  Description: GetFanSpeed7 return the reading from the FanTach   *
*               Monitor Speed Register                             *
*               Reading could be invalid, check Speed Ready bit.   *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: speed reading from fan tach 7                     *
*                                                                  *
*******************************************************************/
int GetFanSpeed7(void);

/*******************************************************************
*                                                                  *
*  Description: Speed7Ready returns a 1 if there is a new valid    *
*               reading from fan tach 7                            *
*                                                                  *
*  Input Parameter: None                                           *
*                                                                  *
*  Return value: 1 if reading is valid, 0 if invalid               *
*                                                                  *
*******************************************************************/
int Speed7Ready(void);



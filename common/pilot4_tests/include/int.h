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
////	Interrupt Controller
#define Irq_inten		0x00
#define Irq_intmask		0x08
#define Irq_intforce		0x10
#define Irq_rawstatus		0X18
#define Irq_status		0x20
#define Irq_maskstatus		0x28
#define Irq_finalstatus		0x30
#define Irq_vector		0x38
#define Irq_vector_0		0x40
#define Irq_vector_1		0x48
#define Irq_vector_2		0x50
#define Irq_vector_3		0x58
#define Irq_vector_4		0x60
#define Irq_vector_5		0x68
#define Irq_vector_6		0x70
#define Irq_vector_7		0x78
#define Irq_vector_8		0x80
#define Irq_vector_9		0x88
#define Irq_vector_10		0x90
#define Irq_vector_11		0x98
#define Irq_vector_12		0xA0
#define Irq_vector_13		0xA8
#define Irq_vector_14		0xB0
#define Irq_vector_15		0xB8
#define Fiq_inten		0xC0
#define Fiq_intmask		0xC4
#define Fiq_intforce		0xC8
#define Fiq_rawstatus		0xCC
#define Fiq_status		0xD0
#define Fiq_finalstatus		0xD4
#define Irq_plevel		0xD8
#define Irq_plevel0		0xE8

#define ICTL_COMP_PARAMS_2		0x3f0
#define ICTL_COMP_PARAMS_1		0x3f4
#define ICTL_COMP_VERSION		0x3f8
#define ICTL_COMP_TYPE		0x3FC

#define IRQCTRL			0x38
#define FIQCTRL			0x3C
#define IRQSTS			0x40
#define FIQSTS			0x44

#define IRQCTRL2		0x60
#define FIQCTRL2		0x64
#define IRQSTS2			0x68  
#define FIQSTS2			0x6C
  


#define SSP_IRQCTRL1		0x70
#define SSP_FIQCTRL1		0x74
#define SSP_IRQSTS1		0x78
#define SSP_FIQSTS1		0x7C

//note that its ssp irqctl is not word alinged  
#define SSP_IRQCTRL2		0x62
#define SSP_FIQCTRL2		0x66
#define SSP_IRQSTS2		0x80 //bits are not one to one with arm(arm+<<9)
#define SSP_FIQSTS2		0x8C

  


  


#define MAC1INT			1
#define MAC2INT			1<<1
#define FGEEINT			1<<2
#define USBINT			1<<3


#define GPIOINT			1<<16
#define KCSCOMBOINT		1<<18
#define BMCWDTINT		1<<13   //@@@ 1<<11 Saurabh
#define PS2INT			1<<13
#define PWMINT			1<<13
#define SIOWDTINT		1<<11   //@@@ 1<<13 Saurabh
#define PSRINT			1<<11
#define STANDBYINT  	       1<<27





//VIC INTR NOS
#define MAC1INTNO		0
#define MAC2INTNO		1
#define FGEEINTNO		2
#define USBINTNO		3
#define I2C5INT			4
#define I2C6INT			5
#define Timer1INT		6
#define Timer2INT		7

//8,9   are reused for mac0.mac1,fgb,usb for compatibility wit FIQ
#define USB2INTNO               10  

#define SIOWDTINTNO		11
#define PSRINTNO		11
#define UART6INT            	11	//virtual uart1
#define UART7INT             	11	//virtual uart2

#define Timer3INT		12
#define Timer4INT		12
#define Timer5INT		12
#define Timer6INT		12

#define BMCWDTINTNO		13
#define RTCINT			13
#define PS2INTNO		13
#define PWMINTNO		13
#define ADCINTNO		13
#define DDRINT   		13
#define SPIINTNO		13
#define PECIINTNO		13
#define SDHCINTNO               13
#define NANDINTNO		13
#define AHB2PCIINTNO		13

#define HW_DEBUGINT             14
#define PCIE_FUNC2INT           14
#define UART3INT            	14
//define I2C1INT			15
#define I2C7INT			15
#define I2C8INT			15
#define GPIOINTNO		16
#define SWCINTNO		17
#define KCSCOMBOINTNO		18


#define UART4INT		20
#define UART5INT		21
#define ICMBUART		22

#define I2C1INTNO		23
#define I2C2INT			24
#define I2C3INT			25
#define I2C4INT			26
#define STANDBYINTNO            27
#define SECINTNO                28
#define BOMINTNO		29
#define S2AINTNO               30  
#define A2SINTNO               31
  
#define ICMBUARTBIT            1<<22

#define VECTSIOINTCOMB			12
#define VECTTIMERINTCOMB		13
#define VECTSYSINTCOMB			14
#define VECTUARTINTCOMB			15
#define VECTI2CINTCOMB			16

#define UART1INT            	4	//not used but needed to avoid compile errors
#define UART2INT             	5	//not used but needed to avoid compile errors

//SYSCONTROL IRQ ENABLE BITS..INTNO determones wether IRQCTL 1 or 2 is used
//some of the bits are using irqctl2 and status irqsts2 it would have been nice if we had
//defined them as STS2,CTL2   
#define SYSCTRL_MAC1INT			1
#define SYSCTRL_MAC2INT			1<<1
#define SYSCTRL_FGEEINT			1<<2
#define SYSCTRL_USBINT			1<<3
#define SYSCTRL_GPIOINT			1<<10
#define SYSCTRL_KCSCOMBOINT		1<<4
#define SYSCTRL_BMCWDTINT		1<<14
#define SYSCTRL_PS2INT			1<<16
#define SYSCTRL_SIOWDTINT		1<<5
#define SYSCTRL_PSRINT			1<<6
#define SYSCTRL_UART6INTBIT		1<<8
#define SYSCTRL_UART7INTBIT		1<<9
#define SYSCTRL_STANDBYINT  	       1<<27

#define SYSCTRL_Timer3INTBIT		1<<13
#define SYSCTRL_BMCWDTINTBIT            1<<14
#define SYSCTRL_RTCINTBIT		1<<15
#define SYSCTRL_PS2INTBIT		1<<16
#define SYSCTRL_PWMINTBIT		1<<17
#define SYSCTRL_ADCINTBIT		1<<18
#define SYSCTRL_DDRINTBIT		1<<19
#define SYSCTRL_SPIINTBIT		1<<20
#define SYSCTRL_SDHCBIT                 1<<5 
#define SYSCTRL_NANDBIT                 1<<6 
#define SYSCTRL_AHB2PCIBIT              (1<<7)

//// Timer 4, 5, 6, I2C 7, 8 bit location in IRQSTS2 Reg ////// 
#define IRQSTS2_TIMER4INT               1<<8
#define IRQSTS2_TIMER5INT               1<<9
#define IRQSTS2_TIMER6INT               1<<10
#define IRQSTS2_I2C7INT                 1<<11
#define IRQSTS2_I2C8INT                 1<<12
#define IRQSTS2_PCIE_FUNC2INT           1<<13
#define IRQSTS2_HW_DEBUGINT             1<<14

  
  

#define SYSCTRL_MAC1INTNO		0
#define SYSCTRL_MAC2INTNO		1
#define SYSCTRL_FGEEINTNO		2
#define SYSCTRL_USBDeviceINTNO		3
#define SYSCTRL_KCSCOMBOINTNO		4
#define SYSCTRL_SIOWDTINTNO		5
#define SYSCTRL_PSRINTNO		6
#define SYSCTRL_SWCINTNO		7
#define SYSCTRL_UART6INT            	8	//virtual uart1
#define SYSCTRL_UART7INT             	9	//virtual uart2
#define SYSCTRL_GPIOINTNO		10
#define SYSCTRL_Timer1INT		11
#define SYSCTRL_Timer2INT		12
#define SYSCTRL_Timer3INT		13
#define SYSCTRL_BMCWDTINTNO		14
#define SYSCTRL_RTCINT			15
#define SYSCTRL_PS2INTNO		16
#define SYSCTRL_PWMINTNO		17
#define SYSCTRL_PECIINTNO               18  
#define SYSCTRL_DDRINT			19
#define SYSCTRL_SPIINTNO		20
#define SYSCTRL_UART3INT		21
#define SYSCTRL_UART4INT		22
#define SYSCTRL_UART5INT		23
#define SYSCTRL_ICMBUART		24
#define SYSCTRL_I2C1INT			25
#define SYSCTRL_I2C2INT			26
#define SYSCTRL_I2C3INT			27
#define SYSCTRL_I2C4INT			28
#define SYSCTRL_I2C5INT			29
#define SYSCTRL_I2C6INT			30
#define SYSCTRL_UART1INT            	4	//not used but needed to avoid compile errors
#define SYSCTRL_UART2INT             	5	//not used but needed to avoid compile errors
#define SYSCTRL_STDBY			31
#define SYSCTRL_SEC_INTR                32
#define SYSCTRL_BOM_TO_ARM              33
#define SYSCTRL_A2S_INTR                34
#define SYSCTRL_S2A_INTR                35
#define SYSCTRL_USB2                    36  
#define SYSCTRL_SDHC                    37  
#define SYSCTRL_NAND                    38  
#define SYSCTRL_AHB2PCI                 39
#define SYSCTRL_Timer4INT               40
#define SYSCTRL_Timer5INT               41
#define SYSCTRL_Timer6INT               42
#define SYSCTRL_I2C7INT      		43
#define SYSCTRL_I2C8INT		        44
#define SYSCTRL_PCIE_FUNC2INT  		45
#define SYSCTRL_HW_DEBUGINT	        46

#define   INTRTYPE_IRQ             0
#define   INTRTYPE_FIQ             1  


////////////////////// FIQ INT NOs //////////////////
#define MAC1_FIQNO                      0
#define MAC2_FIQNO                      1
#define FGEE_FIQNO                      2
#define USB_FIQNO                       3  //usb1.1 fiq is not supported
#define KCSCOMBO_FIQNO                  4
#define SIOWDT_FIQNO                    5
#define PSR_FIQNO                       6
#define SWC_FIQNO                       7
#define UART6_FIQNO                     8
#define UART7_FIQNO                     9
#define GPIO_FIQNO                      10
#define TIMER0_FIQNO                    11
#define TIMER1_FIQNO                    12
#define TIMER2_FIQNO                    13
#define SYSWDT_FIQNO                    14
#define RTC_FIQNO                       15
#define PS2_FIQNO                       16
#define PWM_FIQNO                       17
#define PECI_FIQNO                      18
#define DDR_FIQNO                       19
#define SPI_FIQNO	        	20
#define UART3_FIQNO		        21
#define UART4_FIQNO		        22
#define UART5_FIQNO		        23
#define ICMBUART_FIQNO		        24
#define I2C1_FIQNO			25
#define I2C2_FIQNO			26
#define I2C3_FIQNO			27
#define I2C4_FIQNO			28
#define I2C5_FIQNO			29
#define I2C6_FIQNO			30
#define SEC_FIQNO                       32
#define BOM_TO_ARM_FIQNO                33
#define A2S_FIQNO                       34
#define S2A_FIQNO                       35
#define USB2_FIQNO                      36
#define TIMER4_FIQNO                    40
#define TIMER5_FIQNO                    41
#define TIMER6_FIQNO                    42
#define I2C7_FIQNO		        43
#define I2C8_FIQNO		       	44
#define PCIE_FUNC2_FIQNO                45
#define HW_DEBUG_FIQNO                  46



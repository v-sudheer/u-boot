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

#define GPIO0BASE		GPIO
#define GPIO1BASE		(GPIO+0x10) 
#define GPIO2BASE		(GPIO+0x20)
#define GPIO3BASE		(GPIO+0x30)
#define GPIO4BASE		(GPIO+0x40)
#define GPIO5BASE		(GPIO+0x50)
#define GPIO6BASE		(GPIO+0x60)
#define GPIO7BASE		(GPIO+0x70)
#define GPIO8BASE		(GPIO+0x80)
#define GPIO9BASE		(GPIO+0x90)
#define GPIO10BASE		(GPIO+0xA0)
#define GPIO11BASE		(GPIO+0xB0)
#define GPIO12BASE		(GPIO+0xC0)
#define GPIO13BASE		(GPIO+0xD0)
#define GPIO14BASE		(GPIO+0xE0)
#define GPIOGLB                 (GPIO+0xF0)
#define GPIOINTSTS0		(GPIO+0xF1)
#define GPIOINTSTS1		(GPIO+0xF2)
#define SCKHIGH                 (GPIO+0xF3)
#define SCKLOW                  (GPIO+0xF4)
#define GPIOINTSTS2		(GPIO+0xF5)
#define GPIOINTSTS3		(GPIO+0xF6)
#define GPPCC                   (GPIO+0xF7)


#define GPIO16BASE		(GPIO+0x100)
#define GPIO17BASE		(GPIO+0x110)
#define GPIO18BASE		(GPIO+0x120)
#define GPIO19BASE		(GPIO+0x130)
#define GPIO20BASE		(GPIO+0x140)
#define GPIO21BASE		(GPIO+0x150)
#define GPIO22BASE		(GPIO+0x160)
#define GPIO23BASE		(GPIO+0x170)
#define GPIO24BASE		(GPIO+0x180)
#define GPIO25BASE		(GPIO+0x190)
#define GPIO26BASE		(GPIO+0x1A0)
#define GPIO27BASE		(GPIO+0x1B0)
#define GPIO28BASE		(GPIO+0x1C0)
#define GPIO29BASE		(GPIO+0x1D0)

//Second Group SGPIO Defines. To be used for TB tests only.

#define GPIO_SEC_0BASE		(GPIO+0x200)
#define GPIO_SEC_1BASE		(GPIO+0x210)
#define GPIO_SEC_2BASE		(GPIO+0x220)
#define GPIO_SEC_3BASE		(GPIO+0x230)
#define GPIO_SEC_4BASE		(GPIO+0x240)
#define GPIO_SEC_5BASE		(GPIO+0x250)
#define GPIO_SEC_6BASE		(GPIO+0x260)
#define GPIO_SEC_7BASE		(GPIO+0x270)
#define GPIO_SEC_8BASE		(GPIO+0x280)
#define GPIO_SEC_9BASE		(GPIO+0x290)

//Second Group SGPIO Control Register Defines. To be used for TB tests only.

#define GPIOGLB_SEC                 (GPIO+0x2A0)
#define GPIOINTSTS0_SEC	            (GPIO+0x2A1)
#define GPIOINTSTS1_SEC		    (GPIO+0x2A2)
#define SCKHIGH_SEC                 (GPIO+0x2A3)
#define SCKLOW_SEC                  (GPIO+0x2A4)
#define GPPCC_SEC                   (GPIO+0x2A5)

//GPIO New GROUP For Pilot4. To be used for TB only.

#define GPIO15BASE		(GPIO+0x2F0)  //Change later to 0x200 in Design.

//	OFFSETS FOR GPIO REGISTERS FROM THE GPIOXBASE

#define GPCFG0			0X0
#define GPCFG1			0X1
#define GPCFG2			0X2
#define GPCFG3			0X3
#define GPCFG4			0X4
#define GPCFG5			0X5
#define GPCFG6			0X6
#define GPCFG7			0X7

#define GPDOUT			0x8
#define GPDIN			0x9
#define GPIMEN			0xA
#define GPEVENT			0xB
#define GPDBC0                  0xC
#define GPDBC1                  0xD


// GPIO PIN CONFIGURATION REGISTER BIT DEFINATIONS(GPCFG)
#define OUTPUTEN			(1<<0) 
#define PDCTRL_PUSHPULL			(1<<1)
#define PUCTRL_EN			(1<<2)
#define EVENT_TYPE			(1<<3)
#define EVENTPOL_RISING			(1<<4)
#define EVENT_EDGE_RISING               (2<<3)
#define EVENT_EDGE_FALLING              (0<<3)
#define EVENT_LEVEL_HIGH                (3<<3)
#define EVENT_LEVEL_LOW                 (1<<3)
#define EVENTDB_EN			(1<<5)
#define GPIOTOIRQ			(1<<6)
#define GPIOTOSMI			(1<<7)

// Serial GPIO specific config register bits
#define CASCADE_LTCH_BITS		(3<<1)
#define CASCADE_LTCH_EN			(1<<1)
#define CASCADE_LTCH_HIGH		(1<<2)
#define CASCADE_LTCH_LOW		(0<<2)

//GPIO INTERRUPT STATUS 0 BITS at 0xF0
#define GPIO0INT			(1<<0) 
#define GPIO1INT			(1<<1)
#define GPIO2INT			(1<<2)
#define GPIO3INT			(1<<3)
#define GPIO4INT			(1<<4)
#define GPIO5INT			(1<<5)
#define GPIO6INT			(1<<6)
#define GPIO7INT			(1<<7)

//GPIO INTERRUPT STATUS 1 BITS at 0xF1
#define GPIO8INT			(1<<0) 
#define GPIO9INT			(1<<1)
#define GPIO10INT			(1<<2)
#define GPIO11INT			(1<<3)
#define GPIO12INT			(1<<4)

//Interrupt Mask bits
#define IMEN0			(1<<0) 
#define IMEN1			(1<<1)
#define IMEN2			(1<<2)
#define IMEN3			(1<<3)
#define IMEN4			(1<<4)
#define IMEN5			(1<<5)
#define IMEN6			(1<<6)
#define IMEN7			(1<<7)

//EVENT Status bits
#define EVENT0			(1<<0) 
#define EVENT1			(1<<1)
#define EVENT2			(1<<2)
#define EVENT3			(1<<3)
#define EVENT4			(1<<4)
#define EVENT5			(1<<5)
#define EVENT6			(1<<6)
#define EVENT7			(1<<7)

// Debounce control register 0
#define clksel0                 (1<<0) 
#define clksel1                 (1<<2)
#define clksel2                 (1<<4)
#define clksel3                 (1<<6)

// Debounce control register 1
#define clksel4                 (1<<0)
#define clksel5                 (1<<2)
#define clksel6                 (1<<4)
#define clksel7                 (1<<6)

// Serial interface control register
#define ser_inv_sgld            (1<<7)
#define ser_len_bits            (7<<4)
#define ser_len_80              (0<<4)
#define ser_len_16              (1<<4)
#define ser_len_32              (2<<4)
#define ser_len_48              (3<<4)
#define ser_len_64              (4<<4)
#define ser_di_re_ctl           (1<<3)
#define ser_di_fe_ctl           (0<<3)
#define gpdo_swap		(1<<2)  
#define serial_mode_en		(1<<1)


// GPPCC - port capture control register
#define pcap_on_sgpio_en        (1<<0)
#define pcap_on_sgpio4          (0<<1)
#define pcap_on_sgpio5          (1<<1)
#define inv_pcap_data_on_sgpio  (0<<2)
#define noinv_pcap_data_on_sgpio  (1<<2)

// Interrrupt flag for each group
#define GPIOINTCOUNT		(GPIOWorkBase)
#define GPIOINTFLAG		(GPIOWorkBase + 0x4)

#define GPIOGRP0INTFLAG         (GPIOWorkBase + 0x10)
#define GPIOGRP0INTFLAG0        (GPIOWorkBase + 0x11)
#define GPIOGRP0INTFLAG1        (GPIOWorkBase + 0x12)
#define GPIOGRP0INTFLAG2        (GPIOWorkBase + 0x13)
#define GPIOGRP0INTFLAG3        (GPIOWorkBase + 0x14)
#define GPIOGRP0INTFLAG4        (GPIOWorkBase + 0x15)
#define GPIOGRP0INTFLAG5        (GPIOWorkBase + 0x16)
#define GPIOGRP0INTFLAG6        (GPIOWorkBase + 0x17)
#define GPIOGRP0INTFLAG7        (GPIOWorkBase + 0x18)

#define GPIOGRP1INTFLAG         (GPIOWorkBase + 0x20)
#define GPIOGRP1INTFLAG0        (GPIOWorkBase + 0x21)
#define GPIOGRP1INTFLAG1        (GPIOWorkBase + 0x22)
#define GPIOGRP1INTFLAG2        (GPIOWorkBase + 0x23)
#define GPIOGRP1INTFLAG3        (GPIOWorkBase + 0x24)
#define GPIOGRP1INTFLAG4        (GPIOWorkBase + 0x25)
#define GPIOGRP1INTFLAG5        (GPIOWorkBase + 0x26)
#define GPIOGRP1INTFLAG6        (GPIOWorkBase + 0x27)
#define GPIOGRP1INTFLAG7        (GPIOWorkBase + 0x28)

#define GPIOGRP2INTFLAG         (GPIOWorkBase + 0x30)
#define GPIOGRP2INTFLAG0        (GPIOWorkBase + 0x31)
#define GPIOGRP2INTFLAG1        (GPIOWorkBase + 0x32)
#define GPIOGRP2INTFLAG2        (GPIOWorkBase + 0x33)
#define GPIOGRP2INTFLAG3        (GPIOWorkBase + 0x34)
#define GPIOGRP2INTFLAG4        (GPIOWorkBase + 0x35)
#define GPIOGRP2INTFLAG5        (GPIOWorkBase + 0x36)
#define GPIOGRP2INTFLAG6        (GPIOWorkBase + 0x37)
#define GPIOGRP2INTFLAG7        (GPIOWorkBase + 0x38)

#define GPIOGRP3INTFLAG         (GPIOWorkBase + 0x40)
#define GPIOGRP3INTFLAG0        (GPIOWorkBase + 0x41)
#define GPIOGRP3INTFLAG1        (GPIOWorkBase + 0x42)
#define GPIOGRP3INTFLAG2        (GPIOWorkBase + 0x43)
#define GPIOGRP3INTFLAG3        (GPIOWorkBase + 0x44)
#define GPIOGRP3INTFLAG4        (GPIOWorkBase + 0x45)
#define GPIOGRP3INTFLAG5        (GPIOWorkBase + 0x46)
#define GPIOGRP3INTFLAG6        (GPIOWorkBase + 0x47)
#define GPIOGRP3INTFLAG7        (GPIOWorkBase + 0x48)

#define GPIOGRP4INTFLAG         (GPIOWorkBase + 0x50)
#define GPIOGRP4INTFLAG0        (GPIOWorkBase + 0x51)
#define GPIOGRP4INTFLAG1        (GPIOWorkBase + 0x52)
#define GPIOGRP4INTFLAG2        (GPIOWorkBase + 0x53)
#define GPIOGRP4INTFLAG3        (GPIOWorkBase + 0x54)
#define GPIOGRP4INTFLAG4        (GPIOWorkBase + 0x55)
#define GPIOGRP4INTFLAG5        (GPIOWorkBase + 0x56)
#define GPIOGRP4INTFLAG6        (GPIOWorkBase + 0x57)
#define GPIOGRP4INTFLAG7        (GPIOWorkBase + 0x58)

#define GPIOGRP5INTFLAG         (GPIOWorkBase + 0x60)
#define GPIOGRP5INTFLAG0        (GPIOWorkBase + 0x61)
#define GPIOGRP5INTFLAG1        (GPIOWorkBase + 0x62)
#define GPIOGRP5INTFLAG2        (GPIOWorkBase + 0x63)
#define GPIOGRP5INTFLAG3        (GPIOWorkBase + 0x64)
#define GPIOGRP5INTFLAG4        (GPIOWorkBase + 0x65)
#define GPIOGRP5INTFLAG5        (GPIOWorkBase + 0x66)
#define GPIOGRP5INTFLAG6        (GPIOWorkBase + 0x67)
#define GPIOGRP5INTFLAG7        (GPIOWorkBase + 0x68)

#define GPIOGRP6INTFLAG         (GPIOWorkBase + 0x70)
#define GPIOGRP6INTFLAG0        (GPIOWorkBase + 0x71)
#define GPIOGRP6INTFLAG1        (GPIOWorkBase + 0x72)
#define GPIOGRP6INTFLAG2        (GPIOWorkBase + 0x73)
#define GPIOGRP6INTFLAG3        (GPIOWorkBase + 0x74)
#define GPIOGRP6INTFLAG4        (GPIOWorkBase + 0x75)
#define GPIOGRP6INTFLAG5        (GPIOWorkBase + 0x76)
#define GPIOGRP6INTFLAG6        (GPIOWorkBase + 0x77)
#define GPIOGRP6INTFLAG7        (GPIOWorkBase + 0x78)

#define GPIOGRP7INTFLAG         (GPIOWorkBase + 0x80)
#define GPIOGRP7INTFLAG0        (GPIOWorkBase + 0x81)
#define GPIOGRP7INTFLAG1        (GPIOWorkBase + 0x82)
#define GPIOGRP7INTFLAG2        (GPIOWorkBase + 0x83)
#define GPIOGRP7INTFLAG3        (GPIOWorkBase + 0x84)
#define GPIOGRP7INTFLAG4        (GPIOWorkBase + 0x85)
#define GPIOGRP7INTFLAG5        (GPIOWorkBase + 0x86)
#define GPIOGRP7INTFLAG6        (GPIOWorkBase + 0x87)
#define GPIOGRP7INTFLAG7        (GPIOWorkBase + 0x88)

#define GPIOGRP8INTFLAG         (GPIOWorkBase + 0x90)
#define GPIOGRP8INTFLAG0        (GPIOWorkBase + 0x91)
#define GPIOGRP8INTFLAG1        (GPIOWorkBase + 0x92)
#define GPIOGRP8INTFLAG2        (GPIOWorkBase + 0x93)
#define GPIOGRP8INTFLAG3        (GPIOWorkBase + 0x94)
#define GPIOGRP8INTFLAG4        (GPIOWorkBase + 0x95)
#define GPIOGRP8INTFLAG5        (GPIOWorkBase + 0x96)
#define GPIOGRP8INTFLAG6        (GPIOWorkBase + 0x97)
#define GPIOGRP8INTFLAG7        (GPIOWorkBase + 0x98)

#define GPIOGRP9INTFLAG         (GPIOWorkBase + 0xA0)
#define GPIOGRP9INTFLAG0        (GPIOWorkBase + 0xA1)
#define GPIOGRP9INTFLAG1        (GPIOWorkBase + 0xA2)
#define GPIOGRP9INTFLAG2        (GPIOWorkBase + 0xA3)
#define GPIOGRP9INTFLAG3        (GPIOWorkBase + 0xA4)
#define GPIOGRP9INTFLAG4        (GPIOWorkBase + 0xA5)
#define GPIOGRP9INTFLAG5        (GPIOWorkBase + 0xA6)
#define GPIOGRP9INTFLAG6        (GPIOWorkBase + 0xA7)
#define GPIOGRP9INTFLAG7        (GPIOWorkBase + 0xA8)

#define GPIOGRP10INTFLAG        (GPIOWorkBase + 0xB0)
#define GPIOGRP10INTFLAG0       (GPIOWorkBase + 0xB1)
#define GPIOGRP10INTFLAG1       (GPIOWorkBase + 0xB2)
#define GPIOGRP10INTFLAG2       (GPIOWorkBase + 0xB3)
#define GPIOGRP10INTFLAG3       (GPIOWorkBase + 0xB4)
#define GPIOGRP10INTFLAG4       (GPIOWorkBase + 0xB5)
#define GPIOGRP10INTFLAG5       (GPIOWorkBase + 0xB6)
#define GPIOGRP10INTFLAG6       (GPIOWorkBase + 0xB7)
#define GPIOGRP10INTFLAG7       (GPIOWorkBase + 0xB8)

#define GPIOGRP11INTFLAG        (GPIOWorkBase + 0xC0)
#define GPIOGRP11INTFLAG0       (GPIOWorkBase + 0xC1)
#define GPIOGRP11INTFLAG1       (GPIOWorkBase + 0xC2)
#define GPIOGRP11INTFLAG2       (GPIOWorkBase + 0xC3)
#define GPIOGRP11INTFLAG3       (GPIOWorkBase + 0xC4)
#define GPIOGRP11INTFLAG4       (GPIOWorkBase + 0xC5)
#define GPIOGRP11INTFLAG5       (GPIOWorkBase + 0xC6)
#define GPIOGRP11INTFLAG6       (GPIOWorkBase + 0xC7)
#define GPIOGRP11INTFLAG7       (GPIOWorkBase + 0xC8)

#define GPIOGRP12INTFLAG        (GPIOWorkBase + 0xD0)
#define GPIOGRP12INTFLAG0       (GPIOWorkBase + 0xD1)
#define GPIOGRP12INTFLAG1       (GPIOWorkBase + 0xD2)
#define GPIOGRP12INTFLAG2       (GPIOWorkBase + 0xD3)
#define GPIOGRP12INTFLAG3       (GPIOWorkBase + 0xD4)
#define GPIOGRP12INTFLAG4       (GPIOWorkBase + 0xD5)
#define GPIOGRP12INTFLAG5       (GPIOWorkBase + 0xD6)
#define GPIOGRP12INTFLAG6       (GPIOWorkBase + 0xD7)
#define GPIOGRP12INTFLAG7       (GPIOWorkBase + 0xD8)

#define GPIOGRP13INTFLAG        (GPIOWorkBase + 0xE0)
#define GPIOGRP13INTFLAG0       (GPIOWorkBase + 0xE1)
#define GPIOGRP13INTFLAG1       (GPIOWorkBase + 0xE2)
#define GPIOGRP13INTFLAG2       (GPIOWorkBase + 0xE3)
#define GPIOGRP13INTFLAG3       (GPIOWorkBase + 0xE4)
#define GPIOGRP13INTFLAG4       (GPIOWorkBase + 0xE5)
#define GPIOGRP13INTFLAG5       (GPIOWorkBase + 0xE6)
#define GPIOGRP13INTFLAG6       (GPIOWorkBase + 0xE7)
#define GPIOGRP13INTFLAG7       (GPIOWorkBase + 0xE8)

#define GPIOGRP14INTFLAG        (GPIOWorkBase + 0xF0)
#define GPIOGRP14INTFLAG0       (GPIOWorkBase + 0xF1)
#define GPIOGRP14INTFLAG1       (GPIOWorkBase + 0xF2)
#define GPIOGRP14INTFLAG2       (GPIOWorkBase + 0xF3)
#define GPIOGRP14INTFLAG3       (GPIOWorkBase + 0xF4)
#define GPIOGRP14INTFLAG4       (GPIOWorkBase + 0xF5)
#define GPIOGRP14INTFLAG5       (GPIOWorkBase + 0xF6)
#define GPIOGRP14INTFLAG6       (GPIOWorkBase + 0xF7)
#define GPIOGRP14INTFLAG7       (GPIOWorkBase + 0xF8)

#define GPIOGRP16INTFLAG        (GPIOWorkBase + 0x100)
#define GPIOGRP16INTFLAG0       (GPIOWorkBase + 0x101)
#define GPIOGRP16INTFLAG1       (GPIOWorkBase + 0x102)
#define GPIOGRP16INTFLAG2       (GPIOWorkBase + 0x103)
#define GPIOGRP16INTFLAG3       (GPIOWorkBase + 0x104)
#define GPIOGRP16INTFLAG4       (GPIOWorkBase + 0x105)
#define GPIOGRP16INTFLAG5       (GPIOWorkBase + 0x106)
#define GPIOGRP16INTFLAG6       (GPIOWorkBase + 0x107)
#define GPIOGRP16INTFLAG7       (GPIOWorkBase + 0x108)

#define GPIOGRP17INTFLAG        (GPIOWorkBase + 0x110)
#define GPIOGRP17INTFLAG0       (GPIOWorkBase + 0x111)
#define GPIOGRP17INTFLAG1       (GPIOWorkBase + 0x112)
#define GPIOGRP17INTFLAG2       (GPIOWorkBase + 0x113)
#define GPIOGRP17INTFLAG3       (GPIOWorkBase + 0x114)
#define GPIOGRP17INTFLAG4       (GPIOWorkBase + 0x115)
#define GPIOGRP17INTFLAG5       (GPIOWorkBase + 0x116)
#define GPIOGRP17INTFLAG6       (GPIOWorkBase + 0x117)
#define GPIOGRP17INTFLAG7       (GPIOWorkBase + 0x118)

#define GPIOGRP18INTFLAG        (GPIOWorkBase + 0x120)
#define GPIOGRP18INTFLAG0       (GPIOWorkBase + 0x121)
#define GPIOGRP18INTFLAG1       (GPIOWorkBase + 0x122)
#define GPIOGRP18INTFLAG2       (GPIOWorkBase + 0x123)
#define GPIOGRP18INTFLAG3       (GPIOWorkBase + 0x124)
#define GPIOGRP18INTFLAG4       (GPIOWorkBase + 0x125)
#define GPIOGRP18INTFLAG5       (GPIOWorkBase + 0x126)
#define GPIOGRP18INTFLAG6       (GPIOWorkBase + 0x127)
#define GPIOGRP18INTFLAG7       (GPIOWorkBase + 0x128)

#define GPIOGRP19INTFLAG        (GPIOWorkBase + 0x130)
#define GPIOGRP19INTFLAG0       (GPIOWorkBase + 0x131)
#define GPIOGRP19INTFLAG1       (GPIOWorkBase + 0x132)
#define GPIOGRP19INTFLAG2       (GPIOWorkBase + 0x133)
#define GPIOGRP19INTFLAG3       (GPIOWorkBase + 0x134)
#define GPIOGRP19INTFLAG4       (GPIOWorkBase + 0x135)
#define GPIOGRP19INTFLAG5       (GPIOWorkBase + 0x136)
#define GPIOGRP19INTFLAG6       (GPIOWorkBase + 0x137)
#define GPIOGRP19INTFLAG7       (GPIOWorkBase + 0x138)

#define GPIOGRP20INTFLAG        (GPIOWorkBase + 0x140)
#define GPIOGRP20INTFLAG0       (GPIOWorkBase + 0x141)
#define GPIOGRP20INTFLAG1       (GPIOWorkBase + 0x142)
#define GPIOGRP20INTFLAG2       (GPIOWorkBase + 0x143)
#define GPIOGRP20INTFLAG3       (GPIOWorkBase + 0x144)
#define GPIOGRP20INTFLAG4       (GPIOWorkBase + 0x145)
#define GPIOGRP20INTFLAG5       (GPIOWorkBase + 0x146)
#define GPIOGRP20INTFLAG6       (GPIOWorkBase + 0x147)
#define GPIOGRP20INTFLAG7       (GPIOWorkBase + 0x148)

#define GPIOGRP21INTFLAG        (GPIOWorkBase + 0x150)
#define GPIOGRP21INTFLAG0       (GPIOWorkBase + 0x151)
#define GPIOGRP21INTFLAG1       (GPIOWorkBase + 0x152)
#define GPIOGRP21INTFLAG2       (GPIOWorkBase + 0x153)
#define GPIOGRP21INTFLAG3       (GPIOWorkBase + 0x154)
#define GPIOGRP21INTFLAG4       (GPIOWorkBase + 0x155)
#define GPIOGRP21INTFLAG5       (GPIOWorkBase + 0x156)
#define GPIOGRP21INTFLAG6       (GPIOWorkBase + 0x157)
#define GPIOGRP21INTFLAG7       (GPIOWorkBase + 0x158)

#define GPIOGRP22INTFLAG        (GPIOWorkBase + 0x160)
#define GPIOGRP22INTFLAG0       (GPIOWorkBase + 0x161)
#define GPIOGRP22INTFLAG1       (GPIOWorkBase + 0x162)
#define GPIOGRP22INTFLAG2       (GPIOWorkBase + 0x163)
#define GPIOGRP22INTFLAG3       (GPIOWorkBase + 0x164)
#define GPIOGRP22INTFLAG4       (GPIOWorkBase + 0x165)
#define GPIOGRP22INTFLAG5       (GPIOWorkBase + 0x166)
#define GPIOGRP22INTFLAG6       (GPIOWorkBase + 0x167)
#define GPIOGRP22INTFLAG7       (GPIOWorkBase + 0x168)

#define GPIOGRP23INTFLAG        (GPIOWorkBase + 0x170)
#define GPIOGRP23INTFLAG0       (GPIOWorkBase + 0x171)
#define GPIOGRP23INTFLAG1       (GPIOWorkBase + 0x172)
#define GPIOGRP23INTFLAG2       (GPIOWorkBase + 0x173)
#define GPIOGRP23INTFLAG3       (GPIOWorkBase + 0x174)
#define GPIOGRP23INTFLAG4       (GPIOWorkBase + 0x175)
#define GPIOGRP23INTFLAG5       (GPIOWorkBase + 0x176)
#define GPIOGRP23INTFLAG6       (GPIOWorkBase + 0x177)
#define GPIOGRP23INTFLAG7       (GPIOWorkBase + 0x178)

#define GPIOGRP24INTFLAG        (GPIOWorkBase + 0x180)
#define GPIOGRP24INTFLAG0       (GPIOWorkBase + 0x181)
#define GPIOGRP24INTFLAG1       (GPIOWorkBase + 0x182)
#define GPIOGRP24INTFLAG2       (GPIOWorkBase + 0x183)
#define GPIOGRP24INTFLAG3       (GPIOWorkBase + 0x184)
#define GPIOGRP24INTFLAG4       (GPIOWorkBase + 0x185)
#define GPIOGRP24INTFLAG5       (GPIOWorkBase + 0x186)
#define GPIOGRP24INTFLAG6       (GPIOWorkBase + 0x187)
#define GPIOGRP24INTFLAG7       (GPIOWorkBase + 0x188)

#define GPIOGRP25INTFLAG        (GPIOWorkBase + 0x190)
#define GPIOGRP25INTFLAG0       (GPIOWorkBase + 0x191)
#define GPIOGRP25INTFLAG1       (GPIOWorkBase + 0x192)
#define GPIOGRP25INTFLAG2       (GPIOWorkBase + 0x193)
#define GPIOGRP25INTFLAG3       (GPIOWorkBase + 0x194)
#define GPIOGRP25INTFLAG4       (GPIOWorkBase + 0x195)
#define GPIOGRP25INTFLAG5       (GPIOWorkBase + 0x196)
#define GPIOGRP25INTFLAG6       (GPIOWorkBase + 0x197)
#define GPIOGRP25INTFLAG7       (GPIOWorkBase + 0x198)

#define GPIOGRP26INTFLAG        (GPIOWorkBase + 0x1A0)
#define GPIOGRP26INTFLAG0       (GPIOWorkBase + 0x1A1)
#define GPIOGRP26INTFLAG1       (GPIOWorkBase + 0x1A2)
#define GPIOGRP26INTFLAG2       (GPIOWorkBase + 0x1A3)
#define GPIOGRP26INTFLAG3       (GPIOWorkBase + 0x1A4)
#define GPIOGRP26INTFLAG4       (GPIOWorkBase + 0x1A5)
#define GPIOGRP26INTFLAG5       (GPIOWorkBase + 0x1A6)
#define GPIOGRP26INTFLAG6       (GPIOWorkBase + 0x1A7)
#define GPIOGRP26INTFLAG7       (GPIOWorkBase + 0x1A8)

#define GPIOGRP27INTFLAG        (GPIOWorkBase + 0x1B0)
#define GPIOGRP27INTFLAG0       (GPIOWorkBase + 0x1B1)
#define GPIOGRP27INTFLAG1       (GPIOWorkBase + 0x1B2)
#define GPIOGRP27INTFLAG2       (GPIOWorkBase + 0x1B3)
#define GPIOGRP27INTFLAG3       (GPIOWorkBase + 0x1B4)
#define GPIOGRP27INTFLAG4       (GPIOWorkBase + 0x1B5)
#define GPIOGRP27INTFLAG5       (GPIOWorkBase + 0x1B6)
#define GPIOGRP27INTFLAG6       (GPIOWorkBase + 0x1B7)
#define GPIOGRP27INTFLAG7       (GPIOWorkBase + 0x1B8)

#define GPIOGRP28INTFLAG        (GPIOWorkBase + 0x1C0)
#define GPIOGRP28INTFLAG0       (GPIOWorkBase + 0x1C1)
#define GPIOGRP28INTFLAG1       (GPIOWorkBase + 0x1C2)
#define GPIOGRP28INTFLAG2       (GPIOWorkBase + 0x1C3)
#define GPIOGRP28INTFLAG3       (GPIOWorkBase + 0x1C4)
#define GPIOGRP28INTFLAG4       (GPIOWorkBase + 0x1C5)
#define GPIOGRP28INTFLAG5       (GPIOWorkBase + 0x1C6)
#define GPIOGRP28INTFLAG6       (GPIOWorkBase + 0x1C7)
#define GPIOGRP28INTFLAG7       (GPIOWorkBase + 0x1C8)

#define GPIOGRP29INTFLAG        (GPIOWorkBase + 0x1D0)
#define GPIOGRP29INTFLAG0       (GPIOWorkBase + 0x1D1)
#define GPIOGRP29INTFLAG1       (GPIOWorkBase + 0x1D2)
#define GPIOGRP29INTFLAG2       (GPIOWorkBase + 0x1D3)
#define GPIOGRP29INTFLAG3       (GPIOWorkBase + 0x1D4)
#define GPIOGRP29INTFLAG4       (GPIOWorkBase + 0x1D5)
#define GPIOGRP29INTFLAG5       (GPIOWorkBase + 0x1D6)
#define GPIOGRP29INTFLAG6       (GPIOWorkBase + 0x1D7)
#define GPIOGRP29INTFLAG7       (GPIOWorkBase + 0x1D8)

#define GPIOGRPN0INTFLAG        (GPIOWorkBase + 0x200)
#define GPIOGRPN0INTFLAG0       (GPIOWorkBase + 0x201)
#define GPIOGRPN0INTFLAG1       (GPIOWorkBase + 0x202)
#define GPIOGRPN0INTFLAG2       (GPIOWorkBase + 0x203)
#define GPIOGRPN0INTFLAG3       (GPIOWorkBase + 0x204)
#define GPIOGRPN0INTFLAG4       (GPIOWorkBase + 0x205)
#define GPIOGRPN0INTFLAG5       (GPIOWorkBase + 0x206)
#define GPIOGRPN0INTFLAG6       (GPIOWorkBase + 0x207)
#define GPIOGRPN0INTFLAG7       (GPIOWorkBase + 0x208)

#define GPIOGRPN1INTFLAG        (GPIOWorkBase + 0x210)
#define GPIOGRPN1INTFLAG0       (GPIOWorkBase + 0x211)
#define GPIOGRPN1INTFLAG1       (GPIOWorkBase + 0x212)
#define GPIOGRPN1INTFLAG2       (GPIOWorkBase + 0x213)
#define GPIOGRPN1INTFLAG3       (GPIOWorkBase + 0x214)
#define GPIOGRPN1INTFLAG4       (GPIOWorkBase + 0x215)
#define GPIOGRPN1INTFLAG5       (GPIOWorkBase + 0x216)
#define GPIOGRPN1INTFLAG6       (GPIOWorkBase + 0x217)
#define GPIOGRPN1INTFLAG7       (GPIOWorkBase + 0x218)

#define GPIOGRPN2INTFLAG        (GPIOWorkBase + 0x220)
#define GPIOGRPN2INTFLAG0       (GPIOWorkBase + 0x221)
#define GPIOGRPN2INTFLAG1       (GPIOWorkBase + 0x222)
#define GPIOGRPN2INTFLAG2       (GPIOWorkBase + 0x223)
#define GPIOGRPN2INTFLAG3       (GPIOWorkBase + 0x224)
#define GPIOGRPN2INTFLAG4       (GPIOWorkBase + 0x225)
#define GPIOGRPN2INTFLAG5       (GPIOWorkBase + 0x226)
#define GPIOGRPN2INTFLAG6       (GPIOWorkBase + 0x227)
#define GPIOGRPN2INTFLAG7       (GPIOWorkBase + 0x228)


#define GPIOGRPN3INTFLAG        (GPIOWorkBase + 0x230)
#define GPIOGRPN3INTFLAG0       (GPIOWorkBase + 0x231)
#define GPIOGRPN3INTFLAG1       (GPIOWorkBase + 0x232)
#define GPIOGRPN3INTFLAG2       (GPIOWorkBase + 0x233)
#define GPIOGRPN3INTFLAG3       (GPIOWorkBase + 0x234)
#define GPIOGRPN3INTFLAG4       (GPIOWorkBase + 0x235)
#define GPIOGRPN3INTFLAG5       (GPIOWorkBase + 0x236)
#define GPIOGRPN3INTFLAG6       (GPIOWorkBase + 0x237)
#define GPIOGRPN3INTFLAG7       (GPIOWorkBase + 0x238)


#define GPIOGRPN4INTFLAG        (GPIOWorkBase + 0x240)
#define GPIOGRPN4INTFLAG0       (GPIOWorkBase + 0x241)
#define GPIOGRPN4INTFLAG1       (GPIOWorkBase + 0x242)
#define GPIOGRPN4INTFLAG2       (GPIOWorkBase + 0x243)
#define GPIOGRPN4INTFLAG3       (GPIOWorkBase + 0x244)
#define GPIOGRPN4INTFLAG4       (GPIOWorkBase + 0x245)
#define GPIOGRPN4INTFLAG5       (GPIOWorkBase + 0x246)
#define GPIOGRPN4INTFLAG6       (GPIOWorkBase + 0x247)
#define GPIOGRPN4INTFLAG7       (GPIOWorkBase + 0x248)

#define GPIOGRPN5INTFLAG        (GPIOWorkBase + 0x250)
#define GPIOGRPN5INTFLAG0       (GPIOWorkBase + 0x251)
#define GPIOGRPN5INTFLAG1       (GPIOWorkBase + 0x252)
#define GPIOGRPN5INTFLAG2       (GPIOWorkBase + 0x253)
#define GPIOGRPN5INTFLAG3       (GPIOWorkBase + 0x254)
#define GPIOGRPN5INTFLAG4       (GPIOWorkBase + 0x255)
#define GPIOGRPN5INTFLAG5       (GPIOWorkBase + 0x256)
#define GPIOGRPN5INTFLAG6       (GPIOWorkBase + 0x257)
#define GPIOGRPN5INTFLAG7       (GPIOWorkBase + 0x258)

#define GPIOGRPN6INTFLAG        (GPIOWorkBase + 0x260)
#define GPIOGRPN6INTFLAG0       (GPIOWorkBase + 0x261)
#define GPIOGRPN6INTFLAG1       (GPIOWorkBase + 0x262)
#define GPIOGRPN6INTFLAG2       (GPIOWorkBase + 0x263)
#define GPIOGRPN6INTFLAG3       (GPIOWorkBase + 0x264)
#define GPIOGRPN6INTFLAG4       (GPIOWorkBase + 0x265)
#define GPIOGRPN6INTFLAG5       (GPIOWorkBase + 0x266)
#define GPIOGRPN6INTFLAG6       (GPIOWorkBase + 0x267)
#define GPIOGRPN6INTFLAG7       (GPIOWorkBase + 0x268)

#define GPIOGRPN7INTFLAG        (GPIOWorkBase + 0x270)
#define GPIOGRPN7INTFLAG0       (GPIOWorkBase + 0x271)
#define GPIOGRPN7INTFLAG1       (GPIOWorkBase + 0x272)
#define GPIOGRPN7INTFLAG2       (GPIOWorkBase + 0x273)
#define GPIOGRPN7INTFLAG3       (GPIOWorkBase + 0x274)
#define GPIOGRPN7INTFLAG4       (GPIOWorkBase + 0x275)
#define GPIOGRPN7INTFLAG5       (GPIOWorkBase + 0x276)
#define GPIOGRPN7INTFLAG6       (GPIOWorkBase + 0x277)
#define GPIOGRPN7INTFLAG7       (GPIOWorkBase + 0x278)

#define GPIOGRPN8INTFLAG        (GPIOWorkBase + 0x280)
#define GPIOGRPN8INTFLAG0       (GPIOWorkBase + 0x281)
#define GPIOGRPN8INTFLAG1       (GPIOWorkBase + 0x282)
#define GPIOGRPN8INTFLAG2       (GPIOWorkBase + 0x283)
#define GPIOGRPN8INTFLAG3       (GPIOWorkBase + 0x284)
#define GPIOGRPN8INTFLAG4       (GPIOWorkBase + 0x285)
#define GPIOGRPN8INTFLAG5       (GPIOWorkBase + 0x286)
#define GPIOGRPN8INTFLAG6       (GPIOWorkBase + 0x287)
#define GPIOGRPN8INTFLAG7       (GPIOWorkBase + 0x288)

#define GPIOGRPN9INTFLAG        (GPIOWorkBase + 0x290)
#define GPIOGRPN9INTFLAG0       (GPIOWorkBase + 0x291)
#define GPIOGRPN9INTFLAG1       (GPIOWorkBase + 0x292)
#define GPIOGRPN9INTFLAG2       (GPIOWorkBase + 0x293)
#define GPIOGRPN9INTFLAG3       (GPIOWorkBase + 0x294)
#define GPIOGRPN9INTFLAG4       (GPIOWorkBase + 0x295)
#define GPIOGRPN9INTFLAG5       (GPIOWorkBase + 0x296)
#define GPIOGRPN9INTFLAG6       (GPIOWorkBase + 0x297)
#define GPIOGRPN9INTFLAG7       (GPIOWorkBase + 0x298)


typedef struct PII_GPIOMISC_REGS 
{
  volatile unsigned char gpsic;
  volatile unsigned char gpisr0;
  volatile unsigned char gpisr1;
  volatile unsigned char sclkhigh_prd;
  volatile unsigned char sclklow_prd;
  volatile unsigned char gpisr2;
  volatile unsigned char gpisr3;
  volatile unsigned char gppcc;
} PII_GPIOMISC_REGS;


typedef struct PII_GPIOGRP_REGS 
{
  volatile unsigned char gpcfg0;
  volatile unsigned char gpcfg1;
  volatile unsigned char gpcfg2;
  volatile unsigned char gpcfg3;
  volatile unsigned char gpcfg4;
  volatile unsigned char gpcfg5;
  volatile unsigned char gpcfg6;
  volatile unsigned char gpcfg7;
  volatile unsigned char  gpdo;
  volatile unsigned char  gpdi;
  volatile unsigned char gpimen;
  volatile unsigned char gpevtsts;
  volatile unsigned char gpdbc0;
  volatile unsigned char gpdbc1;
  volatile unsigned char  rsvd0;
  volatile unsigned char  rsvd1;
} PII_GPIOGRP_REGS;

#define  GPGRP0_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE))
#define  GPGRP1_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0x10))
#define  GPGRP2_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0x20))
#define  GPGRP3_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0x30))
#define  GPGRP4_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0x40))
#define  GPGRP5_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0x50))
#define  GPGRP6_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0x60))
#define  GPGRP7_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0x70))
#define  GPGRP8_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0x80))
#define  GPGRP9_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0x90))
#define  GPGRP10_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0xa0))
#define  GPGRP11_REGS	((volatile PII_GPIOGRP_REGS *)(GPIO0BASE+0xb0))
#define  GPMISC_REGS	((volatile PII_GPIOMISC_REGS *)(GPIO0BASE+0xf0))


extern void GPIOHandler (void);
extern void gpio_grp_handler (tU32, tU32);
extern void gpio_set_irq_handler (tU32);
extern void gpio_irq_enable (tU32, tU8);
extern void gpio_irq_disable (tU32, tU8);


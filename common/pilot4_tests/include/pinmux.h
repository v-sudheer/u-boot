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

#define SE_SYS_CLK_VA_BASE      (0x40100100)
  
#define SE_TOP_LVL_PIN_VA_BASE  (TOPLEVEL_PINCTRL)

#define TOP_LVL_PIN_GPIOEN0    (SE_TOP_LVL_PIN_VA_BASE)
#define TOP_LVL_PIN_MUXCTL     (SE_TOP_LVL_PIN_VA_BASE+0x4)
#define TOP_LVL_PIN_GPIOEN1    (SE_TOP_LVL_PIN_VA_BASE+0x8)
#define TOP_LVL_PIN_GPIOEN2    (SE_TOP_LVL_PIN_VA_BASE+0xc)
#define TOP_LVL_PIN_STRNCTL    (SE_TOP_LVL_PIN_VA_BASE+0x14)
#define TOP_LVL_PIN_SWSTRPCTL  (SE_TOP_LVL_PIN_VA_BASE+0x18)
#define TOP_LVL_PIN_PCIECTL    (SE_TOP_LVL_PIN_VA_BASE+0x1c)
#define TOP_LVL_PIN_LPCPINCTL  (SE_TOP_LVL_PIN_VA_BASE+0x20)
  



typedef struct
{
  volatile unsigned int ddr2_mode                      :1;
  volatile unsigned int romboot                        :1;
  volatile unsigned int rsvd3                          :1;
  volatile unsigned int mac1_en                        :1;
  volatile unsigned int mac0_rgmii_mode_en             :1;
  volatile unsigned int nand_en                        :1;
   volatile unsigned int sdstrap_en                        :1;
  volatile unsigned int uart4_en                       :1;
  volatile unsigned int sspjtag_en                     :1;
  volatile unsigned int rstbyp_en                      :1;
  volatile unsigned int eeprom_en                      :1;
  volatile unsigned int intacpi_en                     :1;
  volatile unsigned int bar_strap                      :1;
  volatile unsigned int hspi_en                        :1;
  volatile unsigned int hspi_swap_en                   :1;
  volatile unsigned int hspi_swap_size_0               :1;
  volatile unsigned int hspi_swap_size_1               :1;
  volatile unsigned int crtm_size_0                    :1;
  volatile unsigned int crtm_size_1                    :1;
  volatile unsigned int crtm_en                        :1;
  volatile unsigned int mac1_rgmii_mode_en             :1;
}Se_Starpreg_Sts;


 /*this is the structure passed from user space to driver */
typedef struct PINMUX_CMD
{
  
  union{
    Se_Starpreg_Sts Strap_sts;
    unsigned int val;
  }uStrap_sts;
  
}PINMUX_CMD;

typedef enum { 	
             GET_STRAP_STS,
	     SET_GPIO2_0_PWM4_ASGPIO,
     	     SET_GPIO2_0_PWM4_ASPWM,
	     GET_PINSTAT_GPIO2_0_OR_PWM4,
		
	     SET_GPIO2_1_PWM5_ASGPIO,
     	     SET_GPIO2_1_PWM5_ASPWM,
     	     GET_PINSTAT_GPIO2_1_OR_PWM5,
		
	     SET_GPIO2_2_PWM6_ASGPIO,
     	     SET_GPIO2_2_PWM6_ASPWM,
       	     GET_PINSTAT_GPIO2_2_OR_PWM6,
		
	     SET_GPIO2_3_PWM7_ASGPIO,
     	     SET_GPIO2_3_PWM7_ASPWM,
     	     GET_PINSTAT_GPIO2_3_OR_PWM7,
		
	     SET_GPIO2_4_FT8_ASGPIO,
     	     SET_GPIO2_4_FT8_ASFTACH,
             GET_PINSTAT_GPIO2_4_OR_FT8,
		
	     SET_GPIO2_5_FT9_ASGPIO,
     	     SET_GPIO2_5_FT9_ASFTACH,
	     GET_PINSTAT_GPIO2_5_OR_FT9,
		
	     SET_GPIO2_6_FT10_ASGPIO,
     	     SET_GPIO2_6_FT10_ASFTACH,
	     GET_PINSTAT_GPIO2_6_OR_FT10,
		
	     SET_GPIO2_7_FT11_ASGPIO,
     	     SET_GPIO2_7_FT11_ASFTACH,
	     GET_PINSTAT_GPIO2_7_OR_FT11,
		
             SET_GPIO14_0_FT12_ASGPIO,
             SET_GPIO14_0_FT12_ASFTACH,
	     GET_PINSTAT_GPIO14_0_OR_FT12,
		
             SET_GPIO14_1_FT13_ASGPIO,
             SET_GPIO14_1_FT13_ASFTACH,
	     GET_PINSTAT_GPIO14_1_OR_FT13,
		
             SET_GPIO14_2_FT14_ASGPIO,
             SET_GPIO14_2_FT14_ASFTACH,
	     GET_PINSTAT_GPIO14_2_OR_FT14,
		
             SET_GPIO14_3_FT15_ASGPIO,
             SET_GPIO14_3_FT15_ASFTACH,
   	     GET_PINSTAT_GPIO14_3_OR_FT15,
		
	     SET_GPIO23_0_FT4_U2CTS_ASGPIO,
             SET_GPIO23_0_FT4_U2CTS_ASU2CTS,
     	     SET_GPIO23_0_FT4_U2CTS_ASFT4,
	     GET_PINSTAT_GPIO23_0_FT4_U2CTS,
		
	     SET_GPIO23_1_FT5_U2DCD_ASGPIO,
             SET_GPIO23_1_FT5_U2DCD_ASU2DCD,
     	     SET_GPIO23_1_FT5_U2DCD_ASFT5,
	     GET_PINSTAT_GPIO23_1_FT5_U2DCD,
		
    	     SET_GPIO23_2_FT6_U2DSR_ASGPIO,
	     SET_GPIO23_2_FT6_U2DSR_ASU2DSR,
  	     SET_GPIO23_2_FT6_U2DSR_ASFT6,
	     GET_PINSTAT_GPIO23_2_FT6_U2DSR,
		
  	     SET_GPIO23_3_FT7_U2RI_ASGPIO,
	     SET_GPIO23_3_FT7_U2RI_ASU2RI,
	     SET_GPIO23_3_FT7_U2RI_ASFT7,
	     GET_PINSTAT_GPIO23_3_FT7_U2RI,

 	     SET_GPIO23_4_U2RXD_ASGPIO,
	     SET_GPIO23_4_U2RXD_ASU2RXD,
	     GET_PINSTAT_GPIO23_4_U2RXD,
		
	     SET_GPIO23_5_PWM2_U2DTR_ASGPIO,
	     SET_GPIO23_5_PWM2_U2DTR_ASU2DTR,
	     SET_GPIO23_5_PWM2_U2DTR_ASPWM2,
	     GET_PINSTAT_GPIO23_5_PWM2_U2DTR,
	    
	     SET_GPIO23_6_PWM3_U2RTS_ASGPIO,
	     SET_GPIO23_6_PWM3_U2RTS_ASU2RTS,
	     SET_GPIO23_6_PWM3_U2RTS_ASPWM3,
	     GET_PINSTAT_GPIO23_6_PWM3_U2RTS,

	     SET_GPIO23_7_U2TXD_ASGPIO,
	     SET_GPIO23_7_U2TXD_ASU2TXD,
	     GET_PINSTAT_GPIO23_7_U2TXD,

	     SET_GPIO28_45_I2C6_KBD_AS_ASGPIO,
	     SET_GPIO28_45_I2C6_KBD_AS_ASI2C6,
	     SET_GPIO28_45_I2C6_KBD_AS_ASKBD,
	     GET_PINSTAT_GPIO28_45_I2C6_KBD,

	     SET_GPIO28_67_I2C7_MOUSE_AS_ASGPIO,
	     SET_GPIO28_67_I2C7_MOUSE_AS_ASI2C7,
	     SET_GPIO28_67_I2C7_MOUSE_AS_ASMOUSE,
	     GET_PINSTAT_GPIO28_67_I2C7_MOUSE,

 	     SET_GPIO26_6_BMCWDO_U3TXD_ASGPIO,
	     SET_GPIO26_6_BMCWDO_U3TXD_ASU3TXD,
	     SET_GPIO26_6_BMCWDO_U3TXD_ASBMCWDO,
	     GET_PINSTAT_GPIO26_6_U3TXD_BMCWDO,

	     SET_GPIO26_7_SIOWDO_U3RXD_ASGPIO,
	     SET_GPIO26_7_SIOWDO_U3RXD_ASU3RXD,
	     SET_GPIO26_7_SIOWDO_U3RXD_ASSIOWDO,
	     GET_PINSTAT_GPIO26_7_U3RXD_SIOWDO,

	     SET_GPIO26_2_SIOLDRQ_SIOSCI_ASGPIO,
	     SET_GPIO26_2_SIOLDRQ_SIOSCI_ASSIOSCI,
	     SET_GPIO26_2_SIOLDRQ_SIOSCI_ASSIOLDRQ,
	     GET_PINSTAT_GPIO26_2_SIOLDRQ_SIOSCI,

	     SET_GPIO22_0_U1CTS_ASGPIO,
	     SET_GPIO22_0_U1CTS_ASCTS,
	     GET_PINSTAT_GPIO22_0_U1CTS,

	     SET_GPIO22_1_U1DCD_ASGPIO,
	     SET_GPIO22_1_U1DCD_ASU1DCD,
	     GET_PINSTAT_GPIO22_1_U1DCD,

	     SET_GPIO22_2_U1DSR_ASGPIO,
	     SET_GPIO22_2_U1DSR_ASU1DSR,
	     GET_PINSTAT_GPIO22_2_U1DSR,

	     SET_GPIO22_3_U1RI_ASGPIO,
	     SET_GPIO22_3_U1RI_ASU1RI,
	     GET_PINSTAT_GPIO22_3_U1RI,	

	     SET_GPIO22_4_U1RXD_ASGPIO,
	     SET_GPIO22_4_U1RXD_ASU1RXD,
	     GET_PINSTAT_GPIO22_4_U1RXD,

	     SET_GPIO22_5_U1DTR_ASGPIO,
	     SET_GPIO22_5_U1DTR_ASU1DTR,
	     GET_PINSTAT_GPIO22_5_U1DTR,

	     SET_GPIO22_6_U1RTS_ASGPIO,
	     SET_GPIO22_6_U1RTS_ASU1RTS,
	     GET_PINSTAT_GPIO22_6_U1RTS,

	     SET_GPIO22_7_U1TXD_ASGPIO,
	     SET_GPIO22_7_U1TXD_ASU1TXD,
	     GET_PINSTAT_GPIO22_7_U1TXD,

	     SET_GPIO24_3_0_SGPIO_ASGPIO,
	     SET_GPIO24_3_0_SGPIO_ASSGPIO,
	     GET_PINSTAT_GPIO24_3_0_SGPIO,
	     GET_STAT_GPIO24_7_4,
		
	     SET_GPIO25_1_0_PASSTHRU0_ASGPIO,
	     SET_GPIO25_1_0_PASSTHRU0_ASPASSTHRU0,
	     GET_PINSTAT_GPIO25_1_0_PASSTHRU0,

	     SET_GPIO25_3_2_PASSTHRU1_ASGPIO,
	     SET_GPIO25_3_2_PASSTHRU1_ASPASSTHRU1,
	     GET_PINSTAT_GPIO25_3_2_PASSTHRU1,

	     SET_GPIO25_5_4_PASSTHRU2_ASGPIO,
	     SET_GPIO25_5_4_PASSTHRU2_ASPASSTHRU2,
	     GET_PINSTAT_GPIO25_5_4_PASSTHRU2,
		
	     SET_GPIO25_6_PWRBTNIN_ASGPIO,
	     SET_GPIO25_6_PWRBTNIN_ASPWRBTNIN,
	     GET_PINSTAT_GPIO25_6_PWRBTNIN,

	     SET_GPIO25_7_PWRBTNOUT_ASGPIO,
	     SET_GPIO25_7_PWRBTNOUT_ASPWRBTNOUT,
	     GET_PINSTAT_GPIO25_7_PWRBTNOUT,

	     SET_GPIO26_0_SIOONCTL_ASGPIO,
	     SET_GPIO26_0_SIOONCTL_ASSIOONCTL,
	     GET_PINSTAT_GPIO26_0_SIOONCTL,

	     SET_GPIO26_1_SIOPWRREQ_ASGPIO,
	     SET_GPIO26_1_SIOPWRREQ_ASSIOPWRREQ,
	     GET_PINSTAT_GPIO26_1_SIOPWRREQ,

	     SET_GPIO26_3_SIOSLP3_ASGPIO,
	     SET_GPIO26_3_SIOSLP3_ASSIOSLP3,
	     GET_PINSTAT_GPIO26_3_SIOSLP3,
		
	     SET_GPIO26_4_SIOSLP5_ASGPIO,
	     SET_GPIO26_4_SIOSLP5_ASSIOSLP5,
	     GET_PINSTAT_GPIO26_4_SIOSLP5,

	     SET_GPIO26_5_ICMBTXEN_ASGPIO,
	     SET_GPIO26_5_ICMBTXEN_ASICMBTXEN,
	     GET_PINSTAT_GPIO26_5_ICMBTXEN,

	     SET_GPIO27_1_0_I2C0_ASGPIO,
	     SET_GPIO27_1_0_I2C0_ASI2C0,
	     GET_PINSTAT_GPIO27_1_0_I2C0,

	     SET_GPIO27_3_2_I2C1_ASGPIO,
	     SET_GPIO27_3_2_I2C1_ASI2C1,
	     GET_PINSTAT_GPIO27_3_2_I2C1,
		
	     SET_GPIO27_5_4_I2C2_ASGPIO,
	     SET_GPIO27_5_4_I2C2_ASI2C2,
	     GET_PINSTAT_GPIO27_5_4_I2C2,
		
	     SET_GPIO27_7_6_I2C3_ASGPIO,
	     SET_GPIO27_7_6_I2C3_ASI2C3,
	     GET_PINSTAT_GPIO27_7_6_I2C3,

		
	     SET_GPIO28_1_0_I2C4_ASGPIO,
	     SET_GPIO28_1_0_I2C4_ASI2C4,
	     GET_PINSTAT_GPIO28_1_0_I2C4,

	     SET_GPIO28_3_2_I2C5_ASGPIO,
	     SET_GPIO28_3_2_I2C5_ASI2C5,
	     GET_PINSTAT_GPIO28_3_2_I2C5,

	     SET_GPIO29_0_PWM0_ASGPIO,
	     SET_GPIO29_0_PWM0_ASPWM0,
	     GET_PINSTAT_GPIO29_0_PWM0,

	     SET_GPIO29_1_PWM1_ASGPIO,
	     SET_GPIO29_1_PWM1_ASPWM1,
	     GET_PINSTAT_GPIO29_1_PWM1,

	     SET_GPIO29_2_FT0_ASGPIO,
	     SET_GPIO29_2_FT0_ASFT0,
	     GET_PINSTAT_GPIO29_2_FT0,

	     SET_GPIO29_3_FT1_ASGPIO,
	     SET_GPIO29_3_FT1_ASFT1,
	     GET_PINSTAT_GPIO29_3_FT1,

	     SET_GPIO29_4_FT2_ASGPIO,
	     SET_GPIO29_4_FT2_ASFT2,
	     GET_PINSTAT_GPIO29_4_FT2,

	     SET_GPIO29_5_FT3_ASGPIO,
	     SET_GPIO29_5_FT3_ASFT3,
	     GET_PINSTAT_GPIO29_5_FT3,
				

}se_Pinmux_Func; 

tU32  pinmux_ioctl(unsigned int cmd, PINMUX_CMD *Pinmuxcmd);


//Pinmuxctrl(0x4) Regbits

#define PINMUXCTL_EN_PWM4     (1<<0)
#define PINMUXCTL_EN_PWM5     (1<<1)
#define PINMUXCTL_EN_PWM6     (1<<2)
#define PINMUXCTL_EN_PWM7     (1<<3)
#define PINMUXCTL_EN_FT8      (1<<4)
#define PINMUXCTL_EN_FT9      (1<<5)
#define PINMUXCTL_EN_FT10     (1<<6)
#define PINMUXCTL_EN_FT11     (1<<7)
#define PINMUXCTL_EN_FT12     (1<<8)
#define PINMUXCTL_EN_FT13     (1<<9)
#define PINMUXCTL_EN_FT14     (1<<10)
#define PINMUXCTL_EN_FT15     (1<<11)
#define PINMUXCTL_EN_FT4      (1<<12)
#define PINMUXCTL_EN_FT5      (1<<13)
#define PINMUXCTL_EN_FT6      (1<<14)
#define PINMUXCTL_EN_FT7      (1<<15)
#define PINMUXCTL_EN_PWM2     (1<<16)
#define PINMUXCTL_EN_PWM3     (1<<17)
#define PINMUXCTL_EN_KBD      ((1<<18)|(1<<19))
#define PINMUXCTL_EN_MOUSE    ((1<<20)|(1<<21))
#define PINMUXCTL_EN_BMCWDO   (1<<22)
#define PINMUXCTL_EN_SIOWDO   (1<<23)
#define PINMUXCTL_EN_SIOLDRQ  (1<<24)



//GPIO1EN(0x8)Regbits
#define GPIOEN1_DISABLE_U1CTS   (1<<0)
#define GPIOEN1_DISABLE_U1DCD   (1<<1)
#define GPIOEN1_DISABLE_U1DSR   (1<<2)
#define GPIOEN1_DISABLE_U1RI    (1<<3)
#define GPIOEN1_DISABLE_U1RXD   (1<<4)
#define GPIOEN1_DISABLE_U1DTR   (1<<5)
#define GPIOEN1_DISABLE_U1RTS   (1<<6)
#define GPIOEN1_DISABLE_U1TXD   (1<<7)
  
#define GPIOEN1_DISABLE_U2CTS   (1<<8)
#define GPIOEN1_DISABLE_U2DCD   (1<<9)
#define GPIOEN1_DISABLE_U2DSR   (1<<10)
#define GPIOEN1_DISABLE_U2RI    (1<<11)
#define GPIOEN1_DISABLE_U2RXD   (1<<12)
#define GPIOEN1_DISABLE_U2DTR   (1<<13)
#define GPIOEN1_DISABLE_U2RTS   (1<<14)
#define GPIOEN1_DISABLE_U2TXD   (1<<15)
#define GPIOEN1_DISABLE_SGPIO   ((1<<19)|(1<<8)|(1<<17)|(1<<16))

#define GPIOEN1_DISABLE_PASSTHRU0    ((1<<25)|(1<<24))
#define GPIOEN1_DISABLE_PASSTHRU1    ((1<<27)|(1<<26))
#define GPIOEN1_DISABLE_PASSTHRU2    ((1<<29)|(1<<28))
#define GPIOEN1_DISABLE_SIOPWRBTN_IN  (1<<30) 
#define GPIOEN1_DISABLE_SIOPWRBTN_OUT (1<<31)  

  
//GPIOEN2(0xc) Regbits
#define GPIOEN2_DISABLE_SIOONCTL    (1<<0)
#define GPIOEN2_DISABLE_SIOPWRREQ   (1<<1)
#define GPIOEN2_DISABLE_SIOSCI      (1<<2)
#define GPIOEN2_DISABLE_SIOSLP3     (1<<3)
#define GPIOEN2_DISABLE_SIOSLP5     (1<<4)
#define GPIOEN2_DISABLE_ICMB_TXEN   (1<<5)
#define GPIOEN2_DISABLE_U3TXD       (1<<6)
#define GPIOEN2_DISABLE_U3RXD       (1<<7)
#define GPIOEN2_DISABLE_I2C0        ((1<<9)|(1<<8))
#define GPIOEN2_DISABLE_I2C1        ((1<<11)|(1<<10))
#define GPIOEN2_DISABLE_I2C2        ((1<<13)|(1<<12))
#define GPIOEN2_DISABLE_I2C3        ((1<<15)|(1<<14))
#define GPIOEN2_DISABLE_I2C4        ((1<<17)|(1<<16))
#define GPIOEN2_DISABLE_I2C5        ((1<<19)|(1<<18))
#define GPIOEN2_DISABLE_I2C6        ((1<<21)|(1<<20))
#define GPIOEN2_DISABLE_I2C7        ((1<<23)|(1<<22))
#define GPIOEN2_DISABLE_PWM0        (1<<24)
#define GPIOEN2_DISABLE_PWM1        (1<<25)
#define GPIOEN2_DISABLE_FT0         (1<<26)
#define GPIOEN2_DISABLE_FT1         (1<<27)
#define GPIOEN2_DISABLE_FT2         (1<<28)
#define GPIOEN2_DISABLE_FT3         (1<<29)
  

  
//Strap_Status bits
#define   UART4_STRAP        (1<<7)  
#define   HSPI_EN_STRAP      (1<<13)
#define   RGMII1_MODE_STRAP  (1<<20)







#if 0  
#define P3_PINMUX_GPIO0EN          (TOPLEVEL_PINCTRL+0x0)
#define P3_PINMUX_CTL         (TOPLEVEL_PINCTRL+0x4)
#define P3_PINMUX_GPIO1EN           (TOPLEVEL_PINCTRL+0x8)
#define P3_PINMUX_GPIO2EN           (TOPLEVEL_PINCTRL+0xc)


//I2C registers inside system control block
#define GIPO2EN_I2C7   ((1<<23)|(1<<24))
#define GIPO2EN_I2C6   ((1<<20)|(1<<21))

#define PINMUXCTL_I2C7  ((1<<20)|(1<<21))
#define PINMUXCTL_I2C6  ((1<<18)|(1<<19))  

#define SEL_I2C6      1
#define SEL_PS20_D    2
#define SEL_GPIO28_5  4

typedef struct
{
  volatile unsigned int ddr2_mode     	:1;
  volatile unsigned int romboot     	:1;
}Se_P3_Starpreg_Sts;

typedef struct
{
  unsigned int  i2c6_ps20d_gpio28_5;
  unsigned int  i2c7_ps20c_gpio28_4;
}Se_P3_Pinmux_Ctrl;

#endif

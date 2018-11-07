/*
 *  Copyright (C) 2013-14 Emulex
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __PILOT4_MAP_H__
#define __PILOT4_MAP_H__


/********************************************************************
   Internal Register Mapping (0x40000000 to 0x40AFFFFF) 
*********************************************************************/
#define P4_REGISTER_BASE				0x40000000
#define SYS_PLLLOCK	(1<<14)
#define DDR_PLLLOCK	(1<<29)
#define PLLLOCK		(SYS_PLLLOCK + DDR_PLLLOCK)
#define SYSCONTROL	0x40100100
#define LMEMSTART	0x10000000

#define SCLKSTS		(SYSCONTROL + 0x4)
#define SCLKCTL		(SYSCONTROL + 0x8)

#define P4_INTC_BASE        		  	(P4_REGISTER_BASE + 0x0000000)    	/* 1 MB */
#define P4_LPC_ROM_BASE                	(P4_REGISTER_BASE + 0x0100000)	 	/* 256  */
#define P4_SYS_CLK_BASE                	(P4_REGISTER_BASE + 0x0100100)	 	/* 256  */
#define P4_SCRATCH_128_BASE            	(P4_REGISTER_BASE + 0x0100200)    	/* 256  */ 
#define P4_SCRATCH_RAM_BASE            	(P4_REGISTER_BASE + 0x0101000)	 	/* 24 K */ 	
#define P4_BOOT_SPI_BASE		       	(P4_REGISTER_BASE + 0x0200000)  	/* 1 MB */ 
#define P4_DDR2_CTRL_BASE              	(P4_REGISTER_BASE + 0x0300000) 	 	/* 2 MB */
#define DDR_REG_BASE                    (P4_REGISTER_BASE + 0x0300000)
#define P4_MAC_A_BASE	           	   	(P4_REGISTER_BASE + 0x0500000)    	/* 1 MB */
#define P4_MAC_B_BASE                  	(P4_REGISTER_BASE + 0x0600000)	 	/* 1 MB */
#define P4_USB_0_BASE				   	(P4_REGISTER_BASE + 0x0700000) 	 	/* 1 MB */
#define P4_USB_1_BASE                  	(P4_REGISTER_BASE + 0x0800000)	 	/* 1 MB */
#define P4_FGE_BASE					   	(P4_REGISTER_BASE + 0x0900000)	 	/* 1 MB */	

#define P4_SECURITY_BASE			   	(P4_REGISTER_BASE + 0x0A00000)  	/* 1 MB */
#define P4_AHB_DMA_BASE			       	(P4_REGISTER_BASE + 0x0400000)	 	/* 64 K */	
/*
//#define P4_AHB_SYS_APB_BRIDGE        	(P4_REGISTER_BASE + 0x0410000)
//#define P4_AHB_SIO_BRIDGE 	 	   	(P4_REGISTER_BASE + 0x0420000)
//#define P4_AHB_PER_APB_BRIDGE 	   	(P4_REGISTER_BASE + 0x0430000)	
*/
#define P4_GPIO_BASE				   	(P4_REGISTER_BASE + 0x0420000)	 	/*  4 K */	
#define P4_SIO_UART_0_BASE			   	(P4_REGISTER_BASE + 0x0421000)	 	/*  4 K */
#define P4_SIO_UART_1_BASE			   	(P4_REGISTER_BASE + 0x0422000)    	/*  4 K */
#define P4_SYS_WAKEUP_BASE			   	(P4_REGISTER_BASE + 0x0425000)    	/*  4 K */
#define P4_KCS1_KCS2_BASE			   	(P4_REGISTER_BASE + 0x0426000)    	/*  512 */
#define P4_KCS3_BASE				   	(P4_REGISTER_BASE + 0x0426200)    	/*  256 */
#define P4_KCS4_BASE			       	(P4_REGISTER_BASE + 0x0426300)	 	/*  256 */
#define P4_KCS5_BT_SMIC_MAIL_BASE	   	(P4_REGISTER_BASE + 0x0426400)		/*  256 */
#define P4_PILOT_SPEC_BASE             	(P4_REGISTER_BASE + 0x0426500)		/* 3K - 256 */	
#define P4_WDT_MSFT_BASE  	           	(P4_REGISTER_BASE + 0x0427000)		/*  4 K */
#define P4_RTC_BASE		        	   	(P4_REGISTER_BASE + 0x0428000)		/*  4 K */  
#define P4_HOST_SPI_BASE		       	(P4_REGISTER_BASE + 0x0429000)	    /*  256 */
#define P4_TAP_BASE                    	(P4_REGISTER_BASE + 0x0429100)		/*  256 */
#define P4_WDT_BMC_BASE                	(P4_REGISTER_BASE + 0x0410000)		/*  4 K */		
#define P4_TIMER_BASE				   	(P4_REGISTER_BASE + 0x0411000)		/*  4 K */
#define P4_PS2_BASE					   	(P4_REGISTER_BASE + 0x0412000)		/*  4 K */
#define P4_FAN_TACH_PWM_BASE			(P4_REGISTER_BASE + 0x0413000)		/*  4 K */
#define P4_ADC_THERMAL_BASE				(P4_REGISTER_BASE + 0x0414000)	 	/*  4 K */
#define P4_VIRTUAL_UART_0_BASE			(P4_REGISTER_BASE + 0x0415000)		/*  4 K */
#define P4_VIRTUAL_UART_1_BASE			(P4_REGISTER_BASE + 0x0416000)		/*  4 K */
#define P4_BOMBER_BASE                  (P4_REGISTER_BASE + 0x0417000) 	 	/*  4 K */
#define P4_PECI_BASE                 	(P4_REGISTER_BASE + 0x0418000)		/* 32 K */
#define P4_UART_2_BASE	                (P4_REGISTER_BASE + 0x0430000)		/*  4 K */
#define P4_UART_3_BASE	                (P4_REGISTER_BASE + 0x0431000)		/*  4 K */
#define P4_UART_4_BASE	                (P4_REGISTER_BASE + 0x0432000)		/*  4 K */
#define P4_I2C_0_BASE	                (P4_REGISTER_BASE + 0x0433000)		/*  4 K */
#define P4_I2C_1_BASE	                (P4_REGISTER_BASE + 0x0434000)		/*  4 K */
#define P4_I2C_2_BASE	                (P4_REGISTER_BASE + 0x0435000)		/*  4 K */
#define P4_I2C_3_BASE	                (P4_REGISTER_BASE + 0x0436000)		/*  4 K */
#define P4_I2C_4_BASE	                (P4_REGISTER_BASE + 0x0437000)		/*  4 K */
#define P4_I2C_5_BASE	                (P4_REGISTER_BASE + 0x0438000)		/*  4 K */
#define P4_BACKUP_SPI_BASE	            (P4_REGISTER_BASE + 0x0280000)		/*  4 K */


/*--------------- Virtual address of the IO Registers Region  ------------*/
#define P4_REGISTER_VA_BASE				IO_ADDRESS(P4_REGISTER_BASE)
#define P4_INTC_VA_BASE					IO_ADDRESS(P4_INTC_BASE)
#define P4_LPC_ROM_VA_BASE				IO_ADDRESS(P4_LPC_ROM_BASE)
#define P4_SYS_CLK_VA_BASE				IO_ADDRESS(P4_SYS_CLK_BASE)
#define P4_SCRATCH_128_VA_BASE			IO_ADDRESS(P4_SCRATCH_128_BASE)
#define P4_SCRATCH_RAM_VA_BASE			IO_ADDRESS(P4_SCRATCH_RAM_BASE)
#define P4_BOOT_SPI_VA_BASE				IO_ADDRESS(P4_BOOT_SPI_BASE)
#define P4_DDR2_CTRL_VA_BASE			IO_ADDRESS(P4_DDR2_CTRL_BASE)
#define P4_MAC_A_VA_BASE				IO_ADDRESS(P4_MAC_A_BASE)
#define P4_MAC_B_VA_BASE				IO_ADDRESS(P4_MAC_B_BASE)
#define P4_USB_0_VA_BASE				IO_ADDRESS(P4_USB_0_BASE)
#define P4_USB_1_VA_BASE				IO_ADDRESS(P4_USB_1_BASE)
#define P4_FGE_VA_BASE					IO_ADDRESS(P4_FGE_BASE)
#define P4_SECURITY_VA_BASE				IO_ADDRESS(P4_SECURITY_BASE)
#define P4_AHB_DMA_VA_BASE				IO_ADDRESS(P4_AHB_DMA_BASE)
#define P4_GPIO_VA_BASE					IO_ADDRESS(P4_GPIO_BASE)
#define P4_SIO_UART_0_VA_BASE			IO_ADDRESS(P4_SIO_UART_0_BASE)
#define P4_SIO_UART_1_VA_BASE			IO_ADDRESS(P4_SIO_UART_1_BASE)
#define P4_SYS_WAKEUP_VA_BASE			IO_ADDRESS(P4_SYS_WAKEUP_BASE)
#define P4_KCS1_KCS2_VA_BASE			IO_ADDRESS(P4_KCS1_KCS2_BASE)
#define P4_KCS3_VA_BASE					IO_ADDRESS(P4_KCS3_BASE)
#define P4_KCS4_VA_BASE					IO_ADDRESS(P4_KCS4_BASE)
#define P4_KCS5_BT_SMIC_MAIL_VA_BASE	IO_ADDRESS(P4_KCS5_BT_SMIC_MAIL_BASE)
#define P4_PILOT_SPEC_VA_BASE			IO_ADDRESS(P4_PILOT_SPEC_BASE)
#define P4_WDT_MSFT_VA_BASE				IO_ADDRESS(P4_WDT_MSFT_BASE)
#define P4_RTC_VA_BASE					IO_ADDRESS(P4_RTC_BASE)
#define P4_HOST_SPI_VA_BASE				IO_ADDRESS(P4_HOST_SPI_BASE)
#define P4_TAP_VA_BASE					IO_ADDRESS(P4_TAP_BASE)
#define P4_WDT_BMC_VA_BASE				IO_ADDRESS(P4_WDT_BMC_BASE)
#define P4_TIMER_VA_BASE				IO_ADDRESS(P4_TIMER_BASE)
#define P4_PS2_VA_BASE					IO_ADDRESS(P4_PS2_BASE)
#define P4_FAN_TACH_PWM_VA_BASE			IO_ADDRESS(P4_FAN_TACH_PWM_BASE)
#define P4_ADC_THERMAL_VA_BASE			IO_ADDRESS(P4_ADC_THERMAL_BASE)
#define P4_VIRTUAL_UART_0_VA_BASE		IO_ADDRESS(P4_VIRTUAL_UART_0_BASE)
#define P4_VIRTUAL_UART_1_VA_BASE		IO_ADDRESS(P4_VIRTUAL_UART_1_BASE)
#define P4_BOMBER_VA_BASE				IO_ADDRESS(P4_BOMBER_BASE)
#define P4_PECI_VA_BASE					IO_ADDRESS(P4_PECI_BASE)
#define P4_UART_2_VA_BASE				IO_ADDRESS(P4_UART_2_BASE)
#define P4_UART_3_VA_BASE				IO_ADDRESS(P4_UART_3_BASE)
#define P4_UART_4_VA_BASE				IO_ADDRESS(P4_UART_4_BASE)
#define P4_I2C_0_VA_BASE				IO_ADDRESS(P4_I2C_0_BASE)
#define P4_I2C_1_VA_BASE				IO_ADDRESS(P4_I2C_1_BASE)
#define P4_I2C_2_VA_BASE				IO_ADDRESS(P4_I2C_2_BASE)
#define P4_I2C_3_VA_BASE				IO_ADDRESS(P4_I2C_3_BASE)
#define P4_I2C_4_VA_BASE				IO_ADDRESS(P4_I2C_4_BASE)
#define P4_I2C_5_VA_BASE				IO_ADDRESS(P4_I2C_5_BASE)
#define P4_BACKUP_SPI_VA_BASE			IO_ADDRESS(P4_BACKUP_SPI_BASE)


/*****************************************************************
					  IRQ Assignment
*****************************************************************/

#define NR_IRQS							32	
#define P4_NR_IRQS                     	32
#define P4_NR_FIQS                     	32
#define MAXIRQNUM                       31
#define MAXFIQNUM                       31

#define IRQ_MAC_A		           		0
#define IRQ_MAC_B    	                1
#define IRQ_FGE                     	2
#define IRQ_USB                       	3
#define IRQ_SYS_CLK_SWITCH            	27
#define IRQ_SECURITY					28
#define IRQ_BMC_SSP_DOORBELL           	30 
#define IRQ_SSP_BMC_DOORBELL            31
#define IRQ_GPIO						16
#define IRQ_SYS_WAKEUP					17
#define IRQ_KCS_BT_SMIC_MAIL			18
#define IRQ_TIMER_0                     6
#define IRQ_TIMER_1                     7
#define IRQ_BOMBER						29
#define IRQ_UART_2                      19
#define IRQ_UART_3						20
#define IRQ_UART_4                      21
#define IRQ_I2C_0                       23
#define IRQ_I2C_1                       24
#define IRQ_I2C_2                       25	
#define IRQ_I2C_3                       26	
#define IRQ_I2C_4                       4
#define IRQ_I2C_5						5
#define IRQ_ICMB						22	
#define IRQ_SUPER_IO					11		// Combined Interrupt
#define IRQ_BMC							13		// Combined Interrupt

/*****************************************************************
				    APB DMA channel assignment
*****************************************************************/
/* TODO */

#define MAX_P4_ETH		2

#endif

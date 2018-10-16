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

#define DDRBASE            DDRRAM
#define DDRTOP             DDRBASE + 0x02000000          //total of 32


#define NAND_ASRAM_START  0x20000000
#define NAND_ASRAM_END_ONBOARD  NAND_ASRAM_START+0x20000
#define NAND_ASRAM_END  0x24000000


#define  SSP_MEMSTART	        LMEMSTART
#define  FGB_MEMSTART	        LMEMSTART     

#define  SSP_MEMEND	        LMEMSTART + 0x00008000 // 0x04008000
#define  FGB_MEMEND	        LMEMSTART + 0x00008000 // 0x04008000     

#if defined (CODE_IN_ASRAM)

#if defined (HW_ENV)
#define Bank1start   0x10000000
#define Bank2start   NAND_ASRAM_START+0x6400 //200k for code
#define Bank3start   NAND_ASRAM_START+0x19000
#else
#define Bank1start   NAND_ASRAM_START+0x3200
#define Bank2start   NAND_ASRAM_START+0x6400 //200k for code
#define Bank3start   NAND_ASRAM_START+0x100000
#endif  

#else  

#define Bank1start           DDRBASE + 0x00900000 //1Mb for code 
#define Bank2start           Bank1start+0x32000    //200 K  workbase 
#define Bank3start           Bank1start+0x200000  // 2MB-200k for buffers and uncached area starts from here  0x80b00000
//Bank3ends at 32mb we have ptables at the top of bank3
#define Bank4start           DDRBASE+0x2000000  //32 mb onwards 
#endif

//NOTE top 32k is for MMU PAGE TABLE 0x09ff8000 to 0x0a000000  
//Work bas eoffsets
#define TXPTR			0
#define CTXPTR		4
#define TXSIZE		8
#define CTXSIZE		0xC

#define RXPTR		0x10
#define CRXPTR		0x14
#define RXSIZE		0x18
#define CRXSIZE		0x1C



#define SystemWorkBase       Bank1start	

#ifdef SPI_BOOT
#define INTVectors           LMEMSTART + 0x00004000 // 0x04004000
#else
#define INTVectors           SystemWorkBase+0x400
#endif

#define AbortStackBase       SystemWorkBase+0x800
#define UndefStackBase       SystemWorkBase+0xC00
#define SVCStackBase         SystemWorkBase+0x1000
#define FIQStackBase         SystemWorkBase+0x1400
#define IRQStackBase         SystemWorkBase+0x1800
#define UserStackBase        SystemWorkBase+0x1C00  


#define UART1WorkBase        SystemWorkBase+0x2000
#define UART2WorkBase        SystemWorkBase+0x2400
#define GPIOWorkBase         SystemWorkBase+0x2800	
#define INTWorkBase          SystemWorkBase+0x2C00
#define KBDWorkBase          SystemWorkBase+0x3000
//#define UFCWorkBase          SystemWorkBase+0x3400
#define MAC1WORKBASE	     SystemWorkBase+0x3400    
#define MAC2WORKBASE		SystemWorkBase+0x3c00  

#define I2C1WorkBase         (SystemWorkBase+0x4000)      
#define I2C2WorkBase         (SystemWorkBase+0x4400) 
#define I2C3WorkBase         (SystemWorkBase+0x4800) 
#define I2C4WorkBase         (SystemWorkBase+0x4C00) 
#define I2C5WorkBase         (SystemWorkBase+0x5000) 
#define I2C6WorkBase         (SystemWorkBase+0x5400) 
#define FGEEWorkBase	     (SystemWorkBase+0x5800)
#define FGBSecWorkBase	     (SystemWorkBase+0x8000)
#define BMCWDTWorkBase	     (SystemWorkBase+0x9800)
#define PS2WorkBase	     (SystemWorkBase+0x9840)
#define UART3WorkBase        (SystemWorkBase+0x98C0)
#define UART4WorkBase        (SystemWorkBase+0x9940)
#define UART5WorkBase        (SystemWorkBase+0x99C0)
#define MBWorkBase	     (SystemWorkBase+0x9A40)
#define KCS1WorkBase	     (SystemWorkBase+0x9B40)
#define KCS2WorkBase	     (SystemWorkBase+0x9B80)
#define KCS3WorkBase	     (SystemWorkBase+0x9C00)
#define KCS4WorkBase	     (SystemWorkBase+0x9C40)
#define KCS5WorkBase	     (SystemWorkBase+0x9C80)
#define SMICWorkBase	     (SystemWorkBase+0x9CC0)
#define BTWorkBase	     (SystemWorkBase+0x9D40)
#define UART6WorkBase        (SystemWorkBase+0x9DC0)
#define UART7WorkBase        (SystemWorkBase+0x9E40)
#define PSRWorkBase          (SystemWorkBase+0x9EC0)
#define RTCWorkBase          (SystemWorkBase+0x9F40)
#define FTWorkBase           (SystemWorkBase+0xA0C0) 
#define SWCWorkBase          (SystemWorkBase+0xA040)
#define Timer1WorkBase	     (SystemWorkBase+0xA0C0)
#define Timer2WorkBase	     (SystemWorkBase+0xA140)
#define Timer3WorkBase	     (SystemWorkBase+0xA1C0)
#define SPIWorkBase	     (SystemWorkBase+0xA240)
#define SMBWorkBase	     (SystemWorkBase+0xA340)
#define RTCIntCount	     (SystemWorkBase+0xA400)
#define WdogIntCount	     (SystemWorkBase+0xA480)
#define GPIO0IntCount	     (SystemWorkBase+0xA484)
#define RTCMatchValue	     (SystemWorkBase+0xA488)
#define KCS1INTCount	     (SystemWorkBase+0xA48C)
#define KCS3INTCount	     (SystemWorkBase+0xA490)
#define KCS4INTCount	     (SystemWorkBase+0xA494)
#define KCS5INTCount	     (SystemWorkBase+0xA498)
#define SMICINTCount	     (SystemWorkBase+0xA49C)
#define BTINTCount	     (SystemWorkBase+0xA4A0)
#define HSPIWorkBase	     (SystemWorkBase+0xA500)

#define BomberWorkBase	     (SystemWorkBase+0xA540)
#define PeciWorkBase	     (SystemWorkBase+0xA560)


#define SDHCWorkBase_START   (SystemWorkBase+0xA570) 
#define SDHCWorkBase_END     (SystemWorkBase+0xA5C0)
#define AHB2PCIWorkBase      (SystemWorkBase+0xA5D0)
#define NANDWorkBase_Start   (SystemWorkBase+0xAA00)
#define NANDWorkBase_End     (SystemWorkBase+0xFA00)

#define Timer4WorkBase	     (SystemWorkBase+0xFA00) 
#define Timer5WorkBase	     (SystemWorkBase+0xFA80)
#define Timer6WorkBase	     (SystemWorkBase+0xFB00)
 
#define I2C7WorkBase         (SystemWorkBase+0xFB80)  
#define I2C8WorkBase         (SystemWorkBase+0xFF80)
  
#define I2C9WorkBase         (SystemWorkBase+0xFB80)  
#define I2C10WorkBase         (SystemWorkBase+0xFF80)

#define PCIE_FUNCTION1_Start (SystemWorkBase+0x11000) 
#define PCIE_FUNCTION1_End   (SystemWorkBase+0x12000)

#define PINMUX_WORKAREA      (SystemWorkBase+0x12000)
#define PINMUX_WORKAREA_END  (SystemWorkBase+0x12800)  

#define MBINTFLAG     		    MBWorkBase+0x100
#define I2CConfigTable       I2C1WorkBase+0x0
#define I2COperationTable    I2C1WorkBase+0x80

#define GPIOConfigTable      GPIOWorkBase+0
#define GPIOOperationTable   GPIOWorkBase+0x180


#define UART1ConfigTable     UART1WorkBase+0
#define UART1OperationTable  UART1WorkBase+0x40

#define UART2ConfigTable     UART2WorkBase+0
#define UART2OperationTable  UART2WorkBase+0x40
  
  
 


//Buffers
#define UART1XmitBuffer             (Bank2start)
#define UART1RecvBuffer             (Bank2start+0x800) 

#define UART2XmitBuffer             (Bank2start+0x1000) 
#define UART2RecvBuffer             (Bank2start+0x1800)

#define KBDXmitBuffer               (Bank2start+0x2000)
#define KBDRecvBuffer               (Bank2start+0x2800)

#define I2C1XmitBuffer              (Bank2start+0x3000)
#define I2C1RecvBuffer              (Bank2start+0x3800)

#define I2C2XmitBuffer              (Bank2start+0x4000)
#define I2C2RecvBuffer              (Bank2start+0x4800)

#define I2C3XmitBuffer              (Bank2start+0x5000)
#define I2C3RecvBuffer              (Bank2start+0x5800)

#define I2C4XmitBuffer              (Bank2start+0x6000)
#define I2C4RecvBuffer              (Bank2start+0x6800)
#define I2C_CMDREAD_ENQ             (Bank2start+0x6FF8) //samar

#define I2C5XmitBuffer              (Bank2start+0x7000)
#define I2C5RecvBuffer              (Bank2start+0x7800)
 
#define I2C6XmitBuffer              (Bank2start+0x8000)
#define I2C6RecvBuffer              (Bank2start+0x8800)


#define PS2XmitBuffer               (Bank2start+0x9000)
#define PS2RecvBuffer               (Bank2start+0x9800)

#define UART3XmitBuffer             (Bank2start+0xA000)
#define UART3RecvBuffer             (Bank2start+0xA800)

#define UART4XmitBuffer             (Bank2start+0xB000)
#define UART4RecvBuffer             (Bank2start+0xB800)

#define MBXmitBuffer         	    (Bank2start+0xC000)
#define MBRecvBuffer         	    (Bank2start+0xC800)

#define KCS1XmitBuffer		    (Bank2start+0xD000)
#define KCS1RecvBuffer		    (Bank2start+0xD800)
	
#define KCS2XmitBuffer		    (Bank2start+0xE000)
#define KCS2RecvBuffer		    (Bank2start+0xE800)

#define KCS3XmitBuffer		    (Bank2start+0xF000)
#define KCS3RecvBuffer		    (Bank2start+0xF800) 

#define KCS4XmitBuffer		    (Bank2start+0x20000) 
#define KCS4RecvBuffer		    (Bank2start+0x20800)

#define KCS5XmitBuffer		    (Bank2start+0x21000)
#define KCS5RecvBuffer		    (Bank2start+0x21800)

#define SMICXmitBuffer		    (Bank2start+0x22000)
#define SMICRecvBuffer		    (Bank2start+0x22800)

#define BTXmitBuffer		    (Bank2start+0x23000)
#define BTRecvBuffer		    (Bank2start+0x23800)

#define UART5XmitBuffer             (Bank2start+0x24000)
#define UART5RecvBuffer             (Bank2start+0x24800)

#define UART6XmitBuffer             (Bank2start+0x25000)
#define UART6RecvBuffer             (Bank2start+0x25800)

#define UART7XmitBuffer             (Bank2start+0x26000)
#define UART7RecvBuffer             (Bank2start+0x26800)

#define PSRXmitBuffer             Bank2start+0x27000
#define PSRRecvBuffer             Bank2start+0x27800

#define RTCRecvBuffer             Bank2start+0x28800

#define SPIXmitBuffer             Bank2start+0x29000
#define SPIRecvBuffer             Bank2start+0x29800

#define SMBXmitBuffer             Bank2start+0x2A000
#define SMBRecvBuffer             Bank2start+0x2A800

#define USBXmitBuffer               Bank2start+0x2B000	
#define USBRecvBuffer               Bank2start+0x2C000

#define MAC1XmitBuffer               Bank2start+0x2D000	
#define MAC1RecvBuffer               Bank2start+0x2f000

#define MAC2XmitBuffer               Bank2start+0x31000	
#define MAC2RecvBuffer               Bank2start+0x33000

#define FGEEXmitBuffer               Bank2start+0x35000
#define FGEERecvBuffer               Bank2start+0x38800

#define SEC_ENCBUFF_START           Bank2start+0x35000
#define SEC_DECBUFF_START           Bank2start+0x38800

#define RCEXmitBuffer               Bank2start+0x39000
#define RCERecvBuffer               Bank2start+0x3a000
#define RCEWorkBase	      	    Bank2start+0x3b000

#define TFEXmitBuffer               Bank2start+0x3c000
#define TFERecvBuffer               Bank2start+0x4c000
#define TFEWorkBase	      	    Bank2start+0x5c000

#if defined (CODE_IN_ASRAM) && defined (HW_ENV)
#define USB2XmitBuffer              0x10000000	
#define USB2RecvBuffer              0x10004000	
#else
#define USB2XmitBuffer              Bank2start+0x60000	
#define USB2RecvBuffer              Bank2start+0x65000
#endif

  
  
#define USB2RecvBufferEND           Bank2start+0x6D000

#define SSPTXMEM                    Bank2start+0x6D000
#define SSPRXMEM                    Bank2start+0x6E000

#define SSPEND                    Bank2start+0x78400    

#define ARM_TEST_START		  Bank2start+0x78800
#define ARM_TEST_END		  Bank2start+0x79800


#define SDHC_START                Bank2start+0x79800
#define SDHC_END                  Bank2start+0xF9800
  
#define I2C7XmitBuffer               Bank2start+0xFA000
#define I2C7RecvBuffer               Bank2start+0xFA800
 
#define I2C8XmitBuffer               Bank2start+0xFB000
#define I2C8RecvBuffer               Bank2start+0xFB800

#define LAST_BANK2_ADDR              Bank2start+0xFC000


//Uncached area starts from here
#if defined (CODE_IN_ASRAM)
#define SYSTEST_WORKBASE_START          (Bank3start)            
#define SYSTEST_SSP_CODE_WORKAREA       (SYSTEST_WORKBASE_START)
#define SYSTEST_SSP_CODE_WORKAREA_END   (SYSTEST_WORKBASE_START +0xc0000)
#define SYSTEST_BSE_WORKBASE            (SYSTEST_WORKBASE_START) 
#define SYSTEST_LDMA_WORKBASE           (SYSTEST_WORKBASE_START) 
#define SYSTEST_GDMA_WORKBASE           (SYSTEST_WORKBASE_START) 
#define SYSTEST_TFE_WORKBASE            (SYSTEST_WORKBASE_START) 
#define SYSTEST_MAC1_WORKBASE           (SYSTEST_WORKBASE_START) 
#define SYSTEST_MAC2_WORKBASE           (SYSTEST_WORKBASE_START) 
#define SYSTEST_USB1_WORKBASE           (SYSTEST_WORKBASE_START) 
#define SYSTEST_SPITEST_WORKBASE        (SYSTEST_WORKBASE_START) 
#define SYSTEST_ARMUCST_WORKBASE        (SYSTEST_WORKBASE_START) 
#define SYSTEST_ARMUCEND_WORKBASE       (SYSTEST_WORKBASE_START) 
#define SYSTEST_MEMTEST_WORKBASE        (SYSTEST_WORKBASE_START) 
#define SYSTEST_MEMTESTEND_WORKBASE     (SYSTEST_WORKBASE_START) 
#define SYSTEST_USB2_WORKBASE           (SYSTEST_WORKBASE_START) 
#define SYSTEST_BOMBER_WORKAREA         (SYSTEST_WORKBASE_START) 
#define SYSTEST_XDMA_WORKAREA           (SYSTEST_WORKBASE_START) 
#define SYSTEST_SDHC_UC_WORKAREA        (SYSTEST_WORKBASE_START) 
#define SYSTEST_BSE_DPTR_WORKAREA       (SYSTEST_WORKBASE_START) 
#define SYSTEST_NAND_WORKAREA           (SYSTEST_WORKBASE_START) 
#define SYSTEST_PCIE_SECOND_FUNC        (SYSTEST_WORKBASE_START)
#define SYSTEST_SPITEST_WORKBASE        (SYSTEST_WORKBASE_START)
#define SYSTEST_SPITEST_WORKBASE_END    (SYSTEST_WORKBASE_START)

#define NEXT_GUYS_DDR_START_ADDR        (SYSTEST_WORKBASE_START)
#define SYSTEST_WORKBASE_END            (SYSTEST_WORKBASE_START + 0xc0000)

#else
  
#define SYSTEST_WORKBASE_START          (Bank3start)             //0x80B0_0000
#define SYSTEST_SSP_CODE_WORKAREA       (SYSTEST_WORKBASE_START)
#define SYSTEST_SSP_CODE_WORKAREA_END   (SYSTEST_WORKBASE_START + 0x00200000) 
#define SYSTEST_BSE_WORKBASE            (SYSTEST_WORKBASE_START + 0x00200000) 
#define SYSTEST_LDMA_WORKBASE           (SYSTEST_WORKBASE_START + 0x00A00000)
#define SYSTEST_GDMA_WORKBASE           (SYSTEST_WORKBASE_START + 0x00A80000)
#define SYSTEST_TFE_WORKBASE            (SYSTEST_WORKBASE_START + 0x00B00000)
#define SYSTEST_MAC1_WORKBASE           (SYSTEST_WORKBASE_START + 0x00C00000)
#define SYSTEST_MAC2_WORKBASE           (SYSTEST_WORKBASE_START + 0x00D00000)
#define SYSTEST_USB1_WORKBASE           (SYSTEST_WORKBASE_START + 0x00E00000)
#define SYSTEST_ARMUCST_WORKBASE        (SYSTEST_WORKBASE_START + 0x00E02000)
#define SYSTEST_ARMUCEND_WORKBASE       (SYSTEST_WORKBASE_START + 0x00E04000)      
#define SYSTEST_MEMTEST_WORKBASE        (SYSTEST_WORKBASE_START + 0x00E05800)
#define SYSTEST_MEMTESTEND_WORKBASE     (SYSTEST_WORKBASE_START + 0x00E06000)      
#define SYSTEST_USB2_WORKBASE           (SYSTEST_WORKBASE_START + 0x00E80000)
#define SYSTEST_BOMBER_WORKAREA         (SYSTEST_WORKBASE_START + 0x00E90000)     
#define SYSTEST_XDMA_WORKAREA           (SYSTEST_WORKBASE_START + 0x00EA0000)
#define SYSTEST_SDHC_UC_WORKAREA        (SYSTEST_WORKBASE_START + 0x00EB0000)
#define SYSTEST_BSE_DPTR_WORKAREA       (SYSTEST_WORKBASE_START + 0x00FB0000)  // We need this
#define SYSTEST_NAND_WORKAREA           (SYSTEST_WORKBASE_START + 0x00FB1000)
#define SYSTEST_SPITEST_WORKBASE        (SYSTEST_WORKBASE_START + 0x00FD1000)
#define SYSTEST_SPITEST_WORKBASE_END    (SYSTEST_WORKBASE_START + 0x00FD4000)


#define SYSTEST_WORKBASE_END            (SYSTEST_WORKBASE_START + 0x014f8000) //=base+0x1ff8000
//note if we reach "SYSTEST_WORKBASE_END" start using bank4 as defined above
  
#endif

#define MISC_WORKBASE_START                        	SYSTEST_WORKBASE_END    
#define SYSTEST_MODEL PARAM                        	MISC_WORKBASE_START  
#define SYSTEST_BSE_MODEL_PARAM         		MISC_WORKBASE_START
#define SYSTEST_VERILOG_MEM_CMP         		(MISC_WORKBASE_START + 0x40) 
#define VERILOG_TASK_INIT_PATTERN_PARAMS       		(MISC_WORKBASE_START + 0x80) 
#define VERILOG_TASK_RLE_PARAMS         		(MISC_WORKBASE_START + 0xc0)
#define VERILOG_TASK_TFE_INIT           		(MISC_WORKBASE_START + 0x100)

#define DDR_WRITE_SIMULATOR_FLAG        		(MISC_WORKBASE_START + 0x140)
#define DDR_WRITE_SIMULATOR_OPCODE      		(MISC_WORKBASE_START + 0x150)
#define DDR_PRINT_STRING_BASE           		(MISC_WORKBASE_START + 0x160)
#define VERILOG_CALC_CHKSUM             		(MISC_WORKBASE_START + 0x200)
#define VERILOG_RANDOM_NO               		(MISC_WORKBASE_START + 0x220)
#define LFSR                            		(MISC_WORKBASE_START + 0x23C)
#define VERILOG_TASK_SDHC               		(MISC_WORKBASE_START + 0x250)           
#define VERILOG_TASK_SDHC_END           		(MISC_WORKBASE_START + 0x290)

#define VERILOG_SDHC_STD_DMA_CPQ_CNT                    (MISC_WORKBASE_START + 0x2A0)
#define VERILOG_SDHC_STD_DMA_CPQ_CNT_END                (MISC_WORKBASE_START + 0x2C0)

#define MONITOR_WORKAREA                		(MISC_WORKBASE_START + 0x2C00)

#define DDRINTFLAG                                      (MISC_WORKBASE_START + 0x2D00)
#define DDRERR_STS                                      (MISC_WORKBASE_START + 0x2E80)
#define SYSTEST_GVARS                   		(MISC_WORKBASE_START + 0x3000)
#define MISC_WORKBASE_ENDS              		(MISC_WORKBASE_START + 0x3ffc)   //DDR_BASE + 0x01ffbfff

//Note that there is no memory available below   MISC_WORKBASE_ENDS for s/w.
  
  
#if defined (CODE_IN_ASRAM)
#if defined (HW_ENV)
#define PTABLEINDDR    (NAND_ASRAM_START + 0x1c000)
#else
#define PTABLEINDDR    (NAND_ASRAM_START + 0x01fc000)
#endif
#else  
#define PTABLEINDDR     (DDRBASE + 0x01ffc000) // 0x89ffc000==32MB-16K, //last 16 k is for page table
#endif  //(CODE_IN_ASRAM)



#define SYSTEST_PCIE_SECOND_FUNC        Bank4start
#define SYSTEST_PCIE_SECOND_FUNC_END    (SYSTEST_PCIE_SECOND_FUNC+0x300000)




//following defines should be moved out of this file  

//System level TFE SPACE
#define TFEDPTRS_DDR  SYSTEST_TFE_WORKBASE                        //dhanraj,this should be removed
#define SYSTEST_TFEXmitBuffer   SYSTEST_TFE_WORKBASE+0x800       //dhanraj,this should be removed
#define SYSTEST_TFERecvBuffer   SYSTEST_TFE_WORKBASE+0x00080000  //dhanraj,this should be removed

// USB2 & USB1 Workbase
//#define USB2WorkBase            SYSTEST_USB2_WORKBASE  //dhanraj,this should be removed

#define USB2WorkBase            SYSTEST_USB2_WORKBASE  //dhanraj,this should be removed  
#define USB1WorkBase            SYSTEST_USB1_WORKBASE  //dhanraj,this should be removed

#define FGB_DMA_WORKBASE            SYSTEST_LDMA_WORKBASE     ////saurabh,this should be removed


//following 3 defines should be moved out of here
#define SEC_GDMA_WORKBASE         SYSTEST_GDMA_WORKBASE    ////saurabh,this should be removed
//#define FGB_TSE_WORKBASE            Bank3start + 0x4000  ////saurabh,this should be removed



/////////////////// Verilog Mem Compare task //////////
#define  VERILOG_MEMCMP_PARAM   SYSTEST_VERILOG_MEM_CMP 
#define  VMP_SRCADDR      0x0
#define  VMP_DESTADDR     0x4
#define  VMP_LENGTH       0x8
#define  VMP_BLOCK        0xc
#define  VMP_LOOPNO       0x10
#define  VMP_SRCSTRIDE    0x14
#define  VMP_DESTSTRIDE   0x18
#define  VMP_HEIGHT       0x1C
#define  VMP_RESULT       0x2c

#define  VERILOG_CHKSUM_PARAM   VERILOG_CALC_CHKSUM 
#define  VCC_SRCADDR      0x0
#define  VCC_LENGTH       0x4
#define  VCC_SEED         0x8
#define  VCC_RESULT       0xC
#define  VCC_CHKSUM       0x10

#define  VERILOG_RANDOM_NO_PARAM VERILOG_RANDOM_NO
#define  VRN_LFSR	  0x0
#define  VRN_NO_OF_BITS	  0x4
#define  VRN_RND_NO	  0x8
#define  VRN_LFSR_DATA	  0xC
#define  VRN_RESULT	  0x10
  




/////////////////////////////////////////////////////////////////
/////////////////// LMEM DEFINES FOR SYSTEST ////////////////////
/////////////////////////////////////////////////////////////////
//Following are mem allocation for systests only ,standalone tests can take what ever mem
//they want from LMEMSTART base and define appropriatley in corresponding modules .h files

  
#define SYSTEST_SSP_LMEM_IMEM_START  (LMEMSTART+0x0)  //8k
#define SYSTEST_SSP_LMEM_DMEM_START  (LMEMSTART+0x2000)
  
#define SYSTEST_SSP_LMEM_DMEM_END    (LMEMSTART+0x2fff) //4k
  
#define SYSTEST_LDMA_LMEM_WORKBASE   (LMEMSTART+0x3000) //2k
  
#define SYSTEST_GDMA_LMEM_WORKBASE   (LMEMSTART + 0x3800) //2k
  
#define SYSTEST_NAND_LMEM_START      (LMEMSTART+0x4000) //2.5k
#define SYSTEST_NAND_LMEM_END        (LMEMSTART+0x49ff)
  
#define SYSTEST_TFE_LMEM_START       (LMEMSTART+0x4A00) //1.5k
#define SYSTEST_TFE_LMEM_END         (LMEMSTART+0x4fff)
  
#define SYSTEST_BSE_LMEM_START      (LMEMSTART+0x5000) //4k
#define SYSTEST_BSE_LMEM_END       (LMEMSTART+0x5fff)
  
#define SYSTEST_BOMBERLMEM_START     (LMEMSTART+0x6000) //4k
#define SYSTEST_BOMBERLMEM_END       (LMEMSTART+0x6fff)
  
#define SYSTEST_SDHCLMEM_START     (LMEMSTART+0x7000) //2k
#define SYSTEST_SDHCLMEM_END       (LMEMSTART+0x77ff)
  
#define SYSTEST_PCIE_LMEM_START     (LMEMSTART+0x7800)   //2k
#define SYSTEST_PCIE_LMEM_END       (LMEMSTART+0x7fff)

//these are used inly in ddr low power test from boot code option 8
#define DDR_CLK_SW_LMEM_LOW           (LMEMSTART+0x7000)
#define DDR_CLK_SW_LMEM_HIGH          (LMEMSTART+0x7800)
//#define LSI_FIFO_TRAINING   (0x10007c00)  //lmem used under ddr.h

  
  

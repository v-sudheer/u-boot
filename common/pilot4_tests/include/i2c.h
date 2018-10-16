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
//* Purpose:
//* This file contains all of the I2C related definitions. 
//*****************************************************************************
#include "buffer.h"
#define IC_CON			0
#define IC_TAR			4
#define IC_SAR			8
#define IC_HS_MADDR		0X0C
#define IC_DATA_CMD		0X10
#define IC_SS_SCL_HCNT		0X14
#define IC_SS_SCL_LCNT		0X18
#define IC_FS_SCL_HCNT		0X1C
#define IC_FS_SCL_LCNT		0X20
#define IC_HS_SCL_HCNT		0X24
#define IC_HS_SCL_LCNT		0X28
#define IC_INTR_STAT		0X2C
#define IC_INTR_MASK		0X30
#define IC_RAW_INTR_STAT		0X34
#define IC_RX_TL			0X38
#define IC_TX_TL			0X3C
#define IC_CLR_INTR		0X40
#define IC_CLR_RX_UNDER		0X44
#define IC_CLR_RX_OVER		0X48
#define IC_CLR_TX_OVER		0X4C
#define IC_CLR_RD_REQ		0X50
#define IC_CLR_TX_ABRT		0X54
#define IC_CLR_RX_DONE		0X58
#define IC_CLR_ACTIVITY		0X5C
#define IC_CLR_STOP_DET		0X60
#define IC_CLR_START_DET		0X64
#define IC_CLR_GEN_CALL		0X68
#define IC_ENABLE		0X6C
#define IC_STATUS		0X70
#define IC_TXFLR			0X74
#define IC_RXFLR			0X78
#define IC_SRESET		0X7C
#define IC_TX_ABORT_SOURCE 	0X80
#define IC_SDA_SETUP        0x94
#define IC_ACK_GENERAL_CALL 0x98
#define IC_ENABLE_STATUS    0x9C
#define IC_COMP_PARAM_1     0Xf4
#define IC_COMP_VERSION     0Xf8
#define IC_COMP_TYPE        0xFC
#define IC_CLR_RESTART_DET  0xA8


//	IC_CON		

#define MASTER_MODE			(1<<0)	//1=MASTER ENABLED ,0=MASTER DISABLED
#define SPEED				(3<<1)	//mask for speed mode
#define SPEED_100KB			(1<<1)	//100K Bits
#define SPEED_400KB			(2<<1)	//400K Bits
#define SPEED_34MB			(3<<1)	//3.4M Bits
#define IC_10BIT_ADDR_SLAVE		(1<<3)	//1=10 BIT ADDR SLAVE
#define IC_10BIT_ADDR_MASTER		(1<<4)	//1=10 BIT ADDR MASTER
#define IC_RESTART_EN			(1<<5)	//
#define IC_SLAVE_DISABLE		(1<<6)	//0 =ENABLE 1=DISABLE
#define IC_STOP_DET_IFADDRESSED (1<<7)  //0 = DISABLE 1=ENABLE filter
#define IC_TAR_ADDR			(0x3ff)	//TARGET ADDR field for master transactions		
#define GC_OR_START			(1<<10)
#define SPECIAL				(1<<11) //0 =IGNORE BIT 10
#define IC_10B_MASTER       (1<<12)
#define IC_RX_FF_HLD_CTL    (1<<9)

#define STOP_BIT            (1 << 9)
#define RESTART_BIT         (1 << 10)


#define DAT				0XFF	//DATA FIELD
#define CMDREAD				1<<8  //1=READ,0=WRITE
#define CMDWRITE				0<<8
//	IC_INTR_STAT,IC_INTR_MASK,IC_INTR_STAT
#define RX_UNDER				1<<0
#define RX_OVER				1<<1
#define RX_FULL				1<<2
#define TX_OVER				1<<3
#define TX_EMPTY				1<<4
#define RD_REQ				1<<5
#define TX_ABRT				1<<6
#define RX_DONE				1<<7
#define ACTIVITY				1<<8
#define STOP_DET				1<<9
#define START_DET			1<<10
#define GEN_CALL				1<<11
#define RESTART_DET         1<<12
#define MST_ON_HOLD         1<<13

//	IC_ENABLE
#define ENABLEI2C			1<<0		//1=enable  0 =Disable
#define DISABLEI2C                      0
#define ABORTI2C            1<<1
//	IC_STATUS

#define IC_STATUS_ACTIVITY		1<<0
#define TFNF				1<<1		//Transmit FIFO Not Full
#define TFE				1<<2		//Transmit FIFO empty
#define RFNE				1<<3	// Receive FIFO Not Empty 
#define RFF				1<<4		// Receive FIFO Completely Full


//	IC_SRESET

#define SRESET_ALL			1<<0		//Soft reset all
#define SRESET_MASTER			1<<1		//Soft reset Master machines
#define SRESET_SLAVE			1<<2		//Soft reset Slave machines

//	IC_TX_ABRT_SOURCE

#define ABRT_7B_ADDR_NOACK		1<<0
#define ABRT_10ADDR1_NOACK		1<<1
#define ABRT_10ADDR2_NOACK		1<<2


#define ABRT_TXDATA_NOACK			1<<3
#define ABRT_GCALL_NOACK			1<<4
#define ABRT_GCALL_READ			1<<5

#define ABRT_HS_ACKDET			1<<6
#define ABRT_SBYTE_ACKDET			1<<7

#define ABRT_HS_NORSTRT			1<<8
#define ABRT_SBYTE_NORSTRT		1<<9
#define ABRT_10B_RD_NORSTRT		1<<10




#define ABRT_MASTER_DIS			1<<11

#define ABRT_LOST			1<<12

#define ABRT_SLVFUSH_TXFIFO		1<<13
#define ABRT_SLV_ARBLOST			1<<14
#define ABRT_SLVRD_INTX			1<<15
#define ABRT_USER_ABRT          1<<16

//;IC_COMP_PARAM_1

#define RX_BUFFER_DEPTH			8	// 15:8
#define TX_BUFFER_DEPTH			16	// 23:16


#define I2CTXPTR			0
#define I2CTXSIZE		4
#define I2CCTXPTR		8
#define I2CCTXSIZE		0xC
#define I2CRXPTR		0x10
#define I2CRXSIZE		0x14
#define I2CCRXPTR		0x18
#define I2CCRXSIZE		0x1C
#define I2CFLAG         0x20
#define I2CABRTSRC      0x24

// I2CFLAG bits
#define RESTART_FLAG       (1 << 0)
#define STOP_RECEIVED_FLAG (1 << 1)
#define TXABRT_RCVD_FLAG   (1 << 2)
#define RESTART_RCVD_FLAG  (1 << 3)
#define FORCE_RESTART_FLAG (1 << 4)
#define RXUNDER_RCVD_FLAG  (1 << 5)
#define RXOVER_RCVD_FLAG   (1 << 6)
#define TXOVER_RCVD_FLAG   (1 << 7)
#define RXDONE_RCVD_FLAG   (1 << 8)
#define TFNF_NOT_SET_FLAG  (1 << 9)
#define MSTHLD_RCVD_FLAG   (1 << 10)

#define PCT_BBANG_EN    (1 << 0)
#define PCT_CLK_OUT     (1 << 1)
#define PCT_CLK_IN      (1 << 2)
#define PCT_DAT_OUT     (1 << 5)
#define PCT_DAT_IN      (1 << 6)

// Change it to 10 later
#define MAX_I2CS_P4     8

//I2C Common file function prototypes
void            I2C_WRITEREG_WORD(volatile unsigned int, volatile unsigned int, volatile unsigned int);
void            I2C_WRITEREG_BYTE(volatile unsigned int, volatile unsigned int, volatile unsigned char);
void            RMI2C_WRITEREG_WORD(volatile unsigned int, volatile unsigned int, volatile unsigned int);
void            I2C_RMODWRREG_BYTE(volatile unsigned int, volatile unsigned int, volatile unsigned char);
unsigned int    I2C_READREGWORD(volatile unsigned int, volatile unsigned int);
unsigned char   I2C_READREGBYTE(volatile unsigned int, volatile unsigned int);
void            i2cEnable(unsigned int);
void            I2CDisableInt(unsigned int, unsigned char);
void            i2c1INThandler(void);
void            i2c2INThandler(void);
void            i2c3INThandler(void);
void            i2c4INThandler(void);
void            i2c5INThandler(void);
void            i2c6INThandler(void);
void            i2cCommonIntHandler(unsigned int, unsigned int);
void            i2cRXFULL_handler(volatile unsigned int, volatile unsigned int);
void            i2cTXEMPTY_handler(volatile unsigned int, volatile unsigned int);
void            i2cRD_REQ_handler(volatile unsigned int, volatile unsigned int);
void            I2C_CONFIGURE(volatile unsigned int, unsigned char, unsigned char, unsigned char);
void            i2c_swreset(void);
void            combo15inthandler(void) ;
tU8             I2C_REGREAD_COMPAREWORD(volatile unsigned int offset, volatile unsigned int ActualData, volatile unsigned int ExpData);
void            I2CSetupC(tU32 I2CBASE_ADD, tU32 * regarr);
void            i2cSendData(tU32 I2Cbase_addr, tU32 Buffer_addr, tU32 count);
void            i2cRecvData(tU32 I2Cbase_addr, tU32 Buffer_addr, tU32 count); 
void            i2cReqReadData(tU32 I2Cbase_addr, tU32 offset_addr, tU32 count);
void            i2cWAIT4READREQ(tU32 I2Cbase_addr, tU32 Buffer_addr, tU32 count);
void            InitWorkAreaC(volatile unsigned int WorkBaseAddr, volatile unsigned int Tx_Buffer, volatile unsigned int ToSend, volatile unsigned int Rx_Buffer, volatile unsigned int ToRecv);
void            I2CInitC(volatile unsigned int i2cint, volatile unsigned int handleraddr);

void            I2C1EnableIntC(volatile unsigned int intr_enable_bits);
void            I2C2EnableIntC(volatile unsigned int intr_enable_bits);

void            I2C3EnableIntC(volatile unsigned int intr_enable_bits);
void            I2C4EnableIntC(volatile unsigned int intr_enable_bits);

void            I2C5EnableIntC(volatile unsigned int intr_enable_bits);
void            I2C6EnableIntC(volatile unsigned int intr_enable_bits);

void            I2C7EnableIntC(volatile unsigned int intr_enable_bits);
void            I2C8EnableIntC(volatile unsigned int intr_enable_bits);
void            I2CEnableIntC( tU32 i2c_base, tU32 i2cintr_num, tU32 intr_enable_bits);
void            INITBuffer_I2C(tPVU32 Addrs,tU32 initial,tU32 incrval,tU32 count);

// Defines for the new core
void            i2cEnable_p4(unsigned int);
void            i2cDisable_p4(unsigned int);
void            i2c1INThandler_p4(void);
void            i2c2INThandler_p4(void);
void            i2c3INThandler_p4(void);
void            i2c4INThandler_p4(void);
void            i2c5INThandler_p4(void);
void            i2c6INThandler_p4(void);
void            combo15inthandler_p4(void) ;
void            i2cCommonIntHandler_p4(unsigned int, unsigned int);
void            i2cRXFULL_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cTXEMPTY_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cRD_REQ_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cRESTART_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cSTOP_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cTXABORT_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cRXUNDER_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cRXOVER_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cTXOVER_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cRXDONE_handler_p4(volatile unsigned int, volatile unsigned int);
void            i2cMSTHLD_handler_p4(volatile unsigned int, volatile unsigned int);
void            I2C_CONFIGURE_p4(volatile unsigned int, unsigned char, unsigned char, unsigned char);
int             I2C_Check_Final_Values(tU32 * regarr);
void            reset_i2c(tVU32 i2c_num);

  
//BIT-BANG
#define 	HIGH			0x01	
#define		LOW			0x00	



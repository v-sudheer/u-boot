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

// uncomment one of these to compile for that particualr board
#define P2SPIREGBASE  0x40200000
#define P2SPIMEMBASE  0x02000000
#define P2UART3BASE   0x43000000 
  
#define spi_base     	P2SPIREGBASE   
#define SPI_MEMSTART 	P2SPIMEMBASE  

#define SPI_ADDR     (0x00)
#define SPI_RWDATA   (0x30)
#define SPI_CMD0     (0x08)  //holds opcode
#define SPI_CMD1     (0x09)  //cmd len,dir,no of bytes
#define SPI_CMD2     (0x0a)  //custom opcode
#define SPI_CMD3     (0x0b)  //control reg
#define SPI_TREC     (0x0c)
#define SPI_TCS      (0x0d)
#define SPI_TCSL     (0x0e)
#define SPI_TCSH     (0x0f)
#define SPI_CTRL     (0x10)
#define SPI_CLKDIV   (0x14)
#define SPI_STS      (0x18)
#define SPI_MISC     (0x1c)
#define SPI_INTEN    (0x20)

//spi cmd 1 reg bit defines
#define cmdwrite    (1<<4)
#define cmdread     (0<<4)
#define cmdlen_0    (0<<0)
#define cmdlen_1    (1<<0)
#define cmdlen_2    (2<<0)
#define cmdlen_3    (3<<0)
#define cmdlen_4    (4<<0)
#define cmdlen_5    (5<<0)
#define cmdlen_6    (6<<0)

#define data_bytes_0  (0<<5)
#define data_bytes_1  (1<<5)
#define data_bytes_2  (2<<5)
#define data_bytes_3  (3<<5)
#define data_bytes_4  (4<<5)
#define data_bytes_5  (5<<5)
#define data_bytes_6  (6<<5)
#define data_bytes_7  (7<<5)
#define data_bytes_8  (8<<5) 
  
//spi cmd3 reg defines
#define spi_cs_high     (1<<0)
#define spi_cs_low      (0<<0)
#define wfifo_en        (1<<1)
#define fread_en        (1<<2)
#define regrw_en        (1<<7)

//spi control reg
#define spi_en       (1<<0)
#define clkphase_hi  (1<<1) 
#define clkpol_hi    (1<<2)

//spi status register
#define spi_busy                (1<<0)
#define spi_int_busy            (1<<2)  
#define wfifo_en_sts            (1<<3)
#define wr_fifo_full            (1<<4)
#define rd_fifo_empty           (1<<5)
#define wr_fifo_empty           (1<<5)
#define spi_cmd_phase_done_sts  (1<<6)
#define spi_notbusy_sts         (1<<7)

//spi opcodes
#define wren   (0x6)   //write enable
#define wrdi   (0x4)   //write disable
#define rdsr   (0x5)   //read status register
#define wrsr   (0x1)   //write status register
#define spi_read_cmd   (0x3)   //read data
#define dualspi_read_cmd   (0x3b)   //read data  
#define fast_read  (0xb)  //read data at higher speed
#define pprog      (0x2)     //page pgrogramme(write data)
#define dualspi_pprog      (0xA2)     //page pgrogramme(write data)
#define serase     (0xd8)       //sector erase
#define berase    (0xc7)     //bulk or chip erase
#define deeopdwn   (0xb9)   //deep power down
#define relfrmpdwn  (0xab)   //release from power down
#define getid (0x9f)  //get device id

//spi status from flash
#define write_in_progress   (1<<0)    //write in progress
#define write_en_latchL   (1<<1)    //write en latch

// spi misc register
#define fast_read_en    (1<<3)

// SPI Interrrupt enable register
#define spi_ff_full_int_en              (1<<4)
#define spi_ff_empty_int_en             (1<<5)
#define spi_cmd_phase_done_int_en       (1<<6)
#define spi_not_busy_int_en             (1<<7)



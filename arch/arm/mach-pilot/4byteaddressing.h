/******************************************************************************
 * Copyright (C) 2018 Priyanjali K <priyanjali.karandi@aspeedtech.com>
 *
 * SPDX-License-Identifier:     GPL-2.0+
 *
 *****************************************************************************/
#ifndef __4BYTEADDRESSING_H__
#define __4BYTEADDRESSING_H__

typedef unsigned int              tU32;
typedef unsigned short            tU16;
typedef unsigned char             tU8;

typedef unsigned int *            tPU32;
typedef unsigned short *          tPU16;
typedef unsigned char *           tPU8;

typedef volatile unsigned int     tVU32;
typedef volatile unsigned short   tVU16;
typedef volatile unsigned char    tVU8;

typedef volatile unsigned int *   tPVU32;
typedef volatile unsigned short * tPVU16;
typedef volatile unsigned char *  tPVU8;


#define false 0
#define true 1
#define bool unsigned int
#define nc_printf printf
#define PRINT_FREQ 0x8000

/*READ ID, ERASE, READ, FWRITE, RGWRITE, STATUS1, STATUS2, ENTR4, EXIT4*/
#define FBYTE_SUPPORT 		0x00000001
#define STS_FLAGS_REG 		0x00000002
#define SEND_WR_ENA   		0x00000004
#define UNPROTECT_SECTORS	0x00000008
#define REG_RW			0x00000010

/*Error codes*/
/*Address is wrong or device does not support 4 Byte addressing */
#define ERR_ADDR_OR_SUPPORT	0x80000000 
#define ERR_NO_MATCH_ID		0x80000001
#define ERR_COMPARE_FALIED	0x80000002
#define ERR_4BYTE_ERASE_FAILED	0x80000003

extern tU32 bootspi_priv;
extern tU32 cs_addr[3];
struct spi_dat{
	tU8 uc_MFRID;
	tU8 uc_MTYPE;
	tU8 uc_MCAP;
	tU8 uc_UPADDR;
	tU8 auc_name[16];
	tU32 nofsec;
	//tU32 secsize;
	tU32 size;
	tU32 flags;
};
extern struct spi_dat id_table[];

#endif /*__4BYTEADDRESSING_H__*/

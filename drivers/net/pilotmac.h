/*
    Copyright (C) 2009-2011 American Megatrends Inc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
/*
    Copyright (c) 2010-2014, Emulex Corporation.

    Modifications made by Emulex  Corporation under the terms of the
    GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option)
    any later version.
*/


#ifndef _ASTMAC_H_
#define _ASTMAC_H

#define RXDES_NUM   		16
#define TXDES_NUM			8
#define RX_BUF_SIZE			1536
#define TX_BUF_SIZE			1536

typedef struct
{
	u32 Status;
	u32 Length;
	u32 Buffer1;
	u32 Buffer2;
} RECV_DESC;

typedef struct
{
	u32 Status;
	u32 Length;
	u32 Buffer1;
	u32 Buffer2;
} XMIT_DESC;



/*---------------------------------------------------------------------------------
 * 							Private Information of driver
 *---------------------------------------------------------------------------------*/
typedef struct mac100_local
{
	unsigned long 	hw_addr;				// IO Base Addr
	int 			devnum;					// Instance Number

	volatile RECV_DESC 	*rx_descs;			// receive ring base address
	volatile RECV_DESC	*rx_descs_dma;		// receive ring base physical address
	volatile char		*rx_buf;			// receive buffer cpu address
	volatile char		*rx_buf_dma;		// receive buffer physical address
	int					rx_idx;				// receive descriptor

	volatile XMIT_DESC 	*tx_descs;
	volatile XMIT_DESC	*tx_descs_dma;
	volatile char		*tx_buf;
	volatile char		*tx_buf_dma;
	int					tx_idx;

	volatile XMIT_DESC 	*setup_descs;
	volatile XMIT_DESC	*setup_descs_dma;
	volatile char		*setup_frame;
	volatile char		*setup_frame_dma;

} AST_ETH_PRIV_DATA;

#endif

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
/****************************************************************************************/
/*      This file contains packet related definitions                                   */
/*      Created : 06/26/2006                                                            */
/*      Author  : Vikram Appikatla                                                      */
/****************************************************************************************/


/* Header Lengths of each protocol */

#define         MAC_HEADER_LEN          14
#define         IP_HEADER_LEN           20
#define         ICMP_HEADER_LEN         8
#define         TCP_HEADER_LEN          20
#define         UDP_HEADER_LEN          20
#define 	ETHER_ADDR_LEN  	6

#define         MAX_ETHERNET_HEADER_LEN         (MAC_HEADER_LEN + IP_HEADER_LEN + TCP_HEADER_LEN)
#define         MAX_ETHERNET_FRAME_LEN          1518

#define         IP_ICMP                 0x01
#define         IP_UDP                  0x11
#define         IP_TCP                  0x06
#define         ICMP_ECHO_REQUEST       8
#define         ICMP_ECHO_REPLY         0


#define 	SWAP16(data)   		(short int) ( (((short int)data & 0x00FF) << 8) | (((short int)data & 0xFF00) >> 8) )
#define         SWAP32(data)            (tU32) (                               \
                                        (((tU32)data & 0x000000FF) << 24) |    \
                                        (((tU32)data & 0x0000FF00) << 8)  |    \
                                        (((tU32)data & 0x00FF0000) >> 8)  |    \
                                        (((tU32)data & 0xFF000000) >> 24) )


struct  _MAC_HEADER {
        char mac_dst_addr[ETHER_ADDR_LEN];
        char mac_src_addr[ETHER_ADDR_LEN];
        short int ether_type;
}__attribute__((aligned(1), packed));

typedef struct 	_MAC_HEADER      	MAC_HEADER;


struct _IP_HEADER {
        char ip_ver_hl;
        char ip_tos;
        short int ip_len;
        short int ip_id;
        short int ip_flags_off;
        char ip_ttl;
        char ip_proto;
        short int ip_chksum;
        int ip_src_addr;
        int ip_dst_addr;
}__attribute__((aligned(1), packed));

typedef struct 	_IP_HEADER       	IP_HEADER, *PIP_HEADER;

struct _ICMP_HEADER {
        char icmp_type;
        char icmp_code;
        short int icmp_chksum;
        short int icmp_id;
        short int icmp_seqnum;
}__attribute__((aligned(1), packed));

typedef struct 	_ICMP_HEADER     	ICMP_HEADER, *PICMP_HEADER;

struct _UDP_HEADER {
        short int udp_srcport;
        short int udp_dstport;
        short int udp_len;
        short int udp_chksum;

}__attribute__((aligned(1), packed));

typedef struct 	_UDP_HEADER      	UDP_HEADER, *PUDP_HEADER;

struct _TCP_HEADER {
        short int tcp_srcport;
        short int tcp_dstport;
        int tcp_seqnum;
        int tcp_acknum;
        char  tcp_hl_rsvd;
        char  tcp_flags;
        short int tcp_window;
        short int tcp_chksum;
        short int tcp_uptr;
}__attribute__((aligned(1), packed));

typedef struct 	_TCP_HEADER      	TCP_HEADER;

struct _IP_PSEUDO_HEADER {
	unsigned int ippseudo_srcaddr;
	unsigned int ippseudo_dstaddr;
	unsigned char ippseudo_pad;
	unsigned char ippseudo_proto;
	unsigned short ippseudo_len;
}__attribute__((aligned(1),packed));

typedef	struct _IP_PSEUDO_HEADER	IP_PSEUDO_HEADER, *PIP_PSEUDO_HEADER;

struct _ETHERNET_PACKET {
        MAC_HEADER      mac_hdr;
        IP_HEADER       ip_hdr;
        union {
                 TCP_HEADER     tcp_header;
                 UDP_HEADER     udp_header;
                 ICMP_HEADER    icmp_header;
        }ulp;

        char PktData[MAX_ETHERNET_FRAME_LEN - MAX_ETHERNET_HEADER_LEN];

}__attribute__((aligned(1), packed));

typedef struct 	_ETHERNET_PACKET 	ETHERNET_PACKET;


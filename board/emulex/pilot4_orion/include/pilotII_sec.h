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


/*************************************************************************************************
 * File name:  pilotII_sec.h
 *
 * Description:
 *             This file contains all the bit definitions and data structures related to 
*              hardware independent security block on pilot-II.
 *
 * Date Created: 
 *      08-16-2006
*
 * Date Modified:
 *      08-16-2006
 *
 * Changes:
 *     Initial Creation          ---  Murali  (08-16-2006)
 *
 *
************************************************************************************************/
#define MAX_ENCRYPT_KEY_LEN	32
#define MAX_AUTH_KEY_LEN        20
#define MAX_IV_LENGTH           16
#define MAX_IDIGEST_LEN         64
#define MAX_ODIGEST_LEN         64
#define MAX_AUTH_PAD_LEN        64
#define MAX_IPSEC_PAD_LEN       255
#define AUTH_DATA_LEN_ON_WIRE   12
#define MAX_NUM_SEC_CONTEXTS    4
#define MAX_NUM_HASH_ENTRIES    (MAX_NUM_SEC_CONTEXTS * 2)

#define P2_NULL_ENCRYPT         0
#define P2_DES_CBC              1
#define P2_3DES_CBC             2
#define P2_AES_CBC              4
#define P2_AES_CTR              8

#define P2_NULL_SHA1            0
#define P2_HMAC_SHA1            1

#define P2_SW_PADDING_REQUIRED     1
#define P2_AUTH_BYTES_ON_WIRE_10 1
#define P2_AUTH_BYTES_ON_WIRE_12 2

#define IPSEC_ESP_PROTOCOL      50
#define SECURITY_PROTOCOL       IPSEC_ESP_PROTOCOL
#define IPSEC_HEADER_LEN        16

enum auth_algo {
    SHA1=0,
    HMAC_SHA1
};

enum encryption_algo {
	NULL_ENCRYPTION=0,
	DES_CBC,
	THREEDES_CBC,
	AES_CBC,
	AES_CTR
};


enum p2_sec_dir {
 INBOUND=0,
 OUTBOUND
};

enum p2_sec_status {
 SUCCESS=0,
 NOT_ENOUGH_MEMORY,
 INVALID_SECURITY_INDEX,
 INVALID_HASH_INDEX,
 AUTHENTICATION_ERROR,
 ENCRYPTION_ERROR,
 NO_HASH_ENTRY_FOUND,
 INVALID_PROTOCOL
};

struct _P2_SEC_CONTEXT
{
      tU32  encrypt_algo;
      tU32  auth_algo;
      tU8   encrypt_key_out[MAX_ENCRYPT_KEY_LEN];
      tU8   auth_key_out[MAX_AUTH_KEY_LEN];
      tU8   encrypt_key_in[MAX_ENCRYPT_KEY_LEN];
      tU8   auth_key_in[MAX_AUTH_KEY_LEN];
      tU8   iv_in[MAX_IV_LENGTH];
      tU8   iv_out[MAX_IV_LENGTH];
      tU32  encrypt_key_len;
      tU32  auth_key_len;
      tU32  iv_len;
      tU32  spi_in;
      tU32  spi_out;
      tU8   in_idigest[MAX_IDIGEST_LEN];
      tU8   in_odigest[MAX_ODIGEST_LEN];
      tU8   out_idigest[MAX_IDIGEST_LEN];
      tU8   out_odigest[MAX_ODIGEST_LEN];
      tU8   aes_decrypt_key[MAX_ENCRYPT_KEY_LEN];
      tU8   feedback_iv[MAX_IV_LENGTH];
      tU32  sec_flags;
      tU32  seqnum;
      tU32  in_use;
}__attribute__((aligned(1),packed));

typedef struct _P2_SEC_CONTEXT  P2_SEC_CONTEXT;


struct _P2_SA_HASH_TABLE
{
  tU32  ipsrc;
  tU32  ipdst;
  tU16  srcport;
  tU16  dstport;
  tU8   protocol;
  tU8   direction;
  tU32  spi;
  tU32  sec_index;
  tU32  in_use;
}__attribute__((aligned(1),packed));

typedef struct _P2_SA_HASH_TABLE P2_SA_HASH_TABLE;

/*
 * ESP (IPSEC protocol) header
*/
struct _IPSEC_ESP_HEADER {
        tU32 spi;
        tU32 seq_num;
        tU8 iv[MAX_IV_LENGTH];
}__attribute__((aligned(1),packed));

typedef struct _IPSEC_ESP_HEADER     IPSEC_ESP_HEADER, *PIPSEC_ESP_HEADER;

/* 
 *Ethernet  IPSEC (ESP) Header
*/
struct _IPSEC_ETHERNET_PACKET {
	MAC_HEADER	mac_hdr;
	IP_HEADER	ip_hdr;
        IPSEC_ESP_HEADER      esp_hdr;
	union {
  	       TCP_HEADER	tcp_header;
	       UDP_HEADER	udp_header;
	       ICMP_HEADER	icmp_header;
	}ulp;

	tU8 PktData[MAX_ETHERNET_FRAME_LEN - (MAX_ETHERNET_HEADER_LEN + IPSEC_HEADER_LEN)]; 

}__attribute__((aligned(1), packed));

typedef	struct _IPSEC_ETHERNET_PACKET	IPSEC_ETHERNET_PACKET, *PIPSEC_ETHERNET_PACKET;

#define P2_SEC_CONTEXT_BASE_ADDRESS ((P2_SEC_CONTEXT *) (Bank3start + 0x00800000))
#define P2_SA_HASH_TABLE_BASE_ADDRESS ((P2_SA_HASH_TABLE *) (Bank3start + 0x00801000))
#define P2_ICV_STORE_ADDRESS          (void *) (Bank3start + 0x00802000)
#define P2_ICV_STORE_ADDRESS_1        (void *) (Bank3start + 0x00802100)
#define TEMP_SRC_ADDR                 P2_ICV_STORE_ADDRESS
#define TEMP_DST_ADDR                 P2_ICV_STORE_ADDRESS_1 

unsigned int 
ProcessIPSecPkt(void *psrcpkt, void *pdstpkt, tU32 pktlen, tU8 dir);

void
p2_memcpy(unsigned char *dst, unsigned char *src, unsigned int len);

void
p2_memset(unsigned char *dst, unsigned char pat, unsigned int len);

void
P2_ClearIPSecMemory(void);

int
p2_memcmp(unsigned char *s1, unsigned char *s2, unsigned int len);

void
CalcIDigest(tU8 *key, tU8 *ipad, tU8 *ipad_result, tU32 len );

void
CalcODigest(tU8 *key, tU8 *opad, tU8 *opad_result, tU32 len );

unsigned int
CompareAuthData(tU8 *authdata_inpkt, tU8 *authdata_calc, tU32 len);

unsigned int
CalcIPSecPadLength(void);

void
AddAuthPadding(tU8 *pPadOffset, tU32 padlen, tU32 auth_data_len);

void
AddIPSecPadding(void);

unsigned int 
P2_GetSecIndex(void);

unsigned int 
P2_GetHashIndex(void);

void *
P2_GetSecParamCtxt(unsigned int sec_index);

void *
P2_GetHashTable(unsigned int hash_index);

unsigned int 
P2_CreateSecIndexHashTable(  unsigned int ipsrc, unsigned int ipdst, \
                             unsigned short  srcport, unsigned short dstport, \
                             unsigned int spi, unsigned char protocol, 
                             unsigned int direction, unsigned int sec_index );

unsigned int 
P2_GetSecIndexFromHashTable( unsigned int ipsrc, unsigned int ipdst, \
                             unsigned short srcport, unsigned short dstport, \
                             unsigned int spi, unsigned char protocol, \
                             unsigned int direction, unsigned int *sec_index);

unsigned int
P2_CreateSecParamContext(  unsigned int encr_algo, unsigned int auth_algo, unsigned char *encr_key_in, \
                           unsigned char *encr_key_out, unsigned char *auth_key_in,unsigned char *auth_key_out, \
                           unsigned char *iv, unsigned int encr_key_len, unsigned int auth_key_len, unsigned int iv_len, \
                           unsigned int spi_in, unsigned int spi_out, unsigned int flags, unsigned int *sec_index );

unsigned int
ExtractSPIFromPkt(void *ppkt);

unsigned int 
P2_DoEncryptDecrypt( void *psrcaddr, void *pdstaddr, unsigned int length, \
                  unsigned char *key, unsigned char *iv, unsigned int key_len, \
                  unsigned int iv_len, unsigned int flags, unsigned char encrypt );

unsigned int 
P2_DoAuthenticate( void *psrcaddr, void *auth_result, unsigned int length, \
                   unsigned char *key, unsigned int auth_algo, unsigned int flags );

unsigned int
P2_ApplyTransforms (void  *psrcaddr, void *pdstaddr, unsigned int pktlen, 
                    unsigned char direction, unsigned int sec_index );

void get_last_round_key(unsigned int sec_index);


unsigned char
GetIPSecPadLength(unsigned char *pIPHdr, unsigned char *pIPSecHdr);

unsigned int 
GetULPHdrPtr(unsigned char *pIPHdr, unsigned char *pIPSecHdr);

unsigned int 
GetULPDataLen(unsigned char *pIPHdr, unsigned char *pIPSecHdr);

unsigned char
GetIPSecNextProtocol(unsigned char *pIPHdr, unsigned char *pIPSecHdr);

void
ConstructIPSecHeader(char *pIPHdr, char *pIPSecHdr, tU8 direction);


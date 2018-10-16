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
/*	This file contains TX and RX descriptor data structures				*/
/*	Created : 03/29/2006								*/
/*	Author  : Vikram Appikatla 							*/
/****************************************************************************************/

typedef struct  _MAC_RDES {
	/*RDES0 Field*/
        unsigned char        zero:1;			    /* Zero (ZERO od RDES0)		     */
        unsigned char        crce:1;                        /* CRC error (CE of RDES0)               */
        unsigned char        dribble:1;			    /* Dribble bit (DB of RDES0)	     */
        unsigned char        re:1;                          /* MII Error (RE of RDES0)               */
	unsigned char	     Rsvd2:1;			    /* Reserved bits			     */
        unsigned char        ft:1;                          /* Frame type 1=>eth frame (FT of RDES0) */
        unsigned char        cs:1;                          /* Collision seen (CS of RDES0)          */
        unsigned char        tl:1;                          /* Received frame too long (TL of RDES0) */
        unsigned char        ls:1;                          /* Last descriptor (LS of RDES0)         */
        unsigned char        fs:1;                          /* First descriptor (FS of RDES0) 	     */
        unsigned char        multcst:1;                     /* Multicast frame received (MF of RDES0)*/
        unsigned char        runt:1;                        /* Runt frame received (RF of RDES0)     */
	unsigned char	     Rsvd1:2;			    /* Reserved bits			     */
        unsigned char        desc_err:1;                    /* Descriptor error (DE of RDES0)        */
        unsigned char        error_summ:1;		    /* Error summary (ES of RDES0)	     */
        unsigned int         frame_len:14;	            /* Frame length (FL of RDES0)	     */
        unsigned char        filter_fail:1;                 /* Filter fail bit (FF of RDES0)         */
        unsigned char        ownership:1;                   /* MAC/Host Ownership bit (OWN of RDES0) */

	/*RDES1 Field*/
	unsigned int  	    rbs1:11;			    /* Buffer1 size (RBS1 of RDES1)	     */ 
	unsigned int  	    rbs2:11;			    /* Buffer2 size (RBS2 of RDES1)	     */
	unsigned char	    Rsvd4:2;                        /* Reserved bits                         */
	unsigned char	    rch:1;			    /* Second addres chained (RCH of RDES1)  */
	unsigned char	    rer:1;			    /* Receive end of ring (RER of RDES1)    */
	unsigned char	    Rsvd3:6;			    /* Reserved bits			     */

	/*RDES2 Field*/
        unsigned int        rbuff1;                         /* Receive pkt buff addr1 (RBA1 of RDES2) */

	/*RDES3 Field*/
        unsigned int        rbuff2;			    /* Receive pkt buff addr1 (RBA2 of RDES3) */
} MAC_RDES;


typedef struct  _MAC_TDES {
        /*TDES0 Field*/
        unsigned char        de:1;                           /* Deffered (DE of TDES0)                */
        unsigned char        uflw_err:1;                     /* Underflow error (UE of TDES0)         */
        unsigned char        Rsvd4:1;                        /* Reserved bits                         */
        unsigned char        cc:4;                           /* Collision count(CC of TDES0)          */
        unsigned char        Rsvd3:1;                        /* Reserved bits                         */
        unsigned char        ec:1;                           /* Eccessive collisions (EC of TDES0)    */
        unsigned char        lc:1;                           /* Late collision (LC of TDES0)          */
        unsigned char        no_crr:1;                       /* No carrier (NC of TDES0)              */
        unsigned char        crr_loss:1;                     /* Loss of carrier (LO of TDES0)         */
        unsigned char        Rsvd2:3;                        /* Reserved bits                         */
        unsigned char        error_summ:1;                   /* Error summary (ES of TDES0)           */
        unsigned int	     Rsvd1:15;                       /* Reserved bits                         */
        unsigned char        ownership:1;                    /* MAC/Host Ownership bit (OWN of TDES0) */

        /*TDES1 Field*/
        unsigned int   	     tbs1:11;                        /* Buffer1 size (TBS1 of TDES1)          */
        unsigned int  	     tbs2:11;                        /* Buffer2 size (TBS2 of TDES1)          */
        unsigned char        ft0:1;                          /* Filter type (FT0 of TDES1) 	     */
        unsigned char        dpd:1;                          /* Disable padding (DPD of TDES1)        */
	unsigned char	     tch:1;			     /* Second add chained (TCH of TDES1)     */
	unsigned char	     ter:1;			     /* Transmit end of ring (TER of TDES1)   */
        unsigned char        ac:1;                           /* Add CRC disable (AC of TDES1)         */
        unsigned char        set:1;                          /* Setup type frame (SET of TDES1) 	     */
        unsigned char        ft1:1;                          /* Filter type (FT1 of TDES1) 	     */
        unsigned char        fs:1;                           /* First descriptor (FS of TDES1)        */
        unsigned char        ls:1;                           /* Last descriptor (LS of TDES1)         */
        unsigned char        ic:1;                           /* Interrupt od completion (IC of TDES1) */

        /*TDES2 Field*/
        unsigned int        tbuff1;                          /* Transmit pkt buff addr1 (TBA1 of TDES2)*/

        /*TDES3 Field*/
        unsigned int        tbuff2;                          /* Transmit pkt buff addr1 (TBA2 of TDES3)*/
} MAC_TDES;

#define MAC1_RDES_BASE  SYSTEST_MAC1_WORKBASE
#define MAC1_TDES_BASE  MAC1_RDES_BASE+0x1000
#define MAC1_RBUFF_STRT MAC1_RDES_BASE+0x3000
//#define MAC1_TBUFF_STRT 0x81083000
#define MAC1_TBUFF_STRT MAC1_RBUFF_STRT


/*volatile struct MAC_RDES *P_MAC_RDES = (struct MAC_RDES *) MAC1_RDES_BASE;
volatile struct MAC_TDES *P_MAC_TDES = (struct MAC_TDES *) MAC1_TDES_BASE;*/

void CreateRcvRing(int);
void MacRxIntrHandler(void);
void MacTxIntrHandler(void);
char GetRDES(int);
void CreateReply(int);
void SendReply(int, int);
void MovePkt(int, int, int);
void ClearMem(int, int);
void initBuffers(void);
void replenishRdes(int);
void ConstructMacHeader(char *);
void ConstructIPHeader(char *);
void ConstructICMPHeader(char *, int);
void ConstructUDPHeader(tU8 *pIPHdr, tU8 *pUDPHdr, tU32 IPTotalLen);
tU16 CalcUDPTCPChksum(const tU16 *pseudohdr, const tU16 *header, tU32 length);
void writePhy(int, int);
short int CalcChksum(const unsigned short int *, unsigned int);
void CreateTDES(int , int, int, char, int);
void CreateRDES(int , int , int , char );
void mac_reset(void);
short int mdioCmdPHYRd(char, char);
void mdioCmdPHYWr(char, char,  int);

extern void mac_soft_reset (tU32);
extern void mac_program_rx_bar (tU32, tU32);
extern void mac_program_tx_bar (tU32, tU32);
extern void mac_program_pbl (tU32 , tU8 );
extern void mac_set_irq_handler (tU32, tU32);
extern void mac_program_tap (tU32, tU32);
extern void mac_enable_interrupt (tU32, tU32);
extern void mac1_intr_handler (void);
extern void mac2_intr_handler (void);
extern void mac_AIS_handler (tU32);
extern void mac_NIS_handler (tU32);
extern tU32 mac_start_tx (tU32);
extern tU32 mac_start_rx (tU32);

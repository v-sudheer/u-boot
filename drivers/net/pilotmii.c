/*
    Copyright (C) 2018 Shivah Shankar <shivahshankar.shankarnarayanrao@aspeedtech.com>
    Copyright (C) 2009-2011 American Megatrends Inc.
    Copyright (c) 2009-2011, Intel Corporation.
    Copyright (c) 2010-2014, Emulex Corporation.
    SPDX-License-Identifier:     GPL-2.0+
*/


#include <common.h>
#if defined (CONFIG_COMMANDS) && defined (CONFIG_CMD_NET)
#include <asm/processor.h>
#include <miiphy.h>
#include <configs/pilot.h>
#include "pilotmac.h"
#include "pilotmacreg.h"

#define inl(addr) 		(*((volatile u32 *)(addr)))
#define outl(addr,val) 	(*((volatile u32 *)(addr)) = (val))

#define PILOT_RT_PHYSR	0x11 
//Reg 0x11 defines - PHY specific Status register
#define REG0x11_FULL_DUPLX_MODE         0x00002000
#define REG0x11_10Mbps                  0x00000000
#define REG0x11_100Mbps                 0x00004000
#define REG0x11_1000Mbps                0x00008000
#define Mircel_Phy_Control              0x1F

// micrel phy on RMM3/4
// Phy Control 2 register has result of auto-negotiation
#define MICREL_PHYC2                    0x1f

#define NCSI_PHY_ADDR                   0xff    // Indicates no phy for NC-SI channels


/***************************** Tulip  MII read/write access macros *************************/
#define	AST_MII_BIT_READ(Addr, pBData)                           		\
   	{                                                           			\
    	outl((Addr)+AST_MAC_CSR9,CSR9_MII_RD);								\
        udelay(2);                                            				\
        outl((Addr)+AST_MAC_CSR9,CSR9_MII_RD | CSR9_MDC);					\
        udelay(2);                                                     		\
        *(pBData) |= CSR9_MII_DBIT_RD (inl ((Addr)+ AST_MAC_CSR9));  		\
   	}

#define	AST_MII_BIT_WRITE(Addr, data)                                 		\
    {                                                               		\
    	outl((Addr)+AST_MAC_CSR9, CSR9_MII_DBIT_WR(data) | CSR9_MII_WR);	\
        udelay(2);                                                     		\
    	outl((Addr)+AST_MAC_CSR9, CSR9_MII_DBIT_WR(data) | CSR9_MII_WR | CSR9_MDC);				\
        udelay(2);                                                     		\
    }

#define	AST_MII_RTRISTATE(Addr)                                       		\
	{                                                               		\
        int retBData;                                                     	\
        AST_MII_BIT_READ ((Addr), &retBData);                          		\
    }

#define	AST_MII_WTRISTATE(Addr)                                       		\
	{                                                               		\
        AST_MII_BIT_WRITE((Addr), 0x1);                               		\
        AST_MII_BIT_WRITE((Addr), 0x0);                               		\
	}

#define AST_MII_WRITE(Addr, data, bitCount)                           		\
   	{                                                               		\
        int i=(bitCount);                                               	\
                                                                        	\
        while (i--)                                                    	 	\
            AST_MII_BIT_WRITE ((Addr), ((data) >> i) & 0x1);          		\
    }

#define	AST_MII_READ(Addr, pData, bitCount)                           		\
   	{                                                               		\
        int i=(bitCount);                                               	\
                                                                        	\
        while (i--)                                                     	\
        {                                                           		\
            *(pData) <<= 1;                                             	\
	        AST_MII_BIT_READ ((Addr), (pData));                      		\
        }                                                           		\
    }

int GetDevNum(const char *devname)
{
	int num = -1;

	/* Assumption: single digits 0 to 9 only and occurs at last char */
	while (*devname)
	{
		num= *devname;
		devname++;
	}
	if ((num < '0') || ( num  > '9'))
		return -1;

	/* Convert char to integer */
	num-='0';

	return num;
}


static u32 getmacaddr(const char *devname)
{
	int num;

	if (devname == NULL)
		return 0;
	num = GetDevNum(devname);
	if (num == -1)
		return 0;
	switch (num)
	{
		case 1:
	// Pilot-II does not have MDIO/MDC signals for the second MAC
	// So normally, we set the two PHY to different address and connect 
	// both the PHY to the same MDIO/MDC signals from the first MAC
	//		return SE_MAC_B_BASE;
		case 0:
			return P4_MAC_A_BASE;
		default:
			break;
	}
	return 0;
}


int ast_miiphy_read (const char *devname, unsigned char addr, unsigned char reg,  unsigned short *value)
{
	u32 macaddr;

    *value = 0;
    if (addr == NCSI_PHY_ADDR)
        return -1;

	macaddr = getmacaddr(devname);
	if (macaddr == 0)
		return -1;

    /* Write 34-bit preamble */
    AST_MII_WRITE (macaddr, MII_PREAMBLE, 32);

    /* start of frame + op-code nibble */
    AST_MII_WRITE (macaddr, MII_SOF | MII_RD, 4);

    /* device address */
    AST_MII_WRITE (macaddr, addr, 5);
    AST_MII_WRITE (macaddr, reg, 5);

    /* turn around */
    AST_MII_RTRISTATE (macaddr);

#ifdef CONFIG_PHY_CMD_DELAY
	udelay (CONFIG_PHY_CMD_DELAY);		/* Intel LXT971A needs this */
#endif

    /* read data */
    AST_MII_READ (macaddr, value, 16);

	/* turn around */
    AST_MII_RTRISTATE (macaddr);

//	printf("Phy Read Value = %x Phy Address = %x Reg = %x\n",*value,addr,reg);
	return 0;
}

int ast_miiphy_write (const char *devname, unsigned char addr, unsigned char reg, unsigned short value)
{
	u32 macaddr;

    if (addr == NCSI_PHY_ADDR)
        return -1;

	macaddr = getmacaddr(devname);
	if (macaddr == 0)
		return -1;

//	printf("Phy Write Value = %x Phy Address = %x Reg = %x\n",value,addr,reg);

    /* write 34-bit preamble */
    AST_MII_WRITE (macaddr, MII_PREAMBLE, 32);

    /* start of frame + op-code nibble */
    AST_MII_WRITE (macaddr, MII_SOF | MII_WR, 4);

    /* device address */
    AST_MII_WRITE (macaddr, addr, 5);
    AST_MII_WRITE (macaddr, reg, 5);

    /* turn around */
    AST_MII_WTRISTATE (macaddr);

#ifdef CONFIG_PHY_CMD_DELAY
	udelay (CONFIG_PHY_CMD_DELAY);		/* Intel LXT971A needs this */
#endif
    /* write data */
    AST_MII_WRITE (macaddr,value, 16);

	return 0;

}

int GetPhyAddr(char *devname, unsigned char *addr)
{
	int devnum = GetDevNum(devname);

#ifdef PROD_IS_PILOT
    //*addr = (devnum == 0) ? 1 : 2;
    *addr = (devnum == 0) ? CONFIG_PHY_ADDR0: CONFIG_PHY_ADDR1;
	//printf("The Phy address returned is %x\n", *addr);
    return 0;
#else
    // NCSI channels don't have PHY
    if (devnum == 0)
    {
        *addr = NCSI_PHY_ADDR;
        return 0;
    }
    // For RMM, must probe to find PHY
    else
    {
        static int rmm_probed = 0;
        static unsigned char rmm_addr = 0;
        if (!rmm_probed)
        {
            // RMM3 Micrel PHY is at 7, RMM4 DNM Fab1 Micrel PHY is at 7, RMM4 DNM Fab2 Realtek PHY is at 3
            u16 reg;
            ast_miiphy_read(devname, 7, MII_PHY_ID1, &reg);
            if (reg != 0xffff)
                rmm_addr = 7;
            else
                rmm_addr = 3;

            rmm_probed = 1;
        }
        *addr = rmm_addr;
    }
#endif
    return 0;
}

int
GetLinkStatus(char *devname)
{
	u16 status;
	unsigned char addr;

	if (GetPhyAddr(devname, &addr) != 0)
        return 0;

    // If no PHY, assume link is up (NCSI)
    if (addr == NCSI_PHY_ADDR)
        return 1;	

	/* Needs two reads of Status reg to get correct link status */
	ast_miiphy_read (devname,addr,MII_PHY_SR,&status);
	ast_miiphy_read (devname,addr,MII_PHY_SR,&status);
	if (!(status & MII_PHY_SR_LNK))	/* Link Status */
		return 0;
	if (!(status & MII_PHY_SR_ASS))		/* Auto Nego Status */
		return 0;
	return 1;
}

int
GetPhySpeed(char *devname)
{
	u16 status;
	unsigned char addr;
    u16 phyid=0;

	if (GetPhyAddr(devname, &addr) != 0)
        return 1;

    // If no PHY, assume speed is 100Mbps (NCSI)
    if (addr == NCSI_PHY_ADDR)
        return 1;	

	if (!GetLinkStatus(devname))
		return 0;	/* No Link or AN not complete */

    ast_miiphy_read(devname, addr, MII_PHY_ID1, &phyid);
	  //printf("Phy id Sts is %x\n",status);

#ifndef PROD_IS_PILOT
// Intel RMM3/4 PHYs

    phyid &= 0xfff0;    // Ignore silicon rev nibble
    if (phyid == 0x1510)
    {
        // MICREL KSZ8041NL 100MBit PHY
        ast_miiphy_read(devname, addr, MICREL_PHYC2, &status);
        switch ((status >> 2) & 0x7)
        {
            case 1:
            case 5:
                return 0;
            case 2:
            case 6:
                return 1;
            default:
                printf("Micrel 100MBit Phy Speed Unknown Sts is %x\n", status);
                return 0;
        }
    }
    else if (phyid == 0x1610)
    {
        // MICREL KSZ9021RL 1GBit PHY
        ast_miiphy_read(devname, addr, MICREL_PHYC2, &status);
        switch ((status >> 3) & 0xf)
        {
            case 2:
            case 3:
                return 0;
            case 4:
            case 5:
                return 1;
            case 8:
            case 9:
                return 2;
            default:
                printf("Micrel 1GBit Phy Speed Unknown Sts is %x\n", status);
                return 0;
        }
    }
    else
    {
        // REALTEK RTL8211D 1GBit PHY
        ast_miiphy_read(devname, addr, PILOT_RT_PHYSR, &status);
      
        if ((status & (REG0x11_1000Mbps | REG0x11_100Mbps | REG0x11_10Mbps)) == REG0x11_100Mbps)
            return 1;	/* 100 Mbps */
        else if ((status & (REG0x11_1000Mbps | REG0x11_100Mbps | REG0x11_10Mbps)) == REG0x11_1000Mbps)
            return 2;	/* 1000 Mbps */
        else
            return 0;	/* 10Mbps */
    }

#else // Hornet Board PHYs

  if(((phyid&0x3f0)>>0x4)==0x11)
    {//if realtek
      ast_miiphy_read (devname,addr,PILOT_RT_PHYSR,&status);
      
      if((status & (REG0x11_1000Mbps | REG0x11_100Mbps | REG0x11_10Mbps)) == REG0x11_100Mbps){
	return 1;	/* 100 Mbps */
      } else if ((status & (REG0x11_1000Mbps | REG0x11_100Mbps | REG0x11_10Mbps)) == REG0x11_1000Mbps){
	return 2;	/* 1000 Mbps */
      } else {
	return 0;	/* 10Mbps */
      }
      
    }
  else if((((phyid&0x3f0)>>0x4)==0x21) || (((phyid&0x3f0)>>0x4) == 0x22)) //Shivah Added
    {
      //if mircel
      ast_miiphy_read (devname,addr,Mircel_Phy_Control,&status);
	  //printf("Mircel Phy Speed Sts is %x\n",status);
       //printf("Mircel Phy Speed Sts is %x\n",((status>>3)&(0xf)));
      switch( ( (status>>3)&(0xf) ) )
	{
	case 2:
	case 3:
	  return 0;
	case 4:
	case 5:
	  return 1;
	case 8:
	case 9:
	  return 2;
	default:
	  printf("Mircel Phy Speed Unknown Sts is %x\n",status);
	  
	}
    }
  else if( (((phyid&0x3f0)>>0x4)==0x0f) || (((phyid&0x3f0)>>0x4)==0x0c) )
    {
      ast_miiphy_read (devname,addr,31,&status);
      if( (status&0xc)==0x4)
	{
	  return 0;
	}
      else if( (status&0xc)==0x8)
	{
	  return 1;
	}
    }
  else if( ((phyid&0x3f0)>>0x4) == 0x1D )
    {
      printf("Marvell GetPhySpeed\n");
      ast_miiphy_read (devname,addr,17,&status);
      if( status & 0x800 )   /* Speed & Duplex Resolved bit is set */
        {
          status = ((status >> 14) & 0x3);
	  switch(status) 
	  {
		case 2:
			return 2;
		case 1:
			return 1;
		case 0:
			return 0;
		default:
			printf("Phy speed is wrong \n");
			return 0;
	  } 
        }
    }
  else
    {
      printf("GetPhySpeed Phy Id is not matching %x\n", phyid);
    }

#endif // PROD_IS_PILOT
    return 0;
}

int
GetPhyDuplex(char *devname)
{
	u16 status;
	unsigned char addr;
    u16 phyid=0;

	if (GetPhyAddr(devname, &addr) != 0)
        return 1;

    // If no PHY, assume full duplex (NCSI)
    if (addr == NCSI_PHY_ADDR)
        return 1;	

	if (!GetLinkStatus(devname))
		return 0;	/* No Link or AN not Complete */

    ast_miiphy_read(devname, addr, MII_PHY_ID1, &phyid);

#ifndef PROD_IS_PILOT
// Intel RMM3/4 MICREL PHYs

    phyid &= 0xfff0;    // Ignore silicon rev nibble
    if (phyid == 0x1510)
    {
        // MICREL KSZ8041NL 100MBit PHY
        ast_miiphy_read(devname, addr, MICREL_PHYC2, &status);
        switch ((status >> 2) & 0x7)
        {
            case 1:
            case 2:
                return 0;
            case 5:
            case 6:
                return 1;
            default:
                printf("Micrel 100MBit Phy Duplex Unknown Sts is %x\n", status);
                return 0;
        }
    }
    else if (phyid == 0x1610)
    {
        // MICREL KSZ9021RL 1GBit PHY
        ast_miiphy_read(devname, addr, MICREL_PHYC2, &status);
        switch ((status >> 3) & 0xf)
        {
            case 2:
            case 4:
            case 8:
                return 0;
            case 3:
            case 5:
            case 9:
                return 1;
            default:
                printf("Micrel 1GBit Phy Duplex Unknown Sts is %x\n", status);
                return 0;
        }
    }
    else
    {
        // REALTEK RTL8211D 1GBit PHY
        ast_miiphy_read(devname, addr, PILOT_RT_PHYSR, &status);
	    if (status & REG0x11_FULL_DUPLX_MODE)
            return 1;   /* FD */
        else 
            return 0;	/* HD */
    }

#else // Hornet Board PHYs

	if(((phyid&0x3f0)>>0x4)==0x11)
	  {//if realtek
	    ast_miiphy_read (devname,addr,PILOT_RT_PHYSR,&status);
	    if(status & REG0x11_FULL_DUPLX_MODE)
	      {
		return 1;  /* FD */
	      }
	    else 
	      {
		return 0;	 /* HD */
	      }
	  }
//	else if(((phyid&0x3f0)>>0x4)==0x21)
        else if((((phyid&0x3f0)>>0x4)==0x21) || (((phyid&0x3f0)>>0x4) == 0x22)) //Shivah  Changed
	  {
	    //if mircel
	    ast_miiphy_read (devname,addr,Mircel_Phy_Control,&status);
	    switch( ( (status>>3)&(0xf) ) )
	      {
	      case 3:
	      case 5:
	      case 9:
		return 1;
	      case 2:
	      case 4:
	      case 8:
		return 0;
	      default:
		printf("Mircel Phy Speed/duplex Unknown Sts is %x\n",status);
	      }
	  }
	else if( (((phyid&0x3f0)>>0x4)==0x0f)|| (((phyid&0x3f0)>>0x4)==0x0c) )
	  {
	    ast_miiphy_read (devname,addr,31,&status);
	    if(status & 0x10)
	      {
		return 1;  /* FD */
	      }
	    else 
	      {
		return 0;	 /* HD */
	      }
	  }
	 else if( ((phyid&0x3f0)>>0x4) == 0x1D )
	  {
	    printf("Marvell GetPhyDuplex\n");
            ast_miiphy_read (devname,addr,17,&status);
            if(status & 0x800)
              {
		if(status & 0x2000)	
                	return 1;  /* Full Duplex */
		else
			return 0;  /* Half Duplex */
              }
	  }
	else{
	  printf("GetPhyDuplex Phy Id is not matching %x\n", phyid);
	}
#endif // PROD_IS_PILOT
	return 0;
}
#endif

/*
 * Copyright (C) 2012-2020, 2012 ASPEED Tech., Inc.
 * Ryan Chen (ryan_chen@aspeedtech.com)
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <config.h>
#include <common.h>
#include <asm/io.h>
#include <asm/arch/regs-scu.h>
#include <asm/arch/ast-sdmc.h>
#include <asm/arch/platform.h>

DECLARE_GLOBAL_DATA_PTR;

int checkboard(void)
{
	puts("Board: ");
	puts("ASPEED AST1010 EVB\n");
	return 0;
};

#define INFO		0
#define INFO_HASHMARK	0

#define CBR_SIZE        ((64 << 10) - 1)
#define CBR_PASSNUM     2
#define CBR_THRESHOLD   10
#define TIMEOUT         20000
#define CBR_PATNUM      4

typedef enum {
	TEST_TYPE_SINGLE=0,
	TEST_TYPE_BURST,
	END_OF_INDEX_TEST_TYPE
} IndexTestType;

int MMCTestSingleBurst(u32 bType, u32 datagen)
{
	u32 data, timeout;
	const u32 pattern[2] = {0x00000085, 0x000000C1};

	__raw_writel( 0x00000000, AST_SDMC_BASE + 0x70);
	__raw_writel( pattern[bType] | (datagen << 3), AST_SDMC_BASE + 0x70);

	timeout = 0;
	do {
		data = __raw_readl(AST_SDMC_BASE + 0x70) & 0x300;
		if (data & 0x200) return (0);
#ifdef CONFIG_AST_DVT_TEST
		if(++timeout > TIMEOUT)
		{
			if(INFO) printf("Timeout!!\n");
			__raw_writel( 0x00000000, AST_SDMC_BASE + 0x70);
			return(0);
		}
#endif		
	} while(!data);

	__raw_writel( 0x00000000, AST_SDMC_BASE + 0x70);
	if (INFO_HASHMARK) puts("*");	
	return(1);
}

int MMCTestBurst(u32 datagen)
{
	return (MMCTestSingleBurst(TEST_TYPE_BURST, datagen));
}

int MMCTestSingle(u32 datagen)
{
	return (MMCTestSingleBurst(TEST_TYPE_SINGLE, datagen));
}

int MMCTest(void)
{
	if(!MMCTestBurst(0)) return(0);
	if(!MMCTestBurst(1)) return(0);
	if(!MMCTestBurst(2)) return(0);
	if(!MMCTestBurst(3)) return(0);
	return(1);
}

const u32 pattern[CBR_PATNUM] = {0xFF00FF00, 0xCC33CC33, 0xAA55AA55, 0x72452783};

int CBRScan()
{
	u32 patcnt, loop;


	__raw_writel( CBR_SIZE, AST_SDMC_BASE + 0x74);		
	for(patcnt=0; patcnt < CBR_PATNUM; patcnt++)
	{
		__raw_writel( pattern[patcnt], AST_SDMC_BASE + 0x7C);		
		for(loop = 0; loop < CBR_PASSNUM; loop++)
		{
			if(MMCTest()) break;
		}
		if(loop == CBR_PASSNUM) return(0);
	}
	return(1);
}

#define TIMER_RELOAD 0x24
#define TIMER_STATUS 0x20

#define TIMER_MASK	0x0300

void dram_udelay(int length)
{
	unsigned long data, data1;

	data = __raw_readl(AST_TIMER_BASE + 0x30);
	__raw_writel(data & ~(TIMER_MASK), AST_TIMER_BASE + 0x30);
	__raw_writel(length, AST_TIMER_BASE + TIMER_RELOAD);
	data = __raw_readl(AST_TIMER_BASE + 0x30);
	__raw_writel(data | TIMER_MASK, AST_TIMER_BASE + 0x30);
	data = __raw_readl(AST_TIMER_BASE + TIMER_STATUS);

	while(1)
	{
		data1 = __raw_readl(AST_TIMER_BASE + TIMER_STATUS);
		if (data1 > data)
			break;
		else
			data = data1;
	}
}

int CheckDRAMSize(void)
{
	u32 data;
	const u32 patternCheck[2] = {0x01234567, 0x89ABCDEF};

	// Check DRAM size
	__raw_writel( 1, AST_SDMC_BASE + 0x04);
	__raw_writel( patternCheck[0], CONFIG_SYS_SDRAM_BASE);
	__raw_writel( patternCheck[1], CONFIG_SYS_SDRAM_BASE + 0x200);
	data = __raw_readl(CONFIG_SYS_SDRAM_BASE);
	if (data != patternCheck[0]) __raw_writel( 0, AST_SDMC_BASE + 0x04);
	return 0;
}

void init_chip(void)
{
	u32 data, dlyset, dqsmax[2], dqsmin[2], cnt, passcnt[2], dqsip;

	__raw_writel( 0xFC600309, AST_SDMC_BASE + 0x00);
	__raw_writel( 0x00000002, AST_SDMC_BASE + 0x04);

#if	defined(SG5_200MHz)
	__raw_writel( 0x14845464, AST_SDMC_BASE + 0x10);
	__raw_writel( 0x000000EC, AST_SDMC_BASE + 0x20);
	__raw_writel( 0x00310000, AST_SDMC_BASE + 0x28);
#endif
#if	defined(SG5_166_166MHz) || defined(SG5_166_83MHz)
	__raw_writel( 0x13831064, AST_SDMC_BASE + 0x10);
	__raw_writel( 0x000000C3, AST_SDMC_BASE + 0x20);
	__raw_writel( 0x00310000, AST_SDMC_BASE + 0x28);
#endif
#if	defined(SG5_133MHz)
	__raw_writel( 0x02421064, AST_SDMC_BASE + 0x10);
	__raw_writel( 0x0000009C, AST_SDMC_BASE + 0x20);
#endif
#if	defined(SG6_166MHz)
	__raw_writel( 0x14445464, AST_SDMC_BASE + 0x10);
	__raw_writel( 0x000000C3, AST_SDMC_BASE + 0x20);
#endif
#if	defined(SG6_133MHz)
	__raw_writel( 0x03431464, AST_SDMC_BASE + 0x10);
	__raw_writel( 0x0000009C, AST_SDMC_BASE + 0x20);
#endif
	__raw_writel( 0x02800000, AST_SDMC_BASE + 0x18);
	__raw_writel( 0x00000888, AST_SDMC_BASE + 0x40);
	__raw_writel( 0x00000000, AST_SDMC_BASE + 0x60);
	__raw_writel( 0x00000000, AST_SDMC_BASE + 0x70);
	__raw_writel( 0x00000000, AST_SDMC_BASE + 0x74);
	__raw_writel( 0x00000000, AST_SDMC_BASE + 0x78);
	__raw_writel( 0x00000000, AST_SDMC_BASE + 0x7C);

	/* scratch: initialize registers of sdram controller, note as '2' */
	if (CONFIG_DRAM_STEPINFO) puts("2");

	if (INFO) printf("Wait MCLK2X lock to MCLK ....\n");
	do {
		data = __raw_readl(AST_SDMC_BASE + 0x1C);
	} while(!(data & 0x00000040));

	data = data & 0x3f;
	 if(data > 55 || data < 8)
	 {
			 data = __raw_readl(AST_SDMC_BASE + 0x18) & 0xFF7FFFFF;
			 data = data ^ 0x00400000;
			 __raw_writel( data, AST_SDMC_BASE + 0x18);
			 __raw_writel( data | 0x00800000, AST_SDMC_BASE + 0x18);
			 if (INFO) puts("Invert MCLK2X to MCLK phase\n");
			 do {
					 data = __raw_readl(AST_SDMC_BASE + 0x1C);
			 } while(!(data & 0x00000040));
	 }

	if(INFO) printf("MCLK2X lock to MCLK at value %d\n", (int) (data & 0x3F));

	/* scratch: finish setting MCLK2X, note as '3' */
	if (CONFIG_DRAM_STEPINFO) puts("3");

	__raw_writel( 0x00000001, AST_SDMC_BASE + 0x34);
	dram_udelay(10);	// delay 10 us
	data = __raw_readl(AST_SDMC_BASE + 0x28);
	__raw_writel( data | 0x01, AST_SDMC_BASE + 0x28);
	__raw_writel( 0x00008008, AST_SDMC_BASE + 0x0C);
	dram_udelay(100);		// delay 100 us
	__raw_writel( 0x0000b801, AST_SDMC_BASE + 0x0C);
	__raw_writel( 0x00000033, AST_SDMC_BASE + 0x34);

	if(INFO) printf("DQSI CBR phase locked at value %d\n",
			(__raw_readl(AST_SDMC_BASE + 0x20) >> 12) & 0xf);

	/* scratch: finish setting DQSI CBR phase, note as '4' */
	if (CONFIG_DRAM_STEPINFO) puts("4");

	if(INFO) puts("Search Read DQS delay margin ....\n");
	dlyset = __raw_readl(AST_SDMC_BASE + 0x18) & 0xFFFFFFC0;

CBR_START:
	for (dqsip=0; dqsip<2; dqsip++) {
		dqsmin[dqsip] = 64;
		dqsmax[dqsip] = 0;
		passcnt[dqsip] = 0;
		for(cnt = 0;cnt < 64;cnt++) {

			__raw_writel( 0x00000000, AST_SDMC_BASE + 0x70);
			__raw_writel( dlyset | cnt | (dqsip << 12), AST_SDMC_BASE + 0x18);		/* add */

			// Doing memory access at least 2 double-words for DQSI phase lock
			if(CBRScan()) {
				if (dqsmin[dqsip] > cnt) dqsmin[dqsip] = cnt;
				if (dqsmax[dqsip] < cnt) dqsmax[dqsip] = cnt;
				passcnt[dqsip]++;
			} else if (passcnt[dqsip] > CBR_THRESHOLD) {
				break;
			}
		}
	}
	
	__raw_writel( 0x00000000, AST_SDMC_BASE + 0x70);		 /*add*/
	if(INFO) printf("Search Read DQS delay margin P-edge: %d-%d\n", (int) dqsmin[0], (int) dqsmax[0]);
	if(INFO) printf("Search Read DQS delay margin N-edge: %d-%d\n", (int) dqsmin[1], (int) dqsmax[1]);

	/* scratch: finish searching Read DQS delay margin, note as '5' */
	if (CONFIG_DRAM_STEPINFO) puts("5");

	if(passcnt[0] < CBR_THRESHOLD && passcnt[1] < CBR_THRESHOLD) {
		// window too late, go ahead 0.5T
		if(dqsmin[0] == 0 || dqsmin[1] == 0) {
			data = __raw_readl(AST_SDMC_BASE + 0x10) & 0xff3fffff;
			__raw_writel( data | 0x00400000, AST_SDMC_BASE + 0x10);  /*add*/
			dlyset |= 0x40;
		}
		if(INFO) printf("Retry CBR\n");
		goto CBR_START;
	}
	if (passcnt[1] > passcnt[0])
	{
		dlyset |= 0x1000;
		dqsmax[0] = dqsmax[1];
		dqsmin[0] = dqsmin[1];
	}
	if (dqsmin[0] != 0 || dqsmax[0] > 20)
	{
		dlyset |= (dqsmax[0]+dqsmin[0]+1) >> 1;
	}
	else if(dqsmax[0] > 10)
	{
		dlyset |= (dqsmax[0]-10);
	}

	dlyset |= 0x80;
	__raw_writel( dlyset, AST_SDMC_BASE + 0x18);
	if(INFO) printf("Set Read DQS delay value = %d\n", (int) (dlyset & 0x3F));

	/* scratch: finish setting Read DQS delay value, note as '6' */
	if (CONFIG_DRAM_STEPINFO) puts("6");

	if(INFO) printf("Wait Read DQS delay lock ....\n");
	do {
		data = __raw_readl(AST_SDMC_BASE + 0x24);
	} while(!(data & 0x80000000));
	if(INFO) printf("Read DQS delay lock at value %d\n", (int) (data & 0x3FF));

	/* scratch: finish setting Read DQS delay lock, note as '7' */
	if (CONFIG_DRAM_STEPINFO) puts("7");

	// Doing memory access at least 2 double-words for DQSI phase lock
	__raw_readl(CONFIG_SYS_SDRAM_BASE);
	__raw_readl(CONFIG_SYS_SDRAM_BASE + 0x04);
	
	if(INFO)
	{
		data = __raw_readl(AST_SDMC_BASE + 0x18);
		printf("DQSI to MCLK2X latch phase = %s\n", (data & 0x8000) ? "Negative" : "Positive");
		printf("SDRAM initialize finished!\n");
	}

	/* scratch: DRAM size checking, note as '8' */
	if (CONFIG_DRAM_STEPINFO) puts("8");
	CheckDRAMSize();

	/* scratch: finish SDRAM initialization, note as '9' */
	if (CONFIG_DRAM_STEPINFO) puts("9");
}

#ifdef CONFIG_AST_DVT_TEST
int SDRTest(void)
{

	ULONG loop;
	ULONG patcnt;

        MOutdwm((UCHAR *) mmiobase, AST_GPIO_BASE + 0x00, 0);
        MOutdwm((UCHAR *) mmiobase, AST_GPIO_BASE + 0x04, 0x00ff0000);
	if (!getDRAM_16MBflag()) return (0);
        MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x34, 0x00000013);
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x74, 0x00ffffff);

	for (loop = 0; loop < 2; loop++)
	{
		MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x7c, pattern[loop]);
		if(!MMCTestBurst(loop)) return(0);
	}

        MOutdwm((UCHAR *) mmiobase, AST_GPIO_BASE + 0x00, 0x00ff0000);

	return (1);
}
#endif

#if	(CONFIG_AST_FPGA == 1)
void init_fpga(int sel)
{
	ULONG cl, dqsip, mcr10, mcr18;
	ULONG gcl, gdqsip;
	int pcnt = 0;

	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x00, 0xFC600309);
//	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x10, 0x14845464);  // >  133
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x10, 0x03031464);  // <= 133
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x20, 0x000000C5);
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x18, 0x01000000);  // <= 133
//	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x18, 0x00000020);  // >  133
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x40, 0x00000888);
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x70, 0x00000000);
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x74, 0x00000000);
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x78, 0x00000000);
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x7C, 0x00000000);
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x34, 0x00000001);
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x28, 0x00210001);  // <= 133
//	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x28, 0x00310001);  // >  133
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x0C, 0x00008008);
	dram_udelay(100);	// delay 100 us
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x0C, 0x0000CA01);
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x34, 0x00000033);

	mcr10 = MIndwm((UCHAR *) mmiobase, MMC_BASE + 0x10) & 0xff3fffff;
	mcr18 = MIndwm((UCHAR *) mmiobase, MMC_BASE + 0x18) & 0xffff0000;

	/* scratch: initialize registers of sdram controller, note as '2' */
	if (CONFIG_DRAM_STEPINFO) MOutdwm((UCHAR *) mmiobase, CONFIG_UART_DEBUG, (ULONG) '2');

	for(cl = 0;cl < 8 && pcnt <= sel;cl++)
	{
		for(dqsip = 0;dqsip < 2;dqsip++)
		{
			if (INFO) printf("Scan CL=%x, DQSIP = %d\n", (unsigned int) cl, (unsigned int) dqsip);
			MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x10, mcr10 | ((cl & 0x6) << 21));
			MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x18, mcr18 | ((cl & 0x1) << 6) | (dqsip << 12));
			if(CBRScan())
			{
				if (INFO) printf("Pass at CL = %x and DQSIP = %d\n", (unsigned int) cl, (unsigned int) dqsip);
				gcl = cl;
				gdqsip = dqsip;
				if (pcnt++ == sel) break;
			}
		}
	}
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x10, mcr10 | ((gcl & 0x6) << 21));
	MOutdwm((UCHAR *) mmiobase, MMC_BASE + 0x18, mcr18 | ((gcl & 0x1) << 6) | (gdqsip << 12));

	/* scratch: DRAM size checking, note as '8' */
	if (CONFIG_DRAM_STEPINFO) MOutdwm((UCHAR *) mmiobase, CONFIG_UART_DEBUG, (ULONG) '8');
	CheckDRAMSize();

	/* scratch: finish validation, note as '9' */
	if (CONFIG_DRAM_STEPINFO) MOutdwm((UCHAR *) mmiobase, CONFIG_UART_DEBUG, (ULONG) '9');
}
#endif	/* CONFIG_AST_FPGA */

phys_size_t initdram(int board_type)
{
	u32 scratch;


//	puts("DRAM init....");
	scratch = __raw_readl(AST_SCU_BASE + AST_SCU_SOC_SCRATCH1);
	
	if ((scratch & 0x40) == 0) {
		//unlock 
		__raw_writel( 0x1688a8a8, AST_SCU_BASE);
		
		puts("  init....");
		scratch |= 0x80;
		__raw_writel( scratch, AST_SCU_BASE + AST_SCU_SOC_SCRATCH1);		
		/* scratch: dram init start, note as '1' */
		if (CONFIG_DRAM_STEPINFO) puts("1");

#ifdef CONFIG_AST_FPGA
		init_fpga(0);
#else
		init_chip();
#endif

#ifdef CONFIG_AST_DVT_TEST
		SDRTest();
#endif
		/* scratch: dram init finish, note as 'A' */
		if (CONFIG_DRAM_STEPINFO) puts("A");

		scratch |= 0x40;
		__raw_writel( scratch, AST_SCU_BASE + AST_SCU_SOC_SCRATCH1);				
		
		puts("....Done!  ");		
	}

//	puts("....Done! \n");

	return ast_sdmc_get_mem_size();
}
int testdram(void)
{
	/* TODO: XXX XXX XXX */
	printf("DRAM test not implemented!\n");

	return (0);
}

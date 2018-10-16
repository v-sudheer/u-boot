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


#include <stdio.h>
#include <unistd.h>
#include "uart.h"

//#include "interface.h"
//#include "jtag.h"

//#include "avago_pilot.c"
//void run_me(void);
//int avago_init(void);
//int avago_khz(int khz, int *jtag_speed);


# include "defs.h"
# include "timer.h"

extern void m3_timer_init(void);
extern void systick_handler(void);

//#define DEBUG_PRINTS

#ifdef DEBUG_PRINTS
#define DDR_START 0x81000000
#define DDRMEM ((0x9FEEC000 - 0x81000000) / 4)
#endif

//#define TIMER
#define GDMA_BASE 0x40a00080
#define GDMA_CTRL (GDMA_BASE + 0x0)
#define GDMA_STATUS (GDMA_BASE + 0x4)
#define GDMA_DTB (GDMA_BASE + 0x8)
#define GDMA_CS (GDMA_BASE + 0xC)

#ifdef ENABLE_PRINTS
static void print_reg(unsigned int reg)
{
    int i =0, shift = 24;
    int val;

    for(i=0;i<8;i++)
    {
        shift = 28 - (i *4);
        val = (reg >> shift);
        val = val & 0xF;
        if(val == 0)
            m3_uart_putc('0');
        if(val == 1)
            m3_uart_putc('1');
        if(val == 2)
            m3_uart_putc('2');
        if(val == 3)
            m3_uart_putc('3');
        if(val == 4)
            m3_uart_putc('4');
        if(val == 5)
            m3_uart_putc('5');
        if(val == 6)
            m3_uart_putc('6');
        if(val == 7)
            m3_uart_putc('7');
        if(val == 8)
            m3_uart_putc('8');
        if(val == 9)
            m3_uart_putc('9');
        if(val == 0xa)
            m3_uart_putc('a');
        if(val == 0xb)
            m3_uart_putc('b');
        if(val == 0xc)
            m3_uart_putc('c');
        if(val == 0xd)
            m3_uart_putc('d');
        if(val == 0xE)
            m3_uart_putc('e');
        if(val == 0xf)
            m3_uart_putc('f');
    }
    m3_uart_putc('\n');
    m3_uart_putc('\r');
}
#endif

unsigned int DESC_START = 0;
int main(void)
{
    volatile int i = 0;

    m3_uart_init ();

    m3_uart_puts ("CM3-11\n");

#ifdef DEBUG_PRINTS
    asdasd
        m3_uart_puts ("0x40100820\n");
    print_reg(*((volatile unsigned long*)0x40100820));
    m3_uart_puts ("0x40100828\n");
    print_reg(*((volatile unsigned long*)0x40100824));
    m3_uart_puts ("0x40100828\n");
    print_reg(*((volatile unsigned long*)0x40100828));
#endif

    *(volatile u32*)(0x401008B4) = 0;

    for (i = 0; i < 2000; i++);

    DESC_START = *(volatile u32*)(0x40900010);

#ifdef TIMER
    *(volatile unsigned int*)(0x40411000) = 0xFFFFFFFF;
    *(volatile unsigned int*)(0x40411008) = 0x1;
#endif
    m3_timer_init ();

    //while(1)
    //   systick_handler();

    while(1);

    return 0;
}

#define SE_BOOT_SPI_VA_BASE (0x40200000)
#define SE_SYS_CLK_VA_BASE  (0x40100100)

#define SPI_ADDR    (SE_BOOT_SPI_VA_BASE + 0x0)
#define SPI_CMD     (SE_BOOT_SPI_VA_BASE + 0x8)
#define SPI_STS     (SE_BOOT_SPI_VA_BASE + 0x18)
#define SPI_FIFO    (SE_BOOT_SPI_VA_BASE + 0x30)
#define SPI_MISC    (SE_BOOT_SPI_VA_BASE + 0x1C)

#define RETRIES 10000
int spi_bad = 0;
static void wait_for_spi_ready(void) {
    volatile int i = 0;
    while (((*(volatile unsigned int *)(SPI_STS)) & 0x01) && (i++ < RETRIES));

    if (i >= RETRIES)
    {
        m3_uart_puts("SPIBAD\n");
        spi_bad = 1;
    }
}

static void wait_till_ready(void)
{
    volatile int i = 0;
    volatile unsigned char rdsts = 0;
    wait_for_spi_ready();
    *(volatile unsigned int *)(SPI_CMD) = 0x80000105;
    wait_for_spi_ready();

    rdsts = *(volatile unsigned char*)(SPI_FIFO);

    while (((rdsts & 0x1) == 0x1) && (i++ < RETRIES))
        rdsts = *(volatile unsigned char*)(SPI_FIFO);

    if (i >= RETRIES)
    {
        m3_uart_puts("SPIBAD1\n");
        spi_bad = 1;
    }
}


void reinit_spi(void)
{
    *(volatile unsigned int*)(SPI_MISC) = 0xC0100804;
    *(volatile unsigned int*)(SPI_ADDR) = 0;

    wait_till_ready();
    // If 3B strap is set then send disable 4B mode; just-in-case
    // FIXME: The following two lines has to be un-commented in OEM-1 mode.
    //if (((*(volatile unsigned int*)(0x4010010C) & 0x40) != 0x40))
        //&& ((*(volatile unsigned int*)(0x4010010C) & 0x1) != 0x1))
    {
        //FIXME: This may not be needed for OEM-1 mode since they always are in 3B mode.
        // revisit this.
        wait_for_spi_ready();
        *(volatile unsigned int *)(SPI_CMD) = 0x80001106;
        wait_for_spi_ready();
        wait_till_ready();
        wait_for_spi_ready();
        *(volatile unsigned int *)(SPI_CMD) = 0x800011e9;
        wait_for_spi_ready();
        wait_till_ready();

        // clear BMISC
        *(volatile unsigned int*)(SPI_MISC) &= ~(1 << 24);
    }
    //*(volatile unsigned long*)(SE_SYS_CLK_VA_BASE+0x20) &= 0xFFFFF000;
    //*(volatile unsigned long*)(SE_SYS_CLK_VA_BASE+0x20) |= 0x222;

    *(volatile unsigned int *)(SPI_CMD) &= ~(0xFF000000);
    wait_for_spi_ready();
    m3_uart_puts("reinit_spi\n");
}

int check_wfe(int print)
{
    int do_again = 0;
    if ((*(volatile unsigned int*)(0x40100D00) & (1 << 25)) == (1 << 25))
    {
        if (print)
            m3_uart_puts("c0_wfe\n");
    }
    else
    {
        if (print)
            m3_uart_puts("c0_nowfe!!\n");
        do_again = 1;
    }

    if ((*(volatile unsigned int*)(0x40100D00) & (1 << 26)) == (1 << 26))
    {
        if (print)
            m3_uart_puts("c1_wfe\n");
    }
    else
    {
        if (print)
            m3_uart_puts("c1_nowfe!!\n");
        do_again = 1;
    }

    return do_again;
}

void trigger_dma(void)
{
    volatile unsigned long count1=0x0;
    volatile unsigned long count2=0; 
    volatile int j = 0;

#ifdef DEBUG_PRINTS
    m3_uart_puts ("0x40100820\n");
    print_reg(*((volatile unsigned long*)0x40100820));
    m3_uart_puts ("0x40100828\n");
    print_reg(*((volatile unsigned long*)0x40100824));
    m3_uart_puts ("0x40100828\n");
    print_reg(*((volatile unsigned long*)0x40100828));
#endif

    *(volatile unsigned long*)GDMA_CTRL = 0x1;
    *(volatile unsigned long*)GDMA_DTB=DESC_START;
    //while(((*(volatile unsigned long*)GDMA_STATUS) & 0x1) != 0){
    for(count1=0; count1< 0x3; count1++)
    {
        if (((*(volatile unsigned long*)GDMA_STATUS) & 0x1) == 0)
            break;

        for (j = 0; j < 0x200000; j++);
        //m3_uart_puts(".");

        for(count2=0; count2 < 0xDffffff; count2++)
        {
            if (((*(volatile unsigned long*)GDMA_STATUS) & 0x1) == 0)
                break;

            for (j = 0; j < 0x200000; j++);
            //m3_uart_puts(".");
        }
    }

    if((count1 == 3) && (count2 ==0xDffffff) )
        m3_uart_puts("Forcing bailout\n");
}

#define BASE                    0x40100000
#define SYSWCR                  0x890
#define SYSWRERL                0x894
#define SYSWRERH                0x898
#define SYSWCFR                 0x89C

#if 0
void start_wdt(void){
    m3_uart_puts ("\nFallback wdt\n");
    *(volatile unsigned long*)(BASE+ SYSWCR) = 0xD01;
    *(volatile unsigned long*)(BASE+ SYSWCFR) =  0x5555; 
    *(volatile unsigned long*)(BASE+ SYSWRERL) = 0x1;
    *(volatile unsigned long*)(BASE+ SYSWRERH) = 0x0;
    *(volatile unsigned long*)(BASE+ SYSWCR) = 0x800D01;
}
#endif

void systick_handler(void)
{
    volatile u32 bmc_cpu_reset = 0;
    volatile u32 val = 0;
    volatile int i = 0, j = 0;
#ifdef TIMER
    volatile unsigned int temp1, temp2;
#endif
#ifdef DEBUG_PRINTS
    volatile u32* start = (volatile u32*)(DDR_START);
#endif

    if ((*(volatile unsigned int*)(0x401008b0) & (1 << 10)) == (1 << 10))
        bmc_cpu_reset = 1;

    if (((*(volatile unsigned int*)(0x40100880) & 0x40000) != 0x40000) &&
            ((*(volatile unsigned int*)(0x40100890) & 0x40000) != 0x40000) &&
            ((*(volatile unsigned int*)(0x401008a0) & 0x40000) != 0x40000) &&
            (bmc_cpu_reset == 0))
        return;

    m3_uart_puts ("\ntriggered\n");

    //start_wdt();

#if 1
    // Disable systick
    val = (NVIC_SYSTICK_CTRL_CLKSOURCE|NVIC_SYSTICK_CTRL_TICKINT);
    *(u32 *)(NVIC_SYSTICK_CTRL) = val;
#endif

#ifdef TIMER
    temp1 = *(volatile unsigned int*)(0x40411004);
#endif
    for (i = 0; i < 0x8; i++)
        //for (i = 0; i < 0x4; i++)
    {
        for (j = 0; j < 0x200000; j++);
        // If already in WFE then just go ahead with the reset
        if (check_wfe(0) == 0)
        {
            m3_uart_puts("no_gdma\n");
            goto issue_reset;
        }
    }
#ifdef TIMER
    temp2 = *(volatile unsigned int*)(0x40411004);
    print_reg(temp1);
    print_reg(temp2);
#endif
    //Then trigger DMA
    m3_uart_puts("GDMA-1\n");
    trigger_dma();
    for (i = 0; i < 0x200000; i++);

    // Lets do it twice and see
    if (check_wfe(0) != 0)
    {
        m3_uart_puts("GDMA-2\n");
        trigger_dma();
        check_wfe(1);
    }
    else
        m3_uart_puts("NOGDMA2\n");

#ifdef DEBUG_PRINTS
    check_wfe_pattern();
#endif

issue_reset:

    reinit_spi();
    m3_uart_puts ("done\n");
    //clear pre-trigger status
    if (bmc_cpu_reset)
        *(volatile unsigned int*)(0x401008b0) |= (1 << 10);
    else
    {
        if ((*(volatile unsigned int*)(0x40100880) & 0x40000) == 0x40000)
            *(volatile unsigned int*)(0x40100880) |= 0x40000;
        if ((*(volatile unsigned int*)(0x40100890) & 0x40000) == 0x40000)
            *(volatile unsigned int*)(0x40100890) |= 0x40000;
        if ((*(volatile unsigned int*)(0x401008a0) & 0x40000) == 0x40000)
            *(volatile unsigned int*)(0x401008a0) |= 0x40000;
    }
#if 1
    // Enable systick
    val = (NVIC_SYSTICK_CTRL_CLKSOURCE|NVIC_SYSTICK_CTRL_TICKINT|NVIC_SYSTICK_CTRL_ENABLE);
    *(u32 *)(NVIC_SYSTICK_CTRL) = val;
#endif


    if (bmc_cpu_reset)
    {
        if (spi_bad)
        {
            m3_uart_puts("reset spi controller also \n");
            *(volatile unsigned int*)(0x40100858) = 0x0;
            *(volatile unsigned int*)(0x40100854) = 0x83;
            spi_bad = 0;
        }
        else{
            *(volatile unsigned int*)(0x40100858) = 0;
            *(volatile unsigned int*)(0x40100854) = 3;
        }

        *(volatile unsigned int*)(0x40100850) |= 1;
        bmc_cpu_reset=0;
    }
}

#ifdef DEBUG_PRINTS
void check_wfe_pattern(void)
{
    int i = 0;

    if (*(volatile unsigned int*)(DDR_START - 4) != 0xe320f002)
        m3_uart_puts("start is ok\n");

    // After copying check the integrity
    for (i = 0; i < DDRMEM; i++)
    {
        if (*start != 0xe320f002)
            m3_uart_puts("incorrect\n");

        start++;
    }

    *(volatile unsigned int*)(0x40100210) = i;
    m3_uart_puts("all good\n");
}
#endif

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

#include <stdint.h>

extern unsigned int _end_stack;
extern unsigned int _end_text;
extern unsigned int _start_data;
extern unsigned int _end_data;
extern unsigned int _start_bss;
extern unsigned int _end_bss;
 
extern int main(void);
extern void systick_handler(void);
 
/* Cortex M3 core interrupt handlers */
void Reset_Handler(void);
void NMI_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void HardFault_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void MemManage_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void BusFault_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void UsageFault_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void SVC_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DebugMon_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void PendSV_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void SysTick_Handler(void);
 
void Dummy_Handler(void);
void Dummy_Handler_1(void);
void Dummy_Handler_2(void);
void Dummy_Handler_3(void);
void Dummy_Handler_4(void);
void Dummy_Handler_5(void);
void Dummy_Handler_6(void);
void Dummy_Handler_7(void);

void *vector_table[] __attribute__ ((section(".vectors"))) = {
    &_end_stack,
    Reset_Handler,
    NMI_Handler,
    HardFault_Handler,
    MemManage_Handler,
    BusFault_Handler,
    UsageFault_Handler,
    0,
    0,
    0,
    0,
    SVC_Handler,
    DebugMon_Handler,
    0,
    PendSV_Handler,
    systick_handler,
    [16 ... 72] = Dummy_Handler
};

#if 0
void SysTick_Handler(void) {
    unsigned int read = 0;
    *(volatile unsigned int *)(0x40430000)=0x53;//S
    nc_printf("SysTick_Handler called\n");
    read = *(volatile unsigned int *) (0x80000004);
    read++;
	(*(volatile unsigned int *) 0x80000004) = read;

    if (read < 5)
        return;

	// This will signal the end of the test
	(*(volatile unsigned int *) 0x80000000) = 0xc001b00f;
}
#endif

void Reset_Handler(void) {
    unsigned int *src, *dst;
    /* Copy data section from flash to RAM */
    src = &_end_text;
    dst = &_start_data;
    while (dst < &_end_data)
        *dst++ = *src++;
 
    /* Clear the bss section */
    dst = &_start_bss;
    while (dst < &_end_bss)
        *dst++ = 0;
    //SystemInit();
    main();
}   
 
void Dummy_Handler(void) {
    *(volatile unsigned int *)(0x40431000)=0x30;//D
    while (1){
    *(volatile unsigned int *)(0x40431000)=0x30;//D
        ;
    }
}

#if 0
void Dummy_Handler_1(void) {
    *(volatile unsigned int *)(0x40430000)=0x31;//D
    while (1)
        ;
}
void Dummy_Handler_2(void) {
    *(volatile unsigned int *)(0x40430000)=0x32;//D
    while (1)
        ;
}
void Dummy_Handler_3(void) {
    *(volatile unsigned int *)(0x40430000)=0x33;//D
    while (1)
        ;
}
void Dummy_Handler_4(void) {
    *(volatile unsigned int *)(0x40430000)=0x34;//D
    while (1)
        ;
}
void Dummy_Handler_5(void) {
    *(volatile unsigned int *)(0x40430000)=0x35;//D
    while (1)
        ;
}
void Dummy_Handler_6(void) {
    *(volatile unsigned int *)(0x40430000)=0x36;//D
    while (1)
        ;
}
void Dummy_Handler_7(void) {
    *(volatile unsigned int *)(0x40430000)=0x37;//D
    while (1)
        ;
}
#endif

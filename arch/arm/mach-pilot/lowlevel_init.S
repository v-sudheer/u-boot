@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ 
@ Copyright (C) 2004-2014 Emulex. All rights reserved.
@ EMULEX is a trademark of Emulex.
@ www.emulex.com
@ 
@ This program is free software; you can redistribute it and/or modify it under
@ the terms of version 2 of the GNU General Public License as published by the
@ Free Software Foundation.
@ This program is distributed in the hope that it will be useful. ALL EXPRESS
@ OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING ANY IMPLIED
@ WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
@ NON-INFRINGEMENT, ARE DISCLAIMED, EXCEPT TO THE EXTENT THAT SUCH DISCLAIMERS
@ ARE HELD TO BE LEGALLY INVALID. See the GNU General Public License for more
@ details, a copy of which can be found in the file COPYING included
@ with this package.
@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#define SECURE 1
#if 0
.include  "buffer.equ"
.include  "pilot.equ"
.include  "pilot2.equ"
.include  "arm.equ"
.include  "int.equ"
.include  "ddr_cfg.equ"
.include  "psr.equ"
#endif
#include <config.h>
@;#include "include/check.h"

#define DEBUG_PRINT
#define UART_FIFO_OFFSET	0x00
#define UART_SR_OFFSET		0x14
#define UART_SR_TXFULL		0x00000010
#define CORE1_HOVER_START	0x1000FF80
#define CORE1_HOVER_SCRATCH 0x1000FF78

#define V_BIT	(1<<13)


LOADREMAP:
	ldr   pc, REMAPD_reset 
	ldr   pc, REMAPD_Undef 
	ldr   pc, REMAPD_swi 
	ldr   pc, REMAPD_pref 
	ldr   pc, REMAPD_dataabrt
	ldr   pc, REMAPD_adrexp
	ldr   pc, REMAPD_irq 
	ldr	  pc, REMAPD_fiq


stbyint_sts         = (1<<2)
stbyint_en          = (1<<3)

remap_vectors:
	ldr   pc, _reset_local
	ldr   pc, _undef_local
	ldr   pc, _swi_local
	ldr   pc, _pref_abtr_local
	ldr   pc, _data_abtr_local
	ldr   pc, _adr_exp_local
	ldr   pc, _IRQHandler_LOCAL

_reset_local:
	.word (REMAPD_reset- 0x0)

_swi_local:
	.word (REMAPD_swi - 0x0)

_undef_local:
	.word (REMAPD_Undef - 0x0)

_pref_abtr_local:
	.word (REMAPD_pref - 0x0)

_data_abtr_local:
	.word (REMAPD_dataabrt - 0x0)

_adr_exp_local:
	.word (REMAPD_adrexp - 0x0)

_IRQHandler_LOCAL:
	.word (REMAPD_irq - 0x0)

remap_vectors_offset:
	.word (remap_vectors - 0x0)

.macro get_irq_stack			@ setup IRQ stack
	ldr	sp, =0x10002000
.endm
.macro	irq_save_user_regs
	sub	sp, sp, #72
	stmia	sp, {r0 - r12}			@ Calling r0-r12
	add	r8, sp, #60			@ !! R8 NEEDS to be saved !!
						@ a reserved stack spot would
						@ be good.
	stmdb	r8, {sp, lr}^			@ Calling SP, LR
	str	lr, [r8, #0]			@ Save calling PC
	mrs	r6, spsr
	str	r6, [r8, #4]			@ Save CPSR
	str	r0, [r8, #8]			@ Save OLD_R0
	mov	r0, sp
.endm
.macro	irq_restore_user_regs
	ldmia	sp, {r0 - lr}^			@ Calling r0 - lr
	mov	r0, r0
	ldr	lr, [sp, #60]			@ Get PC
	add	sp, sp, #72
	subs	pc, lr, #4			@ return & move spsr_svc into
						@ cpsr
.endm
REMAPD_reset:
	ldr r0, =0x40430000 
	ldr r1, ='R'
	str r1, [r0]
	b .

REMAPD_Undef:
	ldr r0, =0x40430000 
	ldr r1, ='U'
	str r1, [r0]
	b .
REMAPD_swi:
	ldr r0, =0x40430000 
	ldr r1, ='S'
	str r1, [r0]
	b .
REMAPD_pref:
	ldr r0, =0x40430000 
	ldr r1, ='P'
	str r1, [r0]
	b .
REMAPD_dataabrt:
	ldr r0, =0x40430000 
	ldr r1, ='A'
	str r1, [r0]
	b .
REMAPD_adrexp:
	ldr r0, =0x40430000 
	ldr r1, ='E'
	str r1, [r0]
	b .
REMAPD_fiq:
	ldr r0, =0x40430000 
	ldr r1, ='F'
	str r1, [r0]
	b .
REMAPD_irq:
	get_irq_stack
	irq_save_user_regs
	ldr r0, =0x40430000
	ldr r1, ='I'
	str r1, [r0]
	ldr r2,=stbyint_sts
	ldr r0,=SYSCONTROL
	ldr r1,[r0]
	orr r1,r1,r2
	str r1,[r0]
	ldr r0, =0x40430000
	ldr r1, ='R'
	str r1, [r0]
	irq_restore_user_regs
	subs    r15, r14, #4
	b .

.macro  senduart,rd,rx
	str \rd, [\rx, #UART_FIFO_OFFSET]	@ TXDATA
.endm

.macro  waituart,rd,rx
.endm

.macro  busyuart,rd,rx
1002:	ldr \rd, [\rx, #UART_SR_OFFSET] @ get
		tst \rd, #UART_SR_TXFULL		@
		bne 1002b						@ wait if FIFO
.endm

.macro printch, ch
		ldr r0, =0x40430000
		ldr r1, =0
		busyuart r1, r0 
		ldr r0, =0x40430000
		ldr r1, =\ch
		senduart r1, r0 
.endm

.global lowlevel_init 
.global Enable_standbyfi_intr
.global Wait_For_PLLock
.global Switch_Clk_Pll
.global v7_invalidate_l1

TMP_STACK_PTR	=	LMEMSTART + (0xeFF0) @; Last 200 bytes of lmem is poison dont touch!!
Power_On_Reset	=	1<<1         

#**************************************
#              Low level init         #
#**************************************
.global lowlevel_init
lowlevel_init:
        /* Put secondary core to sleep */
        mrc     p15, 0, r0, c0, c0, 5           @; Read CPU     ID register
        ands    r0, r0, #0x03                   @; Mask off, leaving the CPU ID field
        blne    secondary_cpus_init_exp

@; Here wait for core1 to enter WFE mode
#if 1
wait_for_wfe:
        ldr r0, =0x40100D00 @; The bit number 27 of this register tells if core1 is in WFE
        ldr r1, =0x04000000
        ldr r2, [r0]
        and r1, r1, r2
        cmp r1, #0x0
        beq wait_for_wfe
#endif
	#ldr r13, =0x40100240
	ldr r13, =0x10008000
	stmib    r13,{r0-r12,r14}

#ifdef CONFIG_HIGH_SPEED_DEBUG_UART
        ldr r0, =0x40100100
        ldr r1, [r0]
        and r1,r1,#2
        cmp r1,#2
        beq set_baud @power on reset
@As suggested Set Bit0 of 0x40426510 and Bit 18 of 0x4010012C to use 50MHZ clock to debug UART
        ldr r0,=0x40426510
        ldr r1, [r0]
        orr r1,r1,#1
        str r1, [r0]

        ldr r0,=0x4010012C
        ldr r1,[r0]
        orr r1, r1,#(1 << 18)
        str r1,[r0]
	mov r3,#27 @r3 will have baud rate value for  115200
	b uart_init
set_baud:
	mov r3,#1 @r3 will have baud rate value for  115200
#endif

@;Initialize the debug UART on P4 FPGA
@As suggested Clear Bit0 of 0x40426510 and Clear Bit 18 of 0x4010012C to Disable 50MHZ clock to debug UART
        ldr r0,=0x40426510
        ldr r1, [r0]
        bic r1,r1,#1
        str r1, [r0]
  /*
   * The Debug UART uses 25MHz/14 = 1.786MHz, the standard clock needed is 1.846MHz. 
   * The difference in the clock is within the error margin. By shifting the point when we latch 
   * the incoming RXD to the left of the center will improves the overall error margin. 
   * This allows the transmitter clock to have more margin at the highest supported rate of 115.2K Baud rate.
   */

        ldr r0,=0x4010012C
        ldr r1,[r0]
        bic r1, r1,#(7 << 16)
        orr r1, r1,#(4 << 16)
        str r1,[r0]

#ifdef CONFIG_HIGH_SPEED_DEBUG_UART
uart_init:
#endif
	ldr r0, =0x40430008
	mov r1, #0x7
	str r1, [r0]
	ldr r0, =0x4043000c
	mov r1, #0x83
	str r1, [r0]
	ldr r0, =0x40430000
#ifdef CONFIG_HIGH_SPEED_DEBUG_UART
	mov r1,r3 @r3 will have baud rate value for  115200
#else
	mov r1, #0x1
#endif
	str r1, [r0]
	ldr r0, =0x4043000c
	mov r1, #0x3
	str r1, [r0]

@provide a new stack pointer for DDR Init
	ldr	r13,=TMP_STACK_PTR

@Pass desired DDR2 Initialization Table Here
@Skip ddrinit if the code is already executing from DDR
checkddr:
	adr r0, checkddr 
	ldr r1, =0x80000000
	and r0, r0,r1
	cmp r0, r1
	beq noddrinit
	ldr	r0,=0x401008B0
	ldr	r1,[r0]
	movw r2, #0x4E7
	TST r1,r2
	bne	noddrinit
@;
@; GIC Init
@; ---------
	bl enable_GIC @; MP_GIC.s
	mov r10, #0x24
doag:
	mov r0, r10 @; ID
	bl enable_irq_id @; MP_GIC.s

	mov r0, r10
	mov r1, #0x08 @; Set the priority level for this interrupt
	bl enable_processor_priority

	mov r0, r10
	mov r1, #0
	bl  enable_gic_processor_target

	add r10, r10, #1
	cmp r10, #0x25
	bne doag


	ldr r0, =0xFF @; Priority
	bl	set_priority_mask @; MP_GIC.s

	bl enable_gic_processor_interface @; MP_GIC.s

	cpsie i @; Enable interrupts


	@@@@load handlers at remap address
	ldr     r0,remap_vectors_offset
	@;ldr     r1,=0x40100230
	@;ldr	r10,[r1]
	@;add	r0, r0, r10
	ldr     r1,=0x40100200
	
	ldmia   r0!,{r2-r9}
	stmia   r1!,{r2-r9}

	ldmia   r0!,{r2-r9}
	@;add r2, r2, r10
	@;add r3, r3, r10
	@;add r4, r4, r10
	@;add r5, r5, r10
	@;add r6, r6, r10
	@;add r7, r7, r10
	@;add r8, r8, r10
	@;add r9, r9, r10
	stmia   r1!,{r2-r9}
	/* Set V=0 in CP15 SCTRL register - for VBAR to point to vector */
	mrc	p15, 0, r0, c1, c0, 0	@ Read CP15 SCTRL Register
	bic	r0, #V_BIT @ V = 0
	mcr	p15, 0, r0, c1, c0, 0	@ Write CP15 SCTRL Register


	/* Set vector address in CP15 VBAR register */
	ldr	r0, =0x40100200
	mcr	p15, 0, r0, c12, c0, 0	@Set VBAR
	
	ldr r0, =0x4010010C @; STRAP_STATUS
	ldr r1, [r0]
	ands r1, r1, #0x4000
	beq noespi_init
	ldr r0, =0x40100130 @; Set bit16 of IOENGCCFG and clear bit 17
	ldr r1, [r0]
	bic r1, r1, #0x20000
	orr r1, r1, #0x10000
	str r1, [r0]

	ldr r0, =0x40100150
	ldr r1, [r0]
	and r1,r1,#0xF
	cmp r1,#1
	bls pre_a2_init
    @; Add any A2 espi related code here
	ldr r0, =0x40100190
	ldr r1, [r0]
	orr r1, r1, #0x10000000
	str r1, [r0]
    @; A2 espi init code ends here
	b skip_pre_a2_init

pre_a2_init:
	@;espi init - program capabilities register for pre-A2 chips
	ldr r0, =0x40423008
	ldr r1, =0x03020003
	str r1, [r0]
	@; At this point we know it is pre A2. Directly jump to disable_msio_pre_a2
	b disable_msio_pre_a2

noespi_init:  @; In case of LPC mode check if it is pre A2 and then disable msio
	ldr r0, =0x40100150
	ldr r1, [r0]
	and r1,r1,#0xF
	cmp r1,#1
	bhi skip_pre_a2_init
disable_msio_pre_a2:
	ldr r0,=0x401009A4		@Disable MINI_SIO which was enabled by default
	mov r1, #0x80
	str r1, [r0]

	ldr r0,=0x401009A8		@Disable MINI_SIO which was enabled by default
	mov r1, #0x80
	str r1, [r0]

	ldr r0,=0x401009AC		@Disable MINI_SIO which was enabled by default
	mov r1, #0x80
	str r1, [r0]


skip_pre_a2_init:
	bl	Enable_standbyfi_intr

	ldr     r0,=0x40100108             @; SYSCONTROL Bit 31, 15. Shutoff DDR clock.
	ldr     r1,[r0]
	ldr     r2,=0x80008000
	orr     r2,r2,r1
	str     r2,[r0]	
	ldr     r0,=0x40100110             @; DDRPLLCTRL with correct divider value.

#ifdef DDR_1600MHZ
	ldr r1,=0x0027fd68
#endif
#ifdef DDR_1333MHZ
	ldr r1,=0x0027cd68
#endif
#ifdef DDR_1066MHZ
	ldr r1,=0x0027fc27
#endif
#ifdef DDR_800MHZ
	ldr r1,=0x0027fc1f
#endif
#ifdef DDR_500MHZ
	ldr r1,=0x0028fc27
#endif
#ifdef DDR_350_400MHZ
	ldr r1,=0x0028fc1f
#endif
#ifdef DDR_200_250MHZ
	ldr r1,=0x0029fc1f
#endif
#ifdef DDR_75_125MHZ
	ldr r1,=0x002bfc1f
#endif
	str r1, [r0]

#ifdef DDR_1600MHZ
	ldr	r1,=0x0007fd68
#endif
#ifdef DDR_1333MHZ
	ldr	r1,=0x0007cd68
#endif
#ifdef DDR_1066MHZ
	ldr r1,=0x0007fc27
#endif
#ifdef DDR_800MHZ
	ldr r1,=0x0007fc1f
#endif
#ifdef DDR_500MHZ
	ldr r1,=0x0008fc27
#endif
#ifdef DDR_350_400MHZ
	ldr r1,=0x0008fc1f
#endif
#ifdef DDR_200_250MHZ
	ldr r1,=0x0009fc1f
#endif
#ifdef DDR_75_125MHZ
	ldr r1,=0x000bfc1f
#endif
	str r1, [r0]

	bl	Wait_For_PLLock
#ifdef CONFIG_HIGH_SPEED_DEBUG_UART
@As suggested Set Bit0 of 0x40426510 and Bit 18 of 0x4010012C to use 50MHZ clock to debug UART
	ldr r0,=0x40426510
	ldr r1, [r0]
	orr r1,r1,#1
	str r1, [r0]

	ldr r0,=0x4010012C
	ldr r1,[r0]
	orr r1, r1,#(1 << 18)
	str r1,[r0]

        ldr r0, =0x40430008
        mov r1, #0x7
        str r1, [r0]
        ldr r0, =0x4043000c
        mov r1, #0x83
        str r1, [r0]
        ldr r0, =0x40430000
        mov r1, #27	@Default to 115200
        str r1, [r0]
        ldr r0, =0x4043000c
        mov r1, #0x3
        str r1, [r0]
#endif
	bl Switch_Clk_Pll
	@;bl invalidate_caches
	bl invalidate_icache_all
	bl invalidate_dcache_all
	bl v7_invalidate_l1

@Do clk switch for mac0 and mac1. This is required for both FPGA and ASIC
	ldr	r0,=0x40100108
	ldr	r1,[r0]
	ldr	r2,=0x1E00000
	orr	r2,r2,r1
	str	r2,[r0]



	ldr r0, =0x4010010C @; STRAP_STATUS
	ldr r1, [r0]
	ands r1, r1, #0x1
	beq NOT_OEM1
@in OEM1 mode, watchdog will keep on triggering and resets PILOT. Hence disabling watchdog 
@for firmware developement. This needs to be enabled later in production firmware.
	ldr r0, =0x401008A0	
	ldr r1, =0x10000
	str r1, [r0]
	
	@Setting MAC0 and MAC1 to RGMII mode for OEM1 mode
	ldr r0, =0x40100988	
	ldr r1, [r0]
	ldr r2, =0x180
	orr r1, r1, r2
	str r1, [r0]

	printch 'O'; printch 'E'; printch 'M'; printch '1';
	printch 'M'; printch 'O'; printch 'D'; printch 'E';  printch '-';
NOT_OEM1:
	ldr	r0,=0x401008B0
	ldr	r1,[r0]
	movw r2,#0x4E7
	TST r1,r2
	bne	noddrinit
@;Write PILOT4 on debug UART
	printch 'P'
	printch 'I'
	printch 'L'
	printch 'O'
	printch 'T'
	printch '4'
@	printch '\n'



	ldr r0,=0x40100100
	ldr r1, [r0]
	bic r1,r1,#0x8
	str r1,[r0]	//SYSCFG[3]

	mov	r0, #0x24					@; ID
	bl	disable_irq_id				@; MP_GIC.s

	ldr	r0, =0x0
	mcr	p15, 0, r0, c12, c0, 0	@Set VBAR

#ifdef CONFIG_SFDP
	@; Programming SPI timing registers to support quad mode
	ldr r1, =0x3002160e
	ldr r0, =0x4020000C
	str r1, [r0]
	ldr r0, =0x4028000C
	str r1, [r0]
	ldr r0, =0x4042900C
	str r1, [r0]
#endif

	ldr r0,=0x40100154		@;Enable the VGA OPROM for Pilot4         
	ldr r1,=0x00000600
	str r1, [r0]

	ldr r0, =0x40100130		@set bit 8 for the address aliasing issue 
    ldr r1, [r0]			@while accessing H2B PCIE device
	orr r1, r1, #0x180		@Set bit 7 To Disable prefetch for graphics
	str r1, [r0]

	ldr r0, =0x40100198		@Set bit 0 To enable G200 as device 0
	ldr r1, [r0] 
	orr r1, r1, #1
	str r1, [r0]

	ldr r0,=0x4044F214		@; Enter Non D0
	ldr r1, [r0] 
	ldr r2,=0x00000010
	orr r1, r1, r2
	str r1, [r0]

	ldr r0, =0x40100150
	ldr r1, [r0]
	and r1, r1, #0xF
	ldr r2, =0x1
	cmp r1,r2
	bhi next_cap_ptr
	ldr     r0,=0x4044F340  @Overwrtite Next CAP PTR to fix video NOT coming up on some boards
@;;	ldr     r1,=0x06030001
	ldr     r1,=0x00030001
	str     r1, [r0]
	b jumpcap
next_cap_ptr:
	ldr r0, =0x4044F340
	ldr r1, =0x00035001
	str r1, [r0]
jumpcap:
	ldr r0, =0x40100160		@Set bit 4 to enable USB 2.0 Interrupt to ARM
	ldr r1, [r0] 
	orr r1, r1, #0x10
	str r1, [r0]

	ldr r0, =0x40100820		@Enable bus-hang release
	ldr r1, [r0] 
	orr r1, r1, #0x0F
	str r1, [r0]


	bl ddrinit
	ldr r0,=0x40100100
	ldr r1, [r0]
	and r1,r1,#0x2
	str r1,[r0]               @ remap

#if SECURE
	LDR	R0,=0x10002200
	LDR	R1,=0xBADBABE5
	STR	R1,[R0]
	SEV
WAIT:
	LDR	R1,[R0]
	CMP	R1,#0
	BNE	WAIT

	MOV	R6,pc
	B	start_sec
#endif	






	b no_sw_reset

noddrinit:
	@;bl invalidate_caches
	bl invalidate_icache_all
	bl invalidate_dcache_all
	bl v7_invalidate_l1

	bl  sw_reset_modules
no_sw_reset:
	@;ldr r13, =0x40100240
	ldr r13, =0x10008000
	BL	gic_ns_init
	ldmib    r13,{r0-r12,r15}

.global secondary_cpus_init_exp
secondary_cpus_init_exp:
.ifndef  NEEDED 
   @; Copy the SMP aware code to teh scratch pad area
   LDR r0, =dat
   LDR r2, =0x0
   SUBS r0, r0, r2 
   LDR r1, =CORE1_HOVER_START
   LDMIA	r0!,{r2-r9}
   STMIA	r1!,{r2-r9}
	
   LDMIA	r0!,{r2-r9}
   STMIA	r1!,{r2-r9}

   LDR	r2, [r0]
   STR	r2, [r1]
#if SECURE
	LDR	R0,=0x401008B0
	LDR	R1,[R0]
	AND	R2,R1,#(1<<1)
	CMP	R2,#0
	BNE	no_secure_init
	MOV R6,R15
	b	start_sec

no_secure_init:
#endif
	ldr r13, =0x10008100
	bl invalidate_icache_all
	bl invalidate_dcache_all
	bl v7_invalidate_l1

	BL	gic_ns_init
 
   @; Jump to the scratch pad area
   LDR r1, =CORE1_HOVER_SCRATCH
   LDR r2, =CORE1_HOVER_START
   STR r2, [r1]
   LDR pc, [r1]
   b .
   .endif

Enable_standbyfi_intr:	
	stmfd r13!,{r0-r2,r14}	
	ldr	r0,=SYSCONTROL

@clear any of previous standby interrupt status	
	ldr	r2,=stbyint_sts
	
03:		
	ldr	r1,[r0]	
	orr	r1,r1,r2
	str	r1,[r0]

	ldr	r1,[r0]
	tst     r1,#stbyint_sts
	bne	03b

@enable stby intr	
	ldr	r2,=stbyint_en
	ldr	r1,[r0]
	orr	r1,r1,r2
	str	r1,[r0]

	ldmfd	r13!,{r0-r2,r15}


@Waits till PLL is stable	
Wait_For_PLLock:	
	stmfd  r13!,{r0-r1,r14}
	ldr     r0,=SCLKSTS
	ldr     r2,=PLLLOCK
01:
	ldr     r1,[r0]
	and     r1,r1,r2
	cmp     r1,r2
	bne     01b
	ldmfd  r13!,{r0-r1,r15}


@make sure that Enable_standbyfi_intr is called before this subroutine and appropriate handler is available
@This subroutine switches mux to PLL(500/400Mhz) mode
Switch_Clk_Pll:	
	stmfd  r13!,{r0-r1,r14}			

	ldr r1,=SCLKCTL
	ldr r2,=0xafffffff
	str	r2,[r1]
	
	WFI				@added for cortex A9 by Vikram - Wait for Interrupt
	
	ldmfd  r13!,{r0-r1,r15}

v7_invalidate_l1:
	mov r0, #0
	mcr p15, 2, r0, c0, c0, 0
	mrc p15, 1, r0, c0, c0, 0

	movw r1, #0x7fff
	and r2, r1, r0, lsr #13

	movw r1, #0x3ff

	and r3, r1, r0, lsr #3 @ NumWays - 1
	add r2, r2, #1 @ NumSets

	and r0, r0, #0x7
	add r0, r0, #4 @ SetShift

	clz r1, r3 @ WayShift
	add r4, r3, #1 @ NumWays
1: 	sub r2, r2, #1 @ NumSets--
	mov r3, r4 @ Temp = NumWays
2: 	subs r3, r3, #1 @ Temp--
	mov r5, r3, lsl r1
	mov r6, r2, lsl r0
	orr r5, r5, r6 @ Reg = (Temp<<WayShift)|(NumSets<<SetShift)
	mcr p15, 0, r5, c7, c6, 2
	bgt 2b
	cmp r2, #0
	bgt 1b
	dsb st
	isb
	bx lr



	.ltorg
	.end

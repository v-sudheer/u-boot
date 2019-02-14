/*
 * Copyright (C) 2018 Vishal Nigade <vishal.nigade@aspeedtech.com>
 * Copyright (c) 2011-2014 ARM Ltd.  All rights reserved.
 * SPDX-License-Identifier:     GPL-2.0+
 */
.equ NS_STACK_SP0,0x1E721100
.equ NS_STACK_SP1,0x1E722100
.equ MONSP0,0x1E72C000
.equ MONSP1,0x1E72D000
.equ MVBAR,0x87FF0000
.extern Welcome_Back_NS0
.extern Welcome_Back_NS1

.macro printch, ch
		ldr r0, =0x1e784000
		ldr r1, =\ch
		STR r1, [r0] 
.endm

.equ DEBUG,0

.equ Mode_MON,               0x16
.equ Mode_SVC,   0x13
.equ COLLEAGUE,   0xBABEFACE
.equ I_Bit,  0x80 @; when I bit is set, IRQ is disabled
.equ A_Bit, 0 @; when A bit is set, INT is disabled
.equ F_Bit,  0x40 @; when F bit is set, FIQ is disabled

/*SECURE START CODE*/

.align 5
.global start_sec
start_sec:
MRC     p15, 0, r0, c0, c0, 5     @; Read CPU ID register
ANDS    r0, r0, #0x03             @; Mask off, leaving the CPU ID field

BEQ     primaryWait
/**************************************************/

DO:
	wfe
	LDR	R0,=0x1E722200
	LDR	R1,=0xBADBABE5
	LDR	R2,[R0]
	CMP	R2,R1
	BNE	DO

LDR	sp,=0x1E729000
PUSH	{r6}

B	secondaryCPUsInit

/*************************************************/
primaryWait:

    LDR     R0,=0x1E722200
    LDR     R1,=0xBADBABE5
    STR     R1,[R0]
	SEV
WAIT:
    LDR     R1,[R0]
    CMP     R1,#0
    BNE     WAIT

ldr	r0, =0x80000000
mcr	p15, 0, r0, c12, c0, 0	@Set VBAR

/*Temperary stack*/
LDR	sp,=0x1E72A000
PUSH	{R6}

MRC p15, 0, R0, c1, c1, 0	@; Read Secure Configuration Register data
ORR	R0,R0,#(1<<2)		@; FIQ taken in monitor mode only
MCR p15, 0, R0, c1, c1, 0	@; Write Secure Configuration Register data

mrc p15,0,r0,c1,c1,2    @NSACR
mov r2,#1
orr r0,r0,r2, LSL #18	//SMP
orr r0,r0,r2, LSL #14	//
orr r0,r0,r2, LSL #15	//
orr r0,r0,r2, LSL #10	//
orr r0,r0,r2, LSL #11	//
mcr p15,0,r0,c1,c1,2

MRC p15, 0, R0, c1, c0, 1 @; Read Auxiliary Control Register
ORR R0, R0, #(0x1<<6)
MCR p15, 0, R0, c1, c0, 1 @; Write Auxiliary Control Register

BL	gic_init

@; Install Secure Monitor
@; -----------------------
LDR     r0, =MVBAR              @; Get address of Monitor's vector table
MCR     p15, 0, r0, c12, c0, 1       @; Write Monitor Vector Base Address Register
 
@; Initialize the Monitor mode stack pointer
CPS    #Mode_MON
LDR    sp, =MONSP0
CPS    #Mode_SVC
LDR	  r0,=NS_STACK_SP0
MOV	  r1,r6
STR     r1, [r0]                     @; Save a pointer to the top of the Normal world context
.if DEBUG
	printch 'e'
	printch 'c'
	printch 'u'
	printch 'r'
	printch 'e'
	printch ' '
	printch 'w'
	printch '0'	
	printch 'r'
	printch 'l'
	printch 'd'
	printch '!'
	printch '\n'
	printch '\r'

	MRC p15,0,r0,c0,c1,7
	AND r1,r0,#0x0F000000
	LDR r2,=0x02000000
	CMP r1,r2
	BNE thats_it
	printch 'L'
thats_it:
.endif
B	smc 

@; ------------------------------------------------------------
@; Initialization for SECONDARY CPUs
@; ------------------------------------------------------------

.global secondaryCPUsInit
secondaryCPUsInit:

MRC p15, 0, R0, c1, c1, 0	@; Read Secure Configuration Register data
ORR	R0,R0,#(1<<2)		@; FIQ taken in monitor mode only
MCR p15, 0, R0, c1, c1, 0	@; Write Secure Configuration Register data
mrc p15,0,r0,c1,c1,2    @NSACR
mov r2,#1
orr r0,r0,r2, LSL #18
orr r0,r0,r2, LSL #14
orr r0,r0,r2, LSL #15
orr r0,r0,r2, LSL #10
orr r0,r0,r2, LSL #11
mcr p15,0,r0,c1,c1,2

MRC p15, 0, R0, c1, c0, 1 @; Read Auxiliary Control Register
ORR R0, R0, #(0x1<<6)
MCR p15, 0, R0, c1, c0, 1 @; Write Auxiliary Control Register

.if DEBUG
	printch 'H'
	printch 'e'
	printch 'y'
	printch '!'
	printch 'i'
	printch 't'
	printch '''
	printch 's'
	printch ' '
	printch 'n'
	printch '0'
	printch 'n'
	printch '-'
	printch 's'
.endif
BL	gic_init

	LDR	R0,=remap_start_sec
	LDR	R1,=MVBAR
	LDR	R2,=remap_start_sec_end
	Add	R2,R2,#0x50
cp:
	LDR	R4,[R0]
	STR	R4,[R1]
	ADD	R0,R0,#4
	ADD	R1,R1,#4
	CMP	R0,R2
	BNE	cp

	LDR	R0,=0x1E722200
	MOV	R2,#0
 	STR	R2,[R0]

POP	{r6}
@; Install Secure Monitor
@; -----------------------
LDR     r0, =MVBAR             @; Get address of Monitor's vector table
MCR     p15, 0, r0, c12, c0, 1       @; Write Monitor Vector Base Address Register
@; Initialize the Monitor mode stack pointer
CPS    #Mode_MON
LDR    sp, =MONSP1
CPS    #Mode_SVC

LDR	  r0,=NS_STACK_SP1
MOV	  r1,r6
STR     r1, [r0]                     @; Save a pointer to the top of the Normal worldcontext

B smc

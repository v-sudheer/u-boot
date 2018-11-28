@; ------------------------------------------------------------
@; Cortex-A9 MPCore SMP Prime Number Generator Example
@;
@; Copyright (c) 2011-2014 ARM Ltd.  All rights reserved.
@; ------------------------------------------------------------
@;.section startup
@; Standard definitions of mode bits and interrupt (I&F) flags in PSRs
.equ WD,1
.equ MVBAR,0x80000000
.macro printch, ch
		ldr r0, =0x40430000
		ldr r1, =\ch
		STR r1, [r0] 
.endm

.equ	SYS_CTRL_BASE,	0x40100800
.equ	SYSRST_STATUS,	0xB0
.equ	SYSWCR3,	0x90
.equ	SYSWCR3_EN_BIT,	0
.equ	SYSWCR3_TR_BIT,	23
.equ	SYSWCFR3,	0x9C
.equ	SYSWRERL3,	0x94
.equ	SYSWRERH3,	0x98
.equ	DEFAULT,	0x410C00
.equ	WD_VAL,		0x08A3CF69	/*Pre=45Sec and Act=60Sec(Remmembe thing 2.261ms*val=Required Sec)*/
//.equ	WD_VAL,		0x08A033D4	/*Pre=45Sec and Act=60Sec(Remmembe thing 2.261ms*val=Required Sec)*/
.equ	RESET_MODL,	0x1		/*ARM processor*/
.equ	RESET_MODH,	0x0		/**/




.equ readIntAck,read_irq_ack
.equ writeEOI,write_end_of_irq
.equ DEBUG,0
.equ WT,1

.equ marker_sign,0x90001000
.equ sgi,10

.equ Mode_MON,               0x16
.equ Mode_USR,   0x10
.equ Mode_FIQ,   0x11
.equ Mode_IRQ,   0x12
.equ Mode_SVC,   0x13
.equ Mode_ABT,   0x17
.equ Mode_UNDEF,   0x1B
.equ Mode_SYS,   0x1F
.equ COLLEAGUE,   0xBABEFACE

.equ I_Bit,  0x80 @; when I bit is set, IRQ is disabled
@;   .equ A_Bit,  0x100 @; when A bit is set, INT is disabled
.equ A_Bit, 0 @; when A bit is set, INT is disabled

.equ F_Bit,  0x40 @; when F bit is set, FIQ is disabled



//.extern "include/irq.h"
.align 5
@; ------------------------------------------------------------
@; Reset Handler - Generic initialization, run by all CPUs
@; ------------------------------------------------------------
.global start_sec
start_sec:
MRC     p15, 0, r0, c0, c0, 5     @; Read CPU ID register
ANDS    r0, r0, #0x03             @; Mask off, leaving the CPU ID field

BEQ     primaryWait

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
@;Initializing MMU setup for CPU1 start
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.if DEBUG
//printch 'A'
.endif

MRS	R0,CPSR
BIC	R0,R0,#(I_Bit|F_Bit)
MSR	CPSR,R0

LDR	sp,=0x10009000
PUSH	{r6}

B	secondaryCPUsInit

primaryWait:

MRS	R0,CPSR
BIC	R0,R0,#(I_Bit|F_Bit)
MSR	CPSR,R0
ldr	r0, =0x80000000
mcr	p15, 0, r0, c12, c0, 0	@Set VBAR

/*Temperary stack*/
LDR	sp,=0x1000A000
PUSH	{R6}

MRC p15, 0, R0, c1, c1, 0	@; Read Secure Configuration Register data
ORR	R0,R0,#(1<<2)		@; FIQ taken in monitor mode only
@;ORR	R0,R0,#(1<<1)		@; IRQ taken in monitor mode only
@;ORR	R0,R0,#(1<<4) 		@; Set f bit modifiable	// Dont do this we compromising the security issue instead go for VCR
MCR p15, 0, R0, c1, c1, 0	@; Write Secure Configuration Register data


MRC p15, 0, R0, c1, c1, 3 	@; Read VCR data
ORR	R0,R0,#(1<<6)		@; Normal world cant modify FIQ bit and still FIQ will fired
MCR p15, 0, R0, c1, c1, 3 	@; Write VCR data

mrc p15,0,r0,c1,c1,2    @NSACR
mov r2,#1
orr r0,r0,r2, LSL #18
orr r0,r0,r2, LSL #14
orr r0,r0,r2, LSL #15
orr r0,r0,r2, LSL #10
orr r0,r0,r2, LSL #11
mcr p15,0,r0,c1,c1,2

@; Enable the SCU
@; ---------------
BL      enable_scu
@;BL    dump
@;
@; Join SMP
@; ---------
MOV     r0, #0x0                  @; Move CPU ID into r0
MOV     r1, #0xF                  @; Move 0xF (represents all four ways) into r1
BL      secure_SCU_invalidate
BL      join_smp
BL      enable_maintenance_broadcast
BL	enable_access


BL	gic_init


.if WD
/*Watch Dog Reset Enable*/
	LDR	R0,=SYS_CTRL_BASE
/*	
	LDR	R1,=0xFFFFFFFF
	STR	R1,[R0,#SYSRST_STATUS]
*/
	LDR	R1,[R0,#SYSWCR3]
	LDR	R2,=DEFAULT
	ORR	R1,R1,R2
	BIC	R1,R1,#(1<<SYSWCR3_EN_BIT)
	STR	R1,[R0,#SYSWCR3]

	LDR	R1,=WD_VAL
	STR	R1,[R0,#SYSWCFR3]
	
	bl enable_GIC @; MP_GIC.s
	mov r10, #91
doag1:
	mov r0, r10 @; ID
	bl enable_irq_id @; MP_GIC.s

	mov r0, r10
	mov r1, #0x08 @; Set the priority level for this interrupt
	bl enable_processor_priority

	mov r0, r10
	mov r1, #0
	bl  enable_gic_processor_target

	add r10, r10, #1
	cmp r10, #92
	bne doag1


	ldr r0, =0xFF @; Priority
	bl	set_priority_mask@; MP_GIC.s
	
	MOV	R0,#0x1B
	bl enable_gic_processor_interface1 @; MP_GIC.s


	LDR	R0,=SYS_CTRL_BASE



	LDR	R1,=RESET_MODL
	STR	R1,[R0,#SYSWRERL3]

	LDR	R1,=RESET_MODH
	STR	R1,[R0,#SYSWRERH3]


	LDR	R1,[R0,#SYSWCR3]
	ORR	R1,R1,#(1<<SYSWCR3_EN_BIT)
	ORR	R1,R1,#(1<<SYSWCR3_TR_BIT)
	STR	R1,[R0,#SYSWCR3]
.endif
@;
@; Branch to C lib code
@; ----------------------
.if DEBUG
printch 'H'
.endif
POP	{r6}
MOV	R0,R6
BL      monitorInit0

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


SMC #0



@; ------------------------------------------------------------
@; Initialization for SECONDARY CPUs
@; ------------------------------------------------------------

.global secondaryCPUsInit
secondaryCPUsInit:

.if DEBUG
//printch '3'
.endif
MRC p15, 0, R0, c1, c1, 0	@; Read Secure Configuration Register data
ORR	R0,R0,#(1<<2)		@; FIQ taken in monitor mode only
@;ORR	R0,R0,#(1<<4) 		@; Set f bit modifiable	// Dont do this we compromising the security issue instead go for VCR
MCR p15, 0, R0, c1, c1, 0	@; Write Secure Configuration Register data

MRC p15, 0, R0, c1, c1, 3 	@; Read VCR data
ORR	R0,R0,#(1<<6)		@; Normal world cant modify FIQ bit and still FIQ will fired
MCR p15, 0, R0, c1, c1, 3 	@; Write VCR data


mrc p15,0,r0,c1,c1,2    @NSACR
mov r2,#1
orr r0,r0,r2, LSL #18
orr r0,r0,r2, LSL #14
orr r0,r0,r2, LSL #15
orr r0,r0,r2, LSL #10
orr r0,r0,r2, LSL #11
mcr p15,0,r0,c1,c1,2

@; Enable the SCU
@; ---------------
BL      enable_scu
@;BL    dump
@;
@; Join SMP
@; ---------
MOV     r0, #0x1                  @; Move CPU ID into r0
MOV     r1, #0xF                  @; Move 0xF (represents all four ways) into r1
BL      secure_SCU_invalidate
BL      join_smp
BL      enable_maintenance_broadcast
BL	enable_access


/*
MOV     r0, #0xFF                 @; Priority	@; Do it from non secure world so that NS interrupt will also fired
BL      set_priority_mask
*/

	ldr	r0, =0x80000000
	mcr	p15, 0, r0, c12, c0, 0	@Set VBAR
DO:
	LDR	R0,=0x10002200
	LDR	R1,=0xBADBABE5
	LDR	R2,[R0]
	CMP	R2,R1
	BNE	DO

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


	LDR	R0,=0x10002200
	MOV	R2,#0
 	STR	R2,[R0]

//printch '5'
@;
@; Branch to application
@; --------------------
POP	{r6}
MOV	R0,R6
BL	monitorInit1
SMC #0




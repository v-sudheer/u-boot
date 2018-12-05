/*
 * SPDX-License-Identifier:     GPL-2.0+
 * Copyright (C) 2018 Vishal Nigade <vishal.nigade@aspeedtech.com>
 * Copyright (C) 2018 Shivah Shankar <shivahshankar.shankarnarayanrao@aspeedtech.com>
 */

.equ T1TCR,0x40411808

.equ	S_FRAME_SIZE, 72
.equ	S_PC,60


.global remap_start_sec
remap_start_sec:
	B	.
	B	.
	B	SMC_Handler
	B	.
	B	.
	B	.
	B	.
	B	.
	@; ------------------------------------------------------------
@; Handlers for unused exceptions
@; ------------------------------------------------------------
SMC_Handler:	
  	MRC     p15, 0, r0, c1, c1, 0        @; Read Secure Configuration Register data
  	TST     r0, #NS_BIT                  @; Is the NS bit set?
 	BNE	kick_out_to_ns
	MRC     p15, 0, r0, c0, c0, 5     @; Read CPU ID register
	ANDS    r0, r0, #0x03             @; Mask off, leaving the CPU ID field
	BEQ	SMC_Handler0
	LDR	R0,=0x40300084
	LDR     R1,=0x80000001
	STr	R1,[R0]
	LDR     R1,=0x80001000
	STr	R1,[R0,#4]
	B	SMC_Handler1
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


.align 5
.equ NS_STACK_SP0,0x10001100


  @; Defines used in the code
.equ Mode_MON,               0x16
.equ Mode_SVC,               0x13
.equ NS_BIT,                 0x1
@; SMC Handler
@;
@; - Detect which world executed SMC
@; - Saves state to appropriate stack
@; - Restores other worlds state
@; - Switches world
@; - Performs exception return
@; ------------------------------------------------------------

  .global SMC_Handler0
SMC_Handler0: 
  PUSH   {r0-r3}                       @; r0-r3 contain args to be passed between worlds

  LDR   r0, =NS_STACK_SP0
  LDR	  r6,[r0]
  @; Clear local monitor
  @; --------------------
  CLREX                               @; Not strictly required in this example, as not using LDREX/STREX
                                      @; However, architecturally should execute CLREX on a context switch
  MRC     p15, 0, r0, c1, c1, 0        @; Write Secure Configuration Register data
  ORR     r0, r0, #NS_BIT              @; Toggle NS bit
  MCR     p15, 0, r0, c1, c1, 0        @; Write Secure Configuration Register data
  CPS     #Mode_SVC                   @; Switch into Supervisor mode temporarily
  LDR	 sp,=0
  @; Perform exception return
  @; -------------------------
  MOV	 lr,r6
  MOV    pc, lr

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




.align 5
.equ NS_STACK_SP1,0x10002100

@; ------------------------------------------------------------
@; SMC Handler
@;
@; - Detect which world executed SMC
@; - Saves state to appropriate stack
@; - Restores other worlds state
@; - Switches world
@; - Performs exception return
@; ------------------------------------------------------------

  .global SMC_Handler1
SMC_Handler1: 
  PUSH   {r0-r3}                       @; r0-r3 contain args to be passed between worlds
  LDR   r0, =NS_STACK_SP1
  LDR	  r6,[r0]
  CLREX                               @; Not strictly required in this example, as not using LDREX/STREX
                                      @; However, architecturally should execute CLREX on a context switch
  MRC     p15, 0, r0, c1, c1, 0        @; Write Secure Configuration Register data
  ORR     r0, r0, #NS_BIT              @; Toggle NS bit
  MCR     p15, 0, r0, c1, c1, 0        @; Write Secure Configuration Register data
  @; Perform exception return
  @; -------------------------
  CPS     #Mode_SVC                   @; Switch into Supervisor mode temporarily
  LDR 	  sp,=0
  MOV	 lr,r6
  MOV    pc, lr
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.align 5
@; ------------------------------------------------------------
kick_out_to_ns:
sub     sp, sp, #S_FRAME_SIZE
stmia   sp, {r0 - r12}^                  @ Calling r0-r12
add     r8, sp, #S_PC                   @ !! R8 NEEDS to be saved !!
stmdb   r8, {sp, lr}^                   @ Calling SP, LR
str     lr, [r8, #0]                    @ Save calling PC
mrs     r6, spsr
str     r6, [r8, #4]                    @ Save CPSR
str     r0, [r8, #8]                    @ Save OLD_R0
mov     r0, sp
ldmia   sp, {r0 - lr}^                  @ Calling r0 - lr
mov     r0, r0
ldr     lr, [sp, #S_PC]                 @ Get PC
add     sp, sp, #S_FRAME_SIZE
subs    pc, lr, #0                      @ return & move spsr_svc into


.align 5



.global remap_start_sec_end
remap_start_sec_end:
	B	.
@; End of code

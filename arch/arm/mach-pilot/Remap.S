.equ T1TCR,0x40411808



.global remap_start_sec
remap_start_sec:
	B	Reset_Handler
	B	undefined_instruction
	B	SMC_Handler
	B	prefetch_abort
	B	data_abort
	B	not_used
	B	IRQ_Handler
	B	fiq
@; ------------------------------------------------------------
@; Handlers for unused exceptions
@; ------------------------------------------------------------

	.align  5
Reset_Handler:
	B	.


undefined_instruction:
	b .

	.align	5
SMC_Handler:

	MRC     p15, 0, r0, c0, c0, 5     @; Read CPU ID register
	ANDS    r0, r0, #0x03             @; Mask off, leaving the CPU ID field
	BEQ	SMC_Handler0
	LDR	R0,=0x40300084
	LDR     R1,=0x80000001
	STr	R1,[R0]
	LDR     R1,=0x80001000
	STr	R1,[R0,#4]
	B	SMC_Handler1

	.align	5
prefetch_abort:
	b	.

	.align	5
data_abort:
	b .

	.align	5
not_used:
	b .

	.align 5
IRQ_Handler:
//      PROC {r0-r12}
SUB     lr, lr, #4          @; Pre-adjust lr
SRSFD   sp!, #0x12		    @; Save lr and SPRS to IRQ mode stack
PUSH    {r0-r4, r12}        @; Save APCS corruptible registers to IRQ mode stack (and maintain 8 byte alignment)

@; Acknowledge the interrupt
BL      read_irq_ack
MOV     r4, r2
CMP	r4,#10
BNE	SKIP_IRQ
@; Write end of interrupt reg
SKIP_IRQ:
B	.
MOV     r0, r4
BL      write_end_of_irq

POP     {r0-r4, r12}        @; Restore stacked APCS registers
MOV     r2, #0x01           @; Set r2 so CPU leaves holding pen
RFEFD   sp!                 @; Return from exception

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.align	5
fiq:

sub     sp, sp, #72
stmia   sp, {r0 - r12}^                  @ Calling r0-r12
add     r8, sp, #60                  @ !! R8 NEEDS to be saved !!
                                        @ a reserved stack spot would
                                        @ be good.
stmdb   r8, {sp, lr}^                   @ Calling SP, LR
str     lr, [r8, #0]                    @ Save calling PC
mrs     r6, spsr
str     r6, [r8, #4]                    @ Save CPSR
str     r0, [r8, #8]                    @ Save OLD_R0
mov     r0, sp

@; Acknowledge the interrupt
BL      read_irq_ack
MOV     r4, r2
CMP	r4,#10
BNE	SKIP_FIQ
	MOV	r0,r4

LDR	R0,=0x40430000
MOV	R1,#'i'
STR	R1,[R0]
MOV	R1,#'t'
STR	R1,[R0]
MOV	R1,#'''
STR	R1,[R0]
MOV	R1,#'s'
STR	R1,[R0]
MOV	R1,#' '
STR	R1,[R0]


	WFE
	WFE

LDR	R0,=0x40430000
MOV	R1,#'B'
STR	R1,[R0]

	B	fin
SKIP_FIQ:
CMP	r4,#91
BNE	end

	MOV	R0,#10
	MOV	R1,#0xF
	MOV	R2,#0x1
	BL	send_sgi


wait_for_wfe:
ldr r0, =0x40100D00 @; The bit number 27 of this register tells if core1 is in WFE
ldr r1, =0x04000000
ldr r2, [r0]
and r1, r1, r2
cmp r1, #0x0
beq wait_for_wfe

LDR	R0,=0x40430000
MOV	R1,#'W
STR	R1,[R0]
MOV	R1,#'D'
STR	R1,[R0]
MOV	R1,#'T'
STR	R1,[R0]
MOV	R1,#'!'
STR	R1,[R0]
MOV	R1,#'n'
STR	R1,[R0]
MOV	R1,#'0'
STR	R1,[R0]
MOV	R1,#'w'
STR	R1,[R0]
MOV	R1,#' '
STR	R1,[R0]
MOV	R1,#'s'
STR	R1,[R0]
MOV	R1,#'l'
STR	R1,[R0]
MOV	R1,#'e'
STR	R1,[R0]
MOV	R1,#'e'
STR	R1,[R0]
MOV	R1,#'p'
STR	R1,[R0]
MOV	R1,#'\n'
STR	R1,[R0]
MOV	R1,#'\r'
STR	R1,[R0]

WFE
WFE

LDR	R0,=0x40430000
MOV	R1,#'Z'
STR	R1,[R0]

	B	fin

	@;BL	disable_timerS
@; Write end of interrupt reg
end:
LDR	r0,=0x3ff
CMP	r4,r0
BNE	fin
	MOV	r0,r4
fin:
MOV     r0, r4
BL      write_end_of_irq

B	.

ldmia   sp, {r0 - lr}^                  @ Calling r0 - lr
mov     r0, r0
ldr     lr, [sp, #60]                 @ Get PC
add     sp, sp, #72
subs    pc, lr, #4                      @ return & move spsr_svc into
                                        @ cpsr
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


.align 5
.equ NS_STACK_LIMIT0,0x10001040
.equ S_STACK_LIMIT0,0x10001080
.equ NS_STACK_SP0,0x10001100
.equ S_STACK_SP0,0x10001104


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
                                       @; Temporarily stack, so can be used as scratch regs
  @; Which world have we come from
  @; ------------------------------
  MRC     p15, 0, r0, c1, c1, 0        @; Read Secure Configuration Register data
  TST     r0, #NS_BIT                  @; Is the NS bit set?
  PUSH    {r0}                         @; Save a copy of the SCR on entry
 @; EOR     r0, r0, #NS_BIT              @; Make sure the SCR.NS bit is now clear
  BIC     r0, r0, #NS_BIT              @; Make sure the SCR.NS bit is now clear
  MCR     p15, 0, r0, c1, c1, 0        @; Write Secure Configuration Register data
  ISB

  @; Load save to pointer
  @; ---------------------
  LDREQ   r0, =S_STACK_SP0             @; If NS bit set, was in Normal world.  So restore Secure state
  LDRNE   r0, =NS_STACK_SP0
  LDR     r2, [r0]

  @; Load restore from pointer
  @; --------------------------
  LDREQ   r1, =NS_STACK_SP0
  LDRNE   r1, =S_STACK_SP0
  LDR     r3, [r1]

  @; r2  <-- save to
  @; r3  <-- restore from

  @; Save general purpose registers, SPSR, LR and SP
  @; ------------------------------------------------
  STMFD   r2!, {r4-r12}               @; Save r4 to r12
  MRS     r4, spsr                    @; Also get a copy of the SPSR_mon
  STMFD   r2!, {r4, lr}               @; Save original SPSR_mon and LR_mon

  CPS     #Mode_SVC                   @; Switch into Supervisor mode temporarily
  MOV     r4, sp                      @; Temp for SP_svc
  STMFD   r2!, {r4, lr}               @; Save SP_svc and LR_svc

  STR     r2, [r0]                    @; Save updated pointer back, r0 and r2 now free

  @; Restore other world's registers, SPSR and LR
  @; ---------------------------------------------
  LDMFD   r3!, {r4, lr}               @; Restore SP_svc and LR_svc
  MOV     sp, r4                      @; Temp for SP_svc
  CPS     #Mode_MON                   @; Switch back into Monitor mode

  LDMFD   r3!, {r0, lr}               @; Get SPSR_mon and LR_mon (return address)
  MSR     spsr_cxsf, r0               @; Restore SPSR_mon
  LDMFD   r3!, {r4-r12}               @; Restore registers r4 to r12

  STR     r3, [r1]                    @; Save updated pointer back, r1 and r3 now free

  @; Clear local monitor
  @; --------------------
  CLREX                               @; Not strictly required in this example, as not using LDREX/STREX
                                      @; However, architecturally should execute CLREX on a context switch

  @; Set SCR.NS bit correctly for world we're entering
  @; --------------------------------------------------
  POP     {r0}
  EOR     r0, r0, #NS_BIT              @; Toggle NS bit
//  ORR     r0, r0, #4              @; Toggle NS bit
  MCR     p15, 0, r0, c1, c1, 0        @; Write Secure Configuration Register data
  AND	 r0,r0,#1
  CMP	  r0,#1
  @; NoDataw restore args (r0-r3)
  @; -------------------------
  POP     {r0-r3}
  BNE	  LOAD_MON1
  MOV	  r6,lr
  CPS     #Mode_SVC                   @; Switch into Supervisor mode temporarily
  MOV	  lr,r6
 LOAD_MON1:

  @; Perform exception return
  @; -------------------------
  MOV    pc, lr

@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;




.align 5
.equ NS_STACK_LIMIT1,0x10002040
.equ S_STACK_LIMIT1,0x10002080
.equ NS_STACK_SP1,0x10002100
.equ S_STACK_SP1,0x10002104

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
                                       @; Temporarily stack, so can be used as scratch regs
  @; Which world have we come from
  @; ------------------------------
  MRC     p15, 0, r0, c1, c1, 0        @; Read Secure Configuration Register data
  TST     r0, #NS_BIT                  @; Is the NS bit set?
  PUSH    {r0}                         @; Save a copy of the SCR on entry
 @; EOR     r0, r0, #NS_BIT              @; Make sure the SCR.NS bit is now clear
  BIC     r0, r0, #NS_BIT              @; Make sure the SCR.NS bit is now clear
  MCR     p15, 0, r0, c1, c1, 0        @; Write Secure Configuration Register data
  ISB

  @; Load save to pointer
  @; ---------------------
  LDREQ   r0, =S_STACK_SP1            @; If NS bit set, was in Normal world.  So restore Secure state
  LDRNE   r0, =NS_STACK_SP1
  LDR     r2, [r0]

  @; Load restore from pointer
  @; --------------------------
  LDREQ   r1, =NS_STACK_SP1
  LDRNE   r1, =S_STACK_SP1
  LDR     r3, [r1]

  @; r2  <-- save to
  @; r3  <-- restore from

  @; Save general purpose registers, SPSR, LR and SP
  @; ------------------------------------------------
  STMFD   r2!, {r4-r12}               @; Save r4 to r12
  MRS     r4, spsr                    @; Also get a copy of the SPSR_mon
  STMFD   r2!, {r4, lr}               @; Save original SPSR_mon and LR_mon

  CPS     #Mode_SVC                   @; Switch into Supervisor mode temporarily
  MOV     r4, sp                      @; Temp for SP_svc
  STMFD   r2!, {r4, lr}               @; Save SP_svc and LR_svc

  STR     r2, [r0]                    @; Save updated pointer back, r0 and r2 now free

  @; Restore other world's registers, SPSR and LR
  @; ---------------------------------------------
  LDMFD   r3!, {r4, lr}               @; Restore SP_svc and LR_svc
  MOV     sp, r4                      @; Temp for SP_svc
  CPS     #Mode_MON                   @; Switch back into Monitor mode

  LDMFD   r3!, {r0, lr}               @; Get SPSR_mon and LR_mon (return address)
  MSR     spsr_cxsf, r0               @; Restore SPSR_mon
  LDMFD   r3!, {r4-r12}               @; Restore registers r4 to r12

  STR     r3, [r1]                    @; Save updated pointer back, r1 and r3 now free

  @; Clear local monitor
  @; --------------------
  CLREX                               @; Not strictly required in this example, as not using LDREX/STREX
                                      @; However, architecturally should execute CLREX on a context switch

  @; Set SCR.NS bit correctly for world we're entering
  @; --------------------------------------------------
  POP     {r0}
  EOR     r0, r0, #NS_BIT              @; Toggle NS bit
//  ORR     r0, r0, #4              @; Toggle NS bit
  MCR     p15, 0, r0, c1, c1, 0        @; Write Secure Configuration Register data
  AND	 r0,r0,#1
  CMP	  r0,#1
  @; NoDataw restore args (r0-r3)
  @; -------------------------
  POP     {r0-r3}
  BNE	  LOAD_MON
  MOV	  r6,lr
  CPS     #Mode_SVC                   @; Switch into Supervisor mode temporarily
  MOV	  lr,r6
 LOAD_MON:

  @; Perform exception return
  @; -------------------------
  MOV    pc, lr
@;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

.align 5

@; ------------------------------------------------------------

  @; unsigned int read_irq_ack(void)
  @; Returns the value of the Interrupt Acknowledge Register
read_irq_ack:
  MRC     p15, 4, r0, c15, c0, 0  @; Read periph base address
  LDR     r2, [r0, #0x010C]       @; Read the Interrupt Acknowledge Register  (ICCIAR)
  BX      lr

@; ------------------------------------------------------------
.align 5

  @; void write_end_of_irq(unsigned int ID)
  @; Writes ID to the End Of Interrupt register
write_end_of_irq:

  @; Get base address of private perpherial space
  MOV     r1, r0                  @; Back up passed in ID value
  MRC     p15, 4, r0, c15, c0, 0  @; Read periph base address

  STR     r1, [r0, #0x0110]       @; Write ID to the End of Interrupt register (ICCEOIR)

  BX      lr
  
@; ------------------------------------------------------------
@; SGI
@; ------------------------------------------------------------
.align 5

  @; void send_sgi(unsigned int ID, unsigned int target_list, unsigned int filter_list)@;
  @; Send a software generate interrupt
send_sgi:

  AND     r3, r0, #0x0F           @; Mask off unused bits of ID, and move to r3
  AND     r1, r1, #0x0F           @; Mask off unused bits of target_filter
  AND     r2, r2, #0x0F           @; Mask off unused bits of filter_list

  ORR     r3, r3, r1, LSL #16     @; Combine ID and target_filter
  ORR     r3, r3, r2, LSL #24     @; and now the filter list

  @; Get the address of the GIC
  MRC     p15, 4, r0, c15, c0, 0  @; Read periph base address
  ADD     r0, r0, #0x1F00         @; Add offset of the sgi_trigger reg

  STR     r3, [r0]                @; Write to the Software Generated Interrupt Register  (ICDSGIR)                                                                                 @; samar ref to tabe 1-3 a9 mpcore trm,4.3.13, table 4-1 gic arch spec                                                                                                             

  BX      lr

@; ------------------------------------------------------------
@; End of code
@; ------------------------------------------------------------



.align 5



.global remap_start_sec_end
remap_start_sec_end:
	B	.





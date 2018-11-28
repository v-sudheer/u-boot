
@;==================================================================
@; Copyright ARM Ltd 2009-2012. All rights reserved.
@;
@; Basic Secure Monitor
@;==================================================================

.align 5
.equ NS_STACK_LIMIT1,0x10002040
.equ S_STACK_LIMIT1,0x10002080
.equ NS_STACK_SP1,0x10002100
.equ S_STACK_SP1,0x10002104

.equ MONSP,0x1000D000
.equ MVBAR,0x80000000


  @; Defines used in the code
.equ Mode_MON,               0x16
.equ Mode_SVC,               0x13
.equ NS_BIT,                 0x1

@; ------------------------------------------------------------
@; Monitor Initialization
@;
@; This function is called by the Secure world code at start-up
@; to initialize the Secure Monitor.  The function returns back
@; to the Secure world.  Secure world must execute a SMC to
@; switch to the Normal world
@; ------------------------------------------------------------

  .global monitorInit1
monitorInit1:
MOV	R6,R0
  @; Install Secure Monitor
  @; -----------------------
  LDR     r0, =MVBAR             @; Get address of Monitor's vector table
  MCR     p15, 0, r0, c12, c0, 1       @; Write Monitor Vector Base Address Register
  
  @; Initialize the Monitor mode stack pointer
  CPS    #Mode_MON
  LDR    sp, =MONSP
  CPS    #Mode_SVC

  @; Create and save a "dummy" Normal world state
  @; This will be used for the first entry to the Normal world
  LDR     r1, =NS_STACK_LIMIT1          @; Get top of Normal state stack (assuming FD model)

  MOV     r2,  #0
  MOV     r3,  #0

  STMFD   r1!, {r2-r3}                 @; Save off "dummy" values for r11-r12
  STMFD   r1!, {r2-r3}                 @; Save off "dummy" values for r9-r10
  STMFD   r1!, {r2-r3}                 @; Save off "dummy" values for r7-r8
  STMFD   r1!, {r2-r3}                 @; Save off "dummy" values for r5-r6
  STMFD   r1!, {r2}                    @; Save off "dummy" values for r4
  @; NOT saving value for r0-r3, as we are using these registers to pass information between worlds

  MOV     r2,  #Mode_SVC               @; Set initial mode for the Normal world as SVC
  MOV     r3, R6  	               @; Set starting address as location on Normal world binary
  STMFD   r1!, {r2-r3}
  MOV     r2,  #0                      @; Set initial SP_svc for the Normal world
  MOV     r3,  #0                      @; Set initial LR_svc for the Normal world
  STMFD   r1!, {r2,r3}

  LDR     r0, =NS_STACK_SP1
  STR     r1, [r0]                     @; Save a pointer to the top of the Normal world context


  @; Set up the pointer to the Secure world memory saved state
  @; This buffer is currently empty, as the Secure world is running
  LDR     r0, =S_STACK_LIMIT1           @; Get address of memory for saving Secure state
  LDR     r1, =S_STACK_SP1              @; Get address of the pointer to saved Secure state
  STR     r0, [r1]                     @; Buffer currently empty, so pointer points to the top of the stack (FD stack model)


  @; Return to the Secure world code
  @; --------------------------------
  BX      lr


/*******************************************************************************
 *
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

		.include  "pilot2.equ"

        .global	Enable_stby_intr_vic
        .global	Enable_standbyfi_intr
	    .global Wait_For_PLLock
        .global	Switch_Clk_Pll
	    .global Switch_Clk_OSC
	    .global Pll_In_Standby		
        .global Disable_standbyfi_intr
	    .global Pll_In_Runmode		
	    .global	PutArm_Instby
		.global save_and_change_stack_to_intram
		.global retrive_and_change_stack_to_ddr
		.global put_arm_in_stdby
		.global Switch_Clk_DDR_OSC
		.global Switch_Clk_DDR_Pll
		.global DDR_Pll_In_Standby
		.global DDR_Pll_In_Runmode
	
Enable_stby_intr_vic:
	ldr     r0,=STANDBYINT
	ldr	r1,=VIC
	ldr     r2,[r1]
	orr     r0,r0,r2
	str	r0,[r1]
	mov   pc,r14	

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

	ldr   r1,=SCLKCTL
@	ldrb   r0,[r1]
@	ldr    r2,=0xffffffff
	ldr    r2,=0xffffffff
@	orr   r0,r0,r2
@	str   r0,[r1]
	str	r2,[r1]
	
  	WFI				@added for cortex A9 by Vikram - Wait for Interrupt
	
	ldmfd  r13!,{r0-r1,r15}
			
@make sure that Enable_standbyfi_intr is called before this subroutine and appropriate handler is available
@This subroutine switches mux to oscilator(25Mhz) mode
Switch_Clk_OSC:
	stmfd  r13!,{r0-r1,r14}
	ldr   r1,=SCLKCTL
	ldrb  r0,[r1]
	and   r0,r0,#0xfe
	strb  r0,[r1]

	MCR    15, 0, r0, c7, c0, 4	
	ldmfd  r13!,{r0-r1,r15}

@This subroutine programmes PLL in standby mode
@can be used when we switch to OSC mode	
Pll_In_Standby:
	stmfd  r13!,{r0-r2,r14}
	ldr     r0,=PLLCTL
        ldr     r2,=PLL_STDBY
        ldr     r1,[r0]
        orr     r1,r1,r2
        str     r1,[r0]
	ldmfd  r13!,{r0-r2,r15}
       
Disable_standbyfi_intr:
	stmfd r13!,{r0-r2,r14}		

	ldr	r0,=SYSCONTROL
	ldr	r2,=stbyint_en
	ldr	r1,[r0]
	bic	r1,r1,r2
	str	r1,[r0]

@clear any of previous standby interrupt status
	ldr	r0,=SYSCONTROL
	ldr	r2,=stbyint_sts
	
03:		
	ldr	r1,[r0]	
	orr	r1,r1,r2
	str	r1,[r0]

	ldr	r1,[r0]
	tst     r1,#stbyint_sts
	bne	03b

	
	ldmfd	r13!,{r0-r2,r15}

@This subroutine brings PLL out of  standby mode
Pll_In_Runmode:
	stmfd  r13!,{r0-r2,r14}
	ldr     r0,=PLLCTL
        ldr     r2,=PLL_STDBY
        ldr     r1,[r0]
        bic     r1,r1,r2
        str     r1,[r0]
	ldmfd  r13!,{r0-r2,r15}

PutArm_Instby:
	stmfd r13!,{r0-r12,r14}		

	bl Enable_standbyfi_intr
	bl Switch_Clk_OSC
	bl Disable_standbyfi_intr

	
	bl Pll_In_Standby
        
@putting arm in standby mode and waiting for any interupt source to wake up	
	MCR    15, 0, r0, c7, c0, 4

	bl	Pll_In_Runmode
	bl	Wait_For_PLLock	


	bl	Enable_standbyfi_intr
	bl	Switch_Clk_Pll
	bl	Disable_standbyfi_intr 

	ldmfd r13!,{r0-r12,r15}		


save_and_change_stack_to_intram:
	str	r13,[r0]
	subs	r0,r0,#4
	mov	r13,r0
@	ldr	r13,=INTRAMBASE_END
@	subs    r13, r13, #0x04
	mov	pc,r14


retrive_and_change_stack_to_ddr:
	ldr	r13,[r0]
	mov	pc,r14

     
put_arm_in_stdby:
@	stmfd r13!,{r0-r12,r14}	
	MCR    15, 0, r0, c7, c0, 4
	mov	pc,r14
@	ldmfd r13!,{r0-r12,r15}		

@This subroutine switches mux to oscilator(25Mhz) mode
Switch_Clk_DDR_OSC:
	stmfd  r13!,{r0-r1,r14}
	ldr   r1,=SCLKCTL
	ldrb  r0,[r1, #2]
	and   r0,r0,#0xf8
	strb  r0,[r1, #2]
	ldmfd  r13!,{r0-r1,r15}

Switch_Clk_DDR_Pll:
	stmfd  r13!,{r0-r1,r14}
	ldr   r1,=SCLKCTL
	ldrb  r0,[r1, #2]
	orr    r0,r0,#0x07
	strb  r0,[r1, #2]
	ldmfd  r13!,{r0-r1,r15}
	
DDR_Pll_In_Standby:
	stmfd  r13!,{r0-r2,r14}
	ldr     r0,=DDR_PLLCTL
        ldr     r2,=PLL_STDBY
        ldr     r1,[r0]
        orr     r1,r1,r2
        str     r1,[r0]
	ldmfd  r13!,{r0-r2,r15}
	
DDR_Pll_In_Runmode:
	stmfd  r13!,{r0-r2,r14}
	ldr     r0,=DDR_PLLCTL
    ldr     r2,=PLL_STDBY
    ldr     r1,[r0]
    bic     r1,r1,r2
    str     r1,[r0]
	ldmfd  r13!,{r0-r2,r15}

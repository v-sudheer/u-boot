.globl fast
fast:
    /* save lr */
    mov   r4, lr

/* Check VGA Type by H/W Trapping */
ldr   r0, =0x1e6e2070
ldr   r1, [r0]
ldr   r2, =0x00008000                        @ bit[15]=1 => Video device
tst   r1, r2
bne   kernel_end
b     kernel

kernel:
/* change spi clk */
ldr   r0, =0x1e620010                        @ set to fast read mode dual
ldr   r1, =0x003B0749
/*ldr   r1, =0x003B0749 				@  set to fast read mode single*/
str   r1, [r0]
 
/* check header 0x2008000C get size in r3 */
mov   r1, #0x40
ldr   r0, =0x2008000C
ldr   r3, [r0]
rev   r3, r3        
add   r3, r3, r1

/* cp 0x20080000 to (0x80008000 - 0x40) : size  use spi dma*/
/* src addr 0x20080000 */
ldr   r0, =0x1e620084      
ldr   r1, =0x20080000
str   r1, [r0]

/* dst addr 0x80007FC0 */
ldr   r0, =0x1e620088     
ldr   r1, =0x80007FC0
str   r1, [r0]

/* copy size from r3 */
ldr   r0, =0x1e62008C     
str   r3, [r0]

/* enable dma */
ldr   r0, =0x1e620080      
ldr   r1, =0x1
str   r1, [r0]
 
/* wait poll */
ldr    r0, =0x1e620008
ldr    r2, =0x00000800
wait_poll:
ldr    r1, [r0]
tst    r1, r2
beq  wait_poll

/* clear status */
ldr    r0, =0x1e620080
mov r1, #0x0
str    r1, [r0]
 
/* boot kernel pc = 0x80008000 r0 = 0, r1 = 0x22B8 , r2 = 0x80000100 */
ldr   r0, =0x0
ldr   r1, =0x22B8
ldr   r2, =0x80000100
ldr   r3, =0x80008000
mov   lr, r3
mov   pc, lr
nop 
nop
nop 

kernel_end: 
    /* restore lr */
    mov   lr, r4

    /* back to arch calling code */
    mov   pc, lr


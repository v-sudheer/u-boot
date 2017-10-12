/*
 * Board specific setup info
 *
 ******************************************************************************
 * ASPEED Technology Inc.
 * AST1220 Simulation LPDDR3 SDRAM controller initialization and calibration sequence
 *
 * Gary Hsu, <gary_hsu@aspeedtech.com>
 *
 * Release date:
 *
 * Optional define variable
 ******************************************************************************
 */

#include <config.h>
#include <version.h>

/******************************************************************************
 Calibration Macro Start
 Usable registers:
  r0, r1, r2, r3, r5, r6, r7, r8, r9, r10, r11
 ******************************************************************************/
#define ASTMMC_INIT_VER      0x01                @ 8bit verison number
#define ASTMMC_INIT_DATE     0x20171006          @ Release date

/* PATTERN_TABLE,
   init_delay_timer,
   check_delay_timer,
   clear_delay_timer,
   print_hex_char,
   print_hex_byte,
   are for DRAM calibration */

#define ASTMMC_REG_MCR10     0x00
#define ASTMMC_REG_MCR14     0x04
#define ASTMMC_REG_MCR18     0x08
#define ASTMMC_REG_MCR1C     0x0C
#define ASTMMC_REG_MCR20     0x10
#define ASTMMC_REG_MRW01     0x14
#define ASTMMC_REG_MRW02     0x18
#define ASTMMC_REG_MRW03     0x1C
#define ASTMMC_REG_MRW11     0x20
#define ASTMMC_REG_PHY54     0x24
#define ASTMMC_REG_PHY58     0x28
#define ASTMMC_REG_MCR0C     0x2C
#define ASTMMC_REG_RFCpb     0x30
#define ASTMMC_REG_RFCab     0x34

TIME_TABLE_LPDDR3_1600:
    .word   0x030C0607       @ MCR10
    .word   0x11272353       @ MCR14
    .word   0x07080407       @ MCR18
    .word   0x02130014       @ MCR1C
    .word   0x00000024       @ MCR20
    .word   0x0043010D       @ MRW01
    .word   0x001A020D       @ MRW02
    .word   0x0002030D       @ MRW03
    .word   0x00010B0D       @ MRW11
    .word   0x200060A1       @ MCR0C
    .word   0x00172323       @ MCRFCpb
    .word   0x00335353       @ MCRFCab
TIME_TABLE_LPDDR3_1866:
    .word   0x040E0608       @ MCR10
    .word   0x14492961       @ MCR14
    .word   0x08090508       @ MCR18
    .word   0x03240018       @ MCR1C
    .word   0x0000002A       @ MCR20
    .word   0x0083010D       @ MRW01
    .word   0x001C020D       @ MRW02
    .word   0x0002030D       @ MRW03
    .word   0x00010B0D       @ MRW11
    .word   0x200070A1       @ MCR0C
    .word   0x001B2929       @ MCRFCpb
    .word   0x003C6161       @ MCRFCab
TIME_TABLE_PHY_1866:
    .word   0x02492aae       @ phyr000
    .word   0x0000004c       @ phyr004
    .word   0x55e00a0a       @ phyr008
    .word   0x55e00a0a       @ phyr00c
    .word   0x55e00a0a       @ phyr010
    .word   0x55e00a0a       @ phyr014
    .word   0x20000000       @ phyr018
    .word   0x20000000       @ phyr01c
    .word   0x20000000       @ phyr020
    .word   0x20000000       @ phyr024
    .word   0x00000008       @ phyr028
    .word   0x00000000       @ phyr02c
    .word   0x00077400       @ phyr030
    .word   0x00000000       @ phyr034
    .word   0x00000400       @ phyr038
    .word   0x20000000       @ phyr03c
    .word   0x00000300       @ phyr040
    .word   0x0a0a0000       @ phyr044
    .word   0x00000000       @ phyr048
    .word   0x00003080       @ phyr04c
    .word   0x01000080       @ phyr050
    .word   0x0000000c       @ phyr054
    .word   0x00000043       @ phyr058
    .word   0x04000000       @ phyr05c
    .word   0x00000000       @ phyr060
    .word   0x00000000       @ phyr064
    .word   0x000a000a       @ phyr068
    .word   0x01060400       @ phyr06c
    .word   0x0608000A       @ phyr070
    .word   0x00fa0000       @ phyr074
    .word   0x00014107       @ phyr078
    .word   0x0E160805       @ phyr07c
    .word   0x62000B00       @ phyr080
    .word   0x09800E37       @ phyr084
    .word   0x00001000       @ phyr088
    .word   0x05701018       @ phyr08c
    .word   0x00000000       @ phyr090
    .word   0x0111ffe6       @ phyr094
    .word   0x20202020       @ phyr09c
    .word   0x20202020       @ phyr0a0
    .word   0x00002020       @ phyr0a4
    .word   0x80000000       @ phyr0a8
    .word   0x00000001       @ phyr0ac
    .word   0x09090909       @ phyr0cc
    .word   0x09090909       @ phyr0d0
    .word   0x48484848       @ phyr0d4
    .word   0x48484848       @ phyr0d8
    .word   0x87878787       @ phyr188
    .word   0x87878787       @ phyr18c
    .word   0x87878787       @ phyr190
    .word   0x87878787       @ phyr194
    .word   0x88888888       @ phyr1f8
    .word   0x88888888       @ phyr1fc
    .word   0x00000000       @ phyr200
    .word   0x08060000       @ phyr098
    .word   0x00000000       @ phyr0b0
    .word   0x00000000       @ phyr0b4
    .word   0x00000000       @ phyr0b8
    .word   0x00000000       @ phyr0bc
    .word   0x00000000       @ phyr0c0
    .word   0x00000000       @ phyr0c4
    .word   0x00000000       @ phyr0c8
    .word   0x0014ff2c       @ phyr0c8
    .word   0x00000000       @ phyr0dc
    .word   0x00000000       @ phyr0e0
    .word   0xaa55aa55       @ phyr0e4
    .word   0x55aa55aa       @ phyr0e8
    .word   0xaaaa5555       @ phyr0ec
    .word   0x5555aaaa       @ phyr0f0
    .word   0xaa55aa55       @ phyr0f4
    .word   0x55aa55aa       @ phyr0f8
    .word   0xaaaa5555       @ phyr0fc
    .word   0x5555aaaa       @ phyr100
    .word   0xaa55aa55       @ phyr104
    .word   0x55aa55aa       @ phyr108
    .word   0xaaaa5555       @ phyr10c
    .word   0x5555aaaa       @ phyr110
    .word   0xaa55aa55       @ phyr114
    .word   0x55aa55aa       @ phyr118
    .word   0xaaaa5555       @ phyr11c
    .word   0x5555aaaa       @ phyr120
    .word   0x3f3f3f3f       @ phyr124
    .word   0x3f3f3f3f       @ phyr128
    .word   0x3f3f3f3f       @ phyr12c
    .word   0x3f3f3f3f       @ phyr130
    .word   0x3f3f3f3f       @ phyr134
    .word   0x3f3f3f3f       @ phyr138
    .word   0x3f3f3f3f       @ phyr13c
    .word   0x3f3f3f3f       @ phyr140
    .word   0x3f3f3f3f       @ phyr144
    .word   0x3f3f3f3f       @ phyr148
    .word   0x3f3f3f3f       @ phyr14c
    .word   0x3f3f3f3f       @ phyr150
    .word   0x3f3f3f3f       @ phyr154
    .word   0x3f3f3f3f       @ phyr158
    .word   0x3f3f3f3f       @ phyr15c
    .word   0x3f3f3f3f       @ phyr160
    .word   0x3f3f3f3f       @ phyr164
    .word   0x3f3f3f3f       @ phyr168
    .word   0x3f3f3f3f       @ phyr16c
    .word   0x3f3f3f3f       @ phyr170
    .word   0x3f3f0000       @ phyr198
    .word   0x3f3f3f3f       @ phyr19c
    .word   0x3f3f3f3f       @ phyr1a0
    .word   0x3f3f3f3f       @ phyr1a4
    .word   0x3f3f3f3f       @ phyr1a8
    .word   0x3f3f3f3f       @ phyr1ac
    .word   0x3f3f3f3f       @ phyr1b0
    .word   0x3f3f3f3f       @ phyr1b4
    .word   0x3f3f3f3f       @ phyr1b8
    .word   0x3f3f3f3f       @ phyr1bc
    .word   0x3f3f3f3f       @ phyr1c0
    .word   0x3f3f3f3f       @ phyr1c4
    .word   0x3f3f3f3f       @ phyr1c8
    .word   0x3f3f3f3f       @ phyr1cc
    .word   0x3f3f3f3f       @ phyr1d0
    .word   0x3f3f3f3f       @ phyr1d4
    .word   0x3f3f3f3f       @ phyr1d8
    .word   0x3f3f3f3f       @ phyr1dc
    .word   0x3f3f3f3f       @ phyr1e0
    .word   0x3f3f3f3f       @ phyr1e4
    .word   0x00003f3f       @ phyr1e8
    .word   0xaeedaeed       @ end

PATTERN_TABLE:
    .word   0xff00ff00
    .word   0xcc33cc33
    .word   0xaa55aa55
    .word   0x88778877
    .word   0x92cc4d6e       @ 5
    .word   0x543d3cde
    .word   0xf1e843c7
    .word   0x7c61d253
    .word   0x00000000       @ 8

    .macro init_delay_timer
    ldr r0, =0x1e782024                          @ Set Timer3 Reload
    str r2, [r0]

    ldr r0, =0x1e6c0038                          @ Clear Timer3 ISR
    ldr r1, =0x01000000
    str r1, [r0]

    ldr r0, =0x1e782030                          @ Enable Timer3
    mov r2, #7
    mov r1, r2, lsl #8
    str r1, [r0]

    ldr r0, =0x1e6c0090                          @ Check ISR for Timer3 timeout
    .endm

    .macro check_delay_timer
    ldr r1, [r0]
    bic r1, r1, #0xFEFFFFFF
    mov r2, r1, lsr #24
    cmp r2, #0x01
    .endm

    .macro clear_delay_timer
    ldr r0, =0x1e78203C                          @ Disable Timer3
    mov r2, #0xF
    mov r1, r2, lsl #8
    str r1, [r0]

    ldr r0, =0x1e6c0038                          @ Clear Timer3 ISR
    ldr r1, =0x01000000
    str r1, [r0]
    .endm

    .macro print_hex_char
    and   r1, r1, #0xF
    cmp   r1, #9
    addgt r1, r1, #0x37
    addle r1, r1, #0x30
    str   r1, [r0]
    .endm

    .macro print_hex_byte
    ldr   r0, =0x1e783000
    mov   r1, r2, lsr #4
    print_hex_char
    mov   r1, r2
    print_hex_char
    .endm

/******************************************************************************
 Calibration Macro End
 ******************************************************************************/

.globl lowlevel_init
lowlevel_init:

    /* save lr */
    mov   r4, lr

    /*Set Scratch register Bit 7 before initialize*/
    ldr r0, =0x1e6e2000
    ldr r1, =0x1688a8a8
    str r1, [r0]

    ldr r0, =0x1e6e2040
    ldr r1, [r0]
    orr r1, r1, #0x80
    str r1, [r0]

/******************************************************************************
 Disable WDT3 for SPI Address mode detection function
 ******************************************************************************/
    ldr   r0, =0x1e78504c
    mov   r1, #0
    str   r1, [r0]

/* Debug - UART console message */
    ldr   r0, =0x1e78300c
    mov   r1, #0x83
    str   r1, [r0]

    ldr   r0, =0x1e6e202c
    ldr   r2, [r0]
    mov   r2, r2, lsr #12
    tst   r2, #0x01
    ldr   r0, =0x1e783000
    moveq r1, #0x0D                              @ Baudrate 115200
    movne r1, #0x01                              @ Baudrate 115200, div13
    str   r1, [r0]

    ldr   r0, =0x1e783004
    mov   r1, #0x00
    str   r1, [r0]

    ldr   r0, =0x1e78300c
    mov   r1, #0x03
    str   r1, [r0]

    ldr   r0, =0x1e783008
    mov   r1, #0x07
    str   r1, [r0]

    ldr   r0, =0x1e783000
    mov   r1, #0x0D                              @ '\r'
    str   r1, [r0]
    mov   r1, #0x0A                              @ '\n'
    str   r1, [r0]
    mov   r1, #0x44                              @ 'D'
    str   r1, [r0]
    mov   r1, #0x52                              @ 'R'
    str   r1, [r0]
    mov   r1, #0x41                              @ 'A'
    str   r1, [r0]
    mov   r1, #0x4D                              @ 'M'
    str   r1, [r0]
    mov   r1, #0x20                              @ ' '
    str   r1, [r0]
    mov   r1, #0x49                              @ 'I'
    str   r1, [r0]
    mov   r1, #0x6E                              @ 'n'
    str   r1, [r0]
    mov   r1, #0x69                              @ 'i'
    str   r1, [r0]
    mov   r1, #0x74                              @ 't'
    str   r1, [r0]
    mov   r1, #0x2D                              @ '-'
    str   r1, [r0]
    mov   r1, #0x4C                              @ 'L'
    str   r1, [r0]
    mov   r1, #0x50                              @ 'P'
    str   r1, [r0]
    ldr   r0, =0x1e783014
wait_print_0:
    ldr   r1, [r0]
    tst   r1, #0x40
    beq   wait_print_0
    ldr   r0, =0x1e783000
    mov   r1, #0x44                              @ 'D'
    str   r1, [r0]
    mov   r1, #0x44                              @ 'D'
    str   r1, [r0]
    mov   r1, #0x52                              @ 'R'
    str   r1, [r0]
/* Debug - UART console message */

    clear_delay_timer

    /* Delay about 5us */
    ldr   r2, =0x00000005                        @ Set Timer3 Reload = 5 us
    init_delay_timer
delay_0:
    check_delay_timer
    bne   delay_0
    clear_delay_timer
    /* end delay 5us */
/**************************************************************************************************/
/**************************************************************************************************/
/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #0x33                              @ '3'
    str   r1, [r0]
    mov   r1, #0x0D                              @ '\r'
    str   r1, [r0]
    mov   r1, #0x0A                              @ '\n'
    str   r1, [r0]
/* Debug - UART console message */

    /* Reset MMC */
reset_mmc:
    ldr   r0, =0x1e6e2004
    ldr   r1, [r0]
    orr   r2, r1, #0x01
    str   r2, [r0]
    ldr   r2, [r0]
    str   r1, [r0]

    /* Delay about 5us */
    ldr   r2, =0x00000005                        @ Set Timer3 Reload = 5 us
    init_delay_timer
delay_reset:
    check_delay_timer
    bne   delay_reset
    clear_delay_timer
    /* end delay 5us */

/*  ldr   r0, =0x1e6e2070
    ldr   r1, [r0]
    mov   r1, r1, lsr #7
    and   r1, r1, #0x01
    adrl  r5, TIME_TABLE_LPDDR3_1600             @ Init DRAM parameter table
    cmp   r1, #0x0
    beq   init_start */
    adrl  r5, TIME_TABLE_LPDDR3_1866             @ Init DRAM parameter table

init_start:
    ldr   r0, =0x1e6e0000
    ldr   r1, =0xfc600309
    str   r1, [r0]

    ldr   r0, =0x1e6e0030
    ldr   r1, =0x00040080
    str   r1, [r0]

    ldr   r0, =0x1e6e0034
    ldr   r1, =0x00000000
    str   r1, [r0]

    ldr   r0, =0x1e6e0038
    ldr   r1, =0xFFFFFFFF
    str   r1, [r0]

    ldr   r0, =0x1e6e003c
    ldr   r1, =0x00000000
    str   r1, [r0]

    ldr   r0, =0x1e6e0040
    ldr   r1, =0x88888888
    str   r1, [r0]
    ldr   r0, =0x1e6e0044
    str   r1, [r0]
    ldr   r0, =0x1e6e0048
    str   r1, [r0]
    ldr   r0, =0x1e6e004c
    str   r1, [r0]

    ldr   r0, =0x1e6e0070
    ldr   r1, =0x00000000
    str   r1, [r0]
    ldr   r0, =0x1e6e0074
    str   r1, [r0]
    ldr   r0, =0x1e6e0078
    str   r1, [r0]
    ldr   r0, =0x1e6e007c
    str   r1, [r0]

    ldr   r0, =0x1e6e0004
    ldr   r1, =0x00000002                        @ 16Gb
    str   r1, [r0]

    ldr   r0, =0x1e6e0010                        @ ACTIME1
    ldr   r1, [r5, #ASTMMC_REG_MCR10]
    str   r1, [r0]

    ldr   r0, =0x1e6e0014                        @ ACTIME2
    ldr   r1, [r5, #ASTMMC_REG_MCR14]
    str   r1, [r0]

    ldr   r0, =0x1e6e0018                        @ ACTIME3
    ldr   r1, [r5, #ASTMMC_REG_MCR18]
    str   r1, [r0]

    ldr   r0, =0x1e6e001c                        @ ACTIME4
    ldr   r1, [r5, #ASTMMC_REG_MCR1C]
    str   r1, [r0]

    ldr   r0, =0x1e6e0020                        @ ACTIME5
    ldr   r1, [r5, #ASTMMC_REG_MCR20]
    str   r1, [r0]

    /* Init DDR PHY */
    ldr   r0, =0x1e6e0100                        @ phyr start address
    ldr   r2, =0xaeedaeed                        @ phyr end pattern
    adrl  r3, TIME_TABLE_PHY_1866                @ phyr init value
ddrphy_init_start:
    ldr   r1, [r3]
    cmp   r1, r2
    beq   ddrphy_init_end
    str   r1, [r0]
    add   r0, r0, #4
    add   r3, r3, #4
    b     ddrphy_init_start

ddrphy_init_end:
    ldr   r0, =0x1e6e0060                        @ Fire PHY Init
    ldr   r1, =0x00000005
    str   r1, [r0]

    ldr   r2, =0x00000010
check_phypll_lock:
    ldr   r1, [r0]
    tst   r1, r2
    beq   check_phypll_lock

/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #0x50                              @ 'P'
    str   r1, [r0]
    mov   r1, #0x48                              @ 'H'
    str   r1, [r0]
    mov   r1, #0x59                              @ 'Y'
    str   r1, [r0]
    mov   r1, #0x50                              @ 'P'
    str   r1, [r0]
    mov   r1, #0x4C                              @ 'L'
    str   r1, [r0]
    mov   r1, #0x4C                              @ 'L'
    str   r1, [r0]
    mov   r1, #0x20                              @ ' '
    str   r1, [r0]
    mov   r1, #0x4C                              @ 'L'
    str   r1, [r0]
    mov   r1, #0x6F                              @ 'o'
    str   r1, [r0]
    mov   r1, #0x63                              @ 'c'
    str   r1, [r0]
    mov   r1, #0x6B                              @ 'k'
    str   r1, [r0]
    mov   r1, #0x65                              @ 'e'
    str   r1, [r0]
    mov   r1, #0x64                              @ 'd'
    str   r1, [r0]
    mov   r1, #0x0D                              @ '\r'
    str   r1, [r0]
    mov   r1, #0x0A                              @ '\n'
    str   r1, [r0]
/* Debug - UART console message */

    ldr   r0, =0x1e6e0060
    ldr   r2, =0x00000001
check_phyinit_done:
    ldr   r1, [r0]
    tst   r1, r2
    bne   check_phyinit_done

/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #0x50                              @ 'P'
    str   r1, [r0]
    mov   r1, #0x48                              @ 'H'
    str   r1, [r0]
    mov   r1, #0x59                              @ 'Y'
    str   r1, [r0]
    mov   r1, #0x49                              @ 'I'
    str   r1, [r0]
    mov   r1, #0x6E                              @ 'n'
    str   r1, [r0]
    mov   r1, #0x69                              @ 'i'
    str   r1, [r0]
    mov   r1, #0x74                              @ 't'
    str   r1, [r0]
    mov   r1, #0x20                              @ ' '
    str   r1, [r0]
    mov   r1, #0x44                              @ 'D'
    str   r1, [r0]
    mov   r1, #0x6F                              @ 'o'
    str   r1, [r0]
    mov   r1, #0x6E                              @ 'n'
    str   r1, [r0]
    mov   r1, #0x65                              @ 'e'
    str   r1, [r0]
    mov   r1, #0x0D                              @ '\r'
    str   r1, [r0]
    mov   r1, #0x0A                              @ '\n'
    str   r1, [r0]
/* Debug - UART console message */

    ldr   r0, =0x1e6e0030                        @ PWRCTL, 1st enable CKE, wait at least 200 us
    ldr   r1, =0x00060081
    str   r1, [r0]

    /* Delay about 200us */
    ldr   r2, =0x000000CA                        @ Set Timer3 Reload = 200 us
    init_delay_timer
delay_1:
    check_delay_timer
    bne   delay_1
    clear_delay_timer
    /* end delay 200us */

    ldr   r0, =0x1e6e0024                        @ MRW Reset, wait at least 10 us for MRW command
    ldr   r1, =0x00FC3F0D
    str   r1, [r0]

    /* Delay about 10us */
    ldr   r2, =0x0000000B                        @ Set Timer3 Reload = 10 us
    init_delay_timer
delay_2:
    check_delay_timer
    bne   delay_2
    clear_delay_timer
    /* end delay 10us */

    ldr   r0, =0x1e6e0024                        @ ZQinit, wait at least 1 us for ZQinit done [CS0]
    ldr   r1, =0x00FF0A05
    str   r1, [r0]

    /* Delay about 2us */
    ldr   r2, =0x00000002                        @ Set Timer3 Reload = 2 us
    init_delay_timer
delay_3:
    check_delay_timer
    bne   delay_3
    clear_delay_timer
    /* end delay 2us */

    ldr   r0, =0x1e6e0024                        @ ZQinit, wait at least 1 us for ZQinit done [CS1]
    ldr   r1, =0x00FF0A09
    str   r1, [r0]

    /* Delay about 2us */
    ldr   r2, =0x00000002                        @ Set Timer3 Reload = 2 us
    init_delay_timer
delay_4:
    check_delay_timer
    bne   delay_4
    clear_delay_timer
    /* end delay 2us */

    ldr   r0, =0x1e6e0024                        @ set MR1
    ldr   r1, [r5, #ASTMMC_REG_MRW01]
    str   r1, [r0]

    ldr   r0, =0x1e6e0024                        @ set MR2
    ldr   r1, [r5, #ASTMMC_REG_MRW02]
    str   r1, [r0]

    ldr   r0, =0x1e6e0024                        @ set MR3
    ldr   r1, [r5, #ASTMMC_REG_MRW03]
    str   r1, [r0]

    ldr   r0, =0x1e6e0024                        @ set MR11
    ldr   r1, [r5, #ASTMMC_REG_MRW11]
    str   r1, [r0]

check_MR0:
    mov   r3, #0x0                               @ read MR0
    b     read_MR_process
read_MR0_done:
    tst   r2, #0x01
    bne   check_MR0

    mov   r3, #0x4                               @ read MR4
    b     read_MR_process
read_MR4_done:
    mov   r3, #0x5                               @ read MR5
    b     read_MR_process
read_MR5_done:
    mov   r3, #0x6                               @ read MR6
    b     read_MR_process
read_MR6_done:
    mov   r3, #0x7                               @ read MR7
    b     read_MR_process
read_MR7_done:
    mov   r3, #0x8                               @ read MR8
    b     read_MR_process
read_MR8_done:

    ldr   r0, =0x1e6e000C
    ldr   r1, [r5, #ASTMMC_REG_MCR0C]
    str   r1, [r0]

    ldr   r0, =0x1e6e0030
    ldr   r1, =0x00033783
    str   r1, [r0]

    b     Calibration_End

.LTORG
/******************************************************************************
 Common Process
 *****************************************************************************/
read_MR_process:
/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #0x4D                              @ 'M'
    str   r1, [r0]
    mov   r1, #0x52                              @ 'R'
    str   r1, [r0]
    mov   r2, r3
    print_hex_byte
    mov   r1, #0x3D                              @ '='
    str   r1, [r0]
/* Debug - UART console message */

    ldr   r0, =0x1e6e0024
    mov   r1, r3, lsl #8
    orr   r1, r1, #0x2
    str   r1, [r0]

read_MR_check_loop:
    ldr   r1, [r0]
    bic   r2, r1, #0xFFFFFF3F
    cmp   r2, #0x80
    bne   read_MR_check_loop
    mov   r2, #0x0
    str   r2, [r0]
    mov   r2, r1, lsr #24

/* Debug - UART console message */
    print_hex_byte
    mov   r1, #0x0D                              @ '\r'
    str   r1, [r0]
    mov   r1, #0x0A                              @ '\n'
    str   r1, [r0]
    ldr   r0, =0x1e783014
wait_print_2:
    ldr   r1, [r0]
    tst   r1, #0x40
    beq   wait_print_2
/* Debug - UART console message */

    cmp   r3, #0x0
    beq   read_MR0_done
    cmp   r3, #0x4
    beq   read_MR4_done
    cmp   r3, #0x5
    beq   read_MR5_done
    cmp   r3, #0x6
    beq   read_MR6_done
    cmp   r3, #0x7
    beq   read_MR7_done
    cmp   r3, #0x8
    beq   read_MR8_done
    b     check_MR0

/******************************************************************************
 Other features configuration
 *****************************************************************************/
Calibration_End:

    /*******************************
     Check DRAM Size
     4Gb : 0x80000000 ~ 0x9FFFFFFF
     8Gb : 0x80000000 ~ 0xBFFFFFFF
     16Gb: 0x80000000 ~ 0xFFFFFFFF
    *******************************/
    ldr   r0, =0x1e6e0004
    ldr   r6, [r0]
    bic   r6, r6, #0x00000003                    @ record MCR04
    ldr   r7, [r5, #ASTMMC_REG_RFCpb]
    ldr   r8, [r5, #ASTMMC_REG_RFCab]

check_dram_size:
    ldr   r0, =0xC0100000
    ldr   r1, =0x41424344
    str   r1, [r0]
    ldr   r0, =0xA0100000
    ldr   r1, =0x35363738
    str   r1, [r0]
    ldr   r0, =0x80100000
    ldr   r1, =0x292A2B2C
    str   r1, [r0]
    ldr   r0, =0xC0100000
    ldr   r1, =0x41424344
    ldr   r2, [r0]
    cmp   r2, r1                                 @ == 16Gbit
    orreq r6, r6, #0x02
    mov   r3, #0x16                              @ '16'
    beq   check_dram_size_end
    ldr   r0, =0xA0100000
    ldr   r1, =0x35363738
    ldr   r2, [r0]
    cmp   r2, r1                                 @ == 8Gbit
    orreq r6, r6, #0x01
    moveq r7, r7, lsr #8
    moveq r8, r8, lsr #8
    mov   r3, #0x08                              @ '8'
    beq   check_dram_size_end
    moveq r7, r7, lsr #16                        @ == 4Gbit
    moveq r8, r8, lsr #16
    mov   r3, #0x04                              @ '4'

check_dram_size_end:
    ldr   r0, =0x1e6e0004
    str   r6, [r0]
    ldr   r0, =0x1e6e0014
    ldr   r1, [r0]
    bic   r1, r1, #0x000000FF
    bic   r1, r1, #0x0000FF00
    and   r7, r7, #0xFF
    and   r8, r8, #0xFF
    orr   r1, r1, r8
    orr   r1, r1, r7, lsl #8
    str   r1, [r0]

    /* Version Number */
    ldr   r0, =0x1e6e0004
    ldr   r1, [r0]
    mov   r2, #ASTMMC_INIT_VER
    orr   r1, r1, r2, lsl #20
    str   r1, [r0]

    ldr   r0, =0x1e6e0088
    ldr   r1, =ASTMMC_INIT_DATE
    str   r1, [r0]

/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #0x53                              @ 'S'
    str   r1, [r0]
    mov   r1, #0x69                              @ 'i'
    str   r1, [r0]
    mov   r1, #0x7A                              @ 'z'
    str   r1, [r0]
    mov   r1, #0x65                              @ 'e'
    str   r1, [r0]
    mov   r1, #0x2D                              @ '-'
    str   r1, [r0]
    mov   r1, r3, lsr #4
    print_hex_char
    mov   r1, r3
    print_hex_char
    mov   r1, #0x47                              @ 'G'
    str   r1, [r0]
    mov   r1, #0x62                              @ 'b'
    str   r1, [r0]
    mov   r1, #0x2D                              @ '-'
    str   r1, [r0]
/* Debug - UART console message */

    /********************************************
     DDRTest
    ********************************************/
ddr_test_start:
    ldr   r0, =0x1e6e0074
    ldr   r1, =0x000003FF                        @ test size = 16MB
    str   r1, [r0]
    ldr   r0, =0x1e6e007c
    ldr   r1, =0xFF00FF00
    str   r1, [r0]

ddr_test_single:
    mov   r6, #0x00                              @ initialize loop index, r1 is loop index
ddr_test_single_loop:
/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, r6
    print_hex_char
/* Debug - UART console message */
    ldr   r0, =0x1e6e0070
    ldr   r2, =0x00000000
    str   r2, [r0]
    mov   r2, r6, lsl #3
    orr   r2, r2, #0x85                          @ test command = 0x85 | (datagen << 3)
    str   r2, [r0]
    ldr   r3, =0x3000
    ldr   r11, =0x500000
ddr_wait_engine_idle_0:
    subs  r11, r11, #1
    beq   ddr_test_fail
    ldr   r2, [r0]
    tst   r2, r3                                 @ D[12] = idle bit
    beq   ddr_wait_engine_idle_0

    ldr   r0, =0x1e6e0070                        @ read fail bit status
    ldr   r3, =0x2000
    ldr   r2, [r0]
    tst   r2, r3                                 @ D[13] = fail bit
    bne   ddr_test_fail

    add   r6, r6, #1                             @ increase the test mode index
    cmp   r6, #0x08                              @ test 8 modes
    bne   ddr_test_single_loop

ddr_test_burst:
    mov   r6, #0x00                              @ initialize loop index, r1 is loop index
ddr_test_burst_loop:
/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, r6
    print_hex_char
/* Debug - UART console message */
    ldr   r0, =0x1e6e0070
    ldr   r2, =0x00000000
    str   r2, [r0]
    mov   r2, r6, lsl #3
    orr   r2, r2, #0xC1                          @ test command = 0xC1 | (datagen << 3)
    str   r2, [r0]
    ldr   r3, =0x3000
    ldr   r11, =0x500000
ddr_wait_engine_idle_1:
    subs  r11, r11, #1
    beq   ddr_test_fail
    ldr   r2, [r0]
    tst   r2, r3                                 @ D[12] = idle bit
    beq   ddr_wait_engine_idle_1

    ldr   r0, =0x1e6e0070                        @ read fail bit status
    ldr   r3, =0x2000
    ldr   r2, [r0]
    tst   r2, r3                                 @ D[13] = fail bit
    bne   ddr_test_fail

    add   r6, r6, #1                             @ increase the test mode index
    cmp   r6, #0x08                              @ test 8 modes
    bne   ddr_test_burst_loop

    ldr   r0, =0x1e6e0070
    ldr   r1, =0x00000000
    str   r1, [r0]
    b     set_scratch                            @ CBRTest() return(1)

ddr_test_fail:
/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #0x46                              @ 'F'
    str   r1, [r0]
    mov   r1, #0x61                              @ 'a'
    str   r1, [r0]
    mov   r1, #0x69                              @ 'i'
    str   r1, [r0]
    mov   r1, #0x6C                              @ 'l'
    str   r1, [r0]
    mov   r1, #0x0D                              @ '\r'
    str   r1, [r0]
    mov   r1, #0x0A                              @ '\n'
    str   r1, [r0]
    ldr   r0, =0x1e783014
wait_print_1:
    ldr   r1, [r0]
    tst   r1, #0x40
    beq   wait_print_1
/* Debug - UART console message */
    b     reset_mmc

set_scratch:
    /*Set Scratch register Bit 6 after ddr initial finished */
    ldr   r0, =0x1e6e2040
    ldr   r1, [r0]
    orr   r1, r1, #0x41
    str   r1, [r0]

/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #0x44                              @ 'D'
    str   r1, [r0]
    mov   r1, #0x6F                              @ 'o'
    str   r1, [r0]
    mov   r1, #0x6E                              @ 'n'
    str   r1, [r0]
    mov   r1, #0x65                              @ 'e'
    str   r1, [r0]
    mov   r1, #0x0D                              @ '\r'
    str   r1, [r0]
    mov   r1, #0x0A                              @ '\n'
    str   r1, [r0]
/* Debug - UART console message */

platform_exit:

    /* restore lr */
    mov   lr, r4

    /* back to arch calling code */
    mov   pc, lr


/*
 * Board specific setup info
 *
 ******************************************************************************
 * ASPEED Technology Inc.
 * AST1220 LPDDR3 SDRAM controller initialization and calibration sequence
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

//#define CONFIG_DRAM_1333
//#define CONFIG_DRAM_1600
//#define CONFIG_DRAM_1700
#define CONFIG_DRAM_1800
//#define CONFIG_DRAM_1824
//#define CONFIG_DRAM_1866

#if   defined(CONFIG_DRAM_1333)
  #define CONFIG_MPLL_PARAM1 0x1008206E
  #define CONFIG_MPLL_PARAM2 0x00000037
  #define ASTMMC_PARAM_1600
#elif defined(CONFIG_DRAM_1600)
  #define CONFIG_MPLL_PARAM1 0x10004063
  #define CONFIG_MPLL_PARAM2 0x00000031
  #define ASTMMC_PARAM_1600
#elif defined(CONFIG_DRAM_1700)
  #define CONFIG_MPLL_PARAM1 0x100161A8
  #define CONFIG_MPLL_PARAM2 0x000000D4
  #define ASTMMC_PARAM_1866
#elif defined(CONFIG_DRAM_1800)
  #define CONFIG_MPLL_PARAM1 0x1000204A
  #define CONFIG_MPLL_PARAM2 0x00000025
  #define ASTMMC_PARAM_1866
#elif defined(CONFIG_DRAM_1824)
  #define CONFIG_MPLL_PARAM1 0x10000025
  #define CONFIG_MPLL_PARAM2 0x00000012
  #define ASTMMC_PARAM_1866
#else      // CONFIG_DRAM_1866
  #define CONFIG_MPLL_PARAM1 0x1000E136
  #define CONFIG_MPLL_PARAM2 0x0000009B
  #define ASTMMC_PARAM_1866
#endif

/******************************************************************************
 Calibration Macro Start
 Usable registers:
  r0, r1, r2, r3, r5, r6, r7, r8, r9, r10, r11
 ******************************************************************************/
#define ASTMMC_INIT_VER      0x01                @ 8bit verison number
#define ASTMMC_INIT_DATE     0x20171228          @ Release date

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
#define ASTMMC_REG_MCR0C     0x24
#define ASTMMC_REG_MCR30     0x28
#define ASTMMC_REG_RFCpb     0x2C
#define ASTMMC_REG_RFCab     0x30

TIME_TABLE_LPDDR3_1600:
    .word   0x030C0406       @ MCR10
    .word   0x11272353       @ MCR14
    .word   0x07080407       @ MCR18
    .word   0x02130014       @ MCR1C
    .word   0x00000024       @ MCR20
    .word   0x0043010D       @ MRW01
    .word   0x001A020D       @ MRW02
    .word   0x0001030D       @ MRW03
    .word   0x00000B0D       @ MRW11
    .word   0x200060A1       @ MCR0C
    .word   0x00021783       @ MCR30
    .word   0x00172323       @ MCRFCpb
    .word   0x00335353       @ MCRFCab
TIME_TABLE_LPDDR3_1866:
    .word   0x040E0508       @ MCR10
    .word   0x14492961       @ MCR14
    .word   0x08090508       @ MCR18
    .word   0x03240018       @ MCR1C
    .word   0x0000002A       @ MCR20
    .word   0x0083010D       @ MRW01
    .word   0x001C020D       @ MRW02
    .word   0x0001030D       @ MRW03
    .word   0x00000B0D       @ MRW11
    .word   0x200060A1       @ MCR0C
    .word   0x00023083       @ MCR30
    .word   0x001B2929       @ MCRFCpb
    .word   0x003C6161       @ MCRFCab
TIME_TABLE_PHY_1600:
    .word   0x1e6e0100       @ start address
    .word   0x82402000       @ phyr000 : 0x16000400
    .word   0x0400004c       @ phyr004
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
    .word   0x00007400       @ phyr030
    .word   0x00000000       @ phyr034
    .word   0x00000400       @ phyr038
    .word   0x20000000       @ phyr03c
    .word   0x00000300       @ phyr040
    .word   0x0a0a0000       @ phyr044
    .word   0x00000000       @ phyr048
    .word   0x00003080       @ phyr04c
    .word   0x02001000       @ phyr050
    .word   0x0000000a       @ phyr054
    .word   0x00000043       @ phyr058
    .word   0x04000000       @ phyr05c
    .word   0x00000000       @ phyr060
    .word   0x00000000       @ phyr064
    .word   0x000a000a       @ phyr068
    .word   0x38800190       @ phyr06c
    .word   0x000c160c       @ phyr070
    .word   0x00fa0000       @ phyr074
    .word   0x00011105       @ phyr078
    .word   0x080e0408       @ phyr07c
    .word   0x54000900       @ phyr080
    .word   0x14400618       @ phyr084
    .word   0x00001000       @ phyr088
    .word   0x05701018       @ phyr08c
    .word   0x00000000       @ phyr090
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e019c       @ new address
    .word   0x20202020       @ phyr09c
    .word   0x20202020       @ phyr0a0
    .word   0x00002020       @ phyr0a4
    .word   0x80000000       @ phyr0a8
    .word   0x00000001       @ phyr0ac
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e01cc       @ new address
    .word   0x08080808       @ phyr0cc
    .word   0x08080808       @ phyr0d0
    .word   0x08080808       @ phyr0d4
    .word   0x08080808       @ phyr0d8
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0280       @ new address
    .word   0x1e1e1e1e       @ phyr180
    .word   0x1e1e1e1e       @ phyr184
    .word   0x9b9b9b9b       @ phyr188
    .word   0x9b9b9b9b       @ phyr18c
    .word   0x9b9b9b9b       @ phyr190
    .word   0x9b9b9b9b       @ phyr194
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e02f8       @ new address
    .word   0x83838383       @ phyr1f8
    .word   0x83838383       @ phyr1fc
    .word   0x00000000       @ phyr200
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0194       @ new address
    .word   0x80215AF6       @ phyr094
    .word   0x09090000       @ phyr098
    .word   0x20202020       @ phyr09c
    .word   0x20202020       @ phyr0a0
    .word   0x00002020       @ phyr0a4
    .word   0x80000000       @ phyr0a8
    .word   0x00000001       @ phyr0ac
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e027c       @ new address
    .word   0x00080000       @ phyr17c
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0140       @ new address
    .word   0x00000F00       @ phyr040
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0148       @ new address
    .word   0x00000704       @ phyr048
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e01b0       @ new address
    .word   0x00000000       @ phyr0b0
    .word   0x00000000       @ phyr0b4
    .word   0x00000000       @ phyr0b8
    .word   0x00000000       @ phyr0bc
    .word   0x00000000       @ phyr0c0
    .word   0x00000000       @ phyr0c4
    .word   0x00000000       @ phyr0c8
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e01c8       @ new address
    .word   0x0114ff2c       @ phyr0c8
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e01dc       @ new address
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
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0298       @ new address
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
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0344       @ new address
    .word   0x2aa00000       @ phyr244
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e033c       @ new address
    .word   0x01100000       @ phyr23c
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0310       @ new address
    .word   0xc0300c03       @ phyr210
    .word   0xa0500a05       @ phyr214
    .word   0xaeededed       @ end
TIME_TABLE_PHY_1866:
    .word   0x1e6e0100       @ start address
    .word   0x82402000       @ phyr000 : 0x08002000
    .word   0x0400004c       @ phyr004
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
    .word   0x00007400       @ phyr030
    .word   0x00000000       @ phyr034
    .word   0x00000400       @ phyr038
    .word   0x20000000       @ phyr03c
    .word   0x00000300       @ phyr040
    .word   0x0a0a0000       @ phyr044
    .word   0x00000000       @ phyr048
    .word   0x00003080       @ phyr04c
    .word   0x02001000       @ phyr050
    .word   0x0001000c       @ phyr054
    .word   0x00000043       @ phyr058
    .word   0x04000000       @ phyr05c
    .word   0x00000000       @ phyr060
    .word   0x00000000       @ phyr064
    .word   0x000a000a       @ phyr068
    .word   0x5B310200       @ phyr06c
    .word   0x000E000A       @ phyr070
    .word   0x01234000       @ phyr074
    .word   0x00014107       @ phyr078
    .word   0x0E160805       @ phyr07c
    .word   0x62000B00       @ phyr080
    .word   0x09000E37       @ phyr084
    .word   0x00001010       @ phyr088
    .word   0x05701018       @ phyr08c
    .word   0x00000000       @ phyr090
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e019c       @ new address
    .word   0x20202020       @ phyr09c
    .word   0x20202020       @ phyr0a0
    .word   0x00002020       @ phyr0a4
    .word   0x80000000       @ phyr0a8
    .word   0x00000001       @ phyr0ac
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e01cc       @ new address
    .word   0x09090909       @ phyr0cc
    .word   0x09090909       @ phyr0d0
    .word   0x48484848       @ phyr0d4
    .word   0x48484848       @ phyr0d8
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0280       @ new address
    .word   0x1e1e1e1e       @ phyr180
    .word   0x1e1e1e1e       @ phyr184
    .word   0x87878787       @ phyr188
    .word   0x87878787       @ phyr18c
    .word   0x87878787       @ phyr190
    .word   0x87878787       @ phyr194
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e02f8       @ new address
    .word   0x88888888       @ phyr1f8
    .word   0x88888888       @ phyr1fc
    .word   0x00000000       @ phyr200
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0194       @ new address
    .word   0x80215AF6       @ phyr094
    .word   0x09090000       @ phyr098
    .word   0x20202020       @ phyr09c
    .word   0x20202020       @ phyr0a0
    .word   0x00002020       @ phyr0a4
    .word   0x80000000       @ phyr0a8
    .word   0x00000001       @ phyr0ac
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e027c       @ new address
    .word   0x00080000       @ phyr17c
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0140       @ new address
    .word   0x00000F00       @ phyr040
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0148       @ new address
    .word   0x00000704       @ phyr048
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e01b0       @ new address
    .word   0x00000000       @ phyr0b0
    .word   0x00000000       @ phyr0b4
    .word   0x00000000       @ phyr0b8
    .word   0x00000000       @ phyr0bc
    .word   0x00000000       @ phyr0c0
    .word   0x00000000       @ phyr0c4
    .word   0x00000000       @ phyr0c8
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e01c8       @ new address
    .word   0x0114ff2c       @ phyr0c8
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e01dc       @ new address
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
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0298       @ new address
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
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0344       @ new address
    .word   0x2aa00000       @ phyr244
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e033c       @ new address
    .word   0x01100000       @ phyr23c
    .word   0xaeeddeea       @ change register address
    .word   0x1e6e0310       @ new address
    .word   0xc0300c03       @ phyr210
    .word   0xa0500a05       @ phyr214
    .word   0xaeededed       @ end

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

    ldr r0, =0x1e6c00d8                          @ Clear Timer3 ISR
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

    ldr r0, =0x1e6c00d8                          @ Clear Timer3 ISR
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

/* Test - DRAM initial time */
    ldr   r0, =0x1e78203c
    ldr   r1, =0x0000F000
    str   r1, [r0]

    ldr   r0, =0x1e782044
    ldr   r1, =0xFFFFFFFF
    str   r1, [r0]

    ldr   r0, =0x1e782030
    mov   r2, #3
    mov   r1, r2, lsl #12
    str   r1, [r0]
/* Test - DRAM initial time */

    /*Set Scratch register Bit 7 before initialize*/
    ldr r0, =0x1e6e2000
    ldr r1, =0x1688a8a8
    str r1, [r0]

    ldr r0, =0x1e6e2040
    ldr r1, [r0]
    orr r1, r1, #0x80
    str r1, [r0]

    /* Enable AXI_P */
    ldr   r0, =0x00000016
    mrc   p15, 0, r1, c15, c2, 4
    mcr   p15, 0, r0, c15, c2, 4

/******************************************************************************
 Disable WDT3 for SPI Address mode detection function
 ******************************************************************************/
    ldr   r0, =0x1e78504c
    mov   r1, #0
    str   r1, [r0]

    ldr   r0, =0x1e6e2004
    ldr   r1, [r0]
    orr   r2, r1, #0x01

    /* Set M-PLL */
    ldr   r3, =CONFIG_MPLL_PARAM1
    ldr   r2, =0x03000000
    orr   r2, r2, r3
    ldr   r0, =0x1e6e2020
    str   r2, [r0]

    ldr   r2, =CONFIG_MPLL_PARAM2
    ldr   r0, =0x1e6e2138
    str   r2, [r0]

    /* Delay about 10us */
    ldr   r2, =0x0000000A                        @ Set Timer3 Reload = 10 us
    init_delay_timer
delay_mpll_reset:
    check_delay_timer
    bne   delay_mpll_reset
    clear_delay_timer
    /* end delay 10us */

    ldr   r2, =0x01000000
    orr   r2, r2, r3
    ldr   r0, =0x1e6e2020
    str   r2, [r0]

    ldr   r0, =0x1e6e2138
    ldr   r2, =0x00001000
wait_mpll_lock:
    ldr   r1, [r0]
    tst   r1, r2
    beq   wait_mpll_lock

    ldr   r0, =0x1e6e2020
    str   r3, [r0]

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
    mov   r1, #'\r'                              @ '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              @ '\n'
    str   r1, [r0]
    mov   r1, #'D'                               @ 'D'
    str   r1, [r0]
    mov   r1, #'R'                               @ 'R'
    str   r1, [r0]
    mov   r1, #'A'                               @ 'A'
    str   r1, [r0]
    mov   r1, #'M'                               @ 'M'
    str   r1, [r0]
    mov   r1, #' '                               @ ' '
    str   r1, [r0]
    mov   r1, #'I'                               @ 'I'
    str   r1, [r0]
    mov   r1, #'n'                               @ 'n'
    str   r1, [r0]
    mov   r1, #'i'                               @ 'i'
    str   r1, [r0]
    mov   r1, #'t'                               @ 't'
    str   r1, [r0]
    mov   r1, #'-'                               @ '-'
    str   r1, [r0]
    mov   r1, #'L'                               @ 'L'
    str   r1, [r0]
    mov   r1, #'P'                               @ 'P'
    str   r1, [r0]
    ldr   r0, =0x1e783014
wait_print_0:
    ldr   r1, [r0]
    tst   r1, #0x40
    beq   wait_print_0
    ldr   r0, =0x1e783000
    mov   r1, #'D'                               @ 'D'
    str   r1, [r0]
    mov   r1, #'D'                               @ 'D'
    str   r1, [r0]
    mov   r1, #'R'                               @ 'R'
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
    mov   r1, #'3'                               @ '3'
    str   r1, [r0]
    mov   r1, #'\r'                              @ '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              @ '\n'
    str   r1, [r0]
/* Debug - UART console message */

    mov   r10, #0                                @ init reset retry count

    /* Reset MMC */
reset_mmc:
    ldr   r0, =0x1e6e2004
    ldr   r1, [r0]
    orr   r2, r1, #0x01
    str   r2, [r0]
    ldr   r2, [r0]                               @ doing a dummy read for delay
    bic   r1, r1, #0x01
    str   r1, [r0]

    /* Delay about 10us */
    ldr   r2, =0x0000000B                        @ Set Timer3 Reload = 10 us
    init_delay_timer
delay_mmc_reset:
    check_delay_timer
    bne   delay_mmc_reset
    clear_delay_timer
    /* end delay 10us */

init_start:
    ldr   r0, =0x1e6e0000
    ldr   r1, =0xFC600309
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

#ifdef ASTMMC_PARAM_1600
    adrl  r5, TIME_TABLE_LPDDR3_1600             @ Init DRAM parameter table
#else
    adrl  r5, TIME_TABLE_LPDDR3_1866             @ Init DRAM parameter table
#endif

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
    ldr   r2, =0xaeededed                        @ phyr end pattern
    ldr   r6, =0xaeeddeea                        @ phyr change address
#ifdef ASTMMC_PARAM_1600
    adrl  r3, TIME_TABLE_PHY_1600                @ phyr init value
#else
    adrl  r3, TIME_TABLE_PHY_1866                @ phyr init value
#endif
    ldr   r0, [r3]                               @ phyr start address
    add   r3, r3, #4
ddrphy_init_start:
    ldr   r1, [r3]
    cmp   r1, r2
    beq   ddrphy_init_end
    cmp   r1, r6
    beq   ddrphy_init_change_address
    str   r1, [r0]
    add   r0, r0, #4
    add   r3, r3, #4
    b     ddrphy_init_start

ddrphy_init_change_address:
    add   r3, r3, #4
    ldr   r0, [r3]                               @ phyr new address
    add   r3, r3, #4
    b     ddrphy_init_start

ddrphy_init_end:
    /* Delay about 500us */
    ldr   r2, =0x00000200                        @ Set Timer3 Reload = 500 us
    init_delay_timer
delay_phypll_reset:
    check_delay_timer
    bne   delay_phypll_reset
    clear_delay_timer
    /* end delay 500us */

    ldr   r0, =0x1e6e0060                        @ Release PHY reset
    ldr   r1, =0x00000004
    str   r1, [r0]

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
    mov   r1, #'P'                               @ 'P'
    str   r1, [r0]
    mov   r1, #'H'                               @ 'H'
    str   r1, [r0]
    mov   r1, #'Y'                               @ 'Y'
    str   r1, [r0]
    mov   r1, #'P'                               @ 'P'
    str   r1, [r0]
    mov   r1, #'L'                               @ 'L'
    str   r1, [r0]
    mov   r1, #'L'                               @ 'L'
    str   r1, [r0]
    mov   r1, #' '                               @ ' '
    str   r1, [r0]
    mov   r1, #'L'                               @ 'L'
    str   r1, [r0]
    mov   r1, #'o'                               @ 'o'
    str   r1, [r0]
    mov   r1, #'c'                               @ 'c'
    str   r1, [r0]
    mov   r1, #'k'                               @ 'k'
    str   r1, [r0]
    mov   r1, #'e'                               @ 'e'
    str   r1, [r0]
    mov   r1, #'d'                               @ 'd'
    str   r1, [r0]
    mov   r1, #'\r'                              @ '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              @ '\n'
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
    mov   r1, #'P'                               @ 'P'
    str   r1, [r0]
    mov   r1, #'H'                               @ 'H'
    str   r1, [r0]
    mov   r1, #'Y'                               @ 'Y'
    str   r1, [r0]
    mov   r1, #'I'                               @ 'I'
    str   r1, [r0]
    mov   r1, #'n'                               @ 'n'
    str   r1, [r0]
    mov   r1, #'i'                               @ 'i'
    str   r1, [r0]
    mov   r1, #'t'                               @ 't'
    str   r1, [r0]
    mov   r1, #' '                               @ ' '
    str   r1, [r0]
    mov   r1, #'D'                               @ 'D'
    str   r1, [r0]
    mov   r1, #'o'                               @ 'o'
    str   r1, [r0]
    mov   r1, #'n'                               @ 'n'
    str   r1, [r0]
    mov   r1, #'e'                               @ 'e'
    str   r1, [r0]
    mov   r1, #'\r'                              @ '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              @ '\n'
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
    ldr   r1, [r5, #ASTMMC_REG_MCR30]
    str   r1, [r0]

    b     Calibration_End

.LTORG
/******************************************************************************
 Common Process
 *****************************************************************************/
read_MR_process:
/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #'M'                               @ 'M'
    str   r1, [r0]
    mov   r1, #'R'                               @ 'R'
    str   r1, [r0]
    mov   r2, r3
    print_hex_byte
    mov   r1, #'='                               @ '='
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
     4Gb : 0x80000000 ~ 0x9FFFFFFF : R[13:0],B[2:0],C[9:0]
     8Gb : 0x80000000 ~ 0xBFFFFFFF : R[14:0],B[2:0],C[9:0]
     16Gb: 0x80000000 ~ 0xFFFFFFFF : R[14:0],C[10],B[2:0],C[9:0]
    *******************************/
    ldr   r0, =0x1e6e0004
    ldr   r6, [r0]
    bic   r6, r6, #0x00000003                    @ record MCR04
    ldr   r7, [r5, #ASTMMC_REG_RFCpb]
    ldr   r8, [r5, #ASTMMC_REG_RFCab]

check_dram_size:
    /* Check 16Gb */
    mov   r3, #0x16                              @ '16'
    ldr   r0, =0x1e6e0004                        @ set to 16Gb
    orr   r1, r6, #0x2
    str   r1, [r0]
    ldr   r0, =0x80008000
    ldr   r1, =0x41424344
    str   r1, [r0]
    ldr   r0, =0x80000000
    ldr   r1, =0x35363738
    str   r1, [r0]
    ldr   r0, =0x80008000
    ldr   r1, =0x41424344
    ldr   r2, [r0]
    cmp   r2, r1                                 @ == 16Gbit
    orreq r6, r6, #0x02
    beq   check_dram_size_end
    mov   r7, r7, lsr #8
    mov   r8, r8, lsr #8

    /* Check 8Gb/4Gb */
    mov   r3, #0x08                              @ '8'
    ldr   r0, =0x1e6e0004                        @ set to 8Gb
    orr   r1, r6, #0x1
    str   r1, [r0]
    ldr   r0, =0xA0100000
    ldr   r1, =0x35363738
    str   r1, [r0]
    ldr   r0, =0x80100000
    ldr   r1, =0x292A2B2C
    str   r1, [r0]
    ldr   r0, =0xA0100000
    ldr   r1, =0x35363738
    ldr   r2, [r0]
    cmp   r2, r1                                 @ == 8Gbit
    orreq r6, r6, #0x01
    beq   check_dram_size_end
    mov   r7, r7, lsr #8
    mov   r8, r8, lsr #8                         @ == 4Gbit
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
    mov   r1, #'S'                               @ 'S'
    str   r1, [r0]
    mov   r1, #'i'                               @ 'i'
    str   r1, [r0]
    mov   r1, #'z'                               @ 'z'
    str   r1, [r0]
    mov   r1, #'e'                               @ 'e'
    str   r1, [r0]
    mov   r1, #'-'                               @ '-'
    str   r1, [r0]
    mov   r1, r3, lsr #4
    print_hex_char
    mov   r1, r3
    print_hex_char
    mov   r1, #'G'                               @ 'G'
    str   r1, [r0]
    mov   r1, #'b'                               @ 'b'
    str   r1, [r0]
    mov   r1, #'-'                               @ '-'
    str   r1, [r0]
/* Debug - UART console message */

    /********************************************
     DDRTest
    ********************************************/
ddr_test_start:
    ldr   r0, =0x1e6e0074
    ldr   r1, =0x00FFFFFF                        @ test size = 16MB
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
    mov   r1, #'F'                               @ 'F'
    str   r1, [r0]
    mov   r1, #'a'                               @ 'a'
    str   r1, [r0]
    mov   r1, #'i'                               @ 'i'
    str   r1, [r0]
    mov   r1, #'l'                               @ 'l'
    str   r1, [r0]
    mov   r1, #'\r'                              @ '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              @ '\n'
    str   r1, [r0]
    ldr   r0, =0x1e783014
wait_print_1:
    ldr   r1, [r0]
    tst   r1, #0x40
    beq   wait_print_1
/* Debug - UART console message */
    add   r10, r10, #1
    cmp   r10, #5
    beq   platform_lock
    b     reset_mmc

set_scratch:
    /*Set Scratch register Bit 6 after ddr initial finished */
    ldr   r0, =0x1e6e2040
    ldr   r1, [r0]
    orr   r1, r1, #0x41
    str   r1, [r0]

/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #'D'                               @ 'D'
    str   r1, [r0]
    mov   r1, #'o'                               @ 'o'
    str   r1, [r0]
    mov   r1, #'n'                               @ 'n'
    str   r1, [r0]
    mov   r1, #'e'                               @ 'e'
    str   r1, [r0]
    mov   r1, #'\r'                              @ '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              @ '\n'
    str   r1, [r0]
/* Debug - UART console message */

platform_exit:

/* Test - DRAM initial time */
    ldr   r0, =0x1e782040
    ldr   r1, [r0]
    ldr   r0, =0xFFFFFFFF
    sub   r1, r0, r1
    ldr   r0, =0x1e6e008c
    str   r1, [r0]
    ldr   r0, =0x1e78203c
    ldr   r1, =0x0000F000
    str   r1, [r0]
/* Test - DRAM initial time */

    /* restore lr */
    mov   lr, r4

    /* back to arch calling code */
    mov   pc, lr

platform_lock:
/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #'S'                               @ 'S'
    str   r1, [r0]
    mov   r1, #'t'                               @ 't'
    str   r1, [r0]
    mov   r1, #'o'                               @ 'o'
    str   r1, [r0]
    mov   r1, #'p'                               @ 'p'
    str   r1, [r0]
    mov   r1, #'&'                               @ '&'
    str   r1, [r0]
    mov   r1, #'H'                               @ 'H'
    str   r1, [r0]
    mov   r1, #'a'                               @ 'a'
    str   r1, [r0]
    mov   r1, #'l'                               @ 'l'
    str   r1, [r0]
    mov   r1, #'t'                               @ 't'
    str   r1, [r0]
    mov   r1, #'\r'                              @ '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              @ '\n'
    str   r1, [r0]
/* Debug - UART console message */
platform_lock_all:
    b     platform_lock_all

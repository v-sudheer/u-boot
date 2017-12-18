/*
 * Board specific setup info
 *
 ******************************************************************************
 * ASPEED Technology Inc.
 * AST1220 FPGA LPDDR3 SDRAM controller initialization and calibration sequence
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

TIME_TABLE_LPDDR3_600:
    .word   0x02080305       @ MCR10
    .word   0x07250813       @ MCR14
    .word   0x02030202       @ MCR18
    .word   0x01030008       @ MCR1C
    .word   0x0000000D       @ MCR20
    .word   0x0083010D       @ MRW01
    .word   0x0004020D       @ MRW02
    .word   0x0002030D       @ MRW03
    .word   0x00010B0D       @ MRW11
TIME_TABLE_LPDDR3_800:
    .word   0x02080305       @ MCR10
    .word   0x09250B19       @ MCR14
    .word   0x02030203       @ MCR18
    .word   0x0103000A       @ MCR1C
    .word   0x00000012       @ MCR20
    .word   0x0083010D       @ MRW01
    .word   0x0004020D       @ MRW02
    .word   0x0002030D       @ MRW03
    .word   0x00010B0D       @ MRW11
TIME_TABLE_LPDDR3_1200:
    .word   0x030A0305       @ MCR10
    .word   0x0D491A3E       @ MCR14
    .word   0x05060305       @ MCR18
    .word   0x0213000F       @ MCR1C
    .word   0x0000001B       @ MCR20
    .word   0x00E3010D       @ MRW01
    .word   0x0007020D       @ MRW02
    .word   0x0002030D       @ MRW03
    .word   0x00010B0D       @ MRW11
TIME_TABLE_LPDDR3_1600:
    .word   0x030C0406       @ MCR10
    .word   0x115C2353       @ MCR14
    .word   0x07080407       @ MCR18
    .word   0x42130014       @ MCR1C
    .word   0x00000024       @ MCR20
    .word   0x0043010D       @ MRW01
    .word   0x001A020D       @ MRW02
    .word   0x0002030D       @ MRW03
    .word   0x00010B0D       @ MRW11

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

    /*Set GPIO for SDMMC host  fpga */
    ldr   r0, =0x1e780074
    ldr   r1, =0x00400000
    str   r1, [r0]
    /* for SDIO */
    ldr   r0, =0x1e780004
    ldr   r1, =0x80000000
    str   r1, [r0]

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

init_arm11:
    /* Start of ES40004A PLL init */
    /* Step 1. Program PLL_config and keep power down */
    ldr   r0, =0x33000000
    ldr   r1, =0x01000000
    str   r1, [r0]
    ldr   r1, =0x0102001A                        @ 324 MHz
    str   r1, [r0]

    /* Step 2. Wait 1us for PLL initialization */
    ldr   r2, =0x00000100
delay_ES40004A_pll_init:
    subs  r2, r2, #1
    bne   delay_ES40004A_pll_init

    /* Step 3. Program PLL_config to exit Power down */
    ldr   r1, =0x0002001A
    str   r1, [r0]

    /* Step 4. Check pll_ld = 1?. Read PLL_config, check bit 27. */
    ldr   r2, =0x08000000                        @ bit[27] PLL lock detection
check_pll_ld:
    ldr   r1, [r0]
    tst   r1, r2
    beq   check_pll_ld

    /* Step 5. Program aclk_div */
    ldr   r0, =0x1e6e2070
    ldr   r1, [r0]
    mov   r1, r1, lsr #6
    and   r1, r1, #0x03
    add   r1, r1, #0x05
    ldr   r0, =0x33000004
//  ldr   r1, =0x00000007                        @ CPU/AXI = 8/1
    str   r1, [r0]

    /* Step 6. Program set_pll */
//  ldr   r1, =0x00010007
    ldr   r2, =0x00010000
    orr   r1, r1, r2
    str   r1, [r0]
    /* End of ES40004A PLL init */

    /* Check Scratch Register Bit 6 */
/*  ldr r0, =0x1e6e2040
    ldr r1, [r0]
    bic r1, r1, #0xFFFFFFBF
    mov r2, r1, lsr #6
    cmp r2, #0x01
    beq platform_exit
*/
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

    /* Delay about 100us */
    ldr   r2, =0x00000064                        @ Set Timer3 Reload = 100 us
    init_delay_timer
delay_0:
    check_delay_timer
    bne   delay_0
    clear_delay_timer
    /* end delay 100us */
/**************************************************************************************************/
/**************************************************************************************************/
/* Debug - UART console message */
    ldr   r0, =0x1e783000
    mov   r1, #0x33                              @ '3'
    str   r1, [r0]
    mov   r1, #0x2D                              @ '-'
    str   r1, [r0]
    mov   r1, #0x46                              @ 'F'
    str   r1, [r0]
    mov   r1, #0x50                              @ 'P'
    str   r1, [r0]
    mov   r1, #0x47                              @ 'G'
    str   r1, [r0]
    mov   r1, #0x41                              @ 'A'
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

    /* Delay about 10us */
    ldr   r2, =0x0000000A                        @ Set Timer3 Reload = 10 us
    init_delay_timer
delay_reset:
    check_delay_timer
    bne   delay_reset
    clear_delay_timer
    /* end delay 10us */

    ldr   r0, =0x1e6e2070
    ldr   r1, [r0]
    mov   r1, r1, lsr #4
    and   r1, r1, #0x03
    adrl  r5, TIME_TABLE_LPDDR3_600              @ Init DRAM parameter table
    cmp   r1, #0x0
    beq   init_start
    adrl  r5, TIME_TABLE_LPDDR3_800              @ Init DRAM parameter table
    cmp   r1, #0x1
    beq   init_start
    adrl  r5, TIME_TABLE_LPDDR3_1200             @ Init DRAM parameter table
    cmp   r1, #0x2
    beq   init_start
    adrl  r5, TIME_TABLE_LPDDR3_1600             @ Init DRAM parameter table

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
    ldr   r1, =0x00000021                        @ 4GbX2
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

    /* Delay about 20us */
    ldr   r2, =0x00000014                        @ Set Timer3 Reload = 20 us
    init_delay_timer
delay_2:
    check_delay_timer
    bne   delay_2
    clear_delay_timer
    /* end delay 20us */

    ldr   r0, =0x1e6e0024                        @ ZQinit, wait at least 1 us for ZQinit done [CS0]
    ldr   r1, =0x00FF0A05
    str   r1, [r0]

    /* Delay about 10us */
    ldr   r2, =0x0000000A                        @ Set Timer3 Reload = 10 us
    init_delay_timer
delay_3:
    check_delay_timer
    bne   delay_3
    clear_delay_timer
    /* end delay 10us */

    ldr   r0, =0x1e6e0024                        @ ZQinit, wait at least 1 us for ZQinit done [CS1]
    ldr   r1, =0x00FF0A09
    str   r1, [r0]

    /* Delay about 10us */
    ldr   r2, =0x0000000A                        @ Set Timer3 Reload = 10 us
    init_delay_timer
delay_4:
    check_delay_timer
    bne   delay_4
    clear_delay_timer
    /* end delay 10us */

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
    ldr   r1, =0x200016A1
    str   r1, [r0]

    ldr   r0, =0x1e6e0030
    ldr   r1, =0x00034783
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

#if 0
    /*******************************
     Check DRAM Size
     1Gb : 0x80000000 ~ 0x87FFFFFF
     2Gb : 0x80000000 ~ 0x8FFFFFFF
     4Gb : 0x80000000 ~ 0x9FFFFFFF
     8Gb : 0x80000000 ~ 0xBFFFFFFF
    *******************************/
    ldr   r0, =0x1e6e0004
    ldr   r6, [r0]
    bic   r6, r6, #0x00000003                    @ record MCR04
    ldr   r7, [r5, #ASTMMC_REGIDX_RFC]

check_dram_size:
    ldr   r0, =0xA0100000
    ldr   r1, =0x41424344
    str   r1, [r0]
    ldr   r0, =0x90100000
    ldr   r1, =0x35363738
    str   r1, [r0]
    ldr   r0, =0x88100000
    ldr   r1, =0x292A2B2C
    str   r1, [r0]
    ldr   r0, =0x80100000
    ldr   r1, =0x1D1E1F10
    str   r1, [r0]
    ldr   r0, =0xA0100000
    ldr   r1, =0x41424344
    ldr   r2, [r0]
    cmp   r2, r1                                 @ == 8Gbit
    orreq r6, r6, #0x03
    moveq r7, r7, lsr #24
    mov   r3, #0x38                              @ '8'
    beq   check_dram_size_end
    ldr   r0, =0x90100000
    ldr   r1, =0x35363738
    ldr   r2, [r0]
    cmp   r2, r1                                 @ == 4Gbit
    orreq r6, r6, #0x02
    moveq r7, r7, lsr #16
    mov   r3, #0x34                              @ '4'
    beq   check_dram_size_end
    ldr   r0, =0x88100000
    ldr   r1, =0x292A2B2C
    ldr   r2, [r0]
    cmp   r2, r1                                 @ == 2Gbit
    orreq r6, r6, #0x01
    moveq r7, r7, lsr #8
    mov   r3, #0x32                              @ '2'
    beq   check_dram_size_end
    mov   r3, #0x31                              @ '1'

check_dram_size_end:
    ldr   r0, =0x1e6e0004
    str   r6, [r0]
    ldr   r0, =0x1e6e0014
    ldr   r1, [r0]
    bic   r1, r1, #0x000000FF
    and   r7, r7, #0xFF
    orr   r1, r1, r7
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
    mov   r1, #0x2D                              @ '-'
    str   r1, [r0]
    str   r3, [r0]
    mov   r1, #0x47                              @ 'G'
    str   r1, [r0]
    mov   r1, #0x62                              @ 'b'
    str   r1, [r0]
    mov   r1, #0x2D                              @ '-'
    str   r1, [r0]
/* Debug - UART console message */
#endif

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

    /*Set GPIO for SDMMC host  fpga */
    ldr   r0, =0x1e780070
    ldr   r1, =0x00400000
    str   r1, [r0]
    /* SDIO */
    ldr   r0, =0x1e780000
    ldr   r1, =0x80000000
    str   r1, [r0]

    /* restore lr */
    mov   lr, r4

    /* back to arch calling code */
    mov   pc, lr


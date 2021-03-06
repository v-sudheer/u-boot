/*
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
/*
 * Board specific setup info
 *
 ******************************************************************************
 * ASPEED Technology Inc.
 * AST25x0 DDR3/DDR4 SDRAM controller initialization sequence
 *
 * Gary Hsu, <gary_hsu@aspeedtech.com>
 *
 * Version     : 0
 * Release date: 2018.04.20
 *
 * Priority of fix item:
 * [P1] = critical
 * [P2] = nice to have
 * [P3] = minor
 *
 * Change List :
 * V0 |2018.03.28 : 1.[P1] Initial release for simulation
 *
 * Optional define variable
 * 1. DRAM Speed                  //
 *    ASTMMC_DRAM_800             // (DDR3 FPGA only)
 *    ASTMMC_DRAM_1066            // (DDR3 FPGA only)
 *    ASTMMC_DRAM_1333            //
 *    ASTMMC_DRAM_1600            // (default)
 *    ASTMMC_DRAM_1866            //
 * 2. ECC Function enable
 *    ASTMMC_DRAM_ECC             // define to enable ECC function
 *    ASTMMC_DRAM_ECC_SIZE        // define the ECC protected memory size
 * 3. UART5 message output        //
 *    ASTMMC_UART_BASE            // select UART port base
 *    ASTMMC_UART_38400           // set the UART baud rate to 38400, default is 115200
 *    ASTMMC_UART_TO_UART1        // route UART5 to UART port1
 * 4. DRAM Type
 *    ASTMMC_DDR4_8GX8            // DDR4 (16Gb) 8Gbit X8 stacked part
 * 5. Firmware 2nd boot flash
 *    ASTMMC_FIRMWARE_2ND_BOOT    (Removed)
 * 6. Enable DRAM extended temperature range mode
 *    ASTMMC_DRAM_EXT_TEMP
 ******************************************************************************
 */

#include <config.h>
#include <version.h>

#define ASTMMC_DDR_DDR4
//#define ASTMMC_DRAM_800

/******************************************************************************
 Calibration Macro Start
 Usable registers:
  r0, r1, r2, r3, r5, r6, r7, r8, r9, r10, r11
 ******************************************************************************/
#define ASTMMC_INIT_VER      0x00                // 8bit verison number
#define ASTMMC_INIT_DATE     0x20180328          // Release date

/* PATTERN_TABLE,
   init_delay_timer,
   check_delay_timer,
   clear_delay_timer,
   print_hex_char,
   print_hex_byte,
   print_hex_word,
   print_hex_dword,
   are for DRAM calibration */

#define ASTMMC_UART_BASE     0x1E784000

//#define ASTMMC_DRAM_ECC
#define ASTMMC_DRAM_ECC_SIZE 0x07FFFFFF          // ECC size = 96MB

#define ASTMMC_REG_MCR10     0x00
#define ASTMMC_REG_MCR14     0x04
#define ASTMMC_REG_MCR18     0x08
#define ASTMMC_REG_MCR1C     0x0C
#define ASTMMC_REG_MCR20     0x10
#define ASTMMC_REG_MCR24     0x14
#define ASTMMC_REG_MCR28     0x18
#define ASTMMC_REG_MCR2C     0x1C
#define ASTMMC_REG_RFC       0x20

TIME_TABLE_DDR3_800:
    .word   0x02080206       // MCR10
    .word   0x07010145       // MCR14
    .word   0x0A010200       // MCR18
    .word   0x00000020       // MCR1C
    .word   0x00401510       // MCR20
    .word   0x00000200       // MCR24
    .word   0x00000000       // MCR28
    .word   0x00000000       // MCR2C
    .word   0x4545331F       // MCRFC
TIME_TABLE_DDR3_1066:
    .word   0x02090306       // MCR10
    .word   0x0913015C       // MCR14
    .word   0x0E020301       // MCR18
    .word   0x00008020       // MCR1C
    .word   0x00401930       // MCR20
    .word   0x00000208       // MCR24
    .word   0x00000000       // MCR28
    .word   0x00000000       // MCR2C
    .word   0x5D5D452A       // MCRFC
TIME_TABLE_DDR3_1333:
    .word   0x030B0307       // MCR10
    .word   0x0B250174       // MCR14
    .word   0x0F030302       // MCR18
    .word   0x00000020       // MCR1C
    .word   0x00401B50       // MCR20
    .word   0x00000210       // MCR24
    .word   0x00000000       // MCR28
    .word   0x00000000       // MCR2C
    .word   0x74745635       // MCRFC
TIME_TABLE_DDR3_1600:
    .word   0x030C0408       // MCR10
    .word   0x0D37128B       // MCR14
    .word   0x10040303       // MCR18
    .word   0x00008020       // MCR1C
    .word   0x00401D70       // MCR20
    .word   0x00000218       // MCR24
    .word   0x00000000       // MCR28
    .word   0x00000000       // MCR2C
    .word   0x8B8B673F       // MCRFC
TIME_TABLE_DDR3_1866:
    .word   0x040E0409       // MCR10
    .word   0x0F4912A2       // MCR14
    .word   0x11050304       // MCR18
    .word   0x00000020       // MCR1C
    .word   0x00421F14       // MCR20
    .word   0x00000220       // MCR24
    .word   0x00000000       // MCR28
    .word   0x00000000       // MCR2C
    .word   0xA3A3794A       // MCRFC
TIME_TABLE_DDR4_1333:
    .word   0x030C0207       // MCR10
    .word   0x0C450174       // MCR14
    .word   0x0C040303       // MCR18
    .word   0x00001240       // MCR1C
    .word   0x01010100       // MCR20
    .word   0x00000000       // MCR24
    .word   0x04000000       // MCR28
    .word   0x00000400       // MCR2C
    .word   0x74745635       // MCRFC
TIME_TABLE_DDR4_1600:
    .word   0x030D0307       // MCR10
    .word   0x0D46118B       // MCR14
    .word   0x0E050303       // MCR18
    .word   0x00001240       // MCR1C
    .word   0x01010304       // MCR20
    .word   0x00000000       // MCR24
    .word   0x04000000       // MCR28
    .word   0x00000400       // MCR2C
    .word   0x8B8B673F       // MCRFC
TIME_TABLE_DDR4_1866:
    .word   0x040E0308       // MCR10
    .word   0x0F5812A2       // MCR14
    .word   0x0E060304       // MCR18
    .word   0x00009240       // MCR1C
    .word   0x01010514       // MCR20
    .word   0x00000008       // MCR24
    .word   0x04000000       // MCR28
    .word   0x00000400       // MCR2C
    .word   0xA3A3794A       // MCRFC

PATTERN_TABLE:
    .word   0xff00ff00
    .word   0xcc33cc33
    .word   0xaa55aa55
    .word   0x88778877
    .word   0x92cc4d6e       // 5
    .word   0x543d3cde
    .word   0xf1e843c7
    .word   0x7c61d253
    .word   0x00000000       // 8

    .macro init_delay_timer
    ldr   r0, =0x1e782024                        // Set Timer3 Reload
    str   r2, [r0]

    ldr   r0, =0x1e782034                        // Clear Timer3 ISR
    ldr   r1, =0x00000004
    str   r1, [r0]

    ldr   r0, =0x1e782030                        // Enable Timer3
    mov   r2, #7
    mov   r1, r2, lsl #8
    str   r1, [r0]

    ldr   r0, =0x1e782034                        // Check ISR for Timer3 timeout
    .endm

    .macro check_delay_timer
    ldr   r1, [r0]
    bic   r1, r1, #0xFFFFFFFB
    mov   r2, r1, lsr #2
    cmp   r2, #0x01
    .endm

    .macro clear_delay_timer
    ldr   r0, =0x1e78203C                        // Disable Timer3
    mov   r2, #0xF
    mov   r1, r2, lsl #8
    str   r1, [r0]

    ldr   r0, =0x1e782034                        // Clear Timer3 ISR
    ldr   r1, =0x00000004
    str   r1, [r0]
    .endm

    .macro print_hex_char
    and   r1, r1, #0xF
    cmp   r1, #9
    addgt r1, r1, #0x37
    addle r1, r1, #0x30
    str   r1, [r0]
    .endm

    .macro print_hex_byte
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, r2, lsr #4
    print_hex_char
    mov   r1, r2
    print_hex_char
    .endm

    .macro print_hex_word
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, r2, lsr #12
    print_hex_char
    mov   r1, r2, lsr #8
    print_hex_char
    mov   r1, r2, lsr #4
    print_hex_char
    mov   r1, r2
    print_hex_char
    .endm

    .macro print_hex_dword
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, r2, lsr #28
    print_hex_char
    mov   r1, r2, lsr #24
    print_hex_char
    mov   r1, r2, lsr #20
    print_hex_char
    mov   r1, r2, lsr #16
    print_hex_char
    mov   r1, r2, lsr #12
    print_hex_char
    mov   r1, r2, lsr #8
    print_hex_char
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
  /* Put secondary core to sleep */
  mrc   p15, 0, r0, c0, c0, 5                  @; Read CPU ID register
  ands  r0, r0, #0x03                          @; Mask off, leaving the CPU ID field
#ifdef CONFIG_ASPEED_NONSECUR_MODE
  blne	secondary_cpu_init
#else
  blne  relocate
  @blne  wait_for_kickup
  b     init_uart

#if 1
relocate:
    adrl  r0, wait_for_kickup
    ldr   r1, =0x1000f000     //  ; r1 = pointer to destination block
    mov   r2, #0x20           //  ; r2 = number of words to copy
wordcopy:
    ldr   r3, [r0], #4         // ; load a word from the source and
    str   r3, [r1], #4         // ; store it to the destination
    subs  r2, r2, #1           // ; decrement the counter
    bne   wordcopy             //1 ; ... copy more

   	ldr r0, =0x1E6E2180
	LDR r1, =0x1e784000   	
	ldr r4, =0xABBAADDA
	ldr r3, =0x1E6E2184

   	ldr   r5, =0x10000000
   	ldr   r6, =0x1000f000
   	str   r6, [r5]
	mov   lr, r6
	mov   pc, lr
#endif
#endif

init_uart:
    /* save lr */
    mov   r4, lr

    /*Initialize the Debug UART here*/
    ldr   r0, =(ASTMMC_UART_BASE | 0x0c)
    mov   r1, #0x83
    str   r1, [r0]

    ldr   r0, =(ASTMMC_UART_BASE | 0x00)
    mov   r1, #0x01
    str   r1, [r0]

    ldr   r0, =(ASTMMC_UART_BASE | 0x04)
    mov   r1, #0x00
    str   r1, [r0]

    ldr   r0, =(ASTMMC_UART_BASE | 0x0c)
    mov   r1, #0x03
    str   r1, [r0]

    ldr   r0, =(ASTMMC_UART_BASE | 0x08)
    mov   r1, #0x07
    str   r1, [r0]

init_dram:

/* Test - DRAM initial time */
    ldr   r0, =0x1e78203c
    ldr   r1, =0x0000F000
    str   r1, [r0]

    ldr   r0, =0x1e782044
    ldr   r1, =0xFFFFFFFF
    str   r1, [r0]

    ldr   r0, =0x1e782030
    ldr   r1, =0x00003000
    str   r1, [r0]
/* Test - DRAM initial time */

    /*Set Scratch register Bit 7 before initialize*/
    ldr   r0, =0x1e6e2000
    ldr   r1, =0x1688a8a8
    str   r1, [r0]
    ldr   r0, =0x1e6e2010
    str   r1, [r0]

/*  ldr   r0, =0x1e6e2100
    ldr   r1, [r0]
    orr   r1, r1, #0x80
    str   r1, [r0]
*/
/******************************************************************************
 Disable WDT for SPI Address mode detection function
 ******************************************************************************/
    ldr   r0, =0x1e620060
    mov   r1, #0
    str   r1, [r0]

    ldr   r0, =0x1e620064
    mov   r1, #0
    str   r1, [r0]

    ldr   r0, =0x1e78500c
    mov   r1, #0
    str   r1, [r0]
    ldr   r0, =0x1e78504c
    str   r1, [r0]
    ldr   r0, =0x1e78508c
    str   r1, [r0]
    ldr   r0, =0x1e7850cc
    str   r1, [r0]

    /* Check Scratch Register Bit 6 */
    ldr   r0, =0x1e6e2100
    ldr   r1, [r0]
    bic   r1, r1, #0xFFFFFFBF
    mov   r2, r1, lsr #6
    cmp   r2, #0x01
    beq   platform_exit

ddr_init_start:

/* Debug - UART console message */
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, #'\r'                              // '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              // '\n'
    str   r1, [r0]
    mov   r1, #'D'                               // 'D'
    str   r1, [r0]
    mov   r1, #'R'                               // 'R'
    str   r1, [r0]
    mov   r1, #'A'                               // 'A'
    str   r1, [r0]
    mov   r1, #'M'                               // 'M'
    str   r1, [r0]
    mov   r1, #' '                               // ' '
    str   r1, [r0]
    mov   r1, #'I'                               // 'I'
    str   r1, [r0]
    mov   r1, #'n'                               // 'n'
    str   r1, [r0]
    mov   r1, #'i'                               // 'i'
    str   r1, [r0]
    mov   r1, #'t'                               // 't'
    str   r1, [r0]
    mov   r1, #'-'                               // '-'
    str   r1, [r0]
    mov   r1, #'V'                               // 'V'
    str   r1, [r0]
    mov   r2, #ASTMMC_INIT_VER
    print_hex_byte
    ldr   r0, =(ASTMMC_UART_BASE | 0x14)
wait_print:
    ldr   r1, [r0]
    tst   r1, #0x40
    beq   wait_print
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, #'-'                               // '-'
    str   r1, [r0]
    mov   r1, #'D'                               // 'D'
    str   r1, [r0]
    mov   r1, #'D'                               // 'D'
    str   r1, [r0]
    mov   r1, #'R'                               // 'R'
    str   r1, [r0]
/* Debug - UART console message */

    clear_delay_timer

    /* Delay about 5us */
    ldr   r2, =0x00000005                        // Set Timer3 Reload = 5 us
    init_delay_timer
delay_0:
    check_delay_timer
    bne   delay_0
    clear_delay_timer
    /* end delay 5us */

/******************************************************************************
 Init DRAM common registers
 ******************************************************************************/
    ldr   r0, =0x1e6e0000
    ldr   r1, =0xFC600309
    str   r1, [r0]

    ldr   r0, =0x1e6e0034                        // disable SDRAM reset
    ldr   r1, =0x000000C0
    str   r1, [r0]

    ldr   r0, =0x1e6e0008
    ldr   r1, =0x0044000B                        /* VGA */
    str   r1, [r0]

    ldr   r0, =0x1e6e0038
    ldr   r1, =0x00100000
    str   r1, [r0]

    ldr   r0, =0x1e6e003c
    ldr   r1, =0xFFFFFFFF
    str   r1, [r0]

    ldr   r0, =0x1e6e0040
    ldr   r1, =0x88888888
    str   r1, [r0]

    ldr   r0, =0x1e6e0044
    ldr   r1, =0x88888888
    str   r1, [r0]

    ldr   r0, =0x1e6e0048
    ldr   r1, =0x88888888
    str   r1, [r0]

    ldr   r0, =0x1e6e004c
    ldr   r1, =0x88888888
    str   r1, [r0]

    ldr   r0, =0x1e6e0050
    ldr   r1, =0x80000000
    str   r1, [r0]

    ldr   r0, =0x1e6e0054
    ldr   r1, =ASTMMC_DRAM_ECC_SIZE
    str   r1, [r0]

    ldr   r0, =0x1e6e0070
    ldr   r1, =0x00000000
    str   r1, [r0]
    add   r0, #0x4
    str   r1, [r0]
    add   r0, #0x4
    str   r1, [r0]
    add   r0, #0x4
    str   r1, [r0]

    ldr   r0, =0x1e6e0080
    ldr   r1, =0xFFFFFFFF
    str   r1, [r0]

    ldr   r0, =0x1e6e0084
    ldr   r1, =0x00000000
    str   r1, [r0]

    ldr   r0, =0x1e6e0100
    ldr   r1, =0x000000FF
    str   r1, [r0]

    /* Check DRAM Type by H/W Trapping */
//  ldr   r0, =0x1e6e2070
//  ldr   r1, [r0]
//  ldr   r2, =0x01000000                        // bit[24]=1 => DDR4
//  tst   r1, r2
#ifdef ASTMMC_DDR_DDR3
    b     ddr3_init
#else
    b     ddr4_init
#endif
.LTORG

/******************************************************************************
 DDR3 Init
 ******************************************************************************/
ddr3_init:
/* Debug - UART console message */
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, #'3'                               // '3'
    str   r1, [r0]
    mov   r1, #'\r'                              // '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              // '\n'
    str   r1, [r0]
/* Debug - UART console message */

#if   defined (ASTMMC_DRAM_800)                  // Init DRAM parameter table
    adrl  r5, TIME_TABLE_DDR3_800
#elif defined (ASTMMC_DRAM_1066)
    adrl  r5, TIME_TABLE_DDR3_1066
#elif defined (ASTMMC_DRAM_1333)
    adrl  r5, TIME_TABLE_DDR3_1333
#elif defined (ASTMMC_DRAM_1866)
    adrl  r5, TIME_TABLE_DDR3_1866
#else
    adrl  r5, TIME_TABLE_DDR3_1600
#endif

    ldr   r0, =0x1e6e0004
    ldr   r1, =0x00000006                        // 8Gb
    str   r1, [r0]

    ldr   r0, =0x1e6e0010
    mov   r2, #0x0                               // init loop counter
    mov   r3, r5
ddr3_init_param:
    ldr   r1, [r3]
    str   r1, [r0]
    add   r0, #0x4
    add   r3, #0x4
    add   r2, #0x1
    cmp   r2, #0x8
    blt   ddr3_init_param

    ldr   r0, =0x1e6e0034                        // PWRCTL, first time enable CKE, wait at least 200 us
    ldr   r1, =0x000000C1
    str   r1, [r0]

    /* Delay about 500us */
    ldr   r2, =0x000001F4                        // Set Timer3 Reload = 500 us
    init_delay_timer
ddr3_delay_cke_on:
    check_delay_timer
    bne   ddr3_delay_cke_on
    clear_delay_timer
    /* end delay 500us */

    ldr   r0, =0x1e6e000c
    ldr   r1, =0x00000040
    str   r1, [r0]

    ldr   r0, =0x1e6e0030
    ldr   r1, =0x00000005                        // MR2
    str   r1, [r0]
    ldr   r1, =0x00000007                        // MR3
    str   r1, [r0]
    ldr   r1, =0x00000003                        // MR1
    str   r1, [r0]
    ldr   r1, =0x00000011                        // MR0 + DLL_RESET
    str   r1, [r0]

    ldr   r0, =0x1e6e000c                        // REFSET
    ldr   r1, =0x00005D41
    str   r1, [r0]

    ldr   r0, =0x1e6e0034
    ldr   r2, =0x70000000
ddr3_check_dllrdy:
    ldr   r1, [r0]
    tst   r1, r2
    bne   ddr3_check_dllrdy

    ldr   r0, =0x1e6e000c
    ldr   r1, =0x40005DA1
    str   r1, [r0]

    ldr   r0, =0x1e6e0034
    ldr   r1, =0x000003A3
    str   r1, [r0]

    b     Calibration_End
.LTORG
/******************************************************************************
 End DDR3 Init
 ******************************************************************************/
/******************************************************************************
 DDR4 Init
 ******************************************************************************/
ddr4_init:
/* Debug - UART console message */
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, #'4'                               // '4'
    str   r1, [r0]
    mov   r1, #'\r'                              // '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              // '\n'
    str   r1, [r0]
/* Debug - UART console message */

#if   defined (ASTMMC_DRAM_1333)
    adrl  r5, TIME_TABLE_DDR4_1333               // Init DRAM parameter table
#elif defined (ASTMMC_DRAM_1866)
    adrl  r5, TIME_TABLE_DDR4_1866
#else
    adrl  r5, TIME_TABLE_DDR4_1600
#endif

    ldr   r0, =0x1e6e0004
#ifdef ASTMMC_DDR4_8GX8
    ldr   r1, =0x00000037                        // Init to 16GB
#else
    ldr   r1, =0x00000017                        // Init to 16GB
#endif
    str   r1, [r0]

    ldr   r0, =0x1e6e0010
    mov   r2, #0x0                               // init loop counter
    mov   r3, r5
ddr4_init_param:
    ldr   r1, [r3]
    str   r1, [r0]
    add   r0, #0x4
    add   r3, #0x4
    add   r2, #0x1
    cmp   r2, #0x8
    blt   ddr4_init_param

    ldr   r0, =0x1e6e0034                        // PWRCTL, 1st enable CKE, wait at least 200 us
    ldr   r1, =0x000000C1
    str   r1, [r0]

    /* Delay about 500us */
    ldr   r2, =0x000001F4                        // Set Timer3 Reload = 500 us
    init_delay_timer
ddr4_delay_cke_on:
    check_delay_timer
    bne   ddr4_delay_cke_on
    clear_delay_timer
    /* end delay 500us */

    ldr   r0, =0x1e6e000c
    ldr   r1, =0x00000040
    str   r1, [r0]

    ldr   r0, =0x1e6e0030
    ldr   r1, =0x00000007                        // MR3
    str   r1, [r0]
    ldr   r1, =0x0000000D                        // MR6
    str   r1, [r0]
    ldr   r1, =0x0000000B                        // MR5
    str   r1, [r0]
    ldr   r1, =0x00000009                        // MR4
    str   r1, [r0]
    ldr   r1, =0x00000005                        // MR2
    str   r1, [r0]
    ldr   r1, =0x00000003                        // MR1
    str   r1, [r0]
    ldr   r1, =0x00000011                        // MR0 + DLL_RESET
    str   r1, [r0]

    ldr   r0, =0x1e6e000c                        // REFSET
    ldr   r1, =0x00005D41
    str   r1, [r0]

    ldr   r0, =0x1e6e0034
    ldr   r2, =0x70000000
ddr4_check_dllrdy:
    ldr   r1, [r0]
    tst   r1, r2
    bne   ddr4_check_dllrdy

    ldr   r0, =0x1e6e000c
    ldr   r1, =0x40005DA1
    str   r1, [r0]

    ldr   r0, =0x1e6e0034
    ldr   r1, =0x000003A3
    str   r1, [r0]

    b     Calibration_End
.LTORG
/******************************************************************************
 Common Process
 *****************************************************************************/

/******************************************************************************
 Other features configuration
 *****************************************************************************/
Calibration_End:

    /***************************************
     Search read gating window
     Use r7, r8, r9
    ****************************************/
    mov   r7, #0xC                               // win

    ldr   r0, =0x1e6e0100
    str   r7, [r0]

ddr_gate_train_pass:
/* Debug - UART console message */
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, #'P'                               // 'P'
    str   r1, [r0]
    mov   r1, #'a'                               // 'a'
    str   r1, [r0]
    mov   r1, #'s'                               // 's'
    str   r1, [r0]
    mov   r1, #'s'                               // 's'
    str   r1, [r0]
    mov   r1, #'\r'                              // '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              // '\n'
    str   r1, [r0]
    ldr   r0, =(ASTMMC_UART_BASE | 0x14)

    /*******************************
     Check DRAM Size
     2Gb : 0x80000000 ~ 0x8FFFFFFF
     4Gb : 0x80000000 ~ 0x9FFFFFFF
     8Gb : 0x80000000 ~ 0xBFFFFFFF
     16Gb: 0x80000000 ~ 0xFFFFFFFF
    *******************************/
#ifndef ASTMMC_DDR4_8GX8
    ldr   r0, =0x1e6e0004
    ldr   r6, [r0]
    bic   r6, r6, #0x00000003                    // record MCR04
    ldr   r7, [r5, #ASTMMC_REG_RFC]

check_dram_size:
    ldr   r0, =0xC0100000
    ldr   r1, =0xC1C2C3C4
    str   r1, [r0]
    ldr   r0, =0xA0100000
    ldr   r1, =0xA1A2A3A4
    str   r1, [r0]
    ldr   r0, =0x90100000
    ldr   r1, =0x91929394
    str   r1, [r0]
    ldr   r0, =0x80100000
    ldr   r1, =0x81828384
    str   r1, [r0]
    ldr   r0, =0xC0100000
    ldr   r1, =0xC1C2C3C4
    ldr   r2, [r0]
    mov   r3, #0x16                              // '16'
    cmp   r2, r1                                 // == 16Gbit
    orreq r6, r6, #0x03
    beq   check_dram_size_end
    mov   r7, r7, lsr #8
    ldr   r0, =0xA0100000
    ldr   r1, =0xA1A2A3A4
    ldr   r2, [r0]
    mov   r3, #0x08                              // '8'
    cmp   r2, r1                                 // == 8Gbit
    orreq r6, r6, #0x02
    beq   check_dram_size_end
    mov   r7, r7, lsr #8
    ldr   r0, =0x90100000
    ldr   r1, =0x91929394
    ldr   r2, [r0]
    mov   r3, #0x04                              // '4'
    cmp   r2, r1                                 // == 4Gbit
    orreq r6, r6, #0x01
    beq   check_dram_size_end
    mov   r7, r7, lsr #8                         // == 2Gbit
    mov   r3, #0x02                              // '2'

check_dram_size_end:
    ldr   r0, =0x1e6e0004
    str   r6, [r0]
    ldr   r0, =0x1e6e0014
    ldr   r1, [r0]
    bic   r1, r1, #0x000000FF
    and   r7, r7, #0xFF
    orr   r1, r1, r7
    str   r1, [r0]
#endif

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
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, #'S'                               // 'S'
    str   r1, [r0]
    mov   r1, #'i'                               // 'i'
    str   r1, [r0]
    mov   r1, #'z'                               // 'z'
    str   r1, [r0]
    mov   r1, #'e'                               // 'e'
    str   r1, [r0]
    mov   r1, #'-'                               // '-'
    str   r1, [r0]
    mov   r2, r3
    print_hex_byte
    mov   r1, #'G'                               // 'G'
    str   r1, [r0]
    mov   r1, #'b'                               // 'b'
    str   r1, [r0]
    mov   r1, #'\r'                              // '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              // '\n'
    str   r1, [r0]
    ldr   r0, =(ASTMMC_UART_BASE | 0x14)
wait_print_2:
    ldr   r1, [r0]
    tst   r1, #0x40
    beq   wait_print_2
/* Debug - UART console message */

    /********************************************
     DDRTest
     Use r0, r1, r2, r3, r6, r11
    ********************************************/
    mov   r9, #0x1                               // jump indication
ddr_test_start:
    ldr   r0, =0x1e6e0074
    ldr   r1, =0x000000FF                        // test size = 16MB
    str   r1, [r0]
    ldr   r0, =0x1e6e007c
    ldr   r1, =0xFF00FF00
    str   r1, [r0]

ddr_test_single:
    mov   r6, #0x00                              // initialize loop index, r1 is loop index
ddr_test_single_loop:
/* Debug - UART console message */
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, r6
    print_hex_char
/* Debug - UART console message */
    ldr   r0, =0x1e6e0070
    ldr   r2, =0x00000000
    str   r2, [r0]
    mov   r2, r6, lsl #3
    orr   r2, r2, #0x85                          // test command = 0x85 | (datagen << 3)
    str   r2, [r0]
    ldr   r3, =0x3000
    ldr   r11, =0x500000
ddr_wait_engine_idle_0:
    subs  r11, r11, #1
    beq   ddr_test_fail
    ldr   r2, [r0]
    tst   r2, r3                                 // D[12] = idle bit
    beq   ddr_wait_engine_idle_0

    ldr   r0, =0x1e6e0070                        // read fail bit status
    ldr   r3, =0x2000
    ldr   r2, [r0]
    tst   r2, r3                                 // D[13] = fail bit
    bne   ddr_test_fail

    add   r6, r6, #1                             // increase the test mode index
    cmp   r6, #0x08                              // test 8 modes
    bne   ddr_test_single_loop

ddr_test_burst:
    mov   r6, #0x00                              // initialize loop index, r1 is loop index
ddr_test_burst_loop:
/* Debug - UART console message */
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, r6
    print_hex_char
/* Debug - UART console message */
    ldr   r0, =0x1e6e0070
    ldr   r2, =0x00000000
    str   r2, [r0]
    mov   r2, r6, lsl #3
    orr   r2, r2, #0xC1                          // test command = 0xC1 | (datagen << 3)
    str   r2, [r0]
    ldr   r3, =0x3000
    ldr   r11, =0x500000
ddr_wait_engine_idle_1:
    subs  r11, r11, #1
    beq   ddr_test_fail
    ldr   r2, [r0]
    tst   r2, r3                                 // D[12] = idle bit
    beq   ddr_wait_engine_idle_1

    ldr   r0, =0x1e6e0070                        // read fail bit status
    ldr   r3, =0x2000
    ldr   r2, [r0]
    tst   r2, r3                                 // D[13] = fail bit
    bne   ddr_test_fail

    add   r6, r6, #1                             // increase the test mode index
    cmp   r6, #0x08                              // test 8 modes
    bne   ddr_test_burst_loop

    ldr   r0, =0x1e6e0070
    ldr   r1, =0x00000000
    str   r1, [r0]

    cmp   r9, #0x0
    beq   ddr_gate_train_pass
    b     set_scratch                            // CBRTest() return(1)

ddr_test_fail:
/* Debug - UART console message */
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, #'F'                               // 'F'
    str   r1, [r0]
    mov   r1, #'a'                               // 'a'
    str   r1, [r0]
    mov   r1, #'i'                               // 'i'
    str   r1, [r0]
    mov   r1, #'l'                               // 'l'
    str   r1, [r0]
    mov   r1, #'\r'                              // '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              // '\n'
    str   r1, [r0]
    ldr   r0, =(ASTMMC_UART_BASE | 0x14)
wait_print_1:
    ldr   r1, [r0]
    tst   r1, #0x40
    beq   wait_print_1
/* Debug - UART console message */
    b     ddr_init_start

set_scratch:
    /*Set Scratch register Bit 6 after ddr initial finished */
/*  ldr   r0, =0x1e6e2100
    ldr   r1, [r0]
    orr   r1, r1, #0x41
    str   r1, [r0]
*/

/* Debug - UART console message */
    ldr   r0, =ASTMMC_UART_BASE
    mov   r1, #'D'                               // 'D'
    str   r1, [r0]
    mov   r1, #'o'                               // 'o'
    str   r1, [r0]
    mov   r1, #'n'                               // 'n'
    str   r1, [r0]
    mov   r1, #'e'                               // 'e'
    str   r1, [r0]
    mov   r1, #'\r'                              // '\r'
    str   r1, [r0]
    mov   r1, #'\n'                              // '\n'
    str   r1, [r0]
/* Debug - UART console message */

platform_exit:

#ifdef ASTMMC_DRAM_ECC
    ldr   r0, =0x1e6e0004
    ldr   r2, =0x00000280
    ldr   r1, [r0]
    orr   r1, r1, r2
    str   r1, [r0]

    ldr   r0, =0x1e6e007C
    ldr   r1, =0x00000000
    str   r1, [r0]
    ldr   r0, =0x1e6e0074
    str   r1, [r0]

    ldr   r0, =0x1e6e0070
    ldr   r1, =0x00000221
    str   r1, [r0]

    ldr   r2, =0x00001000
ECC_Init_Flag:
    ldr   r1, [r0]
    tst   r1, r2                                 @ D[12] = 1, Done
    beq   ECC_Init_Flag

    ldr   r1, =0x00000000
    str   r1, [r0]

    ldr   r0, =0x1e6e0050
    ldr   r1, =0x80000000
    str   r1, [r0]

    ldr   r0, =0x1e6e0050
    ldr   r1, =0x40000000
    str   r1, [r0]
#endif

    /* restore lr */
    mov   lr, r4

    /* back to arch calling code */
    mov   pc, lr

secondary_cpu_init:
#if CONFIG_ASPEED_NONSECUR_MODE
    mov   r6,pc
	bl    start_sec
#endif
wait_for_kickup:
	wfe
	ldr r2,[r3]
	cmp r2,r4
	bne wait_for_kickup

	MOV r2,#'['
	STR r2,[r1]
	MOV r2,#'1'
	STR r2,[r1]
	MOV r2,#'C'
	STR r2,[r1]
	MOV r2,#'P'
	STR r2,[r1]
	MOV r2,#'U'
	STR r2,[r1]
	MOV r2,#']'
	STR r2,[r1]
	MOV r2,#'\n'
	STR r2,[r1]
	MOV r2,#'\r'
	STR r2,[r1]
	ldr pc, [r0]
	ldr pc, [r0]
	b wait_for_kickup

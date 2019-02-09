#ifndef GIC_INIT_H_INCLUDED
#define GIC_INIT_H_INCLUDED

#define enable_interrupts() __asm__ __volatile__ ("cpsie i" : : : "memory")
#define disable_interrupts() __asm__ __volatile__ ("cpsid i" : : : "memory")
#define enable_fiq() __asm__ __volatile__ ("cpsie f" : : : "memory")
#define disable_fiq() __asm__ __volatile__ ("cpsid f" : : : "memory")
// Compute the number of elements in the given array /
#define ARRAY_SIZE(a)    \
 (sizeof(a) / sizeof((a)[0]))
 //(sizeof(a) / sizeof((a)[0]))

/* GICD_CTLR bit definitions */
#define CTLR_ENABLE_G0_SHIFT            0
#define CTLR_ENABLE_G0_MASK             0x1
#define CTLR_ENABLE_G0_BIT              (1 << CTLR_ENABLE_G0_SHIFT)


#define GIC_HIGHEST_SEC_PRIORITY        0x0
#define GIC_HIGHEST_NS_PRIORITY         0x80
#define GIC_LOWEST_SEC_PRIORITY         0x7f
#define GIC_LOWEST_NS_PRIORITY          0xfe /* 0xff would disable all interrupts */

/* Value used to initialize Normal world interrupt priorities four at a time */
#define GICD_IPRIORITYR_DEF_VAL                 \
        (GIC_HIGHEST_NS_PRIORITY        |       \
        (GIC_HIGHEST_NS_PRIORITY << 8)  |       \
        (GIC_HIGHEST_NS_PRIORITY << 16) |       \
        (GIC_HIGHEST_NS_PRIORITY << 24))


/* Interrupt group definitions */
#define GICV2_INTR_GROUP0       0
#define GICV2_INTR_GROUP1       1

#define ARM_IRQ_SEC_PHY_TIMER           29
#define FVP_IRQ_TZ_WDOG                 56
#define FVP_IRQ_SEC_SYS_TIMER           57

/*
Note : Each of this should be added to 32 to get to the exact
interrupt into the GIC

96	irq_espi_flash_bmc,	MUX_PILOT5_IRQS[16]
95	irq_espi_oob_bmc,	MUX_PILOT5_IRQS[15]
94	irq_espi_vwire_bmc,	MUX_PILOT5_IRQS[14]
93	PER_I2C13_INTR,	MUX_PILOT5_IRQS[13]
92	PER_I2C12_INTR,	MUX_PILOT5_IRQS[12]
91	PER_I2C11_INTR,	MUX_PILOT5_IRQS[11]
90	PER_I2C10_INTR,	MUX_PILOT5_IRQS[10]
89	PER_I2C9_INTR,	MUX_PILOT5_IRQS[9]
88	PER_I2C8_INTR,	MUX_PILOT5_IRQS[8]
87	PER_I2C7_INTR,	MUX_PILOT5_IRQS[7]
86	PER_I2C6_INTR,	MUX_PILOT5_IRQS[6]
85	PER_I2C5_INTR,	MUX_PILOT5_IRQS[5]
84	PER_I2C4_INTR,	MUX_PILOT5_IRQS[4]
83	PER_I2C3_INTR,	MUX_PILOT5_IRQS[3]
82	PER_I2C2_INTR,	MUX_PILOT5_IRQS[2]
81	PER_I2C1_INTR,	MUX_PILOT5_IRQS[1]
80	PER_I2C0_INTR	MUX_PILOT5_IRQS[0]
79	NAND_INTR,	PILOT5_IRQS[12]
78	SPI_INTR,	PILOT5_IRQS[11]
77	SYS_RTC_INTR,	PILOT5_IRQS[10]
76	irq_vsp2_bmc,	PILOT5_IRQS[9]
75	irq_vsp1_bmc,	PILOT5_IRQS[8]
74	irq_psr_bmc,	PILOT5_IRQS[7]
73	irq_wdt_bmc,	PILOT5_IRQS[6]
72	PER_UART2_INTR,	PILOT5_IRQS[5]
71	PER_UART1_INTR,	PILOT5_IRQS[4]
70	PER_UART0_INTR,	PILOT5_IRQS[3]
69	SIO_KCS_INTR,	PILOT5_IRQS[2]
68	irq_gpio_bmc,	PILOT5_IRQS[1]
67	FGB_INTR0	PILOT5_IRQS[0]
*/
#define IRQ_NO_CONST_OFFSET     32
#define TIMER8_INTR             (23 + IRQ_NO_CONST_OFFSET)
#define FGB_INTR0               (67 + IRQ_NO_CONST_OFFSET)
#define irq_gpio_bmc            (68 + IRQ_NO_CONST_OFFSET)
#define SIO_KCS_INTR            (69 + IRQ_NO_CONST_OFFSET)
#define PER_UART0_INTR          (70 + IRQ_NO_CONST_OFFSET)
#define PER_UART1_INTR          (71 + IRQ_NO_CONST_OFFSET)
#define PER_UART2_INTR          (72 + IRQ_NO_CONST_OFFSET)
#define irq_wdt_bmc             (73 + IRQ_NO_CONST_OFFSET)
#define irq_psr_bmc             (74 + IRQ_NO_CONST_OFFSET)
#define irq_vsp1_bmc            (75 + IRQ_NO_CONST_OFFSET)
#define irq_vsp2_bmc            (76 + IRQ_NO_CONST_OFFSET)
#define SYS_RTC_INTR            (77 + IRQ_NO_CONST_OFFSET)
#define SPI_INTR                (78 + IRQ_NO_CONST_OFFSET)
#define NAND_INTR               (79 + IRQ_NO_CONST_OFFSET)
#define PER_I2C0_INTR           (80 + IRQ_NO_CONST_OFFSET)
#define PER_I2C1_INTR           (81 + IRQ_NO_CONST_OFFSET)
#define PER_I2C2_INTR           (82 + IRQ_NO_CONST_OFFSET)
#define PER_I2C3_INTR           (83 + IRQ_NO_CONST_OFFSET)
#define PER_I2C4_INTR           (84 + IRQ_NO_CONST_OFFSET)
#define PER_I2C5_INTR           (85 + IRQ_NO_CONST_OFFSET)
#define PER_I2C6_INTR           (86 + IRQ_NO_CONST_OFFSET)
#define PER_I2C7_INTR           (87 + IRQ_NO_CONST_OFFSET)
#define PER_I2C8_INTR           (88 + IRQ_NO_CONST_OFFSET)
#define PER_I2C9_INTR           (89 + IRQ_NO_CONST_OFFSET)
#define PER_I2C10_INTR          (90 + IRQ_NO_CONST_OFFSET)
#define PER_I2C11_INTR          (91 + IRQ_NO_CONST_OFFSET)
#define PER_I2C12_INTR          (92 + IRQ_NO_CONST_OFFSET)
#define PER_I2C13_INTR          (93 + IRQ_NO_CONST_OFFSET)
#define irq_espi_vwire_bmc      (94 + IRQ_NO_CONST_OFFSET)
#define irq_espi_oob_bmc        (95 + IRQ_NO_CONST_OFFSET)
#define irq_espi_flash_bmc      (96 + IRQ_NO_CONST_OFFSET)

#define PILOT5_IRQ_I2C0			112


#define ARM_IRQ_SEC_SGI_0               8
#define ARM_IRQ_SEC_SGI_1               9
#define ARM_IRQ_SEC_SGI_2               10
#define ARM_IRQ_SEC_SGI_3               11
#define ARM_IRQ_SEC_SGI_4               12
#define ARM_IRQ_SEC_SGI_5               13
#define ARM_IRQ_SEC_SGI_6               14
#define ARM_IRQ_SEC_SGI_7               15

/* Priority levels for ARM platforms */
#define PLAT_RAS_PRI                    0x10
#define PLAT_SDEI_CRITICAL_PRI          0x60
#define PLAT_SDEI_NORMAL_PRI            0x70

/* Interrupt configurations: 2-bit fields with LSB reserved */
#define GIC_INTR_CFG_LEVEL              (0 << 1)
#define GIC_INTR_CFG_EDGE               (1 << 1)

/* Create an interrupt property descriptor from various interrupt properties */
#define INTR_PROP_DESC(num, pri, grp, cfg) \
        { \
                .intr_num = num, \
                .intr_pri = pri, \
                .intr_grp = grp, \
                .intr_cfg = cfg, \
        }

typedef struct interrupt_prop {
        unsigned int intr_num:10;
        unsigned int intr_pri:8;
        unsigned int intr_grp:2;
        unsigned int intr_cfg:2;
} interrupt_prop_t;

#define FIQ_EN_SHIFT            3
#define FIQ_EN_BIT              (1 << FIQ_EN_SHIFT)
/* GICC_CTLR bit definitions */
#define EOI_MODE_NS             ((1) << 10)
#define EOI_MODE_S              ((1) << 9)
#define IRQ_BYP_DIS_GRP1        ((1) << 8)
#define FIQ_BYP_DIS_GRP1        ((1) << 7)
#define IRQ_BYP_DIS_GRP0        ((1) << 6)
#define FIQ_BYP_DIS_GRP0        ((1) << 5)
#define CBPR                    ((1) << 4)
#define FIQ_EN                  ((1) << 3)
#define ACK_CTL                 ((1) << 2)




#define ARM_G0_IRQ_PROPS(grp) \
        INTR_PROP_DESC(TIMER8_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL)



/*
#define ARM_G0_IRQ_PROPS(grp) \
        INTR_PROP_DESC(ARM_IRQ_SEC_SGI_0, PLAT_SDEI_NORMAL_PRI, grp, \
                        GIC_INTR_CFG_EDGE), \
        INTR_PROP_DESC(TIMER8_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(FGB_INTR0, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(irq_gpio_bmc, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(SIO_KCS_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_UART0_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_UART1_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_UART2_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(irq_wdt_bmc, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(irq_psr_bmc, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(irq_vsp1_bmc, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(irq_vsp2_bmc, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(SYS_RTC_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(SPI_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(NAND_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C0_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C1_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C2_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C3_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C4_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C5_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C6_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C7_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C8_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C9_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C10_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C11_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C12_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(PER_I2C13_INTR, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(irq_espi_vwire_bmc, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(irq_espi_oob_bmc, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(irq_espi_flash_bmc, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(ARM_IRQ_SEC_SGI_6, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_EDGE)

*/
#define ARM_G1S_IRQ_PROPS(grp) \
        INTR_PROP_DESC(ARM_IRQ_SEC_PHY_TIMER, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(ARM_IRQ_SEC_SGI_1, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_EDGE), \
        INTR_PROP_DESC(ARM_IRQ_SEC_SGI_2, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_EDGE), \
        INTR_PROP_DESC(ARM_IRQ_SEC_SGI_3, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_EDGE), \
        INTR_PROP_DESC(ARM_IRQ_SEC_SGI_4, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_EDGE), \
        INTR_PROP_DESC(ARM_IRQ_SEC_SGI_5, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_EDGE), \
        INTR_PROP_DESC(ARM_IRQ_SEC_SGI_7, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_EDGE)

/*
#define PLAT_ARM_G1S_IRQ_PROPS(grp) \
        ARM_G1S_IRQ_PROPS(grp), \
        INTR_PROP_DESC(FVP_IRQ_TZ_WDOG, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL), \
        INTR_PROP_DESC(FVP_IRQ_SEC_SYS_TIMER, GIC_HIGHEST_SEC_PRIORITY, grp, \
                        GIC_INTR_CFG_LEVEL)
*/
#define PLAT_ARM_G0_IRQ_PROPS(grp)      ARM_G0_IRQ_PROPS(grp)

#endif // GIC_INIT_H_INCLUDED

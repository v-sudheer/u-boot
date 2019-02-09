//#include <defines.h>
#include "gic_init.h"
#define PLAT_ARM_GICD_BASE 0x40461000
#define PLAT_ARM_GICC_BASE 0x40462000

unsigned int target_mask_array[2]={1000,1000};

#define uintptr_t unsigned int
// Constants to categorise interrupts
#define MIN_SGI_ID              0
#define MIN_SEC_SGI_ID          8
#define MIN_PPI_ID              16
#define MIN_SPI_ID              32
//#define MIN_SPI_ID              128
#define MAX_SPI_ID              1019

#define CTLR_ENABLE_G1_BIT (1<<1)
extern void (* gic_handlers[256])(void);

//__smc(0) void smc(void);

/*******************************************************************************
 * GIC Distributor interface register offsets that are common to GICv3 & GICv2
 ******************************************************************************/
#define GICD_CTLR               0x0
#define GICD_TYPER              0x4
#define GICD_IIDR               0x8
#define GICD_IGROUPR            0x80
#define GICD_ISENABLER          0x100
#define GICD_ICENABLER          0x180
#define GICD_ISPENDR            0x200
#define GICD_ICPENDR            0x280
#define GICD_ISACTIVER          0x300
#define GICD_ICACTIVER          0x380
#define GICD_IPRIORITYR         0x400
#define GICD_ICFGR              0xc00
#define GICD_NSACR              0xe00

/* Physical CPU Interface registers */
#define GICC_CTLR               (0x0)
#define GICC_PMR                (0x4)
#define GICC_BPR                (0x8)
#define GICC_IAR                (0xC)
#define GICC_EOIR               (0x10)
#define GICC_RPR                (0x14)
#define GICC_HPPIR              (0x18)
#define GICC_AHPPIR             (0x28)
#define GICC_IIDR               (0xFC)
#define GICC_DIR                (0x1000)
#define GICC_PRIODROP           GICC_EOIR


/* GICD_CTLR bit definitions */
#define CTLR_ENABLE_G0_SHIFT            0
#define CTLR_ENABLE_G0_MASK             0x1
#define CTLR_ENABLE_G0_BIT              (1 << CTLR_ENABLE_G0_SHIFT)

/* Distributor interface definitions */
#define GICD_ITARGETSR          (0x800)


static const interrupt_prop_t arm_interrupt_props[] = {
	//        PLAT_ARM_G1S_IRQ_PROPS(GICV2_INTR_GROUP1),
	PLAT_ARM_G0_IRQ_PROPS(GICV2_INTR_GROUP0)
};
static inline unsigned int gicc_read_iar(uintptr_t base)
{
	return *((volatile unsigned int *)(base + GICC_IAR));
}
static inline void gicc_write_eoi(uintptr_t base, unsigned int val)
{
	*((volatile unsigned int *)(base + GICC_EOIR)) = val;
}
static inline void gicc_write_pmr(uintptr_t base, unsigned int val)
{
	*((volatile unsigned int *)(base + GICC_PMR)) = val;
}
static inline void gicc_write_ctlr(uintptr_t base, unsigned int val)
{
	*((volatile unsigned int *)(base + GICC_CTLR)) = val;
}

static inline unsigned int gicd_read_ctlr(unsigned int base)
{
	return *((volatile unsigned int *)(base + GICD_CTLR));
}
static inline void gicd_write_ctlr(unsigned int base, unsigned int val)
{
	*((volatile unsigned int *)(base + GICD_CTLR)) = val;
}
static inline unsigned int gicd_read_typer(unsigned int base)
{
	return *((volatile unsigned int *)(base + GICD_TYPER));
}
unsigned int gicd_read_igroupr(uintptr_t base, unsigned int id)
{
	unsigned n = id >> 5; //IGROUPR_SHIFT;
	return *((volatile unsigned int *)(base + GICD_IGROUPR + (n << 2)));
}

void gicd_write_igroupr(uintptr_t base, unsigned int id, unsigned int val)
{
	unsigned int n = id >> 5; //IGROUPR_SHIFT;
	*((volatile unsigned int *)(base + GICD_IGROUPR + (n << 2))) = val;
	//*((volatile unsigned int *)(0x40100200)) = val;
}

void gicd_set_ipriorityr(uintptr_t base, unsigned int id, unsigned int pri)
{
	*((volatile unsigned char*)(base + GICD_IPRIORITYR + id)) = (pri & 0xff);//GIC_PRI_MASK);
}

void gicd_set_itargetsr(uintptr_t base, unsigned int id, unsigned int target)
{
	*((volatile unsigned char*)(base + GICD_ITARGETSR + id)) = (target & 0xff);//GIC_TARGET_CPU_MASK);
}

void gicd_write_ipriorityr(uintptr_t base, unsigned int id, unsigned int val)
{
	unsigned n = id >> 2;//IPRIORITYR_SHIFT;
	*((volatile unsigned int *)(base + GICD_IPRIORITYR + (n << 2))) = val;
}

void gicd_write_icfgr(uintptr_t base, unsigned int id, unsigned int val)
{
	unsigned n = id >> 4;//ICFGR_SHIFT;
	*((volatile unsigned int *)(base + GICD_ICFGR + (n << 2))) = val;
}
unsigned int gicd_read_icfgr(uintptr_t base, unsigned int id)
{
	unsigned n = id >> 4;//ICFGR_SHIFT;
	return *((volatile unsigned int *)(base + GICD_ICFGR + (n << 2)));
}

void gicd_clr_igroupr(uintptr_t base, unsigned int id)
{
	//unsigned bit_num = id & ((1 << IGROUPR_SHIFT) - 1);
	unsigned bit_num = id & ((1 << 5) - 1);
	unsigned int reg_val = gicd_read_igroupr(base, id);

	gicd_write_igroupr(base, id, reg_val & ~(1 << bit_num));
}
void gicd_set_icfgr(uintptr_t base, unsigned int id, unsigned int cfg)
{
	/* Interrupt configuration is a 2-bit field */
	unsigned int bit_num = id & ((1 << 4) - 1);//ICFGR_SHIFT
	unsigned int bit_shift = bit_num << 1;

	unsigned int  reg_val = gicd_read_icfgr(base, id);

	/* Clear the field, and insert required configuration */
	reg_val &= ~(3 << bit_shift);//GIC_CFG_MASK = 3
	reg_val |= ((cfg & 3) << bit_shift);//GIC_CFG_MASK = 3

	gicd_write_icfgr(base, id, reg_val);
}
void gicd_write_isenabler(uintptr_t base, unsigned int id, unsigned int val)
{

	unsigned n = id >> 5;//ISENABLER_SHIFT = 5
	//mmio_write_32(base + GICD_ISENABLER + (n << 2), val);
	*((volatile unsigned int *)(base + GICD_ISENABLER + (n << 2))) = val;
}
void gicd_write_isactiver(uintptr_t base, unsigned int id)
{
	unsigned bit_num = id & ((1 << 5) - 1);
	unsigned n = id >> 5;//ISENABLER_SHIFT = 5
	//mmio_write_32(base + GICD_ISENABLER + (n << 2), val);
	*((volatile unsigned int *)(base + GICD_ISACTIVER + (n << 2))) = 1<<bit_num;
}
void gicd_write_icenabler(uintptr_t base, unsigned int id, unsigned int val)
{
	unsigned n = id >> 5;//ICENABLER_SHIFT = 5
	//mmio_write_32(base + GICD_ICENABLER + (n << 2), val);
	*((volatile unsigned int *)(base + GICD_ICENABLER + (n << 2))) = val;
}
unsigned int gicd_read_isenabler(uintptr_t base, unsigned int id)
{
	unsigned n = id >> 5;
	return *((volatile unsigned int *)(base + GICD_ISENABLER + (n << 2)));
}
void gicd_set_isenabler(uintptr_t base, unsigned int id)
{
	unsigned bit_num = id & ((1 << 5) - 1);//ISENABLER_SHIFT = 5

	unsigned int data = gicd_read_isenabler(base, id);
	gicd_write_isenabler(base, id, (data | (1 << bit_num)));
}
void gicd_unset_isenabler(uintptr_t base, unsigned int id)
{
	unsigned bit_num = id & ((1 << 5) - 1);//ISENABLER_SHIFT = 5

	unsigned int data = gicd_read_isenabler(base, id);
	gicd_write_isenabler(base, id, (data & (~(1 << bit_num))) );
}

unsigned int gicd_read_itargetsr(uintptr_t base, unsigned int id)
{
	unsigned n = id >> 2; //ITARGETSR_SHIFT;
	//Write_hex(*((volatile unsigned int *)(base + GICD_ITARGETSR     + (n << 2))));
	return *((volatile unsigned int *)(base + GICD_ITARGETSR     + (n << 2)));
}
unsigned int gicv2_get_cpuif_id(uintptr_t base)
{
	unsigned int val;

	val = gicd_read_itargetsr(base, 0);
	return val & 0xff; //GIC_TARGET_CPU_MASK;
}

void gicv2_spis_configure_defaults(uintptr_t gicd_base)
{
	volatile unsigned int index, num_ints;
	//num_ints = gicd_read_typer(PLAT_ARM_GICD_BASE);
	num_ints = *((volatile unsigned int *)(PLAT_ARM_GICD_BASE + GICD_IIDR));
	num_ints = *((volatile unsigned int *)(PLAT_ARM_GICC_BASE + GICC_IIDR));

	num_ints = gicd_read_typer(PLAT_ARM_GICD_BASE);
	num_ints &= 0x1f;//TYPER_IT_LINES_NO_MASK
	num_ints = (num_ints + 1) << 5;
	/* Disable all SGI and PPI before we are configuring them */
	gicd_write_icenabler(gicd_base, 0, ~0);

	//*((volatile unsigned int*) (0x40461084)) = 0xFFFFFFFF;
	//*((volatile unsigned int*) (0x40461088)) = 0xFFFFFFFF;
	//*((volatile unsigned int*) (0x4046108c)) = 0xFFFFFFFF;
	/*
	 * Treat all SPIs as G1NS by default. The number of interrupts is
	 * calculated as 32 * (IT_LINES + 1). We do 32 at a time.
	 */
/*	for (index = MIN_SPI_ID; index < num_ints; index += 32)
		gicd_write_igroupr(PLAT_ARM_GICD_BASE, index, ~0U);	*/
	for (index = MIN_SGI_ID; index < num_ints; index += 32)
		gicd_write_igroupr(PLAT_ARM_GICD_BASE, index, ~0U);

	/* Setup the default SPI priorities doing four at a time */
/*	for (index = MIN_SPI_ID; index < num_ints; index += 4)
		gicd_write_ipriorityr(PLAT_ARM_GICD_BASE,
				index,
				GICD_IPRIORITYR_DEF_VAL);
*/
	for (index = MIN_SGI_ID; index < num_ints; index += 4)
		gicd_write_ipriorityr(PLAT_ARM_GICD_BASE,
				index,
				GICD_IPRIORITYR_DEF_VAL);
	/* Treat all SPIs as level triggered by default, 16 at a time */
/*	for (index = MIN_SPI_ID; index < num_ints; index += 16)
		gicd_write_icfgr(PLAT_ARM_GICD_BASE, index, 0);	*/
	for (index = MIN_SGI_ID; index < num_ints; index += 16)
		gicd_write_icfgr(PLAT_ARM_GICD_BASE, index, 0);
}
void gic_mask_irq(int irq_nr)
{
	unsigned int  mask = 1 << (irq_nr % 32);

	//writel_relaxed(mask, gic_dist_base + GIC_DIST_ENABLE_CLEAR + (irq_nr / 32) * 4);
	unsigned int i;
	const interrupt_prop_t *prop_desc;
	unsigned int data;

	//unsigned int  mask = 1 << (irq_nr % 32);
	//nc_printf("Enabling gic irq %d gic_dist_base %x\n", irq_nr, gic_dist_base);
	//writel_relaxed(mask, gic_dist_base + GIC_DIST_ENABLE_SET + ((irq_nr / 32) * 4));
	//	nc_printf("unmask:GIC_DIST_ENABLE_SET %x = %x\n",gic_dist_base + GIC_DIST_ENABLE_SET + ((irq_nr / 32) * 4), readl_relaxed(gic_dist_base + GIC_DIST_ENABLE_SET + ((irq_nr / 32) * 4)));
	//get_cpsr();
	for (i = 0; i < ARRAY_SIZE(arm_interrupt_props); i++) {
		prop_desc = &arm_interrupt_props[i];

		//We are interested only in the SPI Interupts and only that
		//interrupt number that we are interested in
		if ( (prop_desc->intr_num < MIN_SPI_ID) || (prop_desc->intr_num != irq_nr) )
			continue;

		/* Mask/Disable this interrupt */
		//gicd_unset_isenabler(gicd_base, prop_desc->intr_num);
		gicd_unset_isenabler(PLAT_ARM_GICD_BASE, prop_desc->intr_num);

		//              nc_printf("Interrupt disabled for interrupt %x from grp %x is %x\n" , prop_desc->intr_num , prop_desc->intr_grp ,gicd_read_isenabler(PLAT_ARM_GICD_BASE, prop_desc->intr_num));
	}
}

void gic_unmask_irq(int irq_nr)
{
	unsigned int i;
	const interrupt_prop_t *prop_desc;
	unsigned int data;

	//unsigned int  mask = 1 << (irq_nr % 32);
	//nc_printf("Enabling gic irq %d gic_dist_base %x\n", irq_nr, gic_dist_base);
	//writel_relaxed(mask, gic_dist_base + GIC_DIST_ENABLE_SET + ((irq_nr / 32) * 4));
	//	nc_printf("unmask:GIC_DIST_ENABLE_SET %x = %x\n",gic_dist_base + GIC_DIST_ENABLE_SET + ((irq_nr / 32) * 4), readl_relaxed(gic_dist_base + GIC_DIST_ENABLE_SET + ((irq_nr / 32) * 4)));
	//get_cpsr();
	for (i = 0; i < ARRAY_SIZE(arm_interrupt_props); i++) {
		prop_desc = &arm_interrupt_props[i];

		//We are interested only in the SPI Interupts and only that
		//interrupt number that we are interested in
		if ( (prop_desc->intr_num < MIN_SGI_ID) || (prop_desc->intr_num != irq_nr) )
			continue;

		//		writeuartstr("Enabling IRQ no= ");
		//		Write_hex(irq_nr);
		//		writeuartstr("Prop IRQ no= ");
		//		Write_hex(prop_desc->intr_num);
		/* Enable this interrupt */
		gicd_set_isenabler(PLAT_ARM_GICD_BASE, prop_desc->intr_num);
		//	gicd_write_isactiver(PLAT_ARM_GICD_BASE, prop_desc->intr_num);
		nc_printf("Intr enable %x  is %x pri of %x\n" , prop_desc->intr_num , gicd_read_isenabler(PLAT_ARM_GICD_BASE, prop_desc->intr_num),prop_desc->intr_pri);
	}
}

void gicv2_secure_spis_configure_props(uintptr_t gicd_base,
		const interrupt_prop_t *interrupt_props,
		unsigned int interrupt_props_num)
{
	unsigned int i;
	const interrupt_prop_t *prop_desc;
	for (i = 0; i < interrupt_props_num; i++) {
		prop_desc = &interrupt_props[i];

		if (prop_desc->intr_num < MIN_SPI_ID)
			continue;

		/* Configure this interrupt as a secure interrupt */
		//assert(prop_desc->intr_grp == GICV2_INTR_GROUP0);
		gicd_clr_igroupr(gicd_base, prop_desc->intr_num);

		/* Set the priority of this interrupt */
		gicd_set_ipriorityr(gicd_base, prop_desc->intr_num,
				prop_desc->intr_pri);

		/* Target the secure interrupts to primary CPU */
		gicd_set_itargetsr(gicd_base, prop_desc->intr_num,
				gicv2_get_cpuif_id(gicd_base));

		/* Set interrupt configuration */
		gicd_set_icfgr(gicd_base, prop_desc->intr_num,
				prop_desc->intr_cfg);
#if 0 //NOTE Interrupt is enabled in the gic_unmask_irq function and disabled in gic_mask_irq
		/* Enable this interrupt */
		gicd_set_isenabler(gicd_base, prop_desc->intr_num);
#endif
#if !defined (SIMULATION)
//		nc_printf("Interrupt enable for interrupt %x from grp %x is %x\n" , prop_desc->intr_num ,prop_desc->intr_grp, gicd_read_isenabler(gicd_base, prop_desc->intr_num));
#endif
	}
}
void gicv2_secure_ppi_sgi_setup_props(uintptr_t gicd_base,
		const interrupt_prop_t *interrupt_props,
		unsigned int interrupt_props_num)
{
	unsigned int i;
	unsigned int  sec_ppi_sgi_mask = 0;
	const interrupt_prop_t *prop_desc;
	/*
	 * Disable all SGIs (imp. def.)/PPIs before configuring them. This is a
	 * more scalable approach as it avoids clearing the enable bits in the
	 * GICD_CTLR.
	 */
	gicd_write_icenabler(gicd_base, 0, ~0);

	/* Setup the default PPI/SGI priorities doing four at a time */
	for (i = 0; i < MIN_SPI_ID; i += 4)
		gicd_write_ipriorityr(gicd_base, i, GICD_IPRIORITYR_DEF_VAL);

	for (i = 0; i < interrupt_props_num; i++) {
		prop_desc = &interrupt_props[i];

		if (prop_desc->intr_num >= MIN_SPI_ID)
			continue;

		/* Configure this interrupt as a secure interrupt */
		//assert(prop_desc->intr_grp == GICV2_INTR_GROUP0);
		//		nc_printf("Prop desc interrupt number is %0x int priority is %0x \n",prop_desc->intr_num, prop_desc->intr_pri);
		//		nc_printf("Prop desc interrupt group is %0x int cfg is  %0x \n",prop_desc->intr_grp, prop_desc->intr_cfg);

		/*
		 * Set interrupt configuration for PPIs. Configuration for SGIs
		 * are ignored.
		 */
		if ((prop_desc->intr_num >= MIN_PPI_ID) &&
				(prop_desc->intr_num < MIN_SPI_ID)) {
			gicd_set_icfgr(gicd_base, prop_desc->intr_num,
					prop_desc->intr_cfg);
		}
		/* We have an SGI or a PPI. They are Group0 at reset */
		sec_ppi_sgi_mask |= (1u << prop_desc->intr_num);

		/* Set the priority of this interrupt */
		gicd_set_ipriorityr(gicd_base, prop_desc->intr_num,
				prop_desc->intr_pri);
	}
	/*
	 * Invert the bitmask to create a mask for non-secure PPIs and SGIs.
	 * Program the GICD_IGROUPR0 with this bit mask.
	 */
	gicd_write_igroupr(gicd_base, 0, ~sec_ppi_sgi_mask);

	/* Enable the Group 0 SGIs and PPIs */
	gicd_write_isenabler(gicd_base, 0, sec_ppi_sgi_mask);

}

/*******************************************************************************
 * This function sets the GICv2 target mask pattern for the current PE. The PE
 * target mask is used to translate linear PE index (returned by platform core
 * position) to a bit mask used when targeting interrupts to a PE, viz. when
 * raising SGIs and routing SPIs.
 ******************************************************************************/
void gicv2_set_pe_target_mask(unsigned int proc_num)
{
	/* Return if the target mask is already populated */
	if (target_mask_array[proc_num]<10)
		return;

	/*
	 * Update target register corresponding to this CPU and flush for it to
	 * be visible to other CPUs.
	 */
	if (target_mask_array[proc_num] == 1000) {
		target_mask_array[proc_num] =
			gicv2_get_cpuif_id(PLAT_ARM_GICD_BASE);
	}
}

void gicv2_cpuif_enable(void)
{
	unsigned int val;
	/*
	 * Enable the Group 0 interrupts, FIQEn and disable Group 0/1
	 * bypass.
	 */
	//val = CTLR_ENABLE_G0_BIT | FIQ_EN_BIT | FIQ_BYP_DIS_GRP0;
	val = CTLR_ENABLE_G0_BIT | CTLR_ENABLE_G1_BIT;
	//val |= IRQ_BYP_DIS_GRP0 | FIQ_BYP_DIS_GRP1 | IRQ_BYP_DIS_GRP1;
	val |= IRQ_BYP_DIS_GRP0 | IRQ_BYP_DIS_GRP1;

	/* Program the idle priority in the PMR */
	gicc_write_pmr(PLAT_ARM_GICC_BASE, 0xFF); //GIC_PRI_MASK
	gicc_write_ctlr(PLAT_ARM_GICC_BASE, val);
}
void gicv2_distif_init(void){
	unsigned int ctlr;
	unsigned int index, num_ints;

	/* Disable the distributor before going further */
	ctlr = gicd_read_ctlr(PLAT_ARM_GICD_BASE);
	gicd_write_ctlr(PLAT_ARM_GICD_BASE, (ctlr & ~(CTLR_ENABLE_G0_BIT | CTLR_ENABLE_G1_BIT)));


	//num_ints = *(volatile unsigned int *) (PLAT_ARM_GICD_BASE + GICD_CTLR);GICD_TYPER
	/* Get the number of interrupts supported*/
	/* Set the default attribute of all SPIs */
	gicv2_spis_configure_defaults(PLAT_ARM_GICD_BASE);


/*	gicv2_secure_spis_configure_props(PLAT_ARM_GICD_BASE,
			arm_interrupt_props,
			ARRAY_SIZE(arm_interrupt_props));	*/
	//2);
#if 0
#if !ERROR_DEPRECATED
} else {
	/*
	 * Suppress deprecated declaration warnings in compatibility
	 * function
	 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

	assert(driver_data->g0_interrupt_array);

	/* Configure the G0 SPIs */
	gicv2_secure_spis_configure(PLAT_ARM_GICD_BASE,
			driver_data->g0_interrupt_num,
			driver_data->g0_interrupt_array);
#pragma GCC diagnostic pop
}
#endif
#endif

/* Re-enable the secure SPIs now that they have been configured */
gicd_write_ctlr(PLAT_ARM_GICD_BASE, ctlr | CTLR_ENABLE_G0_BIT | CTLR_ENABLE_G1_BIT);
}
void gicv2_pcpu_distif_init(void)
{
	unsigned int ctlr;
/*	gicv2_secure_ppi_sgi_setup_props(PLAT_ARM_GICD_BASE,
			arm_interrupt_props,
			ARRAY_SIZE(arm_interrupt_props));
*/
	/* Enable G0 interrupts if not already */
	ctlr = gicd_read_ctlr(PLAT_ARM_GICD_BASE);
	if ((ctlr & CTLR_ENABLE_G0_BIT) == 0) {
		gicd_write_ctlr(PLAT_ARM_GICD_BASE,
				ctlr | CTLR_ENABLE_G0_BIT);
	}

}

/*
   unsigned int get_cpu_id1(void)
   {
   unsigned int val;
   asm("mrc p15, 0, %0, c0, c0, 5	@ get CR" : "=r" (val) : : "cc");

   return val;
   }*/

void gic_fiq_spurious(void)
{
	nc_printf("\n!!!gic_fiq_spurious Unexpected!!!\n\n");
}
void gic_fiq_handler(void)
{
	unsigned int id;
	unsigned int int_num;
	id = get_cpu_id1();
	nc_printf("Received the interrupt for CPU %d", (id&0x3));
	int_num = gicc_read_iar(PLAT_ARM_GICC_BASE);
	nc_printf("Received int num %d", int_num);

	if(int_num > 1000)
		nc_printf("Spurious Interrupt received %d", int_num);

	gicc_write_eoi(PLAT_ARM_GICC_BASE, int_num);
}

void  gic_handle_irq(void)
{
	//unsigned int  irqstat, irqnr;
	//void *cpu_base = gic_data_cpu_base();
	unsigned int irqstat, irqnr;
	void *cpu_base = PLAT_ARM_GICC_BASE;
#if !defined (SIMULATION)
//	writeuartstr("IN HANDLER\n\r");
#endif
	//irqstat = readl_relaxed(cpu_base + GIC_CPU_INTACK);
	irqstat = gicc_read_iar(PLAT_ARM_GICC_BASE);
	irqnr = irqstat & ~0x1c00;
//	nc_printf("gic_handle_irq gic_handlers[%d] %x\n", irqnr, gic_handlers[irqnr]);
	if ((irqnr > 15 && irqnr < 1022)) {
		/* Handle IRQ Here */
		if(gic_handlers[irqnr] != 0x0) {
			gic_handlers[irqnr]();	
		}
		else
			nc_printf("Unregistered Interrupt %d\n", irqnr);
		gicc_write_eoi(PLAT_ARM_GICC_BASE, irqstat);
	}
	else if (irqnr < 16) {
		//writel_relaxed(irqstat, cpu_base + GIC_CPU_EOI);
		gicc_write_eoi(PLAT_ARM_GICC_BASE, irqstat);
	}
	else{
		nc_printf("Spurious Interrupt %d\n", irqnr);
	}
}

/*
   void timer_handler(void){
   static int count=0;
   nc_printf("Hello Aspeed!%d\n",count);
   }
 */


void gic_init(void)
{
#if 0
	unsigned int j;
	unsigned int regword;
	for (j = 0x40461000 ; j < 0x40465000; j+=4)
	{
		regword = *((volatile unsigned int *) j) ;
		*((volatile unsigned int *) 0x40100200) = regword;
	}
#endif
	gicv2_distif_init();
	gicv2_pcpu_distif_init();
	gicv2_set_pe_target_mask(0);
	gicv2_cpuif_enable();
}

void smc(void){
	asm(".arch_extension sec\n\t");
	__asm__ __volatile__ ("smc #0": : : "memory");
}
void HVC(void){
//	printf("\n\rHVC exe\n\r");
	asm(".arch_extension virt\n\t");
	__asm__ __volatile__ ("hvc #0": : : "memory");
}

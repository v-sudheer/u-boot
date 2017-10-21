/*
 * ast_gpio.c - GPIO driver for the Aspeed SoC
 *
 * Copyright (C) ASPEED Technology Inc.
 * Ryan Chen <ryan_chen@aspeedtech.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <common.h>
#include <asm/io.h>
#include <asm/gpio.h>
//#include <plat/regs-gpio.h>

//#define AST_GPIO_DEBUG

#ifdef AST_GPIO_DEBUG
#define GPIODBUG(fmt, args...) printf(fmt, ## args)
#else
#define GPIODBUG(fmt, args...)
#endif

u32 ast_gpio_base = AST_GPIO_BASE;

struct ast_gpio_bank {
//TODO remove base	
	u32  	index;	
	u32		data_offset;
	u32		dir_offset;
	u32		int_en_offset;	
	u32		int_type_offset;		
	u32		int_sts_offset;	
	u32		rst_tol_offset;		
	u32		debounce_offset;	
	u32		cmd_source_offset;		
};

static inline u32
ast_gpio_read(struct ast_gpio_bank *ast_gpio ,u32 offset)
{
	GPIODBUG("base = 0x%08x, offset = 0x%08x \n", ast_gpio_base, offset);

	return readl(ast_gpio_base + offset);
}

static inline void
ast_gpio_write(struct ast_gpio_bank *ast_gpio , u32 val, u32 offset)
{
    GPIODBUG("base = 0x%08x, offset = 0x%08x, val = 0x%08x\n", ast_gpio_base, offset, val);
    writel(val, ast_gpio_base + offset);
}

static int
ast_gpio_direction_input(struct ast_gpio_bank *ast_gpio, unsigned offset)
{
	u32 v;
	int ret = -1;

	v = ast_gpio_read(ast_gpio, ast_gpio->dir_offset);
	
	v &= ~(GPIO_OUTPUT_MODE << (offset + (ast_gpio->index * 8)));
	ast_gpio_write(ast_gpio, v, ast_gpio->dir_offset);

	ret = 0;

	return ret;
}

static int
ast_gpio_direction_output(struct ast_gpio_bank *ast_gpio, unsigned offset, int val)
{
    u32 v;
    int ret = -1;

	/* Drive as an output */
	v = ast_gpio_read(ast_gpio, ast_gpio->dir_offset);

	v |= (GPIO_OUTPUT_MODE << (offset + (ast_gpio->index * 8)));

	ast_gpio_write(ast_gpio, v, ast_gpio->dir_offset);

	ret = 0;	
	return ret;
}

#define AST_GPIO_BANK(name, index_no, data, dir, int_en, int_type, int_sts, rst_tol, debounce, cmd_s)	\
	{												\
		.index = index_no,							\
		.data_offset = data,						\
		.dir_offset = dir, 							\
		.int_en_offset = int_en,					\
		.int_type_offset = int_type,				\
		.int_sts_offset = int_sts, 					\
		.rst_tol_offset = rst_tol,					\
		.debounce_offset = debounce,				\
		.cmd_source_offset = cmd_s,					\
	}

static struct ast_gpio_bank ast_gpio_gp[] = {
	AST_GPIO_BANK("GPIOA", 0, 0x000, 0x004, 0x008, 0x00c, 0x018, 0x01c, 0x040, 0x060),
	AST_GPIO_BANK("GPIOB", 1, 0x000, 0x004, 0x008, 0x00c, 0x018, 0x01c, 0x040, 0x060),
	AST_GPIO_BANK("GPIOC", 2, 0x000, 0x004, 0x008, 0x00c, 0x018, 0x01c, 0x040, 0x060),
	AST_GPIO_BANK("GPIOD", 3, 0x000, 0x004, 0x008, 0x00c, 0x018, 0x01c, 0x040, 0x060),
	AST_GPIO_BANK("GPIOE", 0, 0x020, 0x024, 0x028, 0x02c, 0x038, 0x03c, 0x048, 0x068),
	AST_GPIO_BANK("GPIOF", 1, 0x020, 0x024, 0x028, 0x02c, 0x038, 0x03c, 0x048, 0x068),
	AST_GPIO_BANK("GPIOG", 2, 0x020, 0x024, 0x028, 0x02c, 0x038, 0x03c, 0x048, 0x068),
	AST_GPIO_BANK("GPIOH", 3, 0x020, 0x024, 0x028, 0x02c, 0x038, 0x03c, 0x048, 0x068),
	AST_GPIO_BANK("GPIOI", 0, 0x070, 0x074, 0x098, 0x09c, 0x0a8, 0x0ac, 0x0b0, 0x090),
	AST_GPIO_BANK("GPIOJ", 1, 0x070, 0x074, 0x098, 0x09c, 0x0a8, 0x0ac, 0x0b0, 0x090),
	AST_GPIO_BANK("GPIOK", 2, 0x070, 0x074, 0x098, 0x09c, 0x0a8, 0x0ac, 0x0b0, 0x090),
	AST_GPIO_BANK("GPIOL", 3, 0x070, 0x074, 0x098, 0x09c, 0x0a8, 0x0ac, 0x0b0, 0x090),	
	AST_GPIO_BANK("GPIOM", 0, 0x078, 0x07c, 0x0e8, 0x0ec, 0x0f8, 0x0fc, 0x100, 0x0e0),	
	AST_GPIO_BANK("GPION", 1, 0x078, 0x07c, 0x0e8, 0x0ec, 0x0f8, 0x0fc, 0x100, 0x0e0),
	AST_GPIO_BANK("GPIOO", 2, 0x078, 0x07c, 0x0e8, 0x0ec, 0x0f8, 0x0fc, 0x100, 0x0e0),	
	AST_GPIO_BANK("GPIOP", 3, 0x078, 0x07c, 0x0e8, 0x0ec, 0x0f8, 0x0fc, 0x100, 0x0e0),
	AST_GPIO_BANK("GPIOQ", 0, 0x080, 0x084, 0x118, 0x11c, 0x128, 0x12c, 0x130, 0x110),
	AST_GPIO_BANK("GPIOR", 1, 0x080, 0x084, 0x118, 0x11c, 0x128, 0x12c, 0x130, 0x110),	
	AST_GPIO_BANK("GPIOS", 2, 0x080, 0x084, 0x118, 0x11c, 0x128, 0x12c, 0x130, 0x110),
#if defined(AST_SOC_G4)
	AST_GPIO_BANK("GPIOT", 4, 0x080, 0x084, 0x118, 0x11c, 0x128, 0x12c, 0x130, 0x110),	
	AST_GPIO_BANK("GPIOU", 0, 0x088, 0x08c, 0x148, 0x14c, 0x158, 0x15c, 0x160, 0x140),
	AST_GPIO_BANK("GPIOV", 1, 0x088, 0x08c, 0x148, 0x14c, 0x158, 0x15c, 0x160, 0x140),
	AST_GPIO_BANK("GPIOW", 2, 0x088, 0x08c, 0x148, 0x14c, 0x158, 0x15c, 0x160, 0x140),
	AST_GPIO_BANK("GPIOX", 3, 0x088, 0x08c, 0x148, 0x14c, 0x158, 0x15c, 0x160, 0x140),
	AST_GPIO_BANK("GPIOY", 0, 0x1e0, 0x1e4, 0x178, 0x17c, 0x188, 0x18c, 0x190, 0x170),
	AST_GPIO_BANK("GPIOZ", 1, 0x1e0, 0x1e4, 0x178, 0x17c, 0x188, 0x18c, 0x190, 0x170),
	AST_GPIO_BANK("GPIOAA", 2, 0x1e0, 0x1e4, 0x178, 0x17c, 0x188, 0x18c, 0x190, 0x170),
	AST_GPIO_BANK("GPIOAB", 3, 0x1e0, 0x1e4, 0x178, 0x17c, 0x188, 0x18c, 0x190, 0x170),	
#elif defined(AST_SOC_G5)
	AST_GPIO_BANK("GPIOT", 4, 0x080, 0x084, 0x118, 0x11c, 0x128, 0x12c, 0x130, 0x110),	
	AST_GPIO_BANK("GPIOU", 0, 0x088, 0x08c, 0x148, 0x14c, 0x158, 0x15c, 0x160, 0x140),
	AST_GPIO_BANK("GPIOV", 1, 0x088, 0x08c, 0x148, 0x14c, 0x158, 0x15c, 0x160, 0x140),
	AST_GPIO_BANK("GPIOW", 2, 0x088, 0x08c, 0x148, 0x14c, 0x158, 0x15c, 0x160, 0x140),
	AST_GPIO_BANK("GPIOX", 3, 0x088, 0x08c, 0x148, 0x14c, 0x158, 0x15c, 0x160, 0x140),
	AST_GPIO_BANK("GPIOY", 0, 0x1e0, 0x1e4, 0x178, 0x17c, 0x188, 0x18c, 0x190, 0x170),
	AST_GPIO_BANK("GPIOZ", 1, 0x1e0, 0x1e4, 0x178, 0x17c, 0x188, 0x18c, 0x190, 0x170),
	AST_GPIO_BANK("GPIOAA", 2, 0x1e0, 0x1e4, 0x178, 0x17c, 0x188, 0x18c, 0x190, 0x170),
	AST_GPIO_BANK("GPIOAB", 3, 0x1e0, 0x1e4, 0x178, 0x17c, 0x188, 0x18c, 0x190, 0x170), 
	//TODO ....
	AST_GPIO_BANK("GPIOAC", 3, 0x1e0, 0x1e4, 0x178, 0x17c, 0x188, 0x18c, 0x190, 0x170), 	
#endif	
};

int ast_set_gpio_value(unsigned gpio_pin, int value)
{
	u32 data;
	u32 gp, pin;
	gp = gpio_pin / GPIO_PER_PORT_PIN_NUM;
	pin = gpio_pin % 32;
	data = ast_gpio_read(&ast_gpio_gp[gp], ast_gpio_gp[gp].data_offset);
	if(value)
		data |= (1 << pin);
	else
		data &= ~(1 << pin);
	
	GPIODBUG("gp : %d, pin %d, data = %x \n ", gp, pin, data);
	ast_gpio_write(&ast_gpio_gp[gp], data, ast_gpio_gp[gp].data_offset);

	return 0;
}

int ast_get_gpio_value(unsigned gpio_pin)
{
	u32 data;
	u32 gp, pin;
	gp = gpio_pin / GPIO_PER_PORT_PIN_NUM;
	pin = gpio_pin % 32;
	data = ast_gpio_read(&ast_gpio_gp[gp], ast_gpio_gp[gp].data_offset);

	GPIODBUG("gp : %d, pin %d, data = %x, value = %d \n ", gp, pin, data, (data >> pin) & 1);

	return ((data >> pin) & 1);
}

/* Common GPIO API */
int gpio_request(unsigned gpio, const char *label)
{
	return 0;
}

int gpio_free(unsigned gpio)
{
	return 0;
}

int gpio_direction_input(unsigned gpio)
{
 	struct ast_gpio_bank *gpio_band = &ast_gpio_gp[gpio/8];

	ast_gpio_direction_input(gpio_band,
				 (gpio%8));
	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
 	struct ast_gpio_bank *gpio_band = &ast_gpio_gp[gpio/8];

	ast_gpio_direction_output(gpio_band,
				 (gpio%8), value);
	return 0;
}

int gpio_get_value(unsigned gpio)
{
	return ast_get_gpio_value(gpio);
}

int gpio_set_value(unsigned gpio, int value)
{
	return ast_set_gpio_value(gpio, value);
}


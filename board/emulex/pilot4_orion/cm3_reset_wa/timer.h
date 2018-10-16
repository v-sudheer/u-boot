/*******************************************************************************
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


# ifndef	__PILOT4_TIMER_H__
# define	__PILOT4_TIMER_H__

# define ARMV7M_NVIC_BASE					0xe000e000

# define NVIC_SYSH12_15_PRIORITY_OFFSET	0x0d20 /* System handler 12-15 priority register */
# define NVIC_SYSTICK_CTRL_OFFSET		0x0010 /* SysTick control and status register */
# define NVIC_SYSTICK_RELOAD_OFFSET		0x0014 /* SysTick reload value register */

# define NVIC_SYSH_PRIORITY_PR15_SHIFT	24
# define NVIC_SYSH_PRIORITY_PR15_MASK	(0xff << NVIC_SYSH_PRIORITY_PR15_SHIFT)

# define NVIC_SYSTICK_CTRL_ENABLE		(1 << 0)  /* Bit 0:  Enable */
# define NVIC_SYSTICK_CTRL_TICKINT		(1 << 1)  /* Bit 1:  Tick interrupt */
# define NVIC_SYSTICK_CTRL_CLKSOURCE	(1 << 2)  /* Bit 2:  Clock source */
# define NVIC_SYSTICK_CTRL_COUNTFLAG	(1 << 16) /* Bit 16: Count Flag */

# define NVIC_SYSH12_15_PRIORITY		(ARMV7M_NVIC_BASE + NVIC_SYSH12_15_PRIORITY_OFFSET)
# define NVIC_SYSTICK_CTRL				(ARMV7M_NVIC_BASE + NVIC_SYSTICK_CTRL_OFFSET)
# define NVIC_SYSTICK_RELOAD			(ARMV7M_NVIC_BASE + NVIC_SYSTICK_RELOAD_OFFSET)

# define NVIC_SYSH_PRIORITY_DEFAULT		0x80 /* Midpoint is the default */

# define SYSTICK_RELOAD  				140

# endif

/*
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __ASPEED_MDIO_H__
#define __ASPEED_MDIO_H__

#include <net.h>
#include <miiphy.h>

extern int aspeed_mdio_init(const char *name, ulong ioaddr);

#endif /* __ASPEED_MDIO_H__ */

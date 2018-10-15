// SPDX-License-Identifier: GPL-2.0
/*
 * NC-SI (Network Controller Sideband Interface)
 *
 * Copyright (C) 2018 IBM Corp.
 */

#ifndef __NCSI_H__
#define __NCSI_H____

#include <common.h>

void ncsi_init(void);
void ncsi_start(void);
void ncsi_stop(void);
void ncsi_receive(struct ethernet_hdr *et, struct ip_udp_hdr *ip, int len);

#endif /* __NCSI_H__ */

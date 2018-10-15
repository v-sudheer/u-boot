// SPDX-License-Identifier: GPL-2.0
/*
 * NC-SI (Network Controller Sideband Interface)
 *
 * Copyright (C) 2016-2018 IBM Corp.
 */


#include <common.h>
#include "ncsi.h"
#include "ncsi-pkt.h"

/*
 * packet format : ncsi header + payload + checksum
 */
#define NCSI_MAX_PAYLOAD 172
#define NCSI_HDR_SIZE    sizeof(struct ncsi_pkt_hdr)
#define NCSI_MAX_LEN     (NCSI_HDR_SIZE + NCSI_MAX_PAYLOAD + 4)

static int	ncsi_req_cmd;
static uchar	ncsi_req_id;

static uchar   *ncsi_tx_packet;	/* THE NCSI transmit packet */
static uchar	ncsi_tx_packet_buf[PKTSIZE_ALIGN + PKTALIGN];

void ncsi_init(void)
{
	ncsi_req_cmd = 0;
	ncsi_req_id = 0;
	ncsi_tx_packet = &ncsi_tx_packet_buf[0] + (PKTALIGN - 1);
	ncsi_tx_packet -= (ulong)ncsi_tx_packet % PKTALIGN;
}

static void ncsi_timeout_handler(void)
{
	eth_halt();
	net_set_state(NETLOOP_FAIL);	/* we did not get the reply */
}

static void ncsi_request(u8 channel, u8 cmd, uchar* payload, int len)
{
	uchar *pkt;
	struct ncsi_pkt_hdr *nh;
	int eth_hdr_size;

	ncsi_req_id++;
	ncsi_req_cmd = cmd;

	printf("New NCSI request id=%d cmd=%d\n", ncsi_req_id, cmd);

	pkt = ncsi_tx_packet;

	eth_hdr_size = net_set_ether(pkt, net_bcast_ethaddr, PROT_NCSI);
	pkt += eth_hdr_size;

	nh = (struct ncsi_pkt_hdr *)pkt;

	nh->mc_id = 0x0; /* Management Controller ID */
	nh->revision = 0x1;
	nh->id = ncsi_req_id;
	nh->type = cmd;
	nh->channel = (channel << 5) | 0x1f;
	nh->length = len;
	pkt += NCSI_HDR_SIZE;

	if (payload && len)
		memcpy(pkt, payload, len);

	net_set_timeout_handler(1000UL, ncsi_timeout_handler);

	net_send_packet(ncsi_tx_packet, eth_hdr_size + NCSI_HDR_SIZE + len + 4);
}

static void ncsi_request_clear_initial_state(u8 channel)
{
	ncsi_request(channel, NCSI_PKT_CMD_CIS, NULL, 0);
}

static void ncsi_request_deselect_package(u8 channel)
{
	ncsi_request(channel, NCSI_PKT_CMD_DP, NULL, 0);
}

static void ncsi_request_select_package(u8 channel)
{
	uchar payload[4];

	payload[0] = 0x0;
	payload[1] = 0x0;
	payload[2] = 0x0;
	payload[3] = 0xF; /* HW Arbitration Disable */

	ncsi_request(channel, NCSI_PKT_CMD_SP, payload, sizeof(payload));
}

static void ncsi_request_get_version_id(u8 channel)
{
	ncsi_request(channel, NCSI_PKT_CMD_GVI, NULL, 0);
}

static void ncsi_request_get_capabilities(u8 channel)
{
	ncsi_request(channel, NCSI_PKT_CMD_GC, NULL, 0);
}

static void ncsi_request_set_mac_address(u8 channel)
{
	uchar payload[8];

	payload[0] = net_ethaddr[0];
	payload[1] = net_ethaddr[1];
	payload[2] = net_ethaddr[2];
	payload[3] = net_ethaddr[3];
	payload[4] = net_ethaddr[4];
	payload[5] = net_ethaddr[5];
	payload[6] = 0x1;  /* MAC address filter number 1 */
	payload[7] = (0x0 << 5) | 0x1; /* Unicast + Enable MAC address filter */

	ncsi_request(channel, NCSI_PKT_CMD_SMA, payload, sizeof(payload));
}

static void ncsi_request_enable_broadcast_filter(u8 channel)
{
	uchar payload[4];

	payload[0] = 0x0;
	payload[1] = 0x0;
	payload[2] = 0x0;
	payload[3] = 0xF; /* ARP, DHCP, NetBIOS */

	ncsi_request(channel, NCSI_PKT_CMD_EBF, payload, sizeof(payload));
}

static void ncsi_request_disable_channel_tx(u8 channel)
{
	ncsi_request(channel, NCSI_PKT_CMD_DCNT, NULL, 0);
}

static void ncsi_request_enable_channel_tx(u8 channel)
{
	ncsi_request(channel, NCSI_PKT_CMD_ECNT, NULL, 0);
}

static void ncsi_request_get_link_status(u8 channel)
{
	ncsi_request(channel, NCSI_PKT_CMD_GLS, NULL, 0);
}

static void ncsi_request_enable_channel(u8 channel)
{
	ncsi_request(channel, NCSI_PKT_CMD_EC, NULL, 0);
}

static void ncsi_request_disable_channel(u8 channel)
{
	ncsi_request(channel, NCSI_PKT_CMD_DC, NULL, 0);
}

void ncsi_start(void)
{
	/* TODO: loop on all packages */
	ncsi_request_deselect_package(0);
}

void ncsi_stop(void)
{
	;
}

static void ncsi_handle_link_status(struct ncsi_rsp_pkt_hdr *nrh)
{
	static int retry = 10;

	struct ncsi_rsp_gls_pkt *gls = (struct ncsi_rsp_gls_pkt *)nrh;
	u32 status = ntohl(gls->status);

	printf("NCSI: link status %x\n", status);
	if (!status && retry--) {
		ncsi_request_get_link_status(nrh->common.channel);
	}

	/* TODO handle link error */
}

void ncsi_receive(struct ethernet_hdr *et, struct ip_udp_hdr *ip, int len)
{
	struct ncsi_rsp_pkt_hdr *nrh;
	u8 channel = 0;

	nrh = (struct ncsi_rsp_pkt_hdr *)ip;

	if (len < NCSI_HDR_SIZE) {
		printf("NCSI: bad length %d < %d\n", len, NCSI_HDR_SIZE);
		return;
	}

	debug("NCSI: mc_id=%d id=%d type=%d channel=%d code=%d reason=%d\n",
	       nrh->common.mc_id, nrh->common.id, nrh->common.type,
	       nrh->common.channel, nrh->code, nrh->reason);

	if (nrh->common.type != (ncsi_req_cmd | 0x80) ||
	    nrh->common.id != ncsi_req_id) {
		printf("NCSI: unexpected response to command: %d !?\n",
		       ncsi_req_cmd);
		return;
	}

	if (nrh->code != NCSI_PKT_RSP_C_COMPLETED) {
		printf("NCSI: command %d failed : code=%d reason=%d\n",
		       ncsi_req_cmd, nrh->code, nrh->reason);
		return;
	}

	switch (nrh->common.type) {
	case NCSI_PKT_RSP_DP:
		/* TODO: loop on all packages */
		ncsi_request_select_package(channel);
		break;
	case NCSI_PKT_RSP_SP:
		/* TODO: loop on all packages to select one */
		ncsi_request_clear_initial_state(channel);
		break;
	case NCSI_PKT_RSP_CIS:
		ncsi_request_set_mac_address(channel);
		break;
	case NCSI_PKT_RSP_SMA:
		ncsi_request_enable_broadcast_filter(channel);
		break;
	case NCSI_PKT_RSP_EBF:
		ncsi_request_enable_channel_tx(channel);
		break;
	case NCSI_PKT_RSP_ECNT:
		ncsi_request_enable_channel(channel);
		break;
	case NCSI_PKT_RSP_EC:
		ncsi_request_get_link_status(channel);
		break;
	case NCSI_PKT_RSP_GLS:
		ncsi_handle_link_status(nrh);
		break;
	default:
		printf("NCSI: unexpected response to command: %d !?\n",
		       nrh->common.type);
		break;
	}
}

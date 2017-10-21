/*
 * ast_udc.c - UDC driver for the Aspeed SoC
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
#include <config.h>
#include <asm/byteorder.h>
#include <usbdevice.h>
#include <asm/io.h>


#include "ep0.h"


#define MIN(x,y) (x < y ? x : y)

/* number of endpoints on this UDC */
#define UDC_MAX_ENDPOINTS	24

static struct urb *ep0_urb;
static struct usb_device_instance *udc_device;
//static int ep0state = EP0_IDLE;

/*************************************************************************************/
#define AST_VHUB_CTRL 				0x00
#define AST_VHUB_CONF 				0x04
#define AST_VHUB_IER 				0x08
#define AST_VHUB_ISR 				0x0C
#define AST_VHUB_EP_ACK_IER 		0x10
#define AST_VHUB_EP_NAK_IER 		0x14
#define AST_VHUB_EP_ACK_ISR 		0x18
#define AST_VHUB_EP_NAK_ISR 		0x1C
#define AST_VHUB_DEV_RESET 			0x20
#define AST_VHUB_USB_STS 			0x24
#define AST_VHUB_EP_DATA 			0x28
#define AST_VHUB_ISO_TX_FAIL		0x2C
#define AST_VHUB_EP0_CTRL			0x30
#define AST_VHUB_EP0_DATA_BUFF		0x34
#define AST_VHUB_EP1_CTRL			0x38
#define AST_VHUB_EP1_STS_CHG		0x3C

#define AST_VHUB_SETUP_DATA0		0x80
#define AST_VHUB_SETUP_DATA1		0x84

/*  ************************************************************************************/
/* AST_VHUB_CTRL 		0x00 */
#define ROOT_PHY_CLK_EN				(1 << 31)
#define ROOT_PHY_SELF_TEST_EN			(1 << 25)
#define ROOT_DN_15K_EN					(1 << 24)
#define ROOT_DP_15K_EN					(1 << 23)
#define ROOT_FIFO_DYNP_EN				(1 << 19)
#define ROOT_EP_LONG_DESC				(1 << 18)
#define ROOT_ISO_IN_NULL_RESP			(1 << 17)
#define ROOT_SPLIT_IN					(1 << 16)
#define ROOT_LOOP_TEST_PASS			(1 << 15)
#define ROOT_LOOP_TEST_FINISH			(1 << 14)
#define ROOT_BIST_TEST_PASS				(1 << 13)
#define ROOT_BIST_ON					(1 << 12)
#define ROOT_PHY_RESET_DIS				(1 << 11)
#define ROOT_TEST_MODE(x)				((x) << 8)
#define ROOT_FORCE_TIMER_HS			(1 << 7)
#define ROOT_FORCE_HS					(1 << 6)
#define ROOT_REMOTE_WAKEUP_12MS		(1 << 5)
#define ROOT_REMOTE_WAKEUP_EN			(1 << 4)
#define ROOT_AUTO_REMOTE_WAKEUP_EN	(1 << 3)
#define ROOT_STOP_CLK_IN_SUPEND		(1 << 2)
#define ROOT_UPSTREAM_FS				(1 << 1)
#define ROOT_UPSTREAM_EN				(1)

/* AST_VHUB_CONF 		0x04 */
#define CONF_GET_DMA_STS(x)				((x >> 16) & 0xff)
#define CONF_GET_DEV_ADDR(x)			(x & 0x7f)

/* AST_VHUB_IER 		0x08	*/
#define ISR_USB_CMD_DEADLOCK			(1 << 18)

#define ISR_EP_NAK						(1 << 17)
#define ISR_EP_ACK_STALL				(1 << 16)
#define ISR_DEVICE5						(1 << 13)
#define ISR_DEVICE4						(1 << 12)
#define ISR_DEVICE3						(1 << 11)
#define ISR_DEVICE2						(1 << 10)
#define ISR_DEVICE1						(1 << 9)
#define ISR_SUSPEND_RESUME				(1 << 8)
#define ISR_BUS_SUSPEND 				(1 << 7)
#define ISR_BUS_RESET 					(1 << 6)
#define ISR_HUB_EP1_IN_DATA_ACK		(1 << 5)
#define ISR_HUB_EP0_IN_DATA_NAK		(1 << 4)
#define ISR_HUB_EP0_IN_ACK_STALL		(1 << 3)
#define ISR_HUB_EP0_OUT_NAK			(1 << 2)
#define ISR_HUB_EP0_OUT_ACK_STALL		(1 << 1)
#define ISR_HUB_EP0_SETUP				(1)

/* AST_VHUB_EP_ACK_IER 		0x10 */
#define EP14_ISR							(1 << 14)
#define EP13_ISR							(1 << 13)
#define EP12_ISR							(1 << 12)
#define EP11_ISR							(1 << 11)
#define EP10_ISR							(1 << 10)
#define EP9_ISR							(1 << 9)
#define EP8_ISR							(1 << 8)
#define EP7_ISR							(1 << 7)
#define EP6_ISR							(1 << 6)
#define EP5_ISR							(1 << 5)
#define EP4_ISR							(1 << 4)
#define EP3_ISR							(1 << 3)
#define EP2_ISR							(1 << 2)
#define EP1_ISR							(1 << 1)
#define EP0_ISR							(1)

/* AST_VHUB_DEV_RESET_ISR 		0x20 */
#define DEV5_SOFT_RESET					(1 << 5)
#define DEV4_SOFT_RESET					(1 << 4)
#define DEV3_SOFT_RESET					(1 << 3)
#define DEV2_SOFT_RESET					(1 << 2)
#define DEV1_SOFT_RESET					(1 << 1)
#define ROOT_HUB_SOFT_RESET			(1)


/* AST_VHUB_EP0_CTRL			0x30 */
#define EP0_GET_RX_LEN(x)				((x >> 16) & 0x7f)
#define EP0_TX_LEN(x)						((x & 0x7f) << 8)
#define EP0_RX_BUFF_RDY					(1 << 2)
#define EP0_TX_BUFF_RDY					(1 << 1)
#define EP0_STALL						(1)

#define AST_UDC_DEV_CTRL 				0x00
#define AST_UDC_DEV_ISR				0x04
#define AST_UDC_DEV_EP0_CTRL			0x08
#define AST_UDC_DEV_EP0_DATA_BUFF	0x0C

/*  ************************************************************************************/
//#define AST_UDC_DEV_CTRL 				0x00
#define DEV_CTRL_DEV_ADDR_MASK		(0x3f << 8)
#define DEV_CTRL_IN_NAK_EN				(1 << 6)
#define DEV_CTRL_IN_ACK_STALL_EN		(1 << 5)
#define DEV_CTRL_OUT_NAK_EN			(1 << 4)
#define DEV_CTRL_OUT_ACK_STALL_EN		(1 << 3)
#define DEV_CTRL_SETUP_EN				(1 << 2)
#define DEV_CTRL_HIGH_SPEED_MODE		(1 << 1)
#define DEV_CTRL_DEV_EN				(1)

//define AST_UDC_DEV_ISR					0x04
#define DEV_CTRL_IN_NAK_INT			(1 << 4)
#define DEV_CTRL_IN_ACK_STALL_INT		(1 << 3)
#define DEV_CTRL_OUT_NAK_INT			(1 << 2)
#define DEV_CTRL_OUT_ACK_STALL_INT	(1 << 1)
#define DEV_CTRL_SETUP_INT				(1)

//#define AST_UDC_DEV_EP0_CTRL			0x08
#define DEV_EP0_GET_RX_SIZE(x)			((x >> 16) & 0x7f)
#define DEV_EP0_TX_SIZE_MASK			(0x7f << 8)
#define DEV_EP0_SET_TX_SIZE(x)			((x & 0x7f) << 8)
#define DEV_EP0_RX_BUFF_RDY			(1 << 2)
#define DEV_EP0_TX_BUFF_RDY			(1 << 1)
#define DEV_EP0_STALL					(1)

/*************************************************************************************/
#define AST_EP_CONFIG					0x00
#define AST_EP_DMA_CTRL				0x04
#define AST_EP_DMA_BUFF				0x08
#define AST_EP_DMA_STS					0x0C

/*************************************************************************************/
//#define AST_EP_CONFIG					0x00
#define EP_SET_MAX_PKT(x)				((x & 0x3ff) << 16)

#define EP_AUTO_DATA_DISABLE			(0x1 << 13)
#define EP_SET_EP_STALL					(0x1 << 12)

#define EP_SET_EP_NUM(x)				((x & 0xf) << 8)

#define EP_SET_TYPE_MASK(x)				((x) << 4)
#define EP_TYPE_BULK_IN					(0x2 << 4)
#define EP_TYPE_BULK_OUT				(0x3 << 4)
#define EP_TYPE_INT_IN					(0x4 << 4)
#define EP_TYPE_INT_OUT					(0x5 << 4)
#define EP_TYPE_ISO_IN					(0x6 << 4)
#define EP_TYPE_ISO_OUT					(0x7 << 4)

#define EP_ALLOCATED_MASK				(0x7 << 1)

#define EP_ENABLE						(1)

//#define AST_EP_DMA_STS					0x0C


//#define AST_EP_DMA_CTRL				0x04

#define EP_SINGLE_DMA_MODE			(0x1 << 1)

/*************************************************************************************/
#ifdef AST_VHUB_BASE
u32 udc_reg = AST_VHUB_BASE;
#else
u32 udc_reg = AST_UDC_BASE;
#endif

/*-------------------------------------------------------------------------*/
#define ast_udc_read(offset) \
	__raw_readl(udc_reg + (offset))
#define ast_udc_write(val, offset) \
	__raw_writel((val), udc_reg + (offset))

#define ast_ep_read(ep_reg, reg) \
	__raw_readl(ep_reg + (reg))
#define ast_ep_write(ep_reg, val, reg) \
	__raw_writel((val), ep_reg + (reg))
/*-------------------------------------------------------------------------*/


/*
 * If the endpoint has an active tx_urb, then the next packet of data from the
 * URB is written to the tx FIFO.
 * The total amount of data in the urb is given by urb->actual_length.
 * The maximum amount of data that can be sent in any one packet is given by
 * endpoint->tx_packetSize.
 * The number of data bytes from this URB that have already been transmitted
 * is given by endpoint->sent.
 * endpoint->last is updated by this routine with the number of data bytes
 * transmitted in this packet.
 */
int is_usbd_high_speed(void)
{
	if(ast_udc_read(AST_VHUB_USB_STS) & (0x1 << 27))
		return 1;
	else
		return 0;
}

/* Stall endpoint */
static void udc_stall_ep(u32 ep_num)
{
	u32 ep_reg;
	usbdbg("udc_stall_ep %d ~~~~~ \n", ep_num);

	if(ep_num) {
		ep_reg = udc_reg + 0x200 + (0x10 * (ep_num - 1));
		ast_ep_write(ep_reg, ast_ep_read(ep_reg, AST_EP_CONFIG) | EP_SET_EP_STALL, AST_EP_CONFIG);
	} else {
		ast_udc_write(ast_udc_read(AST_VHUB_EP0_CTRL) | EP0_STALL, AST_VHUB_EP0_CTRL);		
	}
}

int udc_endpoint_write(struct usb_endpoint_instance *endpoint)
{
	struct urb *urb = endpoint->tx_urb;
	int ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	unsigned int remaining_packet = urb->actual_length -endpoint->sent;
	u32 ep_reg = udc_reg + 0x200 + (0x10 * (ep_num - 1));
	u32 ep_isr;
	u32 length;
	u8 *data;

	if (endpoint->tx_packetSize < remaining_packet)
		length = endpoint->tx_packetSize;
	else
		length = remaining_packet;

//	printf("udc_endpoint_write [%d] %d %d , %d \n", ep_num, length, endpoint->sent, urb->actual_length);
	
	data = (u8 *) urb->buffer;
	data += endpoint->sent;
	
	//tx trigger 
	ast_ep_write(ep_reg, data, AST_EP_DMA_BUFF);
	ast_ep_write(ep_reg, length << 16, AST_EP_DMA_STS);
	ast_ep_write(ep_reg, length << 16 | 0x1, AST_EP_DMA_STS);		
	
	endpoint->last = length;

	/* usbd_tx_complete will take care of updating 'sent' */
	while(1) {
		ep_isr = ast_udc_read(AST_VHUB_EP_ACK_ISR);
		if(ep_isr & (0x1 << (ep_num - 1))) {
			ast_udc_write(0x1 <<  (ep_num - 1), AST_VHUB_EP_ACK_ISR); 
			break;
		}
		
	}

	usbd_tx_complete(endpoint);

	return 0;
}

void ast_udc_ep_handle(struct usb_endpoint_instance *endpoint)
{
	int nbytes = 0;
	u32 ep_reg;
	int ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	int ep_isout = (endpoint->endpoint_address & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT;

	ep_reg = udc_reg + 0x200 + (0x10 * (ep_num - 1));

	if(ep_isout) {
		nbytes = (ast_ep_read(ep_reg , AST_EP_DMA_STS) >> 16) & 0x7ff;
//		printf("[%d] rx %d bytes \n", ep_num, nbytes);
		usbd_rcv_complete(endpoint, nbytes, 0);
		//trigger next 
		ast_ep_write(ep_reg, (endpoint->rcv_urb->buffer_length << 16) | 0x1, AST_EP_DMA_STS);
	} else {
		printf("handle error !!!! \n");
		*((volatile ulong*) 0x1e720000) = 0xbeefdead;
		//printf("ast_udc_ep_handle %x, %x, %x ~~~~~~~~~~~~~~~~~ TODO \n", ep_addr, ep_num, ep_isout);
	}
		
}

void ast_udc_ep0_rx(struct usb_endpoint_instance *endpoint)
{
	int nbytes = 0;
	struct urb *urb;

	if (endpoint) {
		urb = endpoint->rcv_urb;

		if (urb) {
			u8 *cp = urb->buffer;
			if(((u32)cp % 64) != 0) printf("error aligned %x\n", cp);
			ast_udc_write(cp, AST_VHUB_EP0_DATA_BUFF);
			//trigger
			ast_udc_write(EP0_RX_BUFF_RDY, AST_VHUB_EP0_CTRL); 			
		}
	}
}

void ast_udc_ep0_out(struct usb_endpoint_instance *endpoint)
{
	u16 rx_len = EP0_GET_RX_LEN(ast_udc_read(AST_VHUB_EP0_CTRL));

	usbdbg("");
	
	/* Check direction */
	if ((ep0_urb->device_request.bmRequestType & USB_REQ_DIRECTION_MASK) == USB_REQ_HOST2DEVICE) {
		if(rx_len !=0) 	usberr("Stalling unexpected EP0 control write data stage packet");
	} else {
	      if(rx_len !=0) 	
		usbdbg("ACK on EP0 control read status stage packet TODO ~~~~~");
//		u32 len = (readl(&outep_regs_p[0].endp_status) >> 11) & 0xfff;
//		usbgetpckfromfifo(0, dummy, len);
	}


}

void ast_udc_ep0_tx(struct usb_endpoint_instance *endpoint)
{
	struct urb *urb = endpoint->tx_urb;
	usbdbg("");
	int align;

	if (urb) {
		u32 last;

		usbdbg("urb->buffer %p, buffer_length %d, actual_length %d sent %d ",
			urb->buffer, urb->buffer_length, urb->actual_length, endpoint->sent);

		last = MIN(urb->actual_length - endpoint->sent,
			   endpoint->tx_packetSize);

		if (last) {
			u8 *cp = urb->buffer + endpoint->sent;
			
			usbdbg("send address %x ", cp);
			
			/*
			 * This ensures that USBD packet fifo is accessed
			 * - through word aligned pointer or
			 * - through non word aligned pointer but only
			 *   with a max length to make the next packet
			 *   word aligned
			 */

			align = ((ulong)cp % sizeof(int));
			if (align)
				last = MIN(last, sizeof(int) - align);

			usbdbg("endpoint->sent %d, tx_packetSize %d, last %d",
				endpoint->sent, endpoint->tx_packetSize, last);

			if(((u32)cp % 64) != 0) printf("error aligned xxx %x\n", cp);
			//AST UDC must 64byte align
			ast_udc_write(cp, AST_VHUB_EP0_DATA_BUFF);
			//trigger
			ast_udc_write(EP0_TX_LEN(last), AST_VHUB_EP0_CTRL);
			ast_udc_write(EP0_TX_LEN(last) | EP0_TX_BUFF_RDY, AST_VHUB_EP0_CTRL);
		}
		endpoint->last = last;
	}
}

ast_udc_ep0_in(struct usb_endpoint_instance *endpoint)
{
	struct usb_device_request *request = &ep0_urb->device_request;
	int ep_addr;

	usbdbg("ast_udc_ep0_in");

	/* Check direction */
	if ((request->bmRequestType & USB_REQ_DIRECTION_MASK) ==
	    USB_REQ_HOST2DEVICE) {
		/*
		 * This tx interrupt must be for a control write status
		 * stage packet.
		 */
		usbdbg("ACK on EP0 control write status stage packet");
	} else {
		/*
		 * This tx interrupt must be for a control read data
		 * stage packet.
		 */
		int wLength = le16_to_cpu(request->wLength);

		/*
		 * Update our count of bytes sent so far in this
		 * transfer.
		 */
		endpoint->sent += endpoint->last;

		/*
		 * We are finished with this transfer if we have sent
		 * all of the bytes in our tx urb (urb->actual_length)
		 * unless we need a zero-length terminating packet.  We
		 * need a zero-length terminating packet if we returned
		 * fewer bytes than were requested (wLength) by the host,
		 * and the number of bytes we returned is an exact
		 * multiple of the packet size endpoint->tx_packetSize.
		 */
		if ((endpoint->sent == ep0_urb->actual_length) &&
		    ((ep0_urb->actual_length == wLength) ||
		     (endpoint->last != endpoint->tx_packetSize))) {
			/* Done with control read data stage. */
			usbdbg("control read data stage complete");
			//trigger for rx 
			endpoint->rcv_urb = ep0_urb;
			endpoint->sent = 0;
			ast_udc_ep0_rx(endpoint);			
			
		} else {
			/*
			 * We still have another packet of data to send
			 * in this control read data stage or else we
			 * need a zero-length terminating packet.
			 */
			usbdbg("ACK control read data stage packet");

			ast_udc_ep0_tx(endpoint);
		}
	}

}

void ast_udc_setup_handle(struct usb_endpoint_instance *endpoint)
{
	void *setup = (void *) (udc_reg + 0x80);
	u8 *datap = (u8 *)&ep0_urb->device_request;
	int ep_addr = endpoint->endpoint_address;

	usbdbg("-> Entering device setup");

	memcpy(datap, setup, 8);

	/* Try to process setup packet */
	if (ep0_recv_setup(ep0_urb)) {
		/* Not a setup packet, stall next EP0 transaction */
		udc_stall_ep(0);
		usberr("can't parse setup packet, still waiting for setup xxxxxx \n");
		return;
	}

	/* Check direction */
	if ((ep0_urb->device_request.bmRequestType & USB_REQ_DIRECTION_MASK) == USB_REQ_HOST2DEVICE) {
		
		switch(ep0_urb->device_request.bRequest) {
			case USB_REQ_SET_ADDRESS:
				usbdbg("set addr %x \n", ep0_urb->device_request.wValue);
				ast_udc_write(ep0_urb->device_request.wValue, AST_VHUB_CONF); 
				ast_udc_write(EP0_TX_BUFF_RDY, AST_VHUB_EP0_CTRL);				
				usbd_device_event_irq(udc_device, DEVICE_ADDRESS_ASSIGNED, 0);
				break;
			case USB_REQ_SET_CONFIGURATION:
				usbdbg("set configuration \n");
				ast_udc_write(EP0_TX_BUFF_RDY, AST_VHUB_EP0_CTRL);
				usbd_device_event_irq(udc_device, DEVICE_CONFIGURED, 0);
				break;
			default:
//				usberr("control write on EP0 TODO ~~~  [%x]", ep0_urb->device_request.bRequest);	
				ast_udc_write(EP0_TX_BUFF_RDY, AST_VHUB_EP0_CTRL);
//				ast_udc_write(ast_udc_read(AST_VHUB_EP0_CTRL) | EP0_STALL, AST_VHUB_EP0_CTRL);
				break;
		}
#if 0		
		if (le16_to_cpu(ep0_urb->device_request.wLength)) {
			/* Stall this request */
			UDCDBG("Stalling unsupported EP0 control write data "
			       "stage.");
			udc_stall_ep(0);
		}
#endif		
	} else {
		usbdbg("control read on EP0");
		/*
		 * The ep0_recv_setup function has already placed our response
		 * packet data in ep0_urb->buffer and the packet length in
		 * ep0_urb->actual_length.
		 */
		endpoint->tx_urb = ep0_urb;
		endpoint->sent = 0;

		ast_udc_ep0_tx(endpoint);
	}

	usbdbg("<- Leaving device setup");
}

void udc_irq(void)
{
	u32 isr = ast_udc_read(AST_VHUB_ISR);
	u32 ep_isr = 0;
	int i = 0;

	if(*((volatile ulong*) 0x1e720080) == 0xdeadbeef) {
		*((volatile ulong*) 0x1e720080) =0;
		eprintf("ryan \n"); 
		
	}
#if 0
	if(isr) 
		usbdbg("isr : %x \n", isr);
	else
		return;
#else
	if(!isr)
		return;
#endif
	if(isr & ISR_BUS_RESET) {
		usbdbg("ISR_BUS_RESET \n");
		ast_udc_write(ISR_BUS_RESET, AST_VHUB_ISR);
		usbd_device_event_irq(udc_device, DEVICE_RESET, 0);
	}

	if(isr & ISR_BUS_SUSPEND) {
		//Suspend, we don't handle this in sample
		usbdbg("ISR_BUS_SUSPEND \n");
		ast_udc_write(ISR_BUS_SUSPEND, AST_VHUB_ISR);
		usbd_device_event_irq(udc_device, DEVICE_BUS_INACTIVE, 0);
	}

	if(isr & ISR_SUSPEND_RESUME) {
		//Suspend, we don't handle this in sample
		usbdbg("ISR_SUSPEND_RESUME \n");
		ast_udc_write(ISR_SUSPEND_RESUME, AST_VHUB_ISR);
		usbd_device_event_irq(udc_device, DEVICE_BUS_ACTIVITY, 0);
	}

	if(isr & ISR_HUB_EP0_IN_ACK_STALL) {
		usbdbg("ISR_HUB_EP0_IN_ACK_STALL \n");
		ast_udc_write(ISR_HUB_EP0_IN_ACK_STALL, AST_VHUB_ISR);
		ast_udc_ep0_in(udc_device->bus->endpoint_array + 0);
	}

	if(isr & ISR_HUB_EP0_OUT_ACK_STALL) {
		usbdbg("ISR_HUB_EP0_OUT_ACK_STALL \n");
		ast_udc_write(ISR_HUB_EP0_OUT_ACK_STALL, AST_VHUB_ISR);
		ast_udc_ep0_out(udc_device->bus->endpoint_array + 0);
	}

	if(isr & ISR_HUB_EP0_OUT_NAK) {
//		usbdbg("ISR_HUB_EP0_OUT_NAK \n");
		ast_udc_write(ISR_HUB_EP0_OUT_NAK, AST_VHUB_ISR);
	}

	if(isr & ISR_HUB_EP0_IN_DATA_NAK) {
		//IN NAK, we don't handle this in sample
		usbdbg("ISR_HUB_EP0_IN_DATA_ACK \n");
		ast_udc_write(ISR_HUB_EP0_IN_DATA_NAK, AST_VHUB_ISR);
	}

	if(isr & ISR_HUB_EP0_SETUP) {
		usbdbg("SETUP \n");
		ast_udc_write(ISR_HUB_EP0_SETUP, AST_VHUB_ISR);
		ast_udc_setup_handle(udc_device->bus->endpoint_array + 0);
	}

	if(isr & ISR_HUB_EP1_IN_DATA_ACK) {
		//HUB Bitmap control
		usberr("ERROR EP1 IN\n");
		ast_udc_write(ISR_HUB_EP1_IN_DATA_ACK, AST_VHUB_ISR);
		ast_udc_write(0x00, AST_VHUB_EP1_STS_CHG);
	}

	if(isr & ISR_DEVICE1) {
		usberr("ISR_DEVICE1\n");
	}

	if(isr & ISR_DEVICE2) {
		usberr("ISR_DEVICE2\n");
	}

	if(isr & ISR_DEVICE3) {
		usberr("ISR_DEVICE3 \n");
	}

	if(isr & ISR_DEVICE4) {
		usberr("ISR_DEVICE4 \n");
	}

	if(isr & ISR_DEVICE5) {
		usberr("ISR_DEVICE5 \n");
	}

	if(isr & ISR_EP_ACK_STALL) {
		usbdbg("ISR_EP_ACK_STALL \n");
		ep_isr = ast_udc_read(AST_VHUB_EP_ACK_ISR);
//		printf("ep_isr %x \n", ep_isr);
		for(i = 0; i < 15; i++) {
			if(ep_isr & (0x1 << i)) {
				ast_udc_write(0x1 <<  i, AST_VHUB_EP_ACK_ISR); 
				ast_udc_ep_handle(udc_device->bus->endpoint_array +  i + 1);			
			}
		}
	}

	if(isr & ISR_EP_NAK) {
		printf("ISR_EP_NAK\n");
		usbdbg("ISR_EP_NAK ****************************************\n");
		ast_udc_write(ISR_EP_NAK, AST_VHUB_ISR);
	}

}

/*
 * udc_unset_nak
 *
 * Suspend sending of NAK tokens for DATA OUT tokens on a given endpoint.
 * Switch off NAKing on this endpoint to accept more data output from host.
 */
void udc_unset_nak(int ep_num)
{
	/* TODO */
}

/*
 * udc_set_nak
 *
 * Allow upper layers to signal lower layers should not accept more RX data
 */
void udc_set_nak(int ep_num)
{
	/* TODO */
}

/* Associate a physical endpoint with endpoint instance */
void udc_setup_ep(struct usb_device_instance *device, unsigned int id,
				struct usb_endpoint_instance *endpoint)
{
	int ep_num, ep_addr, ep_isout, ep_type, ep_size;
	int config, interface, alternate;
	u32 tmp;
	u32 	ep_conf = 0;
	u32 ep_reg = 0;

	usbdbg("setting up endpoint id %d", id);

	if (!endpoint) {
		usberr("endpoint void!");
		return;
	}

	ep_num = endpoint->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
	if (ep_num >= UDC_MAX_ENDPOINTS) {
		usberr("unable to setup ep %d!", ep_num);
		return;
	}

	if (ep_num == 0) {
		/* Done for ep0 */
		return;
	}

	config = 1;
	interface = 0;
	alternate = 0;

	usbdbg("config %d - interface %d - alternate %d",
		config, interface, alternate);

	ep_addr = endpoint->endpoint_address;
	ep_num = ep_addr & USB_ENDPOINT_NUMBER_MASK;
	ep_isout = (ep_addr & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT;
	ep_type = ep_isout ? endpoint->rcv_attributes : endpoint->tx_attributes;
	ep_size = ep_isout ? endpoint->rcv_packetSize : endpoint->tx_packetSize;

	usbdbg("addr %x, num %d, dir %s, type %s, packet size %d",
		ep_addr, ep_num,
		ep_isout ? "out" : "in",
		ep_type == USB_ENDPOINT_XFER_ISOC ? "isoc" :
		ep_type == USB_ENDPOINT_XFER_BULK ? "bulk" :
		ep_type == USB_ENDPOINT_XFER_INT ? "int" : "???",
		ep_size
		);

	/* Configure EP */

	if(ep_size == 1024) {
		ep_conf = 0;
	} else {
		ep_conf = EP_SET_MAX_PKT(ep_size);
	}
	
	ep_conf |= EP_SET_EP_NUM(ep_num);
	
	switch(ep_type) {
		case USB_ENDPOINT_XFER_ISOC:
			if(ep_isout) {
				ep_conf |= EP_TYPE_ISO_OUT;
			} else {
				ep_conf |= EP_TYPE_ISO_IN;				
			}
			break;
		case USB_ENDPOINT_XFER_BULK:
			if(ep_isout) {
				ep_conf |= EP_TYPE_BULK_OUT;
			} else {
				ep_conf |= EP_TYPE_BULK_IN;				
			}			
			break;
		case USB_ENDPOINT_XFER_INT:
			if(ep_isout) {
				ep_conf |= EP_TYPE_INT_OUT;
			} else {
				ep_conf |= EP_TYPE_INT_IN;
			}			
			break;
	}

	ep_reg = udc_reg + 0x200 + (0x10 * (ep_num - 1));	

	ast_ep_write(ep_reg, 0x4, AST_EP_DMA_CTRL);
	ast_ep_write(ep_reg, 0x2, AST_EP_DMA_CTRL); 
	ast_ep_write(ep_reg, 0, AST_EP_DMA_STS);
	ast_ep_write(ep_reg, ep_conf | EP_ENABLE, AST_EP_CONFIG);


	//also setup dma
	if(ep_isout) {
		if(((u32)endpoint->rcv_urb->buffer % 64) != 0) printf("rx addr %x fail ~~\n", endpoint->rcv_urb->buffer);
		ast_ep_write(ep_reg, endpoint->rcv_urb->buffer, AST_EP_DMA_BUFF);
		ast_ep_write(ep_reg, endpoint->rcv_urb->buffer_length << 16, AST_EP_DMA_STS);
		ast_ep_write(ep_reg, (endpoint->rcv_urb->buffer_length << 16) | 0x1, AST_EP_DMA_STS);
	} else {
		if(((u32)endpoint->tx_urb->buffer % 64) != 0) printf("rx addr %x fail ~~\n", endpoint->tx_urb->buffer);
		ast_ep_write(ep_reg, endpoint->tx_urb->buffer, AST_EP_DMA_BUFF);
	}
	
	//TODO ~~
//	ast_ep_write(ep_reg, tx_len << 16, AST_EP_DMA_STS);
//	ast_ep_write(ep_reg, tx_len << 16 | 0x1, AST_EP_DMA_STS);		
	
}

/* Connect the USB device to the bus */
void udc_connect(void)
{
	usbdbg("UDC connect");
	ast_udc_write(ast_udc_read(AST_VHUB_CTRL) | ROOT_UPSTREAM_EN, AST_VHUB_CTRL);	

}

/* Disconnect the USB device to the bus */
void udc_disconnect(void)
{
	usbdbg("UDC disconnect");
	ast_udc_write(ast_udc_read(AST_VHUB_CTRL) & ~ROOT_UPSTREAM_EN, AST_VHUB_CTRL);

}

/* Switch on the UDC */
void udc_enable(struct usb_device_instance *device)
{
	usbdbg("udc_enable");

	udc_device = device;
	if (!ep0_urb)
		ep0_urb = usbd_alloc_urb(udc_device,
				udc_device->bus->endpoint_array);
	else
		usbinfo("ep0_urb %p already allocated", ep0_urb);

}

/* Need to check this again */
void udc_disable(void)
{
	usbdbg("disable UDC");
#if 0
	udc_clear_mask_UDCCR(UDCCR_UDE);

	/* Disable clock for USB device */
	writel(readl(CKEN) & ~CKEN11_USB, CKEN);

	/* Free ep0 URB */
	if (ep0_urb) {
		usbd_dealloc_urb(ep0_urb);
		ep0_urb = NULL;
	}

	/* Reset device pointer */
	udc_device = NULL;
#endif	
}

/* Allow udc code to do any additional startup */
void udc_startup_events(struct usb_device_instance *device)
{
	usbdbg("udc_startup_events");

	/* The DEVICE_INIT event puts the USB device in the state STATE_INIT */
	usbd_device_event_irq(device, DEVICE_INIT, 0);

	/* The DEVICE_CREATE event puts the USB device in the state
	 * STATE_ATTACHED */
	usbd_device_event_irq(device, DEVICE_CREATE, 0);

	/* Some USB controller driver implementations signal
	 * DEVICE_HUB_CONFIGURED and DEVICE_RESET events here.
	 * DEVICE_HUB_CONFIGURED causes a transition to the state
	 * STATE_POWERED, and DEVICE_RESET causes a transition to
	 * the state STATE_DEFAULT.
	 */
	udc_enable(device);

}

/* Initialize h/w stuff */
int udc_init(void)
{
	udc_device = NULL;
	usbdbg("udc_init");
	
	ast_scu_multi_func_usb_port1_mode(0);
	ast_scu_init_usb_port1();

	ast_udc_write(ROOT_PHY_CLK_EN | ROOT_PHY_RESET_DIS, AST_VHUB_CTRL);
	
	udelay(1);
	ast_udc_write(0, AST_VHUB_DEV_RESET);

	ast_udc_write(0x1ffff, AST_VHUB_IER);
	
	ast_udc_write(0x1ffff, AST_VHUB_ISR);

	ast_udc_write(0x7ffff, AST_VHUB_EP_ACK_ISR);		
	
	ast_udc_write(0x7ffff, AST_VHUB_EP_ACK_IER);	
	ast_udc_write(0, AST_VHUB_EP0_CTRL);
//	ast_udc_write(0x1, AST_VHUB_EP1_CTRL);
	ast_udc_write(0, AST_VHUB_EP1_CTRL);	
	return 0;
}

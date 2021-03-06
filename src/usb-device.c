/*
 * usb-device.c
 *
 *  Created on: Oct 20, 2017
 *      Author: technix
 */

#include <stm32f1xx.h>
#include <stm32f1xx_it.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdbool.h>
#include <usb.h>
#include <usb_hid.h>
#include <time.h>
#include <unistd.h>
#include <dreamos-rt/gpio.h>

#include "usb-config.h"
#include "usb-device.h"
#include "board.h"

static usbd_device usb_device;
static uint32_t usb_buffer[64];

#define USB_RXD_EP	0x01
#define USB_TXD_EP	0x82
#define USB_PKT_SZ	64

__attribute__((packed)) struct config_descriptor
{
	struct usb_config_descriptor	 config;
	struct usb_interface_descriptor interface;
	struct usb_hid_descriptor hid_descriptor;
	struct usb_endpoint_descriptor endpoints[2];
};

static const struct usb_device_descriptor device_descriptor =
{
	    .bLength            = sizeof(struct usb_device_descriptor),
	    .bDescriptorType    = USB_DTYPE_DEVICE,
	    .bcdUSB             = VERSION_BCD(2, 0, 0),
	    .bDeviceClass       = USB_CLASS_PER_INTERFACE,
	    .bDeviceSubClass    = USB_SUBCLASS_NONE,
	    .bDeviceProtocol    = USB_PROTO_NONE,
	    .bMaxPacketSize0    = USB_EP0_SIZE,
	    .idVendor           = USB_VID,
	    .idProduct          = USB_PID,
	    .bcdDevice          = VERSION_BCD(0, 1, 0),
	    .iManufacturer      = 1,
	    .iProduct           = 2,
	    .iSerialNumber      = INTSERIALNO_DESCRIPTOR,
	    .bNumConfigurations = 1
};

static const struct usb_qualifier_descriptor qualifier_descriptor =
{
	    .bLength            = sizeof(struct usb_qualifier_descriptor),
	    .bDescriptorType    = USB_DTYPE_QUALIFIER,
	    .bcdUSB             = VERSION_BCD(2, 0, 0),
	    .bDeviceClass       = USB_CLASS_PER_INTERFACE,
	    .bDeviceSubClass    = USB_SUBCLASS_NONE,
	    .bDeviceProtocol    = USB_PROTO_NONE,
	    .bMaxPacketSize0    = USB_EP0_SIZE,
	    .bNumConfigurations = 1,
	    .bReserved          = 0
};

static const uint8_t usb_hid_report[] =
{
	    0x06, 0x00, 0xff,              // USAGE_PAGE (Vendor Defined Page 1)
	    0x09, 0x01,                    // USAGE (Vendor Usage 1)
	    0xa1, 0x01,                    // COLLECTION (Application)
	    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
	    0x27, 0xff, 0xff, 0x00, 0x00,  //   LOGICAL_MAXIMUM (65535)
	    0x75, 0x10,                    //   REPORT_SIZE (16)
	    0x95, 0x01,                    //   REPORT_COUNT (1)
	    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
	    0x81, 0x82,                    //   INPUT (Data,Var,Abs,Vol)
	    0x95, 0x01,                    //   REPORT_COUNT (1)
	    0x09, 0x01,                    //   USAGE (Vendor Usage 1)
	    0x91, 0x82,                    //   OUTPUT (Data,Var,Abs,Vol)
	    0xc0                           // END_COLLECTION
};

#define USB_REPORT	sizeof(usb_hid_report)

static const struct config_descriptor config_descriptor =
{
		.config =
		{
		        .bLength                = sizeof(struct usb_config_descriptor),
		        .bDescriptorType        = USB_DTYPE_CONFIGURATION,
		        .wTotalLength           = sizeof(config_descriptor),
		        .bNumInterfaces         = 1,
		        .bConfigurationValue    = 1,
		        .iConfiguration         = NO_DESCRIPTOR,
		        .bmAttributes           = USB_CFG_ATTR_RESERVED,
		        .bMaxPower              = USB_CFG_POWER_MA(500)
		},
		.interface =
		{
		        .bLength                = sizeof(struct usb_interface_descriptor),
		        .bDescriptorType        = USB_DTYPE_INTERFACE,
		        .bInterfaceNumber       = 0,
		        .bAlternateSetting      = 0,
		        .bNumEndpoints          = 2,
		        .bInterfaceClass        = USB_CLASS_HID,
		        .bInterfaceSubClass     = USB_SUBCLASS_NONE,
		        .bInterfaceProtocol     = USB_PROTO_NONE,
		        .iInterface             = NO_DESCRIPTOR,
		},
		.hid_descriptor =
		{
				.bLength                = sizeof(struct usb_hid_descriptor),
				.bDescriptorType        = USB_DTYPE_HID,
				.bcdHID                 = VERSION_BCD(1, 1, 1),
				.bCountryCode           = USB_HID_COUNTRY_US,
				.bNumDescriptors        = 1,
				.bDescriptorType0       = USB_DTYPE_HID_REPORT,
				.wDescriptorLength0     = USB_REPORT
		},
		.endpoints =
		{
				{
				        .bLength                = sizeof(struct usb_endpoint_descriptor),
				        .bDescriptorType        = USB_DTYPE_ENDPOINT,
				        .bEndpointAddress       = USB_RXD_EP,
				        .bmAttributes           = USB_EPTYPE_INTERRUPT,
				        .wMaxPacketSize         = USB_PKT_SZ,
				        .bInterval              = 100,
				},
				{
				        .bLength                = sizeof(struct usb_endpoint_descriptor),
				        .bDescriptorType        = USB_DTYPE_ENDPOINT,
				        .bEndpointAddress       = USB_TXD_EP,
				        .bmAttributes           = USB_EPTYPE_INTERRUPT,
				        .wMaxPacketSize         = USB_PKT_SZ,
				        .bInterval              = 100,
				}
		}
};

static const struct usb_string_descriptor lang_desc     = USB_ARRAY_DESC(USB_LANGID_ENG_US);
static const struct usb_string_descriptor manuf_desc_en = USB_STRING_DESC(USB_MANUFACTURER);
static const struct usb_string_descriptor prod_desc_en  = USB_STRING_DESC(USB_PRODUCT);
static const struct usb_string_descriptor *const desc_string_table[] =
{
		&lang_desc,
		&manuf_desc_en,
		&prod_desc_en,
};

static usbd_respond usb_get_descriptor(usbd_ctlreq *req, void **address, uint16_t *length);
static usbd_respond usb_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback);
static usbd_respond usb_set_config(usbd_device *dev, uint8_t cfg);
static void usb_handle(usbd_device *dev, uint8_t event, uint8_t ep);
clock_t last_poll = 0;

__attribute__((constructor(2000))) void usb_init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;

	usbd_init(&usb_device, &usbd_hw, USB_EP0_SIZE, usb_buffer, sizeof(usb_buffer));
	usbd_reg_descr(&usb_device, usb_get_descriptor);
	usbd_reg_control(&usb_device, usb_control);
	usbd_reg_config(&usb_device, usb_set_config);
	__DSB();

	usbd_enable(&usb_device, true);
	usbd_connect(&usb_device, true);
	digitalWrite(0x08, true);
	__DSB();

	NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	NVIC_EnableIRQ(USBWakeUp_IRQn);
}

__attribute__((section(".datacode"))) void USB_IRQHandler(void)
{
	usbd_poll(&usb_device);
}

__attribute__((alias("USB_IRQHandler"))) void USB_HP_CAN1_TX_IRQHandler(void);
__attribute__((alias("USB_IRQHandler"))) void USB_LP_CAN1_RX0_IRQHandler(void);
__attribute__((alias("USB_IRQHandler"))) void USBWakeUp_IRQHandler(void);

static usbd_respond usb_get_descriptor(usbd_ctlreq *req, void **address, uint16_t *length)
{
	uint8_t descriptor_type = (req->wValue & 0xff00) >> 8;
	uint8_t descriptor_idx = req->wValue & 0xff;

	const void *buf = NULL;
	uint16_t len = 0;

	switch (descriptor_type)
	{
	case USB_DTYPE_DEVICE:
		buf = &device_descriptor;
		break;

	case USB_DTYPE_CONFIGURATION:
		buf = &config_descriptor;
		len = sizeof(config_descriptor);
		break;

	case USB_DTYPE_QUALIFIER:
		buf = &qualifier_descriptor;
		break;

	case USB_DTYPE_HID:
		buf = &config_descriptor.hid_descriptor;
		break;

	case USB_DTYPE_HID_REPORT:
		buf = usb_hid_report;
		len = USB_REPORT;
		break;

	case USB_DTYPE_STRING:
		if (descriptor_idx < sizeof(desc_string_table) / sizeof(struct usb_string_descriptor))
			buf = desc_string_table[descriptor_idx];
		break;

	default:
		break;
	}

	if (!buf)
		return usbd_fail;
	len = len ?: ((struct usb_device_descriptor *)buf)->bLength;

	if (address)
		*address = (void *)buf;
	if (length)
		*length = len;

	return usbd_ack;
}

static usbd_respond usb_control(usbd_device *dev, usbd_ctlreq *req, usbd_rqc_callback *callback)
{
	usbd_respond result = usbd_fail;

	if (req->bRequest == USB_STD_GET_DESCRIPTOR)
		return usb_get_descriptor(req, &dev->status.data_ptr, &dev->status.data_count);

	return result;
}

static usbd_respond usb_set_config(usbd_device *dev, uint8_t cfg)
{
	switch (cfg)
	{
	case 0:
		// Deconfigure everything.
		usbd_ep_deconfig(&usb_device, USB_RXD_EP);
		usbd_ep_deconfig(&usb_device, USB_TXD_EP);
		usbd_reg_endpoint(&usb_device, USB_RXD_EP, NULL);
		usbd_reg_endpoint(&usb_device, USB_TXD_EP, NULL);
		return usbd_ack;

	case 1:
		usbd_ep_config(dev, USB_RXD_EP, USB_EPTYPE_INTERRUPT, USB_PKT_SZ);
		usbd_ep_config(dev, USB_TXD_EP, USB_EPTYPE_INTERRUPT, USB_PKT_SZ);
		usbd_reg_endpoint(dev, USB_RXD_EP, usb_handle);
		usbd_reg_endpoint(dev, USB_TXD_EP, usb_handle);
		usbd_ep_write(dev, USB_TXD_EP, NULL, 0);
        return usbd_ack;

	default:
		break;
	}
	return usbd_fail;
}

static void usb_handle(usbd_device *dev, uint8_t event, uint8_t ep)
{
	bool update_poll = false;
	switch (event)
	{
	case usbd_evt_eptx:
		usbd_ep_write(dev, USB_TXD_EP, NULL, 0);
		update_poll = true;
		break;

	case usbd_evt_eprx:
		usbd_ep_read(dev, USB_RXD_EP, NULL, 0);
		update_poll = true;
		break;

	default:
		break;
	}

	if (update_poll)
	{
		last_poll = clock();
	}
}

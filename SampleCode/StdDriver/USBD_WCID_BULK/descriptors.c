/******************************************************************************
 * @file     descriptors.c
 * @version  V1.00
 * $Revision: 6 $
 * $Date: 18/04/13 3:43p $
 * @brief    M031 series USBD driver source file
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/
/*!<Includes */
#include "NuMicro.h"
#include "vcom_serial.h"

/*----------------------------------------------------------------------------*/
/*!<USB Device Descriptor */
uint8_t gu8DeviceDescriptor[] =
{
    LEN_DEVICE,     /* bLength */
    DESC_DEVICE,    /* bDescriptorType */
    0x00, 0x02,     /* bcdUSB */
    0x02,           /* bDeviceClass */
    0x00,           /* bDeviceSubClass */
    0x00,           /* bDeviceProtocol */
    EP0_MAX_PKT_SIZE,   /* bMaxPacketSize0 */
    /* idVendor */
    USBD_VID & 0x00FF,
    ((USBD_VID & 0xFF00) >> 8),
    /* idProduct */
    USBD_PID & 0x00FF,
    ((USBD_PID & 0xFF00) >> 8),
    0x00, 0x01,     /* bcdDevice */
    0x01,           /* iManufacture */
    0x02,           /* iProduct */
    0x03,           /* iSerialNumber - no serial */
    0x01            /* bNumConfigurations */
};

/*!<USB Configure Descriptor */
uint8_t gu8ConfigDescriptor[] =
{
    LEN_CONFIG,     /* bLength              */
    DESC_CONFIG,    /* bDescriptorType      */
    (0x19 + 7), 0x00,   /* wTotalLength         */
    0x01,           /* bNumInterfaces       */
    0x01,           /* bConfigurationValue  */
    0x00,           /* iConfiguration       */
    0x80,           /* bmAttributes         */
    0x64,           /* MaxPower             */


    /* INTERFACE descriptor */
    LEN_INTERFACE,  /* bLength              */
    DESC_INTERFACE, /* bDescriptorType      */
    0x00,           /* bInterfaceNumber     */
    0x00,           /* bAlternateSetting    */
    0x02,           /* bNumEndpoints        */
    0xff,           /* bInterfaceClass      */
    0xff,           /* bInterfaceSubClass   */
    0x00,           /* bInterfaceProtocol   */
    0x00,           /* iInterface           */

    /* ENDPOINT descriptor */
    LEN_ENDPOINT,                   /* bLength          */
    DESC_ENDPOINT,                  /* bDescriptorType  */
    (EP_INPUT | BULK_IN_EP_NUM),    /* bEndpointAddress */
    EP_BULK,                        /* bmAttributes     */
    EP2_MAX_PKT_SIZE, 0x00,         /* wMaxPacketSize   */
    0x00,                           /* bInterval        */

    /* ENDPOINT descriptor */
    LEN_ENDPOINT,                   /* bLength          */
    DESC_ENDPOINT,                  /* bDescriptorType  */
    (EP_OUTPUT | BULK_OUT_EP_NUM),  /* bEndpointAddress */
    EP_BULK,                        /* bmAttributes     */
    EP3_MAX_PKT_SIZE, 0x00,         /* wMaxPacketSize   */
    0x00,                           /* bInterval        */

};

/*!<USB Language String Descriptor */
uint8_t gu8StringLang[4] =
{
    4,              /* bLength */
    DESC_STRING,    /* bDescriptorType */
    0x09, 0x04
};

/*!<USB Vendor String Descriptor */
uint8_t gu8VendorStringDesc[] =
{
    0x10,                                             /* bLength */
    DESC_STRING,                                      /* bDescriptorType */
    'N', 0x00,                                        /* wcChar0 */
    'U', 0x00,                                        /* wcChar1 */
    'V', 0x00,                                        /* wcChar2 */
    'O', 0x00,                                        /* wcChar3 */
    'T', 0x00,                                        /* wcChar4 */
    'O', 0x00,                                        /* wcChar5 */
    'N', 0x00,                                        /* wcChar6 */
};

/*!<USB Product String Descriptor */
uint8_t gu8ProductStringDesc[] =
{
    0x14,                                             /* bLength */
    DESC_STRING,                                      /* bDescriptorType */
    'M', 0x00,                                        /* wcChar0 */
    '0', 0x00,                                        /* wcChar1 */
    '3', 0x00,                                        /* wcChar2 */
    '1', 0x00,                                        /* wcChar3 */
    ' ', 0x00,                                        /* wcChar4 */
    'B', 0x00,                                        /* wcChar5 */
    'U', 0x00,                                        /* wcChar6 */
    'L', 0x00,                                        /* wcChar7 */
    'K', 0x00,                                        /* wcChar8 */
};

uint8_t gu8SerialStringDesc[] =
{
    0x0e,                                             /* bLength */
    DESC_STRING,                                      /* bDescriptorType */
    'v', 0x00,                                        /* wcChar0 */
    '1', 0x00,                                        /* wcChar1 */
    '2', 0x00,                                        /* wcChar2 */
    '3', 0x00,                                        /* wcChar3 */
    '4', 0x00,                                        /* wcChar4 */
    '5', 0x00,                                        /* wcChar5 */
};
#define WCID_VENDOR_CODE    0xA1
const uint8_t  g_HID_sOSStringDesc[18] =
{
    ///////////////////////////////////////
    /// MS OS string descriptor
    ///////////////////////////////////////
    0x12,                                             /* bLength */
    DESC_STRING,                                      /* bDescriptorType */
    /* MSFT100 */
    'M', 0x00, 'S', 0x00, 'F', 0x00, 'T', 0x00,       /* wcChar_7 */
    '1', 0x00, '0', 0x00, '0', 0x00,                  /* wcChar_7 */
    WCID_VENDOR_CODE,                                 /* bVendorCode */
    0x00,                                             /* bReserved */
};

/*!<USB BOS Descriptor */
const uint8_t gu8BOSDescriptor[] =
{
    LEN_BOS,        /* bLength */
    DESC_BOS,       /* bDescriptorType */
    /* wTotalLength */
    0x0C & 0x00FF,
    ((0x0C & 0xFF00) >> 8),
    0x01,           /* bNumDeviceCaps */

    /* Device Capability */
    0x7,            /* bLength */
    DESC_CAPABILITY,/* bDescriptorType */
    CAP_USB20_EXT,  /* bDevCapabilityType */
    0x02, 0x00, 0x00, 0x00  /* bmAttributes */
};

uint8_t *gpu8UsbString[4] =
{
    gu8StringLang,
    gu8VendorStringDesc,
    gu8ProductStringDesc,
    gu8SerialStringDesc,
};


const S_USBD_INFO_T gsInfo =
{
    (uint8_t *)gu8DeviceDescriptor,
    (uint8_t *)gu8ConfigDescriptor,
    (uint8_t **)gpu8UsbString,
    (uint8_t **)NULL,
    (uint8_t *)gu8BOSDescriptor,
    (uint32_t *)NULL,
    (uint32_t *)NULL
};

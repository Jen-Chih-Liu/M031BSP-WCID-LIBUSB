/******************************************************************************
 * @file     vcom_serial.c
 * @version  V1.00
 * $Revision: 10 $
 * $Date: 18/07/18 4:46p $
 * @brief    M031 series USBD driver Sample file
 *
 * @note
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (C) 2018 Nuvoton Technology Corp. All rights reserved.
 *****************************************************************************/

/*!<Includes */
#include <stdio.h>
#include <string.h>
#include "NuMicro.h"
#include "vcom_serial.h"

uint8_t volatile g_u8Suspend = 0;
uint8_t volatile g_u8EP3Ready = 0;
uint8_t volatile g_u8EP2Ready = 0;
/*--------------------------------------------------------------------------*/
void USBD_IRQHandler(void)
{
    uint32_t u32IntSts = USBD_GET_INT_FLAG();
    uint32_t u32State = USBD_GET_BUS_STATE();

    if (u32IntSts & USBD_INTSTS_FLDET)
    {
        /* Floating detect */
        USBD_CLR_INT_FLAG(USBD_INTSTS_FLDET);

        if (USBD_IS_ATTACHED())
        {
            /* USB Plug In */
            USBD_ENABLE_USB();
        }
        else
        {
            /* USB Un-plug */
            USBD_DISABLE_USB();
        }
    }

    if (u32IntSts & USBD_INTSTS_BUS)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_BUS);

        if (u32State & USBD_STATE_USBRST)
        {
            /* Bus reset */
            USBD_ENABLE_USB();
            USBD_SwReset();
            g_u8Suspend = 0;
        }

        if (u32State & USBD_STATE_SUSPEND)
        {
            /* Enter power down to wait USB attached */
            g_u8Suspend = 1;

            /* Enable USB but disable PHY */
            USBD_DISABLE_PHY();
        }

        if (u32State & USBD_STATE_RESUME)
        {
            /* Enable USB and enable PHY */
            USBD_ENABLE_USB();
            g_u8Suspend = 0;
        }
    }

    if (u32IntSts & USBD_INTSTS_SOF)
    {
        /* Clear SOF flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_SOF);
    }

    if (u32IntSts & USBD_INTSTS_WAKEUP)
    {
        /* Clear event flag */
        USBD_CLR_INT_FLAG(USBD_INTSTS_WAKEUP);
    }

    if (u32IntSts & USBD_INTSTS_USB)
    {
        extern uint8_t g_usbd_SetupPacket[];

        /* USB event */
        if (u32IntSts & USBD_INTSTS_SETUP)
        {
            /* Setup packet */
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_SETUP);

            /* Clear the data IN/OUT ready flag of control end-points */
            USBD_STOP_TRANSACTION(EP0);
            USBD_STOP_TRANSACTION(EP1);

            USBD_ProcessSetupPacket();
        }

        /* EP events */
        if (u32IntSts & USBD_INTSTS_EP0)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP0);
            /* control IN */
            USBD_CtrlIn();
        }

        if (u32IntSts & USBD_INTSTS_EP1)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP1);
            /* control OUT */
            USBD_CtrlOut();

        }

        if (u32IntSts & USBD_INTSTS_EP2)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP2);
            /* Bulk IN */
            g_u8EP2Ready = 1;
        }

        if (u32IntSts & USBD_INTSTS_EP3)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP3);
            /* Bulk Out */
            g_u8EP3Ready = 1;
        }

        if (u32IntSts & USBD_INTSTS_EP4)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP4);
        }

        if (u32IntSts & USBD_INTSTS_EP5)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP5);
        }

        if (u32IntSts & USBD_INTSTS_EP6)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP6);
        }

        if (u32IntSts & USBD_INTSTS_EP7)
        {
            /* Clear event flag */
            USBD_CLR_INT_FLAG(USBD_INTSTS_EP7);
        }
    }
}






/*--------------------------------------------------------------------------*/
/**
  * @brief  USBD Endpoint Config.
  * @param  None.
  * @retval None.
  */
void VCOM_Init(void)
{
    /* Init setup packet buffer */
    /* Buffer for setup packet -> [0 ~ 0x7] */
    USBD->STBUFSEG = SETUP_BUF_BASE;

    /*****************************************************/
    /* EP0 ==> control IN endpoint, address 0 */
    USBD_CONFIG_EP(EP0, USBD_CFG_CSTALL | USBD_CFG_EPMODE_IN | 0);
    /* Buffer range for EP0 */
    USBD_SET_EP_BUF_ADDR(EP0, EP0_BUF_BASE);

    /* EP1 ==> control OUT endpoint, address 0 */
    USBD_CONFIG_EP(EP1, USBD_CFG_CSTALL | USBD_CFG_EPMODE_OUT | 0);
    /* Buffer range for EP1 */
    USBD_SET_EP_BUF_ADDR(EP1, EP1_BUF_BASE);

    /*****************************************************/
    /* EP2 ==> Bulk IN endpoint, address 1 */
    USBD_CONFIG_EP(EP2, USBD_CFG_EPMODE_IN | BULK_IN_EP_NUM);
    /* Buffer offset for EP2 */
    USBD_SET_EP_BUF_ADDR(EP2, EP2_BUF_BASE);
    /* trigger receive in data */
    USBD_SET_PAYLOAD_LEN(EP2, EP2_MAX_PKT_SIZE);

    /* EP3 ==> Bulk Out endpoint, address 2 */
    USBD_CONFIG_EP(EP3, USBD_CFG_EPMODE_OUT | BULK_OUT_EP_NUM);
    /* Buffer offset for EP3 */
    USBD_SET_EP_BUF_ADDR(EP3, EP3_BUF_BASE);
    /* trigger receive OUT data */
    USBD_SET_PAYLOAD_LEN(EP3, EP3_MAX_PKT_SIZE);
}



#define WCID_VENDOR_CODE    0xA1
extern volatile uint8_t  g_usbd_CtrlInZeroFlag;

/***************************************************/
uint8_t WINUSB_WCIDDescriptor[] =
{
    ///////////////////////////////////////
    /// WCID descriptor
    ///////////////////////////////////////
    0x28, 0x00, 0x00, 0x00,                           /* dwLength */
    0x00, 0x01,                                       /* bcdVersion */
    0x04, 0x00,                                       /* wIndex */
    0x01,                                             /* bCount */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,         /* bReserved_7 */

    ///////////////////////////////////////
    /// WCID function descriptor
    ///////////////////////////////////////
    0x00,                                             /* bFirstInterfaceNumber */
    0x01,                                             /* bReserved */
    /* WINUSB */
    'W', 'I', 'N', 'U', 'S', 'B', 0x00, 0x00,         /* cCID_8 */
    /*  */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   /* cSubCID_8 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00,               /* bReserved_6 */
};
uint8_t WINUSB_IF0_WCIDProperties[142] =
{
    ///////////////////////////////////////
    /// WCID property descriptor
    ///////////////////////////////////////
    0x8e, 0x00, 0x00, 0x00,                           /* dwLength */
    0x00, 0x01,                                       /* bcdVersion */
    0x05, 0x00,                                       /* wIndex */
    0x01, 0x00,                                       /* wCount */

    ///////////////////////////////////////
    /// registry propter descriptor
    ///////////////////////////////////////
    0x84, 0x00, 0x00, 0x00,                           /* dwSize */
    0x01, 0x00, 0x00, 0x00,                           /* dwPropertyDataType */
    0x28, 0x00,                                       /* wPropertyNameLength */
    /* DeviceInterfaceGUID */
    'D', 0x00, 'e', 0x00, 'v', 0x00, 'i', 0x00,       /* wcName_20 */
    'c', 0x00, 'e', 0x00, 'I', 0x00, 'n', 0x00,       /* wcName_20 */
    't', 0x00, 'e', 0x00, 'r', 0x00, 'f', 0x00,       /* wcName_20 */
    'a', 0x00, 'c', 0x00, 'e', 0x00, 'G', 0x00,       /* wcName_20 */
    'U', 0x00, 'I', 0x00, 'D', 0x00, 0x00, 0x00,      /* wcName_20 */
    0x4e, 0x00, 0x00, 0x00,                           /* dwPropertyDataLength */
    /* {1D4B2365-4749-48EA-B38A-7C6FDDDD7E26} */
    '{', 0x00, '1', 0x00, 'D', 0x00, '4', 0x00,       /* wcData_39 */
    'B', 0x00, '2', 0x00, '3', 0x00, '6', 0x00,       /* wcData_39 */
    '5', 0x00, '-', 0x00, '4', 0x00, '7', 0x00,       /* wcData_39 */
    '4', 0x00, '9', 0x00, '-', 0x00, '4', 0x00,       /* wcData_39 */
    '8', 0x00, 'E', 0x00, 'A', 0x00, '-', 0x00,       /* wcData_39 */
    'B', 0x00, '3', 0x00, '8', 0x00, 'A', 0x00,       /* wcData_39 */
    '-', 0x00, '7', 0x00, 'C', 0x00, '6', 0x00,       /* wcData_39 */
    'F', 0x00, 'D', 0x00, 'D', 0x00, 'D', 0x00,       /* wcData_39 */
    'D', 0x00, '7', 0x00, 'E', 0x00, '2', 0x00,       /* wcData_39 */
    '7', 0x00, '}', 0x00, 0x00, 0x00,                 /* wcData_39 */
};

void VendorLBK_VendorRequest(void)
{
    uint8_t   buf[8];
    uint8_t   data_len;
    uint16_t wIndex;
    uint32_t u32Len;

    USBD_GetSetupPacket(buf);

    data_len = (buf[7] << 8) | buf[6];
    wIndex = (buf[5] << 8) | buf[4];

    if (buf[0] & 0x80)
    {
        // Device to host
        switch (buf[1])
        {
            case WCID_VENDOR_CODE:
                if (wIndex == 0x0004)
                {
                    u32Len = WINUSB_WCIDDescriptor[0];

                    if (u32Len > data_len)
                        u32Len = data_len;

                    if ((u32Len % 64) == 0ul)
                    {
                        g_usbd_CtrlInZeroFlag = (uint8_t)1ul;
                    }

                    USBD_PrepareCtrlIn(WINUSB_WCIDDescriptor, u32Len);

                    /* Status stage */
                    USBD_PrepareCtrlOut(0, 0);

                }
                else if (wIndex == 0x0005)
                {
                    u32Len = WINUSB_IF0_WCIDProperties[0];

                    if (u32Len > data_len)
                        u32Len = data_len;

                    if ((u32Len % 64) == 0ul)
                    {
                        g_usbd_CtrlInZeroFlag = (uint8_t)1ul;
                    }

                    USBD_PrepareCtrlIn((uint8_t *)WINUSB_IF0_WCIDProperties, u32Len);

                    /* Status stage */
                    USBD_PrepareCtrlOut(0, 0);
                }

                break;

            default:
                // Stall
                /* Setup error, stall the device */
                USBD_SetStall(0);
                break;
        }
    }
    else
    {
        // Host to device
        switch (buf[1])
        {
            default:
                // Stall
                /* Setup error, stall the device */
                USBD_SetStall(0);
                break;
        }
    }
}

volatile uint8_t g_Ctrl_Buff[64];   // buffer for ctrl
/* Vendor LBK device vendor commands */
#define REQ_SET_DATA        0x01
#define REQ_GET_DATA        0x12
void VendorLBK_ClassRequest(void)
{
    uint8_t   buf[8];
    uint8_t   data_len;

    USBD_GetSetupPacket(buf);

    data_len = (buf[7] << 8) | buf[6];

    if (buf[0] & 0x80)   /* request data transfer direction */
    {
        // Device to host
        switch (buf[1])
        {
            case REQ_GET_DATA:
                if (data_len > 64)
                    data_len = 64;

                //USBD_MemCopy((uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP0)), (uint8_t *)&SendBuffer, g_SendLen);
                /* Data stage */
                USBD_PrepareCtrlIn((uint8_t *)g_Ctrl_Buff, data_len);
                /* Status stage */
                USBD_PrepareCtrlOut(0, 0);
                break;

            default:
                /* Setup error, stall the device */
                USBD_SetStall(0);
                break;
        }
    }
    else
    {
        // Host to device
        switch (buf[1])
        {
            case REQ_SET_DATA:
                USBD_PrepareCtrlOut((uint8_t *)g_Ctrl_Buff, buf[6]);

                /* Request Type = INPUT */
                USBD_SET_DATA1(EP1);
                USBD_SET_PAYLOAD_LEN(EP1, data_len);
                /* Status stage */
                USBD_PrepareCtrlIn(0, 0);
                break;

            default:
                // Stall
                /* Setup error, stall the device */
                USBD_SetStall(0);
                break;
        }
    }
}

static void dump_raw_hex(uint8_t *buf, uint32_t len)
{
    int nIdx, i;

    nIdx = 0;

    while (len > 0)
    {
        printf("0x%04X  ", nIdx);

        for (i = 0; (i < 16) && (len > 0); i++)
        {
            printf("%02x ", buf[nIdx + i]);
            len--;
        }

        nIdx += 16;
        printf("\n");
    }

    printf("\n");
}
uint8_t g_Bulk_Buff[EP3_BUF_LEN];   // buffer for bulk
uint32_t out_count = 0;
uint32_t in_count = 0;
void VendorLBK_ProcessData(void)
{
    uint32_t data_len, i;

    if (g_u8EP3Ready)
    {
        g_u8EP3Ready = 0;
        out_count = out_count + 64;
        data_len = USBD_GET_PAYLOAD_LEN(EP3);

        if (data_len > EP3_MAX_PKT_SIZE)
        {
            printf("Error! EP3 data overrun!\n");
            data_len = EP3_MAX_PKT_SIZE;
        }

        /* Read bulk-out packet from USBD SRAM to g_Bulk_Buff[] */
        for (i = 0; i < data_len; i++)
        {
            g_Bulk_Buff[i] = inpb(USBD_BUF_BASE + EP3_BUF_BASE + i);
        }

        printf("[BULK-OUT]\r\n");
        USBD_SET_PAYLOAD_LEN(EP3, EP3_MAX_PKT_SIZE);
    }

    if (out_count == 1024)
    {
        uint8_t *buf;
        out_count = 0;
        in_count = 0;
        //for new data in ready first
        buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2));

        for (i = 0; i < EP2_MAX_PKT_SIZE; i++)
        {
            buf[i] = in_count;
            in_count++;
        }

        USBD_SET_PAYLOAD_LEN(EP2, EP2_MAX_PKT_SIZE);
    }

    if (g_u8EP2Ready)
    {
        uint8_t *buf;
        g_u8EP2Ready = 0;
        buf = (uint8_t *)(USBD_BUF_BASE + USBD_GET_EP_BUF_ADDR(EP2));

        for (i = 0; i < EP2_MAX_PKT_SIZE; i++)
        {
            buf[i] = in_count;
            in_count++;
        }

        printf("[BULK-IN]\r\n");
        USBD_SET_PAYLOAD_LEN(EP2, EP2_MAX_PKT_SIZE);
    }

}

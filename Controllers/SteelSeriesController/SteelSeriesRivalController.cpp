/*-----------------------------------------*\
|  SteelSeriesRivalController.h             |
|                                           |
|  Definitions and types for SteelSeries    |
|  Rival lighting controller                |
|                                           |
|  B Horn (bahorn) 13/5/2020                |
\*-----------------------------------------*/

#include "SteelSeriesRivalController.h"
#include <cstring>
#include <stdio.h>
#include <stdlib.h>

static void send_usb_msg(hid_device* dev, char * data_pkt, unsigned int size)
{
    char* usb_pkt = new char[size + 1];
    
    usb_pkt[0] = 0x00;
    for(unsigned int i = 1; i < size + 1; i++)
    {
        usb_pkt[i] = data_pkt[i-1];
    }
    
    hid_write(dev, (unsigned char *)usb_pkt, size + 1);
    
    delete[] usb_pkt;
}

SteelSeriesRivalController::SteelSeriesRivalController
    (
    hid_device*         dev_handle,
    steelseries_type    proto_type,
    const char*         path
    )
{
    dev         = dev_handle;
    location    = path;
    proto       = proto_type;
}

SteelSeriesRivalController::~SteelSeriesRivalController()
{
    hid_close(dev);
}

std::string SteelSeriesRivalController::GetDeviceLocation()
{
    return("HID: " + location);
}

char* SteelSeriesRivalController::GetDeviceName()
{
    return device_name;
}

std::string SteelSeriesRivalController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev, serial_string, 128);
    
    if (ret != 0)
    {
        return("");
    }

    std::wstring return_wstring = serial_string;
    std::string return_string(return_wstring.begin(), return_wstring.end());

    return(return_string);
}

steelseries_type SteelSeriesRivalController::GetMouseType()
{
    return proto;
}

/* Saves to the internal configuration */
void SteelSeriesRivalController::Save()
{
    char usb_buf[9];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]       = 0x09;
    send_usb_msg(dev, usb_buf, 9);
}

void SteelSeriesRivalController::SetLightEffect
    (
    unsigned char   zone_id,
    unsigned char   effect
    )
{
    char usb_buf[9];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    switch (proto)
    {
        case RIVAL_100:
            usb_buf[0x00]       = 0x07;
            usb_buf[0x01]       = 0x00;
            break;

        case RIVAL_300:
            usb_buf[0x00]       = 0x07;
            usb_buf[0x01]       = zone_id + 1;
            break;

        default:
            break;
    }
    usb_buf[0x02]       = effect;
    send_usb_msg(dev, usb_buf, 9);
}

void SteelSeriesRivalController::SetLightEffectAll
    (
    unsigned char   effect
    )
{
    switch(proto)
    {
        case RIVAL_100:
            SetLightEffect(0, effect);
            break;

        case RIVAL_300:
            SetLightEffect(0, effect);
            SetLightEffect(1, effect);
            break;

        case RIVAL_650:
            for(int i=0x10; i<0x18; i++)
            {
                SetLightEffect(i, effect);
            }
            break;

        default:
            break;
    }
}

void SteelSeriesRivalController::SetRival650Color
    (
    unsigned char   zone_id,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    char usb_buf[60];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]       = 0x03;
    usb_buf[0x04]       = 0x30;
    usb_buf[0x06]       = 0x10;
    usb_buf[0x07]       = 0x27;
    usb_buf[0x16]       = 0x01;
    usb_buf[0x1E]       = 0x04;
    usb_buf[0x1F]       = red;
    usb_buf[0x20]       = green;
    usb_buf[0x21]       = blue;
    usb_buf[0x22]       = 0xFF;
    usb_buf[0x27]       = 0xFF;
    usb_buf[0x29]       = 0x54;
    usb_buf[0x2C]       = 0xFF;
    usb_buf[0x2D]       = 0x54;
    usb_buf[0x2E]       = red;
    usb_buf[0x2F]       = green;
    usb_buf[0x30]       = blue;
    usb_buf[0x31]       = 0x56;

    send_usb_msg(dev, usb_buf, 60);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]       = 0x03;
    usb_buf[0x02]       = 0x30;
    usb_buf[0x04]       = 0x2C;

    send_usb_msg(dev, usb_buf, 60);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]       = 0x05;
    usb_buf[0x02]       = zone_id;//mousekey 0x10-0x17
    usb_buf[0x03]       = 0xFF;
    usb_buf[0x08]       = 0x5C;

    send_usb_msg(dev, usb_buf, 60);

    memset(usb_buf, 0x00, sizeof(usb_buf));
    usb_buf[0x00]       = 0x1C;
    usb_buf[0x02]       = 0x55;
    usb_buf[0x04]       = 0x46;

    send_usb_msg(dev, usb_buf, 60);
}

void SteelSeriesRivalController::SetRival600Color
    (
    unsigned char   zone_id,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    char usb_buf[0x25];

    memset(usb_buf, 0x00, sizeof(usb_buf));

    usb_buf[0x00]       = 0x05;

    memset(usb_buf+0x2, zone_id, 6);

    usb_buf[0x08]       = 0xE8;     //set time to 1000
    usb_buf[0x09]       = 0x3;


    usb_buf[0x18]       = 1;        //set trigger flag
    usb_buf[0x1D]       = 1;        //set number of colors

    usb_buf[0x1E]       = red;
    usb_buf[0x1F]       = green;
    usb_buf[0x20]       = blue;
    usb_buf[0x21]       = red;
    usb_buf[0x22]       = green;
    usb_buf[0x23]       = blue;

    unsigned char* usb_pkt = new unsigned char[0x25 + 1];
    usb_pkt[0] = 0x00;
    for(unsigned int i = 1; i < 0x25 + 1; i++)
    {
        usb_pkt[i] = usb_buf[i-1];
    }

    hid_write(dev, (unsigned char *)usb_pkt, 0x25 + 1);
    hid_send_feature_report(dev, (unsigned char *)usb_pkt, 0x25 + 1);

    delete []  usb_pkt;

    usb_buf[0x00]       = 0x09;
    usb_buf[0x01]       = 0x00;
    send_usb_msg(dev, usb_buf, 0x02);
}

void SteelSeriesRivalController::SetColor
    (
    unsigned char   zone_id,
    unsigned char   red,
    unsigned char   green,
    unsigned char   blue
    )
{
    char usb_buf[9];
    memset(usb_buf, 0x00, sizeof(usb_buf));
    switch (proto)
    {
        case RIVAL_100:
            usb_buf[0x00]       = 0x05;
            usb_buf[0x01]       = 0x00;
            break;

        case RIVAL_300:
            usb_buf[0x00]       = 0x08;
            usb_buf[0x01]       = zone_id + 1;
            break;

        case RIVAL_650:
            SetRival650Color(zone_id, red, green, blue);
            return;

        case RIVAL_600:
            SetRival600Color(zone_id, red, green, blue);
            return;

        default:
            break;
    }

    usb_buf[0x02]       = red;
    usb_buf[0x03]       = green;
    usb_buf[0x04]       = blue;

    send_usb_msg(dev, usb_buf, 9);
}

void SteelSeriesRivalController::SetColorAll
    (
        unsigned char   red,
        unsigned char   green,
        unsigned char   blue
    )
{
    switch(proto)
    {
        case RIVAL_100:
            SetColor(0, red, green, blue);
            break;

        case RIVAL_300:
            SetColor(0, red, green, blue);
            SetColor(1, red, green, blue);
            break;

        case RIVAL_650:
            for(int i = 0x10; i < 0x18; i++)
            {
                SetColor(i, red, green, blue);
            }
            break;

        default:
            break;
    }
}


/*---------------------------------------------------------*\
|  Processing Code for Corsair Lighting Node Pro            |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/12/2020        |
\*---------------------------------------------------------*/

#include "CorsairLightingNodeController.h"

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

//Include thread libraries for Windows or Linux
#ifdef WIN32
#include <process.h>
#else
#include "pthread.h"
#include "unistd.h"
#endif

//Thread functions have different types in Windows and Linux
#ifdef WIN32
#define THREAD static void
#define THREADRETURN
#else
#define THREAD static void*
#define THREADRETURN return(NULL);
#endif

using namespace std::chrono_literals;

THREAD keepalive_thread(void *param)
{
    CorsairLightingNodeController* corsair = static_cast<CorsairLightingNodeController*>(param);
    corsair->KeepaliveThread();
    THREADRETURN
}

CorsairLightingNodeController::CorsairLightingNodeController(hid_device* dev_handle)
{
    dev = dev_handle;

    SendFirmwareRequest();

    /*-----------------------------------------------------*\
    | The Corsair Lighting Node Pro requires a packet within|
    | 20 seconds of sending the lighting change in order    |
    | to not revert back into rainbow mode.  Start a thread |
    | to continuously send a keepalive packet every 5s      |
    \*-----------------------------------------------------*/
#ifdef WIN32
    _beginthread(keepalive_thread, 0, this);
#else
    pthread_t thread;
    pthread_create(&thread, NULL, &keepalive_thread, this);
#endif
}

CorsairLightingNodeController::~CorsairLightingNodeController()
{
}

void CorsairLightingNodeController::KeepaliveThread()
{
    while(1)
    {
        if((clock() - last_commit_time) > 5000)
        {
            SendCommit();
        }
        std::this_thread::sleep_for(1s);
    }
}

unsigned short CorsairLightingNodeController::GetFanRPM(unsigned char fan_channel)
{
    int             actual;
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Get Fan RPM packet                             |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_GET_FAN_RPM;
    usb_buf[0x01]   = fan_channel;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    actual = hid_read(dev, usb_buf, 64);

    if(actual > 0)
    {
        return((usb_buf[0x02] << 8) + (usb_buf[0x01] & 0xFF));
    }
    else
    {
        return(0);
    }
}

std::string CorsairLightingNodeController::GetFirmwareString()
{
    return(firmware_version);
}

void CorsairLightingNodeController::SetChannelEffect(unsigned char channel,
                                                unsigned char num_leds,
                                                unsigned char mode,
                                                unsigned char speed,
                                                unsigned char direction,
                                                bool          random,
                                                unsigned char red1,
                                                unsigned char grn1,
                                                unsigned char blu1,
                                                unsigned char red2,
                                                unsigned char grn2,
                                                unsigned char blu2,
                                                unsigned char red3,
                                                unsigned char grn3,
                                                unsigned char blu3
                                               )
{
    /*-----------------------------------------------------*\
    | Send Reset packet                                     |
    \*-----------------------------------------------------*/
    SendReset(channel);

    /*-----------------------------------------------------*\
    | Send Begin packet                                     |
    \*-----------------------------------------------------*/
    SendBegin(channel);

    /*-----------------------------------------------------*\
    | Set Port State packet                                 |
    \*-----------------------------------------------------*/
    SendPortState(channel, CORSAIR_LIGHTING_NODE_PORT_STATE_HARDWARE);

    /*-----------------------------------------------------*\
    | Set Effect Configuration packet                       |
    \*-----------------------------------------------------*/
    SendEffectConfig
    (
        channel,
        0,
        num_leds,
        mode,
        speed,
        direction,
        random,
        red1,
        grn1,
        blu1,
        red2,
        grn2,
        blu2,
        red3,
        grn3,
        blu3,
        0,
        0,
        0
    );

    /*-----------------------------------------------------*\
    | Send Commit packet                                    |
    \*-----------------------------------------------------*/
    SendCommit();
}

void CorsairLightingNodeController::SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors)
{
    unsigned char   red_color_data[50];
    unsigned char   grn_color_data[50];
    unsigned char   blu_color_data[50];
    unsigned char   pkt_offset          = 0;
    unsigned char   pkt_size            = 0;
    unsigned int    colors_remaining    = num_colors;

    /*-----------------------------------------------------*\
    | Send Port State packet                                |
    \*-----------------------------------------------------*/
    SendPortState(channel, CORSAIR_LIGHTING_NODE_PORT_STATE_SOFTWARE);

    /*-----------------------------------------------------*\
    | Loop through colors and send 50 at a time             |
    \*-----------------------------------------------------*/
    while(colors_remaining > 0)
    {
        if(colors_remaining < 50)
        {
            pkt_size = colors_remaining;
        }
        else
        {
            pkt_size = 50;
        }
        
        for(int color_idx = 0; color_idx < pkt_size; color_idx++)
        {
            red_color_data[color_idx] = RGBGetRValue(colors[pkt_offset + color_idx]);
            grn_color_data[color_idx] = RGBGetGValue(colors[pkt_offset + color_idx]);
            blu_color_data[color_idx] = RGBGetBValue(colors[pkt_offset + color_idx]);
        }

        SendDirect(channel, pkt_offset, pkt_size, CORSAIR_LIGHTING_NODE_DIRECT_CHANNEL_RED,   red_color_data);
        SendDirect(channel, pkt_offset, pkt_size, CORSAIR_LIGHTING_NODE_DIRECT_CHANNEL_GREEN, grn_color_data);
        SendDirect(channel, pkt_offset, pkt_size, CORSAIR_LIGHTING_NODE_DIRECT_CHANNEL_BLUE,  blu_color_data);

        colors_remaining -= pkt_size;
        pkt_offset       += pkt_size;
    }

    /*-----------------------------------------------------*\
    | Send Commit packet                                    |
    \*-----------------------------------------------------*/
    SendCommit();
}

void CorsairLightingNodeController::SetFanPercent
    (
        unsigned char   fan_channel,
        unsigned char   fan_cmd
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Fan Fixed Percent Command packet               |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_FAN_CMD_PERCENT;
    usb_buf[0x01]   = fan_channel;
    usb_buf[0x02]   = fan_cmd;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
}

void CorsairLightingNodeController::SetFanRPM
    (
        unsigned char   fan_channel,
        unsigned short  fan_cmd
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Fan Fixed RPM Command packet                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_FAN_CMD_RPM;
    usb_buf[0x01]   = fan_channel;
    usb_buf[0x02]   = fan_cmd & 0xFF;
    usb_buf[0x03]   = fan_cmd >> 8;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void CorsairLightingNodeController::SendFirmwareRequest()
{
    int             actual;
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Firmware Version Request packet                |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_FIRMWARE;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    actual = hid_read(dev, usb_buf, 16);

    if(actual > 0)
    {
        firmware_version = std::to_string(usb_buf[0x01]) + "." + std::to_string(usb_buf[0x02]) + "." + std::to_string(usb_buf[0x03]);
    }
}

void CorsairLightingNodeController::SendDirect
    (
    unsigned char   channel,
    unsigned char   start,
    unsigned char   count,
    unsigned char   color_channel,
    unsigned char*  color_data
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Direct packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_DIRECT;
    usb_buf[0x01]   = channel;
    usb_buf[0x02]   = start;
    usb_buf[0x03]   = count;
    usb_buf[0x04]   = color_channel;

    /*-----------------------------------------------------*\
    | Copy in color data bytes                              |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x05], color_data, count);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 16);
}

void CorsairLightingNodeController::SendCommit()
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Update last commit time                               |
    \*-----------------------------------------------------*/
    last_commit_time = clock();

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_COMMIT;
    usb_buf[0x01]   = 0xFF;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 16);
}

void CorsairLightingNodeController::SendBegin
    (
    unsigned char   channel
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Begin packet                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_BEGIN;
    usb_buf[0x01]   = channel;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 16);
}

void CorsairLightingNodeController::SendEffectConfig
    (
    unsigned char   channel,
    unsigned char   count,
    unsigned char   led_type,
    unsigned char   mode,
    unsigned char   speed,
    unsigned char   direction,
    unsigned char   change_style,
    unsigned char   color_0_red,
    unsigned char   color_0_green,
    unsigned char   color_0_blue,
    unsigned char   color_1_red,
    unsigned char   color_1_green,
    unsigned char   color_1_blue,
    unsigned char   color_2_red,
    unsigned char   color_2_green,
    unsigned char   color_2_blue,
    unsigned short  temperature_0,
    unsigned short  temperature_1,
    unsigned short  temperature_2
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Effect Config packet                           |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_EFFECT_CONFIG;
    usb_buf[0x01]   = channel;
    usb_buf[0x02]   = count;
    usb_buf[0x03]   = led_type;

    /*-----------------------------------------------------*\
    | Set up mode parameters                                |
    \*-----------------------------------------------------*/
    usb_buf[0x04]   = mode;
    usb_buf[0x05]   = speed;
    usb_buf[0x06]   = direction;
    usb_buf[0x07]   = change_style;
    usb_buf[0x08]   = 0;

    /*-----------------------------------------------------*\
    | Set up mode colors                                    |
    \*-----------------------------------------------------*/
    usb_buf[0x09]   = color_0_red;
    usb_buf[0x0A]   = color_0_green;
    usb_buf[0x0B]   = color_0_blue;
    usb_buf[0x0C]   = color_1_red;
    usb_buf[0x0D]   = color_1_green;
    usb_buf[0x0E]   = color_1_blue;
    usb_buf[0x0F]   = color_2_red;
    usb_buf[0x10]   = color_2_green;
    usb_buf[0x11]   = color_2_blue;

    /*-----------------------------------------------------*\
    | Set up temperatures                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x12]   = (temperature_0 >> 8);
    usb_buf[0x13]   = (temperature_0 & 0xFF);
    usb_buf[0x14]   = (temperature_1 >> 8);
    usb_buf[0x15]   = (temperature_1 & 0xFF);
    usb_buf[0x16]   = (temperature_2 >> 8);
    usb_buf[0x17]   = (temperature_2 & 0xFF);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 16);
}

void CorsairLightingNodeController::SendTemperature()
{

}

void CorsairLightingNodeController::SendReset
    (
    unsigned char   channel
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Reset packet                                   |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_RESET;
    usb_buf[0x01]   = channel;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 16);
}

void CorsairLightingNodeController::SendPortState
    (
    unsigned char   channel,
    unsigned char   state
    )
{
    unsigned char   usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Port State packet                              |
    \*-----------------------------------------------------*/
    usb_buf[0x00]   = CORSAIR_LIGHTING_NODE_PACKET_ID_PORT_STATE;
    usb_buf[0x01]   = channel;
    usb_buf[0x02]   = state;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev, usb_buf, 64);
    hid_read(dev, usb_buf, 16);
}

void CorsairLightingNodeController::SendBrightness()
{

}

void CorsairLightingNodeController::SendLEDCount()
{

}

void CorsairLightingNodeController::SendProtocol()
{

}

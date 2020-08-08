/*-----------------------------------------*\
|  HyperXPulsefireSurgeController.cpp       |
|                                           |
|  Driver for HyperX Pulsefire Surge        |
|  lighting controller                      |
|                                           |
|  Adam Honse (CalcProgrammer1) 7/25/2020   |
\*-----------------------------------------*/

#include "HyperXPulsefireSurgeController.h"

#include <cstring>

HyperXPulsefireSurgeController::HyperXPulsefireSurgeController(hid_device* dev_handle)
{
    dev = dev_handle;

    //SelectProfile(0x01);
    //SetProfileBrightness(0x01, 0x64);
}

HyperXPulsefireSurgeController::~HyperXPulsefireSurgeController()
{

}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void HyperXPulsefireSurgeController::SelectProfile
    (
    unsigned char   profile
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PULSEFIRE_SURGE_PACKET_ID_SELECT_PROFILE;
    buf[0x02]   = profile;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXPulsefireSurgeController::SetProfileBrightness
    (
    unsigned char   profile,
    unsigned char   brightness
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PULSEFIRE_SURGE_PACKET_ID_SET_BRIGHTNESS;
    buf[0x02]   = profile;
    buf[0x03]   = 0x01;
    buf[0x04]   = 0x01;
    buf[0x05]   = 0x01;
    buf[0x06]   = 0x64;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

void HyperXPulsefireSurgeController::SendDirect
    (
    RGBColor*       color_data
    )
{
    unsigned char buf[264];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(buf, 0x00, sizeof(buf));

    /*-----------------------------------------------------*\
    | Set up Select Profile packet                          |
    \*-----------------------------------------------------*/
    buf[0x00]   = 0x07;
    buf[0x01]   = HYPERX_PULSEFIRE_SURGE_PACKET_ID_DIRECT;
    buf[0x03]   = 0xA0;

    for(int red_idx = 0; red_idx < 32; red_idx++)
    {
        buf[0x08 + red_idx] = RGBGetRValue(color_data[red_idx]);
    }

    for(int grn_idx = 0; grn_idx < 32; grn_idx++)
    {
        buf[0x28 + grn_idx] = RGBGetGValue(color_data[grn_idx]);
    }

    for(int blu_idx = 0; blu_idx < 32; blu_idx++)
    {
        buf[0x48 + blu_idx] = RGBGetBValue(color_data[blu_idx]);
    }

    buf[0x6C] = RGBGetRValue(color_data[32]);
    buf[0x6D] = RGBGetGValue(color_data[32]);
    buf[0x6E] = RGBGetBValue(color_data[32]);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_send_feature_report(dev, buf, 264);
}

/*---------------------------------------------------------*\
|  Definitions for Corsair Lighting Node Pro                |
|                                                           |
|  Adam Honse (calcprogrammer1@gmail.com), 1/12/2020        |
\*---------------------------------------------------------*/

#include "RGBController.h"
#include <vector>
#include <hidapi/hidapi.h>

#pragma once

enum
{
    CORSAIR_LIGHTING_NODE_PACKET_ID_FIRMWARE         = 0x02,     /* Get firmware version                 */
    CORSAIR_LIGHTING_NODE_PACKET_ID_GET_FAN_RPM      = 0x21,     /* Get fan RPM                          */
    CORSAIR_LIGHTING_NODE_PACKET_ID_FAN_CMD_PERCENT  = 0x23,     /* Set fan fixed percent command        */
    CORSAIR_LIGHTING_NODE_PACKET_ID_FAN_CMD_RPM      = 0x24,     /* Set fan fixed RPM command            */
    CORSAIR_LIGHTING_NODE_PACKET_ID_DIRECT           = 0x32,     /* Direct mode LED update packet        */
    CORSAIR_LIGHTING_NODE_PACKET_ID_COMMIT           = 0x33,     /* Commit changes packet                */
    CORSAIR_LIGHTING_NODE_PACKET_ID_BEGIN            = 0x34,     /* Begin effect packet                  */
    CORSAIR_LIGHTING_NODE_PACKET_ID_EFFECT_CONFIG    = 0x35,     /* Effect mode configuration packet     */
    CORSAIR_LIGHTING_NODE_PACKET_ID_TEMPERATURE      = 0x36,     /* Update temperature value packet      */
    CORSAIR_LIGHTING_NODE_PACKET_ID_RESET            = 0x37,     /* Reset channel packet                 */
    CORSAIR_LIGHTING_NODE_PACKET_ID_PORT_STATE       = 0x38,     /* Set port state packet                */
    CORSAIR_LIGHTING_NODE_PACKET_ID_BRIGHTNESS       = 0x39,     /* Set brightness packet                */
    CORSAIR_LIGHTING_NODE_PACKET_ID_LED_COUNT        = 0x3A,     /* Set LED count packet                 */
    CORSAIR_LIGHTING_NODE_PACKET_ID_PROTOCOL         = 0x3B,     /* Set protocol packet                  */
};

enum
{
    CORSAIR_LIGHTING_NODE_DIRECT_CHANNEL_RED         = 0x00,     /* Red channel for direct update        */
    CORSAIR_LIGHTING_NODE_DIRECT_CHANNEL_GREEN       = 0x01,     /* Green channel for direct update      */
    CORSAIR_LIGHTING_NODE_DIRECT_CHANNEL_BLUE        = 0x02,     /* Blue channel for direct update       */
};

enum
{
    CORSAIR_LIGHTING_NODE_PORT_STATE_HARDWARE        = 0x01,     /* Effect hardware control of channel   */
    CORSAIR_LIGHTING_NODE_PORT_STATE_SOFTWARE        = 0x02,     /* Direct software control of channel   */
};

enum
{
    CORSAIR_LIGHTING_NODE_LED_TYPE_LED_STRIP         = 0x0A,     /* Corsair LED Strip Type               */
    CORSAIR_LIGHTING_NODE_LED_TYPE_HD_FAN            = 0x0C,     /* Corsair HD-series Fan Type           */
    CORSAIR_LIGHTING_NODE_LED_TYPE_SP_FAN            = 0x01,     /* Corsair SP-series Fan Type           */
    CORSAIR_LIGHTING_NODE_LED_TYPE_ML_FAN            = 0x02,     /* Corsair ML-series Fan Type           */
};

enum
{
    CORSAIR_LIGHTING_NODE_CHANNEL_1                  = 0x00,     /* Channel 1                            */
    CORSAIR_LIGHTING_NODE_CHANNEL_2                  = 0x01,     /* Channel 2                            */
    CORSAIR_LIGHTING_NODE_NUM_CHANNELS               = 0x02,     /* Number of channels                   */
};

enum
{
    CORSAIR_LIGHTING_NODE_FAN_CHANNEL_1              = 0x00,     /* Fan channel 1                        */
    CORSAIR_LIGHTING_NODE_FAN_CHANNEL_2              = 0x01,     /* Fan channel 2                        */
    CORSAIR_LIGHTING_NODE_FAN_CHANNEL_3              = 0x02,     /* Fan channel 3                        */
    CORSAIR_LIGHTING_NODE_FAN_CHANNEL_4              = 0x03,     /* Fan channel 4                        */
    CORSAIR_LIGHTING_NODE_FAN_CHANNEL_5              = 0x04,     /* Fan channel 5                        */
    CORSAIR_LIGHTING_NODE_FAN_CHANNEL_6              = 0x05,     /* Fan channel 6                        */
    CORSAIR_LIGHTING_NODE_NUM_FAN_CHANNELS           = 0x06,     /* Number of fan channels               */
};

enum
{
    CORSAIR_LIGHTING_NODE_SPEED_FAST                 = 0x00,     /* Fast speed                           */
    CORSAIR_LIGHTING_NODE_SPEED_MEDIUM               = 0x01,     /* Medium speed                         */
    CORSAIR_LIGHTING_NODE_SPEED_SLOW                 = 0x02,     /* Slow speed                           */
};

enum
{
    CORSAIR_LIGHTING_NODE_MODE_RAINBOW_WAVE          = 0x00,     /* Rainbow Wave mode                    */
    CORSAIR_LIGHTING_NODE_MODE_COLOR_SHIFT           = 0x01,     /* Color Shift mode                     */
    CORSAIR_LIGHTING_NODE_MODE_COLOR_PULSE           = 0x02,     /* Color Pulse mode                     */
    CORSAIR_LIGHTING_NODE_MODE_COLOR_WAVE            = 0x03,     /* Color Wave mode                      */
    CORSAIR_LIGHTING_NODE_MODE_STATIC                = 0x04,     /* Static mode                          */
    CORSAIR_LIGHTING_NODE_MODE_TEMPERATURE           = 0x05,     /* Temperature mode                     */
    CORSAIR_LIGHTING_NODE_MODE_VISOR                 = 0x06,     /* Visor mode                           */
    CORSAIR_LIGHTING_NODE_MODE_MARQUEE               = 0x07,     /* Marquee mode                         */
    CORSAIR_LIGHTING_NODE_MODE_BLINK                 = 0x08,     /* Blink mode                           */
    CORSAIR_LIGHTING_NODE_MODE_SEQUENTIAL            = 0x09,     /* Sequential mode                      */
    CORSAIR_LIGHTING_NODE_MODE_RAINBOW               = 0x0A,     /* Rainbow mode                         */
};

class CorsairLightingNodeController
{
public:
    CorsairLightingNodeController(hid_device* dev_handle);
    ~CorsairLightingNodeController();

    unsigned short  GetFanRPM(unsigned char fan_channel);

    std::string     GetFirmwareString();

    unsigned int    GetStripsOnChannel(unsigned int channel);

    void            SetChannelEffect(unsigned char channel,
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
                                    );

    void            SetChannelLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);

    void            SetFanPercent
                        (
                            unsigned char   fan_channel,
                            unsigned char   fan_cmd
                        );

    void            SetFanRPM
                        (
                            unsigned char   fan_channel,
                            unsigned short  fan_cmd
                        );

    void            KeepaliveThread();

private:
    hid_device*             dev;
    std::string             firmware_version;
    clock_t                 last_commit_time;

    void            SendFirmwareRequest();

    void            SendDirect
                        (
                        unsigned char   channel,
                        unsigned char   start,
                        unsigned char   count,
                        unsigned char   color_channel,
                        unsigned char*  color_data
                        );

    void            SendCommit();

    void            SendBegin
                        (
                        unsigned char   channel
                        );

    void            SendEffectConfig
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
                        );

    void            SendTemperature();

    void            SendReset
                        (
                        unsigned char   channel
                        );

    void            SendPortState
                        (
                        unsigned char   channel,
                        unsigned char   state
                        );

    void            SendBrightness();

    void            SendLEDCount();

    void            SendProtocol();
};

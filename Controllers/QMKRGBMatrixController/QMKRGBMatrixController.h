/*-------------------------------------------------------------------*\
|  QMKRGBMatrixController.h                                           |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper         10th Octobber 2020                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "hsv.h"
#include <hidapi/hidapi.h>
#include <cstring>
#include <map>

#define QMK_RGBMATRIX_PACKET_SIZE 65

enum CommandsId
{
    QMK_RGBMATRIX_GET_PROTOCOL_VERSION = 1,
    QMK_RGBMATRIX_GET_DEVICE_INFO,
    QMK_RGBMATRIX_GET_MODE_INFO,
    QMK_RGBMATRIX_GET_LED_INFO,
    QMK_RGBMATRIX_GET_IS_MODE_ENABLED,
    QMK_RGBMATRIX_GET_DIRECT_MODE_LED_COLOR,

    QMK_RGBMATRIX_SET_MODE,
    QMK_RGBMATRIX_DIRECT_MODE_SET_SINGLE_LED,
    QMK_RGBMATRIX_DIRECT_MODE_SET_LEDS,
};

enum Modes
{
    QMK_RGBMATRIX_MODE_OPENRGB_DIRECT = 1,
    QMK_RGBMATRIX_MODE_SOLID_COLOR,
    QMK_RGBMATRIX_MODE_ALPHA_MOD,
    QMK_RGBMATRIX_MODE_GRADIENT_UP_DOWN,
    QMK_RGBMATRIX_MODE_GRADIENT_LEFT_RIGHT,
    QMK_RGBMATRIX_MODE_BREATHING,
    QMK_RGBMATRIX_MODE_BAND_SAT,
    QMK_RGBMATRIX_MODE_BAND_VAL,
    QMK_RGBMATRIX_MODE_BAND_PINWHEEL_SAT,
    QMK_RGBMATRIX_MODE_BAND_PINWHEEL_VAL,
    QMK_RGBMATRIX_MODE_BAND_SPIRAL_SAT,
    QMK_RGBMATRIX_MODE_BAND_SPIRAL_VAL,
    QMK_RGBMATRIX_MODE_CYCLE_ALL,
    QMK_RGBMATRIX_MODE_CYCLE_LEFT_RIGHT,
    QMK_RGBMATRIX_MODE_CYCLE_UP_DOWN,
    QMK_RGBMATRIX_MODE_CYCLE_OUT_IN,
    QMK_RGBMATRIX_MODE_CYCLE_OUT_IN_DUAL,
    QMK_RGBMATRIX_MODE_RAINBOW_MOVING_CHEVRON,
    QMK_RGBMATRIX_MODE_CYCLE_PINWHEEL,
    QMK_RGBMATRIX_MODE_CYCLE_SPIRAL,
    QMK_RGBMATRIX_MODE_DUAL_BEACON,
    QMK_RGBMATRIX_MODE_RAINBOW_BEACON,
    QMK_RGBMATRIX_MODE_RAINBOW_PINWHEELS,
    QMK_RGBMATRIX_MODE_RAINDROPS,
    QMK_RGBMATRIX_MODE_JELLYBEAN_RAINDROPS,
    QMK_RGBMATRIX_MODE_TYPING_HEATMAP,
    QMK_RGBMATRIX_MODE_DIGITAL_RAIN,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_SIMPLE,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_WIDE,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTIWIDE,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_CROSS,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTICROSS,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_NEXUS,
    QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTINEXUS,
    QMK_RGBMATRIX_MODE_SPLASH,
    QMK_RGBMATRIX_MODE_MULTISPLASH,
    QMK_RGBMATRIX_MODE_SOLID_SPLASH,
    QMK_RGBMATRIX_MODE_SOLID_MULTISPLASH,
};

enum SpeedCommands
{
    QMK_RGBMATRIX_SPEED_SLOWEST = 0x00, /* Slowest speed                   */
    QMK_RGBMATRIX_SPEED_NORMAL = 0x7F,  /* Normal speed                    */
    QMK_RGBMATRIX_SPEED_FASTEST = 0xFF, /* Fastest speed                   */
};

enum
{
    QMK_RGBMATRIX_FAILURE = 25,
    QMK_RGBMATRIX_SUCCESS = 50,
    QMK_RGBMATRIX_END_OF_MESSAGE = 100,
};

enum
{
    QMK_RGBMATRIX_MODE_BYTE = 1,
    QMK_RGBMATRIX_SPEED_BYTE = 2,
    QMK_RGBMATRIX_HUE_BYTE = 3,
    QMK_RGBMATRIX_SATURATION_BYTE = 4,
    QMK_RGBMATRIX_VALUE_BYTE = 5,
};

enum
{
    QMK_RGBMATRIX_TOTAL_NUMBER_OF_LEDS_BYTE = 1
};

enum
{
    QMK_RGBMATRIX_ZONE_TYPE_BYTE = 1,
    QMK_RGBMATRIX_ZONE_SIZE_BYTE = 2
};

enum
{
    QMK_RGBMATRIX_LED_MATRIX_COLUMNS_COUNT_BYTE = 1,
    QMK_RGBMATRIX_LED_MATRIX_ROWS_COUNT_BYTE = 2
};

typedef struct
{
    uint8_t x;
    uint8_t y;
} point_t;

class QMKRGBMatrixController
{
public:
    QMKRGBMatrixController(hid_device *dev_handle, const char *path);
    ~QMKRGBMatrixController();

    std::string GetLocation();
    std::string GetDeviceName();
    std::string GetDeviceVendor();
    unsigned int GetTotalNumberOfLEDs();
    unsigned int GetMode();
    unsigned int GetModeSpeed();
    unsigned int GetModeColor();
    std::vector<point_t> GetLEDPoints();
    std::vector<unsigned int> GetLEDFlags();
    std::vector<std::string> GetLEDNames();

    unsigned int GetProtocolVersion();
    void GetDeviceInfo();
    void GetModeInfo();
    void GetLEDInfo(unsigned int led);
    bool GetIsModeEnabled(unsigned int mode);
    RGBColor GetDirectModeLEDColor(unsigned int led);

    void SetMode(hsv_t hsv_color, unsigned char mode, unsigned char speed);
    void DirectModeSetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void DirectModeSetLEDs(std::vector<RGBColor> colors, unsigned int num_colors);

protected:
    hid_device *dev;

private:
    std::string location;

    std::string device_name;
    std::string device_vendor;
    unsigned int total_number_of_leds;
    unsigned int mode;
    unsigned int mode_speed;
    RGBColor mode_color;
    std::vector<point_t> led_points;
    std::vector<unsigned int> led_flags;
    std::vector<std::string> led_names;
};

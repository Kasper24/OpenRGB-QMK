/*-------------------------------------------------------------------*\
|  QMKRGBMatrixController.h                                           |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper         10th Octobber 2020                                  |
|                                                                     |
\*-------------------------------------------------------------------*/

#include <string>
#include <array>
#include <hidapi/hidapi.h>
#include "RGBController.h"

#pragma once

enum IdCommands
{
    QMK_RGBMATRIX_SET_MODE_AND_SPEED = 3,
    QMK_RGBMATRIX_SET_SINGLE_LED,
    QMK_RGBMATRIX_SET_LEDS,

    QMK_RGBMATRIX_SET_COLOR_ALL,
    QMK_RGBMATRIX_SET_COLOR,

    QMK_RGBMATRIX_TOGGLE,
    QMK_RGBMATRIX_ENABLE,
    QMK_RGBMATRIX_DISABLE,

    QMK_RGBMATRIX_MODE,
    QMK_RGBMATRIX_STEP,
    QMK_RGBMATRIX_STEP_REVERSE,
    QMK_RGBMATRIX_INCREASE_SPEED,
    QMK_RGBMATRIX_DECREASE_SPEED,
    QMK_RGBMATRIX_SET_SPEED,

    QMK_RGBMATRIX_INCREASE_HUE,
    QMK_RGBMATRIX_DECREASE_HUE,
    QMK_RGBMATRIX_INCREASE_SAT,
    QMK_RGBMATRIX_DECREASE_SAT,
    QMK_RGBMATRIX_INCREASE_VAL,
    QMK_RGBMATRIX_DECREASE_VAL,
    QMK_RGBMATRIX_SET_HSV,
  
    QMK_GET_FIRMWARE,
    QMK_RGBMATRIX_GET_IS_ENABLED,
    QMK_RGBMATRIX_GET_MODE,
    QMK_RGBMATRIX_GET_HUE,
    QMK_RGBMATRIX_GET_SAT,
    QMK_RGBMATRIX_GET_VAL,
    QMK_RGBMATRIX_GET_HSV,
    QMK_RGBMATRIX_GET_SPEED,
    QMK_RGBMATRIX_GET_SUSPEND_STATE,
};

enum EEPROMCommands
{
    QMK_EEPROM_SAVE,
    QMK_EEPROM_DONT_SAVE
};

enum ModeCommands
{
    QMK_MODE_HID_DIRECT = 1,
    QMK_MODE_STATIC,
    QMK_MODE_ALPHA_MOD,
    QMK_MODE_STATIC_GRADIENT_LEFT_RIGHT,
    QMK_MODE_STATIC_GRADIENT_UP_DOWN,
    QMK_MODE_BREATHING,
    QMK_MODE_BAND_SAT,
    QMK_MODE_BAND_VAL,
    QMK_MODE_BAND_PINWHEEL_SAT,
    QMK_MODE_BAND_PINWHEEL_VAL,
    QMK_MODE_BAND_SPIRAL_SAT,
    QMK_MODE_BAND_SPIRAL_VAL,
    QMK_MODE_CYCLE_ALL,
    QMK_MODE_CYCLE_LEFT_RIGHT,
    QMK_MODE_CYCLE_UP_DOWN,
    QMK_MODE_CYCLE_OUT_IN,
    QMK_MODE_CYCLE_OUT_IN_DUAL,
    QMK_MODE_RAINBOW_MOVING_CHEVRON,
    QMK_MODE_DUAL_BEACON,
    QMK_MODE_CYCLE_PINWHEEL,
    QMK_MODE_CYCLE_SPIRAL,
    QMK_MODE_RAINBOW_BEACON,
    QMK_MODE_RAINBOW_PINWHEELS,
    QMK_MODE_RAINDROPS,
    QMK_MODE_JELLYBEAN_RAINDROPS,
    QMK_MODE_TYPING_HEATMAP,
    QMK_MODE_DIGITAL_RAIN,
    QMK_MODE_SOLID_REACTIVE,
    QMK_MODE_SOLID_REACTIVE_SIMPLE,
    QMK_MODE_SOLID_REACTIVE_WIDE,
    QMK_MODE_SOLID_REACTIVE_MULTIWIDE,
    QMK_MODE_SOLID_REACTIVE_CROSS,
    QMK_MODE_SOLID_REACTIVE_MULTICROSS,
    QMK_MODE_SOLID_REACTIVE_NEXUS,
    QMK_MODE_SOLID_REACTIVE_MULTINEXUS,
    QMK_MODE_SPLASH,
    QMK_MODE_MULTISPLASH,
    QMK_MODE_SOLID_SPLASH,
    QMK_MODE_SOLID_MULTISPLASH,
};

enum SpeedCommands
{
    QMK_SPEED_SLOWEST          = 0x00,  /* Slowest speed                   */
    QMK_SPEED_NORMAL           = 0x7F,  /* Normal speed                    */
    QMK_SPEED_FASTEST          = 0xFF,  /* Fastest speed                   */
};

class QMKRGBMatrixController
{
public:
    QMKRGBMatrixController(hid_device* dev_handle);
    ~QMKRGBMatrixController();

    void SetModeAndSpeed(unsigned char mode, unsigned char speed);
    void SetSingleLED(unsigned int led, unsigned char red, unsigned char green, unsigned char blue);
    void SetLEDs(unsigned char channel, RGBColor * colors, unsigned int num_colors);
    void GetFirmware();
    int GetPID();
protected:
    hid_device* dev;
private:
    int pid;
};

/*-------------------------------------------------------------------*\
|  MassdropAlt.cpp                                              |
|                                                                     |
|  Driver for Massdrop Alt keyboard                             |
|                                                                     |
|  Kasper         10th Octobber 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "RGBController_QMKRGBMatrix.h"
#include <QDebug>

//0xFFFFFFFF indicates an unused entry in matrix
#define NA  0xFFFFFFFF

static const char* zone_names[] =
{
    "Keyboard",
    "Light Bar"
};

static const zone_type zone_types[] =
{
    ZONE_TYPE_MATRIX,
    ZONE_TYPE_LINEAR
};

static unsigned int matrix_map_alt[5][15] =
{
    { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10,  11,  12,  13,  14 },
    { 15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29 },
    { 30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  NA,  42,  43 },
    { 44,  NA,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57 },
    { 58,  59,  60,  NA,  NA,  NA,  61,  NA,  NA,  NA,  62,  63,  64,  65,  66 }
};

static unsigned int matrix_map_alt_test[7][17] =
{
    { 100,  99,  98,  97,  96,  95,  NA,  94,  NA,  93,  92,  91,  90,  89,  88,  87,  86 },
    { 101,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  85 },
    { 102,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  84 },
    { 103,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  NA,  42,  43,  83 },
    { 104,  44,  NA,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  82 },
    { 105,  58,  59,  60,  NA,  NA,  NA,  61,  NA,  NA,  NA,  62,  63,  64,  65,  66,  NA },
    { 67,   68,  69,  70,  71,  72,  NA,  73,  NA,  74,  75,  76,  77,  78,  79,  80,  81 }
};

static unsigned int matrix_map_ctrl[6][17] =
{
    { 0,   NA,  1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15 },
    { 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32 },
    { 33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49 },
    { 50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  NA,  NA,  NA,  NA },
    { 63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  NA,  NA,  NA,  72,  NA },
    { 76,  77,  78,  NA,  NA,  NA,  79,  NA,  NA,  NA,  80,  81,  82,  83,  84,  85,  86 }
};

static const unsigned int zone_sizes_alt[] =
{
    67,
    38
};

static const unsigned int zone_sizes_ctrl[] =
{
    87,
    32
};

static const char* led_names_alt[] =
{
    "Key: Escape", // 0
    "Key: 1", // 1
    "Key: 2", // 2
    "Key: 3", // 3
    "Key: 4", // 4
    "Key: 5", // 5
    "Key: 6", // 6
    "Key: 7", // 7
    "Key: 8", // 8
    "Key: 9", // 9
    "Key: 0", // 10
    "Key: -", // 11
    "Key: =", // 12
    "Key: Backspace", // 13
    "Key: Delete", // 14
    "Key: Tab", // 15
    "Key: Q", // 16
    "Key: W", // 17
    "Key: E", // 18
    "Key: R", // 19
    "Key: T", // 20
    "Key: Y", // 21
    "Key: U", // 22
    "Key: I", // 23
    "Key: O", // 24
    "Key: P", // 25
    "Key: [", // 26
    "Key: ]", // 27
    "Key: /", // 28
    "Key: Home", // 29
    "Key: Caps Lock", // 30
    "Key: A", // 31
    "Key: S", // 32
    "Key: D", // 33
    "Key: F", // 34
    "Key: G", // 35
    "Key: H", // 36
    "Key: J", // 37
    "Key: K", // 38
    "Key: L", // 39
    "Key: ;", // 40
    "Key: '", // 41
    "Key: Enter", // 42
    "Key: Page Up", // 43
    "Key: Left Shift", // 44
    "Key: Z", // 45
    "Key: X", // 46
    "Key: C", // 47
    "Key: V", // 48
    "Key: B", // 49
    "Key: N", // 50
    "Key: M", // 51
    "Key: ,", // 52
    "Key: .", // 53
    "Key: /", // 54
    "Key: Right Shift", // 55
    "Key: Up Arrow", // 56
    "Key: Page Down", // 57
    "Key: Left Control", // 58
    "Key: Left Windows", // 59
    "Key: Left Alt", // 60
    "Key: Space", // 61
    "Key: Right Alt", // 62
    "Key: Right Windows", // 63
    "Key: Left Arrow", // 64
    "Key: Down Arrow", // 65
    "Key: Right Arrow", // 66
};

static const char* led_names_ctrl[] =
{
    "Key: Escape", // 0
    "Key: F1", // 1
    "Key: F2", // 2
    "Key: F3", // 3
    "Key: F4", // 4
    "Key: F5", // 5
    "Key: F6", // 6
    "Key: F7", // 7
    "Key: F8", // 8
    "Key: F9", // 9
    "Key: F10", // 10
    "Key: F11", // 11
    "Key: F12", // 12
    "Key: Print Screen", // 13
    "Key: Scroll Lock", // 14
    "Key: Pause/Break", // 15
    "Key: `", // 16
    "Key: 1", // 17
    "Key: 2", // 18
    "Key: 3", // 19
    "Key: 4", // 20
    "Key: 5", // 21
    "Key: 6", // 22
    "Key: 7", // 23
    "Key: 8", // 24
    "Key: 9", // 25
    "Key: 0", // 26
    "Key: -", // 27
    "Key: =", // 28
    "Key: Backspace", // 29
    "Key: Insert", // 30
    "Key: Home", // 31
    "Key: Page Up", // 32
    "Key: Tab", // 33
    "Key: Q", // 34
    "Key: W", // 35
    "Key: E", // 36
    "Key: R", // 37
    "Key: T", // 38
    "Key: Y", // 39
    "Key: U", // 40
    "Key: I", // 41
    "Key: O", // 42
    "Key: P", // 43
    "Key: [", // 44
    "Key: ]", // 45
    "Key: /", // 46
    "Key: Delete", // 47
    "Key: End", // 48
    "Key: Page Down", // 49
    "Key: Caps Lock", // 50
    "Key: A", // 51
    "Key: S", // 52
    "Key: D", // 53
    "Key: F", // 54
    "Key: G", // 55
    "Key: H", // 56
    "Key: J", // 57
    "Key: K", // 58
    "Key: L", // 59
    "Key: ;", // 60
    "Key: '", // 61
    "Key: Enter", // 62
    "Key: Left Shift", // 63
    "Key: Z", // 64
    "Key: X", // 65
    "Key: C", // 66
    "Key: V", // 67
    "Key: B", // 68
    "Key: N", // 69
    "Key: M", // 70
    "Key: ,", // 71
    "Key: .", // 72
    "Key: /", // 73
    "Key: Right Shift", // 74
    "Key: Up Arrow", // 75
    "Key: Left Control", // 76
    "Key: Left Windows", // 77
    "Key: Left Alt", // 78
    "Key: Space", // 79
    "Key: Right Alt", // 80
    "Key: Right Fn", // 81
    "Key: Right Windows", // 82
    "Key: Right Control", // 83
    "Key: Left Arrow", // 84
    "Key: Down Arrow", // 85
    "Key: Right Arrow", // 86
};

RGBController_QMKRGBMatrix::RGBController_QMKRGBMatrix(QMKRGBMatrixController* qmk_rgb_matrix_ptr)
{
    qmk_rgb_matrix             = qmk_rgb_matrix_ptr;
    pid  = qmk_rgb_matrix->GetPID();

    name                = pid == 0 ? "Massdrop Alt" : "Massdrop Ctrl";
    type                = DEVICE_TYPE_KEYBOARD;
    description         = pid == 0 ? "Massdrop Alt" : "Massdrop Ctrl";

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = QMK_MODE_HID_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode Static;
    Static.name       = "Static";
    Static.value      = QMK_MODE_STATIC;
    Static.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Static.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Static);

    /*mode AlphaMod;
    AlphaMod.name       = "Alpha Mod";
    AlphaMod.value      = QMK_MODE_ALPHA_MOD;
    AlphaMod.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    AlphaMod.color_mode = MODE_COLORS_PER_LED;
    AlphaMod.speed_min     = QMK_SPEED_SLOWEST;
    AlphaMod.speed_max     = QMK_SPEED_FASTEST;
    AlphaMod.speed         = QMK_SPEED_NORMAL;
    modes.push_back(AlphaMod);

    mode StaticHorizontalGradient;
    StaticHorizontalGradient.name       = "Static Horizontal Gradient";
    StaticHorizontalGradient.value      = QMK_MODE_STATIC_GRADIENT_LEFT_RIGHT;
    StaticHorizontalGradient.flags      = 0;
    modes.push_back(StaticHorizontalGradient);

    mode StaticVerticalGradient;
    StaticVerticalGradient.name       = "Static Vertical Gradient";
    StaticVerticalGradient.value      = QMK_MODE_STATIC_GRADIENT_UP_DOWN;
    StaticVerticalGradient.flags      = 0;
    modes.push_back(StaticVerticalGradient);*/

    mode Breathing;
    Breathing.name          = "Breathing";
    Breathing.value         = QMK_MODE_BREATHING;
    Breathing.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_BRIGHTNESS;
    Breathing.color_mode    = MODE_COLORS_PER_LED;
    Breathing.speed_min     = QMK_SPEED_SLOWEST;
    Breathing.speed_max     = QMK_SPEED_FASTEST;
    Breathing.speed         = QMK_SPEED_NORMAL;
    modes.push_back(Breathing);

    mode ScrollOnWhite;
    ScrollOnWhite.name          = "Scroll On White";
    ScrollOnWhite.value         = QMK_MODE_BAND_SAT;
    ScrollOnWhite.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    ScrollOnWhite.color_mode    = MODE_COLORS_PER_LED;
    ScrollOnWhite.speed_min     = QMK_SPEED_SLOWEST;
    ScrollOnWhite.speed_max     = QMK_SPEED_FASTEST;
    ScrollOnWhite.speed         = QMK_SPEED_NORMAL;
    modes.push_back(ScrollOnWhite);

    mode Scroll;
    Scroll.name          = "Scroll";
    Scroll.value         = QMK_MODE_BAND_VAL;
    Scroll.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    Scroll.color_mode    = MODE_COLORS_PER_LED;
    Scroll.speed_min     = QMK_SPEED_SLOWEST;
    Scroll.speed_max     = QMK_SPEED_FASTEST;
    Scroll.speed         = QMK_SPEED_NORMAL;
    modes.push_back(Scroll);
 
    mode PinWheelSpinOnWhite;
    PinWheelSpinOnWhite.name          = "Pin Wheel Spin On White";
    PinWheelSpinOnWhite.value         = QMK_MODE_BAND_PINWHEEL_SAT;
    PinWheelSpinOnWhite.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    PinWheelSpinOnWhite.color_mode    = MODE_COLORS_PER_LED;
    PinWheelSpinOnWhite.speed_min     = QMK_SPEED_SLOWEST;
    PinWheelSpinOnWhite.speed_max     = QMK_SPEED_FASTEST;
    PinWheelSpinOnWhite.speed         = QMK_SPEED_NORMAL;
    modes.push_back(PinWheelSpinOnWhite);

    mode PinWheelSpin;
    PinWheelSpin.name          = "Pin Wheel Spin";
    PinWheelSpin.value         = QMK_MODE_BAND_PINWHEEL_VAL;
    PinWheelSpin.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    PinWheelSpin.color_mode    = MODE_COLORS_PER_LED;
    PinWheelSpin.speed_min     = QMK_SPEED_SLOWEST;
    PinWheelSpin.speed_max     = QMK_SPEED_FASTEST;
    PinWheelSpin.speed         = QMK_SPEED_NORMAL;
    modes.push_back(PinWheelSpin);

    mode SpiralSpinOnWhite;
    SpiralSpinOnWhite.name          = "Spiral Spin On White";
    SpiralSpinOnWhite.value         = QMK_MODE_BAND_SPIRAL_SAT;
    SpiralSpinOnWhite.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SpiralSpinOnWhite.color_mode    = MODE_COLORS_PER_LED;
    SpiralSpinOnWhite.speed_min     = QMK_SPEED_SLOWEST;
    SpiralSpinOnWhite.speed_max     = QMK_SPEED_FASTEST;
    SpiralSpinOnWhite.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SpiralSpinOnWhite);

    mode SpiralSpin;
    SpiralSpin.name          = "Spiral Spin";
    SpiralSpin.value         = QMK_MODE_BAND_SPIRAL_VAL;
    SpiralSpin.flags         = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SpiralSpin.color_mode    = MODE_COLORS_PER_LED;
    SpiralSpin.speed_min     = QMK_SPEED_SLOWEST;
    SpiralSpin.speed_max     = QMK_SPEED_FASTEST;
    SpiralSpin.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SpiralSpin);

    mode ColorCycle;
    ColorCycle.name          = "Color Cycle";
    ColorCycle.value         = QMK_MODE_CYCLE_ALL;
    ColorCycle.flags         = MODE_FLAG_HAS_SPEED;
    ColorCycle.speed_min     = QMK_SPEED_SLOWEST;
    ColorCycle.speed_max     = QMK_SPEED_FASTEST;
    ColorCycle.speed         = QMK_SPEED_NORMAL;
    modes.push_back(ColorCycle);

    mode RainbowHorizontal;
    RainbowHorizontal.name          = "Rainbow Horizontal";
    RainbowHorizontal.value         = QMK_MODE_CYCLE_LEFT_RIGHT;
    RainbowHorizontal.flags         = MODE_FLAG_HAS_SPEED;
    RainbowHorizontal.speed_min     = QMK_SPEED_SLOWEST;
    RainbowHorizontal.speed_max     = QMK_SPEED_FASTEST;
    RainbowHorizontal.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowHorizontal);

    mode RainbowVertical;
    RainbowVertical.name          = "Rainbow Vertical";
    RainbowVertical.value         = QMK_MODE_CYCLE_UP_DOWN;
    RainbowVertical.flags         = MODE_FLAG_HAS_SPEED;
    RainbowVertical.speed_min     = QMK_SPEED_SLOWEST;
    RainbowVertical.speed_max     = QMK_SPEED_FASTEST;
    RainbowVertical.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowVertical);

    mode RainbowOutIn;
    RainbowOutIn.name          = "Rainbow Out In";
    RainbowOutIn.value         = QMK_MODE_CYCLE_OUT_IN;
    RainbowOutIn.flags         = MODE_FLAG_HAS_SPEED;
    RainbowOutIn.speed_min     = QMK_SPEED_SLOWEST;
    RainbowOutIn.speed_max     = QMK_SPEED_FASTEST;
    RainbowOutIn.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowOutIn);

    mode RainbowOutInDual;
    RainbowOutInDual.name          = "Rainbow Out In Dual";
    RainbowOutInDual.value         = QMK_MODE_CYCLE_OUT_IN_DUAL;
    RainbowOutInDual.flags         = MODE_FLAG_HAS_SPEED;
    RainbowOutInDual.speed_min     = QMK_SPEED_SLOWEST;
    RainbowOutInDual.speed_max     = QMK_SPEED_FASTEST;
    RainbowOutInDual.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowOutInDual);

    mode RainbowMovingChevron;
    RainbowMovingChevron.name          = "Rainbow Moving Chevron";
    RainbowMovingChevron.value         = QMK_MODE_RAINBOW_MOVING_CHEVRON;
    RainbowMovingChevron.flags         = MODE_FLAG_HAS_SPEED;
    RainbowMovingChevron.speed_min     = QMK_SPEED_SLOWEST;
    RainbowMovingChevron.speed_max     = QMK_SPEED_FASTEST;
    RainbowMovingChevron.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowMovingChevron);

    mode RainbowDualBeacon;
    RainbowDualBeacon.name          = "Rainbow Dual Beacons";
    RainbowDualBeacon.value         = QMK_MODE_DUAL_BEACON;
    RainbowDualBeacon.flags         = MODE_FLAG_HAS_SPEED;
    RainbowDualBeacon.speed_min     = QMK_SPEED_SLOWEST;
    RainbowDualBeacon.speed_max     = QMK_SPEED_FASTEST;
    RainbowDualBeacon.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowDualBeacon);

    mode RainbowPinWheel;
    RainbowPinWheel.name          = "Rainbow Pin Wheel";
    RainbowPinWheel.value         = QMK_MODE_CYCLE_PINWHEEL;
    RainbowPinWheel.flags         = MODE_FLAG_HAS_SPEED;
    RainbowPinWheel.speed_min     = QMK_SPEED_SLOWEST;
    RainbowPinWheel.speed_max     = QMK_SPEED_FASTEST;
    RainbowPinWheel.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowPinWheel);

    mode RainbowCycleSpiral;
    RainbowCycleSpiral.name          = "Rainbow Spiral";
    RainbowCycleSpiral.value         = QMK_MODE_CYCLE_SPIRAL;
    RainbowCycleSpiral.flags         = MODE_FLAG_HAS_SPEED;
    RainbowCycleSpiral.speed_min     = QMK_SPEED_SLOWEST;
    RainbowCycleSpiral.speed_max     = QMK_SPEED_FASTEST;
    RainbowCycleSpiral.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowCycleSpiral);

    mode RainbowBeacon;
    RainbowBeacon.name          = "Rainbow Beacon";
    RainbowBeacon.value         = QMK_MODE_RAINBOW_BEACON;
    RainbowBeacon.flags         = MODE_FLAG_HAS_SPEED;
    RainbowBeacon.speed_min     = QMK_SPEED_SLOWEST;
    RainbowBeacon.speed_max     = QMK_SPEED_FASTEST;
    RainbowBeacon.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowBeacon);

    mode RainbowDualPinWheels;
    RainbowDualPinWheels.name          = "Rainbow Dual Pin Wheels";
    RainbowDualPinWheels.value         = QMK_MODE_RAINBOW_PINWHEELS;
    RainbowDualPinWheels.flags         = MODE_FLAG_HAS_SPEED;
    RainbowDualPinWheels.speed_min     = QMK_SPEED_SLOWEST;
    RainbowDualPinWheels.speed_max     = QMK_SPEED_FASTEST;
    RainbowDualPinWheels.speed         = QMK_SPEED_NORMAL;
    modes.push_back(RainbowDualPinWheels);

    mode Raindrops;
    Raindrops.name          = "Raindrops";
    Raindrops.value         = QMK_MODE_RAINDROPS;
    Raindrops.flags         = 0;
    modes.push_back(Raindrops);

    mode JellybeanRaindrops;
    JellybeanRaindrops.name          = "Jellybean Raindrops";
    JellybeanRaindrops.value         = QMK_MODE_JELLYBEAN_RAINDROPS;
    JellybeanRaindrops.flags         = 0;
    modes.push_back(JellybeanRaindrops);

    mode TypingHeatmap;
    TypingHeatmap.name          = "Typing Heatmap";
    TypingHeatmap.value         = QMK_MODE_TYPING_HEATMAP;
    TypingHeatmap.flags         = 0;
    modes.push_back(TypingHeatmap);

    mode DigitalRain;
    DigitalRain.name          = "Digital Rain";
    DigitalRain.value         = QMK_MODE_DIGITAL_RAIN;
    DigitalRain.flags         = 0;
    modes.push_back(DigitalRain);

    mode SolidReactive;
    SolidReactive.name       = "Solid Reactive";
    SolidReactive.value      = QMK_MODE_SOLID_REACTIVE;
    SolidReactive.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SolidReactive.color_mode = MODE_COLORS_PER_LED;
    SolidReactive.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactive.speed_max     = QMK_SPEED_FASTEST;
    SolidReactive.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactive);

    mode SolidReactiveSimple;
    SolidReactiveSimple.name       = "Solid Reactive Simple";
    SolidReactiveSimple.value      = QMK_MODE_SOLID_REACTIVE_SIMPLE;
    SolidReactiveSimple.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SolidReactiveSimple.color_mode = MODE_COLORS_PER_LED;
    SolidReactiveSimple.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactiveSimple.speed_max     = QMK_SPEED_FASTEST;
    SolidReactiveSimple.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactiveSimple);

    mode SolidReactiveWide;
    SolidReactiveWide.name       = "Solid Reactive Wide";
    SolidReactiveWide.value      = QMK_MODE_SOLID_REACTIVE_WIDE;
    SolidReactiveWide.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SolidReactiveWide.color_mode = MODE_COLORS_PER_LED;
    SolidReactiveWide.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactiveWide.speed_max     = QMK_SPEED_FASTEST;
    SolidReactiveWide.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactiveWide);

    /*mode SolidReactiveMultiWide;
    SolidReactiveMultiWide.name       = "Solid Reactive Multi Wide";
    SolidReactiveMultiWide.value      = QMK_MODE_SOLID_REACTIVE_MULTIWIDE;
    SolidReactiveMultiWide.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SolidReactiveMultiWide.color_mode = MODE_COLORS_PER_LED;
    SolidReactiveMultiWide.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactiveMultiWide.speed_max     = QMK_SPEED_FASTEST;
    SolidReactiveMultiWide.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactiveMultiWide);*/

    mode SolidReactiveCross;
    SolidReactiveCross.name       = "Solid Reactive Cross";
    SolidReactiveCross.value      = QMK_MODE_SOLID_REACTIVE_CROSS;
    SolidReactiveCross.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SolidReactiveCross.color_mode = MODE_COLORS_PER_LED;
    SolidReactiveCross.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactiveCross.speed_max     = QMK_SPEED_FASTEST;
    SolidReactiveCross.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactiveCross);

    /*mode SolidReactiveMultiCross;
    SolidReactiveMultiCross.name       = "Solid Reactive Multi Cross";
    SolidReactiveMultiCross.value      = QMK_MODE_SOLID_REACTIVE_MULTICROSS;
    SolidReactiveMultiCross.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SolidReactiveMultiCross.color_mode = MODE_COLORS_PER_LED;
    SolidReactiveMultiCross.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactiveMultiCross.speed_max     = QMK_SPEED_FASTEST;
    SolidReactiveMultiCross.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactiveMultiCross);*/

    mode SolidReactiveNexus;
    SolidReactiveNexus.name       = "Solid Reactive Nexus";
    SolidReactiveNexus.value      = QMK_MODE_SOLID_REACTIVE_NEXUS;
    SolidReactiveNexus.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SolidReactiveNexus.color_mode = MODE_COLORS_PER_LED;
    SolidReactiveNexus.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactiveNexus.speed_max     = QMK_SPEED_FASTEST;
    SolidReactiveNexus.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactiveNexus);

    /*mode SolidReactiveMultiNexus;
    SolidReactiveMultiNexus.name       = "Solid Reactive Multi Nexus";
    SolidReactiveMultiNexus.value      = QMK_MODE_SOLID_REACTIVE_MULTINEXUS;
    SolidReactiveMultiNexus.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;
    SolidReactiveMultiNexus.color_mode = MODE_COLORS_PER_LED;
    SolidReactiveMultiNexus.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactiveMultiNexus.speed_max     = QMK_SPEED_FASTEST;
    SolidReactiveMultiNexus.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactiveMultiNexus);*/

    mode Splash;
    Splash.name       = "Rainbow Reactive Splash";
    Splash.value      = QMK_MODE_SPLASH;
    Splash.flags      = MODE_FLAG_HAS_SPEED;
    Splash.speed_min     = QMK_SPEED_SLOWEST;
    Splash.speed_max     = QMK_SPEED_FASTEST;
    Splash.speed         = QMK_SPEED_NORMAL;
    modes.push_back(Splash);

    /*mode MultiSplash;
    MultiSplash.name       = "Rainbow Reactive Multi Splash";
    MultiSplash.value      = QMK_MODE_MULTISPLASH;
    MultiSplash.flags      = MODE_FLAG_HAS_SPEED;    
    MultiSplash.speed_min     = QMK_SPEED_SLOWEST;
    MultiSplash.speed_max     = QMK_SPEED_FASTEST;
    MultiSplash.speed         = QMK_SPEED_NORMAL;
    modes.push_back(MultiSplash);*/

    mode SolidSplash;
    SolidReactiveWide.name       = "Solid Reactive Splash";
    SolidReactiveWide.value      = QMK_MODE_SOLID_SPLASH;
    SolidReactiveWide.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;;
    SolidReactiveWide.color_mode = MODE_COLORS_PER_LED;
    SolidReactiveWide.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactiveWide.speed_max     = QMK_SPEED_FASTEST;
    SolidReactiveWide.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactiveWide);

    /*mode SolidMultiSplash;
    SolidReactiveWide.name       = "Solid Reactive Multi Splash";
    SolidReactiveWide.value      = QMK_MODE_SOLID_MULTISPLASH;
    SolidReactiveWide.flags      = MODE_FLAG_HAS_PER_LED_COLOR | MODE_FLAG_HAS_SPEED;;
    SolidReactiveWide.color_mode = MODE_COLORS_PER_LED;
    SolidReactiveWide.speed_min     = QMK_SPEED_SLOWEST;
    SolidReactiveWide.speed_max     = QMK_SPEED_FASTEST;
    SolidReactiveWide.speed         = QMK_SPEED_NORMAL;
    modes.push_back(SolidReactiveWide);*/

    SetupZones();
}

RGBController_QMKRGBMatrix::~RGBController_QMKRGBMatrix()
{
    for(unsigned int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            delete zones[zone_index].matrix_map;
        }
    }
}

void RGBController_QMKRGBMatrix::SetupZones()
{
    /*zone keyboard_zone;
    keyboard_zone.name           = zone_names[0];
    keyboard_zone.type           = zone_types[0];
    keyboard_zone.leds_min       = pid == 0 ? zone_sizes_alt[0] : zone_sizes_ctrl[0];
    keyboard_zone.leds_max       = pid == 0 ? zone_sizes_alt[0] : zone_sizes_ctrl[0];
    keyboard_zone.leds_count     = pid == 0 ? zone_sizes_alt[0] : zone_sizes_ctrl[0];
    keyboard_zone.matrix_map     = new matrix_map_type;
    keyboard_zone.matrix_map->height = pid == 0 ? 5 : 6;
    keyboard_zone.matrix_map->width  = pid == 0 ? 15 : 17;
    keyboard_zone.matrix_map->map    = pid == 0 ? (unsigned int *)&matrix_map_alt : (unsigned int *)&matrix_map_ctrl;
    zones.push_back(keyboard_zone);*/

    zone keyboard_zone;
    keyboard_zone.name           = "Keyboard";
    keyboard_zone.type           = ZONE_TYPE_MATRIX;
    keyboard_zone.leds_min       = 105;
    keyboard_zone.leds_max       = 105;
    keyboard_zone.leds_count     = 105;
    keyboard_zone.matrix_map     = new matrix_map_type;
    keyboard_zone.matrix_map->height = 7;
    keyboard_zone.matrix_map->width  = 17;
    keyboard_zone.matrix_map->map    = (unsigned int *)&matrix_map_alt_test;
    zones.push_back(keyboard_zone);

    for(std::size_t led_idx = 0; led_idx < zones[0].leds_count; led_idx++)
    {
        led new_led;
        if(led_idx > zone_sizes_alt[0] - 1)
        {
            new_led.name = "LightBar";
            new_led.name.append(std::to_string(led_idx + 1));        
        }
        else new_led.name = pid == 0 ? led_names_alt[led_idx] : led_names_ctrl[led_idx];
        leds.push_back(new_led);
    }

    /*zone lightbar_zone;
    lightbar_zone.name           = zone_names[1];
    lightbar_zone.type           = zone_types[1];
    lightbar_zone.leds_min       = pid == 0 ? zone_sizes_alt[1] : zone_sizes_ctrl[1];
    lightbar_zone.leds_max       = pid == 0 ? zone_sizes_alt[1] : zone_sizes_ctrl[1];
    lightbar_zone.leds_count     = pid == 0 ? zone_sizes_alt[1] : zone_sizes_ctrl[1];
    lightbar_zone.matrix_map     = NULL;
    zones.push_back(lightbar_zone);

    for(std::size_t led_idx = 0; led_idx < zones[1].leds_count; led_idx++)
    {
        led new_led;
        new_led.name = "LightBar";
        new_led.name.append(std::to_string(led_idx + 1));
        leds.push_back(new_led);
    }*/

    SetupColors();
}

void RGBController_QMKRGBMatrix::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_QMKRGBMatrix::DeviceUpdateLEDs()
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        qmk_rgb_matrix->SetLEDs(zone_idx, zones[zone_idx].colors, zones[zone_idx].leds_count);
    }
}

void RGBController_QMKRGBMatrix::UpdateZoneLEDs(int zone)
{
    qDebug() << "UpdateZoneLEDs" ;

    //DeviceUpdateLEDs();
}

void RGBController_QMKRGBMatrix::UpdateSingleLED(int led)
{
    qDebug() << "UpdateSingleLED" ;

    /*RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);
    mdalt->SingleLED(led, red, grn, blu);*/
}

void RGBController_QMKRGBMatrix::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_QMKRGBMatrix::DeviceUpdateMode()
{
    qmk_rgb_matrix->SetModeAndSpeed(modes[active_mode].value, modes[active_mode].speed);
}

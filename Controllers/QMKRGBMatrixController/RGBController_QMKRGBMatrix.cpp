/*-------------------------------------------------------------------*\
|  RGBController_QMKRGBMatrix.cpp                                     |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "hsv.h"
#include "RGBController_QMKRGBMatrix.h"

RGBController_QMKRGBMatrix::RGBController_QMKRGBMatrix(QMKRGBMatrixController* qmk_rgb_matrix_ptr)
{
    qmk_rgb_matrix = qmk_rgb_matrix_ptr;

    name = qmk_rgb_matrix_ptr->GetDeviceName();
    description = "QMK RGB Matrix Device";
    type = DEVICE_TYPE_KEYBOARD;
    location = qmk_rgb_matrix_ptr->GetLocation();
    version = qmk_rgb_matrix_ptr->GetQMKVersion();
    unsigned int hsv = qmk_rgb_matrix->GetHSV();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = QMK_RGBMATRIX_MODE_OPENRGB_DIRECT;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    mode SolidColor;
    SolidColor.name       = "Static";
    SolidColor.value      = QMK_RGBMATRIX_MODE_SOLID_COLOR;
    SolidColor.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidColor.colors_min = 1;
    SolidColor.colors_max = 1;
    SolidColor.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidColor.colors.resize(1);
    SolidColor.colors[0] = hsv;
    modes.push_back(SolidColor);

    mode AlphaMod;
    AlphaMod.name       = "Alpha Mod";
    AlphaMod.value      = QMK_RGBMATRIX_MODE_ALPHA_MOD;
    AlphaMod.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
    AlphaMod.color_mode = MODE_COLORS_PER_LED;
    AlphaMod.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    AlphaMod.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    AlphaMod.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(AlphaMod);

    mode GradientUpDown;
    GradientUpDown.name       = "Gradient Up Down";
    GradientUpDown.value      = QMK_RGBMATRIX_MODE_GRADIENT_UP_DOWN;
    GradientUpDown.flags      = 0;
    modes.push_back(GradientUpDown);

    mode GradientLeftRight;
    GradientLeftRight.name       = "Gradient Left Right";
    GradientLeftRight.value      = QMK_RGBMATRIX_MODE_GRADIENT_LEFT_RIGHT;
    GradientLeftRight.flags      = 0;
    modes.push_back(GradientLeftRight);

    mode Breathing;
    Breathing.name       = "Breathing";
    Breathing.value      = QMK_RGBMATRIX_MODE_BREATHING;
    Breathing.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    Breathing.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    Breathing.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    Breathing.colors_min = 1;
    Breathing.colors_max = 1;
    Breathing.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    Breathing.color_mode = MODE_COLORS_MODE_SPECIFIC;
    Breathing.colors.resize(1);
    Breathing.colors[0] = hsv;
    modes.push_back(Breathing);

    mode BandSat;
    BandSat.name       = "Band Saturation";
    BandSat.value      = QMK_RGBMATRIX_MODE_BAND_SAT;
    BandSat.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    BandSat.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    BandSat.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    BandSat.colors_min = 1;
    BandSat.colors_max = 1;
    BandSat.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    BandSat.color_mode = MODE_COLORS_MODE_SPECIFIC;
    BandSat.colors.resize(1);
    BandSat.colors[0] = hsv;
    modes.push_back(BandSat);

    mode BandVal;
    BandVal.name       = "Band Value";
    BandVal.value      = QMK_RGBMATRIX_MODE_BAND_VAL;
    BandVal.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    BandVal.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    BandVal.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    BandVal.colors_min = 1;
    BandVal.colors_max = 1;
    BandVal.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    BandVal.color_mode = MODE_COLORS_MODE_SPECIFIC;
    BandVal.colors.resize(1);
    BandVal.colors[0] = hsv;
    modes.push_back(BandVal);

    mode BandPinwheelSat;
    BandPinwheelSat.name       = "Band Pinwheel Saturation";
    BandPinwheelSat.value      = QMK_RGBMATRIX_MODE_BAND_PINWHEEL_SAT;
    BandPinwheelSat.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    BandPinwheelSat.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    BandPinwheelSat.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    BandPinwheelSat.colors_min = 1;
    BandPinwheelSat.colors_max = 1;
    BandPinwheelSat.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    BandPinwheelSat.color_mode = MODE_COLORS_MODE_SPECIFIC;
    BandPinwheelSat.colors.resize(1);
    BandPinwheelSat.colors[0] = hsv;
    modes.push_back(BandPinwheelSat);

    mode BandPinwheelVal;
    BandPinwheelVal.name       = "Band Pinwheel Value";
    BandPinwheelVal.value      = QMK_RGBMATRIX_MODE_BAND_PINWHEEL_VAL;
    BandPinwheelVal.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    BandPinwheelVal.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    BandPinwheelVal.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    BandPinwheelVal.colors_min = 1;
    BandPinwheelVal.colors_max = 1;
    BandPinwheelVal.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    BandPinwheelVal.color_mode = MODE_COLORS_MODE_SPECIFIC;
    BandPinwheelVal.colors.resize(1);
    BandPinwheelVal.colors[0] = hsv;
    modes.push_back(BandPinwheelVal);

    mode BandSpiralSat;
    BandSpiralSat.name       = "Band Spiral Saturation";
    BandSpiralSat.value      = QMK_RGBMATRIX_MODE_BAND_SPIRAL_SAT;
    BandSpiralSat.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    BandSpiralSat.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    BandSpiralSat.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    BandSpiralSat.colors_min = 1;
    BandSpiralSat.colors_max = 1;
    BandSpiralSat.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    BandSpiralSat.color_mode = MODE_COLORS_MODE_SPECIFIC;
    BandSpiralSat.colors.resize(1);
    BandSpiralSat.colors[0] = hsv;
    modes.push_back(BandSpiralSat);

    mode BandSpiralVal;
    BandSpiralVal.name       = "Band Spiral Value";
    BandSpiralVal.value      = QMK_RGBMATRIX_MODE_BAND_SPIRAL_VAL;
    BandSpiralVal.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    BandSpiralVal.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    BandSpiralVal.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    BandSpiralVal.colors_min = 1;
    BandSpiralVal.colors_max = 1;
    BandSpiralVal.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    BandSpiralVal.color_mode = MODE_COLORS_MODE_SPECIFIC;
    BandSpiralVal.colors.resize(1);
    BandSpiralVal.colors[0] = hsv;
    modes.push_back(BandSpiralVal);

    mode CycleAll;
    CycleAll.name          = "Cycle All";
    CycleAll.value         = QMK_RGBMATRIX_MODE_CYCLE_ALL;
    CycleAll.flags         = MODE_FLAG_HAS_SPEED;
    CycleAll.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    CycleAll.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    CycleAll.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(CycleAll);

    mode CycleLeftRight;
    CycleLeftRight.name          = "Cycle Left Right";
    CycleLeftRight.value         = QMK_RGBMATRIX_MODE_CYCLE_LEFT_RIGHT;
    CycleLeftRight.flags         = MODE_FLAG_HAS_SPEED;
    CycleLeftRight.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    CycleLeftRight.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    CycleLeftRight.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(CycleLeftRight);

    mode CycleUpDown;
    CycleUpDown.name          = "Cycle Up Down";
    CycleUpDown.value         = QMK_RGBMATRIX_MODE_CYCLE_UP_DOWN;
    CycleUpDown.flags         = MODE_FLAG_HAS_SPEED;
    CycleUpDown.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    CycleUpDown.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    CycleUpDown.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(CycleUpDown);

    mode RainbowOutIn;
    RainbowOutIn.name          = "Rainbow Out In";
    RainbowOutIn.value         = QMK_RGBMATRIX_MODE_CYCLE_OUT_IN;
    RainbowOutIn.flags         = MODE_FLAG_HAS_SPEED;
    RainbowOutIn.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowOutIn.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowOutIn.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowOutIn);

    mode RainbowOutInDual;
    RainbowOutInDual.name          = "Rainbow Out In Dual";
    RainbowOutInDual.value         = QMK_RGBMATRIX_MODE_CYCLE_OUT_IN_DUAL;
    RainbowOutInDual.flags         = MODE_FLAG_HAS_SPEED;
    RainbowOutInDual.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowOutInDual.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowOutInDual.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowOutInDual);

    mode RainbowMovingChevron;
    RainbowMovingChevron.name          = "Rainbow Moving Chevron";
    RainbowMovingChevron.value         = QMK_RGBMATRIX_MODE_RAINBOW_MOVING_CHEVRON;
    RainbowMovingChevron.flags         = MODE_FLAG_HAS_SPEED;
    RainbowMovingChevron.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowMovingChevron.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowMovingChevron.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowMovingChevron);

    mode CyclePinWheel;
    CyclePinWheel.name          = "Cycle Pinwheel";
    CyclePinWheel.value         = QMK_RGBMATRIX_MODE_CYCLE_PINWHEEL;
    CyclePinWheel.flags         = MODE_FLAG_HAS_SPEED;
    CyclePinWheel.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    CyclePinWheel.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    CyclePinWheel.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(CyclePinWheel);

    mode CycleSpiral;
    CycleSpiral.name          = "Cycle Spiral";
    CycleSpiral.value         = QMK_RGBMATRIX_MODE_CYCLE_SPIRAL;
    CycleSpiral.flags         = MODE_FLAG_HAS_SPEED;
    CycleSpiral.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    CycleSpiral.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    CycleSpiral.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(CycleSpiral);

    mode DualBeacon;
    DualBeacon.name          = "Dual Beacon";
    DualBeacon.value         = QMK_RGBMATRIX_MODE_DUAL_BEACON;
    DualBeacon.flags         = MODE_FLAG_HAS_SPEED;
    DualBeacon.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    DualBeacon.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    DualBeacon.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(DualBeacon);

    mode RainbowBeacon;
    RainbowBeacon.name          = "Rainbow Beacon";
    RainbowBeacon.value         = QMK_RGBMATRIX_MODE_RAINBOW_BEACON;
    RainbowBeacon.flags         = MODE_FLAG_HAS_SPEED;
    RainbowBeacon.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowBeacon.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowBeacon.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowBeacon);

    mode RainbowPinwheels;
    RainbowPinwheels.name          = "Rainbow Pinwheels";
    RainbowPinwheels.value         = QMK_RGBMATRIX_MODE_RAINBOW_PINWHEELS;
    RainbowPinwheels.flags         = MODE_FLAG_HAS_SPEED;
    RainbowPinwheels.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    RainbowPinwheels.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    RainbowPinwheels.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(RainbowPinwheels);

    mode Raindrops;
    Raindrops.name          = "Raindrops";
    Raindrops.value         = QMK_RGBMATRIX_MODE_RAINDROPS;
    Raindrops.flags         = 0;
    modes.push_back(Raindrops);

    mode JellybeanRaindrops;
    JellybeanRaindrops.name          = "Jellybean Raindrops";
    JellybeanRaindrops.value         = QMK_RGBMATRIX_MODE_JELLYBEAN_RAINDROPS;
    JellybeanRaindrops.flags         = 0;
    modes.push_back(JellybeanRaindrops);

    mode TypingHeatmap;
    TypingHeatmap.name          = "Typing Heatmap";
    TypingHeatmap.value         = QMK_RGBMATRIX_MODE_TYPING_HEATMAP;
    TypingHeatmap.flags         = 0;
    modes.push_back(TypingHeatmap);

    mode DigitalRain;
    DigitalRain.name          = "Digital Rain";
    DigitalRain.value         = QMK_RGBMATRIX_MODE_DIGITAL_RAIN;
    DigitalRain.flags         = 0;
    modes.push_back(DigitalRain);

    mode SolidReactiveSimple;
    SolidReactiveSimple.name       = "Solid Reactive Simple";
    SolidReactiveSimple.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_SIMPLE;
    SolidReactiveSimple.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveSimple.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveSimple.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveSimple.colors_min = 1;
    SolidReactiveSimple.colors_max = 1;
    SolidReactiveSimple.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveSimple.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveSimple.colors.resize(1);
    SolidReactiveSimple.colors[0] = hsv;
    modes.push_back(SolidReactiveSimple);

    mode SolidReactive;
    SolidReactive.name       = "Solid Reactive";
    SolidReactive.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE;
    SolidReactive.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactive.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactive.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactive.colors_min = 1;
    SolidReactive.colors_max = 1;
    SolidReactive.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactive.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactive.colors.resize(1);
    SolidReactive.colors[0] = hsv;
    modes.push_back(SolidReactive);

    mode SolidReactiveWide;
    SolidReactiveWide.name       = "Solid Reactive Wide";
    SolidReactiveWide.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_WIDE;
    SolidReactiveWide.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveWide.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveWide.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveWide.colors_min = 1;
    SolidReactiveWide.colors_max = 1;
    SolidReactiveWide.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveWide.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveWide.colors.resize(1);
    SolidReactiveWide.colors[0] = hsv;
    modes.push_back(SolidReactiveWide);


    mode SolidReactiveMultiWide;
    SolidReactiveMultiWide.name       = "Solid Reactive Multi Wide";
    SolidReactiveMultiWide.value      = QMK_RGBMATRIX_MODE_BREATHING;
    SolidReactiveMultiWide.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveMultiWide.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveMultiWide.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveMultiWide.colors_min = 1;
    SolidReactiveMultiWide.colors_max = 1;
    SolidReactiveMultiWide.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveMultiWide.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveMultiWide.colors.resize(1);
    SolidReactiveMultiWide.colors[0] = hsv;
    modes.push_back(SolidReactiveMultiWide);

    mode SolidReactiveCross;
    SolidReactiveCross.name       = "Solid Reactive Cross";
    SolidReactiveCross.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_CROSS;
    SolidReactiveCross.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveCross.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveCross.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveCross.colors_min = 1;
    SolidReactiveCross.colors_max = 1;
    SolidReactiveCross.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveCross.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveCross.colors.resize(1);
    SolidReactiveCross.colors[0] = hsv;
    modes.push_back(SolidReactiveCross);

    mode SolidReactiveMultiCross;
    SolidReactiveMultiCross.name       = "Solid Reactive Multi Cross";
    SolidReactiveMultiCross.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTICROSS;
    SolidReactiveMultiCross.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveMultiCross.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveMultiCross.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveMultiCross.colors_min = 1;
    SolidReactiveMultiCross.colors_max = 1;
    SolidReactiveMultiCross.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveMultiCross.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveMultiCross.colors.resize(1);
    SolidReactiveMultiCross.colors[0] = hsv;
    modes.push_back(SolidReactiveMultiCross);

    mode SolidReactiveNexus;
    SolidReactiveNexus.name       = "Solid Reactive Nexus";
    SolidReactiveNexus.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_NEXUS;
    SolidReactiveNexus.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveNexus.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveNexus.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveNexus.colors_min = 1;
    SolidReactiveNexus.colors_max = 1;
    SolidReactiveNexus.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveNexus.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveNexus.colors.resize(1);
    SolidReactiveNexus.colors[0] = hsv;
    SolidReactiveNexus.colors[0] = hsv;
    modes.push_back(SolidReactiveNexus);

    mode SolidReactiveMultiNexus;
    SolidReactiveMultiNexus.name       = "Solid Reactive Multi Nexus";
    SolidReactiveMultiNexus.value      = QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTINEXUS;
    SolidReactiveMultiNexus.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidReactiveMultiNexus.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidReactiveMultiNexus.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidReactiveMultiNexus.colors_min = 1;
    SolidReactiveMultiNexus.colors_max = 1;
    SolidReactiveMultiNexus.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidReactiveMultiNexus.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidReactiveMultiNexus.colors.resize(1);
    SolidReactiveMultiNexus.colors[0] = hsv;
    modes.push_back(SolidReactiveMultiNexus);

    mode Splash;
    Splash.name       = "Rainbow Reactive Splash";
    Splash.value      = QMK_RGBMATRIX_MODE_SPLASH;
    Splash.flags      = MODE_FLAG_HAS_SPEED;
    Splash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    Splash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    Splash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(Splash);

    mode MultiSplash;
    MultiSplash.name       = "Rainbow Reactive Multi Splash";
    MultiSplash.value      = QMK_RGBMATRIX_MODE_MULTISPLASH;
    MultiSplash.flags      = MODE_FLAG_HAS_SPEED;
    MultiSplash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    MultiSplash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    MultiSplash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    modes.push_back(MultiSplash);

    mode SolidSplash;
    SolidSplash.name       = "Solid Reactive Splash";
    SolidSplash.value      = QMK_RGBMATRIX_MODE_SOLID_SPLASH;
    SolidSplash.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidSplash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidSplash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidSplash.colors_min = 1;
    SolidSplash.colors_max = 1;
    SolidSplash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidSplash.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidSplash.colors.resize(1);
    SolidSplash.colors[0] = hsv;
    modes.push_back(SolidSplash);

    mode SolidMultiSplash;
    SolidMultiSplash.name       = "Solid Reactive Multi Splash";
    SolidMultiSplash.value      = QMK_RGBMATRIX_MODE_SOLID_MULTISPLASH;
    SolidMultiSplash.flags      = MODE_FLAG_HAS_SPEED | MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
    SolidMultiSplash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
    SolidMultiSplash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
    SolidMultiSplash.colors_min = 1;
    SolidMultiSplash.colors_max = 1;
    SolidMultiSplash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    SolidMultiSplash.color_mode = MODE_COLORS_MODE_SPECIFIC;
    SolidMultiSplash.colors.resize(1);
    SolidMultiSplash.colors[0] = hsv;
    modes.push_back(SolidMultiSplash);

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
    for(unsigned int i = 0; i < qmk_rgb_matrix->GetZonesCount(); i++)
    {
        zone keyboard_zone;
        keyboard_zone.name = qmk_rgb_matrix->GetZoneName(i);
        keyboard_zone.type = qmk_rgb_matrix->GetZoneType(i);
        keyboard_zone.leds_min = qmk_rgb_matrix->GetZoneSize(i);
        keyboard_zone.leds_max = keyboard_zone.leds_min;
        keyboard_zone.leds_count = keyboard_zone.leds_min;

        std::vector<std::string> led_names;
        if(keyboard_zone.type == ZONE_TYPE_MATRIX)
        {
            unsigned int led_matrix_columns = qmk_rgb_matrix->GetLEDMatirxColumns();
            unsigned int led_matrix_rows = qmk_rgb_matrix->GetLEDMatirxRows();

            keyboard_zone.matrix_map = new matrix_map_type;
            keyboard_zone.matrix_map->height = led_matrix_rows;
            keyboard_zone.matrix_map->width = led_matrix_columns;

            unsigned int* matrix_map = new unsigned int[led_matrix_rows * led_matrix_columns];
            for(unsigned int x = 0; x < led_matrix_rows; x++)
            {
                for(unsigned int y = 0; y < led_matrix_columns; y++)
                {
                    unsigned int led_value = qmk_rgb_matrix->GetLEDValueInMatrix(y, x);
                    matrix_map[led_matrix_columns * x + y] = led_value;

                    if(led_value != 255)
                    {
                        std::string led_name = qmk_rgb_matrix->GetLEDName(y, x);
                        if(led_name != "") 
                            led_names.push_back(led_name);
                    }
                }
            }

            keyboard_zone.matrix_map->map = matrix_map;
        }
        else 
        {
            keyboard_zone.matrix_map = NULL;
        }

        zones.push_back(keyboard_zone);

        for(unsigned int led_idx = 0; led_idx < keyboard_zone.leds_count; led_idx++)
        {
            led keyboard_led;

            if(leds.size() < zones[0].leds_count) 
                if(led_idx < led_names.size()) keyboard_led.name = "Key: " + led_names[led_idx];
            else 
                keyboard_led.name = zones[i].name + ": " + std::to_string(led_idx);

            leds.push_back(keyboard_led);
        }
    }

    SetupColors();

    for(unsigned int i = 0; i < leds.size(); i++)
    {
        colors[i] = qmk_rgb_matrix->GetLEDColor(i);
    }
}

void RGBController_QMKRGBMatrix::ResizeZone(int /*zone*/, int /*new_size*/)
{
    /*---------------------------------------------------------*\
    | This device does not support resizing zones               |
    \*---------------------------------------------------------*/
}

void RGBController_QMKRGBMatrix::DeviceUpdateLEDs()
{
    qmk_rgb_matrix->SetLEDs(colors, leds.size());
}

void RGBController_QMKRGBMatrix::UpdateZoneLEDs(int /*zone*/)
{
    DeviceUpdateLEDs();
}

void RGBController_QMKRGBMatrix::UpdateSingleLED(int led)
{
    RGBColor      color = colors[led];
    unsigned char red   = RGBGetRValue(color);
    unsigned char grn   = RGBGetGValue(color);
    unsigned char blu   = RGBGetBValue(color);
    qmk_rgb_matrix->SetSingleLED(led, red, grn, blu);
}

void RGBController_QMKRGBMatrix::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_QMKRGBMatrix::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        qmk_rgb_matrix->SetMode(modes[active_mode].value);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_NONE)
    {
        qmk_rgb_matrix->SetModeAndSpeed(modes[active_mode].value, modes[active_mode].speed);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        RGBColor color = modes[active_mode].colors[0];
        hsv_t hsv_color;
        rgb2hsv(color, &hsv_color);
        qmk_rgb_matrix->SetColorModeAndSpeed(hsv_color, modes[active_mode].value, modes[active_mode].speed);
    }
}


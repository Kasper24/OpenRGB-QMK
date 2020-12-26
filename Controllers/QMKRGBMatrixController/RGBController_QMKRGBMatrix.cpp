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

    std::vector<unsigned int> enabled_modes = qmk_rgb_matrix_ptr->GetEnabledModes();
    unsigned int current_mode = 1;
    unsigned int hsv = qmk_rgb_matrix_ptr->QMKModeGetColor();

    mode Direct;
    Direct.name       = "Direct";
    Direct.value      = 0;
    Direct.flags      = MODE_FLAG_HAS_PER_LED_COLOR;
    Direct.color_mode = MODE_COLORS_PER_LED;
    modes.push_back(Direct);

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_COLOR) != enabled_modes.end())
    {
        mode SolidColor;
        SolidColor.name       = "Static";
        SolidColor.value      = current_mode++;
        SolidColor.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
        SolidColor.colors_min = 1;
        SolidColor.colors_max = 1;
        SolidColor.color_mode = MODE_COLORS_MODE_SPECIFIC;
        SolidColor.colors.resize(1);
        SolidColor.colors[0] = hsv;
        modes.push_back(SolidColor);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_ALPHA_MOD) != enabled_modes.end())
    {
        mode AlphaMod;
        AlphaMod.name       = "Alpha Mod";
        AlphaMod.value      = current_mode++;;
        AlphaMod.flags      = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED;
        AlphaMod.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        AlphaMod.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        AlphaMod.colors_min = 1;
        AlphaMod.colors_max = 1;
        AlphaMod.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        AlphaMod.color_mode = MODE_FLAG_HAS_MODE_SPECIFIC_COLOR;
        AlphaMod.colors.resize(1);
        AlphaMod.colors[0] = hsv;
        modes.push_back(AlphaMod);
    }
    
    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_GRADIENT_UP_DOWN) != enabled_modes.end())
    {
        mode GradientUpDown;
        GradientUpDown.name          = "Gradient Up Down";
        GradientUpDown.value         = current_mode++;
        GradientUpDown.flags         = MODE_FLAG_HAS_SPEED;
        GradientUpDown.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        GradientUpDown.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        GradientUpDown.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(GradientUpDown);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_GRADIENT_LEFT_RIGHT) != enabled_modes.end())
    {
        mode GradientLeftRight;
        GradientLeftRight.name          = "Gradient Left Right";
        GradientLeftRight.value         = current_mode++;
        GradientLeftRight.flags         = MODE_FLAG_HAS_SPEED;
        GradientLeftRight.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        GradientLeftRight.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        GradientLeftRight.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(GradientLeftRight);
    }
    
    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_BREATHING) != enabled_modes.end())
    {
        mode Breathing;
        Breathing.name       = "Breathing";
        Breathing.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_BAND_SAT) != enabled_modes.end())
    {
        mode BandSat;
        BandSat.name       = "Band Saturation";
        BandSat.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_BAND_VAL) != enabled_modes.end())
    {
        mode BandVal;
        BandVal.name       = "Band Value";
        BandVal.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_BAND_PINWHEEL_SAT) != enabled_modes.end())
    {
        mode BandPinwheelSat;
        BandPinwheelSat.name       = "Band Pinwheel Saturation";
        BandPinwheelSat.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_BAND_PINWHEEL_VAL) != enabled_modes.end())
    {
        mode BandPinwheelVal;
        BandPinwheelVal.name       = "Band Pinwheel Value";
        BandPinwheelVal.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_BAND_SPIRAL_SAT) != enabled_modes.end())
    {
        mode BandSpiralSat;
        BandSpiralSat.name       = "Band Spiral Saturation";
        BandSpiralSat.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_BAND_SPIRAL_VAL) != enabled_modes.end())
    {
        mode BandSpiralVal;
        BandSpiralVal.name       = "Band Spiral Value";
        BandSpiralVal.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_CYCLE_ALL) != enabled_modes.end())
    {
        mode CycleAll;
        CycleAll.name          = "Cycle All";
        CycleAll.value         = current_mode++;
        CycleAll.flags         = MODE_FLAG_HAS_SPEED;
        CycleAll.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        CycleAll.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        CycleAll.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(CycleAll);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_CYCLE_LEFT_RIGHT) != enabled_modes.end())
    {
        mode CycleLeftRight;
        CycleLeftRight.name          = "Cycle Left Right";
        CycleLeftRight.value         = current_mode++;
        CycleLeftRight.flags         = MODE_FLAG_HAS_SPEED;
        CycleLeftRight.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        CycleLeftRight.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        CycleLeftRight.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(CycleLeftRight);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_CYCLE_UP_DOWN) != enabled_modes.end())
    {
        mode CycleUpDown;
        CycleUpDown.name          = "Cycle Up Down";
        CycleUpDown.value         = current_mode++;
        CycleUpDown.flags         = MODE_FLAG_HAS_SPEED;
        CycleUpDown.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        CycleUpDown.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        CycleUpDown.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(CycleUpDown);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_CYCLE_OUT_IN) != enabled_modes.end())
    {
        mode RainbowOutIn;
        RainbowOutIn.name          = "Rainbow Out In";
        RainbowOutIn.value         = current_mode++;
        RainbowOutIn.flags         = MODE_FLAG_HAS_SPEED;
        RainbowOutIn.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        RainbowOutIn.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        RainbowOutIn.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(RainbowOutIn);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_CYCLE_OUT_IN_DUAL) != enabled_modes.end())
    {
        mode RainbowOutInDual;
        RainbowOutInDual.name          = "Rainbow Out In Dual";
        RainbowOutInDual.value         = current_mode++;
        RainbowOutInDual.flags         = MODE_FLAG_HAS_SPEED;
        RainbowOutInDual.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        RainbowOutInDual.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        RainbowOutInDual.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(RainbowOutInDual);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_RAINBOW_MOVING_CHEVRON) != enabled_modes.end())
    {
        mode RainbowMovingChevron;
        RainbowMovingChevron.name          = "Rainbow Moving Chevron";
        RainbowMovingChevron.value         = current_mode++;
        RainbowMovingChevron.flags         = MODE_FLAG_HAS_SPEED;
        RainbowMovingChevron.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        RainbowMovingChevron.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        RainbowMovingChevron.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(RainbowMovingChevron);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_CYCLE_PINWHEEL) != enabled_modes.end())
    {
        mode CyclePinWheel;
        CyclePinWheel.name          = "Cycle Pinwheel";
        CyclePinWheel.value         = current_mode++;
        CyclePinWheel.flags         = MODE_FLAG_HAS_SPEED;
        CyclePinWheel.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        CyclePinWheel.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        CyclePinWheel.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(CyclePinWheel);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_CYCLE_SPIRAL) != enabled_modes.end())
    {
        mode CycleSpiral;
        CycleSpiral.name          = "Cycle Spiral";
        CycleSpiral.value         = current_mode++;
        CycleSpiral.flags         = MODE_FLAG_HAS_SPEED;
        CycleSpiral.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        CycleSpiral.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        CycleSpiral.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(CycleSpiral);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_DUAL_BEACON) != enabled_modes.end())
    {
        mode DualBeacon;
        DualBeacon.name          = "Dual Beacon";
        DualBeacon.value         = current_mode++;
        DualBeacon.flags         = MODE_FLAG_HAS_SPEED;
        DualBeacon.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        DualBeacon.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        DualBeacon.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(DualBeacon);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_RAINBOW_BEACON) != enabled_modes.end())
    {
        mode RainbowBeacon;
        RainbowBeacon.name          = "Rainbow Beacon";
        RainbowBeacon.value         = current_mode++;
        RainbowBeacon.flags         = MODE_FLAG_HAS_SPEED;
        RainbowBeacon.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        RainbowBeacon.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        RainbowBeacon.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(RainbowBeacon);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_RAINBOW_PINWHEELS) != enabled_modes.end())
    {
        mode RainbowPinwheels;
        RainbowPinwheels.name          = "Rainbow Pinwheels";
        RainbowPinwheels.value         = current_mode++;
        RainbowPinwheels.flags         = MODE_FLAG_HAS_SPEED;
        RainbowPinwheels.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        RainbowPinwheels.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        RainbowPinwheels.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(RainbowPinwheels);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_RAINDROPS) != enabled_modes.end())
    {
        mode Raindrops;
        Raindrops.name          = "Raindrops";
        Raindrops.value         = current_mode++;
        Raindrops.flags         = 0;
        modes.push_back(Raindrops);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_JELLYBEAN_RAINDROPS) != enabled_modes.end())
    {
        mode JellybeanRaindrops;
        JellybeanRaindrops.name          = "Jellybean Raindrops";
        JellybeanRaindrops.value         = current_mode++;
        JellybeanRaindrops.flags         = 0;
        modes.push_back(JellybeanRaindrops);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_TYPING_HEATMAP) != enabled_modes.end())
    {
        mode TypingHeatmap;
        TypingHeatmap.name          = "Typing Heatmap";
        TypingHeatmap.value         = current_mode++;
        TypingHeatmap.flags         = 0;
        modes.push_back(TypingHeatmap);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_TYPING_HEATMAP) != enabled_modes.end())
    {
        mode DigitalRain;
        DigitalRain.name          = "Digital Rain";
        DigitalRain.value         = current_mode++;
        DigitalRain.flags         = 0;
        modes.push_back(DigitalRain);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_REACTIVE_SIMPLE) != enabled_modes.end())
    {
        mode SolidReactiveSimple;
        SolidReactiveSimple.name       = "Solid Reactive Simple";
        SolidReactiveSimple.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_REACTIVE) != enabled_modes.end())
    {
        mode SolidReactive;
        SolidReactive.name       = "Solid Reactive";
        SolidReactive.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_REACTIVE_WIDE) != enabled_modes.end())
    {
        mode SolidReactiveWide;
        SolidReactiveWide.name       = "Solid Reactive Wide";
        SolidReactiveWide.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTIWIDE) != enabled_modes.end())
    {
        mode SolidReactiveMultiWide;
        SolidReactiveMultiWide.name       = "Solid Reactive Multi Wide";
        SolidReactiveMultiWide.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_REACTIVE_CROSS) != enabled_modes.end())
    {
        mode SolidReactiveCross;
        SolidReactiveCross.name       = "Solid Reactive Cross";
        SolidReactiveCross.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTICROSS) != enabled_modes.end())
    {
        mode SolidReactiveMultiCross;
        SolidReactiveMultiCross.name       = "Solid Reactive Multi Cross";
        SolidReactiveMultiCross.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_REACTIVE_NEXUS) != enabled_modes.end())
    {
        mode SolidReactiveNexus;
        SolidReactiveNexus.name       = "Solid Reactive Nexus";
        SolidReactiveNexus.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTINEXUS) != enabled_modes.end())
    {
        mode SolidReactiveMultiNexus;
        SolidReactiveMultiNexus.name       = "Solid Reactive Multi Nexus";
        SolidReactiveMultiNexus.value      = current_mode++;
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

    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SPLASH) != enabled_modes.end())
    {
        mode Splash;
        Splash.name       = "Rainbow Reactive Splash";
        Splash.value      = current_mode++;
        Splash.flags      = MODE_FLAG_HAS_SPEED;
        Splash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        Splash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        Splash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(Splash);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_MULTISPLASH) != enabled_modes.end())
    {
        mode MultiSplash;
        MultiSplash.name       = "Rainbow Reactive Multi Splash";
        MultiSplash.value      = current_mode++;
        MultiSplash.flags      = MODE_FLAG_HAS_SPEED;
        MultiSplash.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        MultiSplash.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        MultiSplash.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
        modes.push_back(MultiSplash);
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_SPLASH) != enabled_modes.end())
    {
        mode SolidSplash;
        SolidSplash.name       = "Solid Reactive Splash";
        SolidSplash.value      = current_mode++;
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
    }

    if (std::find(enabled_modes.begin(), enabled_modes.end(), QMK_RGBMATRIX_MODE_SOLID_MULTISPLASH) != enabled_modes.end())
    {
        mode SolidMultiSplash;
        SolidMultiSplash.name       = "Solid Reactive Multi Splash";
        SolidMultiSplash.value      = current_mode++;
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
    }

    active_mode = qmk_rgb_matrix_ptr->GetActiveMode() - 1;

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
        colors[i] = qmk_rgb_matrix->DirectModeGetLEDColor(i);
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
    /* 
        // There is an issue where you need to press 'Apply' multiple times in order for the colors to
        // render correctly on the keyboard. This is because on direct mode I'm disabling the rgb_matrix code
        // to workaround an issue where the keyboard locks up if ORGB sends more than 2 leds per update
        // to prevent over-flushing the pwm buffer (which would lock up the keyboard as well), 
        // there is a small delay, and if that delay hasn't elapsed yet then the pwm buffers will simply not flush
        // making the keyboard leds not update untiil the next time you press apply  
    */
    qmk_rgb_matrix->DirectModeSetLEDs(colors, leds.size());
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
    qmk_rgb_matrix->DirectModeSetSingleLED(led, red, grn, blu);
}

void RGBController_QMKRGBMatrix::SetCustomMode()
{
    active_mode = 0;
}

void RGBController_QMKRGBMatrix::DeviceUpdateMode()
{
    if(modes[active_mode].color_mode == MODE_COLORS_PER_LED)
    {
        DeviceUpdateLEDs();
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_NONE)
    {
        qmk_rgb_matrix->QMKModeSetModeAndSpeed(modes[active_mode].value, modes[active_mode].speed);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        RGBColor color = modes[active_mode].colors[0];
        hsv_t hsv_color;
        rgb2hsv(color, &hsv_color);
        qmk_rgb_matrix->QMKModeSetColorModeAndSpeed(hsv_color, modes[active_mode].value, modes[active_mode].speed);
    }
}


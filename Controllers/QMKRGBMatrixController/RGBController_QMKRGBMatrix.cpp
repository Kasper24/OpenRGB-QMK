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

static std::map<uint8_t, std::string> QMKKeycodeToKeynameMap
{
    { 0, "" }, { 1, "Right Fn" }, { 2, "" }, { 3, "" },
    { 4, "A" }, { 5, "B" }, { 6, "C" }, { 7, "D" },
    { 8, "E" }, { 9, "F" }, { 10, "G" }, { 11, "H" },
    { 12, "I" }, { 13, "J" }, { 14, "K" }, { 15, "L" },
    { 16, "M" }, { 17, "N" }, { 18, "O" }, { 19, "P" },
    { 20, "Q" }, { 21, "R" }, { 22, "S" }, { 23, "T" },
    { 24, "U" }, { 25, "V" }, { 26, "W" }, { 27, "X" },
    { 28, "Y" }, { 29, "Z" }, { 30, "1" }, { 31, "2" },
    { 32, "3" }, { 33, "4" }, { 34, "5" },{ 35, "6" },
    { 36, "7" }, { 37, "8" }, { 38, "9" }, { 39, "0" },
    { 40, "Enter" }, { 41, "Escape" }, { 42, "Backspace" }, { 43, "Tab" },
    { 44, "Space" }, { 45, "-" }, { 46, "=" }, { 47, "[" },
    { 48, "]" }, { 49, "\\ (ANSI)" }, { 50, "" }, { 51, ";" },
    { 52, "'" }, { 53, "`" }, { 54, "," }, { 55, "." },
    { 56, "/" }, { 57, "Caps Lock" }, { 58, "F1" }, { 59, "F2" },
    { 60, "F3" }, { 61, "F4" }, { 62, "F5" }, { 63, "F6" },
    { 64, "F7" }, { 65, "F8" }, { 66, "F9" }, { 67, "F10" },
    { 68, "F11" }, { 69, "F12" }, { 70, "Print Screen" }, { 71, "Scroll Lock" },
    { 72, "Pause/Break" }, { 73, "Insert" }, { 74, "Home" }, { 75, "Page Up" },
    { 76, "Delete" }, { 77, "End" }, { 78, "Page Down" }, { 79, "Right Arrow" },
    { 80, "Left Arrow" }, { 81, "Down Arrow" }, { 82, "Up Arrow" }, { 83, "Num Lock" },
    { 84, "Number Pad /" }, { 85, "Number Pad *" }, { 86, "Number Pad -" }, { 87, "Number Pad +" },
    { 88, "Number Pad Enter" }, { 89, "Number Pad 1" }, { 90, "Number Pad 2" }, { 91, "Number Pad 3" },
    { 92, "Number Pad 4" }, { 93, "Number Pad 5" }, { 94, "Number Pad 6" }, { 95, "Number Pad 7" },
    { 96, "Number Pad 8" }, { 97, "Number Pad 9" }, { 98, "Number Pad 0" }, { 99, "Number Pad ." },
    { 100, "" }, { 101, "Menu" }, { 102, "" }, { 103, "" },
    { 104, "" }, { 105, "" }, { 106, "" }, { 107, "" },
    { 108, "" }, { 109, "" }, { 110, "" }, { 111, "" },
    { 112, "" }, { 113, "" }, { 114, "" }, { 115, "" },
    { 116, "" }, { 117, "" }, { 118, "" }, { 119, "" },
    { 120, "" }, { 121, "" }, { 122, "" }, { 123, "" },
    { 124, "" }, { 125, "" }, { 126, "" }, { 127, "" },
    { 128, "" }, { 129, "" }, { 130, "" }, { 131, "" },
    { 132, "" }, { 133, "" }, { 134, "" }, { 135, "" },
    { 136, "" }, { 137, "" }, { 138, "" }, { 139, "" },
    { 140, "" }, { 141, "" }, { 142, "" }, { 143, "" },
    { 144, "" }, { 145, "" }, { 146, "" }, { 147, "" },
    { 148, "" }, { 149, "" }, { 150, "" }, { 151, "" },
    { 152, "" }, { 153, "" }, { 154, "" }, { 155, "" },
    { 156, "" }, { 157, "" }, { 158, "" }, { 159, "" },
    { 160, "" }, { 161, "" }, { 162, "" }, { 163, "" }, { 164, "" },
    /*Space Cadet Left Shift*/ { 216, "Left Shift"}, /*Space Cadet Right Shift*/{ 217, "Right Shift"},
    { 224, "Left Control" }, { 225, "Left Shift" }, { 226, "Left Alt" }, { 227, "Left Windows" },
    { 228, "Right Control" }, { 229, "Right Shift" }, { 230, "Right Alt" }, { 231, "Right Windows" },
};

RGBController_QMKRGBMatrix::RGBController_QMKRGBMatrix(QMKRGBMatrixController* qmk_rgb_matrix_ptr, unsigned int protocol_version)
{
    qmk_rgb_matrix = qmk_rgb_matrix_ptr;

    name = qmk_rgb_matrix->GetDeviceName();
    vendor = qmk_rgb_matrix->GetDeviceVendor();
    description = "QMK RGB Matrix Device";
    type = DEVICE_TYPE_KEYBOARD;
    location = qmk_rgb_matrix->GetLocation();
    version = std::to_string(protocol_version);

    unsigned int current_mode = 1;
    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_OPENRGB_DIRECT))
        InitializeMode("Direct", current_mode, MODE_FLAG_HAS_PER_LED_COLOR, MODE_COLORS_PER_LED);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_COLOR))
        InitializeMode("Static", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR, MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_ALPHA_MOD))
        InitializeMode("Alpha Mod", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_GRADIENT_UP_DOWN))
        InitializeMode("Gradient Up Down", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_GRADIENT_LEFT_RIGHT))
        InitializeMode("Gradient Left Right", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_BREATHING))
        InitializeMode("Breathing", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_BAND_SAT))
        InitializeMode("Band Saturation", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_BAND_VAL))
        InitializeMode("Band Value", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_BAND_PINWHEEL_SAT))
        InitializeMode("Band Pinwheel Saturation", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_BAND_PINWHEEL_VAL))
        InitializeMode("Band Pinwheel Value", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_BAND_SPIRAL_SAT))
        InitializeMode("Band Spiral Saturation", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_BAND_SPIRAL_VAL))
        InitializeMode("Band Spiral Value", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_CYCLE_ALL))
        InitializeMode("Cycle All", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_CYCLE_LEFT_RIGHT))
        InitializeMode("Cycle Left Right", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_CYCLE_UP_DOWN))
        InitializeMode("Cycle Up Down", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_CYCLE_OUT_IN))
        InitializeMode("Cycle Out In", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_CYCLE_OUT_IN_DUAL))
        InitializeMode("Cycle Out In Dual", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_RAINBOW_MOVING_CHEVRON))
        InitializeMode("Rainbow Moving Chevron", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_CYCLE_PINWHEEL))
        InitializeMode("Cycle Pinwheel", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_CYCLE_SPIRAL))
        InitializeMode("Cycle Spiral", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_DUAL_BEACON))
        InitializeMode("Dual Beacon", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_RAINBOW_BEACON))
        InitializeMode("Rainbow Beacon", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_RAINBOW_PINWHEELS))
        InitializeMode("Rainbow Pinwheels", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_RAINDROPS))
        InitializeMode("Raindrops", current_mode, 0, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_JELLYBEAN_RAINDROPS))
        InitializeMode("Jellybean Raindrops", current_mode, 0, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_TYPING_HEATMAP))
        InitializeMode("Typing Heatmap", current_mode, 0, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_DIGITAL_RAIN))
        InitializeMode("Digital Rain", current_mode, 0, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_SIMPLE))
        InitializeMode("Solid Reactive Simple", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_REACTIVE))
        InitializeMode("Solid Reactive", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_WIDE))
        InitializeMode("Solid Reactive Wide", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTIWIDE))
        InitializeMode("Solid Reactive Multi Wide", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_CROSS))
        InitializeMode("Solid Reactive Cross", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTICROSS))
        InitializeMode("Solid Reactive Multi Cross", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_NEXUS))
        InitializeMode("Solid Reactive Nexus", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_REACTIVE_MULTINEXUS))
        InitializeMode("Solid Reactive Multi Nexus", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SPLASH))
        InitializeMode("Rainbow Reactive Splash", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_MULTISPLASH))
        InitializeMode("Rainbow Reactive Multi Splash", current_mode, MODE_FLAG_HAS_SPEED, MODE_COLORS_NONE);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_SPLASH))
        InitializeMode("Solid Reactive Splash", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    if (qmk_rgb_matrix->GetIsModeEnabled(QMK_RGBMATRIX_MODE_SOLID_MULTISPLASH))
        InitializeMode("Solid Reactive Multi Splash", current_mode, MODE_FLAG_HAS_MODE_SPECIFIC_COLOR | MODE_FLAG_HAS_SPEED,
            MODE_COLORS_MODE_SPECIFIC);

    active_mode = qmk_rgb_matrix->GetMode() - 1;

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

void RGBController_QMKRGBMatrix::InitializeMode
    (
    std::string name,
    unsigned int &current_mode,
    unsigned int flags,
    unsigned int color_mode
    )
{
    mode qmk_mode;
    qmk_mode.name       = name;
    qmk_mode.value      = current_mode++;
    qmk_mode.flags      = flags;
    qmk_mode.color_mode =  color_mode;

    if(flags & MODE_FLAG_HAS_SPEED)
    {
        qmk_mode.speed_min     = QMK_RGBMATRIX_SPEED_SLOWEST;
        qmk_mode.speed_max     = QMK_RGBMATRIX_SPEED_FASTEST;
        qmk_mode.speed         = QMK_RGBMATRIX_SPEED_NORMAL;
    }
    if(flags & MODE_FLAG_HAS_MODE_SPECIFIC_COLOR)
    {
        qmk_mode.colors_min = 1;
        qmk_mode.colors_max = 1;
        qmk_mode.colors.resize(1);
        qmk_mode.colors[0] = qmk_rgb_matrix->GetModeColor();
    }

    modes.push_back(qmk_mode);
}

void RGBController_QMKRGBMatrix::SetupZones()
{
    for(unsigned int i = 0; i < qmk_rgb_matrix->GetZonesCount(); i++)
    {
        qmk_rgb_matrix->GetZoneInfo(i);

        zone keyboard_zone;
        keyboard_zone.name = qmk_rgb_matrix->GetCurrentZoneName();
        keyboard_zone.type = qmk_rgb_matrix->GetCurrentZoneType();
        keyboard_zone.leds_min = qmk_rgb_matrix->GetCurrentZoneSize();
        keyboard_zone.leds_max = keyboard_zone.leds_min;
        keyboard_zone.leds_count = keyboard_zone.leds_min;

        std::vector<std::string> led_names;
        if(keyboard_zone.type == ZONE_TYPE_MATRIX)
            SetupMatrix(keyboard_zone, led_names);
        else
            keyboard_zone.matrix_map = NULL;

        zones.push_back(keyboard_zone);
        SetupLEDs(led_names, i, keyboard_zone.leds_count);
    }

    SetupColors();
    GetInitialLEDColors();
}

void RGBController_QMKRGBMatrix::SetupMatrix(zone &keyboard_zone, std::vector<std::string> &led_names)
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
            if(led_value != 255)
            {
                unsigned int keycode = qmk_rgb_matrix->GetLEDName(y, x);
                if(keycode != 255)
                {
                    led_names.push_back(QMKKeycodeToKeynameMap[keycode]);
                }
            }
            else
            {
                led_value = 0xFFFFFFFF;
            }
            matrix_map[led_matrix_columns * x + y] = led_value;
        }
    }

    keyboard_zone.matrix_map->map = matrix_map;
}

void RGBController_QMKRGBMatrix::SetupLEDs
    (
    std::vector<std::string> &led_names,
    unsigned int zone_idx,
    unsigned int zone_led_count
    )
{
    for(unsigned int led_idx = 0; led_idx < zone_led_count; led_idx++)
    {
        led keyboard_led;
        if(leds.size() < led_names.size())
        {
            if(led_idx < led_names.size())
                keyboard_led.name = "Key: " + led_names[led_idx];
        }
        else
        {
            keyboard_led.name = zones[zone_idx].name + ": " + std::to_string(led_idx);
        }

        leds.push_back(keyboard_led);
    }
}

void RGBController_QMKRGBMatrix::GetInitialLEDColors()
{
    for(unsigned int i = 0; i < leds.size(); i++)
    {
        colors[i] = qmk_rgb_matrix->GetDirectModeLEDColor(i);
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
        qmk_rgb_matrix->SetMode({ 0, 255, 255 }, modes[active_mode].value, 127);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_NONE)
    {
        qmk_rgb_matrix->SetMode({ 0, 255, 255 }, modes[active_mode].value, modes[active_mode].speed);
    }
    else if(modes[active_mode].color_mode == MODE_COLORS_MODE_SPECIFIC)
    {
        RGBColor rgb_color = modes[active_mode].colors[0];
        hsv_t hsv_color;
        rgb2hsv(rgb_color, &hsv_color);

        if(modes[active_mode].flags & MODE_FLAG_HAS_SPEED)
            qmk_rgb_matrix->SetMode(hsv_color, modes[active_mode].value, modes[active_mode].speed);
        else
            qmk_rgb_matrix->SetMode(hsv_color, modes[active_mode].value, 127);
    }
}

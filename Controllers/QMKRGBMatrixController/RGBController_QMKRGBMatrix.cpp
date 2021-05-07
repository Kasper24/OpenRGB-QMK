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

#include <iostream>
#include <set>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <cmath>
#include <QDebug>

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
    const unsigned int number_of_key_leds = qmk_rgb_matrix->GetNumberOfKeyLEDs();
    const unsigned int number_of_underglow_leds = qmk_rgb_matrix->GetNumberOfUnderglowLEDs();
    const unsigned int number_of_leds = number_of_key_leds + number_of_underglow_leds;
    for (int i = 0; i < number_of_leds; i++)
    {
        qmk_rgb_matrix->GetLEDInfo(i);
    }

    std::vector<point_t> led_points = qmk_rgb_matrix->GetLEDPoints();
    std::vector<unsigned int> led_flags = qmk_rgb_matrix->GetLEDFlags();
    std::vector<std::string> led_names = qmk_rgb_matrix->GetLEDNames();

    std::set<int> rows, columns;
    for (unsigned int i = 0; i < number_of_leds; i++)
    {
        rows.insert(led_points[i].y);
        columns.insert(led_points[i].x);
    }
    unsigned int divisor = CalculateDivisor(led_points, rows, columns);

    unsigned int matrix_map_xl[rows.size()][columns.size()];
    unsigned int underglow_map_xl[rows.size()][columns.size()];
    for (int i = 0; i < rows.size(); i++)
    {
        for (int j = 0; j < columns.size(); j++)
        {
            matrix_map_xl[i][j] = 255;
            underglow_map_xl[i][j] = 255;
        }
    }

    int x, y;
    for (int i = 0; i < number_of_leds; i++)
    {
        if (led_points[i].x != 255 && led_points[i].y != 255)
        {
            bool underglow = led_flags[i] & 2;
            x = std::round(led_points[i].x/divisor);
            y = std::distance(rows.begin(), rows.find(led_points[i].y));
            if (!underglow)
            {
                while (matrix_map_xl[y][x] != 255)
                {
                    x++;
                }
                matrix_map_xl[y][x] = i;
            }
            else
            {
                while (underglow_map_xl[y][x] != 255)
                {
                    x++;
                }
                underglow_map_xl[y][x] = i;
            }
        }
    }

    bool empty_col = true, empty_col_udg = true, empty_row = true;
    int width = 0, width_udg = 0;
    std::vector<int> empty_rows;
    bool can_break;
    for (int i = 0; i < rows.size(); i++)
    {
        empty_row = true;
        for (std::size_t j = (sizeof matrix_map_xl[i] / sizeof *matrix_map_xl[i]) - 1; j --> 0; )
        {
            can_break = false;
            if (matrix_map_xl[i][j] != 255 && width < (j + 1))
            {
                width = j + 1;
                can_break = true;
                empty_row = false;
            }
            else if (matrix_map_xl[i][j] != 255) empty_row = false;
            if (underglow_map_xl[i][j] != 255 && width_udg < (j + 1))
            {
                width_udg = j + 1;
                if (can_break) break;
            }

        }
        if (matrix_map_xl[i][0] != 255) empty_col = false;
        if (underglow_map_xl[i][0] != 255) empty_col_udg = false;
        if (empty_row) empty_rows.push_back(i);
    }

    unsigned int* matrix_map = new unsigned int[(rows.size() - empty_rows.size()) *(empty_col ? width - 1 : width)];
    unsigned int* underglow_map = new unsigned int[rows.size() * (empty_col_udg ? width_udg - 1 : width_udg)];
    int s = 0;
    for (int i = 0; i < rows.size(); i++)
    {
        if (std::find(empty_rows.begin(), empty_rows.end(), i) != empty_rows.end())
        {
            s++;
            continue;
        }
        for (int j = 0; j < width; j++)
        {
            matrix_map[(empty_col ? width - 1 : width) * (i - s) + j] = matrix_map_xl[i][j];
        }
    }
    for (int i = 0; i < rows.size(); i++)
    {
        for (int j = 0; j < width_udg; j++)
        {
            underglow_map[(empty_col_udg ? width_udg - 1 : width_udg) * i + j] = underglow_map_xl[i][j];
            qDebug() << underglow_map[(empty_col_udg ? width_udg - 1 : width_udg) * i + j];
        }
    }

    zone keys_zone;
    keys_zone.name = "Keys";
    keys_zone.type = ZONE_TYPE_MATRIX;
    keys_zone.leds_min = number_of_key_leds;
    keys_zone.leds_max = number_of_key_leds;
    keys_zone.leds_count = number_of_key_leds;
    keys_zone.matrix_map = new matrix_map_type;
    keys_zone.matrix_map->height = rows.size() - empty_rows.size();
    keys_zone.matrix_map->width = empty_col ? width - 1 : width;
    keys_zone.matrix_map->map = matrix_map;
    zones.push_back(keys_zone);

    if(number_of_underglow_leds > 0)
    {
        zone underglow_zone;
        underglow_zone.name = "Underglow";
        underglow_zone.type = ZONE_TYPE_MATRIX;
        underglow_zone.leds_min = number_of_underglow_leds;
        underglow_zone.leds_max = number_of_underglow_leds;
        underglow_zone.leds_count = number_of_underglow_leds;
        underglow_zone.matrix_map = new matrix_map_type;
        underglow_zone.matrix_map->height = rows.size();
        underglow_zone.matrix_map->width = empty_col_udg ? width_udg - 1 : width_udg;
        underglow_zone.matrix_map->map = underglow_map;
        zones.push_back(underglow_zone);
    }

    for(unsigned int led_idx = 0; led_idx < number_of_leds; led_idx++)
    {
         led keyboard_led;
         keyboard_led.name = led_names[led_idx];
         leds.push_back(keyboard_led);
    }

    SetupColors();
    GetInitialLEDColors();
}

unsigned int RGBController_QMKRGBMatrix::CalculateDivisor
    (
    std::vector<point_t> led_points,
    std::set<int> rows,
    std::set<int> columns
    )
{
    std::vector<std::vector<point_t>> row_points(rows.size());
    for (const auto &pt : led_points)
    {
        for (const auto &i : rows)
        {
            if (pt.y == i)
            {
                row_points[std::distance(rows.begin(), rows.find(i))].push_back(pt);
            }
        }
    }

    int last_pos;
    std::vector<int> distances;
    for (const auto &row : row_points)
    {
        last_pos = 0;
        std::for_each(row.begin(), row.end(), [&distances, &last_pos](const point_t &pt)
        {
            distances.push_back(pt.x - last_pos);
            last_pos = pt.x;
        });
    }
    std::map<int, int> counts;
    for (const auto &i : distances)
    {
        counts[i]++;
    }

    unsigned int divisor = distances[0];
    for (const auto &i : counts)
    {
        if (counts[divisor] < i.second)
        {
            divisor = i.first;
        }
    }
    return divisor;
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

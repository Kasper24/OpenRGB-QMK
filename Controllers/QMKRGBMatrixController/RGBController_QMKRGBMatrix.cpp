/*-------------------------------------------------------------------*\
|  RGBController_QMKRGBMatrix.cpp                                     |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|  Jath03           28th May 2021                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "hsv.h"
#include "RGBController_QMKRGBMatrix.h"

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

void RGBController_QMKRGBMatrix::SetupZones()
{
    const unsigned int total_number_of_leds = qmk_rgb_matrix->GetTotalNumberOfLEDs();

    for (int i = 0; i < total_number_of_leds; i++)
    {
        qmk_rgb_matrix->GetLEDInfo(i);
    }

    std::vector<point_t> led_points = qmk_rgb_matrix->GetLEDPoints();
    std::vector<unsigned int> led_flags = qmk_rgb_matrix->GetLEDFlags();
    std::vector<std::string> led_names = qmk_rgb_matrix->GetLEDNames();

    std::pair<unsigned int, unsigned int> uint_pair = CountKeyTypes(led_flags, total_number_of_leds);
    const unsigned int number_of_key_leds = uint_pair.first;
    const unsigned int number_of_underglow_leds = uint_pair.second;
    const unsigned int number_of_leds = number_of_key_leds + number_of_underglow_leds;

    for(unsigned int i = 0; i < number_of_underglow_leds; i++)
    {
        led_names.push_back("Underglow: " + std::to_string(number_of_key_leds + i));
    }

    std::set<int> rows, columns;
    for (unsigned int i = 0; i < number_of_leds; i++)
    {
        rows.insert(led_points[i].y);
        columns.insert(led_points[i].x);
    }
    unsigned int divisor = CalculateDivisor(led_points, rows, columns);

    std::pair<VectorMatrix, VectorMatrix> map_pair = PlaceLEDsInMaps(rows, columns, divisor, led_points, led_flags);
    VectorMatrix matrix_map = map_pair.first;
    VectorMatrix underglow_map = map_pair.second;

    std::pair<VectorMatrix, VectorMatrix> map_pair2 = CleanMatrixMaps(matrix_map, underglow_map, rows.size());
    VectorMatrix new_matrix_map = map_pair2.first;
    VectorMatrix new_underglow_map = map_pair2.second;

    // These vectors are class members because if they go out of scope, the
    // underlying array (used by each zones' matrix_map) is unallocated.
    flat_matrix_map = FlattenMatrixMap(new_matrix_map);
    flat_underglow_map = FlattenMatrixMap(new_underglow_map);

    zone keys_zone;
    keys_zone.name = "Keys";
    keys_zone.type = ZONE_TYPE_MATRIX;
    keys_zone.leds_min = number_of_key_leds;
    keys_zone.leds_max = keys_zone.leds_min;
    keys_zone.leds_count = keys_zone.leds_min;
    keys_zone.matrix_map = new matrix_map_type;
    keys_zone.matrix_map->width = new_matrix_map[0].size();
    keys_zone.matrix_map->height = new_matrix_map.size();
    keys_zone.matrix_map->map = flat_matrix_map.data();
    zones.push_back(keys_zone);

    if(number_of_underglow_leds > 0)
    {
        zone underglow_zone;
        underglow_zone.name = "Underglow";
        underglow_zone.type = ZONE_TYPE_MATRIX;
        underglow_zone.leds_min = number_of_underglow_leds;
        underglow_zone.leds_max = underglow_zone.leds_min;
        underglow_zone.leds_count = underglow_zone.leds_min;
        underglow_zone.matrix_map = new matrix_map_type;
        underglow_zone.matrix_map->width = new_underglow_map[0].size();
        underglow_zone.matrix_map->height = new_underglow_map.size();
        underglow_zone.matrix_map->map = flat_underglow_map.data();
        zones.push_back(underglow_zone);
    }

    for(int led_idx = 0; led_idx < number_of_leds; led_idx++)
    {
        led keyboard_led;
        keyboard_led.name = led_names[led_idx];
        keyboard_led.value = led_idx;
        leds.push_back(keyboard_led);
    }

    SetupColors();
    GetInitialLEDColors();
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
    qmk_rgb_matrix->DirectModeSetLEDs(colors, qmk_rgb_matrix->GetTotalNumberOfLEDs());
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
        {
            qmk_rgb_matrix->SetMode(hsv_color, modes[active_mode].value, modes[active_mode].speed);
        }
        else
        {
            qmk_rgb_matrix->SetMode(hsv_color, modes[active_mode].value, 127);
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

unsigned int RGBController_QMKRGBMatrix::CalculateDivisor
    (
    std::vector<point_t> led_points,
    std::set<int> rows,
    std::set<int> columns
    )
{
    std::vector< std::vector<point_t> > row_points(rows.size());
    for (const point_t &pt : led_points)
    {
        for (const int &i : rows)
        {
            if (pt.y == i)
            {
                row_points[std::distance(rows.begin(), rows.find(i))].push_back(pt);
            }
        }
    }

    int last_pos;
    std::vector<int> distances;
    for (const std::vector<point_t> &row : row_points)
    {
        last_pos = 0;
        std::for_each(row.begin(), row.end(), [&distances, &last_pos](const point_t &pt)
        {
            distances.push_back(pt.x - last_pos);
            last_pos = pt.x;
        });
    }
    std::map<int, int> counts;
    for (const int &i : distances)
    {
        counts[i]++;
    }

    unsigned int divisor = distances[0];
    for (const std::pair<int, int> &i : counts)
    {
        if (counts[divisor] < i.second)
        {
            divisor = i.first;
        }
    }
    return divisor;
}

std::pair<unsigned int, unsigned int> RGBController_QMKRGBMatrix::CountKeyTypes
    (
        std::vector<unsigned int> led_flags,
        unsigned int total_led_count
    )
{
    unsigned int underglow_leds = 0, key_leds = 0;
    for(int i = 0; i < total_led_count; i++)
    {
        if(led_flags[i] & 2)
        {
            underglow_leds++;
        }
        else if (led_flags[i] != 0)
        {
            key_leds++;
        }
    }
    return  std::make_pair(key_leds, underglow_leds);
}

std::pair<VectorMatrix, VectorMatrix> RGBController_QMKRGBMatrix::PlaceLEDsInMaps
    (
        std::set<int> unique_rows,
        std::set<int> unique_cols,
        unsigned int divisor,
        std::vector<point_t> led_points,
        std::vector<unsigned int> led_flags
    )
{
    VectorMatrix matrix_map_xl = MakeEmptyMatrixMap(unique_rows.size(), unique_cols.size());
    VectorMatrix underglow_map_xl = MakeEmptyMatrixMap(unique_rows.size(), unique_cols.size());

    unsigned int x, y, openrgb_idx, underglow_counter = 0;
    for (int i = 0; i < qmk_rgb_matrix->GetTotalNumberOfLEDs(); i++)
    {
        if (led_points[i].x != 255 && led_points[i].y != 255)
        {
            bool underglow = led_flags[i] & 2;
            x = std::round(led_points[i].x/divisor);
            y = std::distance(unique_rows.begin(), unique_rows.find(led_points[i].y));
            if (!underglow)
            {
                while (matrix_map_xl[y][x] != NO_LED)
                {
                    x++;
                }
                matrix_map_xl[y][x] = i;
            }
            else
            {
                while (underglow_map_xl[y][x] != NO_LED)
                {
                    x++;
                }
                underglow_map_xl[y][x] = underglow_counter;
                underglow_counter++;
            }
        }
    }
    return std::make_pair(matrix_map_xl, underglow_map_xl);
}

VectorMatrix RGBController_QMKRGBMatrix::MakeEmptyMatrixMap
    (
        unsigned int height,
        unsigned int width
    )
{
    std::vector<std::vector<unsigned int> > matrix_map(height);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            matrix_map[i].push_back(NO_LED);
        }
    }
    return matrix_map;
}

std::pair<VectorMatrix, VectorMatrix> RGBController_QMKRGBMatrix::CleanMatrixMaps
    (
        VectorMatrix matrix_map,
        VectorMatrix underglow_map,
        unsigned int height
    )
{
    bool empty_col = true, empty_col_udg = true, empty_row = true;
    int width = 0, width_udg = 0;
    std::vector<int> empty_rows;
    bool can_break;
    for (int i = 0; i < height; i++)
    {
        empty_row = true;
        for (std::size_t j = matrix_map[i].size() - 1; j --> 0; )
        {
            can_break = false;
            if (matrix_map[i][j] != NO_LED && width < (j + 1))
            {
                width = (j + 1);
                can_break = true;
                empty_row = false;
            }
            else if (matrix_map[i][j] != NO_LED)
                empty_row = false;
            if (underglow_map[i][j] != NO_LED && width_udg < (j + 1))
            {
                width_udg = (j + 1);
                if (can_break) break;
            }

        }
        if (matrix_map[i][0] != NO_LED) empty_col = false;
        if (underglow_map[i][0] != NO_LED) empty_col_udg = false;
        if (empty_row) empty_rows.push_back(i);
    }
    width = empty_col ? width - 1 : width;
    width_udg = empty_col_udg ? width_udg - 1 : width_udg;
    unsigned int new_height = height - empty_rows.size();
    for (unsigned i = empty_rows.size(); i --> 0; )
    {
        matrix_map.erase(matrix_map.begin()+empty_rows[i]);
    }

    for (int i = 0; i < new_height; i++)
    {
        matrix_map[i].erase(matrix_map[i].begin()+width, matrix_map[i].end());
        underglow_map[i].erase(underglow_map[i].begin()+width_udg, underglow_map[i].end());
    }
    for (int i = new_height; i < height; i++)
    {
        underglow_map[i].erase(underglow_map[i].begin()+width_udg, underglow_map[i].end());
    }

    return std::make_pair(matrix_map, underglow_map);
}

std::vector<unsigned int> RGBController_QMKRGBMatrix::FlattenMatrixMap
    (
        VectorMatrix matrix_map
    )
{
    std::vector<unsigned int> flat_map;

    for (const std::vector<unsigned int> &row : matrix_map)
    {
        for (const unsigned int &item : row)
        {
            flat_map.push_back(item);
        }
    }
    return flat_map;
}

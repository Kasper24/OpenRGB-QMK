#include "RGBController.h"
#include <cstring>

using namespace std::chrono_literals;

RGBController::RGBController()
{
    DeviceThreadRunning = true;
    DeviceCallThread = new std::thread(&RGBController::DeviceCallThreadFunction, this);
}

RGBController::~RGBController()
{
    DeviceThreadRunning = false;
    DeviceCallThread->join();
    delete DeviceCallThread;

    /*---------------------------------------------------------*\
    | Delete the matrix map                                     |
    \*---------------------------------------------------------*/
    for(int zone_index = 0; zone_index < zones.size(); zone_index++)
    {
        if(zones[zone_index].matrix_map != NULL)
        {
            if(zones[zone_index].matrix_map->map != NULL)
            {
                delete[] zones[zone_index].matrix_map->map;
            }

            delete zones[zone_index].matrix_map;
        }
    }
}

unsigned char * RGBController::GetDeviceDescription()
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    unsigned short name_len         = strlen(name.c_str())          + 1;
    unsigned short description_len  = strlen(description.c_str())   + 1;
    unsigned short version_len      = strlen(version.c_str())       + 1;
    unsigned short serial_len       = strlen(serial.c_str())        + 1;
    unsigned short location_len     = strlen(location.c_str())      + 1;
    unsigned short num_modes        = modes.size();
    unsigned short num_zones        = zones.size();
    unsigned short num_leds         = leds.size();
    unsigned short num_colors       = colors.size();

    unsigned short *mode_name_len   = new unsigned short[num_modes];
    unsigned short *zone_name_len   = new unsigned short[num_zones];
    unsigned short *led_name_len    = new unsigned short[num_leds];

    unsigned short *zone_matrix_len = new unsigned short[num_zones];
    unsigned short *mode_num_colors = new unsigned short[num_modes];

    data_size += sizeof(data_size);
    data_size += sizeof(device_type);
    data_size += name_len           + sizeof(name_len);
    data_size += description_len    + sizeof(description_len);
    data_size += version_len        + sizeof(version_len);
    data_size += serial_len         + sizeof(serial_len);
    data_size += location_len       + sizeof(location_len);

    data_size += sizeof(num_modes);
    data_size += sizeof(active_mode);

    for(int mode_index = 0; mode_index < num_modes; mode_index++)
    {
        mode_name_len[mode_index]   = strlen(modes[mode_index].name.c_str()) + 1;
        mode_num_colors[mode_index] = modes[mode_index].colors.size();

        data_size += mode_name_len[mode_index] + sizeof(mode_name_len[mode_index]);
        data_size += sizeof(modes[mode_index].value);
        data_size += sizeof(modes[mode_index].flags);
        data_size += sizeof(modes[mode_index].speed_min);
        data_size += sizeof(modes[mode_index].speed_max);
        data_size += sizeof(modes[mode_index].colors_min);
        data_size += sizeof(modes[mode_index].colors_max);
        data_size += sizeof(modes[mode_index].speed);
        data_size += sizeof(modes[mode_index].direction);
        data_size += sizeof(modes[mode_index].color_mode);
        data_size += sizeof(mode_num_colors[mode_index]);
        data_size += (mode_num_colors[mode_index] * sizeof(RGBColor));
    }

    data_size += sizeof(num_zones);

    for(int zone_index = 0; zone_index < num_zones; zone_index++)
    {
        zone_name_len[zone_index]   = strlen(zones[zone_index].name.c_str()) + 1;
        
        data_size += zone_name_len[zone_index] + sizeof(zone_name_len[zone_index]);
        data_size += sizeof(zones[zone_index].type);
        data_size += sizeof(zones[zone_index].leds_min);
        data_size += sizeof(zones[zone_index].leds_max);
        data_size += sizeof(zones[zone_index].leds_count);

        if(zones[zone_index].matrix_map == NULL)
        {
            zone_matrix_len[zone_index] = 0;
        }
        else
        {
            zone_matrix_len[zone_index] = (2 * sizeof(unsigned int)) + (zones[zone_index].matrix_map->height * zones[zone_index].matrix_map->width * sizeof(unsigned int));
        }

        data_size += sizeof(zone_matrix_len[zone_index]);
        data_size += zone_matrix_len[zone_index];
    }

    data_size += sizeof(num_leds);

    for(int led_index = 0; led_index < num_leds; led_index++)
    {
        led_name_len[led_index] = strlen(leds[led_index].name.c_str()) + 1;

        data_size += led_name_len[led_index] + sizeof(led_name_len[led_index]);

        data_size += sizeof(leds[led_index].value);
    }

    data_size += sizeof(num_colors);
    data_size += num_colors * sizeof(RGBColor);

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in type                                              |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &type, sizeof(device_type));
    data_ptr += sizeof(device_type);

    /*---------------------------------------------------------*\
    | Copy in name (size+data)                                  |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &name_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], name.c_str());
    data_ptr += name_len;

    /*---------------------------------------------------------*\
    | Copy in description (size+data)                           |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &description_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], description.c_str());
    data_ptr += description_len;
    
    /*---------------------------------------------------------*\
    | Copy in version (size+data)                               |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &version_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], version.c_str());
    data_ptr += version_len;

    /*---------------------------------------------------------*\
    | Copy in serial (size+data)                                |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &serial_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], serial.c_str());
    data_ptr += serial_len;

    /*---------------------------------------------------------*\
    | Copy in location (size+data)                              |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &location_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], location.c_str());
    data_ptr += location_len;

    /*---------------------------------------------------------*\
    | Copy in number of modes (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_modes, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in active mode (data)                                |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &active_mode, sizeof(active_mode));
    data_ptr += sizeof(active_mode);

    /*---------------------------------------------------------*\
    | Copy in modes                                             |
    \*---------------------------------------------------------*/
    for(int mode_index = 0; mode_index < num_modes; mode_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in mode name (size+data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &mode_name_len[mode_index], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], modes[mode_index].name.c_str());
        data_ptr += mode_name_len[mode_index];

        /*---------------------------------------------------------*\
        | Copy in mode value (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].value, sizeof(modes[mode_index].value));
        data_ptr += sizeof(modes[mode_index].value);

        /*---------------------------------------------------------*\
        | Copy in mode flags (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].flags, sizeof(modes[mode_index].flags));
        data_ptr += sizeof(modes[mode_index].flags);

        /*---------------------------------------------------------*\
        | Copy in mode speed_min (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].speed_min, sizeof(modes[mode_index].speed_min));
        data_ptr += sizeof(modes[mode_index].speed_min);

        /*---------------------------------------------------------*\
        | Copy in mode speed_max (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].speed_max, sizeof(modes[mode_index].speed_max));
        data_ptr += sizeof(modes[mode_index].speed_max);

        /*---------------------------------------------------------*\
        | Copy in mode colors_min (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].colors_min, sizeof(modes[mode_index].colors_min));
        data_ptr += sizeof(modes[mode_index].colors_min);

        /*---------------------------------------------------------*\
        | Copy in mode colors_max (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].colors_max, sizeof(modes[mode_index].colors_max));
        data_ptr += sizeof(modes[mode_index].colors_max);

        /*---------------------------------------------------------*\
        | Copy in mode speed (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].speed, sizeof(modes[mode_index].speed));
        data_ptr += sizeof(modes[mode_index].speed);

        /*---------------------------------------------------------*\
        | Copy in mode direction (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].direction, sizeof(modes[mode_index].direction));
        data_ptr += sizeof(modes[mode_index].direction);

        /*---------------------------------------------------------*\
        | Copy in mode color_mode (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode_index].color_mode, sizeof(modes[mode_index].color_mode));
        data_ptr += sizeof(modes[mode_index].color_mode);

        /*---------------------------------------------------------*\
        | Copy in mode number of colors                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &mode_num_colors[mode_index], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        /*---------------------------------------------------------*\
        | Copy in mode mode colors                                  |
        \*---------------------------------------------------------*/
        for(int color_index = 0; color_index < mode_num_colors[mode_index]; color_index++)
        {
            /*---------------------------------------------------------*\
            | Copy in color (data)                                      |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &modes[mode_index].colors[color_index], sizeof(modes[mode_index].colors[color_index]));
            data_ptr += sizeof(modes[mode_index].colors[color_index]);
        }
    }

    /*---------------------------------------------------------*\
    | Copy in number of zones (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_zones, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in zones                                             |
    \*---------------------------------------------------------*/
    for(int zone_index = 0; zone_index < num_zones; zone_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in zone name (size+data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zone_name_len[zone_index], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], zones[zone_index].name.c_str());
        data_ptr += zone_name_len[zone_index];

        /*---------------------------------------------------------*\
        | Copy in zone type (data)                                  |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zones[zone_index].type, sizeof(zones[zone_index].type));
        data_ptr += sizeof(zones[zone_index].type);

        /*---------------------------------------------------------*\
        | Copy in zone minimum LED count (data)                     |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zones[zone_index].leds_min, sizeof(zones[zone_index].leds_min));
        data_ptr += sizeof(zones[zone_index].leds_min);

        /*---------------------------------------------------------*\
        | Copy in zone maximum LED count (data)                     |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zones[zone_index].leds_max, sizeof(zones[zone_index].leds_max));
        data_ptr += sizeof(zones[zone_index].leds_max);

        /*---------------------------------------------------------*\
        | Copy in zone LED count (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zones[zone_index].leds_count, sizeof(zones[zone_index].leds_count));
        data_ptr += sizeof(zones[zone_index].leds_count);

        /*---------------------------------------------------------*\
        | Copy in size of zone matrix                               |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zone_matrix_len[zone_index], sizeof(zone_matrix_len[zone_index]));
        data_ptr += sizeof(zone_matrix_len[zone_index]);

        /*---------------------------------------------------------*\
        | Copy in matrix data if size is nonzero                    |
        \*---------------------------------------------------------*/
        if(zone_matrix_len[zone_index] > 0)
        {
            /*---------------------------------------------------------*\
            | Copy in matrix height                                     |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &zones[zone_index].matrix_map->height, sizeof(zones[zone_index].matrix_map->height));
            data_ptr += sizeof(zones[zone_index].matrix_map->height);

            /*---------------------------------------------------------*\
            | Copy in matrix width                                      |
            \*---------------------------------------------------------*/
            memcpy(&data_buf[data_ptr], &zones[zone_index].matrix_map->width, sizeof(zones[zone_index].matrix_map->width));
            data_ptr += sizeof(zones[zone_index].matrix_map->width);

            /*---------------------------------------------------------*\
            | Copy in matrix map                                        |
            \*---------------------------------------------------------*/
            for(int matrix_idx = 0; matrix_idx < (zones[zone_index].matrix_map->height * zones[zone_index].matrix_map->width); matrix_idx++)
            {
                memcpy(&data_buf[data_ptr], &zones[zone_index].matrix_map->map[matrix_idx], sizeof(zones[zone_index].matrix_map->map[matrix_idx]));
                data_ptr += sizeof(zones[zone_index].matrix_map->map[matrix_idx]);
            }
        }
    }

    /*---------------------------------------------------------*\
    | Copy in number of LEDs (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_leds, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in LEDs                                              |
    \*---------------------------------------------------------*/
    for(int led_index = 0; led_index < num_leds; led_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in LED name (size+data)                              |
        \*---------------------------------------------------------*/
        unsigned short ledname_len = strlen(leds[led_index].name.c_str()) + 1;
        memcpy(&data_buf[data_ptr], &ledname_len, sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        strcpy((char *)&data_buf[data_ptr], leds[led_index].name.c_str());
        data_ptr += ledname_len;

        /*---------------------------------------------------------*\
        | Copy in LED value (data)                                  |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &leds[led_index].value, sizeof(leds[led_index].value));
        data_ptr += sizeof(leds[led_index].value);
    }

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &colors[color_index], sizeof(colors[color_index]));
        data_ptr += sizeof(colors[color_index]);
    }

    delete[] mode_name_len;
    delete[] zone_name_len;
    delete[] led_name_len;

    delete[] zone_matrix_len;
    delete[] mode_num_colors;

    return(data_buf);
}

void RGBController::ReadDeviceDescription(unsigned char* data_buf)
{
    unsigned int data_ptr = 0;

    data_ptr += sizeof(unsigned int);

    /*---------------------------------------------------------*\
    | Copy in type                                              |
    \*---------------------------------------------------------*/
    memcpy(&type, &data_buf[data_ptr], sizeof(device_type));
    data_ptr += sizeof(device_type);

    /*---------------------------------------------------------*\
    | Copy in name                                              |
    \*---------------------------------------------------------*/
    unsigned short name_len;
    memcpy(&name_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    name = (char *)&data_buf[data_ptr];
    data_ptr += name_len;

    /*---------------------------------------------------------*\
    | Copy in description                                       |
    \*---------------------------------------------------------*/
    unsigned short description_len;
    memcpy(&description_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    description = (char *)&data_buf[data_ptr];
    data_ptr += description_len;

    /*---------------------------------------------------------*\
    | Copy in version                                           |
    \*---------------------------------------------------------*/
    unsigned short version_len;
    memcpy(&version_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    version = (char *)&data_buf[data_ptr];
    data_ptr += version_len;
    
    /*---------------------------------------------------------*\
    | Copy in serial                                            |
    \*---------------------------------------------------------*/
    unsigned short serial_len;
    memcpy(&serial_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    serial = (char *)&data_buf[data_ptr];
    data_ptr += serial_len;

    /*---------------------------------------------------------*\
    | Copy in location                                          |
    \*---------------------------------------------------------*/
    unsigned short location_len;
    memcpy(&location_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    location = (char *)&data_buf[data_ptr];
    data_ptr += location_len;

    /*---------------------------------------------------------*\
    | Copy in number of modes (data)                            |
    \*---------------------------------------------------------*/
    unsigned short num_modes;
    memcpy(&num_modes, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in active mode (data)                                |
    \*---------------------------------------------------------*/
    memcpy(&active_mode, &data_buf[data_ptr], sizeof(active_mode));
    data_ptr += sizeof(active_mode);

    /*---------------------------------------------------------*\
    | Copy in modes                                             |
    \*---------------------------------------------------------*/
    for(int mode_index = 0; mode_index < num_modes; mode_index++)
    {
        mode new_mode;

        /*---------------------------------------------------------*\
        | Copy in mode name (size+data)                             |
        \*---------------------------------------------------------*/
        unsigned short modename_len;
        memcpy(&modename_len, &data_buf[data_ptr], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        new_mode.name = (char *)&data_buf[data_ptr];
        data_ptr += modename_len;

        /*---------------------------------------------------------*\
        | Copy in mode value (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.value, &data_buf[data_ptr], sizeof(new_mode.value));
        data_ptr += sizeof(new_mode.value);

        /*---------------------------------------------------------*\
        | Copy in mode flags (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.flags, &data_buf[data_ptr], sizeof(new_mode.flags));
        data_ptr += sizeof(new_mode.flags);

        /*---------------------------------------------------------*\
        | Copy in mode speed_min (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.speed_min, &data_buf[data_ptr], sizeof(new_mode.speed_min));
        data_ptr += sizeof(new_mode.speed_min);

        /*---------------------------------------------------------*\
        | Copy in mode speed_max (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.speed_max, &data_buf[data_ptr], sizeof(new_mode.speed_max));
        data_ptr += sizeof(new_mode.speed_max);

        /*---------------------------------------------------------*\
        | Copy in mode colors_min (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.colors_min, &data_buf[data_ptr], sizeof(new_mode.colors_min));
        data_ptr += sizeof(new_mode.colors_min);

        /*---------------------------------------------------------*\
        | Copy in mode colors_max (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.colors_max, &data_buf[data_ptr], sizeof(new_mode.colors_max));
        data_ptr += sizeof(new_mode.colors_max);

        /*---------------------------------------------------------*\
        | Copy in mode speed (data)                                 |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.speed, &data_buf[data_ptr], sizeof(new_mode.speed));
        data_ptr += sizeof(new_mode.speed);

        /*---------------------------------------------------------*\
        | Copy in mode direction (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.direction, &data_buf[data_ptr], sizeof(new_mode.direction));
        data_ptr += sizeof(new_mode.direction);

        /*---------------------------------------------------------*\
        | Copy in mode color_mode (data)                            |
        \*---------------------------------------------------------*/
        memcpy(&new_mode.color_mode, &data_buf[data_ptr], sizeof(new_mode.color_mode));
        data_ptr += sizeof(new_mode.color_mode);

        /*---------------------------------------------------------*\
        | Copy in mode number of colors                             |
        \*---------------------------------------------------------*/
        unsigned short mode_num_colors;
        memcpy(&mode_num_colors, &data_buf[data_ptr], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        /*---------------------------------------------------------*\
        | Copy in mode mode colors                                  |
        \*---------------------------------------------------------*/
        for(int color_index = 0; color_index < mode_num_colors; color_index++)
        {
            /*---------------------------------------------------------*\
            | Copy in color (data)                                      |
            \*---------------------------------------------------------*/
            RGBColor new_color;
            memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
            data_ptr += sizeof(modes[mode_index].colors[color_index]);

            new_mode.colors.push_back(new_color);
        }

        modes.push_back(new_mode);
    }

    /*---------------------------------------------------------*\
    | Copy in number of zones (data)                            |
    \*---------------------------------------------------------*/
    unsigned short num_zones;
    memcpy(&num_zones, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in zones                                             |
    \*---------------------------------------------------------*/
    for(int zone_index = 0; zone_index < num_zones; zone_index++)
    {
        zone new_zone;

        /*---------------------------------------------------------*\
        | Copy in zone name (size+data)                             |
        \*---------------------------------------------------------*/
        unsigned short zonename_len;
        memcpy(&zonename_len, &data_buf[data_ptr], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        new_zone.name = (char *)&data_buf[data_ptr];
        data_ptr += zonename_len;

        /*---------------------------------------------------------*\
        | Copy in zone type (data)                                  |
        \*---------------------------------------------------------*/
        memcpy(&new_zone.type, &data_buf[data_ptr], sizeof(new_zone.type));
        data_ptr += sizeof(new_zone.type);

        /*---------------------------------------------------------*\
        | Copy in zone minimum LED count (data)                     |
        \*---------------------------------------------------------*/
        memcpy(&new_zone.leds_min, &data_buf[data_ptr], sizeof(new_zone.leds_min));
        data_ptr += sizeof(new_zone.leds_min);

        /*---------------------------------------------------------*\
        | Copy in zone maximum LED count (data)                     |
        \*---------------------------------------------------------*/
        memcpy(&new_zone.leds_max, &data_buf[data_ptr], sizeof(new_zone.leds_max));
        data_ptr += sizeof(new_zone.leds_max);

        /*---------------------------------------------------------*\
        | Copy in zone LED count (data)                             |
        \*---------------------------------------------------------*/
        memcpy(&new_zone.leds_count, &data_buf[data_ptr], sizeof(new_zone.leds_count));
        data_ptr += sizeof(new_zone.leds_count);

        /*---------------------------------------------------------*\
        | Copy in size of zone matrix                               |
        \*---------------------------------------------------------*/
        unsigned short zone_matrix_len;
        memcpy(&zone_matrix_len, &data_buf[data_ptr], sizeof(zone_matrix_len));
        data_ptr += sizeof(zone_matrix_len);

        /*---------------------------------------------------------*\
        | Copy in matrix data if size is nonzero                    |
        \*---------------------------------------------------------*/
        if(zone_matrix_len > 0)
        {
            /*---------------------------------------------------------*\
            | Create a map data structure to fill in and attach it to   |
            | the new zone                                              |
            \*---------------------------------------------------------*/
            matrix_map_type * new_map = new matrix_map_type;

            new_zone.matrix_map = new_map;

            /*---------------------------------------------------------*\
            | Copy in matrix height                                     |
            \*---------------------------------------------------------*/
            memcpy(&new_map->height, &data_buf[data_ptr], sizeof(new_map->height));
            data_ptr += sizeof(new_map->height);

            /*---------------------------------------------------------*\
            | Copy in matrix width                                      |
            \*---------------------------------------------------------*/
            memcpy(&new_map->width, &data_buf[data_ptr], sizeof(new_map->width));
            data_ptr += sizeof(new_map->width);

            /*---------------------------------------------------------*\
            | Copy in matrix map                                        |
            \*---------------------------------------------------------*/
            new_map->map = new unsigned int[new_map->height * new_map->width];

            for(int matrix_idx = 0; matrix_idx < (new_map->height * new_map->width); matrix_idx++)
            {
                memcpy(&new_map->map[matrix_idx], &data_buf[data_ptr], sizeof(new_map->map[matrix_idx]));
                data_ptr += sizeof(new_map->map[matrix_idx]);
            }
        }
        else
        {
            new_zone.matrix_map = NULL;
        }
        
        zones.push_back(new_zone);
    }

    /*---------------------------------------------------------*\
    | Copy in number of LEDs (data)                             |
    \*---------------------------------------------------------*/
    unsigned short num_leds;
    memcpy(&num_leds, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in LEDs                                              |
    \*---------------------------------------------------------*/
    for(int led_index = 0; led_index < num_leds; led_index++)
    {
        led new_led;

        /*---------------------------------------------------------*\
        | Copy in LED name (size+data)                              |
        \*---------------------------------------------------------*/
        unsigned short ledname_len;
        memcpy(&ledname_len, &data_buf[data_ptr], sizeof(unsigned short));
        data_ptr += sizeof(unsigned short);

        new_led.name = (char *)&data_buf[data_ptr];
        data_ptr += ledname_len;

        /*---------------------------------------------------------*\
        | Copy in LED value (data)                                  |
        \*---------------------------------------------------------*/
        memcpy(&new_led.value, &data_buf[data_ptr], sizeof(new_led.value));
        data_ptr += sizeof(new_led.value);

        leds.push_back(new_led);
    }

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    unsigned short num_colors;
    memcpy(&num_colors, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        RGBColor new_color;

        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
        data_ptr += sizeof(RGBColor);

        colors.push_back(new_color);
    }

    /*---------------------------------------------------------*\
    | Setup colors                                              |
    \*---------------------------------------------------------*/
    SetupColors();
}

unsigned char * RGBController::GetModeDescription(int mode)
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    unsigned short mode_name_len;
    unsigned short mode_num_colors;

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    mode_name_len   = strlen(modes[mode].name.c_str()) + 1;
    mode_num_colors = modes[mode].colors.size();

    data_size += sizeof(data_size);
    data_size += sizeof(mode);
    data_size += sizeof(mode_name_len);
    data_size += mode_name_len;
    data_size += sizeof(modes[mode].value);
    data_size += sizeof(modes[mode].flags);
    data_size += sizeof(modes[mode].speed_min);
    data_size += sizeof(modes[mode].speed_max);
    data_size += sizeof(modes[mode].colors_min);
    data_size += sizeof(modes[mode].colors_max);
    data_size += sizeof(modes[mode].speed);
    data_size += sizeof(modes[mode].direction);
    data_size += sizeof(modes[mode].color_mode);
    data_size += sizeof(mode_num_colors);
    data_size += (mode_num_colors * sizeof(RGBColor));

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in mode index                                        |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &mode, sizeof(int));
    data_ptr += sizeof(int);

    /*---------------------------------------------------------*\
    | Copy in mode name (size+data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &mode_name_len, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    strcpy((char *)&data_buf[data_ptr], modes[mode].name.c_str());
    data_ptr += mode_name_len;

    /*---------------------------------------------------------*\
    | Copy in mode value (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].value, sizeof(modes[mode].value));
    data_ptr += sizeof(modes[mode].value);

    /*---------------------------------------------------------*\
    | Copy in mode flags (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].flags, sizeof(modes[mode].flags));
    data_ptr += sizeof(modes[mode].flags);

    /*---------------------------------------------------------*\
    | Copy in mode speed_min (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].speed_min, sizeof(modes[mode].speed_min));
    data_ptr += sizeof(modes[mode].speed_min);

    /*---------------------------------------------------------*\
    | Copy in mode speed_max (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].speed_max, sizeof(modes[mode].speed_max));
    data_ptr += sizeof(modes[mode].speed_max);

    /*---------------------------------------------------------*\
    | Copy in mode colors_min (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].colors_min, sizeof(modes[mode].colors_min));
    data_ptr += sizeof(modes[mode].colors_min);

    /*---------------------------------------------------------*\
    | Copy in mode colors_max (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].colors_max, sizeof(modes[mode].colors_max));
    data_ptr += sizeof(modes[mode].colors_max);

    /*---------------------------------------------------------*\
    | Copy in mode speed (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].speed, sizeof(modes[mode].speed));
    data_ptr += sizeof(modes[mode].speed);

    /*---------------------------------------------------------*\
    | Copy in mode direction (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].direction, sizeof(modes[mode].direction));
    data_ptr += sizeof(modes[mode].direction);

    /*---------------------------------------------------------*\
    | Copy in mode color_mode (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &modes[mode].color_mode, sizeof(modes[mode].color_mode));
    data_ptr += sizeof(modes[mode].color_mode);

    /*---------------------------------------------------------*\
    | Copy in mode number of colors                             |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &mode_num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in mode mode colors                                  |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < mode_num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &modes[mode].colors[color_index], sizeof(modes[mode].colors[color_index]));
        data_ptr += sizeof(modes[mode].colors[color_index]);
    }

    return(data_buf);
}

void RGBController::SetModeDescription(unsigned char* data_buf)
{
    int mode_idx;
    unsigned int data_ptr = sizeof(unsigned int);

    /*---------------------------------------------------------*\
    | Copy in mode index                                        |
    \*---------------------------------------------------------*/
    memcpy(&mode_idx, &data_buf[data_ptr], sizeof(int));
    data_ptr += sizeof(int);

    /*---------------------------------------------------------*\
    | Check if we aren't reading beyond the list of modes.      |
    \*---------------------------------------------------------*/
    if(((size_t) mode_idx) >  modes.size())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Get pointer to target mode                                |
    \*---------------------------------------------------------*/
    mode * new_mode = &modes[mode_idx];
    
    /*---------------------------------------------------------*\
    | Set active mode to the new mode                           |
    \*---------------------------------------------------------*/
    active_mode = mode_idx;

    /*---------------------------------------------------------*\
    | Copy in mode name (size+data)                             |
    \*---------------------------------------------------------*/
    unsigned short modename_len;
    memcpy(&modename_len, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    new_mode->name = (char *)&data_buf[data_ptr];
    data_ptr += modename_len;

    /*---------------------------------------------------------*\
    | Copy in mode value (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->value, &data_buf[data_ptr], sizeof(new_mode->value));
    data_ptr += sizeof(new_mode->value);

    /*---------------------------------------------------------*\
    | Copy in mode flags (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->flags, &data_buf[data_ptr], sizeof(new_mode->flags));
    data_ptr += sizeof(new_mode->flags);

    /*---------------------------------------------------------*\
    | Copy in mode speed_min (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->speed_min, &data_buf[data_ptr], sizeof(new_mode->speed_min));
    data_ptr += sizeof(new_mode->speed_min);

    /*---------------------------------------------------------*\
    | Copy in mode speed_max (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->speed_max, &data_buf[data_ptr], sizeof(new_mode->speed_max));
    data_ptr += sizeof(new_mode->speed_max);

    /*---------------------------------------------------------*\
    | Copy in mode colors_min (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->colors_min, &data_buf[data_ptr], sizeof(new_mode->colors_min));
    data_ptr += sizeof(new_mode->colors_min);

    /*---------------------------------------------------------*\
    | Copy in mode colors_max (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->colors_max, &data_buf[data_ptr], sizeof(new_mode->colors_max));
    data_ptr += sizeof(new_mode->colors_max);

    /*---------------------------------------------------------*\
    | Copy in mode speed (data)                                 |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->speed, &data_buf[data_ptr], sizeof(new_mode->speed));
    data_ptr += sizeof(new_mode->speed);

    /*---------------------------------------------------------*\
    | Copy in mode direction (data)                             |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->direction, &data_buf[data_ptr], sizeof(new_mode->direction));
    data_ptr += sizeof(new_mode->direction);

    /*---------------------------------------------------------*\
    | Copy in mode color_mode (data)                            |
    \*---------------------------------------------------------*/
    memcpy(&new_mode->color_mode, &data_buf[data_ptr], sizeof(new_mode->color_mode));
    data_ptr += sizeof(new_mode->color_mode);

    /*---------------------------------------------------------*\
    | Copy in mode number of colors                             |
    \*---------------------------------------------------------*/
    unsigned short mode_num_colors;
    memcpy(&mode_num_colors, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in mode mode colors                                  |
    \*---------------------------------------------------------*/
    new_mode->colors.clear();
    for(int color_index = 0; color_index < mode_num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        RGBColor new_color;
        memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
        data_ptr += sizeof(RGBColor);

        new_mode->colors.push_back(new_color);
    }
}

unsigned char * RGBController::GetColorDescription()
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    unsigned short num_colors = colors.size();

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    data_size += sizeof(data_size);
    data_size += sizeof(num_colors);
    data_size += num_colors * sizeof(RGBColor);

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &colors[color_index], sizeof(colors[color_index]));
        data_ptr += sizeof(colors[color_index]);
    }

    return(data_buf);
}

void RGBController::SetColorDescription(unsigned char* data_buf)
{
    unsigned int data_ptr = sizeof(unsigned int);

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    unsigned short num_colors;
    memcpy(&num_colors, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Check if we aren't reading beyond the list of colors.     |
    \*---------------------------------------------------------*/
    if(((size_t) num_colors) > colors.size())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        RGBColor new_color;

        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
        data_ptr += sizeof(RGBColor);

        colors[color_index] = new_color;
    }
}

unsigned char * RGBController::GetZoneColorDescription(int zone)
{
    unsigned int data_ptr = 0;
    unsigned int data_size = 0;

    unsigned short num_colors = zones[zone].leds_count;

    /*---------------------------------------------------------*\
    | Calculate data size                                       |
    \*---------------------------------------------------------*/
    data_size += sizeof(data_size);
    data_size += sizeof(zone);
    data_size += sizeof(num_colors);
    data_size += num_colors * sizeof(RGBColor);

    /*---------------------------------------------------------*\
    | Create data buffer                                        |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[data_size];

    /*---------------------------------------------------------*\
    | Copy in data size                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &data_size, sizeof(data_size));
    data_ptr += sizeof(data_size);

    /*---------------------------------------------------------*\
    | Copy in zone index                                        |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &zone, sizeof(zone));
    data_ptr += sizeof(zone);

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[data_ptr], &num_colors, sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&data_buf[data_ptr], &zones[zone].colors[color_index], sizeof(zones[zone].colors[color_index]));
        data_ptr += sizeof(zones[zone].colors[color_index]);
    }

    return(data_buf);
}

void RGBController::SetZoneColorDescription(unsigned char* data_buf)
{
    unsigned int data_ptr = sizeof(unsigned int);
    unsigned int zone_idx;

    /*---------------------------------------------------------*\
    | Copy in zone index                                        |
    \*---------------------------------------------------------*/
    memcpy(&zone_idx, &data_buf[data_ptr], sizeof(zone_idx));
    data_ptr += sizeof(zone_idx);

    /*---------------------------------------------------------*\
    | Check if we aren't reading beyond the list of zones.      |
    \*---------------------------------------------------------*/
    if(((size_t) zone_idx) > zones.size())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Copy in number of colors (data)                           |
    \*---------------------------------------------------------*/
    unsigned short num_colors;
    memcpy(&num_colors, &data_buf[data_ptr], sizeof(unsigned short));
    data_ptr += sizeof(unsigned short);

    /*---------------------------------------------------------*\
    | Copy in colors                                            |
    \*---------------------------------------------------------*/
    for(int color_index = 0; color_index < num_colors; color_index++)
    {
        RGBColor new_color;

        /*---------------------------------------------------------*\
        | Copy in color (data)                                      |
        \*---------------------------------------------------------*/
        memcpy(&new_color, &data_buf[data_ptr], sizeof(RGBColor));
        data_ptr += sizeof(RGBColor);

        zones[zone_idx].colors[color_index] = new_color;
    }
}

unsigned char * RGBController::GetSingleLEDColorDescription(int led)
{
    /*---------------------------------------------------------*\
    | Fixed size descrption:                                    |
    |       int:      LED index                                 |
    |       RGBColor: LED color                                 |
    \*---------------------------------------------------------*/
    unsigned char *data_buf = new unsigned char[sizeof(int) + sizeof(RGBColor)];

    /*---------------------------------------------------------*\
    | Copy in LED index                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[0], &led, sizeof(int));

    /*---------------------------------------------------------*\
    | Copy in LED color                                         |
    \*---------------------------------------------------------*/
    memcpy(&data_buf[sizeof(led)], &colors[led], sizeof(RGBColor));

    return(data_buf);
}

void RGBController::SetSingleLEDColorDescription(unsigned char* data_buf)
{
    /*---------------------------------------------------------*\
    | Fixed size descrption:                                    |
    |       int:      LED index                                 |
    |       RGBColor: LED color                                 |
    \*---------------------------------------------------------*/
    int led_idx;

    /*---------------------------------------------------------*\
    | Copy in LED index                                         |
    \*---------------------------------------------------------*/
    memcpy(&led_idx, &data_buf[0], sizeof(led_idx));

    /*---------------------------------------------------------*\
    | Check if we aren't reading beyond the list of leds.       |
    \*---------------------------------------------------------*/
    if(((size_t) led_idx) > leds.size())
    {
        return;
    }

    /*---------------------------------------------------------*\
    | Copy in LED color                                         |
    \*---------------------------------------------------------*/
    memcpy(&colors[led_idx], &data_buf[sizeof(led_idx)], sizeof(RGBColor));
}

void RGBController::SetupColors()
{
    unsigned int total_led_count;

    /*---------------------------------------------------------*\
    | Determine total number of LEDs on the device              |
    \*---------------------------------------------------------*/
    total_led_count = 0;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        total_led_count += zones[zone_idx].leds_count;
    }

    /*---------------------------------------------------------*\
    | Set the size of the color buffer to the number of LEDs    |
    \*---------------------------------------------------------*/
    colors.resize(total_led_count);

    /*---------------------------------------------------------*\
    | Set the color buffer pointers on each zone                |
    \*---------------------------------------------------------*/
    total_led_count = 0;

    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        zones[zone_idx].start_idx=total_led_count;

        if((colors.size() > 0) && (zones[zone_idx].leds_count > 0))
        {
            zones[zone_idx].colors = &colors[total_led_count];
        }
        else
        {
            zones[zone_idx].colors = NULL;
        }

        if((leds.size() > 0) && (zones[zone_idx].leds_count > 0))
        {
            zones[zone_idx].leds   = &leds[total_led_count];
        }
        else
        {
            zones[zone_idx].leds    = NULL;
        }


        total_led_count += zones[zone_idx].leds_count;
    }

    /*---------------------------------------------------------*\
    | Prepare the graphical representation matrix               |
    \*---------------------------------------------------------*/
    RearrangeLedPositions();
}

RGBColor RGBController::GetLED(unsigned int led)
{
    if(led < colors.size())
    {
        return(colors[led]);
    }
    else
    {
        return(0x00000000);
    }
}

void RGBController::SetLED(unsigned int led, RGBColor color)
{
    if(led < colors.size())
    {
        colors[led] = color;
    }
}

void RGBController::SetAllLEDs(RGBColor color)
{
    for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
    {
        SetAllZoneLEDs(zone_idx, color);
    }
}

void RGBController::SetAllZoneLEDs(int zone, RGBColor color)
{
    for (std::size_t color_idx = 0; color_idx < zones[zone].leds_count; color_idx++)
    {
        zones[zone].colors[color_idx] = color;
    }
}

int RGBController::GetMode()
{
    return(active_mode);
}

void RGBController::SetMode(int mode)
{
    active_mode = mode;

    UpdateMode();
}

void RGBController::RegisterUpdateCallback(RGBControllerCallback new_callback, void * new_callback_arg)
{
    UpdateCallbacks.push_back(new_callback);
    UpdateCallbackArgs.push_back(new_callback_arg);
}

void RGBController::SignalUpdate()
{
    UpdateMutex.lock();

    /*-------------------------------------------------*\
    | Client info has changed, call the callbacks       |
    \*-------------------------------------------------*/
    for(unsigned int callback_idx = 0; callback_idx < UpdateCallbacks.size(); callback_idx++)
    {
        UpdateCallbacks[callback_idx](UpdateCallbackArgs[callback_idx]);
    }

    UpdateMutex.unlock();
}
void RGBController::UpdateLEDs()
{
    CallFlag_UpdateLEDs = true;

    SignalUpdate();
}

void RGBController::UpdateMode()
{
    CallFlag_UpdateMode = true;
}

void RGBController::DeviceUpdateLEDs()
{

}

void RGBController::DeviceUpdateMode()
{

}

void RGBController::DeviceCallThreadFunction()
{
    CallFlag_UpdateLEDs = false;
    CallFlag_UpdateMode = false;

    while(DeviceThreadRunning.load() == true)
    {
        if(CallFlag_UpdateMode.load() == true)
        {
            DeviceUpdateMode();
            CallFlag_UpdateMode = false;
        }
        if(CallFlag_UpdateLEDs.load() == true)
        {
            DeviceUpdateLEDs();
            CallFlag_UpdateLEDs = false;
        }
        else
        {
           std::this_thread::sleep_for(1ms);
        }
    }
}

void RGBController::RearrangeLedPositions()
{
    if(!LedPositionsSet)
    {
        unsigned int maxWidth = 0;
        float totalHeight = 0;
        unsigned int maxCols = 20;
        float zonePadding = 1; // Amount of space between zones
        float ledPadding = 0.1;

        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            if((zones[zone_idx].type == ZONE_TYPE_MATRIX) && (zones[zone_idx].matrix_map))
            {
                totalHeight += zones[zone_idx].matrix_map->height;
                zones[zone_idx].matrix_w = zones[zone_idx].matrix_map->width;
            }
            else
            {
                unsigned int count = zones[zone_idx].leds_count;
                zones[zone_idx].matrix_w = std::min(count, maxCols);
                totalHeight += count / maxCols + !!(count % maxCols); // Equivalent to ceil(float(count) / maxCols);
            }
            if(zones[zone_idx].matrix_w > maxWidth)
            {
                maxWidth = zones[zone_idx].matrix_w;
            }
        }

        totalHeight += zones.size() * zonePadding; // Add some space for zone names & padding;

        float current_y = 0; // We will be descending, placing each zone one atom below the previous one
        float atom = 1.0 / maxWidth; // Atom is the width of a single square; if the whole thing becomes too tall, we ignore it and let the view widget take care of it
        matrix_h = totalHeight * atom;

        for(std::size_t zone_idx = 0; zone_idx < zones.size(); zone_idx++)
        {
            zones[zone_idx].matrix_x = (1.0 - (zones[zone_idx].matrix_w * atom)) / 2;
            zones[zone_idx].matrix_y = current_y + 0.5 * atom;
            zones[zone_idx].matrix_w *= atom;
            zones[zone_idx].matrix_h = 0.4 * atom;
            current_y += zonePadding * atom;

            // And now, for the LEDs
            if((zones[zone_idx].type == ZONE_TYPE_MATRIX) && (zones[zone_idx].matrix_map))
            {
                auto& map = zones[zone_idx].matrix_map;
                for(int led_x = 0; led_x < map->width; ++led_x)
                {
                    for(int led_y = 0; led_y < map->height; ++led_y)
                    {
                        unsigned int map_idx = led_y * map->width + led_x;
                        unsigned int color_idx = map->map[map_idx];
                        if(color_idx != 0xFFFFFFFF)
                        {
                            leds[color_idx].matrix_x = (zones[zone_idx].matrix_x + led_x + ledPadding) * atom;
                            leds[color_idx].matrix_y = current_y + (led_y + ledPadding) * atom;
                            leds[color_idx].matrix_w = (1 - 2 * ledPadding) * atom;
                            leds[color_idx].matrix_h = (1 - 2 * ledPadding) * atom;
                        }
                    }
                }
                current_y += map->height * atom;
            }
            else
            {
#warning this might be an issue with resizable zones - I don't know how to handle them!
                for(int i = 0; i < zones[zone_idx].leds_count; ++i)
                {
                    zones[zone_idx].leds[i].matrix_x = zones[zone_idx].matrix_x + (i % maxCols + ledPadding) * atom;
                    zones[zone_idx].leds[i].matrix_y = current_y + (i / maxCols + ledPadding) * atom;
                    zones[zone_idx].leds[i].matrix_w = (1 - 2 * ledPadding) * atom;
                    zones[zone_idx].leds[i].matrix_h = (1 - 2 * ledPadding) * atom;
                }
                current_y += (zones[zone_idx].leds_count / maxCols + !!(zones[zone_idx].leds_count % maxCols)) * atom;
            }
        }
    }
    // current_y and matrix_h must be equal at this point
}

std::string device_type_to_str(device_type type)
{
    switch(type)
    {
    case DEVICE_TYPE_MOTHERBOARD:
        return "Motherboard";
    case DEVICE_TYPE_DRAM:
        return "DRAM";
    case DEVICE_TYPE_GPU:
        return "GPU";
    case DEVICE_TYPE_COOLER:
        return "Cooler";
    case DEVICE_TYPE_LEDSTRIP:
        return "LED Strip";
    case DEVICE_TYPE_KEYBOARD:
        return "Keyboard";
    case DEVICE_TYPE_MOUSE:
        return "Mouse";
    case DEVICE_TYPE_MOUSEMAT:
        return "Mousemat";
    case DEVICE_TYPE_HEADSET:
        return "Headset";
    case DEVICE_TYPE_HEADSET_STAND:
        return "Headset Stand";
    default:
        return "Unknown";
    }
}

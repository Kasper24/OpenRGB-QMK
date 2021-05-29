/*-------------------------------------------------------------------*\
|  RGBController_QMKRGBMatrix.h                                       |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|  Jath03           28th May 2021                                     |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "QMKRGBMatrixController.h"
#include <algorithm>
#include <cmath>
#include <map>
#include <set>

#define NO_LED 0xFFFFFFFF

typedef std::vector<std::vector<unsigned int>> VectorMatrix;

class RGBController_QMKRGBMatrix : public RGBController
{
public:
    RGBController_QMKRGBMatrix(QMKRGBMatrixController* qmk_rgb_matrix_ptr, unsigned int protocol_version);
    ~RGBController_QMKRGBMatrix();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
private:
    QMKRGBMatrixController* qmk_rgb_matrix;
    std::vector<unsigned int> flat_matrix_map;
    std::vector<unsigned int> flat_underglow_map;
    void            InitializeMode
                    (
                    std::string name,
                    unsigned int &current_mode,
                    unsigned int flags,
                    unsigned int color_mode
                    );

    unsigned int    CalculateDivisor
                    (
                    std::vector<point_t> led_points,
                    std::set<int> rows,
                    std::set<int> columns
                    );

    void            GetInitialLEDColors();

    std::pair<unsigned int, unsigned int>   CountKeyTypes
                                            (
                                            std::vector<unsigned int> led_points,
                                            unsigned int total_led_count
                                            );
    std::pair<VectorMatrix, VectorMatrix> PlaceLEDsInMaps
                                        (
                                        std::set<int> unique_rows,
                                        std::set<int> unique_cols,
                                        unsigned int divisor,
                                        std::vector<point_t> led_points,
                                        std::vector<unsigned int> led_flags
                                        );
    VectorMatrix MakeEmptyMatrixMap
                (
                unsigned int height,
                unsigned int width
                );
    std::pair<VectorMatrix, VectorMatrix> CleanMatrixMaps
                                        (
                                        std::vector<std::vector<unsigned int> > matrix_map,
                                        std::vector<std::vector<unsigned int> > underglow_map,
                                        unsigned int height,
                                        bool has_underglow
                                        );
    std::vector<unsigned int>   FlattenMatrixMap
                                (
                                VectorMatrix matrix_map
                                );
};

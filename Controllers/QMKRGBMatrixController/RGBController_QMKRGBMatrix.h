/*-------------------------------------------------------------------*\
|  RGBController_QMKRGBMatrix.h                                       |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once

#include "RGBController.h"
#include "QMKRGBMatrixController.h"
#include <map>
#include <set>

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

    unsigned int*   OpenRGBIdxToQMKIdx
                    (
                    std::vector<point_t> led_points
                    );
};

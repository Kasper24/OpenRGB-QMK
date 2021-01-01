/*-------------------------------------------------------------------*\
|  QMKRGBMatrix.cpp                                              |
|                                                                     |
|  Driver for Massdrop Alt keyboard                             |
|                                                                     |
|  Kasper         10th Octobber 2020                             |
|                                                                     |
\*-------------------------------------------------------------------*/

#pragma once
#include "RGBController.h"
#include "QMKRGBMatrixController.h"

class RGBController_QMKRGBMatrix : public RGBController
{
public:
    RGBController_QMKRGBMatrix(QMKRGBMatrixController* qmk_rgb_matrix_ptr);
    ~RGBController_QMKRGBMatrix();

    void        SetupZones();
    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        SetCustomMode();
    void        DeviceUpdateMode();
private:
    int         pid;
    QMKRGBMatrixController* qmk_rgb_matrix;
};

/*-------------------------------------------------------------------*\
|  QMKRGBMatrixControllerDetect.cpp                                     |
|                                                                     |
|  Driver for QMK keybaords using RGB Matrix                          |
|                                                                     |
|  Kasper24         11th November 2020                                |
|                                                                     |
\*-------------------------------------------------------------------*/

#include "Detector.h"
#include "QMKRGBMatrixController.h"
#include "RGBController.h"
#include "RGBController_QMKRGBMatrix.h"
#include <hidapi/hidapi.h>

#define QMK_USAGE_PAGE 0xFF60
#define QMK_USAGE 0x61

/*-----------------------------------------------------*\
| Massdrop                                     |
\*-----------------------------------------------------*/
#define MASSDROP_VID      0x04D8
#define MASSDROP_ALT_PID  0xEED3
#define MASSDROP_CTRL_PID 0xEED2

void DetectQMKRGBMatrixControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);
    if( dev )
    {
        QMKRGBMatrixController* controller = new QMKRGBMatrixController(dev, info->path);
        RGBController_QMKRGBMatrix* rgb_controller = new RGBController_QMKRGBMatrix(controller);
        ResourceManager::get()->RegisterRGBController(rgb_controller);
    }
} 

REGISTER_HID_DETECTOR_IPU("Massdrop Alt", DetectQMKRGBMatrixControllers, MASSDROP_VID, MASSDROP_ALT_PID, 1, QMK_USAGE_PAGE, QMK_USAGE);
REGISTER_HID_DETECTOR_IPU("Massdrop Ctrl", DetectQMKRGBMatrixControllers, MASSDROP_VID, MASSDROP_CTRL_PID, 1, QMK_USAGE_PAGE, QMK_USAGE);
